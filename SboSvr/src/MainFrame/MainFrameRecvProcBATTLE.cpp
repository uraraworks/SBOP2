/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:MainFrameRecvProcBATTLE.cpp								 */
/* 内容			:サーバーメインフレーム(戦闘系受信処理) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/27													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoCharSvr.h"
#include "InfoCharSvr.h"
#include "MainFrame.h"


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcBATTLE										 */
/* 内容		:受信処理(戦闘系)												 */
/* 日付		:2007/07/27														 */
/* ========================================================================= */

void CMainFrame::RecvProcBATTLE(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_BATTLE_REQ_ATACK:	RecvProcBATTLE_REQ_ATACK (pData, dwSessionID);	break;	/* 攻撃要求 */
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcBATTLE_REQ_ATACK							 */
/* 内容		:受信処理(攻撃要求)												 */
/* 日付		:2007/07/27														 */
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
