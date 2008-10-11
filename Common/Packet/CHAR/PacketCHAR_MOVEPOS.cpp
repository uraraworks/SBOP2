/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_MOVEPOS.cpp										 */
/* 内容			:コマンド(キャラ系:移動通知) 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/01/14													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_MOVEPOS.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_MOVEPOS::CPacketCHAR_MOVEPOS						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/01/14														 */
/* ========================================================================= */

CPacketCHAR_MOVEPOS::CPacketCHAR_MOVEPOS()
{
	m_bUpdate		= FALSE;
	m_dwMapID		= 0;
	m_dwCharID		= 0;
	m_nDirection	= 0;
	m_pos.x = m_pos.y = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_MOVEPOS::~CPacketCHAR_MOVEPOS						 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/01/14														 */
/* ========================================================================= */

CPacketCHAR_MOVEPOS::~CPacketCHAR_MOVEPOS()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_MOVEPOS::Make										 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/01/14														 */
/* ========================================================================= */

void CPacketCHAR_MOVEPOS::Make(
	DWORD dwMapID,		/* [in] マップID */
	DWORD dwCharID,		/* [in] キャラID */
	int nDirection,		/* [in] 向き */
	int x,				/* [in] X座標 */
	int y,				/* [in] Y座標 */
	BOOL bUpdate)		/* [in] 自キャラの座標を更新する？ */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwMapID)		+
			 sizeof (dwCharID)		+
			 sizeof (nDirection)	+
			 sizeof (x)				+
			 sizeof (y)				+
			 sizeof (bUpdate);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_MOVEPOS;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID,	sizeof (dwMapID),		pDataTmp);	/* マップID */
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),		pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &nDirection,	sizeof (nDirection),	pDataTmp);	/* 向き */
	CopyMemoryRenew (pDataTmp, &x,			sizeof (x),				pDataTmp);	/* X座標 */
	CopyMemoryRenew (pDataTmp, &y,			sizeof (y),				pDataTmp);	/* Y座標 */
	CopyMemoryRenew (pDataTmp, &bUpdate,	sizeof (bUpdate),		pDataTmp);	/* 自キャラの座標を更新する？ */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_MOVEPOS::Set										 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/01/14														 */
/* ========================================================================= */

PBYTE CPacketCHAR_MOVEPOS::Set(PBYTE pPacket)
{
	int x, y;
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMapID,	pDataTmp, sizeof (m_dwMapID),		pDataTmp);	/* マップID */
	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),		pDataTmp);	/* キャラID */
	CopyMemoryRenew (&m_nDirection,	pDataTmp, sizeof (m_nDirection),	pDataTmp);	/* 向き */
	CopyMemoryRenew (&x,			pDataTmp, sizeof (x),				pDataTmp);	/* X座標 */
	CopyMemoryRenew (&y,			pDataTmp, sizeof (y),				pDataTmp);	/* Y座標 */
	CopyMemoryRenew (&m_bUpdate,	pDataTmp, sizeof (m_bUpdate),		pDataTmp);	/* 自キャラの座標を更新する？ */

	m_pos.x = x;
	m_pos.y = y;

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
