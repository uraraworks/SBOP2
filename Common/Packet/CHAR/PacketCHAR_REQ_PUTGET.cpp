/// @file PacketCHAR_REQ_PUTGET.cpp
/// @brief コマンド(キャラ系:アイテムを拾う置く要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/05/05
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_PUTGET.h"

CPacketCHAR_REQ_PUTGET::CPacketCHAR_REQ_PUTGET()
{
	m_dwCharID = 0;
	m_dwItemID = 0;
}

CPacketCHAR_REQ_PUTGET::~CPacketCHAR_REQ_PUTGET()
{
}

void CPacketCHAR_REQ_PUTGET::Make(
	DWORD dwCharID,	// [in] キャラID
	DWORD dwItemID)	// [in] アイテムID
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)	+
			 sizeof (dwItemID);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_PUTGET;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwCharID, sizeof (dwCharID), pDataTmp);	// キャラID
	CopyMemoryRenew(pDataTmp, &dwItemID, sizeof (dwItemID), pDataTmp);	// アイテムID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketCHAR_REQ_PUTGET::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwCharID, pDataTmp, sizeof (m_dwCharID), pDataTmp);	// キャラID
	CopyMemoryRenew(&m_dwItemID, pDataTmp, sizeof (m_dwItemID), pDataTmp);	// アイテムID

	pRet = pDataTmp;
	return pRet;
}
