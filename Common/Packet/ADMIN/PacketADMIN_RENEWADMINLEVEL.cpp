/// @file PacketADMIN_RENEWADMINLEVEL.cpp
/// @brief コマンド(管理者系:管理者権限レベル更新) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/05
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_RENEWADMINLEVEL.h"

CPacketADMIN_RENEWADMINLEVEL::CPacketADMIN_RENEWADMINLEVEL()
{
	m_dwAccountID = 0;
	m_nAdminLevel = 0;
}

CPacketADMIN_RENEWADMINLEVEL::~CPacketADMIN_RENEWADMINLEVEL()
{
}

void CPacketADMIN_RENEWADMINLEVEL::Make(
	DWORD dwAccountID,	// [in] アカウントID
	int nLevel)	// [in] 管理者レベル
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)  +
			 sizeof (dwAccountID) +
			 sizeof (nLevel);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_RENEWADMINLEVEL;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwAccountID,	sizeof (dwAccountID),	pDataTmp);	// アカウントID
	CopyMemoryRenew(pDataTmp, &nLevel,	sizeof (nLevel),	pDataTmp);	// 管理者レベル

	RenewPacket(pData, dwSize);
}

PBYTE CPacketADMIN_RENEWADMINLEVEL::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	// アカウントID
	CopyMemoryRenew(&m_nAdminLevel, pDataTmp, sizeof (m_nAdminLevel), pDataTmp);	// 管理者レベル

	pRet = pDataTmp;
	return pRet;
}
