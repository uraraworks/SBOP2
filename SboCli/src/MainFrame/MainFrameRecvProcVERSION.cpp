/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MainFrameRecvProcVERSION.cpp								 */
/* ���e			:�N���C�A���g���C���t���[��(�o�[�W�����n��M����) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "WindowLOGIN.h"
#include "MacAddr.h"
#include "MgrWindow.h"
#include "MainFrame.h"


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcVERSION									 */
/* ���e		:��M����(�o�[�W�����n)											 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcVERSION(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_VERSION_RES_VERSIONCHECK:		RecvProcVERSION_RES_VERSIONCHECK	(pData);	break;	/* �o�[�W�����`�F�b�N���� */
	case SBOCOMMANDID_SUB_VERSION_RES_FILELISTCHECK:	RecvProcVERSION_RES_FILELISTCHECK	(pData);	break;	/* �t�@�C�����X�g�`�F�b�N���� */
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcVERSION_RES_VERSIONCHECK					 */
/* ���e		:��M����(�o�[�W�����`�F�b�N����)								 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcVERSION_RES_VERSIONCHECK(PBYTE pData)
{
	BYTE abyTmp[10];
	PCWindowLOGIN pWindow;
	CPacketVERSION_RES_VERSIONCHECK Packet;
	CPacketCONNECT_REQ_LOGIN PacketLOGIN;
	CMacAddr MacAddr;

	Packet.Set (pData);

	switch (Packet.m_nResult) {
	case VERSIONCHECKRES_OK:			/* ��薳�� */
		pWindow = (PCWindowLOGIN)m_pMgrWindow->GetWindow (WINDOWTYPE_LOGIN);
		if (pWindow == NULL) {
			break;
		}
		ZeroMemory (abyTmp, sizeof (abyTmp));
		MacAddr.Get (abyTmp);
		PacketLOGIN.Make (pWindow->GetAccount (), pWindow->GetPassword (), abyTmp);
		m_pSock->Send (&PacketLOGIN);
		break;

	case VERSIONCHECKRES_NG_VERSION:	/* �o�[�W�����s��v */
		m_pSock->Destroy ();
		DisConnectProc (DISCONNECTID_VERSION);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcVERSION_RES_FILELISTCHECK					 */
/* ���e		:��M����(�t�@�C�����X�g�`�F�b�N����)							 */
/* ���t		:2007/09/01														 */
/* ========================================================================= */

void CMainFrame::RecvProcVERSION_RES_FILELISTCHECK(PBYTE pData)
{
	CPacketVERSION_RES_FILELISTCHECK Packet;

	Packet.Set (pData);

	switch (Packet.m_nResult) {
	case FILELISTCHECKRES_OK:			/* ��薳�� */
		break;
	case FILELISTCHECKRES_NG:			/* �s��v */
		break;
	}
}

/* Copyright(C)URARA-works 2006 */
