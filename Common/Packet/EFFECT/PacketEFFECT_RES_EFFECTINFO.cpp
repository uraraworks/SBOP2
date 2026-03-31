/// @file PacketEFFECT_RES_EFFECTINFO.cpp
/// @brief コマンド(エフェクト系:エフェクト情報応答) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/26
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "InfoEffect.h"
#include "Command.h"
#include "PacketEFFECT_RES_EFFECTINFO.h"

CPacketEFFECT_RES_EFFECTINFO::CPacketEFFECT_RES_EFFECTINFO()
{
	m_pInfo = NULL;
}

CPacketEFFECT_RES_EFFECTINFO::~CPacketEFFECT_RES_EFFECTINFO()
{
	SAFE_DELETE(m_pInfo);
}

void CPacketEFFECT_RES_EFFECTINFO::Make(
	CInfoEffect *pInfo)	// [in] エフェクト情報
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_EFFECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_EFFECT_RES_EFFECTINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// エフェクト情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketEFFECT_RES_EFFECTINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	SAFE_DELETE(m_pInfo);
	m_pInfo	= new CInfoEffect;
	pDataTmp	= m_pInfo->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
