/// @file PacketADMIN_EFC_RENEWBALLOON.cpp
/// @brief コマンド(管理者系:噴出し情報の更新) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/12/30
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "LibInfoEfcBalloon.h"
#include "Command.h"
#include "PacketADMIN_EFC_RENEWBALLOON.h"

CPacketADMIN_EFC_RENEWBALLOON::CPacketADMIN_EFC_RENEWBALLOON()
{
	m_dwListID	= 0;
	m_pInfo	= NULL;
}

CPacketADMIN_EFC_RENEWBALLOON::~CPacketADMIN_EFC_RENEWBALLOON()
{
	SAFE_DELETE(m_pInfo);
}

void CPacketADMIN_EFC_RENEWBALLOON::Make(
	DWORD dwListID,	// [in] 種別ID
	CLibInfoEfcBalloon *pSrc)	// [in] 噴出し情報
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pSrc->GetSendDataSize(dwListID);
	pInfoDataTmp	= pSrc->GetSendData(dwListID);

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwListID) +
			 dwSizeDataTmp;

	pData = ZeroNew(dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_EFC_RENEWBALLOON;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwListID,	sizeof (dwListID),	pDataTmp);	// 種別ID
	CopyMemoryRenew(pDataTmp, pInfoDataTmp,	dwSizeDataTmp,	 	pDataTmp);	// 噴出し情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketADMIN_EFC_RENEWBALLOON::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwListID, pDataTmp, sizeof (m_dwListID), pDataTmp);	// 種別ID

	SAFE_DELETE(m_pInfo);
	m_pInfo	= new CLibInfoEfcBalloon;
	m_pInfo->Create();
	pDataTmp	= m_pInfo->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
