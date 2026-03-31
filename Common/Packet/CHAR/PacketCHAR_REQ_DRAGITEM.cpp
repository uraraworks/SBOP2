/// @file PacketCHAR_REQ_DRAGITEM.cpp
/// @brief コマンド(キャラ系:アイテム位置変更要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/11/22
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_DRAGITEM.h"

CPacketCHAR_REQ_DRAGITEM::CPacketCHAR_REQ_DRAGITEM()
{
	m_dwCharID = 0;
	m_dwItemID = 0;
	m_ptNewPos.x = m_ptNewPos.y = 0;
}

CPacketCHAR_REQ_DRAGITEM::~CPacketCHAR_REQ_DRAGITEM()
{
}

void CPacketCHAR_REQ_DRAGITEM::Make(
	DWORD dwCharID,	// [in] キャラID
	DWORD dwItemID,	// [in] アイテムID
	POINT ptNewPos)	// [in] 新しい位置
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)	+
			 sizeof (dwItemID)	+
			 sizeof (ptNewPos);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_DRAGITEM;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwCharID, sizeof (dwCharID), pDataTmp);	// キャラID
	CopyMemoryRenew(pDataTmp, &dwItemID, sizeof (dwItemID), pDataTmp);	// アイテムID
	CopyMemoryRenew(pDataTmp, &ptNewPos, sizeof (ptNewPos), pDataTmp);	// 新しい位置

	RenewPacket(pData, dwSize);
}

PBYTE CPacketCHAR_REQ_DRAGITEM::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwCharID, pDataTmp, sizeof (m_dwCharID), pDataTmp);	// キャラID
	CopyMemoryRenew(&m_dwItemID, pDataTmp, sizeof (m_dwItemID), pDataTmp);	// アイテムID
	CopyMemoryRenew(&m_ptNewPos, pDataTmp, sizeof (m_ptNewPos), pDataTmp);	// 新しい位置

	pRet = pDataTmp;
	return pRet;
}
