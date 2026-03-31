/// @file PacketACCOUNT_RES_ACCOUNTINFO.cpp
/// @brief コマンド(アカウント系:アカウント情報応答) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/07
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "InfoAccount.h"
#include "Command.h"
#include "PacketACCOUNT_RES_ACCOUNTINFO.h"

CPacketACCOUNT_RES_ACCOUNTINFO::CPacketACCOUNT_RES_ACCOUNTINFO()
{
	m_pInfo = NULL;
}

CPacketACCOUNT_RES_ACCOUNTINFO::~CPacketACCOUNT_RES_ACCOUNTINFO()
{
	SAFE_DELETE(m_pInfo);
}

void CPacketACCOUNT_RES_ACCOUNTINFO::Make(
	CInfoAccount *pInfo)	// [in] アカウント情報
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ACCOUNT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ACCOUNT_RES_ACCOUNTINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// アカウント情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketACCOUNT_RES_ACCOUNTINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	SAFE_DELETE(m_pInfo);
	m_pInfo	= new CInfoAccount;
	pDataTmp	= m_pInfo->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
