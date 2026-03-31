/// @file PacketCHAR_REQ_TAIL.cpp
/// @brief コマンド(キャラ系:付いて行く要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/04
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_TAIL.h"

CPacketCHAR_REQ_TAIL::CPacketCHAR_REQ_TAIL()
{
	m_bTail	= TRUE;
	m_dwCharID	= 0;
	m_dwTargetCharID	= 0;
}

CPacketCHAR_REQ_TAIL::~CPacketCHAR_REQ_TAIL()
{
}

void CPacketCHAR_REQ_TAIL::Make(
	DWORD dwCharID,	// [in] キャラID
	DWORD dwTargetCharID,	// [in] 付いて行く相手のキャラID
	BOOL bTail)	// [in] TRUE:ついていく FALSE:解除
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	 +
			 sizeof (dwCharID)	 +
			 sizeof (dwTargetCharID) +
			 sizeof (bTail);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_TAIL;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(pDataTmp, &dwTargetCharID,	sizeof (dwTargetCharID),	pDataTmp);	// 付いて行く相手のキャラID
	CopyMemoryRenew(pDataTmp, &bTail,	sizeof (bTail),	pDataTmp);	// ついていくか解除か

	RenewPacket(pData, dwSize);
}

PBYTE CPacketCHAR_REQ_TAIL::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(&m_dwTargetCharID,	pDataTmp, sizeof (m_dwTargetCharID),	pDataTmp);	// 付いて行く相手のキャラID
	CopyMemoryRenew(&m_bTail,	pDataTmp, sizeof (m_bTail),	pDataTmp);	// ついていくか解除か

	pRet = pDataTmp;
	return pRet;
}
