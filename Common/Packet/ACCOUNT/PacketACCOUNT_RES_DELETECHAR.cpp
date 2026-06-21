/// @file PacketACCOUNT_RES_DELETECHAR.cpp
/// @brief コマンド(アカウント系:キャラ削除応答) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/14
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "Command.h"
#include "PacketACCOUNT_RES_DELETECHAR.h"

CPacketACCOUNT_RES_DELETECHAR::CPacketACCOUNT_RES_DELETECHAR()
{
	m_nResult	= DELETECHARRES_NONE;
	m_dwCharID	= 0;
}

CPacketACCOUNT_RES_DELETECHAR::~CPacketACCOUNT_RES_DELETECHAR()
{
}

void CPacketACCOUNT_RES_DELETECHAR::Make(
	int nResult,	// [in] 結果
	DWORD dwCharID)	// [in] キャラID
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (nResult)	+
			 sizeof (dwCharID);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ACCOUNT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ACCOUNT_RES_DELETECHAR;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &nResult,	sizeof (nResult),	pDataTmp);	// 結果
	CopyMemoryRenew(pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	// キャラID

	RenewPacket(pData, dwSize);
}

PBYTE CPacketACCOUNT_RES_DELETECHAR::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_nResult,	pDataTmp, sizeof (m_nResult),	pDataTmp);	// 結果
	CopyMemoryRenew(&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	// キャラID

	pRet = pDataTmp;
	return pRet;
}
