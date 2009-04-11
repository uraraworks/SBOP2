/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MainFrameRecvProcADMIN.cpp									 */
/* 内容			:クライアントメインフレーム(管理者系受信処理) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/06													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoCharCli.h"
#include "LibInfoDisable.h"
#include "InfoAccount.h"
#include "MgrData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "MainFrame.h"


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN										 */
/* 内容		:受信処理(管理者系)												 */
/* 日付		:2007/07/06														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_ADMIN_RES_ADMINLEVEL:		RecvProcADMIN_RES_ADMINLEVEL	(pData);	break;	/* 管理者レベル応答 */
	case SBOCOMMANDID_SUB_ADMIN_RENEWADMINLEVEL:	RecvProcADMIN_RENEWADMINLEVEL	(pData);	break;	/* 管理者レベル更新 */
	case SBOCOMMANDID_SUB_ADMIN_PLAYSOUND:			RecvProcADMIN_PLAYSOUND			(pData);	break;	/* 効果音の再生 */
	case SBOCOMMANDID_SUB_ADMIN_CHAR_RES_ACCOUNT:	RecvProcADMIN_CHAR_RES_ACCOUNT	(pData);	break;	/* アカウント情報応答 */
	case SBOCOMMANDID_SUB_ADMIN_CHAR_RES_ONLINE:	RecvProcADMIN_CHAR_RES_ONLINE	(pData);	break;	/* オンライン中キャラ一覧応答 */
	case SBOCOMMANDID_SUB_ADMIN_DISABLE_RES_INFO:	RecvProcADMIN_DISABLE_RES_INFO	(pData);	break;	/* 拒否情報応答 */
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_RES_ADMINLEVEL						 */
/* 内容		:受信処理(管理者レベル応答)										 */
/* 日付		:2007/07/06														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_RES_ADMINLEVEL(PBYTE pData)
{
	CPacketADMIN_RES_ADMINLEVEL Packet;

	Packet.Set (pData);
	PostMessage (m_hWnd, WM_ADMINMSG, ADMINMSG_ADMINLEVEL, Packet.m_nAdminLevel);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_RENEWADMINLEVEL						 */
/* 内容		:受信処理(管理者レベル更新)										 */
/* 日付		:2007/07/06														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_RENEWADMINLEVEL(PBYTE pData)
{
	DWORD dwAccountID;
	CPacketADMIN_RENEWADMINLEVEL Packet;

	Packet.Set (pData);

	dwAccountID = m_pMgrData->GetAccountID ();
	if (dwAccountID != Packet.m_dwAccountID) {
		return;
	}
	m_pMgrData->SetAdminLevel (Packet.m_nAdminLevel);
	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWADMINLEVEL, Packet.m_nAdminLevel);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_PLAYSOUND							 */
/* 内容		:受信処理(効果音の再生)											 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_PLAYSOUND(PBYTE pData)
{
	CPacketADMIN_PLAYSOUND Packet;

	Packet.Set (pData);

	m_pMgrSound->PlaySound (Packet.m_dwSoundID);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_CHAR_RES_ACCOUNT						 */
/* 内容		:受信処理(アカウント情報応答)									 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_CHAR_RES_ACCOUNT(PBYTE pData)
{
	DWORD dwDataID, dwDataSize;
	PBYTE pTmpData;
	CInfoAccount InfoTmp;
	CPacketADMIN_CHAR_RES_ACCOUNT Packet;

	Packet.Set (pData);
	InfoTmp.m_dwAccountID	= Packet.m_dwAccountID;
	InfoTmp.m_strAccount	= Packet.m_strAccount;
	InfoTmp.m_strPassword	= Packet.m_strPassword;
	InfoTmp.m_dwIP			= Packet.m_dwIP;
	InfoTmp.m_strLastMacAddr= Packet.m_strMacAddress;
	pTmpData = InfoTmp.GetTmpData (dwDataSize);

	dwDataID = m_pMgrData->Add (dwDataSize, pTmpData);
	PostMessage (m_hWnd, WM_ADMINMSG, ADMINMSG_ACCOUNTINFO, dwDataID);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_CHAR_RES_ONLINE						 */
/* 内容		:受信処理(オンライン中キャラ一覧応答)							 */
/* 日付		:2008/12/01														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_CHAR_RES_ONLINE(PBYTE pData)
{
	PBYTE pTmp;
	PCLibInfoCharCli pLibInfoCharOnline;
	CPacketADMIN_CHAR_RES_ONLINE Packet;

	Packet.Set (pData);
	pTmp = Packet.m_pLibInfo->GetSendData ();

	pLibInfoCharOnline = m_pMgrData->GetLibInfoCharOnline ();
	pLibInfoCharOnline->SetSendData (pTmp);

	SAFE_DELETE_ARRAY (pTmp);

	PostMessage (m_hWnd, WM_ADMINMSG, ADMINMSG_CHAR_ONLINE, 0);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcADMIN_DISABLE_RES_INFO						 */
/* 内容		:受信処理(拒否情報応答)											 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_DISABLE_RES_INFO(PBYTE pData)
{
	CPacketADMIN_DISABLE_RES_INFO Packet;

	Packet.Set (pData);
	m_pLibInfoDisable->CopyAll (Packet.m_pLibInfoDisable);

	PostMessage (m_hWnd, WM_ADMINMSG, ADMINMSG_RENEWDISABLE, 0);
}

/* Copyright(C)URARA-works 2006 */
