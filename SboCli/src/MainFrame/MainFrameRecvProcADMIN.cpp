/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MainFrameRecvProcADMIN.cpp									 */
/* ���e			:�N���C�A���g���C���t���[��(�Ǘ��Ҍn��M����) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/06													 */
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
/* �֐���	:CMainFrame::RecvProcADMIN										 */
/* ���e		:��M����(�Ǘ��Ҍn)												 */
/* ���t		:2007/07/06														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_ADMIN_RES_ADMINLEVEL:		RecvProcADMIN_RES_ADMINLEVEL	(pData);	break;	/* �Ǘ��҃��x������ */
	case SBOCOMMANDID_SUB_ADMIN_RENEWADMINLEVEL:	RecvProcADMIN_RENEWADMINLEVEL	(pData);	break;	/* �Ǘ��҃��x���X�V */
	case SBOCOMMANDID_SUB_ADMIN_PLAYSOUND:			RecvProcADMIN_PLAYSOUND			(pData);	break;	/* ���ʉ��̍Đ� */
	case SBOCOMMANDID_SUB_ADMIN_CHAR_RES_ACCOUNT:	RecvProcADMIN_CHAR_RES_ACCOUNT	(pData);	break;	/* �A�J�E���g��񉞓� */
	case SBOCOMMANDID_SUB_ADMIN_CHAR_RES_ONLINE:	RecvProcADMIN_CHAR_RES_ONLINE	(pData);	break;	/* �I�����C�����L�����ꗗ���� */
	case SBOCOMMANDID_SUB_ADMIN_DISABLE_RES_INFO:	RecvProcADMIN_DISABLE_RES_INFO	(pData);	break;	/* ���ۏ�񉞓� */
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcADMIN_RES_ADMINLEVEL						 */
/* ���e		:��M����(�Ǘ��҃��x������)										 */
/* ���t		:2007/07/06														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_RES_ADMINLEVEL(PBYTE pData)
{
	CPacketADMIN_RES_ADMINLEVEL Packet;

	Packet.Set (pData);
	PostMessage (m_hWnd, WM_ADMINMSG, ADMINMSG_ADMINLEVEL, Packet.m_nAdminLevel);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcADMIN_RENEWADMINLEVEL						 */
/* ���e		:��M����(�Ǘ��҃��x���X�V)										 */
/* ���t		:2007/07/06														 */
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
/* �֐���	:CMainFrame::RecvProcADMIN_PLAYSOUND							 */
/* ���e		:��M����(���ʉ��̍Đ�)											 */
/* ���t		:2008/06/02														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_PLAYSOUND(PBYTE pData)
{
	CPacketADMIN_PLAYSOUND Packet;

	Packet.Set (pData);

	m_pMgrSound->PlaySound (Packet.m_dwSoundID);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcADMIN_CHAR_RES_ACCOUNT						 */
/* ���e		:��M����(�A�J�E���g��񉞓�)									 */
/* ���t		:2008/06/07														 */
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
/* �֐���	:CMainFrame::RecvProcADMIN_CHAR_RES_ONLINE						 */
/* ���e		:��M����(�I�����C�����L�����ꗗ����)							 */
/* ���t		:2008/12/01														 */
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
/* �֐���	:CMainFrame::RecvProcADMIN_DISABLE_RES_INFO						 */
/* ���e		:��M����(���ۏ�񉞓�)											 */
/* ���t		:2009/04/11														 */
/* ========================================================================= */

void CMainFrame::RecvProcADMIN_DISABLE_RES_INFO(PBYTE pData)
{
	CPacketADMIN_DISABLE_RES_INFO Packet;

	Packet.Set (pData);
	m_pLibInfoDisable->CopyAll (Packet.m_pLibInfoDisable);

	PostMessage (m_hWnd, WM_ADMINMSG, ADMINMSG_RENEWDISABLE, 0);
}

/* Copyright(C)URARA-works 2006 */
