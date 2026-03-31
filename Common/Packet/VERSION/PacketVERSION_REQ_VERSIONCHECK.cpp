/// @file PacketVERSION_REQ_VERSIONCHECK.cpp
/// @brief コマンド(バージョン系:バージョンチェック要求) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/05
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "Command.h"
#include "PacketVERSION_REQ_VERSIONCHECK.h"

CPacketVERSION_REQ_VERSIONCHECK::CPacketVERSION_REQ_VERSIONCHECK()
{
	m_dwVersion = 0;
}

CPacketVERSION_REQ_VERSIONCHECK::~CPacketVERSION_REQ_VERSIONCHECK()
{
}

void CPacketVERSION_REQ_VERSIONCHECK::Make(
	DWORD dwVersion)	// [in] バージョンコード
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwVersion);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_REQ_VERSIONCHECK;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwVersion, sizeof (dwVersion), pDataTmp);	// バージョンコード

	RenewPacket(pData, dwSize);
}

PBYTE CPacketVERSION_REQ_VERSIONCHECK::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwVersion, pDataTmp, sizeof (m_dwVersion), pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
