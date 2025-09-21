/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_CHAR_REQ_ACCOUNT.cpp							 */
/* 内容			:コマンド(管理者系:アカウント情報要求) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/07													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_CHAR_REQ_ACCOUNT.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHAR_REQ_ACCOUNT::CPacketADMIN_CHAR_REQ_ACCOUNT	 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

CPacketADMIN_CHAR_REQ_ACCOUNT::CPacketADMIN_CHAR_REQ_ACCOUNT()
{
	m_dwAccountID = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHAR_REQ_ACCOUNT::~CPacketADMIN_CHAR_REQ_ACCOUNT	 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

CPacketADMIN_CHAR_REQ_ACCOUNT::~CPacketADMIN_CHAR_REQ_ACCOUNT()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHAR_REQ_ACCOUNT::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

void CPacketADMIN_CHAR_REQ_ACCOUNT::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_REQ_ACCOUNT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwAccountID, sizeof (dwAccountID), pDataTmp);	/* アカウントID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHAR_REQ_ACCOUNT::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

PBYTE CPacketADMIN_CHAR_REQ_ACCOUNT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	/* アカウントID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
