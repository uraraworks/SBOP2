/// @file PacketCONNECT_KEEPALIVE.cpp
/// @brief コマンド(接続系:生存確認通知) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/21
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Command.h"
#include "PacketCONNECT_KEEPALIVE.h"

CPacketCONNECT_KEEPALIVE::CPacketCONNECT_KEEPALIVE()
{
	m_dwData = 0;
}

CPacketCONNECT_KEEPALIVE::~CPacketCONNECT_KEEPALIVE()
{
}

void CPacketCONNECT_KEEPALIVE::Make(DWORD dwData)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE);
	dwSize += sizeof (dwData);

	pData = ZeroNew(dwSize);
	pPacketBase = (PPACKETBASE)pData;
	pDataTmp = pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CONNECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CONNECT_KEEPALIVE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwData, sizeof (dwData), pDataTmp);	// 返信用データ

	RenewPacket(pData, dwSize);
}

PBYTE CPacketCONNECT_KEEPALIVE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwData, pDataTmp, sizeof (m_dwData), pDataTmp);	// 返信用データ

	pRet = pDataTmp;
	return pRet;
}
