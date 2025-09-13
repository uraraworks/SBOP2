/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_RES_PUTGET.cpp									 */
/* 内容			:コマンド(キャラ系:アイテムを拾う置く応答) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/08													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_RES_PUTGET.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_RES_PUTGET::CPacketCHAR_RES_PUTGET					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/09/08														 */
/* ========================================================================= */

CPacketCHAR_RES_PUTGET::CPacketCHAR_RES_PUTGET()
{
	m_dwItemID	= 0;
	m_nResult	= RESULTID_CHAR_RES_PUTGET_NONE;
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_RES_PUTGET::~CPacketCHAR_RES_PUTGET				 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/09/08														 */
/* ========================================================================= */

CPacketCHAR_RES_PUTGET::~CPacketCHAR_RES_PUTGET()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_RES_PUTGET::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/09/08														 */
/* ========================================================================= */

void CPacketCHAR_RES_PUTGET::Make(
	DWORD dwItemID,		/* [in] アイテムID */
	int nResult)		/* [in] 結果 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwItemID)		+
			 sizeof (nResult);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_RES_PUTGET;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwItemID,	sizeof (dwItemID), pDataTmp);	/* アイテムID */
	CopyMemoryRenew (pDataTmp, &nResult,	 sizeof (nResult), pDataTmp);	/* 結果 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_RES_PUTGET::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/09/08														 */
/* ========================================================================= */

PBYTE CPacketCHAR_RES_PUTGET::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwItemID,	pDataTmp, sizeof (m_dwItemID),	pDataTmp);	/* アイテムID */
	CopyMemoryRenew (&m_nResult,	pDataTmp, sizeof (m_nResult),	pDataTmp);	/* 結果 */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
