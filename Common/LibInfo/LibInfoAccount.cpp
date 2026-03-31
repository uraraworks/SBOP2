/// @file LibInfoAccount.cpp
/// @brief アカウント情報基底クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/05
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "LibInfoAccount.h"

CLibInfoAccount::CLibInfoAccount()
{
	m_paInfo = NULL;
}

CLibInfoAccount::~CLibInfoAccount()
{
	Destroy();
}

void CLibInfoAccount::Create(void)
{
	m_paInfo = new ARRAYINFOACCOUNT;
}

void CLibInfoAccount::Destroy(void)
{
	DeleteAll();
	SAFE_DELETE(m_paInfo);
}

BOOL CLibInfoAccount::CheckPassword(LPCSTR pszAccount, LPCSTR pszPassword)
{
	BOOL bRet;
	PCInfoAccount pInfoAccount;

	bRet = FALSE;

	pInfoAccount = GetPtr(pszAccount);
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

BOOL CLibInfoAccount::IsUseMacAddr(LPCSTR pszMacAddr)
{
	BOOL bRet;
	int i, nCount;
	PCInfoAccount pInfoAccount;

	bRet = FALSE;

	if (strcmp(pszMacAddr, "00-00-00-00-00-00") == 0) {
		return FALSE;
	}

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoAccount = (PCInfoAccount)GetPtr(i);
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

PCInfoBase CLibInfoAccount::GetNew(void)
{
	return (PCInfoBase)new CInfoAccount;
}

int CLibInfoAccount::GetCount(void)
{
	int nRet;

	nRet = 0;

	if (m_paInfo == NULL) {
		goto Exit;
	}

	nRet = m_paInfo->size();
Exit:
	return nRet;
}

void CLibInfoAccount::Add(PCInfoBase pInfo)
{
	PCInfoAccount pAccountInfo;

	pAccountInfo = (PCInfoAccount)pInfo;
	if (pAccountInfo->m_dwAccountID == 0) {
		pAccountInfo->m_dwAccountID = GetNewID();
	}

	m_paInfo->Add(pAccountInfo);
}

void CLibInfoAccount::Delete(
	int nNo)	// [in] 配列番号
{
	PCInfoAccount pInfo;

	pInfo = m_paInfo->at(nNo);
	SAFE_DELETE(pInfo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paInfo->size()))) {
		m_paInfo->erase(m_paInfo->begin() + nNo);
	}
}

void CLibInfoAccount::Delete(
	DWORD dwAccountID)	// [in] アカウントID
{
	int i, nCount, nNo;
	PCInfoAccount pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwAccountID != dwAccountID) {
			continue;
	}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete(nNo);
	}
}

void CLibInfoAccount::DeleteAll(void)
{
	int i, nCount;

	if (m_paInfo == NULL) {
		return;
	}

	nCount = m_paInfo->size();
	for (i = nCount - 1; i >= 0; i --) {
		Delete(i);
	}
}

DWORD CLibInfoAccount::GetAccountID(DWORD dwCharID)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoAccount pInfo;

	dwRet = 0;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->at(i);
		if (pInfo->m_dwCharID == dwCharID) {
			dwRet = pInfo->m_dwAccountID;
			break;
	}
	}

	return dwRet;
}

PCInfoBase CLibInfoAccount::GetPtr(int nNo)
{
	return (PCInfoBase)m_paInfo->at(nNo);
}

PCInfoAccount CLibInfoAccount::GetPtr(
	DWORD dwAccountID)	// [in] アカウントID
{
	int i, nCount;
	PCInfoAccount pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwAccountID != dwAccountID) {
			continue;
	}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}

PCInfoAccount CLibInfoAccount::GetPtr(
	LPCSTR pszAccount)	// [in] アカウント
{
	int i, nCount;
	PCInfoAccount pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_strAccount != pszAccount) {
			continue;
	}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}

PCInfoAccount CLibInfoAccount::GetPtrSessionID(
	DWORD dwSessionID)	// [in] セッションID
{
	int i, nCount;
	PCInfoAccount pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwSessionID != dwSessionID) {
			continue;
	}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}

PCInfoAccount CLibInfoAccount::GetPtrMacAddr(
	LPCSTR pszMacAddr)	// [in] 作成時のMACアドレス
{
	int i, nCount;
	PCInfoAccount pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_strMacAddr != pszMacAddr) {
			continue;
	}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}

DWORD CLibInfoAccount::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoAccount pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwAccountID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
	}
	}

	return dwRet;
}

