/// @file MainFrameRecvProcCONNECT.cpp
/// @brief サーバーメインフレーム(バージョン系受信処理) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/05
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include <time.h>
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "InfoCharSvr.h"
#include "LibInfoAccount.h"
#include "LibInfoDisable.h"
#include "LibInfoSystem.h"
#include "LibInfoMapBase.h"
#include "LibInfoCharSvr.h"
#include "TextOutput.h"
#include "MgrData.h"
#include "MainFrame.h"
#include "PasswordHash.h"
#include "../Web/ProxyIpRegistry.h"
#include "Account/LoginCode.h"
#include "Account/AccountAuthStore.h"

void CMainFrame::RecvProcCONNECT(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_CONNECT_REQ_LOGIN:	RecvProcCONNECT_REQ_LOGIN(pData, dwSessionID);	break;	// ログイン要求
	case SBOCOMMANDID_SUB_CONNECT_REQ_LOGIN_TOKEN:	RecvProcCONNECT_REQ_LOGIN_TOKEN(pData, dwSessionID);	break;	// 端末トークンログイン要求
	case SBOCOMMANDID_SUB_CONNECT_REQ_PLAY:	RecvProcCONNECT_REQ_PLAY(pData, dwSessionID);	break;	// ゲーム開始要求
	case SBOCOMMANDID_SUB_CONNECT_KEEPALIVE:	RecvProcCONNECT_KEEPALIVE(pData, dwSessionID);	break;	// 生存確認通知
	}
}

/// @brief アカウントが確定した後のログイン共通処理。
/// @details 既存の RecvProcCONNECT_REQ_LOGIN から、アカウント特定(新規作成含む)より
///          後ろの処理(拒否判定・ログイン済み判定・成功時の更新・CHAR_MOTION/RES_LOGIN送信)
///          を切り出したもの。順序・副作用(拒否時のIP追加、ログ内容)は元のコードのまま。
void CMainFrame::CompleteLogin(
	DWORD dwSessionID,			// [in] セッションID
	CInfoAccount *pInfoAccount,	// [in] 確定済みのアカウント(非NULL)
	int nResult,				// [in] ここまでの結果(通常はLOGINRES_OK)
	BOOL bDisable,				// [in] MAC/IPでの拒否判定結果
	DWORD dwAddr,				// [in] IPアドレス(ネットワークバイトオーダー)
	unsigned int nAddrHost,		// [in] ↑をホストバイトオーダーへ直したもの
	LPCSTR pszMacAddr)			// [in] ログ表示用のMACアドレス文字列
{
	PCInfoAccount pInfo = (PCInfoAccount)pInfoAccount;
	CPacketCONNECT_RES_LOGIN PacketRes;
	CPacketCHAR_MOTION PacketCHAR_MOTION;

	if (nResult == LOGINRES_OK) {
		// 拒否？
		if (bDisable || pInfo->m_bDisable) {
			nResult = LOGINRES_NG_DISABLE;
			m_pLog->Write("ログイン拒否 dwSessionID:%u [%d.%d.%d.%d][%s][%s]",
					dwSessionID,
					(nAddrHost >> 24) & 0xFF, (nAddrHost >> 16) & 0xFF, (nAddrHost >> 8) & 0xFF, nAddrHost & 0xFF,
					pszMacAddr,
					pInfo->m_strAccount.GetUtf8Pointer());
			// IPアドレスで拒否しておく
			m_pLibInfoDisable->AddIP(dwAddr);
		// 使用中？
		} else if (pInfo->m_dwSessionID != 0) {
			nResult = LOGINRES_NG_LOGIN;
		} else {
			time_t timeTmp;

			pInfo->m_dwLoginCount ++;
			pInfo->m_dwSessionID = dwSessionID;
			time(&timeTmp);
			pInfo->m_dwTimeLastLogin = (DWORD)timeTmp;
			pInfo->m_strLastMacAddr	= pszMacAddr;
			// m_dwIPはホストバイトオーダーで保持する(ServerSessionsHandler/
			// CharacterItemHandlerの表示ロジックがホストバイトオーダー前提のため)
			pInfo->m_dwIP = nAddrHost;

			m_pLog->Write("ログイン dwSessionID:%u [%d.%d.%d.%d][%s][%s]",
					dwSessionID,
					(nAddrHost >> 24) & 0xFF, (nAddrHost >> 16) & 0xFF, (nAddrHost >> 8) & 0xFF, nAddrHost & 0xFF,
					pszMacAddr,
					pInfo->m_strAccount.GetUtf8Pointer());
		}
		PacketCHAR_MOTION.Make(0, 0, m_pLibInfoMotion);
		m_pSock->SendTo(dwSessionID, &PacketCHAR_MOTION);
	}

	PacketRes.Make(nResult, pInfo->m_dwAccountID);
	m_pSock->SendTo(dwSessionID, &PacketRes);
}

