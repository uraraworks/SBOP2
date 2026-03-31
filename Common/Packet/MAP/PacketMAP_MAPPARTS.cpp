/// @file PacketMAP_MAPPARTS.cpp
/// @brief コマンド(マップ系:マップパーツ情報通知) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/30
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "LibInfoMapParts.h"
#include "InfoMapParts.h"
#include "Command.h"
#include "PacketMAP_MAPPARTS.h"

CPacketMAP_MAPPARTS::CPacketMAP_MAPPARTS()
{
	m_pLibInfoMapParts = NULL;
}

CPacketMAP_MAPPARTS::~CPacketMAP_MAPPARTS()
{
	SAFE_DELETE(m_pLibInfoMapParts);
}

void CPacketMAP_MAPPARTS::Make(
	CLibInfoMapParts *pLibInfo)	// [in] マップパーツ情報
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pLibInfo->GetSendDataSize();
	pInfoDataTmp	= pLibInfo->GetSendData();

	dwSize = sizeof (PACKETBASE) +
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_MAPPARTS;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// マップパーツ情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

void CPacketMAP_MAPPARTS::Make(
	CInfoMapParts *pInfo)	// [in] マップパーツ情報
{
	PCInfoMapParts pInfoTmp;
	CLibInfoMapParts LibInfoMapParts;

	LibInfoMapParts.Create();

	pInfoTmp = (PCInfoMapParts)LibInfoMapParts.GetNew();
	pInfoTmp->Copy(pInfo);
	LibInfoMapParts.Add(pInfoTmp);

	Make(&LibInfoMapParts);
}

PBYTE CPacketMAP_MAPPARTS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	SAFE_DELETE(m_pLibInfoMapParts);
	m_pLibInfoMapParts	= new CLibInfoMapParts;
	m_pLibInfoMapParts->Create();
	pDataTmp	= m_pLibInfoMapParts->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
