/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MainFrameRecvProcCONNECT.cpp								 */
/* 内容			:サーバーメインフレーム(バージョン系受信処理) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include <time.h>
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoAccount.h"
#include "InfoAccount.h"
#include "InfoMapBase.h"
#include "LibInfoSystem.h"
#include "TextOutput.h"
#include "LibInfoMapBase.h"
#include "InfoCharSvr.h"
#include "LibInfoCharSvr.h"
#include "MgrData.h"
#include "MainFrame.h"


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCONNECT									 */
/* 内容		:受信処理(接続系)												 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcCONNECT(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_CONNECT_REQ_LOGIN:	RecvProcCONNECT_REQ_LOGIN	(pData, dwSessionID);	break;	/* ログイン要求 */
	case SBOCOMMANDID_SUB_CONNECT_REQ_PLAY:		RecvProcCONNECT_REQ_PLAY	(pData, dwSessionID);	break;	/* ゲーム開始要求 */
	case SBOCOMMANDID_SUB_CONNECT_KEEPALIVE:	RecvProcCONNECT_KEEPALIVE	(pData, dwSessionID);	break;	/* 生存確認通知 */
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCONNECT_REQ_LOGIN							 */
/* 内容		:受信処理(ログイン要求)											 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcCONNECT_REQ_LOGIN(PBYTE pData, DWORD dwSessionID)
{
	int nResult;
	BOOL bResult;
	PCInfoAccount pInfoAccount;
	CPacketCONNECT_REQ_LOGIN Packet;
	CPacketCONNECT_RES_LOGIN PacketRes;
	CPacketCHAR_MOTION PacketCHAR_MOTION;
	CmyString strTmp;

	Packet.Set (pData);

	nResult			= LOGINRES_NG_PASSWORD;
	pInfoAccount	= m_pLibInfoAccount->GetPtr (Packet.m_strAccount);

	strTmp.Format (
		"%02X-%02X-%02X-%02X-%02X-%02X",
		Packet.m_byMacAddr[0], Packet.m_byMacAddr[1], Packet.m_byMacAddr[2],
		Packet.m_byMacAddr[3], Packet.m_byMacAddr[4], Packet.m_byMacAddr[5]);

	/* 登録済み？ */
	if (pInfoAccount) {
		bResult = m_pLibInfoAccount->CheckPassword (Packet.m_strAccount, Packet.m_strPassword);
		if (bResult) {
			nResult = LOGINRES_OK;
		}

	/* 未登録 */
	} else {
		bResult = m_pLibInfoAccount->IsUseMacAddr ((LPCSTR)strTmp);
		if (bResult) {
			PacketRes.Make (LOGINRES_NG_MAC, 0);
			m_pSock->SendTo (dwSessionID, &PacketRes);
			return;
		}
		nResult = LOGINRES_OK;
		pInfoAccount = (PCInfoAccount)m_pLibInfoAccount->GetNew ();
		TrimViewString (pInfoAccount->m_strAccount,  Packet.m_strAccount);
		TrimViewString (pInfoAccount->m_strPassword, Packet.m_strPassword);
		pInfoAccount->m_strMacAddr = strTmp;

		/* 管理者権限アカウント？ */
		if (pInfoAccount->m_strAccount == m_pMgrData->GetAdminAccount ()) {
			pInfoAccount->m_nAdminLevel = ADMINLEVEL_ALL;
		}

		m_pLibInfoAccount->Add (pInfoAccount);
	}

	if (nResult == LOGINRES_OK) {
		/* 使用中？ */
		if (pInfoAccount->m_dwSessionID != 0) {
			nResult = LOGINRES_NG_LOGIN;
		} else {
			time_t timeTmp;

			pInfoAccount->m_dwLoginCount ++;
			pInfoAccount->m_dwSessionID = dwSessionID;
			time (&timeTmp);
			pInfoAccount->m_dwTimeLastLogin = (DWORD)timeTmp;

			m_pLog->Write ("ログイン dwSessionID:%u [%s]", dwSessionID, (LPCSTR)pInfoAccount->m_strAccount);
		}
		PacketCHAR_MOTION.Make (0, 0, m_pLibInfoMotion);
		m_pSock->SendTo (dwSessionID, &PacketCHAR_MOTION);
	}

	PacketRes.Make (nResult, pInfoAccount->m_dwAccountID);
	m_pSock->SendTo (dwSessionID, &PacketRes);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCONNECT_REQ_PLAY							 */
