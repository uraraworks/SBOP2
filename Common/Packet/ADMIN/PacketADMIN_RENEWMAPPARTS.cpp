/// @file PacketADMIN_RENEWMAPPARTS.cpp
/// @brief コマンド(管理者系:マップパーツ更新) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/29
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "InfoMapParts.h"
#include "Command.h"
#include "PacketADMIN_RENEWMAPPARTS.h"

CPacketADMIN_RENEWMAPPARTS::CPacketADMIN_RENEWMAPPARTS()
{
	m_pInfoMapParts = NULL;
}

CPacketADMIN_RENEWMAPPARTS::~CPacketADMIN_RENEWMAPPARTS()
{
	SAFE_DELETE(m_pInfoMapParts);
}

void CPacketADMIN_RENEWMAPPARTS::Make(
	CInfoMapParts *pInfo)	// [in] マップパーツ情報
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_RENEWMAPPARTS;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// マップパーツ情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketADMIN_RENEWMAPPARTS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	SAFE_DELETE(m_pInfoMapParts);
	m_pInfoMapParts	= new CInfoMapParts;
	pDataTmp	= m_pInfoMapParts->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
