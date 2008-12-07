/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:MainFrameRecvProcSKILL.cpp									 */
/* 内容			:クライアントメインフレーム(スキル系受信処理) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/06													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoSkill.h"
#include "MgrData.h"
#include "MainFrame.h"


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcSKILL										 */
/* 内容		:受信処理(スキル系)												 */
/* 日付		:2008/12/06														 */
/* ========================================================================= */

void CMainFrame::RecvProcSKILL(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_SKILL_SKILLINFO:	RecvProcSKILL_SKILLINFO	(pData);	break;	/* スキル情報通知 */
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcSKILL_SKILLINFO							 */
/* 内容		:受信処理(スキル情報通知)										 */
/* 日付		:2008/12/06														 */
/* ========================================================================= */

void CMainFrame::RecvProcSKILL_SKILLINFO(PBYTE pData)
{
	CPacketSKILL_SKILLINFO Packet;

	Packet.Set (pData);

	m_pLibInfoSkill->Merge (Packet.m_pLibInfo);

	PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWSKILLINFO, 0);
}

/* Copyright(C)URARA-works 2008 */
