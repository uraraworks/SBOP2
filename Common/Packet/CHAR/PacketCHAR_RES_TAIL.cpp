/// @file PacketCHAR_RES_TAIL.cpp
/// @brief コマンド(キャラ系:付いて行く応答) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/04
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_RES_TAIL.h"

CPacketCHAR_RES_TAIL::CPacketCHAR_RES_TAIL()
{
	m_dwCharID	= 0;
	m_dwTargetCharID	= 0;
	m_dwPara	= 0;
	m_nResult	= 0;
}

CPacketCHAR_RES_TAIL::~CPacketCHAR_RES_TAIL()
{
}

void CPacketCHAR_RES_TAIL::Make(
	DWORD dwCharID,	// [in] キャラID
	DWORD dwTargetCharID,	// [in] 付いて行く相手のキャラID
	int nResult,	// [in] 結果
	DWORD dwPara/*=0*/)	// [in] パラメータ
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)	+
			 sizeof (dwTargetCharID)	+
			 sizeof (nResult)	+
			 sizeof (dwPara);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_RES_TAIL;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(pDataTmp, &dwTargetCharID,	sizeof (dwTargetCharID),	pDataTmp);	// 付いて行く相手のキャラID
	CopyMemoryRenew(pDataTmp, &nResult,	sizeof (nResult),	pDataTmp);	// 結果
	CopyMemoryRenew(pDataTmp, &dwPara,	sizeof (dwPara),	pDataTmp);	// パラメータ

	RenewPacket(pData, dwSize);
}

PBYTE CPacketCHAR_RES_TAIL::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(&m_dwTargetCharID,	pDataTmp, sizeof (m_dwTargetCharID),	pDataTmp);	// 付いて行く相手のキャラID
	CopyMemoryRenew(&m_nResult,	pDataTmp, sizeof (m_nResult),	pDataTmp);	// 結果
	CopyMemoryRenew(&m_dwPara,	pDataTmp, sizeof (m_dwPara),	pDataTmp);	// パラメータ

	pRet = pDataTmp;
	return pRet;
}
