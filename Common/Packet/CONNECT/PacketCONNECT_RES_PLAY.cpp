/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketCONNECT_RES_PLAY.cpp									 */
/* 内容			:コマンド(接続系:ゲーム開始応答) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/12/31													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCONNECT_RES_PLAY.h"

/* ========================================================================= */
/* 関数名	:CPacketCONNECT_RES_PLAY::CPacketCONNECT_RES_PLAY				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

CPacketCONNECT_RES_PLAY::CPacketCONNECT_RES_PLAY()
{
	m_nResult = PLAYRES_NONE;
}


/* ========================================================================= */
/* 関数名	:CPacketCONNECT_RES_PLAY::~CPacketCONNECT_RES_PLAY				 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

CPacketCONNECT_RES_PLAY::~CPacketCONNECT_RES_PLAY()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCONNECT_RES_PLAY::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

void CPacketCONNECT_RES_PLAY::Make(
	int nResult)		/* [in] 結果 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (nResult);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CONNECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CONNECT_RES_PLAY;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &nResult, sizeof (nResult), pDataTmp);	/* 結果 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCONNECT_RES_PLAY::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2006/12/31														 */
/* ========================================================================= */

PBYTE CPacketCONNECT_RES_PLAY::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_nResult, pDataTmp, sizeof (m_nResult), pDataTmp);	/* 結果 */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
