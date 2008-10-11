/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_ONLINE.cpp										 */
/* 内容			:コマンド(マップ系:オンライン数通知) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/02/10													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketMAP_ONLINE.h"

/* ========================================================================= */
/* 関数名	:CPacketMAP_ONLINE::CPacketMAP_ONLINE							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/02/10														 */
/* ========================================================================= */

CPacketMAP_ONLINE::CPacketMAP_ONLINE()
{
	m_nCount = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_ONLINE::~CPacketMAP_ONLINE							 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/02/10														 */
/* ========================================================================= */

CPacketMAP_ONLINE::~CPacketMAP_ONLINE()
{
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_ONLINE::Make										 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/02/10														 */
/* ========================================================================= */

void CPacketMAP_ONLINE::Make(
	int nCount)		/* [in] オンライン数 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (nCount);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_ONLINE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &nCount, sizeof (nCount), pDataTmp);	/* オンライン数 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_ONLINE::Set											 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/02/10														 */
/* ========================================================================= */

PBYTE CPacketMAP_ONLINE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_nCount, pDataTmp, sizeof (m_nCount), pDataTmp);	/* オンライン数 */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
