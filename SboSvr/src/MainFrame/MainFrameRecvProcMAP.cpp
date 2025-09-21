/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:MainFrameRecvProcMAP.cpp									 */
/* 内容			:サーバーメインフレーム(マップ系受信処理) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/01/04													 */
/* ========================================================================= */

#include "stdafx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "InfoMapBase.h"
#include "LibInfoMapBase.h"
#include "MgrData.h"
#include "MainFrame.h"


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcMAP										 */
/* 内容		:受信処理(マップ系)												 */
/* 日付		:2007/01/04														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP(BYTE byCmdSub, PBYTE pData, DWORD dwSessionID)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_MAP_REQ_MAPINFO:	RecvProcMAP_REQ_MAPINFO (pData, dwSessionID);	break;	/* マップ情報要求 */
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RecvProcMAP_REQ_MAPINFO							 */
/* 内容		:受信処理(マップ情報要求)										 */
/* 日付		:2007/01/04														 */
/* ========================================================================= */

void CMainFrame::RecvProcMAP_REQ_MAPINFO(PBYTE pData, DWORD dwSessionID)
{
	PCInfoMapBase pInfoMap;
	CPacketMAP_REQ_MAPINFO Packet;
	CPacketMAP_RES_MAPINFO PacketRES_MAPINFO;

	Packet.Set (pData);

	pInfoMap	= (PCInfoMapBase)m_pLibInfoMap->GetPtr (Packet.m_dwMapID);
	if (pInfoMap == NULL) {
		return;
	}

	PacketRES_MAPINFO.Make (pInfoMap);
	m_pSock->SendTo (dwSessionID, &PacketRES_MAPINFO);
}

/* Copyright(C)URARA-works 2007 */
