/// @file PacketCHAR_RES_PUTGET.cpp
/// @brief コマンド(キャラ系:アイテムを拾う置く応答) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/08
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_RES_PUTGET.h"

CPacketCHAR_RES_PUTGET::CPacketCHAR_RES_PUTGET()
{
	m_dwItemID	= 0;
	m_nResult	= RESULTID_CHAR_RES_PUTGET_NONE;
}

CPacketCHAR_RES_PUTGET::~CPacketCHAR_RES_PUTGET()
{
}

void CPacketCHAR_RES_PUTGET::Make(
	DWORD dwItemID,	// [in] アイテムID
	int nResult)	// [in] 結果
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwItemID)	+
			 sizeof (nResult);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_RES_PUTGET;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwItemID,	sizeof (dwItemID), pDataTmp);	// アイテムID
	CopyMemoryRenew(pDataTmp, &nResult,	 sizeof (nResult), pDataTmp);	// 結果

	RenewPacket(pData, dwSize);
}

PBYTE CPacketCHAR_RES_PUTGET::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwItemID,	pDataTmp, sizeof (m_dwItemID),	pDataTmp);	// アイテムID
	CopyMemoryRenew(&m_nResult,	pDataTmp, sizeof (m_nResult),	pDataTmp);	// 結果

	pRet = pDataTmp;
	return pRet;
}
