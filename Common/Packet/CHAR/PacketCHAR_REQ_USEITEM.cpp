/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_REQ_USEITEM.cpp									 */
/* 内容			:コマンド(キャラ系:アイテムを使う要求) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_USEITEM.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_USEITEM::CPacketCHAR_REQ_USEITEM				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/07/05														 */
/* ========================================================================= */

CPacketCHAR_REQ_USEITEM::CPacketCHAR_REQ_USEITEM()
{
	m_dwCharID = 0;
	m_dwItemID = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_USEITEM::~CPacketCHAR_REQ_USEITEM				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/07/05														 */
/* ========================================================================= */

CPacketCHAR_REQ_USEITEM::~CPacketCHAR_REQ_USEITEM()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_USEITEM::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/07/05														 */
/* ========================================================================= */

void CPacketCHAR_REQ_USEITEM::Make(
	DWORD dwCharID,		/* [in] キャラID */
	DWORD dwItemID)		/* [in] アイテムID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (dwItemID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_USEITEM;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID, sizeof (dwCharID), pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &dwItemID, sizeof (dwItemID), pDataTmp);	/* アイテムID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_USEITEM::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/07/05														 */
/* ========================================================================= */

PBYTE CPacketCHAR_REQ_USEITEM::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID, pDataTmp, sizeof (m_dwCharID), pDataTmp);	/* キャラID */
	CopyMemoryRenew (&m_dwItemID, pDataTmp, sizeof (m_dwItemID), pDataTmp);	/* アイテムID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
