/// @file PacketADMIN_SERVER_SAVEINFO.cpp
/// @brief コマンド(管理者系:サーバー情報保存) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/08
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_SERVER_SAVEINFO.h"

CPacketADMIN_SERVER_SAVEINFO::CPacketADMIN_SERVER_SAVEINFO()
{
}

CPacketADMIN_SERVER_SAVEINFO::~CPacketADMIN_SERVER_SAVEINFO()
{
}

void CPacketADMIN_SERVER_SAVEINFO::Make(void)
{
	PBYTE pData;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_SERVER_SAVEINFO;

	RenewPacket(pData, dwSize);
}
