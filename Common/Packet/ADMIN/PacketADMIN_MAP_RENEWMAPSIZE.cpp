﻿/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_MAP_RENEWMAPSIZE.cpp							 */
/* 内容			:コマンド(管理者系:マップサイズ更新) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/01													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoMapParts.h"
#include "Command.h"
#include "PacketADMIN_MAP_RENEWMAPSIZE.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_RENEWMAPSIZE::CPacketADMIN_MAP_RENEWMAPSIZE	 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

CPacketADMIN_MAP_RENEWMAPSIZE::CPacketADMIN_MAP_RENEWMAPSIZE()
{
	m_dwMapID		= 0;
	m_nDirection	= 0;
	m_nSize			= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_RENEWMAPSIZE::~CPacketADMIN_MAP_RENEWMAPSIZE	 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

CPacketADMIN_MAP_RENEWMAPSIZE::~CPacketADMIN_MAP_RENEWMAPSIZE()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_RENEWMAPSIZE::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

void CPacketADMIN_MAP_RENEWMAPSIZE::Make(
	DWORD dwMapID,		/* [in] マップID */
	int nDirection,		/* [in] 増減する方向 */
	int nSize)			/* [in] 増減する距離 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwMapID)		+
			 sizeof (nDirection)	+
			 sizeof (nSize);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_RENEWMAPSIZE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID,	sizeof (dwMapID),		pDataTmp);	/* マップID */
	CopyMemoryRenew (pDataTmp, &nDirection,	sizeof (nDirection),	pDataTmp);	/* 増減する方向 */
	CopyMemoryRenew (pDataTmp, &nSize,		sizeof (nSize),			pDataTmp);	/* 増減する距離 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_RENEWMAPSIZE::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

PBYTE CPacketADMIN_MAP_RENEWMAPSIZE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMapID,	pDataTmp, sizeof (m_dwMapID),		pDataTmp);	/* マップID */
	CopyMemoryRenew (&m_nDirection,	pDataTmp, sizeof (m_nDirection),	pDataTmp);	/* 増減する方向 */
	CopyMemoryRenew (&m_nSize,		pDataTmp, sizeof (m_nSize),			pDataTmp);	/* 増減する距離 */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
