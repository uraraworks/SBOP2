/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_STATE.cpp										 */
/* 内容			:コマンド(キャラ系:キャラ状態通知) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/01/24													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "InfoCharBase.h"
#include "PacketCHAR_STATE.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_STATE::CPacketCHAR_STATE							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/01/24														 */
/* ========================================================================= */

CPacketCHAR_STATE::CPacketCHAR_STATE()
{
	m_dwCharID	= 0;
	m_nState	= CHARMOVESTATE_STAND;
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_STATE::~CPacketCHAR_STATE							 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/01/24														 */
/* ========================================================================= */

CPacketCHAR_STATE::~CPacketCHAR_STATE()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_STATE::Make										 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/01/24														 */
/* ========================================================================= */

void CPacketCHAR_STATE::Make(
	DWORD dwCharID,		/* [in] キャラID */
	int nState)			/* [in] 状態 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (nState);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_STATE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &nState,		sizeof (nState),	pDataTmp);	/* 状態 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_STATE::Set											 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/01/24														 */
/* ========================================================================= */

PBYTE CPacketCHAR_STATE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* キャラID */
	CopyMemoryRenew (&m_nState,		pDataTmp, sizeof (m_nState),	pDataTmp);	/* 状態 */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
