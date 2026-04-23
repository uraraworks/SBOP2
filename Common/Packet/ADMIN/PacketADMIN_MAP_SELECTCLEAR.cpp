/// @file PacketADMIN_MAP_SELECTCLEAR.cpp
/// @brief コマンド(管理者系:マップ選択解除) 実装ファイル

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_MAP_SELECTCLEAR.h"

CPacketADMIN_MAP_SELECTCLEAR::CPacketADMIN_MAP_SELECTCLEAR()
{
}

CPacketADMIN_MAP_SELECTCLEAR::~CPacketADMIN_MAP_SELECTCLEAR()
{
}

void CPacketADMIN_MAP_SELECTCLEAR::Make(void)
{
	PBYTE pData;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain = SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub  = SBOCOMMANDID_SUB_ADMIN_MAP_SELECTCLEAR;

	RenewPacket(pData, dwSize);
}

PBYTE CPacketADMIN_MAP_SELECTCLEAR::Set(PBYTE pPacket)
{
	PBYTE pDataTmp;

	pDataTmp = CPacketBase::Set(pPacket);
	// ペイロードなし
	return pDataTmp;
}
