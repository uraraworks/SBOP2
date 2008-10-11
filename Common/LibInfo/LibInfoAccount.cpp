/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:LibInfoAccount.cpp											 */
/* 内容			:アカウント情報基底クラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoAccount.h"


/* ========================================================================= */
/* 関数名	:CLibInfoAccount::CLibInfoAccount								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

CLibInfoAccount::CLibInfoAccount()
{
	m_paInfo = NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoAccount::~CLibInfoAccount								 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

CLibInfoAccount::~CLibInfoAccount()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoAccount::Create										 */
/* 内容		:作成															 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CLibInfoAccount::Create(void)
{
	m_paInfo = new ARRAYINFOACCOUNT;
}


/* ========================================================================= */
/* 関数名	:CLibInfoAccount::Destroy										 */
/* 内容		:破棄															 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CLibInfoAccount::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoAccount::CheckPassword									 */
/* 内容		:パスワードのチェック											 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

BOOL CLibInfoAccount::CheckPassword(LPCSTR pszAccount, LPCSTR pszPassword)
{
	BOOL bRet;
	PCInfoAccount pInfoAccount;

	bRet = FALSE;

	pInfoAccount = GetPtr (pszAccount);
	if (pInfoAccount == NULL) {
		goto Exit;
	}

	if (pInfoAccount->m_strPassword != pszPassword) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoAccount::IsUseMacAddr									 */
/* 内容		:MACアドレスが使用済みかチェック								 */
/* 戻り値	:TRUE:使用済み													 */
/* 日付		:2008/07/20														 */
/* ========================================================================= */

BOOL CLibInfoAccount::IsUseMacAddr(LPCSTR pszMacAddr)
{
	BOOL bRet;
	int i, nCount;
	PCInfoAccount pInfoAccount;

	bRet = FALSE;

	if (strcmp (pszMacAddr, "00-00-00-00-00-00") == 0) {
		return FALSE;
	}

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoAccount = (PCInfoAccount)GetPtr (i);
		if (pInfoAccount->m_strMacAddr == pszMacAddr) {
			break;
		}
	}
	if (i >= nCount) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoAccount::GetNew										 */
/* 内容		:新規データを取得												 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoAccount::GetNew(void)
{
	return (PCInfoBase)new CInfoAccount;
}


/* ========================================================================= */
/* 関数名	:CLibInfoAccount::GetCount										 */
/* 内容		:データ数を取得													 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

int CLibInfoAccount::GetCount(void)
{
	int nRet;

	nRet = 0;

	if (m_paInfo == NULL) {
		goto Exit;
	}

	nRet = m_paInfo->GetSize ();
Exit:
	return nRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoAccount::Add											 */
/* 内容		:追加															 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CLibInfoAccount::Add(PCInfoBase pInfo)
{
	PCInfoAccount pAccountInfo;

	pAccountInfo = (PCInfoAccount)pInfo;
	if (pAccountInfo->m_dwAccountID == 0) {
		pAccountInfo->m_dwAccountID = GetNewID ();
	}

	m_paInfo->Add (pAccountInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoAccount::Delete										 */
/* 内容		:削除															 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CLibInfoAccount::Delete(
	int nNo)		/* [in] 配列番号 */
{
	PCInfoAccount pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoAccount::Delete										 */
/* 内容		:削除															 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CLibInfoAccount::Delete(
	DWORD dwAccountID)		/* [in] アカウントID */
{
	int i, nCount, nNo;
	PCInfoAccount pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwAccountID != dwAccountID) {
			continue;
		}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete (nNo);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoAccount::DeleteAll										 */
/* 内容		:全て削除														 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CLibInfoAccount::DeleteAll(void)
{
	int i, nCount;

	if (m_paInfo == NULL) {
		return;
	}

	nCount = m_paInfo->GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		Delete (i);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoAccount::GetAccountID									 */
/* 内容		:キャラIDからアカウントIDを取得									 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

DWORD CLibInfoAccount::GetAccountID(DWORD dwCharID)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoAccount pInfo;

	dwRet = 0;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		if (pInfo->m_dwCharID == dwCharID) {
			dwRet = pInfo->m_dwAccountID;
			break;
		}
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoAccount::GetPtr										 */
/* 内容		:アカウント情報を取得											 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoAccount::GetPtr(int nNo)
{
	return (PCInfoBase)m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoAccount::GetPtr										 */
/* 内容		:アカウント情報を取得											 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

PCInfoAccount CLibInfoAccount::GetPtr(
	DWORD dwAccountID)		/* [in] アカウントID */
{
	int i, nCount;
	PCInfoAccount pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwAccountID != dwAccountID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoAccount::GetPtr										 */
/* 内容		:アカウント情報を取得											 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

PCInfoAccount CLibInfoAccount::GetPtr(
	LPCSTR pszAccount)		/* [in] アカウント */
{
	int i, nCount;
	PCInfoAccount pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_strAccount != pszAccount) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoAccount::GetPtrSessionID								 */
/* 内容		:アカウント情報を取得											 */
/* 日付		:2007/02/15														 */
/* ========================================================================= */

PCInfoAccount CLibInfoAccount::GetPtrSessionID(
	DWORD dwSessionID)		/* [in] セッションID */
{
	int i, nCount;
	PCInfoAccount pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwSessionID != dwSessionID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoAccount::GetNewID										 */
/* 内容		:新しいアカウントIDを取得										 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

DWORD CLibInfoAccount::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoAccount pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwAccountID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}

	return dwRet;
}

/* Copyright(C)URARA-works 2006 */
