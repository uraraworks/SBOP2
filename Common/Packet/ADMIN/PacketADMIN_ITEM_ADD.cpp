/// @file PacketADMIN_ITEM_ADD.cpp
/// @brief コマンド(管理者系:アイテム情報追加) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/08/16
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "InfoItem.h"
#include "Command.h"
#include "PacketADMIN_ITEM_ADD.h"

CPacketADMIN_ITEM_ADD::CPacketADMIN_ITEM_ADD()
{
	m_pInfoItem = NULL;
}

CPacketADMIN_ITEM_ADD::~CPacketADMIN_ITEM_ADD()
{
	SAFE_DELETE(m_pInfoItem);
}

void CPacketADMIN_ITEM_ADD::Make(
	CInfoItem *pInfo)	// [in] アイテム情報
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_ITEM_ADD;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// アイテム情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketADMIN_ITEM_ADD::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	SAFE_DELETE(m_pInfoItem);
	m_pInfoItem	= new CInfoItem;
	pDataTmp	= m_pInfoItem->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
