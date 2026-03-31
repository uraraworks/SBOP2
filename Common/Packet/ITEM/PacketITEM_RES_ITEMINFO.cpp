/// @file PacketITEM_RES_ITEMINFO.cpp
/// @brief コマンド(アイテム系:アイテム情報応答) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/05/05
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "InfoItem.h"
#include "Command.h"
#include "PacketITEM_RES_ITEMINFO.h"

CPacketITEM_RES_ITEMINFO::CPacketITEM_RES_ITEMINFO()
{
	m_pInfo = NULL;
}

CPacketITEM_RES_ITEMINFO::~CPacketITEM_RES_ITEMINFO()
{
	SAFE_DELETE(m_pInfo);
}

void CPacketITEM_RES_ITEMINFO::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ITEM;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_RES_ITEMINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// アイテム情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketITEM_RES_ITEMINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	SAFE_DELETE(m_pInfo);
	m_pInfo	= new CInfoItem;
	pDataTmp	= m_pInfo->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
