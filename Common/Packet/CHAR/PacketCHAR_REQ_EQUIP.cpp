/// @file PacketCHAR_REQ_EQUIP.cpp
/// @brief コマンド(キャラ系:装備要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/10/08
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_EQUIP.h"

CPacketCHAR_REQ_EQUIP::CPacketCHAR_REQ_EQUIP()
{
	m_nUnsetType	= -1;
	m_dwCharID	= 0;
	m_dwItemID	= 0;
}

CPacketCHAR_REQ_EQUIP::~CPacketCHAR_REQ_EQUIP()
{
}

void CPacketCHAR_REQ_EQUIP::Make(
	DWORD dwCharID,	// [in] キャラID
	DWORD dwItemID,	// [in] 装備するアイテムID
	int nUnsetType)	// [in] 装備解除する種別
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwCharID)	 +
			 sizeof (dwItemID)	 +
			 sizeof (nUnsetType);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_EQUIP;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(pDataTmp, &dwItemID,	sizeof (dwItemID),	pDataTmp);	// 装備するアイテムID
	CopyMemoryRenew(pDataTmp, &nUnsetType,	sizeof (nUnsetType),	pDataTmp);	// 装備解除する種別

	RenewPacket(pData, dwSize);
}

PBYTE CPacketCHAR_REQ_EQUIP::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(&m_dwItemID,	pDataTmp, sizeof (m_dwItemID),	pDataTmp);	// 装備するアイテムID
	CopyMemoryRenew(&m_nUnsetType,	pDataTmp, sizeof (m_nUnsetType),	pDataTmp);	// 装備解除する種別

	pRet = pDataTmp;
	return pRet;
}
