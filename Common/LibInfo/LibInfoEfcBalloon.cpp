/// @file LibInfoEfcBalloon.cpp
/// @brief 噴出し情報ライブラリ基底クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/12/24
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "InfoEfcBalloon.h"
#include "LibInfoEfcBalloon.h"

CLibInfoEfcBalloon::CLibInfoEfcBalloon()
{
	m_dwNewIDTmp	= 0;
	m_paInfo	= NULL;
}

CLibInfoEfcBalloon::~CLibInfoEfcBalloon()
{
	Destroy();
}

void CLibInfoEfcBalloon::Create(void)
{
	m_paInfo = new ARRAYEFCBALLOON;
}

void CLibInfoEfcBalloon::Destroy(void)
{
	DeleteAll();
	SAFE_DELETE(m_paInfo);
}

PCInfoBase CLibInfoEfcBalloon::GetNew(void)
{
	PCInfoEfcBalloon pInfoEfcBalloon;

	pInfoEfcBalloon = new CInfoEfcBalloon;

	return pInfoEfcBalloon;
}

PCInfoBase CLibInfoEfcBalloon::GetPtr(int nNo)
{
	PCInfoBase pRet;

	pRet = NULL;
	if (nNo < 0 || nNo >= GetCount()) {
		goto Exit;
	}
	pRet = m_paInfo->at(nNo);
Exit:
	return pRet;
}

int CLibInfoEfcBalloon::GetCount(void)
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

void CLibInfoEfcBalloon::Add(PCInfoBase pInfo)
{
	PCInfoEfcBalloon pInfoEfcBalloon;

	pInfoEfcBalloon = (PCInfoEfcBalloon)pInfo;
	if (pInfoEfcBalloon->m_dwEfcBalloonID == 0) {
		pInfoEfcBalloon->m_dwEfcBalloonID = GetNewID();
	}

	m_paInfo->Add(pInfoEfcBalloon);
}

void CLibInfoEfcBalloon::Delete(
	int nNo)	// [in] 配列番号
{
	PCInfoEfcBalloon pInfo;

	pInfo = m_paInfo->at(nNo);
	SAFE_DELETE(pInfo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paInfo->size()))) {
		m_paInfo->erase(m_paInfo->begin() + nNo);
	}
}

void CLibInfoEfcBalloon::Delete(
	DWORD dwEfcBalloonID)	// [in] モーションID
{
	int i, nCount, nNo;
	PCInfoEfcBalloon pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwEfcBalloonID != dwEfcBalloonID) {
			continue;
	}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete(nNo);
	}
}

void CLibInfoEfcBalloon::DeleteAll(void)
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

void CLibInfoEfcBalloon::SetList(DWORD dwListID, CLibInfoEfcBalloon *pSrc)
{
	int i, nCount;
	PCInfoEfcBalloon pInfo, pInfoTmp;

	// まずは対象となるリストIDの情報を削除
	nCount = m_paInfo->size();
	for (i = nCount - 1; i >= 0; i --) {
		pInfo = m_paInfo->at(i);
		if (dwListID != 0) {
			if (pInfo->m_dwListID != dwListID) {
				continue;
		}
	}
		Delete(i);
	}

	nCount = pSrc->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoEfcBalloon)pSrc->GetPtr(i);
		if (dwListID != 0) {
			if (pInfo->m_dwListID != dwListID) {
				continue;
		}
	}
		pInfoTmp = new CInfoEfcBalloon;
		pInfoTmp->Copy(pInfo);
		Add(pInfoTmp);
	}

	Sort();
}

void CLibInfoEfcBalloon::GetListID(ARRAYDWORD &aDst)
{
	int i, j, nCount, nCountTmp;
	PCInfoEfcBalloon pInfo;

	aDst.clear();

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->at(i);

		nCountTmp = aDst.size();
		for (j = 0; j < nCountTmp; j ++) {
			if (pInfo->m_dwListID == aDst[j]) {
				break;
		}
	}
		if (j < nCountTmp) {
			continue;
	}
		aDst.push_back(pInfo->m_dwListID);
	}
}

void CLibInfoEfcBalloon::GetName(
	DWORD dwListID,	// [in] 種別ID
	CmyString &strDst)	// [ou] 噴出し名
{
	int i, nCount;
	PCInfoEfcBalloon pInfo;

	strDst.Empty();

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->at(i);
		if (pInfo->m_dwListID != dwListID) {
			continue;
	}
		strDst = pInfo->m_strName;
		break;
	}
}