void CMainFrame::RecvProcCONNECT_REQ_LOGIN(PBYTE pData, DWORD dwSessionID)
{
	int nResult;
	BOOL bResult, bDisable;
	PCInfoAccount pInfoAccount;
	CPacketCONNECT_REQ_LOGIN Packet;
	CPacketCONNECT_RES_LOGIN PacketRes;
	CPacketCHAR_MOTION PacketCHAR_MOTION;
	CmyString strTmp, strLog;
	CString strClientVer;
	DWORD dwAddr;			// IPアドレス(ネットワークバイトオーダー)
	unsigned int nAddrHost;	// ↑をホストバイトオーダーへ直したもの(オクテット取り出し用)

	Packet.Set(pData);

	nResult	= LOGINRES_NG_PASSWORD;
	pInfoAccount	= m_pLibInfoAccount->GetPtr(Packet.m_strAccount);
	// IN_ADDR.S_un はWindows固有のメンバ名のため使わず、生の DWORD で扱う
	dwAddr = m_pSock->GetIPAddress(dwSessionID);

	// ブラウザ版はWebSocketBridgeが同一プロセス内で127.0.0.1として繋ぎ直すため、
	// ここで見えるIPは常にloopbackになってしまう。loopbackの場合に限り、
	// ブリッジが登録した「接続元ポート→実IP」対応表を引いて実IPに差し替える。
	// (対応表はブリッジ側でgetsockname()したローカルポートをキーにしており、
	//  それがそのままこちら側から見た相手ポートになる)
	if (ProxyIpRegistry::IsLoopbackIPv4(dwAddr)) {
		DWORD dwPeerPort = m_pSock->GetPeerPort(dwSessionID);
		unsigned long dwRealIp = 0;
		if ((dwPeerPort != 0) &&
		    ProxyIpRegistry::Lookup(static_cast<unsigned short>(dwPeerPort), dwRealIp)) {
			dwAddr = dwRealIp;
		}
	}

	nAddrHost = ntohl(dwAddr);

	strTmp.Format(
		"%02X-%02X-%02X-%02X-%02X-%02X",
		Packet.m_byMacAddr[0], Packet.m_byMacAddr[1], Packet.m_byMacAddr[2],
		Packet.m_byMacAddr[3], Packet.m_byMacAddr[4], Packet.m_byMacAddr[5]);
	// 拒否されているか判定
	bDisable = m_pLibInfoDisable->IsDisable((LPCSTR)strTmp);
	if (strTmp == "00-00-00-00-00-00") {
//		bDisable = TRUE;
	}
	// IPアドレスで拒否しているか判定
	bDisable |= m_pLibInfoDisable->IsDisableIP(dwAddr);

	// 登録済み？
	if (pInfoAccount) {
		bResult = PasswordHash::Verify(pInfoAccount->m_strPassword.GetUtf8Pointer(), Packet.m_strPassword.GetUtf8Pointer());
		if (bResult) {
			nResult = LOGINRES_OK;
		}

	// 未登録
	} else {
		// ログインコード方式(docs/login-code-auth-plan.md S4)への移行に伴い、知らない名前での
		// アカウント自動作成は廃止する(方針は LoginCode::ShouldAutoCreateAccountOnUnknownLogin()
		// に切り出してあり、常に false。TestLoginCode.cpp でこの前提を検出する)。アカウントの
		// 有無を区別させないため、登録済みでパスワード不一致の場合と同じ LOGINRES_NG_PASSWORD を
		// 返す(旧「管理者名と一致したらADMINLEVEL_ALLを付与」処理もここで消える)。
		if (!LoginCode::ShouldAutoCreateAccountOnUnknownLogin()) {
			nResult = LOGINRES_NG_PASSWORD;
			PacketRes.Make(nResult, 0);
			m_pSock->SendTo(dwSessionID, &PacketRes);
			return;
		}
	}

	CompleteLogin(dwSessionID, (CInfoAccount *)pInfoAccount, nResult, bDisable, dwAddr, nAddrHost, strTmp.GetUtf8Pointer());
}

