/// @file PacketADMIN_MAP_DELETEMAPSHADOW.cpp
/// @brief コマンド(管理者系:マップ影削除) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/07
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_MAP_DELETEMAPSHADOW.h"

CPacketADMIN_MAP_DELETEMAPSHADOW::CPacketADMIN_MAP_DELETEMAPSHADOW()
{
	m_dwShadowID = 0;
}

CPacketADMIN_MAP_DELETEMAPSHADOW::~CPacketADMIN_MAP_DELETEMAPSHADOW()
{
}

void CPacketADMIN_MAP_DELETEMAPSHADOW::Make(
	DWORD dwShadowID)	// [in] 影ID
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwShadowID);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_DELETEMAPSHADOW;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwShadowID, sizeof (dwShadowID), pDataTmp);	// 影ID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketADMIN_MAP_DELETEMAPSHADOW::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwShadowID,	pDataTmp, sizeof (m_dwShadowID), pDataTmp);	// 影ID

	pRet = pDataTmp;
	return pRet;
}