/* 内容		:受信処理(ゲーム開始要求)										 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

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

	pTmp	= NULL;
	nResult	= PLAYRES_NONE;
	time (&timeTmp);

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtr (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		goto Exit;
	}
	pInfoAccount = m_pLibInfoAccount->GetPtrSessionID (dwSessionID);
	if (pInfoAccount == NULL) {
		goto Exit;
	}
	bResult = FALSE;

	pInfoSystem		= (PCInfoSystem)m_pMgrData->GetLibInfoSystem ()->GetPtr ();
	pInitCharStatus	= pInfoSystem->m_pInitCharStatus;

	pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);
	if (pInfoMap == NULL) {
		m_pLog->Write ("所属マップ不明 dwSessionID:%u [%s] dwMapID:%d", dwSessionID, (LPCSTR)pInfoChar->m_strCharName, pInfoChar->m_dwMapID);
	}

	m_pLibInfoChar->LogIn (Packet.m_dwCharID, dwSessionID, pInfoAccount->m_dwAccountID);
	pInfoAccount->m_dwLastKeepalive = (DWORD)timeTmp;
	nOnlineCount = m_pLibInfoChar->GetCountOnline ();

	m_pLog->Write ("ゲーム開始 dwSessionID:%u [%s](Online:%d)", dwSessionID, (LPCSTR)pInfoChar->m_strCharName, nOnlineCount);

	for (i = 0; i < 4; i ++) {
		nTmp = i;
		bResult |= m_pLibInfoChar->IsMove (pInfoChar, nTmp);
	}
	if (bResult == FALSE) {
		m_pLog->Write ("埋まり救出 MAP:%d(%d,%d)", pInfoChar->m_dwMapID, pInfoChar->m_nMapX, pInfoChar->m_nMapY);
		/* 初期位置に転送 */
		pInfoChar->m_dwMapID	= pInitCharStatus->dwInitPosMapID;		/* マップID */
		pInfoChar->m_nMapX		= pInitCharStatus->ptInitPos.x;			/* X座標 */
		pInfoChar->m_nMapY		= pInitCharStatus->ptInitPos.y;			/* Y座標 */
		pInfoChar->m_bProcMoveMapOut = TRUE;
		pInfoMap = (PCInfoMapBase)m_pLibInfoMap->GetPtr (pInfoChar->m_dwMapID);
		if (pInfoMap == NULL) {
			m_pLog->Write ("所属マップ不明 dwSessionID:%u [%s] dwMapID:%d", dwSessionID, (LPCSTR)pInfoChar->m_strCharName, pInfoChar->m_dwMapID);
			goto Exit;
		}
	}

	nResult = PLAYRES_OK;
	/* 管理者アカウント？ */
	if (pInfoAccount->m_nAdminLevel == ADMINLEVEL_ALL) {
		nResult = PLAYRES_ADMINLEVEL_ALL;
	}
