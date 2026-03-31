/// @file LibInfoDisable.cpp
/// @brief 拒否情報ライブラリ基底クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/04/08
/// @copyright Copyright(C)URARA-works 2009

#include "stdafx.h"
#include "LibInfoDisable.h"

CLibInfoDisable::CLibInfoDisable()
{
	m_dwNewIDTmp	= 0;
	m_dwLastTimeProc	= 0;
	m_paInfo	= NULL;
	m_paInfoIPADdress	= NULL;
}

CLibInfoDisable::~CLibInfoDisable()
{
	Destroy();
}

void CLibInfoDisable::Create(void)
{
	m_paInfo	= new ARRAYDISABLEINFO;
	m_paInfoIPADdress	= new ARRAYDISABLEIPADDRESS;
}

void CLibInfoDisable::Destroy(void)
{
	DeleteAll();
	DeleteAllIP();
	SAFE_DELETE(m_paInfo);
	SAFE_DELETE(m_paInfoIPADdress);
}

BOOL CLibInfoDisable::Proc(void)
{
	int i, nCount;
	BOOL bRet;
	DWORD dwTime;
	PSTDISABLEIPADDRESS pInfo;

	bRet = FALSE;

	if (m_paInfoIPADdress == NULL) {
		goto Exit;
	}
	nCount = m_paInfoIPADdress->size();
	if (nCount <= 0) {
		goto Exit;
	}

	dwTime = timeGetTime();
	if (dwTime - m_dwLastTimeProc < 1000 * 10) {
		// 10秒以内なら処理しない
		goto Exit;
	}
	m_dwLastTimeProc = dwTime;

	for (i = nCount - 1; i >= 0; i --) {
		pInfo = m_paInfoIPADdress->at(i);
		// 1時間以上経過している？
		if (dwTime - pInfo->dwLastTime >= 1000 * 60 * 60) {
			// 拒否解除
			DeleteIP(i);
	}
	}

	bRet = TRUE;
Exit:
	return bRet;
}

PCInfoBase CLibInfoDisable::GetNew(void)
{
	PCInfoDisable pInfoDisable;

	pInfoDisable = new CInfoDisable;

	return pInfoDisable;
}

int CLibInfoDisable::GetCount(void)
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

void CLibInfoDisable::Add(PCInfoBase pInfo)
{
	PCInfoDisable pDisableInfo;

	pDisableInfo = (PCInfoDisable)pInfo;
	if (pDisableInfo->m_dwDisableID == 0) {
		pDisableInfo->m_dwDisableID = GetNewID();
	}

	m_paInfo->Add(pDisableInfo);
}

void CLibInfoDisable::Delete(
	int nNo)	// [in] 配列番号
{
	PCInfoDisable pInfo;

	pInfo = m_paInfo->at(nNo);
	SAFE_DELETE(pInfo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paInfo->size()))) {
		m_paInfo->erase(m_paInfo->begin() + nNo);
	}
}

void CLibInfoDisable::Delete(
	DWORD dwDisableID)	// [in] 拒否情報ID
{
	int i, nCount, nNo;
	PCInfoDisable pInfoTmp;

	Enter();

	nNo = -1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwDisableID != dwDisableID) {
			continue;
	}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete(nNo);
	}

	Leave();
}

void CLibInfoDisable::DeleteAll(void)
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

void CLibInfoDisable::CopyAll(CLibInfoDisable *pSrc)
{
	int i, nCount;
	PCInfoDisable pInfoTmp, pInfoNew;

	DeleteAll();

	nCount = pSrc->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = (PCInfoDisable)pSrc->GetPtr(i);
		pInfoNew = (PCInfoDisable)GetNew();
		pInfoNew->Copy(pInfoTmp);
		Add(pInfoNew);
	}

}

BOOL CLibInfoDisable::IsDisable(LPCSTR pszMacAddress)
{
	BOOL bRet;
	PCInfoBase pInfo;

	bRet = FALSE;

	pInfo = GetPtr(pszMacAddress);
	if (pInfo) {
		bRet = TRUE;
	}

	return bRet;
}

void CLibInfoDisable::Merge(CLibInfoDisable *pSrc)
{
	int i, nCount;
	PCInfoDisable pInfoTmp, pInfoSrc;

	nCount = pSrc->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoDisable)pSrc->GetPtr(i);
		pInfoTmp = (PCInfoDisable)GetPtr(pInfoSrc->m_dwDisableID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoDisable)GetNew();
			pInfoTmp->Copy(pInfoSrc);
			Add(pInfoTmp);
	}
		pInfoTmp->Copy(pInfoSrc);
	}
}