void CMainFrame::RecvProcCONNECT_REQ_LOGIN_TOKEN(PBYTE pData, DWORD dwSessionID)
{
	int nResult;
	BOOL bDisable;
	PCInfoAccount pInfoAccount;
	CPacketCONNECT_REQ_LOGIN_TOKEN Packet;
	CPacketCONNECT_RES_LOGIN PacketRes;
	CmyString strTmp;
	DWORD dwAddr;			// IPアドレス(ネットワークバイトオーダー)
	unsigned int nAddrHost;	// ↑をホストバイトオーダーへ直したもの(オクテット取り出し用)
	unsigned int dwAccountID;
	std::string strTokenUtf8;

	Packet.Set(pData);

	nResult = LOGINRES_NG_TOKEN;
	pInfoAccount = NULL;

	// トークンをログに出さない。形式(16進64文字)を先に検証し、
	// 不正な入力ではDBを引かない。
	strTokenUtf8 = (LPCSTR)Packet.m_strDeviceToken.GetUtf8Pointer();
	if ((strTokenUtf8.size() == LoginCode::kDeviceTokenBytes * 2) &&
	    (strTokenUtf8.find_first_not_of("0123456789abcdefABCDEF") == std::string::npos)) {
		CAccountAuthStore Store;
		std::string strTokenHash = LoginCode::HashDeviceToken(strTokenUtf8);
		dwAccountID = 0;
		if (Store.TouchDevice(strTokenHash, (long)time(NULL), dwAccountID)) {
			pInfoAccount = m_pLibInfoAccount->GetPtr((DWORD)dwAccountID);
			if (pInfoAccount != NULL) {
				nResult = LOGINRES_OK;
			}
		}
	}

	if (pInfoAccount == NULL) {
		// アカウントが見つからない場合はここで打ち切る(REQ_LOGINと異なり自動作成はしない)
		m_pLog->Write("端末トークンログイン失敗 dwSessionID:%u", dwSessionID);
		PacketRes.Make(LOGINRES_NG_TOKEN, 0);
		m_pSock->SendTo(dwSessionID, &PacketRes);
		return;
	}

	// IN_ADDR.S_un はWindows固有のメンバ名のため使わず、生の DWORD で扱う
	dwAddr = m_pSock->GetIPAddress(dwSessionID);

	// ブラウザ版はWebSocketBridgeが同一プロセス内で127.0.0.1として繋ぎ直すため、
	// ここで見えるIPは常にloopbackになってしまう。loopbackの場合に限り、
	// ブリッジが登録した「接続元ポート→実IP」対応表を引いて実IPに差し替える。
	if (ProxyIpRegistry::IsLoopbackIPv4(dwAddr)) {
		DWORD dwPeerPort = m_pSock->GetPeerPort(dwSessionID);
		unsigned long dwRealIp = 0;
		if ((dwPeerPort != 0) &&
		    ProxyIpRegistry::Lookup(static_cast<unsigned short>(dwPeerPort), dwRealIp)) {
			dwAddr = dwRealIp;
		}
	}

	nAddrHost = ntohl(dwAddr);

	// MACアドレスは既存のREQ_LOGINと同じくダミー扱い(トークンログインでは意味を持たない)
	strTmp.Format(
		"%02X-%02X-%02X-%02X-%02X-%02X",
		Packet.m_byMacAddr[0], Packet.m_byMacAddr[1], Packet.m_byMacAddr[2],
		Packet.m_byMacAddr[3], Packet.m_byMacAddr[4], Packet.m_byMacAddr[5]);
	// 拒否されているか判定(REQ_LOGINと同じ判定)
	bDisable = m_pLibInfoDisable->IsDisable((LPCSTR)strTmp);
	bDisable |= m_pLibInfoDisable->IsDisableIP(dwAddr);

	CompleteLogin(dwSessionID, (CInfoAccount *)pInfoAccount, nResult, bDisable, dwAddr, nAddrHost, strTmp.GetUtf8Pointer());
}

