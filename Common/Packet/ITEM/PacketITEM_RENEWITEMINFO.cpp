/// @file PacketITEM_RENEWITEMINFO.cpp
/// @brief コマンド(アイテム系:アイテム情報更新) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/08/19
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "InfoItem.h"
#include "Command.h"
#include "PacketITEM_RENEWITEMINFO.h"

CPacketITEM_RENEWITEMINFO::CPacketITEM_RENEWITEMINFO()
{
	m_pInfo = NULL;
}

CPacketITEM_RENEWITEMINFO::~CPacketITEM_RENEWITEMINFO()
{
	SAFE_DELETE(m_pInfo);
}

void CPacketITEM_RENEWITEMINFO::Make(
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_RENEWITEMINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// アイテム情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketITEM_RENEWITEMINFO::Set(PBYTE pPacket)
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
