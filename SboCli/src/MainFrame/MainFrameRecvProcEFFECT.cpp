/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:MainFrameRecvProcEFFECT.cpp								 */
/* ���e			:�N���C�A���g���C���t���[��(�G�t�F�N�g�n��M����) �����t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoEfcBalloon.h"
#include "LibInfoEffect.h"
#include "MgrData.h"
#include "MainFrame.h"


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcEFFECT										 */
/* ���e		:��M����(�G�t�F�N�g�n)											 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CMainFrame::RecvProcEFFECT(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_EFFECT_RES_EFFECTINFO:	RecvProcEFFECT_RES_EFFECTINFO	(pData);	break;	/* �G�t�F�N�g��񉞓� */
	case SBOCOMMANDID_SUB_EFFECT_EFFECTINFO:		RecvProcEFFECT_EFFECTINFO		(pData);	break;	/* �G�t�F�N�g���ʒm */
	case SBOCOMMANDID_SUB_EFFECT_DELETEEFFECTINFO:	RecvProcEFFECT_DELETEEFFECTINFO	(pData);	break;	/* �G�t�F�N�g���폜 */
	case SBOCOMMANDID_SUB_EFFECT_BALLOONINFO:		RecvProcEFFECT_BALLOONINFO		(pData);	break;	/* ���o�����ʒm */
	}
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcEFFECT_RES_EFFECTINFO						 */
/* ���e		:��M����(�G�t�F�N�g��񉞓�)									 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CMainFrame::RecvProcEFFECT_RES_EFFECTINFO(PBYTE pData)
{
	PCInfoEffect pInfoEffect;
	CPacketEFFECT_RES_EFFECTINFO Packet;

	Packet.Set (pData);

	pInfoEffect = (PCInfoEffect)m_pLibInfoEffect->GetPtr (Packet.m_pInfo->m_dwEffectID);
	if (pInfoEffect == NULL) {
		pInfoEffect = (PCInfoEffect)m_pLibInfoEffect->GetNew ();
		pInfoEffect->Copy (Packet.m_pInfo);
		m_pLibInfoEffect->Add (pInfoEffect);

	} else {
		pInfoEffect->Copy (Packet.m_pInfo);
	}
	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWEFFECT, pInfoEffect->m_dwEffectID);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcEFFECT_EFFECTINFO							 */
/* ���e		:��M����(�G�t�F�N�g���ʒm)									 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CMainFrame::RecvProcEFFECT_EFFECTINFO(PBYTE pData)
{
	int i, nCount;
	PCInfoEffect pInfoEffect, pInfoEffectTmp;
	CPacketEFFECT_EFFECTINFO Packet;

	Packet.Set (pData);

	nCount = Packet.m_pLibInfo->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoEffectTmp = (PCInfoEffect)Packet.m_pLibInfo->GetPtr (i);
		pInfoEffect	 = (PCInfoEffect)m_pLibInfoEffect->GetPtr (pInfoEffectTmp->m_dwEffectID);
		if (pInfoEffect == NULL) {
			pInfoEffect = (PCInfoEffect)m_pLibInfoEffect->GetNew ();
			pInfoEffect->Copy (pInfoEffectTmp);
			m_pLibInfoEffect->Add (pInfoEffect);
		} else {
			pInfoEffect->Copy (pInfoEffectTmp);
		}
	}
	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWEFFECT, 0);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcEFFECT_DELETEEFFECTINFO					 */
/* ���e		:��M����(�G�t�F�N�g���폜)									 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CMainFrame::RecvProcEFFECT_DELETEEFFECTINFO(PBYTE pData)
{
	CPacketEFFECT_DELETEEFFECTINFO Packet;

	Packet.Set (pData);

	m_pLibInfoEffect->Delete (Packet.m_dwEffectID);
	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWEFFECT, Packet.m_dwEffectID);
}


/* ========================================================================= */
/* �֐���	:CMainFrame::RecvProcEFFECT_BALLOONINFO							 */
/* ���e		:��M����(���o�����ʒm)										 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CMainFrame::RecvProcEFFECT_BALLOONINFO(PBYTE pData)
{
	CPacketEFFECT_BALLOONINFO Packet;

	Packet.Set (pData);

	m_pLibInfoEfcBalloon->SetList (Packet.m_dwListID, Packet.m_pLibInfo);

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWBALLOON, Packet.m_dwListID);
}

/* Copyright(C)URARA-works 2007 */
