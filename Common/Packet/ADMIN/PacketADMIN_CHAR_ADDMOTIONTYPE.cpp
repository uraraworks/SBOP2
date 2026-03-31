/// @file PacketADMIN_CHAR_ADDMOTIONTYPE.cpp
/// @brief コマンド(管理者系:キャラモーション種別情報の追加) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/09
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_CHAR_ADDMOTIONTYPE.h"

CPacketADMIN_CHAR_ADDMOTIONTYPE::CPacketADMIN_CHAR_ADDMOTIONTYPE()
{
}

CPacketADMIN_CHAR_ADDMOTIONTYPE::~CPacketADMIN_CHAR_ADDMOTIONTYPE()
{
}

void CPacketADMIN_CHAR_ADDMOTIONTYPE::Make(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_ADDMOTIONTYPE;

	pDataTmp = (PBYTE)(pPacketBase + 1);

	RenewPacket(pData, dwSize);
}

PBYTE CPacketADMIN_CHAR_ADDMOTIONTYPE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	pRet = pDataTmp;
	return pRet;
}
