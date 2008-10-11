/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketITEM_RES_ITEMWEAPONINFO.cpp							 */
/* 内容			:コマンド(アイテム系:武器情報応答) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/11													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoItemWeapon.h"
#include "Command.h"
#include "PacketITEM_RES_ITEMWEAPONINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketITEM_RES_ITEMWEAPONINFO::CPacketITEM_RES_ITEMWEAPONINFO	 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

CPacketITEM_RES_ITEMWEAPONINFO::CPacketITEM_RES_ITEMWEAPONINFO()
{
	m_pInfo = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_RES_ITEMWEAPONINFO::~CPacketITEM_RES_ITEMWEAPONINFO */
/* 内容		:デストラクタ													 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

CPacketITEM_RES_ITEMWEAPONINFO::~CPacketITEM_RES_ITEMWEAPONINFO()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_RES_ITEMWEAPONINFO::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

void CPacketITEM_RES_ITEMWEAPONINFO::Make(
	CInfoItemWeapon *pInfo)		/* [in] 武器情報 */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize ();
	pInfoDataTmp	= pInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE) +
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ITEM;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_RES_ITEMWEAPONINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* 武器情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_RES_ITEMWEAPONINFO::Set							 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

PBYTE CPacketITEM_RES_ITEMWEAPONINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CInfoItemWeapon;
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
