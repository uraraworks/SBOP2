/// @file PacketCHAR_REQ_CHARINFO.cpp
/// @brief コマンド(キャラ系:キャラ情報要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/12/31
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_CHARINFO.h"

CPacketCHAR_REQ_CHARINFO::CPacketCHAR_REQ_CHARINFO()
{
	m_dwCharID = 0;
}

CPacketCHAR_REQ_CHARINFO::~CPacketCHAR_REQ_CHARINFO()
{
}

void CPacketCHAR_REQ_CHARINFO::Make(
	DWORD dwCharID)	// [in] キャラID
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwCharID);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_CHARINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwCharID, sizeof (dwCharID), pDataTmp);	// キャラID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketCHAR_REQ_CHARINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwCharID, pDataTmp, sizeof (m_dwCharID), pDataTmp);	// キャラID

	pRet = pDataTmp;
	return pRet;
}
