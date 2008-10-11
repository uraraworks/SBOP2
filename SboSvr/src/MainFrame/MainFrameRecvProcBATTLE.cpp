/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:MainFrameRecvProcBATTLE.cpp								 */
/* ���e			:�T�[�o�[���C���t���[��(�퓬�n��M����) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/27													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoCharSvr.h"
#include "InfoCharSvr.h"
#include "MainFrame.h"


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcBATTLE										 */
/* ���e		:��M����(�퓬�n)												 */
/* ���t		:2007/07/27														 */
/* ========================================================================= */

void CMainFrame::RecvProcBATTLE(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_BATTLE_REQ_ATACK:	RecvProcBATTLE_REQ_ATACK (pData, dwSessionID);	break;	/* �U���v�� */
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcBATTLE_REQ_ATACK							 */
/* ���e		:��M����(�U���v��)												 */
/* ���t		:2007/07/27														 */
/* ========================================================================= */

void CMainFrame::RecvProcBATTLE_REQ_ATACK(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharSvr pInfoChar;
	CPacketBATTLE_REQ_ATACK Packet;

	Packet.Set (pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		goto Exit;
	}

	m_pLibInfoChar->ProcAtack (pInfoChar);
Exit:
	return;
}

/* Copyright(C)URARA-works 2007 */
