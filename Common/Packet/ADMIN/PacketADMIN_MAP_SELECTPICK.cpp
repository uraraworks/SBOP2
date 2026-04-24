/// @file PacketADMIN_MAP_SELECTPICK.cpp
/// @brief コマンド(管理者系:マップ選択ピック) 実装ファイル

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_MAP_SELECTPICK.h"

CPacketADMIN_MAP_SELECTPICK::CPacketADMIN_MAP_SELECTPICK()
{
	m_byType  = SELECTPICK_TYPE_MAPCELL;
	m_dwMapID = 0;
	m_wX      = 0;
	m_wY      = 0;
	m_dwRefID = 0;
}

CPacketADMIN_MAP_SELECTPICK::~CPacketADMIN_MAP_SELECTPICK()
{
}

void CPacketADMIN_MAP_SELECTPICK::Make(
	BYTE  byType,	// [in] 選択種別
	DWORD dwMapID,	// [in] マップID
	WORD  wX,		// [in] タイルX
	WORD  wY,		// [in] タイルY
	DWORD dwRefID)	// [in] 参照ID
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (byType)		+
			 sizeof (dwMapID)		+
			 sizeof (wX)			+
			 sizeof (wY)			+
			 sizeof (dwRefID);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain = SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub  = SBOCOMMANDID_SUB_ADMIN_MAP_SELECTPICK;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &byType,  sizeof (byType),  pDataTmp);	// 選択種別
	CopyMemoryRenew(pDataTmp, &dwMapID, sizeof (dwMapID), pDataTmp);	// マップID
	CopyMemoryRenew(pDataTmp, &wX,      sizeof (wX),      pDataTmp);	// タイルX
	CopyMemoryRenew(pDataTmp, &wY,      sizeof (wY),      pDataTmp);	// タイルY
	CopyMemoryRenew(pDataTmp, &dwRefID, sizeof (dwRefID), pDataTmp);	// 参照ID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketADMIN_MAP_SELECTPICK::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet     = pPacket;
	pDataTmp = CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_byType,  pDataTmp, sizeof (m_byType),  pDataTmp);	// 選択種別
	CopyMemoryRenew(&m_dwMapID, pDataTmp, sizeof (m_dwMapID), pDataTmp);	// マップID
	CopyMemoryRenew(&m_wX,      pDataTmp, sizeof (m_wX),      pDataTmp);	// タイルX
	CopyMemoryRenew(&m_wY,      pDataTmp, sizeof (m_wY),      pDataTmp);	// タイルY
	CopyMemoryRenew(&m_dwRefID, pDataTmp, sizeof (m_dwRefID), pDataTmp);	// 参照ID

	pRet = pDataTmp;
	return pRet;
}
