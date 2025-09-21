/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketITEM_ITEMWEAPONINFO.cpp								 */
/* 内容			:コマンド(アイテム系:武器情報通知) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/11													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketITEM_ITEMWEAPONINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketITEM_ITEMWEAPONINFO::CPacketITEM_ITEMWEAPONINFO			 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

CPacketITEM_ITEMWEAPONINFO::CPacketITEM_ITEMWEAPONINFO()
{
	m_pLibInfo = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_ITEMWEAPONINFO::~CPacketITEM_ITEMWEAPONINFO		 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

CPacketITEM_ITEMWEAPONINFO::~CPacketITEM_ITEMWEAPONINFO()
{
	SAFE_DELETE (m_pLibInfo);
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_ITEMWEAPONINFO::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

void CPacketITEM_ITEMWEAPONINFO::Make(
	PCLibInfoItemWeapon pInfo)		/* [in] 武器情報 */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_ITEMWEAPONINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* 武器情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_ITEMWEAPONINFO::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

PBYTE CPacketITEM_ITEMWEAPONINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pLibInfo);
	m_pLibInfo	= new CLibInfoItemWeapon;
	m_pLibInfo->Create ();
	pDataTmp	= m_pLibInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
