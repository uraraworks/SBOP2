/// @file PacketADMIN_RENEWMAPSHADOW.cpp
/// @brief コマンド(管理者系:マップ影更新) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/06
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "InfoMapShadow.h"
#include "Command.h"
#include "PacketADMIN_RENEWMAPSHADOW.h"

CPacketADMIN_RENEWMAPSHADOW::CPacketADMIN_RENEWMAPSHADOW()
{
	m_pInfoMapShadow = NULL;
}

CPacketADMIN_RENEWMAPSHADOW::~CPacketADMIN_RENEWMAPSHADOW()
{
	SAFE_DELETE(m_pInfoMapShadow);
}

void CPacketADMIN_RENEWMAPSHADOW::Make(
	CInfoMapShadow *pInfo)	// [in] マップ影情報
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_RENEWMAPSHADOW;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// マップ影情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketADMIN_RENEWMAPSHADOW::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	SAFE_DELETE(m_pInfoMapShadow);
	m_pInfoMapShadow	= new CInfoMapShadow;
	pDataTmp	= m_pInfoMapShadow->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
