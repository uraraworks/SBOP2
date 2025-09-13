/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketITEM_REQ_ITEMINFO.cpp								 */
/* 内容			:コマンド(アイテム系:アイテム情報要求) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/08/16													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketITEM_REQ_ITEMINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketITEM_REQ_ITEMINFO::CPacketITEM_REQ_ITEMINFO				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/08/16														 */
/* ========================================================================= */

CPacketITEM_REQ_ITEMINFO::CPacketITEM_REQ_ITEMINFO()
{
	m_dwItemID = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_REQ_ITEMINFO::~CPacketITEM_REQ_ITEMINFO			 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/08/16														 */
/* ========================================================================= */

CPacketITEM_REQ_ITEMINFO::~CPacketITEM_REQ_ITEMINFO()
{
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_REQ_ITEMINFO::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/08/16														 */
/* ========================================================================= */

void CPacketITEM_REQ_ITEMINFO::Make(
	DWORD dwItemID)		/* [in] アイテムID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwItemID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ITEM;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_REQ_ITEMINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwItemID, sizeof (dwItemID), pDataTmp);	/* アイテムID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_REQ_ITEMINFO::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/08/16														 */
/* ========================================================================= */

PBYTE CPacketITEM_REQ_ITEMINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwItemID, pDataTmp, sizeof (m_dwItemID), pDataTmp);	/* アイテムID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
