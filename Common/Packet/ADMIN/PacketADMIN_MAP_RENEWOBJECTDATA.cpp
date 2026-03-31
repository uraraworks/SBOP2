/// @file PacketADMIN_MAP_RENEWOBJECTDATA.cpp
/// @brief コマンド(管理者系:マップオブジェクト配置データ通知) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/11/03
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "LibInfoMapObjectData.h"
#include "Command.h"
#include "PacketADMIN_MAP_RENEWOBJECTDATA.h"

CPacketADMIN_MAP_RENEWOBJECTDATA::CPacketADMIN_MAP_RENEWOBJECTDATA()
{
	m_dwMapID	= 0;
	m_pInfo	= NULL;
}

CPacketADMIN_MAP_RENEWOBJECTDATA::~CPacketADMIN_MAP_RENEWOBJECTDATA()
{
	SAFE_DELETE(m_pInfo);
}

void CPacketADMIN_MAP_RENEWOBJECTDATA::Make(
	DWORD dwMapID,	// [in] マップID
	CInfoMapObjectData *pInfo)	// [in] 配置データ
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize();
	pInfoDataTmp	= pInfo->GetSendData();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwMapID)	+
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_RENEWOBJECTDATA;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwMapID,	sizeof (dwMapID),	pDataTmp);	// マップID
	CopyMemoryRenew(pDataTmp, pInfoDataTmp,	dwSizeDataTmp,	pDataTmp);	// 配置データ

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketADMIN_MAP_RENEWOBJECTDATA::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp, pDataTmpBack;
	CLibInfoMapObjectData LibInfo;
	CInfoMapObjectData InfoTmp;

	pRet	 = pPacket;
	pDataTmp	 = CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwMapID, pDataTmp, sizeof (m_dwMapID), pDataTmp);	// マップID
	pDataTmpBack = pDataTmp;
	InfoTmp.SetSendData(pDataTmp);

	SAFE_DELETE(m_pInfo);
	m_pInfo	= (PCInfoMapObjectData)LibInfo.GetNew();
	pDataTmp	= m_pInfo->SetSendData(pDataTmpBack);

	pRet = pDataTmp;
	return pRet;
}
