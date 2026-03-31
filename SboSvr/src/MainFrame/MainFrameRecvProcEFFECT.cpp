/// @file MainFrameRecvProcEFFECT.cpp
/// @brief サーバーメインフレーム(エフェクト系受信処理) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/12/24
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoCharSvr.h"
#include "MainFrame.h"

void CMainFrame::RecvProcEFFECT(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_EFFECT_REQ_BALLOONINFO:	RecvProcEFFECT_REQ_BALLOONINFO(pData, dwSessionID);	break;	// 噴出し情報要求
	case SBOCOMMANDID_SUB_EFFECT_DELETEBALLOONINFO:	RecvProcEFFECT_DELETEBALLOONINFO(pData, dwSessionID);	break;	// 噴出し情報削除
	}
}

void CMainFrame::RecvProcEFFECT_REQ_BALLOONINFO(PBYTE pData, DWORD dwSessionID)
{
	CPacketEFFECT_REQ_BALLOONINFO Packet;

	Packet.Set(pData);
}

void CMainFrame::RecvProcEFFECT_DELETEBALLOONINFO(PBYTE pData, DWORD dwSessionID)
{
	CPacketEFFECT_DELETEBALLOONINFO Packet;

	Packet.Set(pData);
}
