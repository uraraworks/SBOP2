/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketCONNECT_KEEPALIVE.cpp								 */
/* 内容			:コマンド(接続系:生存確認通知) 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/21													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCONNECT_KEEPALIVE.h"

/* ========================================================================= */
/* 関数名	:CPacketCONNECT_KEEPALIVE::CPacketCONNECT_KEEPALIVE				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/06/21														 */
/* ========================================================================= */

CPacketCONNECT_KEEPALIVE::CPacketCONNECT_KEEPALIVE()
{
	m_dwData = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketCONNECT_KEEPALIVE::~CPacketCONNECT_KEEPALIVE			 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/06/21														 */
/* ========================================================================= */

CPacketCONNECT_KEEPALIVE::~CPacketCONNECT_KEEPALIVE()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCONNECT_KEEPALIVE::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/06/21														 */
/* ========================================================================= */

void CPacketCONNECT_KEEPALIVE::Make(DWORD dwData)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE);
	dwSize += sizeof (dwData);

	pData = ZeroNew (dwSize);
	pPacketBase = (PPACKETBASE)pData;
	pDataTmp = pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CONNECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CONNECT_KEEPALIVE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwData, sizeof (dwData), pDataTmp);	/* 返信用データ */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCONNECT_KEEPALIVE::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/06/21														 */
/* ========================================================================= */

PBYTE CPacketCONNECT_KEEPALIVE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwData, pDataTmp, sizeof (m_dwData), pDataTmp);	/* 返信用データ */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
