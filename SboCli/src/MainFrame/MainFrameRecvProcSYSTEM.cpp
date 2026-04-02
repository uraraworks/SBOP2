/// @file MainFrameRecvProcSYSTEM.cpp
/// @brief クライアントメインフレーム(システム系受信処理) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/10/03
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoSystem.h"
#include "MgrData.h"
#include "MainFrame.h"


void CMainFrame::RecvProcSYSTEM(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_SYSTEM_INFO: RecvProcSYSTEM_INFO(pData); break; // システム情報通知
	}
}


void CMainFrame::RecvProcSYSTEM_INFO(PBYTE pData)
{
	PCInfoSystem pInfoSystem;
	CPacketSYSTEM_INFO Packet;

	Packet.Set(pData);

	pInfoSystem = (PCInfoSystem)m_pLibInfoSystem->GetPtr();
	pInfoSystem->Copy(Packet.m_pInfoSystem);

	PostMainFrameMessage(MAINFRAMEMSG_RENEWSYSTEMINFO, 0);
}
