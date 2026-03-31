/// @file PacketSYSTEM_INFO.cpp
/// @brief コマンド(システム系:システム情報通知) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/09/29
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Command.h"
#include "InfoSystem.h"
#include "PacketSYSTEM_INFO.h"

CPacketSYSTEM_INFO::CPacketSYSTEM_INFO()
{
	m_pInfoSystem = NULL;
}

CPacketSYSTEM_INFO::~CPacketSYSTEM_INFO()
{
	SAFE_DELETE(m_pInfoSystem);
}

void CPacketSYSTEM_INFO::Make(
	CInfoSystem *pInfoSystem)	// [in] システム情報
{
	PBYTE pData, pDataTmp;
	DWORD dwSize, dwSizeInfoSystem;
	PBYTE pDataInfoSystem;
	PPACKETBASE pPacketBase;

	dwSizeInfoSystem = pInfoSystem->GetSendDataSize();
	pDataInfoSystem	 = pInfoSystem->GetSendData();

	dwSize = sizeof (PACKETBASE);
	dwSize += dwSizeInfoSystem;

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_SYSTEM;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_SYSTEM_INFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pDataInfoSystem, dwSizeInfoSystem, pDataTmp);	// システム情報

	RenewPacket(pData, dwSize);
}

PBYTE CPacketSYSTEM_INFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	SAFE_DELETE(m_pInfoSystem);
	m_pInfoSystem = new CInfoSystem;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);
	pDataTmp	= m_pInfoSystem->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
