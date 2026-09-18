/// @file MainFrameRecvProcCONNECT.cpp
/// @brief クライアントメインフレーム(バージョン系受信処理) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/05
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoAccount.h"
#include "InfoAccount.h"
#include "Window/ILoginWindow.h"
#include "WindowLOGIN.h"
#include "MgrData.h"
#include "MgrWindow.h"
#include "Platform/SDLEventUtil.h"
#include "MainFrame.h"

#if defined(__EMSCRIPTEN__)
#include <emscripten/em_js.h>

// ブラウザ版: 端末トークンが無効と返された。ログインコード方式(docs/login-code-auth-plan.md)の
// 端末情報を localStorage から消し、再入力してもらうためアカウントページへ移動する。
EM_JS(void, SBOP2_HandleTokenExpired, (), {
	try {
		window.localStorage.removeItem('sbop2_device_token');
		window.localStorage.removeItem('sbop2_device_account');
	} catch (e) {
		/* ignore */
	}
	window.location.href = '/account/?reason=expired';
});
#endif

void CMainFrame::RecvProcCONNECT(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_CONNECT_RES_LOGIN: RecvProcCONNECT_RES_LOGIN(pData); break; // ログイン応答
	case SBOCOMMANDID_SUB_CONNECT_RES_PLAY: RecvProcCONNECT_RES_PLAY(pData); break; // ゲーム開始応答
	case SBOCOMMANDID_SUB_CONNECT_KEEPALIVE: RecvProcCONNECT_KEEPALIVE(pData); break; // 生存確認応答
	}
}


void CMainFrame::RecvProcCONNECT_RES_LOGIN(PBYTE pData)
{
	CPacketCONNECT_RES_LOGIN Packet;
	CPacketACCOUNT_REQ_ACCOUNTINFO PacketREQ_ACCOUNTINFO;
	ILoginWindow *pWindow;

	Packet.Set(pData);

	switch (Packet.m_nResult) {
	case LOGINRES_OK: // 問題無し
		// 入力内容とチェック状態を保存
		pWindow = m_pMgrWindow->GetLoginWindow();
		if (pWindow) {
			pWindow->Save();
		}

//Todo:お知らせ要求
		PostMainFrameMessage(MAINFRAMEMSG_CHGSTATE, GAMESTATE_LOGINMENU);
		// アカウント情報を要求
		PacketREQ_ACCOUNTINFO.Make(Packet.m_dwAccountID);
		m_pSock->Send(&PacketREQ_ACCOUNTINFO);
		break;

	case LOGINRES_NG_PASSWORD: // パスワード不一致
		DisConnectProc(DISCONNECTID_PASSWORD);
		break;

	case LOGINRES_NG_LOGIN: // ログイン済み
		DisConnectProc(DISCONNECTID_LOGIN);
		break;

	case LOGINRES_NG_MAC: // 作成済み
		DisConnectProc(DISCONNECTID_MAC);
		break;

	case LOGINRES_NG_DISABLE: // ログイン拒否
		m_pMgrData->SetDisableLogin(TRUE);
		m_pMgrData->SaveIniData();
		PushSDLQuitEvent();
		break;

	case LOGINRES_NG_TOKEN: // 端末トークン無効
#if defined(__EMSCRIPTEN__)
		SBOP2_HandleTokenExpired();
#else
		DisConnectProc(DISCONNECTID_LOGIN);
#endif
		break;
	}
}


void CMainFrame::RecvProcCONNECT_RES_PLAY(PBYTE pData)
{
	CPacketCONNECT_RES_PLAY Packet;

	Packet.Set(pData);

	switch (Packet.m_nResult) {
	case PLAYRES_ADMINLEVEL_ALL: // 管理者(全権限)
		m_pMgrData->SetAdminLevel(ADMINLEVEL_ALL);
		break;
	}

	ChgGameState(GAMESTATE_MAP);
}


void CMainFrame::RecvProcCONNECT_KEEPALIVE(PBYTE pData)
{
	CPacketCONNECT_KEEPALIVE Packet;

	Packet.Set(pData);
	m_pMgrData->SetPing(timeGetTime() - Packet.m_dwData);
}
