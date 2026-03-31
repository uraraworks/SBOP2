/// @file LibInfoMapParts.cpp
/// @brief マップパーツ情報ライブラリクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/04/28
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "LibInfoMapParts.h"

CLibInfoMapParts::CLibInfoMapParts()
{
	m_dwNewIDTmp	= 0;
	m_paInfo	= NULL;
}

CLibInfoMapParts::~CLibInfoMapParts()
{
	Destroy();
}

void CLibInfoMapParts::Create(void)
{
	m_paInfo = new ARRAYMAPPARTS;
}

void CLibInfoMapParts::Destroy(void)
{
	DeleteAll();
	SAFE_DELETE(m_paInfo);
}

BOOL CLibInfoMapParts::Proc(void)
{
	BOOL bRet;
	int i, nCount;
	PCInfoMapParts pInfo;

	bRet = FALSE;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapParts)GetPtr(i);
		bRet |= pInfo->TimerProc(timeGetTime());
	}

	return bRet;
}

PCInfoBase CLibInfoMapParts::GetNew(void)
{
	return new CInfoMapParts;
}

int CLibInfoMapParts::GetCount(void)
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

void CLibInfoMapParts::Add(PCInfoBase pInfo)
{
	PCInfoMapParts pMapPartsInfo;

	pMapPartsInfo = (PCInfoMapParts)pInfo;
	if (pMapPartsInfo->m_dwPartsID == 0) {
		pMapPartsInfo->m_dwPartsID = GetNewID();
	}

	m_paInfo->Add(pMapPartsInfo);
	m_mapIDPtr.insert(pair<DWORD, PVOID>(pMapPartsInfo->m_dwPartsID, (PVOID)pMapPartsInfo));
}

void CLibInfoMapParts::Delete(
	int nNo)	// [in] 配列番号
{
	PCInfoMapParts pInfo;

	pInfo = m_paInfo->at(nNo);
	SAFE_DELETE(pInfo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paInfo->size()))) {
		m_paInfo->erase(m_paInfo->begin() + nNo);
	}
}

void CLibInfoMapParts::Delete(
	DWORD dwPartsID)	// [in] マップパーツID
{
	int i, nCount, nNo;
	PCInfoMapParts pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwPartsID != dwPartsID) {
			continue;
	}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete(nNo);
	}
	RenewIDPtr();
}

void CLibInfoMapParts::DeleteAll(void)
{
	int i, nCount;

	if (m_paInfo == NULL) {
		return;
	}

	nCount = m_paInfo->size();
	for (i = nCount - 1; i >= 0; i --) {
		Delete(i);
	}
	m_dwNewIDTmp = 0;
	RenewIDPtr();
}

void CLibInfoMapParts::Merge(CLibInfoMapParts *pSrc)
{
	int i, nCount;
	PCInfoMapParts pInfoTmp, pInfoSrc;

	nCount = pSrc->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoMapParts)pSrc->GetPtr(i);
		pInfoTmp = (PCInfoMapParts)GetPtr(pInfoSrc->m_dwPartsID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoMapParts)GetNew();
			pInfoTmp->Copy(pInfoSrc);
			Add(pInfoTmp);
	} else {
			pInfoTmp->Copy(pInfoSrc);
	}
	}
}

void CLibInfoMapParts::ResetAnime(void)
{
	int i, nCount;
	PCInfoMapParts pInfo;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapParts)GetPtr(i);
		pInfo->m_byAnimeNo	= 0;
		pInfo->m_dwLastAnime	= timeGetTime();
	}
}

PCInfoBase CLibInfoMapParts::GetPtr(int nNo)
{
	return m_paInfo->at(nNo);
}

PCInfoBase CLibInfoMapParts::GetPtr(
	DWORD dwPartsID)	// [in] マップパーツID
{
	PCInfoBase pRet;

	pRet = NULL;
	if (dwPartsID == 0) {
		return pRet;
	}

	MapIDPtr::iterator ite;
	ite = m_mapIDPtr.find(dwPartsID);
	if (ite != m_mapIDPtr.end()) {
		pRet = (PCInfoBase)ite->second;
	}

	return pRet;
}

PCInfoBase CLibInfoMapParts::GetPtr(
	POINT *pPos)	// [in] 表示位置
{
	int i, nCount;
	PCInfoMapParts pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (!((pInfoTmp->m_ptViewPos.x == pPos->x) &&
			(pInfoTmp->m_ptViewPos.y == pPos->y))) {
			continue;
	}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}

DWORD CLibInfoMapParts::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwSize;
	PCInfoMapParts pInfoMapParts;

	dwSize = 0;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoMapParts = (PCInfoMapParts)GetPtr(i);
		dwSize += pInfoMapParts->GetSendDataSize();
	}
	// 終端用
	dwSize += sizeof(DWORD);

	return dwSize;
}

PBYTE CLibInfoMapParts::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataPos, pDataTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoMapParts pInfoMapParts;

	dwSize	= GetSendDataSize();
	pData	= new BYTE[dwSize];
	ZeroMemory(pData, dwSize);

	pDataPos = pData;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoMapParts = (PCInfoMapParts)GetPtr(i);

		dwSizeTmp	= pInfoMapParts->GetSendDataSize();
		pDataTmp	= pInfoMapParts->GetSendData();
		CopyMemoryRenew(pDataPos, pDataTmp, dwSizeTmp, pDataPos);
		SAFE_DELETE_ARRAY(pDataTmp);
	}

	return pData;
}

PBYTE CLibInfoMapParts::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp;
	DWORD dwTmp;
	PCInfoMapParts pInfoMapParts, pInfoMapPartsTmp;

	pDataTmp = pSrc;

	while (1) {
		CopyMemory(&dwTmp, pDataTmp, sizeof(DWORD));
		if (dwTmp == 0) {
			pDataTmp += sizeof(DWORD);
			break;
	}
		pInfoMapPartsTmp = (PCInfoMapParts)GetNew();
		pDataTmp = pInfoMapPartsTmp->SetSendData(pDataTmp);

		pInfoMapParts = (PCInfoMapParts)GetPtr(pInfoMapPartsTmp->m_dwPartsID);
		if (pInfoMapParts) {
			pInfoMapParts->Copy(pInfoMapPartsTmp);
			SAFE_DELETE(pInfoMapPartsTmp);
	} else {
			Add(pInfoMapPartsTmp);
	}
	}
	RenewIDPtr();

	return pDataTmp;
}

DWORD CLibInfoMapParts::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMapParts pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwPartsID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
	}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

void CLibInfoMapParts::RenewIDPtr(void)
{
	int i, nCount;
	PCInfoMapParts pInfoMapParts;

	m_mapIDPtr.clear();

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoMapParts = (PCInfoMapParts)GetPtr(i);

		m_mapIDPtr.insert(pair<DWORD, PVOID>(pInfoMapParts->m_dwPartsID, (PVOID)pInfoMapParts));
	}
}

