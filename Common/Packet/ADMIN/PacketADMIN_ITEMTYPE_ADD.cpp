/// @file PacketADMIN_ITEMTYPE_ADD.cpp
/// @brief コマンド(管理者系:アイテム種別情報追加) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/10/01
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "LibInfoItemType.h"
#include "Command.h"
#include "PacketADMIN_ITEMTYPE_ADD.h"

CPacketADMIN_ITEMTYPE_ADD::CPacketADMIN_ITEMTYPE_ADD()
{
	m_pInfoItem = NULL;
}

CPacketADMIN_ITEMTYPE_ADD::~CPacketADMIN_ITEMTYPE_ADD()
{
	SAFE_DELETE(m_pInfoItem);
}

void CPacketADMIN_ITEMTYPE_ADD::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_ITEMTYPE_ADD;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// アイテム種別情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketADMIN_ITEMTYPE_ADD::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;
	CLibInfoItemType LibInfo;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);
	pDataTmp	= LibInfo.GetNewSendData(pDataTmp, m_pInfoItem);

	pRet = pDataTmp;
	return pRet;
}
