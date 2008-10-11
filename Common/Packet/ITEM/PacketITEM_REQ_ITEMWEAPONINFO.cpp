/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketITEM_REQ_ITEMWEAPONINFO.cpp							 */
/* 内容			:コマンド(アイテム系:武器情報要求) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/11													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketITEM_REQ_ITEMWEAPONINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketITEM_REQ_ITEMWEAPONINFO::CPacketITEM_REQ_ITEMWEAPONINFO	 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

CPacketITEM_REQ_ITEMWEAPONINFO::CPacketITEM_REQ_ITEMWEAPONINFO()
{
	m_dwWeaponInfoID = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_REQ_ITEMWEAPONINFO::~CPacketITEM_REQ_ITEMWEAPONINFO */
/* 内容		:デストラクタ													 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

CPacketITEM_REQ_ITEMWEAPONINFO::~CPacketITEM_REQ_ITEMWEAPONINFO()
{
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_REQ_ITEMWEAPONINFO::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

void CPacketITEM_REQ_ITEMWEAPONINFO::Make(
	DWORD dwWeaponInfoID)		/* [in] 武器情報ID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwWeaponInfoID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ITEM;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_REQ_ITEMWEAPONINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwWeaponInfoID, sizeof (dwWeaponInfoID), pDataTmp);	/* 武器情報ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_REQ_ITEMWEAPONINFO::Set							 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

PBYTE CPacketITEM_REQ_ITEMWEAPONINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwWeaponInfoID, pDataTmp, sizeof (m_dwWeaponInfoID), pDataTmp);	/* 武器情報ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
