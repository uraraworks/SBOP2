/// @file PacketADMIN_MAP_DELETEPARTS.cpp
/// @brief コマンド(管理者系:マップパーツ削除) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/05/04
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_MAP_DELETEPARTS.h"

CPacketADMIN_MAP_DELETEPARTS::CPacketADMIN_MAP_DELETEPARTS()
{
	m_dwPartsID = 0;
}

CPacketADMIN_MAP_DELETEPARTS::~CPacketADMIN_MAP_DELETEPARTS()
{
}

void CPacketADMIN_MAP_DELETEPARTS::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_DELETEPARTS;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwPartsID, sizeof (dwPartsID), pDataTmp);	// パーツID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketADMIN_MAP_DELETEPARTS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwPartsID,	pDataTmp, sizeof (m_dwPartsID), pDataTmp);	// パーツID

	pRet = pDataTmp;
	return pRet;
}
