/// @file PacketMAP_MAPOBJECT.cpp
/// @brief コマンド(マップ系:マップオブジェクト情報通知) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/11/01
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "LibInfoMapObject.h"
#include "InfoMapObject.h"
#include "Command.h"
#include "PacketMAP_MAPOBJECT.h"

CPacketMAP_MAPOBJECT::CPacketMAP_MAPOBJECT()
{
	m_pLibInfoMapObject = NULL;
}

CPacketMAP_MAPOBJECT::~CPacketMAP_MAPOBJECT()
{
	SAFE_DELETE(m_pLibInfoMapObject);
}

void CPacketMAP_MAPOBJECT::Make(
	CLibInfoMapObject *pLibInfo)	// [in] マップオブジェクト情報
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_MAPOBJECT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// マップオブジェクト情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

void CPacketMAP_MAPOBJECT::Make(
	CInfoMapObject *pInfo)	// [in] マップオブジェクト情報
{
	PCInfoMapObject pInfoTmp;
	CLibInfoMapObject LibInfoMapObject;

	LibInfoMapObject.Create();

	pInfoTmp = (PCInfoMapObject)LibInfoMapObject.GetNew();
	pInfoTmp->Copy(pInfo);
	LibInfoMapObject.Add(pInfoTmp);

	Make(&LibInfoMapObject);
}

PBYTE CPacketMAP_MAPOBJECT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	SAFE_DELETE(m_pLibInfoMapObject);
	m_pLibInfoMapObject	= new CLibInfoMapObject;
	m_pLibInfoMapObject->Create();
	pDataTmp	= m_pLibInfoMapObject->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