void CMainFrame::RecvProcCONNECT_REQ_PLAY(PBYTE pData, DWORD dwSessionID)
{
	BOOL bResult;
	int i, nTmp, nResult, nOnlineCount;
	time_t timeTmp;
	DWORD dwTmp;
	PBYTE pTmp;
	PCInfoCharSvr pInfoChar;
	PCInfoAccount pInfoAccount;
	PCInfoMapBase pInfoMap;
	PCInfoSystem pInfoSystem;
	PSTSYSTEM_INITCHARSTATUS pInitCharStatus;
	CPacketCONNECT_REQ_PLAY Packet;
	CPacketCONNECT_RES_PLAY PacketRES_PLAY;
	CPacketCHAR_CHARINFO PacketCHAR_CHARINFO;
	CPacketCHAR_RES_CHARINFO PacketCHAR_RES_CHARINFO;
	CPacketCHAR_MOTION PacketCHAR_MOTION;
	CPacketCHAR_MOTIONTYPE PacketCHAR_MOTIONTYPE;
	CPacketMAP_ONLINE PacketMAP_ONLINE;
	CPacketMAP_SYSTEMMSG PacketMAP_SYSTEMMSG;
	CPacketMAP_MAPOBJECT PacketMAP_MAPOBJECT;
	CPacketMAP_MAPPARTS PacketMAP_MAPPARTS;
	CPacketMAP_MAPSHADOW PacketMAP_MAPSHADOW;
	CPacketMAP_RES_MAPINFO PacketRES_MAPINFO;
	CPacketITEM_ITEMTYPEINFO PacketITEM_ITEMTYPEINFO;
	CPacketITEM_ITEMINFO PacketITEM_ITEMINFO;
	CPacketITEM_ITEMWEAPONINFO PacketITEM_ITEMWEAPONINFO;
	CPacketEFFECT_EFFECTINFO PacketEFFECT_EFFECTINFO;
	CPacketEFFECT_BALLOONINFO PacketEFFECT_BALLOONINFO;
	CPacketSKILL_SKILLINFO PacketSKILL_SKILLINFO;
	CLibInfoCharSvr LibInfoCharTmp;
        CmyString strTmp, strTmp2;
        CString strClientVer;

	pTmp	= NULL;
	nResult	= PLAYRES_NONE;
	time(&timeTmp);

	Packet.Set(pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtr(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		goto Exit;
	}
	pInfoAccount = m_pLibInfoAccount->GetPtrSessionID(dwSessionID);
	if (pInfoAccount == NULL) {
		goto Exit;
	}

	// なりすまし対策: 本人アカウントが所有するキャラかを確認
	bResult = FALSE;
	for (i = 0; i < (int)pInfoAccount->m_adwCharID.size(); i++) {
		if (pInfoAccount->m_adwCharID[i] == Packet.m_dwCharID) {
			bResult = TRUE;
			break;
		}
	}
	if (bResult == FALSE) {
		m_pLog->Write("■ 他アカウントのキャラ指定(PLAY) dwSessionID:%u dwCharID:%u", dwSessionID, Packet.m_dwCharID);
		goto Exit;
	}

	bResult = FALSE;

	pInfoSystem	= (PCInfoSystem)m_pMgrData->GetLibInfoSystem()->GetPtr();
	pInitCharStatus	= pInfoSystem->m_pInitCharStatus;

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr(pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		m_pLog->Write("所属マップ不明 dwSessionID:%u [%s] dwMapID:%d", dwSessionID, pInfoChar->m_strCharName.GetUtf8Pointer(), pInfoChar->m_dwMapID);
	}

	m_pLibInfoChar->LogIn(Packet.m_dwCharID, dwSessionID, pInfoAccount->m_dwAccountID);
	pInfoAccount->m_dwLastKeepalive = (DWORD)timeTmp;
	nOnlineCount = m_pLibInfoChar->GetCountOnline();

	m_pLog->Write("ゲーム開始 dwSessionID:%u [%s](Online:%d)", dwSessionID, pInfoChar->m_strCharName.GetUtf8Pointer(), nOnlineCount);

	for (i = 0; i < 4; i ++) {
		nTmp = i;
		bResult |= m_pLibInfoChar->IsMove(pInfoChar, nTmp);
	}
	if (bResult == FALSE) {
		m_pLog->Write("埋まり救出 MAP:%d(%d,%d)", pInfoChar->m_dwMapID, pInfoChar->m_nMapX, pInfoChar->m_nMapY);

		if (pInfoChar->m_dwMaxHP == 0) {
			m_pLibInfoChar->SetInitStatus(pInfoChar, TRUE);
		}

		// 初期位置に転送（ptInitPos はピクセル座標・足元基準）
		pInfoChar->m_dwMapID	= pInitCharStatus->dwInitPosMapID;	// マップID
		pInfoChar->m_nMapX	= pInitCharStatus->ptInitPos.x;	// X座標
		pInfoChar->m_nMapY	= pInitCharStatus->ptInitPos.y;	// Y座標
		pInfoChar->m_bProcMoveMapOut = TRUE;
		pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr(pInfoChar->m_dwMapID);
		if (pInfoMap == NULL) {
			m_pLog->Write("所属マップ不明 dwSessionID:%u [%s] dwMapID:%d", dwSessionID, pInfoChar->m_strCharName.GetUtf8Pointer(), pInfoChar->m_dwMapID);
			goto Exit;
		}
	}

	nResult = PLAYRES_OK;
	// 管理者アカウント？
	if (pInfoAccount->m_nAdminLevel == ADMINLEVEL_ALL) {
		nResult = PLAYRES_ADMINLEVEL_ALL;
	}
//Todo:
	pInfoChar->m_abyMark.clear();
	if (pInfoChar->m_strCharName == "春うらら") {
		pInfoChar->m_abyMark.push_back(2);
		nResult = PLAYRES_ADMINLEVEL_ALL;
	} else if (pInfoChar->m_strCharName == "VeLTiNA") {
		pInfoChar->m_abyMark.push_back(3);
		nResult = PLAYRES_ADMINLEVEL_ALL;
	}
	pInfoChar->m_abyMark.push_back(1);

	PacketITEM_ITEMTYPEINFO.Make(m_pLibInfoItemType);
	m_pSock->SendTo(dwSessionID, &PacketITEM_ITEMTYPEINFO);
	PacketITEM_ITEMINFO.Make(m_pLibInfoItem);
	m_pSock->SendTo(dwSessionID, &PacketITEM_ITEMINFO);
	PacketITEM_ITEMWEAPONINFO.Make(m_pLibInfoItemWeapon);
	m_pSock->SendTo(dwSessionID, &PacketITEM_ITEMWEAPONINFO);
	PacketMAP_MAPOBJECT.Make(m_pLibInfoMapObject);
	m_pSock->SendTo(dwSessionID, &PacketMAP_MAPOBJECT);
	PacketMAP_MAPPARTS.Make(m_pLibInfoMapParts);
	m_pSock->SendTo(dwSessionID, &PacketMAP_MAPPARTS);
	PacketMAP_MAPSHADOW.Make(m_pLibInfoMapShadow);
	m_pSock->SendTo(dwSessionID, &PacketMAP_MAPSHADOW);
	PacketRES_MAPINFO.Make(pInfoMap);
	m_pSock->SendTo(dwSessionID, &PacketRES_MAPINFO);
	PacketEFFECT_EFFECTINFO.Make(m_pLibInfoEffect);
	m_pSock->SendTo(dwSessionID, &PacketEFFECT_EFFECTINFO);
	PacketEFFECT_BALLOONINFO.Make(0, m_pLibInfoEfcBalloon);
	m_pSock->SendTo(dwSessionID, &PacketEFFECT_BALLOONINFO);
	PacketSKILL_SKILLINFO.Make(m_pLibInfoSkill);
	m_pSock->SendTo(dwSessionID, &PacketSKILL_SKILLINFO);

	// 周りのキャラ情報を送信
	PacketCHAR_MOTION.Make(0, 0, m_pLibInfoMotion);
	m_pSock->SendTo(dwSessionID, &PacketCHAR_MOTION);
	PacketCHAR_MOTIONTYPE.Make(0, m_pLibInfoMotionType);
	m_pSock->SendTo(dwSessionID, &PacketCHAR_MOTIONTYPE);
	LibInfoCharTmp.Create(m_pMgrData);
	pTmp = m_pLibInfoChar->GetDataScreen(pInfoChar);
	LibInfoCharTmp.SetSendData(pTmp);
	PacketCHAR_CHARINFO.Make(&LibInfoCharTmp);
	m_pSock->SendTo(dwSessionID, &PacketCHAR_CHARINFO);

	// ゲーム開始応答を通知
	PacketRES_PLAY.Make(nResult);
	m_pSock->SendTo(dwSessionID, &PacketRES_PLAY);

	// 周りのキャラに通知
	PacketCHAR_RES_CHARINFO.Make(pInfoChar);
	SendToScreenChar(pInfoChar, &PacketCHAR_RES_CHARINFO);
	m_pSock->SendTo(dwSessionID, &PacketCHAR_RES_CHARINFO);

	NotifyOnlineCount();

	strTmp.Format(_T("SYSTEM:スクラップブックオンラインの世界へようこそ♪"));
	PacketMAP_SYSTEMMSG.Make(strTmp);
	m_pSock->SendTo(dwSessionID, &PacketMAP_SYSTEMMSG);
	strTmp.Format(_T("SYSTEM:現在のオンライン数: %d"), nOnlineCount);
	PacketMAP_SYSTEMMSG.Make(strTmp, 0, FALSE);
	m_pSock->SendTo(dwSessionID, &PacketMAP_SYSTEMMSG);
        strClientVer = Utf8ToTString(m_pMgrData->GetClientVersion());
        strTmp.Format(_T("SYSTEM:最新クライアントバージョン: %s"), (LPCTSTR)strClientVer);
	PacketMAP_SYSTEMMSG.Make(strTmp, 0, FALSE);
	m_pSock->SendTo(dwSessionID, &PacketMAP_SYSTEMMSG);

	if (nOnlineCount % 10 == 0) {
		strTmp.Format(_T("SYSTEM:オンライン数が %d になりました"), nOnlineCount);
		PacketMAP_SYSTEMMSG.Make(strTmp);
		m_pSock->SendTo(0, &PacketMAP_SYSTEMMSG);
	}

	strTmp.Format(_T("SYSTEM:現在最も人が集まっている場所は"));
	PacketMAP_SYSTEMMSG.Make(strTmp, 0, FALSE);
	m_pSock->SendTo(dwSessionID, &PacketMAP_SYSTEMMSG);
	dwTmp = m_pLibInfoChar->GetPlaceName(strTmp2);
	if (strTmp2.IsEmpty()) {
		strTmp.Format(_T("SYSTEM:マップ番号[%d]のようです"), dwTmp);
	} else {
		strTmp.Format(_T("SYSTEM:[%s]のようです"), (LPCTSTR)strTmp2);
	}
	PacketMAP_SYSTEMMSG.Make(strTmp, 0, FALSE);
	m_pSock->SendTo(dwSessionID, &PacketMAP_SYSTEMMSG);

	// MoveMapIn を直接呼んで NPC 情報送信とマップイベント初期化を確実に実行する。
	// ProcLocalFlgCheck の else if 連鎖では m_bProcMoveMapIn に到達する前に
	// 他のフラグが優先されて走らないことがあるため、フラグ経由ではなく直接呼ぶ。
	m_pLibInfoChar->MoveMapIn(pInfoChar);

Exit:
	SAFE_DELETE_ARRAY(pTmp);
}

void CMainFrame::RecvProcCONNECT_KEEPALIVE(PBYTE pData, DWORD dwSessionID)
{
	time_t timeTmp;
	PCInfoAccount pInfoAccount;
	CPacketCONNECT_KEEPALIVE Packet;

	Packet.Set(pData);
	pInfoAccount = m_pLibInfoAccount->GetPtrSessionID(dwSessionID);
	if (pInfoAccount == NULL) {
		goto Exit;
	}

	time(&timeTmp);
	pInfoAccount->m_dwLastKeepalive = (DWORD)timeTmp;

	Packet.Make(Packet.m_dwData);
	m_pSock->SendTo(dwSessionID, &Packet);
Exit:
	return;
}
