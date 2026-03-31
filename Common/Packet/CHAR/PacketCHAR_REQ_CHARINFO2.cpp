/// @file PacketCHAR_REQ_CHARINFO2.cpp
/// @brief コマンド(キャラ系:キャラID通知) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/02/13
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_CHARINFO2.h"

CPacketCHAR_REQ_CHARINFO2::CPacketCHAR_REQ_CHARINFO2()
{
}

CPacketCHAR_REQ_CHARINFO2::~CPacketCHAR_REQ_CHARINFO2()
{
}

void CPacketCHAR_REQ_CHARINFO2::Make(
	PARRAYDWORD paSrc)	// [in] キャラID
{
	PBYTE pData, pDataTmp;
	DWORD i, dwSize, dwCount, dwTmp;
	PPACKETBASE pPacketBase;

	dwCount = paSrc->size();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (DWORD)	+
			 (sizeof (DWORD) * dwCount);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_CHARINFO2;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwCount, sizeof (dwCount), pDataTmp);	// 個数
	for (i = 0; i < dwCount; i ++) {
		dwTmp = paSrc->at(i);
		CopyMemoryRenew(pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);	// キャラID
	}

	RenewPacket(pData, dwSize);
}

PBYTE CPacketCHAR_REQ_CHARINFO2::Set(PBYTE pPacket)
{
	DWORD i, dwCount, dwTmp;
	PBYTE pRet, pDataTmp;

	m_adwCharID.clear();

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&dwCount, pDataTmp, sizeof (dwCount), pDataTmp);	// 個数
	for (i = 0; i < dwCount; i ++) {
		CopyMemoryRenew(&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);	// キャラID
		m_adwCharID.push_back(dwTmp);
	}

	pRet = pDataTmp;
	return pRet;
}
