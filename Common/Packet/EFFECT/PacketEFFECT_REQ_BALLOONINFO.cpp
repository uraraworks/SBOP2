/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketEFFECT_REQ_BALLOONINFO.cpp							 */
/* 内容			:コマンド(噴出し系:噴出し情報要求) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/24													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketEFFECT_REQ_BALLOONINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketEFFECT_REQ_BALLOONINFO::CPacketEFFECT_REQ_BALLOONINFO	 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

CPacketEFFECT_REQ_BALLOONINFO::CPacketEFFECT_REQ_BALLOONINFO()
{
	m_dwBalloonID = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketEFFECT_REQ_BALLOONINFO::~CPacketEFFECT_REQ_BALLOONINFO	 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

CPacketEFFECT_REQ_BALLOONINFO::~CPacketEFFECT_REQ_BALLOONINFO()
{
}


/* ========================================================================= */
/* 関数名	:CPacketEFFECT_REQ_BALLOONINFO::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CPacketEFFECT_REQ_BALLOONINFO::Make(
	DWORD dwBalloonID)		/* [in] 噴出しID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwBalloonID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_EFFECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_EFFECT_REQ_BALLOONINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwBalloonID, sizeof (dwBalloonID), pDataTmp);	/* 噴出しID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketEFFECT_REQ_BALLOONINFO::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

PBYTE CPacketEFFECT_REQ_BALLOONINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwBalloonID, pDataTmp, sizeof (m_dwBalloonID), pDataTmp);	/* 噴出しID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
