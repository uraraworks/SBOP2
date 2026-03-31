/// @file MainFrameRecvProcADMIN.cpp
/// @brief クライアントメインフレーム(管理者系受信処理) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/06
/// @copyright Copyright(C)URARA-works 2006

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


void CMainFrame::RecvProcADMIN(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_ADMIN_RES_ADMINLEVEL: RecvProcADMIN_RES_ADMINLEVEL(pData); break; // 管理者レベル応答
	case SBOCOMMANDID_SUB_ADMIN_RENEWADMINLEVEL: RecvProcADMIN_RENEWADMINLEVEL(pData); break; // 管理者レベル更新
	case SBOCOMMANDID_SUB_ADMIN_PLAYSOUND: RecvProcADMIN_PLAYSOUND(pData); break; // 効果音の再生
	case SBOCOMMANDID_SUB_ADMIN_CHAR_RES_ACCOUNT: RecvProcADMIN_CHAR_RES_ACCOUNT(pData); break; // アカウント情報応答
	case SBOCOMMANDID_SUB_ADMIN_CHAR_RES_ONLINE: RecvProcADMIN_CHAR_RES_ONLINE(pData); break; // オンライン中キャラ一覧応答
	case SBOCOMMANDID_SUB_ADMIN_DISABLE_RES_INFO: RecvProcADMIN_DISABLE_RES_INFO(pData); break; // 拒否情報応答
	}
}


void CMainFrame::RecvProcADMIN_RES_ADMINLEVEL(PBYTE pData)
{
	CPacketADMIN_RES_ADMINLEVEL Packet;

	Packet.Set(pData);
	PostMessage(m_hWnd, WM_ADMINMSG, ADMINMSG_ADMINLEVEL, Packet.m_nAdminLevel);
}


void CMainFrame::RecvProcADMIN_RENEWADMINLEVEL(PBYTE pData)
{
	DWORD dwAccountID;
	CPacketADMIN_RENEWADMINLEVEL Packet;

	Packet.Set(pData);

	dwAccountID = m_pMgrData->GetAccountID();
	if (dwAccountID != Packet.m_dwAccountID) {
		return;
	}
	m_pMgrData->SetAdminLevel(Packet.m_nAdminLevel);
	PostMessage(m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWADMINLEVEL, Packet.m_nAdminLevel);
}


void CMainFrame::RecvProcADMIN_PLAYSOUND(PBYTE pData)
{
	CPacketADMIN_PLAYSOUND Packet;

	Packet.Set(pData);

	m_pMgrSound->PlaySound(Packet.m_dwSoundID);
}


void CMainFrame::RecvProcADMIN_CHAR_RES_ACCOUNT(PBYTE pData)
{
	DWORD dwDataID, dwDataSize;
	PBYTE pTmpData;
	CInfoAccount InfoTmp;
	CPacketADMIN_CHAR_RES_ACCOUNT Packet;

	Packet.Set(pData);
	InfoTmp.m_dwAccountID = Packet.m_dwAccountID;
	InfoTmp.m_strAccount = Packet.m_strAccount;
	InfoTmp.m_strPassword = Packet.m_strPassword;
	InfoTmp.m_dwIP = Packet.m_dwIP;
	InfoTmp.m_strLastMacAddr = Packet.m_strMacAddress;
	pTmpData = InfoTmp.GetTmpData(dwDataSize);

	dwDataID = m_pMgrData->Add(dwDataSize, pTmpData);
	PostMessage(m_hWnd, WM_ADMINMSG, ADMINMSG_ACCOUNTINFO, dwDataID);
}


void CMainFrame::RecvProcADMIN_CHAR_RES_ONLINE(PBYTE pData)
{
	PBYTE pTmp;
	PCLibInfoCharCli pLibInfoCharOnline;
	CPacketADMIN_CHAR_RES_ONLINE Packet;

	Packet.Set(pData);
	pTmp = Packet.m_pLibInfo->GetSendData();

	pLibInfoCharOnline = m_pMgrData->GetLibInfoCharOnline();
	pLibInfoCharOnline->SetSendData(pTmp);

	SAFE_DELETE_ARRAY(pTmp);

	PostMessage(m_hWnd, WM_ADMINMSG, ADMINMSG_CHAR_ONLINE, 0);
}


void CMainFrame::RecvProcADMIN_DISABLE_RES_INFO(PBYTE pData)
{
	CPacketADMIN_DISABLE_RES_INFO Packet;

	Packet.Set(pData);
	m_pLibInfoDisable->CopyAll(Packet.m_pLibInfoDisable);

	PostMessage(m_hWnd, WM_ADMINMSG, ADMINMSG_RENEWDISABLE, 0);
}
