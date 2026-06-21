/// @file PacketACCOUNT_REQ_DELETECHAR.cpp
/// @brief コマンド(アカウント系:キャラ削除要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/14
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "Command.h"
#include "PacketACCOUNT_REQ_DELETECHAR.h"

CPacketACCOUNT_REQ_DELETECHAR::CPacketACCOUNT_REQ_DELETECHAR()
{
	m_dwAccountID	= 0;
	m_dwCharID	= 0;
}

CPacketACCOUNT_REQ_DELETECHAR::~CPacketACCOUNT_REQ_DELETECHAR()
{
}

void CPacketACCOUNT_REQ_DELETECHAR::Make(
	DWORD dwAccountID,	// [in] アカウントID
	DWORD dwCharID)	// [in] キャラID
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwAccountID)	+
			 sizeof (dwCharID);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ACCOUNT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ACCOUNT_REQ_DELETECHAR;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwAccountID,	sizeof (dwAccountID),	pDataTmp);	// アカウントID
	CopyMemoryRenew(pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	// キャラID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketACCOUNT_REQ_DELETECHAR::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwAccountID,	pDataTmp, sizeof (m_dwAccountID),	pDataTmp);	// アカウントID
	CopyMemoryRenew(&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	// キャラID

	pRet = pDataTmp;
	return pRet;
}
