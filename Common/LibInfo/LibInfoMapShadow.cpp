/// @file LibInfoMapShadow.cpp
/// @brief マップ影情報ライブラリクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/05
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "LibInfoMapShadow.h"

CLibInfoMapShadow::CLibInfoMapShadow()
{
	m_paInfo = NULL;
}

CLibInfoMapShadow::~CLibInfoMapShadow()
{
	Destroy();
}

void CLibInfoMapShadow::Create(void)
{
	m_paInfo = new ARRAYMAPSHADOW;
}

void CLibInfoMapShadow::Destroy(void)
{
	DeleteAll();
	SAFE_DELETE(m_paInfo);
}

BOOL CLibInfoMapShadow::Proc(void)
{
	BOOL bRet;
	int i, nCount;
	PCInfoMapShadow pInfo;

	bRet = FALSE;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapShadow)GetPtr(i);
		bRet |= pInfo->TimerProc(timeGetTime());
	}

	return bRet;
}

PCInfoBase CLibInfoMapShadow::GetNew(void)
{
	return new CInfoMapShadow;
}

int CLibInfoMapShadow::GetCount(void)
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

void CLibInfoMapShadow::Add(PCInfoBase pInfo)
{
	PCInfoMapShadow pMapShadowInfo;

	pMapShadowInfo = (PCInfoMapShadow)pInfo;
	if (pMapShadowInfo->m_dwShadowID == 0) {
		pMapShadowInfo->m_dwShadowID = GetNewID();
	}

	m_paInfo->Add(pMapShadowInfo);
	m_mapIDPtr.insert(pair<DWORD, PVOID>(pMapShadowInfo->m_dwShadowID, (PVOID)pMapShadowInfo));
}

void CLibInfoMapShadow::RenewIDPtr(void)
{
	int i, nCount;
	PCInfoMapShadow pInfo;

	m_mapIDPtr.clear();

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapShadow)GetPtr(i);

		m_mapIDPtr.insert(pair<DWORD, PVOID>(pInfo->m_dwShadowID, (PVOID)pInfo));
	}
}

void CLibInfoMapShadow::Delete(
	int nNo)	// [in] 配列番号
{
	PCInfoMapShadow pInfo;

	pInfo = m_paInfo->at(nNo);
	SAFE_DELETE(pInfo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paInfo->size()))) {
		m_paInfo->erase(m_paInfo->begin() + nNo);
	}
}

void CLibInfoMapShadow::Delete(
	DWORD dwShadowID)	// [in] マップ影ID
{
	int i, nCount, nNo;
	PCInfoMapShadow pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwShadowID != dwShadowID) {
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

void CLibInfoMapShadow::DeleteAll(void)
{
	int i, nCount;

	if (m_paInfo == NULL) {
		return;
	}

	nCount = m_paInfo->size();
	for (i = nCount - 1; i >= 0; i --) {
		Delete(i);
	}
	RenewIDPtr();
}

void CLibInfoMapShadow::Merge(CLibInfoMapShadow *pSrc)
{
	int i, nCount;
	PCInfoMapShadow pInfoTmp, pInfoSrc;

	nCount = pSrc->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoMapShadow)pSrc->GetPtr(i);
		pInfoTmp = (PCInfoMapShadow)GetPtr(pInfoSrc->m_dwShadowID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoMapShadow)GetNew();
			pInfoTmp->Copy(pInfoSrc);
			Add(pInfoTmp);
	} else {
			pInfoTmp->Copy(pInfoSrc);
	}
	}
}

void CLibInfoMapShadow::ResetAnime(void)
{
	int i, nCount;
	PCInfoMapShadow pInfo;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapShadow)GetPtr(i);
		pInfo->m_byAnimeNo	= 0;
		pInfo->m_dwLastAnime	= timeGetTime();
	}
}

PCInfoBase CLibInfoMapShadow::GetPtr(int nNo)
{
	return m_paInfo->at(nNo);
}

PCInfoBase CLibInfoMapShadow::GetPtr(
	DWORD dwShadowID)	// [in] マップ影ID
{
	PCInfoBase pRet;

	pRet = NULL;
	if (dwShadowID == 0) {
		return pRet;
	}

	MapIDPtr::iterator ite;
	ite = m_mapIDPtr.find(dwShadowID);
	if (ite != m_mapIDPtr.end()) {
		pRet = (PCInfoBase)ite->second;
	}

	return pRet;
}

PCInfoBase CLibInfoMapShadow::GetPtr(
	POINT *pPos)	// [in] 表示位置
{
	int i, nCount;
	PCInfoMapShadow pRet, pInfoTmp;

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

DWORD CLibInfoMapShadow::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwSize;
	PCInfoMapShadow pInfoMapShadow;

	dwSize = 0;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoMapShadow = (PCInfoMapShadow)GetPtr(i);
		dwSize += pInfoMapShadow->GetSendDataSize();
	}
	// 終端用
	dwSize += sizeof(DWORD);

	return dwSize;
}

PBYTE CLibInfoMapShadow::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataPos, pDataTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoMapShadow pInfoMapShadow;

	dwSize	= GetSendDataSize();
	pData	= new BYTE[dwSize];
	ZeroMemory(pData, dwSize);

	pDataPos = pData;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoMapShadow = (PCInfoMapShadow)GetPtr(i);

		dwSizeTmp	= pInfoMapShadow->GetSendDataSize();
		pDataTmp	= pInfoMapShadow->GetSendData();
		CopyMemoryRenew(pDataPos, pDataTmp, dwSizeTmp, pDataPos);
		SAFE_DELETE_ARRAY(pDataTmp);
	}

	return pData;
}

PBYTE CLibInfoMapShadow::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp;
	DWORD dwTmp;
	PCInfoMapShadow pInfoMapShadow, pInfoMapShadowTmp;

	pDataTmp = pSrc;

	while (1) {
		CopyMemory(&dwTmp, pDataTmp, sizeof(DWORD));
		if (dwTmp == 0) {
			pDataTmp += sizeof(DWORD);
			break;
	}
		pInfoMapShadowTmp = (PCInfoMapShadow)GetNew();
		pDataTmp = pInfoMapShadowTmp->SetSendData(pDataTmp);

		pInfoMapShadow = (PCInfoMapShadow)GetPtr(pInfoMapShadowTmp->m_dwShadowID);
		if (pInfoMapShadow) {
			pInfoMapShadow->Copy(pInfoMapShadowTmp);
			SAFE_DELETE(pInfoMapShadowTmp);
	} else {
			Add(pInfoMapShadowTmp);
	}
	}
	RenewIDPtr();

	return pDataTmp;
}

DWORD CLibInfoMapShadow::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMapShadow pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwShadowID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
	}
	}

	return dwRet;
}