//Todo:
	pInfoChar->m_abyMark.RemoveAll ();
	if (pInfoChar->m_strCharName == "春うらら") {
		pInfoChar->m_abyMark.Add (2);
		nResult = PLAYRES_ADMINLEVEL_ALL;
	} else if (pInfoChar->m_strCharName == "VeLTiNA") {
		pInfoChar->m_abyMark.Add (3);
		nResult = PLAYRES_ADMINLEVEL_ALL;
	}
	pInfoChar->m_abyMark.Add (1);

	PacketITEM_ITEMTYPEINFO.Make (m_pLibInfoItemType);
	m_pSock->SendTo (dwSessionID, &PacketITEM_ITEMTYPEINFO);
	PacketITEM_ITEMINFO.Make (m_pLibInfoItem);
	m_pSock->SendTo (dwSessionID, &PacketITEM_ITEMINFO);
	PacketITEM_ITEMWEAPONINFO.Make (m_pLibInfoItemWeapon);
	m_pSock->SendTo (dwSessionID, &PacketITEM_ITEMWEAPONINFO);
	PacketMAP_MAPOBJECT.Make (m_pLibInfoMapObject);
	m_pSock->SendTo (dwSessionID, &PacketMAP_MAPOBJECT);
	PacketMAP_MAPPARTS.Make (m_pLibInfoMapParts);
	m_pSock->SendTo (dwSessionID, &PacketMAP_MAPPARTS);
	PacketMAP_MAPSHADOW.Make (m_pLibInfoMapShadow);
	m_pSock->SendTo (dwSessionID, &PacketMAP_MAPSHADOW);
	PacketRES_MAPINFO.Make (pInfoMap);
	m_pSock->SendTo (dwSessionID, &PacketRES_MAPINFO);
	PacketEFFECT_EFFECTINFO.Make (m_pLibInfoEffect);
	m_pSock->SendTo (dwSessionID, &PacketEFFECT_EFFECTINFO);
	PacketEFFECT_BALLOONINFO.Make (0, m_pLibInfoEfcBalloon);
	m_pSock->SendTo (dwSessionID, &PacketEFFECT_BALLOONINFO);
	PacketSKILL_SKILLINFO.Make (m_pLibInfoSkill);
	m_pSock->SendTo (dwSessionID, &PacketSKILL_SKILLINFO);

	/* 周りのキャラ情報を送信 */
	PacketCHAR_MOTION.Make (0, 0, m_pLibInfoMotion);
	m_pSock->SendTo (dwSessionID, &PacketCHAR_MOTION);
	PacketCHAR_MOTIONTYPE.Make (0, m_pLibInfoMotionType);
	m_pSock->SendTo (dwSessionID, &PacketCHAR_MOTIONTYPE);
	LibInfoCharTmp.Create (m_pMgrData);
	pTmp = m_pLibInfoChar->GetDataScreen (pInfoChar);
	LibInfoCharTmp.SetSendData (pTmp);
	PacketCHAR_CHARINFO.Make (&LibInfoCharTmp);
	m_pSock->SendTo (dwSessionID, &PacketCHAR_CHARINFO);

	/* ゲーム開始応答を通知 */
	PacketRES_PLAY.Make (nResult);
	m_pSock->SendTo (dwSessionID, &PacketRES_PLAY);

	/* 周りのキャラに通知 */
	PacketCHAR_RES_CHARINFO.Make (pInfoChar);
	SendToScreenChar (pInfoChar, &PacketCHAR_RES_CHARINFO);
	m_pSock->SendTo (dwSessionID, &PacketCHAR_RES_CHARINFO);

	UpdateServerInfo ();

	strTmp.Format ("SYSTEM:スクラップブックオンラインの世界へようこそ♪");
	PacketMAP_SYSTEMMSG.Make (strTmp);
	m_pSock->SendTo (dwSessionID, &PacketMAP_SYSTEMMSG);
	strTmp.Format ("SYSTEM:現在のオンライン数: %d", nOnlineCount);
	PacketMAP_SYSTEMMSG.Make (strTmp, 0, FALSE);
	m_pSock->SendTo (dwSessionID, &PacketMAP_SYSTEMMSG);
	strTmp.Format ("SYSTEM:最新クライアントバージョン: %s", m_pMgrData->GetClientVersion ());
	PacketMAP_SYSTEMMSG.Make (strTmp, 0, FALSE);
	m_pSock->SendTo (dwSessionID, &PacketMAP_SYSTEMMSG);

	if (nOnlineCount % 10 == 0) {
		strTmp.Format ("SYSTEM:オンライン数が %d になりました", nOnlineCount);
		PacketMAP_SYSTEMMSG.Make (strTmp);
		m_pSock->SendTo (0, &PacketMAP_SYSTEMMSG);
	}

	strTmp.Format ("SYSTEM:現在最も人が集まっている場所は");
	PacketMAP_SYSTEMMSG.Make (strTmp, 0, FALSE);
	m_pSock->SendTo (dwSessionID, &PacketMAP_SYSTEMMSG);
	dwTmp = m_pLibInfoChar->GetPlaceName (strTmp2);
	if (strTmp2.IsEmpty ()) {
		strTmp.Format ("SYSTEM:マップ番号[%d]のようです", dwTmp);
	} else {
		strTmp.Format ("SYSTEM:[%s]のようです", (LPCSTR)strTmp2);
	}
	PacketMAP_SYSTEMMSG.Make (strTmp, 0, FALSE);
	m_pSock->SendTo (dwSessionID, &PacketMAP_SYSTEMMSG);

Exit:
	SAFE_DELETE_ARRAY (pTmp);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCONNECT_KEEPALIVE							 */
/* 内容		:受信処理(生存確認通知)											 */
/* 日付		:2008/06/21														 */
/* ========================================================================= */

void CMainFrame::RecvProcCONNECT_KEEPALIVE(PBYTE pData, DWORD dwSessionID)
{
	time_t timeTmp;
	PCInfoAccount pInfoAccount;

	pInfoAccount = m_pLibInfoAccount->GetPtrSessionID (dwSessionID);
	if (pInfoAccount == NULL) {
		goto Exit;
	}

	time (&timeTmp);
	pInfoAccount->m_dwLastKeepalive = (DWORD)timeTmp;
Exit:
	return;
}

/* Copyright(C)URARA-works 2006 */
