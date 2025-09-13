/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MainFrameRecvProcCONNECT.cpp								 */
/* 内容			:クライアントメインフレーム(バージョン系受信処理) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoAccount.h"
#include "InfoAccount.h"
#include "WindowLOGIN.h"
#include "MgrData.h"
#include "MgrWindow.h"
#include "MainFrame.h"


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCONNECT									 */
/* 内容		:受信処理(接続系)												 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcCONNECT(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_CONNECT_RES_LOGIN:	RecvProcCONNECT_RES_LOGIN	(pData);	break;	/* ログイン応答 */
	case SBOCOMMANDID_SUB_CONNECT_RES_PLAY:		RecvProcCONNECT_RES_PLAY	(pData);	break;	/* ゲーム開始応答 */
	case SBOCOMMANDID_SUB_CONNECT_KEEPALIVE:	RecvProcCONNECT_KEEPALIVE	(pData);	break;	/* 生存確認応答 */
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCONNECT_RES_LOGIN							 */
/* 内容		:受信処理(ログイン応答)											 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcCONNECT_RES_LOGIN(PBYTE pData)
{
	CPacketCONNECT_RES_LOGIN Packet;
	CPacketACCOUNT_REQ_ACCOUNTINFO PacketREQ_ACCOUNTINFO;
	PCWindowLOGIN pWindow;

	Packet.Set (pData);

	switch (Packet.m_nResult) {
	case LOGINRES_OK:			/* 問題無し */
		/* 入力内容とチェック状態を保存 */
		pWindow = (PCWindowLOGIN)m_pMgrWindow->GetWindow (WINDOWTYPE_LOGIN);
		pWindow->Save ();

//Todo:お知らせ要求
		PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_CHGSTATE, GAMESTATE_LOGINMENU);
		/* アカウント情報を要求 */
		PacketREQ_ACCOUNTINFO.Make (Packet.m_dwAccountID);
		m_pSock->Send (&PacketREQ_ACCOUNTINFO);
		break;

	case LOGINRES_NG_PASSWORD:	/* パスワード不一致 */
		DisConnectProc (DISCONNECTID_PASSWORD);
		break;

	case LOGINRES_NG_LOGIN:		/* ログイン済み */
		DisConnectProc (DISCONNECTID_LOGIN);
		break;

	case LOGINRES_NG_MAC:		/* 作成済み */
		DisConnectProc (DISCONNECTID_MAC);
		break;

	case LOGINRES_NG_DISABLE:	/* ログイン拒否 */
		m_pMgrData->SetDisableLogin(TRUE);
		m_pMgrData->SaveIniData();
		PostMessage (m_hWnd, WM_CLOSE, 0, 0);
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCONNECT_RES_PLAY							 */
/* 内容		:受信処理(ゲーム開始応答)										 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

void CMainFrame::RecvProcCONNECT_RES_PLAY(PBYTE pData)
{
	CPacketCONNECT_RES_PLAY Packet;

	Packet.Set (pData);

	switch (Packet.m_nResult) {
	case PLAYRES_ADMINLEVEL_ALL:	/* 管理者(全権限) */
		m_pMgrData->SetAdminLevel (ADMINLEVEL_ALL);
		break;
	}

	ChgGameState (GAMESTATE_MAP);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcCONNECT_KEEPALIVE							 */
/* 内容		:受信処理(生存確認応答)											 */
/* 日付		:2009/04/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcCONNECT_KEEPALIVE(PBYTE pData)
{
	CPacketCONNECT_KEEPALIVE Packet;

	Packet.Set (pData);
	m_pMgrData->SetPing (timeGetTime () - Packet.m_dwData);
}

/* Copyright(C)URARA-works 2006 */
