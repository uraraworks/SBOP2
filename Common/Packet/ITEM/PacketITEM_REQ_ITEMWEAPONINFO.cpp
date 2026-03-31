/// @file PacketITEM_REQ_ITEMWEAPONINFO.cpp
/// @brief コマンド(アイテム系:武器情報要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/08/11
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Command.h"
#include "PacketITEM_REQ_ITEMWEAPONINFO.h"

CPacketITEM_REQ_ITEMWEAPONINFO::CPacketITEM_REQ_ITEMWEAPONINFO()
{
	m_dwWeaponInfoID = 0;
}

CPacketITEM_REQ_ITEMWEAPONINFO::~CPacketITEM_REQ_ITEMWEAPONINFO()
{
}

void CPacketITEM_REQ_ITEMWEAPONINFO::Make(
	DWORD dwWeaponInfoID)	// [in] 武器情報ID
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwWeaponInfoID);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ITEM;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_REQ_ITEMWEAPONINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwWeaponInfoID, sizeof (dwWeaponInfoID), pDataTmp);	// 武器情報ID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketITEM_REQ_ITEMWEAPONINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwWeaponInfoID, pDataTmp, sizeof (m_dwWeaponInfoID), pDataTmp);	// 武器情報ID

	pRet = pDataTmp;
	return pRet;
}
