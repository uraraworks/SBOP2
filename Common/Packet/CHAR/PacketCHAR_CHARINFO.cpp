/// @file PacketCHAR_CHARINFO.cpp
/// @brief コマンド(キャラ系:キャラ情報通知) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/01/27
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_CHARINFO.h"

CPacketCHAR_CHARINFO::CPacketCHAR_CHARINFO()
{
	m_pLibInfo = NULL;
}

CPacketCHAR_CHARINFO::~CPacketCHAR_CHARINFO()
{
	SAFE_DELETE(m_pLibInfo);
}

void CPacketCHAR_CHARINFO::Make(
	PCLibInfoCharBase pInfo)	// [in] キャラ情報
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_CHARINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// キャラ情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketCHAR_CHARINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	SAFE_DELETE(m_pLibInfo);
	m_pLibInfo	= new CLibInfoCharBase;
	m_pLibInfo->Create();
	pDataTmp	= m_pLibInfo->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
