/// @file PacketADMIN_REQ_PLAYSOUND.cpp
/// @brief コマンド(管理者系:効果音の再生要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/02
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_REQ_PLAYSOUND.h"

CPacketADMIN_REQ_PLAYSOUND::CPacketADMIN_REQ_PLAYSOUND()
{
	m_dwSoundID = 0;
}

CPacketADMIN_REQ_PLAYSOUND::~CPacketADMIN_REQ_PLAYSOUND()
{
}

void CPacketADMIN_REQ_PLAYSOUND::Make(
	DWORD dwSoundID)	// [in] 効果音ID
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwSoundID);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_REQ_PLAYSOUND;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwSoundID, sizeof (dwSoundID), pDataTmp);	// 効果音ID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketADMIN_REQ_PLAYSOUND::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwSoundID, pDataTmp, sizeof (m_dwSoundID), pDataTmp);	// 効果音ID

	pRet = pDataTmp;
	return pRet;
}
