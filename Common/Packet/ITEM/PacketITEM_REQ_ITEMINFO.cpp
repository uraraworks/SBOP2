/// @file PacketITEM_REQ_ITEMINFO.cpp
/// @brief コマンド(アイテム系:アイテム情報要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/08/16
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketITEM_REQ_ITEMINFO.h"

CPacketITEM_REQ_ITEMINFO::CPacketITEM_REQ_ITEMINFO()
{
	m_dwItemID = 0;
}

CPacketITEM_REQ_ITEMINFO::~CPacketITEM_REQ_ITEMINFO()
{
}

void CPacketITEM_REQ_ITEMINFO::Make(
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_REQ_ITEMINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwItemID, sizeof (dwItemID), pDataTmp);	// アイテムID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketITEM_REQ_ITEMINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwItemID, pDataTmp, sizeof (m_dwItemID), pDataTmp);	// アイテムID

	pRet = pDataTmp;
	return pRet;
}
