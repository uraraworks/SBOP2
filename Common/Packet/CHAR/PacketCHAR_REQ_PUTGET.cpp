/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_REQ_PUTGET.cpp									 */
/* 内容			:コマンド(キャラ系:アイテムを拾う置く要求) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_PUTGET.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_PUTGET::CPacketCHAR_REQ_PUTGET					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

CPacketCHAR_REQ_PUTGET::CPacketCHAR_REQ_PUTGET()
{
	m_dwCharID = 0;
	m_dwItemID = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_PUTGET::~CPacketCHAR_REQ_PUTGET				 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

CPacketCHAR_REQ_PUTGET::~CPacketCHAR_REQ_PUTGET()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_PUTGET::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

void CPacketCHAR_REQ_PUTGET::Make(
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_PUTGET;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID, sizeof (dwCharID), pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &dwItemID, sizeof (dwItemID), pDataTmp);	/* アイテムID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_PUTGET::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

PBYTE CPacketCHAR_REQ_PUTGET::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID, pDataTmp, sizeof (m_dwCharID), pDataTmp);	/* キャラID */
	CopyMemoryRenew (&m_dwItemID, pDataTmp, sizeof (m_dwItemID), pDataTmp);	/* アイテムID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
