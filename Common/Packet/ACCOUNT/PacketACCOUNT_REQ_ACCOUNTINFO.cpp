/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:PacketACCOUNT_REQ_ACCOUNTINFO.cpp							 */
/* 内容			:コマンド(アカウント系:アカウント情報要求) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/06													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketACCOUNT_REQ_ACCOUNTINFO.h"

/* ========================================================================= */
/* 関数名	:CPacketACCOUNT_REQ_ACCOUNTINFO::CPacketACCOUNT_REQ_ACCOUNTINFO	 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/11/06														 */
/* ========================================================================= */

CPacketACCOUNT_REQ_ACCOUNTINFO::CPacketACCOUNT_REQ_ACCOUNTINFO()
{
	m_dwAccountID = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketACCOUNT_REQ_ACCOUNTINFO::~CPacketACCOUNT_REQ_ACCOUNTINFO */
/* 内容		:デストラクタ													 */
/* 日付		:2006/11/06														 */
/* ========================================================================= */

CPacketACCOUNT_REQ_ACCOUNTINFO::~CPacketACCOUNT_REQ_ACCOUNTINFO()
{
}


/* ========================================================================= */
/* 関数名	:CPacketACCOUNT_REQ_ACCOUNTINFO::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2006/11/06														 */
/* ========================================================================= */

void CPacketACCOUNT_REQ_ACCOUNTINFO::Make(
	DWORD dwAccountID)		/* [in] アカウントID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwAccountID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ACCOUNT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ACCOUNT_REQ_ACCOUNTINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwAccountID, sizeof (dwAccountID), pDataTmp);	/* アカウントID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketACCOUNT_REQ_ACCOUNTINFO::Set							 */
/* 内容		:パケットを設定													 */
/* 日付		:2006/11/06														 */
/* ========================================================================= */

PBYTE CPacketACCOUNT_REQ_ACCOUNTINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	/* アカウントID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
