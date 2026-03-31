/// @file PacketMAP_RES_MAPINFO.cpp
/// @brief コマンド(マップ系:マップ情報応答) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/01/04
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "InfoMapBase.h"
#include "Command.h"
#include "PacketMAP_RES_MAPINFO.h"

CPacketMAP_RES_MAPINFO::CPacketMAP_RES_MAPINFO()
{
	m_pInfo = NULL;
}

CPacketMAP_RES_MAPINFO::~CPacketMAP_RES_MAPINFO()
{
	SAFE_DELETE(m_pInfo);
}

void CPacketMAP_RES_MAPINFO::Make(
	CInfoMapBase *pInfo)	// [in] マップ情報
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_RES_MAPINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// マップ情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketMAP_RES_MAPINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	SAFE_DELETE(m_pInfo);
	m_pInfo	= new CInfoMapBase;
	pDataTmp	= m_pInfo->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
