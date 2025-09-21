/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketMSGCMD_MAKEITEM.cpp									 */
/* 内容			:コマンド(メッセージコマンド系:アイテム作成) 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/10/20													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketMSGCMD_MAKEITEM.h"

/* ========================================================================= */
/* 関数名	:CPacketMSGCMD_MAKEITEM::CPacketMSGCMD_MAKEITEM					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/10/20														 */
/* ========================================================================= */

CPacketMSGCMD_MAKEITEM::CPacketMSGCMD_MAKEITEM()
{
	m_dwCharID		= 0;
	m_dwItemTypeID	= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketMSGCMD_MAKEITEM::~CPacketMSGCMD_MAKEITEM				 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/10/20														 */
/* ========================================================================= */

CPacketMSGCMD_MAKEITEM::~CPacketMSGCMD_MAKEITEM()
{
}


/* ========================================================================= */
/* 関数名	:CPacketMSGCMD_MAKEITEM::Make									 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/10/20														 */
/* ========================================================================= */

void CPacketMSGCMD_MAKEITEM::Make(
	DWORD dwCharID,			/* [in] キャラID */
	DWORD dwItemTypeID)		/* [in] アイテム種別ID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (dwItemTypeID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MSGCMD;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MSGCMD_MAKEITEM;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,		sizeof (dwCharID),		pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &dwItemTypeID,	sizeof (dwItemTypeID),	pDataTmp);	/* アイテム種別ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketMSGCMD_MAKEITEM::Set									 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/10/20														 */
/* ========================================================================= */

PBYTE CPacketMSGCMD_MAKEITEM::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,		pDataTmp, sizeof (m_dwCharID),		pDataTmp);	/* キャラID */
	CopyMemoryRenew (&m_dwItemTypeID,	pDataTmp, sizeof (m_dwItemTypeID),	pDataTmp);	/* アイテム種別ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