PCInfoBase CLibInfoDisable::GetPtr(int nNo)
{
	return m_paInfo->at(nNo);
}

PCInfoBase CLibInfoDisable::GetPtr(
	DWORD dwDisableID)	// [in] 拒否情報ID
{
	int i, nCount;
	PCInfoDisable pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwDisableID != dwDisableID) {
			continue;
	}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}

PCInfoBase CLibInfoDisable::GetPtr(
	LPCSTR pszMacAddress)	// [in] MACアドレス
{
	int i, nCount;
	PCInfoDisable pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_strMacAddress != pszMacAddress) {
			continue;
	}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}

DWORD CLibInfoDisable::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoDisable pDisable;

	dwRet = dwSize = 0;

	// データ数分のサイズ
	dwSize += sizeof(DWORD);

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pDisable = (PCInfoDisable)GetPtr(i);

		dwSize += pDisable->GetSendDataSize();
	}

	dwRet = dwSize;
	return dwRet;
}

PBYTE CLibInfoDisable::GetSendData(void)
{
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwCount, dwSize, dwSizeTmp, dwOffset;
	PCInfoDisable pDisable;

	pRet = NULL;

	dwSize	= GetSendDataSize();
	pData	= ZeroNew(dwSize);
	dwOffset	= 0;

	// データ数を書き込み
	dwCount = (DWORD)GetCount();
	CopyMemory(pData, &dwCount, sizeof(dwCount));
	dwOffset += sizeof(dwCount);

	// 拒否情報を書き込み
	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pDisable = (PCInfoDisable)GetPtr(i);

		dwSizeTmp	= pDisable->GetSendDataSize();
		pDataTmp	= pDisable->GetSendData();
		CopyMemory(&pData[dwOffset], pDataTmp, dwSizeTmp);
		dwOffset += dwSizeTmp;

		SAFE_DELETE_ARRAY(pDataTmp);
	}

	pRet = pData;
	return pRet;
}

PBYTE CLibInfoDisable::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	DWORD dwCount;
	PBYTE pRet, pDataTmp;
	PCInfoDisable pDisable;

	pRet	= pSrc;
	pDataTmp	= pSrc;

	DeleteAll();

	// データ数を読み込み
	CopyMemory(&dwCount, pDataTmp, sizeof(dwCount));
	nCount	= (int)dwCount;
	pDataTmp	+= sizeof(dwCount);

	for (i = 0; i < nCount; i ++) {
		pDisable = (PCInfoDisable)GetNew();

		pDataTmp = pDisable->SetSendData(pDataTmp);
		Add(pDisable);
	}

	pRet = pDataTmp;
	return pRet;
}

void CLibInfoDisable::AddIP(ULONG ulIPAddress)
{
	BOOL bResult;
	PSTDISABLEIPADDRESS pInfo;

	bResult = IsDisableIP(ulIPAddress);
	if (bResult) {
		// 登録済み
		return;
	}

	pInfo = new STDISABLEIPADDRESS;
	pInfo->ulIPAddress	= ulIPAddress;
	pInfo->dwLastTime	= timeGetTime();
	m_paInfoIPADdress->Add(pInfo);
}

void CLibInfoDisable::DeleteIP(int nNo)
{
	PSTDISABLEIPADDRESS pInfo;

	pInfo = m_paInfoIPADdress->at(nNo);
	SAFE_DELETE(pInfo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paInfoIPADdress->size()))) { m_paInfoIPADdress->erase(m_paInfoIPADdress->begin() + nNo); }
}

void CLibInfoDisable::DeleteAllIP(void)
{
	int i, nCount;

	if (m_paInfoIPADdress == NULL) {
		return;
	}
	nCount = m_paInfoIPADdress->size();
	for (i = nCount - 1; i >= 0; nCount --) {
		DeleteIP(i);
	}
}

BOOL CLibInfoDisable::IsDisableIP(ULONG ulIPAddress)
{
	BOOL bRet;
	int i, nCount;
	PSTDISABLEIPADDRESS pInfo;

	bRet = FALSE;

	nCount = m_paInfoIPADdress->size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfoIPADdress->at(i);
		if (pInfo->ulIPAddress != ulIPAddress) {
			continue;
	}
		bRet = TRUE;
		break;
	}

	return bRet;
}

DWORD CLibInfoDisable::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoDisable pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwDisableID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
	}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

