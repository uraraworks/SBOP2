/// @file PacketMAP_MAPSHADOW.cpp
/// @brief コマンド(マップ系:マップ影情報通知) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/05
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "LibInfoMapShadow.h"
#include "InfoMapShadow.h"
#include "Command.h"
#include "PacketMAP_MAPSHADOW.h"

CPacketMAP_MAPSHADOW::CPacketMAP_MAPSHADOW()
{
	m_pLibInfoMapShadow = NULL;
}

CPacketMAP_MAPSHADOW::~CPacketMAP_MAPSHADOW()
{
	SAFE_DELETE(m_pLibInfoMapShadow);
}

void CPacketMAP_MAPSHADOW::Make(
	CLibInfoMapShadow *pLibInfo)	// [in] マップ影情報
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_MAPSHADOW;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// マップ影情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

void CPacketMAP_MAPSHADOW::Make(
	CInfoMapShadow *pInfo)	// [in] マップ影情報
{
	PCInfoMapShadow pInfoTmp;
	CLibInfoMapShadow LibInfoMapShadow;

	LibInfoMapShadow.Create();

	pInfoTmp = (PCInfoMapShadow)LibInfoMapShadow.GetNew();
	pInfoTmp->Copy(pInfo);
	LibInfoMapShadow.Add(pInfoTmp);

	Make(&LibInfoMapShadow);
}

PBYTE CPacketMAP_MAPSHADOW::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	SAFE_DELETE(m_pLibInfoMapShadow);
	m_pLibInfoMapShadow	= new CLibInfoMapShadow;
	m_pLibInfoMapShadow->Create();
	pDataTmp	= m_pLibInfoMapShadow->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
