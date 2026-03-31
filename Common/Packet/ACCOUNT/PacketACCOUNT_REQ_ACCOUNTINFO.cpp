/// @file PacketACCOUNT_REQ_ACCOUNTINFO.cpp
/// @brief コマンド(アカウント系:アカウント情報要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/06
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "Command.h"
#include "PacketACCOUNT_REQ_ACCOUNTINFO.h"

CPacketACCOUNT_REQ_ACCOUNTINFO::CPacketACCOUNT_REQ_ACCOUNTINFO()
{
	m_dwAccountID = 0;
}

CPacketACCOUNT_REQ_ACCOUNTINFO::~CPacketACCOUNT_REQ_ACCOUNTINFO()
{
}

void CPacketACCOUNT_REQ_ACCOUNTINFO::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ACCOUNT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ACCOUNT_REQ_ACCOUNTINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwAccountID, sizeof (dwAccountID), pDataTmp);	// アカウントID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketACCOUNT_REQ_ACCOUNTINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	// アカウントID

	pRet = pDataTmp;
	return pRet;
}
