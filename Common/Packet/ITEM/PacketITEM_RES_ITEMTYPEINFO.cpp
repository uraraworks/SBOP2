/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketITEM_RES_ITEMTYPEINFO.cpp							 */
/* 内容			:コマンド(アイテム系:アイテム種別情報応答) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/03													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoItemType/InfoItemTypeBase.h"
#include "LibInfoItemType.h"
#include "Command.h"
#include "PacketITEM_RES_ITEMTYPEINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketITEM_RES_ITEMTYPEINFO::CPacketITEM_RES_ITEMTYPEINFO		 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/10/03														 */
/* ========================================================================= */

CPacketITEM_RES_ITEMTYPEINFO::CPacketITEM_RES_ITEMTYPEINFO()
{
	m_pInfo = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_RES_ITEMTYPEINFO::~CPacketITEM_RES_ITEMTYPEINFO	 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/10/03														 */
/* ========================================================================= */

CPacketITEM_RES_ITEMTYPEINFO::~CPacketITEM_RES_ITEMTYPEINFO()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_RES_ITEMTYPEINFO::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/10/03														 */
/* ========================================================================= */

void CPacketITEM_RES_ITEMTYPEINFO::Make(
	CInfoItemTypeBase *pInfo)		/* [in] アイテム種別情報 */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_RES_ITEMTYPEINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* アイテム種別情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_RES_ITEMTYPEINFO::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/10/03														 */
/* ========================================================================= */

PBYTE CPacketITEM_RES_ITEMTYPEINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;
	CLibInfoItemType LibInfo;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);
	pDataTmp	= LibInfo.GetNewSendData (pDataTmp, m_pInfo);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
