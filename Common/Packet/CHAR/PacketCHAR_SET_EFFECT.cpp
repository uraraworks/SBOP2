/// @file PacketCHAR_SET_EFFECT.cpp
/// @brief コマンド(キャラ系:エフェクトを指定) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/27
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_SET_EFFECT.h"

CPacketCHAR_SET_EFFECT::CPacketCHAR_SET_EFFECT()
{
	m_dwCharID	= 0;
	m_dwEffectID	= 0;
}

CPacketCHAR_SET_EFFECT::~CPacketCHAR_SET_EFFECT()
{
}

void CPacketCHAR_SET_EFFECT::Make(
	DWORD dwCharID,	// [in] キャラID
	DWORD dwEffectID)	// [in] エフェクトID
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)	+
			 sizeof (dwEffectID);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_SET_EFFECT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(pDataTmp, &dwEffectID,	sizeof (dwEffectID),	pDataTmp);	// エフェクトID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketCHAR_SET_EFFECT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(&m_dwEffectID,	pDataTmp, sizeof (m_dwEffectID),	pDataTmp);	// エフェクトID

	pRet = pDataTmp;
	return pRet;
}
