/// @file PacketMAP_DELETEPARTS.cpp
/// @brief コマンド(マップ系:マップパーツ削除) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/05/04
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketMAP_DELETEPARTS.h"

CPacketMAP_DELETEPARTS::CPacketMAP_DELETEPARTS()
{
	m_dwPartsID = 0;
}

CPacketMAP_DELETEPARTS::~CPacketMAP_DELETEPARTS()
{
}

void CPacketMAP_DELETEPARTS::Make(
	DWORD dwPartsID)	// [in] パーツID
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwPartsID);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_DELETEPARTS;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwPartsID, sizeof (dwPartsID), pDataTmp);	// パーツID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketMAP_DELETEPARTS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwPartsID,	pDataTmp, sizeof (m_dwPartsID), pDataTmp);	// パーツID

	pRet = pDataTmp;
	return pRet;
}
