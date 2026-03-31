/// @file PacketADMIN_SYSTEM_RENEWINFO.cpp
/// @brief コマンド(管理者系:システム情報の更新) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/10/04
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "Command.h"
#include "InfoSystem.h"
#include "PacketADMIN_SYSTEM_RENEWINFO.h"

CPacketADMIN_SYSTEM_RENEWINFO::CPacketADMIN_SYSTEM_RENEWINFO()
{
	m_pInfoSystem = NULL;
}

CPacketADMIN_SYSTEM_RENEWINFO::~CPacketADMIN_SYSTEM_RENEWINFO()
{
	SAFE_DELETE(m_pInfoSystem);
}

void CPacketADMIN_SYSTEM_RENEWINFO::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_SYSTEM_RENEWINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pDataInfoSystem, dwSizeInfoSystem, pDataTmp);	// システム情報

	RenewPacket(pData, dwSize);
}

PBYTE CPacketADMIN_SYSTEM_RENEWINFO::Set(PBYTE pPacket)
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
