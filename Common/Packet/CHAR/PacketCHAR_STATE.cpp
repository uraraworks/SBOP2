/// @file PacketCHAR_STATE.cpp
/// @brief コマンド(キャラ系:キャラ状態通知) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/01/24
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#include "Command.h"
#include "InfoCharBase.h"
#include "PacketCHAR_STATE.h"

CPacketCHAR_STATE::CPacketCHAR_STATE()
{
	m_dwCharID	= 0;
	m_nState	= CHARMOVESTATE_STAND;
}

CPacketCHAR_STATE::~CPacketCHAR_STATE()
{
}

void CPacketCHAR_STATE::Make(
	DWORD dwCharID,	// [in] キャラID
	int nState)	// [in] 状態
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)	+
			 sizeof (nState);

	pData = new BYTE[dwSize];
	ZeroMemory(pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_STATE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew(pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(pDataTmp, &nState,	sizeof (nState),	pDataTmp);	// 状態

	RenewPacket(pData, dwSize);
}

PBYTE CPacketCHAR_STATE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet	= pPacket;
	pDataTmp	= CPacketBase::Set(pPacket);

	CopyMemoryRenew(&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	// キャラID
	CopyMemoryRenew(&m_nState,	pDataTmp, sizeof (m_nState),	pDataTmp);	// 状態

	pRet = pDataTmp;
	return pRet;
}
