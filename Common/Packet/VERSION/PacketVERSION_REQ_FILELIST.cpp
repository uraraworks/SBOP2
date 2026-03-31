/// @file PacketVERSION_REQ_FILELIST.cpp
/// @brief コマンド(バージョン系:ファイルリスト要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/03/08
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Command.h"
#include "PacketVERSION_REQ_FILELIST.h"

CPacketVERSION_REQ_FILELIST::CPacketVERSION_REQ_FILELIST()
{
}

CPacketVERSION_REQ_FILELIST::~CPacketVERSION_REQ_FILELIST()
{
}

void CPacketVERSION_REQ_FILELIST::Make(void)
{
	PBYTE pData;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE);

	pData = ZeroNew(dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_REQ_FILELIST;

	RenewPacket(pData, dwSize);
}
