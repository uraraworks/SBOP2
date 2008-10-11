/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MainFrameRecvProcACCOUNT.cpp								 */
/* ���e			:�N���C�A���g���C���t���[��(�A�J�E���g�n��M����) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/08													 */
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
/* �֐���	:CMainFrame::RecvProcACCOUNT									 */
/* ���e		:��M����(�A�J�E���g�n)											 */
/* ���t		:2006/11/08														 */
/* ========================================================================= */

void CMainFrame::RecvProcACCOUNT(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_ACCOUNT_RES_ACCOUNTINFO:	RecvProcACCOUNT_RES_ACCOUNTINFO (pData);	break;	/* �A�J�E���g��񉞓� */
	case SBOCOMMANDID_SUB_ACCOUNT_RES_MAKECHAR:		RecvProcACCOUNT_RES_MAKECHAR (pData);		break;	/* �L�����쐬���� */
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcACCOUNT_RES_ACCOUNTINFO					 */
/* ���e		:��M����(�A�J�E���g��񉞓�)									 */
/* ���t		:2006/11/08														 */
/* ========================================================================= */

void CMainFrame::RecvProcACCOUNT_RES_ACCOUNTINFO(PBYTE pData)
{
	CPacketACCOUNT_RES_ACCOUNTINFO Packet;

	Packet.Set (pData);

	m_pMgrData->SetAccount (Packet.m_pInfo);
	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWACCOUNTINFO, 0);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcACCOUNT_RES_MAKECHAR						 */
/* ���e		:��M����(�L�����쐬����)										 */
/* ���t		:2006/12/29														 */
/* ========================================================================= */

void CMainFrame::RecvProcACCOUNT_RES_MAKECHAR(PBYTE pData)
{
	CPacketACCOUNT_RES_MAKECHAR Packet;
	PCInfoAccount pAccountInfo;

	Packet.Set (pData);

	switch (Packet.m_nResult) {
	case MAKECHARRES_OK:			/* ��薳�� */
		pAccountInfo = m_pMgrData->GetAccount ();
		pAccountInfo->m_adwCharID.Add (Packet.m_dwCharID);
		PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWACCOUNTINFO, 0);
		break;
	case MAKECHARRES_NG_USE:		/* �g�p�ς� */
	case MAKECHARRES_NG_SPACE:		/* �󔒂��g�p����Ă��� */
		break;
	}
	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RES_MAKECHAR, Packet.m_nResult);
}

/* Copyright(C)URARA-works 2006 */
