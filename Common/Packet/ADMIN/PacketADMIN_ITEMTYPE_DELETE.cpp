/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_ITEMTYPE_DELETE.cpp							 */
/* 内容			:コマンド(管理者系:アイテム種別情報削除) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/01													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_ITEMTYPE_DELETE.h"


/* ========================================================================= */
/* 関数名	:CPacketADMIN_ITEMTYPE_DELETE::CPacketADMIN_ITEMTYPE_DELETE		 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/10/01														 */
/* ========================================================================= */

CPacketADMIN_ITEMTYPE_DELETE::CPacketADMIN_ITEMTYPE_DELETE()
{
	m_dwTypeID = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_ITEMTYPE_DELETE::~CPacketADMIN_ITEMTYPE_DELETE	 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/10/01														 */
/* ========================================================================= */

CPacketADMIN_ITEMTYPE_DELETE::~CPacketADMIN_ITEMTYPE_DELETE()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_ITEMTYPE_DELETE::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/10/01														 */
/* ========================================================================= */

void CPacketADMIN_ITEMTYPE_DELETE::Make(
	DWORD dwTypeID)		/* [in] 種別ID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwTypeID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_ITEMTYPE_DELETE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwTypeID, sizeof (dwTypeID), pDataTmp);	/* 種別ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_ITEMTYPE_DELETE::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/10/01														 */
/* ========================================================================= */

PBYTE CPacketADMIN_ITEMTYPE_DELETE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwTypeID, pDataTmp, sizeof (m_dwTypeID), pDataTmp);	/* 種別ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
