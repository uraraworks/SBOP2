/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_REQ_CHARINFO.cpp								 */
/* 内容			:コマンド(キャラ系:キャラ情報要求) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/12/31													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_CHARINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_CHARINFO::CPacketCHAR_REQ_CHARINFO				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

CPacketCHAR_REQ_CHARINFO::CPacketCHAR_REQ_CHARINFO()
{
	m_dwCharID = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_CHARINFO::~CPacketCHAR_REQ_CHARINFO			 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

CPacketCHAR_REQ_CHARINFO::~CPacketCHAR_REQ_CHARINFO()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_CHARINFO::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

void CPacketCHAR_REQ_CHARINFO::Make(
	DWORD dwCharID)		/* [in] キャラID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwCharID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_CHARINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID, sizeof (dwCharID), pDataTmp);	/* キャラID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_CHARINFO::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

PBYTE CPacketCHAR_REQ_CHARINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID, pDataTmp, sizeof (m_dwCharID), pDataTmp);	/* キャラID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
