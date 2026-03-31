/// @file PacketITEM_RENEWITEMTYPEINFO.cpp
/// @brief コマンド(アイテム系:アイテム種別情報更新) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/10/03
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "LibInfoItemType.h"
#include "Command.h"
#include "PacketITEM_RENEWITEMTYPEINFO.h"

CPacketITEM_RENEWITEMTYPEINFO::CPacketITEM_RENEWITEMTYPEINFO()
{
	m_pInfo = NULL;
}

CPacketITEM_RENEWITEMTYPEINFO::~CPacketITEM_RENEWITEMTYPEINFO()
{
	SAFE_DELETE(m_pInfo);
}

void CPacketITEM_RENEWITEMTYPEINFO::Make(
	CInfoItemTypeBase *pInfo)	// [in] アイテム種別情報
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_RENEWITEMTYPEINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// アイテム種別情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketITEM_RENEWITEMTYPEINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;
	CLibInfoItemType LibInfo;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);
	pDataTmp	= LibInfo.GetNewSendData(pDataTmp, m_pInfo);

	pRet = pDataTmp;
	return pRet;
}
