/// @file PacketADMIN_CHAR_RES_ONLINE.cpp
/// @brief コマンド(管理者系:キャラ情報通知) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/01
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_CHAR_RES_ONLINE.h"

CPacketADMIN_CHAR_RES_ONLINE::CPacketADMIN_CHAR_RES_ONLINE()
{
	m_pLibInfo = NULL;
}

CPacketADMIN_CHAR_RES_ONLINE::~CPacketADMIN_CHAR_RES_ONLINE()
{
	SAFE_DELETE(m_pLibInfo);
}

void CPacketADMIN_CHAR_RES_ONLINE::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_RES_ONLINE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	// キャラ情報

	RenewPacket(pData, dwSize);

	SAFE_DELETE_ARRAY(pInfoDataTmp);
}

PBYTE CPacketADMIN_CHAR_RES_ONLINE::Set(PBYTE pPacket)
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
