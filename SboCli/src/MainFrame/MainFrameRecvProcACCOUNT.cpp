/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MainFrameRecvProcACCOUNT.cpp								 */
/* 内容			:クライアントメインフレーム(アカウント系受信処理) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/08													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoAccount.h"
#include "InfoAccount.h"
#include "MgrData.h"
#include "MainFrame.h"


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcACCOUNT									 */
/* 内容		:受信処理(アカウント系)											 */
/* 日付		:2006/11/08														 */
/* ========================================================================= */

void CMainFrame::RecvProcACCOUNT(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_ACCOUNT_RES_ACCOUNTINFO:	RecvProcACCOUNT_RES_ACCOUNTINFO (pData);	break;	/* アカウント情報応答 */
	case SBOCOMMANDID_SUB_ACCOUNT_RES_MAKECHAR:		RecvProcACCOUNT_RES_MAKECHAR (pData);		break;	/* キャラ作成応答 */
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcACCOUNT_RES_ACCOUNTINFO					 */
/* 内容		:受信処理(アカウント情報応答)									 */
/* 日付		:2006/11/08														 */
/* ========================================================================= */

void CMainFrame::RecvProcACCOUNT_RES_ACCOUNTINFO(PBYTE pData)
{
	CPacketACCOUNT_RES_ACCOUNTINFO Packet;

	Packet.Set (pData);

	m_pMgrData->SetAccount (Packet.m_pInfo);
	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWACCOUNTINFO, 0);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcACCOUNT_RES_MAKECHAR						 */
/* 内容		:受信処理(キャラ作成応答)										 */
/* 日付		:2006/12/29														 */
/* ========================================================================= */

void CMainFrame::RecvProcACCOUNT_RES_MAKECHAR(PBYTE pData)
{
	CPacketACCOUNT_RES_MAKECHAR Packet;
	PCInfoAccount pAccountInfo;

	Packet.Set (pData);

	switch (Packet.m_nResult) {
	case MAKECHARRES_OK:			/* 問題無し */
		pAccountInfo = m_pMgrData->GetAccount ();
		pAccountInfo->m_adwCharID.Add (Packet.m_dwCharID);
		PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWACCOUNTINFO, 0);
		break;
	case MAKECHARRES_NG_USE:		/* 使用済み */
	case MAKECHARRES_NG_SPACE:		/* 空白が使用されている */
		break;
	}
	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RES_MAKECHAR, Packet.m_nResult);
}

/* Copyright(C)URARA-works 2006 */
