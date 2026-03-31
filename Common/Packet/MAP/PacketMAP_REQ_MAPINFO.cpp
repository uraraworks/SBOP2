/// @file PacketMAP_REQ_MAPINFO.cpp
/// @brief コマンド(マップ系:マップ情報要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/01/04
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketMAP_REQ_MAPINFO.h"

CPacketMAP_REQ_MAPINFO::CPacketMAP_REQ_MAPINFO()
{
	m_dwMapID = 0;
}

CPacketMAP_REQ_MAPINFO::~CPacketMAP_REQ_MAPINFO()
{
}

void CPacketMAP_REQ_MAPINFO::Make(
	DWORD dwMapID)	// [in] マップID
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwMapID);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_REQ_MAPINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwMapID, sizeof (dwMapID), pDataTmp);	// マップID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketMAP_REQ_MAPINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwMapID, pDataTmp, sizeof (m_dwMapID), pDataTmp);	// マップID

	pRet = pDataTmp;
	return pRet;
}
