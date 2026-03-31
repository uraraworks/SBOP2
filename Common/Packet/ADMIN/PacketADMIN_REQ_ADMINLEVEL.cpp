/// @file PacketADMIN_REQ_ADMINLEVEL.cpp
/// @brief コマンド(管理者系:管理者権限レベル要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/05
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_REQ_ADMINLEVEL.h"

CPacketADMIN_REQ_ADMINLEVEL::CPacketADMIN_REQ_ADMINLEVEL()
{
	m_dwAccountID = 0;
}

CPacketADMIN_REQ_ADMINLEVEL::~CPacketADMIN_REQ_ADMINLEVEL()
{
}

void CPacketADMIN_REQ_ADMINLEVEL::Make(
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_REQ_ADMINLEVEL;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwAccountID, sizeof (dwAccountID), pDataTmp);	// アカウントID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketADMIN_REQ_ADMINLEVEL::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	// アカウントID

	pRet = pDataTmp;
	return pRet;
}
