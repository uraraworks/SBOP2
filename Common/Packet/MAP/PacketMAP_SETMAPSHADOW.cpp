/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_SETMAPSHADOW.cpp									 */
/* 内容			:コマンド(マップ系:マップ影配置) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/08													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoMapShadow.h"
#include "Command.h"
#include "PacketMAP_SETMAPSHADOW.h"

/* ========================================================================= */
/* 関数名	:CPacketMAP_SETMAPSHADOW::CPacketMAP_SETMAPSHADOW				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/06/08														 */
/* ========================================================================= */

CPacketMAP_SETMAPSHADOW::CPacketMAP_SETMAPSHADOW()
{
	m_ptPos.x		= 0;
	m_ptPos.y		= 0;
	m_dwMapID		= 0;
	m_dwShadowID	= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_SETMAPSHADOW::~CPacketMAP_SETMAPSHADOW				 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/06/08														 */
/* ========================================================================= */

CPacketMAP_SETMAPSHADOW::~CPacketMAP_SETMAPSHADOW()
{
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_SETMAPSHADOW::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/06/08														 */
/* ========================================================================= */

void CPacketMAP_SETMAPSHADOW::Make(
	DWORD dwMapID,			/* [in] マップID */
	int x,					/* [in] X座標 */
	int y,					/* [in] Y座標 */
	DWORD dwShadowID)		/* [in] マップ影ID */
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
			 sizeof (dwShadowID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_SETMAPSHADOW;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID,	sizeof (dwMapID),		pDataTmp);	/* マップID */
	CopyMemoryRenew (pDataTmp, &ptPos,		sizeof (ptPos),			pDataTmp);	/* 座標 */
	CopyMemoryRenew (pDataTmp, &dwShadowID,	sizeof (dwShadowID),	pDataTmp);	/* マップ影ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_SETMAPSHADOW::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/06/08														 */
/* ========================================================================= */

PBYTE CPacketMAP_SETMAPSHADOW::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMapID,	pDataTmp, sizeof (m_dwMapID),		pDataTmp);	/* マップID */
	CopyMemoryRenew (&m_ptPos,		pDataTmp, sizeof (m_ptPos),			pDataTmp);	/* 座標 */
	CopyMemoryRenew (&m_dwShadowID,	pDataTmp, sizeof (m_dwShadowID),	pDataTmp);	/* マップ影ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
