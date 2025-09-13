/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:PacketADMIN_CHAR_RENEW_ACCOUNT.cpp							 */
/* 内容			:コマンド(管理者系:アカウント情報更新) 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/07													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "InfoAccount.h"
#include "PacketADMIN_CHAR_RENEW_ACCOUNT.h"

/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHAR_RENEW_ACCOUNT::CPacketADMIN_CHAR_RENEW_ACCOUNT */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

CPacketADMIN_CHAR_RENEW_ACCOUNT::CPacketADMIN_CHAR_RENEW_ACCOUNT()
{
	m_bDisable = FALSE;
	m_dwAccountID = 0;
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHAR_RENEW_ACCOUNT::~CPacketADMIN_CHAR_RENEW_ACCOUNT */
/* 内容		:デストラクタ													 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

CPacketADMIN_CHAR_RENEW_ACCOUNT::~CPacketADMIN_CHAR_RENEW_ACCOUNT()
{
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHAR_RENEW_ACCOUNT::Make							 */
/* 内容		:パケットを作成													 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

void CPacketADMIN_CHAR_RENEW_ACCOUNT::Make(
	CInfoAccount *pInfoAccount,		/* [in] アカウント情報 */
	BOOL bDisable,					/* [in] ログイン拒否 */
	LPCSTR pszMacAddress)			/* [in] 拒否MACアドレス */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (bDisable) +
			 sizeof (pInfoAccount->m_dwAccountID) +
			 		(pInfoAccount->m_strAccount.GetLength () + 1) +
			 		(pInfoAccount->m_strPassword.GetLength () + 1);
	if (pszMacAddress) {
		dwSize += strlen (pszMacAddress);
	}
	dwSize ++;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_RENEW_ACCOUNT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &pInfoAccount->m_dwAccountID, sizeof (pInfoAccount->m_dwAccountID), pDataTmp);	/* アカウントID */
	CopyMemoryRenew (pDataTmp, &bDisable, sizeof (bDisable), pDataTmp);	/* ログイン拒否 */
	strcpyRenew ((LPSTR)pDataTmp, pInfoAccount->m_strAccount,  pDataTmp);										/* アカウント */
	strcpyRenew ((LPSTR)pDataTmp, pInfoAccount->m_strPassword, pDataTmp);										/* パスワード */
	if (pszMacAddress) {
		strcpyRenew ((LPSTR)pDataTmp, pszMacAddress, pDataTmp);													/* 拒否MACアドレス */
	}

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* 関数名	:CPacketADMIN_CHAR_RENEW_ACCOUNT::Set							 */
/* 内容		:パケットを設定													 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

PBYTE CPacketADMIN_CHAR_RENEW_ACCOUNT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	/* アカウントID */
	CopyMemoryRenew (&m_bDisable, pDataTmp, sizeof (m_bDisable), pDataTmp);			/* ログイン拒否 */
	StoreRenew (m_strAccount,	(LPCSTR)pDataTmp, pDataTmp);						/* アカウント */
	StoreRenew (m_strPassword,	(LPCSTR)pDataTmp, pDataTmp);						/* パスワード */
	StoreRenew (m_strMacAddress,	(LPCSTR)pDataTmp, pDataTmp);					/* 拒否MACアドレス */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
