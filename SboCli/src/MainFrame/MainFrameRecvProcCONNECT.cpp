/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MainFrameRecvProcCONNECT.cpp								 */
/* ���e			:�N���C�A���g���C���t���[��(�o�[�W�����n��M����) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/05													 */
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
/* �֐���	:CMainFrame::RecvProcCONNECT									 */
/* ���e		:��M����(�ڑ��n)												 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcCONNECT(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_CONNECT_RES_LOGIN:	RecvProcCONNECT_RES_LOGIN (pData);	break;	/* ���O�C������ */
	case SBOCOMMANDID_SUB_CONNECT_RES_PLAY:		RecvProcCONNECT_RES_PLAY (pData);	break;	/* �Q�[���J�n���� */
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCONNECT_RES_LOGIN							 */
/* ���e		:��M����(���O�C������)											 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CMainFrame::RecvProcCONNECT_RES_LOGIN(PBYTE pData)
{
	CPacketCONNECT_RES_LOGIN Packet;
	CPacketACCOUNT_REQ_ACCOUNTINFO PacketREQ_ACCOUNTINFO;
	PCWindowLOGIN pWindow;

	Packet.Set (pData);

	switch (Packet.m_nResult) {
	case LOGINRES_OK:			/* ��薳�� */
		/* ���͓��e�ƃ`�F�b�N��Ԃ�ۑ� */
		pWindow = (PCWindowLOGIN)m_pMgrWindow->GetWindow (WINDOWTYPE_LOGIN);
		pWindow->Save ();

//Todo:���m�点�v��
		PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_CHGSTATE, GAMESTATE_LOGINMENU);
		/* �A�J�E���g����v�� */
		PacketREQ_ACCOUNTINFO.Make (Packet.m_dwAccountID);
		m_pSock->Send (&PacketREQ_ACCOUNTINFO);
		break;

	case LOGINRES_NG_PASSWORD:	/* �p�X���[�h�s��v */
		DisConnectProc (DISCONNECTID_PASSWORD);
		break;

	case LOGINRES_NG_LOGIN:		/* ���O�C���ς� */
		DisConnectProc (DISCONNECTID_LOGIN);
		break;

	case LOGINRES_NG_MAC:		/* �쐬�ς� */
		DisConnectProc (DISCONNECTID_MAC);
		break;

	case LOGINRES_NG_DISABLE:	/* ���O�C������ */
		m_pMgrData->SetDisableLogin(TRUE);
		m_pMgrData->SaveIniData();
		PostMessage (m_hWnd, WM_CLOSE, 0, 0);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcCONNECT_RES_PLAY							 */
/* ���e		:��M����(�Q�[���J�n����)										 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

void CMainFrame::RecvProcCONNECT_RES_PLAY(PBYTE pData)
{
	CPacketCONNECT_RES_PLAY Packet;

	Packet.Set (pData);

	switch (Packet.m_nResult) {
	case PLAYRES_ADMINLEVEL_ALL:	/* �Ǘ���(�S����) */
		m_pMgrData->SetAdminLevel (ADMINLEVEL_ALL);
		break;
	}

	ChgGameState (GAMESTATE_MAP);
}

/* Copyright(C)URARA-works 2006 */
