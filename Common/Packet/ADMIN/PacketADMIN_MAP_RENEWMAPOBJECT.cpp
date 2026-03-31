/// @file PacketADMIN_MAP_RENEWMAPOBJECT.cpp
/// @brief コマンド(管理者系:マップオブジェクト更新) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/11/01
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "InfoMapObject.h"
#include "Command.h"
#include "PacketADMIN_MAP_RENEWMAPOBJECT.h"

CPacketADMIN_MAP_RENEWMAPOBJECT::CPacketADMIN_MAP_RENEWMAPOBJECT()
{
	m_pInfoMapObject = NULL;
}

CPacketADMIN_MAP_RENEWMAPOBJECT::~CPacketADMIN_MAP_RENEWMAPOBJECT()
{
	SAFE_DELETE(m_pInfoMapObject);
}

void CPacketADMIN_MAP_RENEWMAPOBJECT::Make(
	CInfoMapObject *pInfo)	// [in] マップオブジェクト情報
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_RENEWMAPOBJECT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// マップオブジェクト情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketADMIN_MAP_RENEWMAPOBJECT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	SAFE_DELETE(m_pInfoMapObject);
	m_pInfoMapObject	= new CInfoMapObject;
	pDataTmp	= m_pInfoMapObject->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
