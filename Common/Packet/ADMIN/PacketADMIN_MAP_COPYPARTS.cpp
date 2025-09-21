/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_MAP_COPYPARTS.cpp								 */
/* 内容			:コマンド(管理者系:マップパーツコピー) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/14													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoMapParts.h"
#include "Command.h"
#include "PacketADMIN_MAP_COPYPARTS.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_COPYPARTS::CPacketADMIN_MAP_COPYPARTS			 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/06/14														 */
/* ========================================================================= */

CPacketADMIN_MAP_COPYPARTS::CPacketADMIN_MAP_COPYPARTS()
{
	m_ptViewPos.x	= 0;
	m_ptViewPos.y	= 0;
	m_dwSrcPartsID	= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_COPYPARTS::~CPacketADMIN_MAP_COPYPARTS		 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/06/14														 */
/* ========================================================================= */

CPacketADMIN_MAP_COPYPARTS::~CPacketADMIN_MAP_COPYPARTS()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_COPYPARTS::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/06/14														 */
/* ========================================================================= */

void CPacketADMIN_MAP_COPYPARTS::Make(
	DWORD dwSrcPartsID,		/* [in] コピー元パーツID */
	POINT ptViewPos)		/* [in] 表示位置 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwSrcPartsID)	+
			 sizeof (ptViewPos);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_COPYPARTS;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwSrcPartsID,	sizeof (dwSrcPartsID),	pDataTmp);	/* コピー元パーツID */
	CopyMemoryRenew (pDataTmp, &ptViewPos,		sizeof (ptViewPos),		pDataTmp);	/* 表示位置 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_MAP_COPYPARTS::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/06/14														 */
/* ========================================================================= */

PBYTE CPacketADMIN_MAP_COPYPARTS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwSrcPartsID,	pDataTmp, sizeof (m_dwSrcPartsID),	pDataTmp);	/* パーツID */
	CopyMemoryRenew (&m_ptViewPos,		pDataTmp, sizeof (m_ptViewPos),		pDataTmp);	/* 座標 */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
