/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMAP_REQ_MAPINFO.cpp									 */
/* 内容			:コマンド(マップ系:マップ情報要求) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/01/04													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketMAP_REQ_MAPINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketMAP_REQ_MAPINFO::CPacketMAP_REQ_MAPINFO					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/01/04														 */
/* ========================================================================= */

CPacketMAP_REQ_MAPINFO::CPacketMAP_REQ_MAPINFO()
{
	m_dwMapID = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_REQ_MAPINFO::~CPacketMAP_REQ_MAPINFO				 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/01/04														 */
/* ========================================================================= */

CPacketMAP_REQ_MAPINFO::~CPacketMAP_REQ_MAPINFO()
{
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_REQ_MAPINFO::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/01/04														 */
/* ========================================================================= */

void CPacketMAP_REQ_MAPINFO::Make(
	DWORD dwMapID)		/* [in] マップID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwMapID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_REQ_MAPINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID, sizeof (dwMapID), pDataTmp);	/* マップID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketMAP_REQ_MAPINFO::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/01/04														 */
/* ========================================================================= */

PBYTE CPacketMAP_REQ_MAPINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMapID, pDataTmp, sizeof (m_dwMapID), pDataTmp);	/* マップID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
