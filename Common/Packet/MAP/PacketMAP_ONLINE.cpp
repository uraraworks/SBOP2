/// @file PacketMAP_ONLINE.cpp
/// @brief コマンド(マップ系:オンライン数通知) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/02/10
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketMAP_ONLINE.h"

CPacketMAP_ONLINE::CPacketMAP_ONLINE()
{
	m_nCount = 0;
}

CPacketMAP_ONLINE::~CPacketMAP_ONLINE()
{
}

void CPacketMAP_ONLINE::Make(
	int nCount)	// [in] オンライン数
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (nCount);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_ONLINE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &nCount, sizeof (nCount), pDataTmp);	// オンライン数

	RenewPacket(pData, dwSize);
}

PBYTE CPacketMAP_ONLINE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_nCount, pDataTmp, sizeof (m_nCount), pDataTmp);	// オンライン数

	pRet = pDataTmp;
	return pRet;
}
