/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_ITEM_DELETE.cpp								 */
/* 内容			:コマンド(管理者系:アイテム情報削除) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/08/19													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_ITEM_DELETE.h"


/* ========================================================================= */
/* 関数名	:CPacketADMIN_ITEM_DELETE::CPacketADMIN_ITEM_DELETE				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/08/19														 */
/* ========================================================================= */

CPacketADMIN_ITEM_DELETE::CPacketADMIN_ITEM_DELETE()
{
	m_dwItemID = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_ITEM_DELETE::~CPacketADMIN_ITEM_DELETE			 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/08/19														 */
/* ========================================================================= */

CPacketADMIN_ITEM_DELETE::~CPacketADMIN_ITEM_DELETE()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_ITEM_DELETE::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/08/19														 */
/* ========================================================================= */

void CPacketADMIN_ITEM_DELETE::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_ITEM_DELETE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwItemID, sizeof (dwItemID), pDataTmp);	/* アイテムID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_ITEM_DELETE::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/08/19														 */
/* ========================================================================= */

PBYTE CPacketADMIN_ITEM_DELETE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwItemID, pDataTmp, sizeof (m_dwItemID), pDataTmp);	/* アイテムID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
