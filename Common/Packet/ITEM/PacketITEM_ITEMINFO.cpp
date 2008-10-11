/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketITEM_ITEMINFO.cpp									 */
/* 内容			:コマンド(アイテム系:アイテム情報通知) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketITEM_ITEMINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketITEM_ITEMINFO::CPacketITEM_ITEMINFO						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

CPacketITEM_ITEMINFO::CPacketITEM_ITEMINFO()
{
	m_pLibInfo = NULL;
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_ITEMINFO::~CPacketITEM_ITEMINFO					 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

CPacketITEM_ITEMINFO::~CPacketITEM_ITEMINFO()
{
	SAFE_DELETE (m_pLibInfo);
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_ITEMINFO::Make										 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

void CPacketITEM_ITEMINFO::Make(
	PCLibInfoItem pInfo)		/* [in] アイテム情報 */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_ITEMINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* アイテム情報 */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* 関数名	:CPacketITEM_ITEMINFO::Set										 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

PBYTE CPacketITEM_ITEMINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pLibInfo);
	m_pLibInfo	= new CLibInfoItem;
	m_pLibInfo->Create ();
	pDataTmp	= m_pLibInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
