/// @file PacketMAP_DELETEMAPSHADOW.cpp
/// @brief コマンド(マップ系:マップ影削除) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/08
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketMAP_DELETEMAPSHADOW.h"

CPacketMAP_DELETEMAPSHADOW::CPacketMAP_DELETEMAPSHADOW()
{
	m_dwShadowID = 0;
}

CPacketMAP_DELETEMAPSHADOW::~CPacketMAP_DELETEMAPSHADOW()
{
}

void CPacketMAP_DELETEMAPSHADOW::Make(
	DWORD dwShadowID)	// [in] マップ影ID
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwShadowID);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_DELETEMAPSHADOW;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwShadowID, sizeof (dwShadowID), pDataTmp);	// マップ影ID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketMAP_DELETEMAPSHADOW::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwShadowID,	pDataTmp, sizeof (m_dwShadowID), pDataTmp);	// マップ影ID

	pRet = pDataTmp;
	return pRet;
}
