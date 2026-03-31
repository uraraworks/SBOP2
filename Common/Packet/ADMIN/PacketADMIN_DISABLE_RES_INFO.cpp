/// @file PacketADMIN_DISABLE_RES_INFO.cpp
/// @brief コマンド(管理者系:拒否情報応答) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/04/11
/// @copyright Copyright(C)URARA-works 2009

#include "StdAfx.h"
#include "Command.h"
#include "LibInfoDisable.h"
#include "PacketADMIN_DISABLE_RES_INFO.h"

CPacketADMIN_DISABLE_RES_INFO::CPacketADMIN_DISABLE_RES_INFO()
{
	m_pLibInfoDisable = NULL;
}

CPacketADMIN_DISABLE_RES_INFO::~CPacketADMIN_DISABLE_RES_INFO()
{
	SAFE_DELETE(m_pLibInfoDisable);
}

void CPacketADMIN_DISABLE_RES_INFO::Make(
	CLibInfoDisable *pLibInfoDisable)	// [in] 拒否情報
{
	PBYTE pData, pDataTmp;
	DWORD dwSize, dwSizeLibInfoDisable;
	PBYTE pDataLibInfoDisable;
	PPACKETBASE pPacketBase;

	dwSizeLibInfoDisable = pLibInfoDisable->GetSendDataSize();
	pDataLibInfoDisable	 = pLibInfoDisable->GetSendData();

	dwSize = sizeof (PACKETBASE);
	dwSize += dwSizeLibInfoDisable;

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_DISABLE_RES_INFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, pDataLibInfoDisable, dwSizeLibInfoDisable, pDataTmp);	// 拒否情報

	RenewPacket(pData, dwSize);
}

PBYTE CPacketADMIN_DISABLE_RES_INFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	SAFE_DELETE(m_pLibInfoDisable);
	m_pLibInfoDisable = new CLibInfoDisable;
	m_pLibInfoDisable->Create();

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);
	pDataTmp	= m_pLibInfoDisable->SetSendData(pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
