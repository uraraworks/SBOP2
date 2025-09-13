/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:MainFrameRecvProcEFFECT.cpp								 */
/* 内容			:サーバーメインフレーム(エフェクト系受信処理) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoCharSvr.h"
#include "MainFrame.h"


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcEFFECT										 */
/* 内容		:受信処理(エフェクト系)											 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CMainFrame::RecvProcEFFECT(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_EFFECT_REQ_BALLOONINFO:	RecvProcEFFECT_REQ_BALLOONINFO		(pData, dwSessionID);	break;	/* 噴出し情報要求 */
	case SBOCOMMANDID_SUB_EFFECT_DELETEBALLOONINFO:	RecvProcEFFECT_DELETEBALLOONINFO	(pData, dwSessionID);	break;	/* 噴出し情報削除 */
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcEFFECT_REQ_BALLOONINFO						 */
/* 内容		:受信処理(噴出し情報要求)										 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CMainFrame::RecvProcEFFECT_REQ_BALLOONINFO(PBYTE pData, DWORD dwSessionID)
{
	CPacketEFFECT_REQ_BALLOONINFO Packet;

	Packet.Set (pData);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcEFFECT_DELETEBALLOONINFO					 */
/* 内容		:受信処理(噴出し情報削除)										 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CMainFrame::RecvProcEFFECT_DELETEBALLOONINFO(PBYTE pData, DWORD dwSessionID)
{
	CPacketEFFECT_DELETEBALLOONINFO Packet;

	Packet.Set (pData);
}

/* Copyright(C)URARA-works 2007 */
