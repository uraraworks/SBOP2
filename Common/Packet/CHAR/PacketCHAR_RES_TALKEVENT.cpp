/// @file PacketCHAR_RES_TALKEVENT.cpp
/// @brief コマンド(会話イベント系:会話イベント情報応答) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/27
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "InfoTalkEvent.h"
#include "Command.h"
#include "PacketCHAR_RES_TALKEVENT.h"

CPacketCHAR_RES_TALKEVENT::CPacketCHAR_RES_TALKEVENT()
{
	m_dwParam	= 0;
	m_pInfo	= NULL;
}

CPacketCHAR_RES_TALKEVENT::~CPacketCHAR_RES_TALKEVENT()
{
	SAFE_DELETE(m_pInfo);
}

void CPacketCHAR_RES_TALKEVENT::Make(
	CInfoTalkEvent *pInfo,	// [in] 会話イベント情報
	DWORD dwParam)	// [in] パラメータ
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize();
	pInfoDataTmp	= pInfo->GetSendData();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwParam)	+
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_RES_TALKEVENT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwParam,	sizeof (dwParam),	pDataTmp);	// パラメータ
	CopyMemoryRenew(pDataTmp, pInfoDataTmp,	dwSizeDataTmp,	pDataTmp);	// 会話イベント情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketCHAR_RES_TALKEVENT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwParam, pDataTmp, sizeof (m_dwParam), pDataTmp);	// パラメータ

	SAFE_DELETE(m_pInfo);
	m_pInfo	= new CInfoTalkEvent;
	pDataTmp	= m_pInfo->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
