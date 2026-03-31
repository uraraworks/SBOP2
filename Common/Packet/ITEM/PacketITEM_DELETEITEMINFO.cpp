/// @file PacketITEM_DELETEITEMINFO.cpp
/// @brief コマンド(アイテム系:アイテム情報削除) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/05/05
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketITEM_DELETEITEMINFO.h"

CPacketITEM_DELETEITEMINFO::CPacketITEM_DELETEITEMINFO()
{
	m_dwItemID = 0;
}

CPacketITEM_DELETEITEMINFO::~CPacketITEM_DELETEITEMINFO()
{
}

void CPacketITEM_DELETEITEMINFO::Make(
	DWORD dwItemID)	// [in] アイテムID
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwItemID);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ITEM;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_DELETEITEMINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwItemID, sizeof (dwItemID), pDataTmp);	// アイテムID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketITEM_DELETEITEMINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwItemID,	pDataTmp, sizeof (m_dwItemID), pDataTmp);	// アイテムID

	pRet = pDataTmp;
	return pRet;
}
