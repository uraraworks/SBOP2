/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_ITEMINFO.cpp									 */
/* 内容			:コマンド(キャラ系:アイテム情報通知) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/31													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_ITEMINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_ITEMINFO::CPacketCHAR_ITEMINFO						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/07/31														 */
/* ========================================================================= */

CPacketCHAR_ITEMINFO::CPacketCHAR_ITEMINFO()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_ITEMINFO::~CPacketCHAR_ITEMINFO					 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/07/31														 */
/* ========================================================================= */

CPacketCHAR_ITEMINFO::~CPacketCHAR_ITEMINFO()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_ITEMINFO::Make										 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/07/31														 */
/* ========================================================================= */

void CPacketCHAR_ITEMINFO::Make(
	DWORD dwCharID,			/* [in] キャラID */
	PARRAYDWORD paSrc)		/* [in] アイテムID */
{
	PBYTE pData, pDataTmp;
	DWORD i, dwSize, dwCount, dwTmp;
	PPACKETBASE pPacketBase;

	dwCount = paSrc->GetSize ();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (DWORD)			+
			 (sizeof (DWORD) * dwCount);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_ITEMINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &dwCount,	sizeof (dwCount),	pDataTmp);	/* 個数 */
	for (i = 0; i < dwCount; i ++) {
		dwTmp = paSrc->GetAt (i);
		CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);		/* アイテムID */
	}

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_ITEMINFO::Set										 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/07/31														 */
/* ========================================================================= */

PBYTE CPacketCHAR_ITEMINFO::Set(PBYTE pPacket)
{
	DWORD i, dwCount, dwTmp;
	PBYTE pRet, pDataTmp;

	m_adwItemID.RemoveAll ();

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* キャラID */
	CopyMemoryRenew (&dwCount,		pDataTmp, sizeof (dwCount),		pDataTmp);	/* 個数 */
	for (i = 0; i < dwCount; i ++) {
		CopyMemoryRenew (&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);			/* アイテムID */
		m_adwItemID.Add (dwTmp);
	}

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
