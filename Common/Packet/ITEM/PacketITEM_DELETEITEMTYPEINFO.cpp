/// @file PacketITEM_DELETEITEMTYPEINFO.cpp
/// @brief コマンド(アイテム系:アイテム種別情報削除) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/10/03
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketITEM_DELETEITEMTYPEINFO.h"

CPacketITEM_DELETEITEMTYPEINFO::CPacketITEM_DELETEITEMTYPEINFO()
{
	m_dwTypeID = 0;
}

CPacketITEM_DELETEITEMTYPEINFO::~CPacketITEM_DELETEITEMTYPEINFO()
{
}

void CPacketITEM_DELETEITEMTYPEINFO::Make(
	DWORD dwTypeID)	// [in] 種別ID
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwTypeID);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ITEM;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_DELETEITEMTYPEINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwTypeID, sizeof (dwTypeID), pDataTmp);	// 種別ID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketITEM_DELETEITEMTYPEINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwTypeID,	pDataTmp, sizeof (m_dwTypeID), pDataTmp);	// 種別ID

	pRet = pDataTmp;
	return pRet;
}
