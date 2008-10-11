/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketITEM_ITEMTYPEINFO.cpp								 */
/* 内容			:コマンド(アイテム系:アイテム種別情報通知) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/03													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoItemType.h"
#include "Command.h"
#include "PacketITEM_ITEMTYPEINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketITEM_ITEMTYPEINFO::CPacketITEM_ITEMTYPEINFO				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/10/03														 */
/* ========================================================================= */

CPacketITEM_ITEMTYPEINFO::CPacketITEM_ITEMTYPEINFO()
{
	m_pLibInfo = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_ITEMTYPEINFO::~CPacketITEM_ITEMTYPEINFO			 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/10/03														 */
/* ========================================================================= */

CPacketITEM_ITEMTYPEINFO::~CPacketITEM_ITEMTYPEINFO()
{
	SAFE_DELETE (m_pLibInfo);
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_ITEMTYPEINFO::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/10/03														 */
/* ========================================================================= */

void CPacketITEM_ITEMTYPEINFO::Make(
	PCLibInfoItemType pInfo)		/* [in] アイテム種別情報 */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_ITEMTYPEINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* アイテム種別情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_ITEMTYPEINFO::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/10/03														 */
/* ========================================================================= */

PBYTE CPacketITEM_ITEMTYPEINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pLibInfo);
	m_pLibInfo	= new CLibInfoItemType;
	m_pLibInfo->Create ();
	pDataTmp	= m_pLibInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
