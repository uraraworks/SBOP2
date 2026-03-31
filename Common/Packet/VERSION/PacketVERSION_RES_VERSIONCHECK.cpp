/// @file PacketVERSION_RES_VERSIONCHECK.cpp
/// @brief コマンド(バージョン系:バージョンチェック応答) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/05
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "Command.h"
#include "PacketVERSION_RES_VERSIONCHECK.h"

CPacketVERSION_RES_VERSIONCHECK::CPacketVERSION_RES_VERSIONCHECK()
{
	m_nResult = VERSIONCHECKRES_NONE;
}

CPacketVERSION_RES_VERSIONCHECK::~CPacketVERSION_RES_VERSIONCHECK()
{
}

void CPacketVERSION_RES_VERSIONCHECK::Make(
	int nResult)	// [in] 結果
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (nResult);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_RES_VERSIONCHECK;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &nResult, sizeof (nResult), pDataTmp);	// 結果

	RenewPacket(pData, dwSize);
}

PBYTE CPacketVERSION_RES_VERSIONCHECK::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_nResult, pDataTmp, sizeof (m_nResult), pDataTmp);

	pRet = pDataTmp;
	return pRet;
}