void CLibInfoEfcBalloon::Sort(void)
{
	int i, nCount;
	DWORD dwListID;
	PCInfoEfcBalloon pInfo;
	ARRAYEFCBALLOON apInfo;

	apInfo = *m_paInfo;
	m_paInfo->clear();

	while (1) {
		nCount = apInfo.size();
		if (nCount <= 0) {
			break;
	}
		dwListID = -1;
		// 最小の種別IDを探す
		for (i = 0; i < nCount; i ++) {
			pInfo = apInfo[i];
			if (pInfo->m_dwListID > dwListID) {
				continue;
		}
			dwListID = pInfo->m_dwListID;
	}

		for (i = 0; i < nCount; i ++) {
			pInfo = apInfo[i];
			if (pInfo->m_dwListID != dwListID) {
				continue;
		}
			m_paInfo->Add(pInfo);
			apInfo.erase(apInfo.begin() + i);
			i --;
			nCount = apInfo.size();
	}
	}
}

DWORD CLibInfoEfcBalloon::GetNextAnimeID(
	DWORD dwListID,	// [in] 種別ID
	DWORD dwAnimeID)	// [in] コマ番号
{
	int i, nCount;
	DWORD dwRet;
	ARRAYEFCBALLOON apBalloon;
	PCInfoEfcBalloon pInfo;

	dwRet = -1;

	GetEfcBalloonInfo(dwListID, apBalloon);
	nCount = apBalloon.size();
	for (i = 0; i < nCount; i ++) {
		pInfo = apBalloon[i];
		if (dwAnimeID + 1 != pInfo->m_dwAnimeID) {
			continue;
	}
		break;
	}
	// 続きがある？
	if (i < nCount) {
		dwRet = dwAnimeID + 1;
	}

	return dwRet;
}

CInfoEfcBalloon *CLibInfoEfcBalloon::GetPtr(
	DWORD dwListID,	// [in] 種別ID
	DWORD dwAnimeID)	// [in] コマ番号
{
	int i, nCount;
	PCInfoEfcBalloon pRet, pInfo;
	ARRAYEFCBALLOON apBalloon;

	pRet = NULL;
	if (dwListID == 0) {
		goto Exit;
	}
	GetEfcBalloonInfo(dwListID, apBalloon);

	nCount = apBalloon.size();
	for (i = 0; i < nCount; i ++) {
		pInfo = apBalloon[i];
		if (dwAnimeID != pInfo->m_dwAnimeID) {
			continue;
	}
		pRet = pInfo;
		break;
	}

Exit:
	return pRet;
}

void CLibInfoEfcBalloon::GetEfcBalloonInfo(DWORD dwListID, ARRAYEFCBALLOON &aDst)
{
	int i, nCount;
	PCInfoEfcBalloon pInfo;

	if (dwListID == 0) {
		return;
	}
	aDst.clear();

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoEfcBalloon)GetPtr(i);
		if (dwListID != 0) {
			if (pInfo->m_dwListID != dwListID) {
				continue;
		}
	}
		aDst.push_back(pInfo);
	}
}

DWORD CLibInfoEfcBalloon::GetSendDataSize(DWORD dwListID)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoEfcBalloon pInfo;

	dwRet = 0;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->at(i);
		if (dwListID > 0) {
			if (pInfo->m_dwListID != dwListID) {
				continue;
		}
	}
		dwRet += pInfo->GetSendDataSize();
	}

	dwRet += sizeof(DWORD);	// 終端分

	return dwRet;
}

PBYTE CLibInfoEfcBalloon::GetSendData(DWORD dwListID)
{
	PBYTE pRet, pDataTmp, pPos;
	DWORD dwSize, dwSizeTmp;
	int i, nCount;
	PCInfoEfcBalloon pInfo;

	pRet = NULL;

	dwSize = GetSendDataSize(dwListID);
	if (dwSize == 0) {
		goto Exit;
	}
	pRet = ZeroNew(dwSize);
	pPos = pRet;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->at(i);
		if (dwListID > 0) {
			if (pInfo->m_dwListID != dwListID) {
				continue;
		}
	}
		dwSizeTmp	= pInfo->GetSendDataSize();
		pDataTmp	= pInfo->GetSendData();
		CopyMemoryRenew(pPos, pDataTmp, dwSizeTmp, pPos);
		SAFE_DELETE_ARRAY(pDataTmp);
	}

Exit:
	return pRet;
}

PBYTE CLibInfoEfcBalloon::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;
	DWORD dwListID;
	PCInfoEfcBalloon pInfoEfcBalloon;

	pRet = pSrc;

	pDataTmp = pSrc;
	while (1) {
		CopyMemory(&dwListID, pDataTmp, sizeof(dwListID));
		if (dwListID == 0) {
			pDataTmp += sizeof(DWORD);
			break;
	}
		pInfoEfcBalloon = new CInfoEfcBalloon;
		pDataTmp = pInfoEfcBalloon->SetSendData(pDataTmp);
		Add(pInfoEfcBalloon);
	}
	pRet = pDataTmp;

	return pRet;
}

DWORD CLibInfoEfcBalloon::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoEfcBalloon pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwEfcBalloonID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
	}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

