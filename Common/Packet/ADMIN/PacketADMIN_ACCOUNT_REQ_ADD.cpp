/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_ACCOUNT_REQ_ADD.cpp							 */
/* 内容			:コマンド(管理者系:アカウントの追加要求) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/01/14													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_ACCOUNT_REQ_ADD.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_ACCOUNT_REQ_ADD::CPacketADMIN_ACCOUNT_REQ_ADD		 */
/* 内容		:コンストラクタ													 */
/* 日付		:2009/01/14														 */
/* ========================================================================= */

CPacketADMIN_ACCOUNT_REQ_ADD::CPacketADMIN_ACCOUNT_REQ_ADD()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_ACCOUNT_REQ_ADD::~CPacketADMIN_ACCOUNT_REQ_ADD	 */
/* 内容		:デストラクタ													 */
/* 日付		:2009/01/14														 */
/* ========================================================================= */

CPacketADMIN_ACCOUNT_REQ_ADD::~CPacketADMIN_ACCOUNT_REQ_ADD()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_ACCOUNT_REQ_ADD::Make								 */
/* 内容		:パケットを作成													 */
/* 日付		:2009/01/14														 */
/* ========================================================================= */

void CPacketADMIN_ACCOUNT_REQ_ADD::Make(
	LPCSTR pszAccount,		/* [in] アカウント */
	LPCSTR pszPassword)		/* [in] パスワード */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;
	CmyString strTmp;

	dwSize = sizeof (PACKETBASE) +
			 		(strlen (pszAccount) + 1) +
			 		(strlen (pszPassword) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_ACCOUNT_REQ_ADD;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	strTmp = pszAccount;
	strcpyRenew ((LPSTR)pDataTmp, strTmp, pDataTmp);	/* アカウント */
	strTmp = pszPassword;
	strcpyRenew ((LPSTR)pDataTmp, strTmp, pDataTmp);	/* パスワード */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_ACCOUNT_REQ_ADD::Set								 */
/* 内容		:パケットを設定													 */
/* 日付		:2009/01/14														 */
/* ========================================================================= */

PBYTE CPacketADMIN_ACCOUNT_REQ_ADD::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	StoreRenew (m_strAccount,	(LPCSTR)pDataTmp, pDataTmp);	/* アカウント */
	StoreRenew (m_strPassword,	(LPCSTR)pDataTmp, pDataTmp);	/* パスワード */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2009 */
