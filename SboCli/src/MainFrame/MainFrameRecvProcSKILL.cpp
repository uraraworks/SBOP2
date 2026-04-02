/// @file MainFrameRecvProcSKILL.cpp
/// @brief クライアントメインフレーム(スキル系受信処理) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/06
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoSkill.h"
#include "MgrData.h"
#include "MainFrame.h"


void CMainFrame::RecvProcSKILL(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_SKILL_SKILLINFO: RecvProcSKILL_SKILLINFO(pData); break; // スキル情報通知
	}
}


void CMainFrame::RecvProcSKILL_SKILLINFO(PBYTE pData)
{
	CPacketSKILL_SKILLINFO Packet;

	Packet.Set(pData);

	m_pLibInfoSkill->Merge(Packet.m_pLibInfo);

	PostMainFrameMessage(MAINFRAMEMSG_RENEWSKILLINFO, 0);
}
