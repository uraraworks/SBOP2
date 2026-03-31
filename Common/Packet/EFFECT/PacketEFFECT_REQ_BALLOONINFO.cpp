/// @file PacketEFFECT_REQ_BALLOONINFO.cpp
/// @brief コマンド(噴出し系:噴出し情報要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/12/24
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketEFFECT_REQ_BALLOONINFO.h"

CPacketEFFECT_REQ_BALLOONINFO::CPacketEFFECT_REQ_BALLOONINFO()
{
	m_dwBalloonID = 0;
}

CPacketEFFECT_REQ_BALLOONINFO::~CPacketEFFECT_REQ_BALLOONINFO()
{
}

void CPacketEFFECT_REQ_BALLOONINFO::Make(
	DWORD dwBalloonID)	// [in] 噴出しID
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwBalloonID);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_EFFECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_EFFECT_REQ_BALLOONINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwBalloonID, sizeof (dwBalloonID), pDataTmp);	// 噴出しID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketEFFECT_REQ_BALLOONINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwBalloonID, pDataTmp, sizeof (m_dwBalloonID), pDataTmp);	// 噴出しID

	pRet = pDataTmp;
	return pRet;
}
