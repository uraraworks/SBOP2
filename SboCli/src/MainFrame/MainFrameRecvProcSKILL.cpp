/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:MainFrameRecvProcSKILL.cpp									 */
/* ���e			:�N���C�A���g���C���t���[��(�X�L���n��M����) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/06													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoSkill.h"
#include "MgrData.h"
#include "MainFrame.h"


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcSKILL										 */
/* ���e		:��M����(�X�L���n)												 */
/* ���t		:2008/12/06														 */
/* ========================================================================= */

void CMainFrame::RecvProcSKILL(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_SKILL_SKILLINFO:	RecvProcSKILL_SKILLINFO	(pData);	break;	/* �X�L�����ʒm */
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcSKILL_SKILLINFO							 */
/* ���e		:��M����(�X�L�����ʒm)										 */
/* ���t		:2008/12/06														 */
/* ========================================================================= */

void CMainFrame::RecvProcSKILL_SKILLINFO(PBYTE pData)
{
	CPacketSKILL_SKILLINFO Packet;

	Packet.Set (pData);

	m_pLibInfoSkill->Merge (Packet.m_pLibInfo);

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWSKILLINFO, 0);
}

/* Copyright(C)URARA-works 2008 */
