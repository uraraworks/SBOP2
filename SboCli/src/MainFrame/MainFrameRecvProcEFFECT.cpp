/// @file MainFrameRecvProcEFFECT.cpp
/// @brief クライアントメインフレーム(エフェクト系受信処理) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/26
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoEfcBalloon.h"
#include "LibInfoEffect.h"
#include "MgrData.h"
#include "MainFrame.h"


void CMainFrame::RecvProcEFFECT(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_EFFECT_RES_EFFECTINFO: RecvProcEFFECT_RES_EFFECTINFO(pData); break; // エフェクト情報応答
	case SBOCOMMANDID_SUB_EFFECT_EFFECTINFO: RecvProcEFFECT_EFFECTINFO(pData); break; // エフェクト情報通知
	case SBOCOMMANDID_SUB_EFFECT_DELETEEFFECTINFO: RecvProcEFFECT_DELETEEFFECTINFO(pData); break; // エフェクト情報削除
	case SBOCOMMANDID_SUB_EFFECT_BALLOONINFO: RecvProcEFFECT_BALLOONINFO(pData); break; // 噴出し情報通知
	}
}


void CMainFrame::RecvProcEFFECT_RES_EFFECTINFO(PBYTE pData)
{
	PCInfoEffect pInfoEffect;
	CPacketEFFECT_RES_EFFECTINFO Packet;

	Packet.Set(pData);

	pInfoEffect = (PCInfoEffect)m_pLibInfoEffect->GetPtr(Packet.m_pInfo->m_dwEffectID);
	if (pInfoEffect == NULL) {
		pInfoEffect = (PCInfoEffect)m_pLibInfoEffect->GetNew();
		pInfoEffect->Copy(Packet.m_pInfo);
		m_pLibInfoEffect->Add(pInfoEffect);

	} else {
		pInfoEffect->Copy(Packet.m_pInfo);
	}
	PostMainFrameMessage(MAINFRAMEMSG_RENEWEFFECT, pInfoEffect->m_dwEffectID);
}


void CMainFrame::RecvProcEFFECT_EFFECTINFO(PBYTE pData)
{
	int i, nCount;
	PCInfoEffect pInfoEffect, pInfoEffectTmp;
	CPacketEFFECT_EFFECTINFO Packet;

	Packet.Set(pData);

	nCount = Packet.m_pLibInfo->GetCount();
	for (i = 0; i < nCount; i++) {
		pInfoEffectTmp = (PCInfoEffect)Packet.m_pLibInfo->GetPtr(i);
		pInfoEffect = (PCInfoEffect)m_pLibInfoEffect->GetPtr(pInfoEffectTmp->m_dwEffectID);
		if (pInfoEffect == NULL) {
			pInfoEffect = (PCInfoEffect)m_pLibInfoEffect->GetNew();
			pInfoEffect->Copy(pInfoEffectTmp);
			m_pLibInfoEffect->Add(pInfoEffect);
		} else {
			pInfoEffect->Copy(pInfoEffectTmp);
		}
	}
	PostMainFrameMessage(MAINFRAMEMSG_RENEWEFFECT, 0);
}


void CMainFrame::RecvProcEFFECT_DELETEEFFECTINFO(PBYTE pData)
{
	CPacketEFFECT_DELETEEFFECTINFO Packet;

	Packet.Set(pData);

	m_pLibInfoEffect->Delete(Packet.m_dwEffectID);
	PostMainFrameMessage(MAINFRAMEMSG_RENEWEFFECT, Packet.m_dwEffectID);
}


void CMainFrame::RecvProcEFFECT_BALLOONINFO(PBYTE pData)
{
	CPacketEFFECT_BALLOONINFO Packet;

	Packet.Set(pData);

	m_pLibInfoEfcBalloon->SetList(Packet.m_dwListID, Packet.m_pLibInfo);

	PostMainFrameMessage(MAINFRAMEMSG_RENEWBALLOON, Packet.m_dwListID);
}
