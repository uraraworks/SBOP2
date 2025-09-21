/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:MainFrameRecvProcSYSTEM.cpp								 */
/* 内容			:クライアントメインフレーム(システム系受信処理) 実装ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/10/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoSystem.h"
#include "MgrData.h"
#include "MainFrame.h"


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcSYSTEM										 */
/* 内容		:受信処理(システム系)											 */
/* 日付		:2008/10/03														 */
/* ========================================================================= */

void CMainFrame::RecvProcSYSTEM(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_SYSTEM_INFO:	RecvProcSYSTEM_INFO	(pData);	break;	/* システム情報通知 */
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcSYSTEM_INFO								 */
/* 内容		:受信処理(システム情報通知)										 */
/* 日付		:2008/10/03														 */
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
