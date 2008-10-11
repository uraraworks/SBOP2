/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:PacketCHAR_SET_EFCBALLOON.cpp								 */
/* 内容			:コマンド(キャラ系:噴出しを指定) 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/31													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_SET_EFCBALLOON.h"

/* ========================================================================= */
/* 関数名	:CPacketCHAR_SET_EFCBALLOON::CPacketCHAR_SET_EFCBALLOON			 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/12/31														 */
/* ========================================================================= */

CPacketCHAR_SET_EFCBALLOON::CPacketCHAR_SET_EFCBALLOON()
{
	m_dwCharID		= 0;
	m_dwBalloonID	= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_SET_EFCBALLOON::~CPacketCHAR_SET_EFCBALLOON		 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/12/31														 */
/* ========================================================================= */

CPacketCHAR_SET_EFCBALLOON::~CPacketCHAR_SET_EFCBALLOON()
{
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_SET_EFCBALLOON::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2007/12/31														 */
/* ========================================================================= */

void CPacketCHAR_SET_EFCBALLOON::Make(
	DWORD dwCharID,			/* [in] キャラID */
	DWORD dwBalloonID)		/* [in] 噴出しID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (dwBalloonID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_SET_EFCBALLOON;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,		sizeof (dwCharID),		pDataTmp);	/* キャラID */
	CopyMemoryRenew (pDataTmp, &dwBalloonID,	sizeof (dwBalloonID),	pDataTmp);	/* 噴出しID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketCHAR_SET_EFCBALLOON::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2007/12/31														 */
/* ========================================================================= */

PBYTE CPacketCHAR_SET_EFCBALLOON::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,		pDataTmp, sizeof (m_dwCharID),		pDataTmp);	/* キャラID */
	CopyMemoryRenew (&m_dwBalloonID,	pDataTmp, sizeof (m_dwBalloonID),	pDataTmp);	/* 噴出しID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
