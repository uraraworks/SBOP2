/// @file PacketADMIN_ITEMWEAPON_RENEW.cpp
/// @brief コマンド(アイテム系:武器情報更新) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/08/11
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "InfoItemWeapon.h"
#include "Command.h"
#include "PacketADMIN_ITEMWEAPON_RENEW.h"

CPacketADMIN_ITEMWEAPON_RENEW::CPacketADMIN_ITEMWEAPON_RENEW()
{
	m_pInfo = NULL;
}

CPacketADMIN_ITEMWEAPON_RENEW::~CPacketADMIN_ITEMWEAPON_RENEW()
{
	SAFE_DELETE(m_pInfo);
}

void CPacketADMIN_ITEMWEAPON_RENEW::Make(
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_ITEMWEAPON_RENEW;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// 武器情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketADMIN_ITEMWEAPON_RENEW::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	SAFE_DELETE(m_pInfo);
	m_pInfo	= new CInfoItemWeapon;
	pDataTmp	= m_pInfo->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
