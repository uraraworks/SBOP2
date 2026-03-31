/// @file MainFrameRecvProcMAP.cpp
/// @brief サーバーメインフレーム(マップ系受信処理) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/01/04
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "InfoMapBase.h"
#include "LibInfoMapBase.h"
#include "MgrData.h"
#include "MainFrame.h"

void CMainFrame::RecvProcMAP(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_MAP_REQ_MAPINFO:	RecvProcMAP_REQ_MAPINFO(pData, dwSessionID);	break;	// マップ情報要求
	}
}

void CMainFrame::RecvProcMAP_REQ_MAPINFO(PBYTE pData, DWORD dwSessionID)
{
	PCInfoMapBase pInfoMap;
	CPacketMAP_REQ_MAPINFO Packet;
	CPacketMAP_RES_MAPINFO PacketRES_MAPINFO;

	Packet.Set(pData);

	pInfoMap	= (PCInfoMapBase)m_pLibInfoMap->GetPtr(Packet.m_dwMapID);
	if (pInfoMap == NULL) {
		return;
	}

	PacketRES_MAPINFO.Make(pInfoMap);
	m_pSock->SendTo(dwSessionID, &PacketRES_MAPINFO);
}
