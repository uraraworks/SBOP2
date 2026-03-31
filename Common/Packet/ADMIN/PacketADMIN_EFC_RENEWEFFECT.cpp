/// @file PacketADMIN_EFC_RENEWEFFECT.cpp
/// @brief コマンド(管理者系:エフェクト情報の更新) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/07/06
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "InfoEffect.h"
#include "Command.h"
#include "PacketADMIN_EFC_RENEWEFFECT.h"

CPacketADMIN_EFC_RENEWEFFECT::CPacketADMIN_EFC_RENEWEFFECT()
{
	m_dwEffectID	= 0;
	m_pInfo	= NULL;
}

CPacketADMIN_EFC_RENEWEFFECT::~CPacketADMIN_EFC_RENEWEFFECT()
{
	SAFE_DELETE(m_pInfo);
}

void CPacketADMIN_EFC_RENEWEFFECT::Make(
	DWORD dwEffectID,	// [in] エフェクトID
	CInfoEffect *pSrc)	// [in] エフェクト情報
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pSrc->GetSendDataSize();
	pInfoDataTmp	= pSrc->GetSendData();

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwEffectID) +
			 dwSizeDataTmp;

	pData = ZeroNew(dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_EFC_RENEWEFFECT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwEffectID,	sizeof (dwEffectID),	pDataTmp);	// エフェクトID
	CopyMemoryRenew(pDataTmp, pInfoDataTmp,	dwSizeDataTmp,	 	pDataTmp);	// エフェクト情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketADMIN_EFC_RENEWEFFECT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwEffectID, pDataTmp, sizeof (m_dwEffectID), pDataTmp);	// エフェクトID

	SAFE_DELETE(m_pInfo);
	m_pInfo	= new CInfoEffect;
	pDataTmp	= m_pInfo->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
