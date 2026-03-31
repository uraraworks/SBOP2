/// @file PacketMAP_RENEWMAPSIZE.cpp
/// @brief コマンド(マップ系:マップサイズ更新) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/05/01
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "InfoMapParts.h"
#include "Command.h"
#include "PacketMAP_RENEWMAPSIZE.h"

CPacketMAP_RENEWMAPSIZE::CPacketMAP_RENEWMAPSIZE()
{
	m_dwMapID	= 0;
	m_nDirection	= 0;
	m_nSize	= 0;
}

CPacketMAP_RENEWMAPSIZE::~CPacketMAP_RENEWMAPSIZE()
{
}

void CPacketMAP_RENEWMAPSIZE::Make(
	DWORD dwMapID,	// [in] マップID
	int nDirection,	// [in] 増減する方向
	int nSize)	// [in] 増減する距離
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwMapID)	+
			 sizeof (nDirection)	+
			 sizeof (nSize);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_RENEWMAPSIZE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwMapID,	sizeof (dwMapID),	pDataTmp);	// マップID
	CopyMemoryRenew(pDataTmp, &nDirection,	sizeof (nDirection),	pDataTmp);	// 増減する方向
	CopyMemoryRenew(pDataTmp, &nSize,	sizeof (nSize),	pDataTmp);	// 増減する距離

	RenewPacket(pData, dwSize);
}

PBYTE CPacketMAP_RENEWMAPSIZE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwMapID,	pDataTmp, sizeof (m_dwMapID),	pDataTmp);	// マップID
	CopyMemoryRenew(&m_nDirection,	pDataTmp, sizeof (m_nDirection),	pDataTmp);	// 増減する方向
	CopyMemoryRenew(&m_nSize,	pDataTmp, sizeof (m_nSize),	pDataTmp);	// 増減する距離

	pRet = pDataTmp;
	return pRet;
}
