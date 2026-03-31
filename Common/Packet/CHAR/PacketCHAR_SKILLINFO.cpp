/// @file PacketCHAR_SKILLINFO.cpp
/// @brief コマンド(キャラ系:スキル情報通知) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/31
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_SKILLINFO.h"

CPacketCHAR_SKILLINFO::CPacketCHAR_SKILLINFO()
{
}

CPacketCHAR_SKILLINFO::~CPacketCHAR_SKILLINFO()
{
}

void CPacketCHAR_SKILLINFO::Make(
	DWORD dwCharID,	// [in] キャラID
	PARRAYDWORD paSrc)	// [in] スキルID
{
	PBYTE pData, pDataTmp;
	DWORD i, dwSize, dwCount, dwTmp;
	PPACKETBASE pPacketBase;

	dwCount = paSrc->size();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)	+
			 sizeof (DWORD)	+
			 (sizeof (DWORD) * dwCount);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_SKILLINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(pDataTmp, &dwCount,	sizeof (dwCount),	pDataTmp);	// 個数
	for (i = 0; i < dwCount; i ++) {
		dwTmp = paSrc->at(i);
		CopyMemoryRenew(pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);	// スキルID
	}

	RenewPacket(pData, dwSize);
}

PBYTE CPacketCHAR_SKILLINFO::Set(PBYTE pPacket)
{
	DWORD i, dwCount, dwTmp;
	PBYTE pRet, pDataTmp;

	m_adwSkillID.clear();

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(&dwCount,	pDataTmp, sizeof (dwCount),	pDataTmp);	// 個数
	for (i = 0; i < dwCount; i ++) {
		CopyMemoryRenew(&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);	// スキルID
		m_adwSkillID.push_back(dwTmp);
	}

	pRet = pDataTmp;
	return pRet;
}
