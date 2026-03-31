/// @file PacketADMIN_ITEM_DELETE.cpp
/// @brief コマンド(管理者系:アイテム情報削除) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/08/19
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_ITEM_DELETE.h"

CPacketADMIN_ITEM_DELETE::CPacketADMIN_ITEM_DELETE()
{
}

CPacketADMIN_ITEM_DELETE::~CPacketADMIN_ITEM_DELETE()
{
}

void CPacketADMIN_ITEM_DELETE::Make(
	DWORD dwItemID)	// [in] アイテムID
{
	ARRAYDWORD adwItemID;

	adwItemID.push_back(dwItemID);
	Make(&adwItemID);
}

void CPacketADMIN_ITEM_DELETE::Make(
	PARRAYDWORD padwItemID)	// [in] アイテムID
{
	int i, nCount;
	PBYTE pData, pDataTmp;
	DWORD dwItemID, dwSize;
	PPACKETBASE pPacketBase;

	nCount = padwItemID->size();

	dwSize = sizeof (PACKETBASE) +
			 ((nCount + 1) * sizeof (DWORD));

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_ITEM_DELETE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	for (i = 0; i < nCount; i ++) {
		dwItemID = padwItemID->at(i);
		CopyMemoryRenew(pDataTmp, &dwItemID, sizeof (dwItemID), pDataTmp);	// アイテムID
	}
	// 終端用
	dwItemID = 0;
	CopyMemoryRenew(pDataTmp, &dwItemID, sizeof (dwItemID), pDataTmp);	// アイテムID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketADMIN_ITEM_DELETE::Set(PBYTE pPacket)
{
	DWORD dwItemID;
	PBYTE pRet, pDataTmp;

	m_adwItemID.clear();

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	while (1) {
		CopyMemoryRenew(&dwItemID, pDataTmp, sizeof (dwItemID), pDataTmp);	// アイテムID
		if (dwItemID == 0) {
			break;
		}
		m_adwItemID.push_back(dwItemID);
	}

	pRet = pDataTmp;
	return pRet;
}
