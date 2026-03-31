/// @file PacketADMIN_RENEW_CLIENTVERSION.cpp
/// @brief コマンド(管理者系:クライアントバージョン更新) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/07
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_RENEW_CLIENTVERSION.h"

CPacketADMIN_RENEW_CLIENTVERSION::CPacketADMIN_RENEW_CLIENTVERSION()
{
}

CPacketADMIN_RENEW_CLIENTVERSION::~CPacketADMIN_RENEW_CLIENTVERSION()
{
}

void CPacketADMIN_RENEW_CLIENTVERSION::Make(
	LPCSTR pszClientVersion)	// [in] クライアントバージョン
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			(strlen(pszClientVersion) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_RENEW_CLIENTVERSION;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, (PBYTE)pszClientVersion, strlen(pszClientVersion) + 1, pDataTmp);	// クライアントバージョン

	RenewPacket(pData, dwSize);
}

PBYTE CPacketADMIN_RENEW_CLIENTVERSION::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	StoreRenew(m_strClientVersion, (LPCSTR)pDataTmp, pDataTmp);	// クライアントバージョン

	pRet = pDataTmp;
	return pRet;
}
