/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:CPacketCONNECT_REQ_PLAY.cpp								 */
/* 内容			:コマンド(接続系:ゲーム開始要求) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/12/31													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCONNECT_REQ_PLAY.h"

/* ========================================================================= */
/* 関数名	:CPacketCONNECT_REQ_PLAY::CPacketCONNECT_REQ_PLAY				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

CPacketCONNECT_REQ_PLAY::CPacketCONNECT_REQ_PLAY()
{
	m_dwAccountID	= 0;
	m_dwCharID		= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketCONNECT_REQ_PLAY::~CPacketCONNECT_REQ_PLAY				 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

CPacketCONNECT_REQ_PLAY::~CPacketCONNECT_REQ_PLAY()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCONNECT_REQ_PLAY::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

void CPacketCONNECT_REQ_PLAY::Make(
	DWORD dwAccountID,		/* [in] アカウントID */
	DWORD dwCharID)			/* [in] キャラID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwAccountID)	+
			 sizeof (dwCharID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CONNECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CONNECT_REQ_PLAY;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwAccountID,	sizeof (dwAccountID),	pDataTmp);	/* アカウントID */
	CopyMemoryRenew (pDataTmp, &dwCharID,		sizeof (dwCharID),		pDataTmp);	/* キャラID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCONNECT_REQ_PLAY::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

PBYTE CPacketCONNECT_REQ_PLAY::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwAccountID,	pDataTmp, sizeof (m_dwAccountID),	pDataTmp);	/* アカウントID */
	CopyMemoryRenew (&m_dwCharID,		pDataTmp, sizeof (m_dwCharID),		pDataTmp);	/* キャラID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
