/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketACCOUNT_RES_MAKECHAR.cpp								 */
/* 内容			:コマンド(アカウント系:キャラ作成応答) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/12/03													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketACCOUNT_RES_MAKECHAR.h"

/* ========================================================================= */
/* 関数名	:CPacketACCOUNT_RES_MAKECHAR::CPacketACCOUNT_RES_MAKECHAR		 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/12/03														 */
/* ========================================================================= */

CPacketACCOUNT_RES_MAKECHAR::CPacketACCOUNT_RES_MAKECHAR()
{
	m_nResult	= MAKECHARRES_NONE;
	m_dwCharID	= 0;
}


/* ========================================================================= */
/* 関数名	:CPacketACCOUNT_RES_MAKECHAR::~CPacketACCOUNT_RES_MAKECHAR		 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/12/03														 */
/* ========================================================================= */

CPacketACCOUNT_RES_MAKECHAR::~CPacketACCOUNT_RES_MAKECHAR()
{
}


/* ========================================================================= */
/* 関数名	:CPacketACCOUNT_RES_MAKECHAR::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2006/12/03														 */
/* ========================================================================= */

void CPacketACCOUNT_RES_MAKECHAR::Make(
	int nResult,		/* [in] 結果 */
	DWORD dwCharID)		/* [in] キャラID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (nResult)		+
			 sizeof (dwCharID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ACCOUNT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ACCOUNT_RES_MAKECHAR;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &nResult,	sizeof (nResult),	pDataTmp);	/* 結果 */
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* キャラID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketACCOUNT_RES_MAKECHAR::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2006/12/03														 */
/* ========================================================================= */

PBYTE CPacketACCOUNT_RES_MAKECHAR::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_nResult,	pDataTmp, sizeof (m_nResult),	pDataTmp);	/* 結果 */
	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* キャラID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
