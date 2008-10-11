/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_REQ_PUSH.cpp									 */
/* 内容			:コマンド(キャラ系:押す要求) 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/02													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_PUSH.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_PUSH::CPacketCHAR_REQ_PUSH						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/09/02														 */
/* ========================================================================= */

CPacketCHAR_REQ_PUSH::CPacketCHAR_REQ_PUSH()
{
	m_dwCharID		= 0;
	m_nDirection	= 0;
	m_nPushCount	= 1;
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_PUSH::~CPacketCHAR_REQ_PUSH					 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/09/02														 */
/* ========================================================================= */

CPacketCHAR_REQ_PUSH::~CPacketCHAR_REQ_PUSH()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_PUSH::Make										 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/09/02														 */
/* ========================================================================= */

void CPacketCHAR_REQ_PUSH::Make(
	DWORD dwCharID,				/* [in] キャラID */
	int nDirection,				/* [in] 向き */
	int nPushCount)/*=1*/		/* [in] 押して進む数 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (nDirection)	+
			 sizeof (nPushCount);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_PUSH;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),		pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &nDirection,	sizeof (nDirection),	pDataTmp);	/* 向き */
	CopyMemoryRenew (pDataTmp, &nPushCount,	sizeof (nPushCount),	pDataTmp);	/* 押して進む数 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_REQ_PUSH::Set										 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/09/02														 */
/* ========================================================================= */

PBYTE CPacketCHAR_REQ_PUSH::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),		pDataTmp);	/* キャラID */
	CopyMemoryRenew (&m_nDirection,	pDataTmp, sizeof (m_nDirection),	pDataTmp);	/* 向き */
	CopyMemoryRenew (&m_nPushCount,	pDataTmp, sizeof (m_nPushCount),	pDataTmp);	/* 押して進む数 */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
