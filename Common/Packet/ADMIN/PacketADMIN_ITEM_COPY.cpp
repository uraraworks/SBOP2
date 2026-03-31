/// @file PacketADMIN_ITEM_COPY.cpp
/// @brief コマンド(管理者系:アイテム情報コピー) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/22
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_ITEM_COPY.h"

CPacketADMIN_ITEM_COPY::CPacketADMIN_ITEM_COPY()
{
	m_dwItemID = 0;
}

CPacketADMIN_ITEM_COPY::~CPacketADMIN_ITEM_COPY()
{
}

void CPacketADMIN_ITEM_COPY::Make(
	DWORD dwItemID)	// [in] コピー元アイテムID
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwItemID);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_ITEM_COPY;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwItemID, sizeof (dwItemID), pDataTmp);	// アイテムID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketADMIN_ITEM_COPY::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwItemID, pDataTmp, sizeof (m_dwItemID), pDataTmp);	// アイテムID

	pRet = pDataTmp;
	return pRet;
}
