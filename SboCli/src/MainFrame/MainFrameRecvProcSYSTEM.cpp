/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:MainFrameRecvProcSYSTEM.cpp								 */
/* ���e			:�N���C�A���g���C���t���[��(�V�X�e���n��M����) �����t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/10/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoSystem.h"
#include "MgrData.h"
#include "MainFrame.h"


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcSYSTEM										 */
/* ���e		:��M����(�V�X�e���n)											 */
/* ���t		:2008/10/03														 */
/* ========================================================================= */

void CMainFrame::RecvProcSYSTEM(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_SYSTEM_INFO:	RecvProcSYSTEM_INFO	(pData);	break;	/* �V�X�e�����ʒm */
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcSYSTEM_INFO								 */
/* ���e		:��M����(�V�X�e�����ʒm)										 */
/* ���t		:2008/10/03														 */
/* ========================================================================= */

void CMainFrame::RecvProcSYSTEM_INFO(PBYTE pData)
{
	PCInfoSystem pInfoSystem;
	CPacketSYSTEM_INFO Packet;

	Packet.Set (pData);

	pInfoSystem = (PCInfoSystem)m_pLibInfoSystem->GetPtr ();
	pInfoSystem->Copy (Packet.m_pInfoSystem);

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWSYSTEMINFO, 0);
}

/* Copyright(C)URARA-works 2008 */
