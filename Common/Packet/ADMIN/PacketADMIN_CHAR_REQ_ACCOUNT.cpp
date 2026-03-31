/// @file PacketADMIN_CHAR_REQ_ACCOUNT.cpp
/// @brief コマンド(管理者系:アカウント情報要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/07
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_CHAR_REQ_ACCOUNT.h"

CPacketADMIN_CHAR_REQ_ACCOUNT::CPacketADMIN_CHAR_REQ_ACCOUNT()
{
	m_dwAccountID = 0;
}

CPacketADMIN_CHAR_REQ_ACCOUNT::~CPacketADMIN_CHAR_REQ_ACCOUNT()
{
}

void CPacketADMIN_CHAR_REQ_ACCOUNT::Make(
	DWORD dwAccountID)	// [in] アカウントID
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwAccountID);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_REQ_ACCOUNT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwAccountID, sizeof (dwAccountID), pDataTmp);	// アカウントID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketADMIN_CHAR_REQ_ACCOUNT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	// アカウントID

	pRet = pDataTmp;
	return pRet;
}
