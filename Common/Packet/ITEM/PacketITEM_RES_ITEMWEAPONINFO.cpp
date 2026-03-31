/// @file PacketITEM_RES_ITEMWEAPONINFO.cpp
/// @brief コマンド(アイテム系:武器情報応答) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/08/11
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "InfoItemWeapon.h"
#include "Command.h"
#include "PacketITEM_RES_ITEMWEAPONINFO.h"

CPacketITEM_RES_ITEMWEAPONINFO::CPacketITEM_RES_ITEMWEAPONINFO()
{
	m_pInfo = NULL;
}

CPacketITEM_RES_ITEMWEAPONINFO::~CPacketITEM_RES_ITEMWEAPONINFO()
{
	SAFE_DELETE(m_pInfo);
}

void CPacketITEM_RES_ITEMWEAPONINFO::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ITEM;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_RES_ITEMWEAPONINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// 武器情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketITEM_RES_ITEMWEAPONINFO::Set(PBYTE pPacket)
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
