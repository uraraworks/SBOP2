/// @file MainFrameRecvProcBATTLE.cpp
/// @brief サーバーメインフレーム(戦闘系受信処理) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/27
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoCharSvr.h"
#include "InfoCharSvr.h"
#include "MainFrame.h"

void CMainFrame::RecvProcBATTLE(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_BATTLE_REQ_ATACK:	RecvProcBATTLE_REQ_ATACK(pData, dwSessionID);	break;	// 攻撃要求
	}
}

void CMainFrame::RecvProcBATTLE_REQ_ATACK(PBYTE pData, DWORD dwSessionID)
{
	PCInfoCharSvr pInfoChar;
	CPacketBATTLE_REQ_ATACK Packet;

	Packet.Set(pData);

	pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(Packet.m_dwCharID);
	if (pInfoChar == NULL) {
		goto Exit;
	}

	m_pLibInfoChar->ProcAtack(pInfoChar);
Exit:
	return;
}
