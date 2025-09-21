/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:MainFrameRecvProcEFFECT.cpp								 */
/* 内容			:クライアントメインフレーム(エフェクト系受信処理) 実装ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/26													 */
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
/* 関数名	:CMainFrame::RecvProcEFFECT										 */
/* 内容		:受信処理(エフェクト系)											 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

void CMainFrame::RecvProcEFFECT(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_EFFECT_RES_EFFECTINFO:	RecvProcEFFECT_RES_EFFECTINFO	(pData);	break;	/* エフェクト情報応答 */
	case SBOCOMMANDID_SUB_EFFECT_EFFECTINFO:		RecvProcEFFECT_EFFECTINFO		(pData);	break;	/* エフェクト情報通知 */
	case SBOCOMMANDID_SUB_EFFECT_DELETEEFFECTINFO:	RecvProcEFFECT_DELETEEFFECTINFO	(pData);	break;	/* エフェクト情報削除 */
	case SBOCOMMANDID_SUB_EFFECT_BALLOONINFO:		RecvProcEFFECT_BALLOONINFO		(pData);	break;	/* 噴出し情報通知 */
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcEFFECT_RES_EFFECTINFO						 */
/* 内容		:受信処理(エフェクト情報応答)									 */
/* 日付		:2007/07/26														 */
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
/* 関数名	:CMainFrame::RecvProcEFFECT_EFFECTINFO							 */
/* 内容		:受信処理(エフェクト情報通知)									 */
/* 日付		:2007/07/26														 */
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
/* 関数名	:CMainFrame::RecvProcEFFECT_DELETEEFFECTINFO					 */
/* 内容		:受信処理(エフェクト情報削除)									 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

void CMainFrame::RecvProcEFFECT_DELETEEFFECTINFO(PBYTE pData)
{
	CPacketEFFECT_DELETEEFFECTINFO Packet;

	Packet.Set (pData);

	m_pLibInfoEffect->Delete (Packet.m_dwEffectID);
	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWEFFECT, Packet.m_dwEffectID);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcEFFECT_BALLOONINFO							 */
/* 内容		:受信処理(噴出し情報通知)										 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CMainFrame::RecvProcEFFECT_BALLOONINFO(PBYTE pData)
{
	CPacketEFFECT_BALLOONINFO Packet;

	Packet.Set (pData);

	m_pLibInfoEfcBalloon->SetList (Packet.m_dwListID, Packet.m_pLibInfo);

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWBALLOON, Packet.m_dwListID);
}

/* Copyright(C)URARA-works 2007 */
