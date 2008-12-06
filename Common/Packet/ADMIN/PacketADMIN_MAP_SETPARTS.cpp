/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_MAP_SETPARTS.cpp								 */
/* 内容			:コマンド(管理者系:マップパーツ配置) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/30													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoMapParts.h"
#include "Command.h"
#include "PacketADMIN_MAP_SETPARTS.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_SETPARTS::CPacketADMIN_MAP_SETPARTS			 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

CPacketADMIN_MAP_SETPARTS::CPacketADMIN_MAP_SETPARTS()
{
	m_bPile		= FALSE;
	m_ptPos.x	= 0;
	m_ptPos.y	= 0;
	m_dwMapID	= 0;
	m_dwPartsID	= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_SETPARTS::~CPacketADMIN_MAP_SETPARTS			 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

CPacketADMIN_MAP_SETPARTS::~CPacketADMIN_MAP_SETPARTS()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_SETPARTS::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

void CPacketADMIN_MAP_SETPARTS::Make(
	DWORD dwMapID,			/* [in] マップID */
	int x,					/* [in] X座標 */
	int y,					/* [in] Y座標 */
	DWORD dwPartsID,		/* [in] パーツID */
	BOOL bPile)				/* [in] 重ね合わせ用 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	POINT ptPos;
	PPACKETBASE pPacketBase;

	ptPos.x = x;
	ptPos.y = y;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (ptPos)			+
			 sizeof (dwMapID)		+
			 sizeof (dwPartsID)		+
			 sizeof (bPile);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_SETPARTS;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID,	sizeof (dwMapID),	pDataTmp);	/* マップID */
	CopyMemoryRenew (pDataTmp, &ptPos,		sizeof (ptPos),		pDataTmp);	/* 座標 */
	CopyMemoryRenew (pDataTmp, &dwPartsID,	sizeof (dwPartsID),	pDataTmp);	/* パーツID */
	CopyMemoryRenew (pDataTmp, &bPile,		sizeof (bPile),		pDataTmp);	/* 重ね合わせ用 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_SETPARTS::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

PBYTE CPacketADMIN_MAP_SETPARTS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMapID,	pDataTmp, sizeof (m_dwMapID),	pDataTmp);	/* マップID */
	CopyMemoryRenew (&m_ptPos,		pDataTmp, sizeof (m_ptPos),		pDataTmp);	/* 座標 */
	CopyMemoryRenew (&m_dwPartsID,	pDataTmp, sizeof (m_dwPartsID),	pDataTmp);	/* パーツID */
	CopyMemoryRenew (&m_bPile,		pDataTmp, sizeof (m_bPile),		pDataTmp);	/* 重ね合わせ用 */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
