/// @file PacketADMIN_ITEMWEAPON_ADD.cpp
/// @brief コマンド(管理者系:武器情報追加) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/08/11
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "LibInfoItemWeapon.h"
#include "Command.h"
#include "PacketADMIN_ITEMWEAPON_ADD.h"

CPacketADMIN_ITEMWEAPON_ADD::CPacketADMIN_ITEMWEAPON_ADD()
{
	m_pInfo = NULL;
}

CPacketADMIN_ITEMWEAPON_ADD::~CPacketADMIN_ITEMWEAPON_ADD()
{
	SAFE_DELETE(m_pInfo);
}

void CPacketADMIN_ITEMWEAPON_ADD::Make(
	CInfoItemWeapon *pInfo)	// [in] 武器情報
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize();
	pInfoDataTmp	= pInfo->GetSendData();

	dwSize = sizeof (PACKETBASE) +
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_ITEMWEAPON_ADD;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// 武器情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketADMIN_ITEMWEAPON_ADD::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;
	CLibInfoItemWeapon LibInfo;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);
	pDataTmp	= LibInfo.GetNewSendData(pDataTmp, m_pInfo);

	pRet = pDataTmp;
	return pRet;
}
