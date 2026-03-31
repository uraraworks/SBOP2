/// @file PacketMAP_MAPEVENT.cpp
/// @brief コマンド(マップ系:マップイベント情報通知) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/25
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "LibInfoMapEvent.h"
#include "InfoMapEvent/InfoMapEventBase.h"
#include "Command.h"
#include "PacketMAP_MAPEVENT.h"

CPacketMAP_MAPEVENT::CPacketMAP_MAPEVENT()
{
	m_pLibInfo = NULL;
}

CPacketMAP_MAPEVENT::~CPacketMAP_MAPEVENT()
{
	SAFE_DELETE(m_pLibInfo);
}

void CPacketMAP_MAPEVENT::Make(
	DWORD dwMapID,	// [in] マップID
	CLibInfoMapEvent *pLibInfo)	// [in] マップイベント情報
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pLibInfo->GetSendDataSize();
	pInfoDataTmp	= pLibInfo->GetSendData();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwMapID)	+
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_MAPEVENT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwMapID, sizeof (dwMapID), pDataTmp);	// マップID
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// マップイベント情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

void CPacketMAP_MAPEVENT::Make(
	DWORD dwMapID,	// [in] マップID
	CInfoMapEventBase *pInfo)	// [in] マップイベント情報
{
	PCInfoMapEventBase pInfoTmp;
	CLibInfoMapEvent LibInfoMapEvent;

	LibInfoMapEvent.Create();

	pInfoTmp = (PCInfoMapEventBase)LibInfoMapEvent.GetNew(pInfo->m_nType);
	pInfoTmp->Copy(pInfo);
	LibInfoMapEvent.Add(pInfoTmp);

	Make(dwMapID, &LibInfoMapEvent);
}

PBYTE CPacketMAP_MAPEVENT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwMapID, pDataTmp, sizeof (m_dwMapID), pDataTmp);	// マップID

	SAFE_DELETE(m_pLibInfo);
	m_pLibInfo	= new CLibInfoMapEvent;
	m_pLibInfo->Create();
	pDataTmp	= m_pLibInfo->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
