/// @file PacketEFFECT_REQ_EFFECTINFO.cpp
/// @brief コマンド(エフェクト系:エフェクト情報要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/26
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketEFFECT_REQ_EFFECTINFO.h"

CPacketEFFECT_REQ_EFFECTINFO::CPacketEFFECT_REQ_EFFECTINFO()
{
	m_dwEffectID = 0;
}

CPacketEFFECT_REQ_EFFECTINFO::~CPacketEFFECT_REQ_EFFECTINFO()
{
}

void CPacketEFFECT_REQ_EFFECTINFO::Make(
	DWORD dwEffectID)	// [in] エフェクトID
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwEffectID);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_EFFECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_EFFECT_REQ_EFFECTINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwEffectID, sizeof (dwEffectID), pDataTmp);	// エフェクトID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketEFFECT_REQ_EFFECTINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwEffectID, pDataTmp, sizeof (m_dwEffectID), pDataTmp);	// エフェクトID

	pRet = pDataTmp;
	return pRet;
}
