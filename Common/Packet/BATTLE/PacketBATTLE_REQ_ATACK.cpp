/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketBATTLE_REQ_ATACK.cpp									 */
/* 内容			:コマンド(戦闘系:攻撃要求) 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/26													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketBATTLE_REQ_ATACK.h"

/* ========================================================================= */
/* 関数名	:CPacketBATTLE_REQ_ATACK::CPacketBATTLE_REQ_ATACK				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

CPacketBATTLE_REQ_ATACK::CPacketBATTLE_REQ_ATACK()
{
	m_dwCharID			= 0;
	m_dwTargetCharID	= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketBATTLE_REQ_ATACK::~CPacketBATTLE_REQ_ATACK				 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

CPacketBATTLE_REQ_ATACK::~CPacketBATTLE_REQ_ATACK()
{
}


/* ========================================================================= */
/* 関数名	:CPacketBATTLE_REQ_ATACK::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

void CPacketBATTLE_REQ_ATACK::Make(
	DWORD dwCharID,				/* [in] キャラID */
	DWORD dwTargetCharID)		/* [in] 攻撃先キャラID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (dwTargetCharID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_BATTLE;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_BATTLE_REQ_ATACK;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,		sizeof (dwCharID),			pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &dwTargetCharID,	sizeof (dwTargetCharID),	pDataTmp);	/* 攻撃先キャラID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketBATTLE_REQ_ATACK::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

PBYTE CPacketBATTLE_REQ_ATACK::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,		pDataTmp, sizeof (m_dwCharID),			pDataTmp);	/* キャラID */
	CopyMemoryRenew (&m_dwTargetCharID,	pDataTmp, sizeof (m_dwTargetCharID),	pDataTmp);	/* 攻撃先キャラID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
