/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:MainFrameRecvProcEFFECT.cpp								 */
/* ���e			:�T�[�o�[���C���t���[��(�G�t�F�N�g�n��M����) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoCharSvr.h"
#include "MainFrame.h"


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcEFFECT										 */
/* ���e		:��M����(�G�t�F�N�g�n)											 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CMainFrame::RecvProcEFFECT(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_EFFECT_REQ_BALLOONINFO:	RecvProcEFFECT_REQ_BALLOONINFO		(pData, dwSessionID);	break;	/* ���o�����v�� */
	case SBOCOMMANDID_SUB_EFFECT_DELETEBALLOONINFO:	RecvProcEFFECT_DELETEBALLOONINFO	(pData, dwSessionID);	break;	/* ���o�����폜 */
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcEFFECT_REQ_BALLOONINFO						 */
/* ���e		:��M����(���o�����v��)										 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CMainFrame::RecvProcEFFECT_REQ_BALLOONINFO(PBYTE pData, DWORD dwSessionID)
{
	CPacketEFFECT_REQ_BALLOONINFO Packet;

	Packet.Set (pData);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcEFFECT_DELETEBALLOONINFO					 */
/* ���e		:��M����(���o�����폜)										 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CMainFrame::RecvProcEFFECT_DELETEBALLOONINFO(PBYTE pData, DWORD dwSessionID)
{
	CPacketEFFECT_DELETEBALLOONINFO Packet;

	Packet.Set (pData);
}

/* Copyright(C)URARA-works 2007 */
