/// @file LibInfoMapObjectData.cpp
/// @brief マップオブジェクト配置データライブラリクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/11/03
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "InfoMapObjectData.h"
#include "LibInfoMapObjectData.h"

CLibInfoMapObjectData::CLibInfoMapObjectData()
{
	m_dwNewIDTmp	= 0;
	m_paInfo	= NULL;
}

CLibInfoMapObjectData::~CLibInfoMapObjectData()
{
	Destroy();
}

void CLibInfoMapObjectData::Create(void)
{
	m_paInfo = new ARRAYMAPOBJECTDATAINFO;
}

void CLibInfoMapObjectData::Destroy(void)
{
	DeleteAll();
	SAFE_DELETE(m_paInfo);
}

PCInfoBase CLibInfoMapObjectData::GetNew(void)
{
	PCInfoMapObjectData pInfo;

	pInfo = new CInfoMapObjectData;

	return pInfo;
}

int CLibInfoMapObjectData::GetCount(void)
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

void CLibInfoMapObjectData::Add(PCInfoBase pInfo)
{
	PCInfoMapObjectData pInfoMapObjectData;

	pInfoMapObjectData = (PCInfoMapObjectData)pInfo;
	if (pInfoMapObjectData->m_dwDataID == 0) {
		pInfoMapObjectData->m_dwDataID = GetNewID();
	}
	m_paInfo->Add(pInfoMapObjectData);
}

void CLibInfoMapObjectData::Delete(
	int nNo)	// [in] 配列番号
{
	PCInfoMapObjectData pInfo;

	pInfo = m_paInfo->at(nNo);
	SAFE_DELETE(pInfo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paInfo->size()))) {
		m_paInfo->erase(m_paInfo->begin() + nNo);
	}
}

void CLibInfoMapObjectData::Delete(DWORD dwDataID)
{
	int i, nCount;
	PCInfoMapObjectData pInfoTmp;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwDataID != dwDataID) {
			continue;
	}
		Delete(i);
		break;
	}
}

void CLibInfoMapObjectData::DeleteAll(void)
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

void CLibInfoMapObjectData::Merge(CLibInfoMapObjectData *pSrc)
{
	int i, nCount;
	PCInfoMapObjectData pInfoTmp, pInfoSrc, pInfoDst;

	nCount = pSrc->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoMapObjectData)pSrc->GetPtr(i);
		pInfoTmp = (PCInfoMapObjectData)GetPtr(pInfoSrc->m_dwDataID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoMapObjectData)GetNew();
			pInfoTmp->Copy(pInfoSrc);
			Add(pInfoTmp);
	} else {
			pInfoDst = (PCInfoMapObjectData)GetNew();
			pInfoDst->Copy(pInfoSrc);
			Renew(pInfoSrc->m_dwDataID, pInfoDst);
	}
	}
}

CInfoMapObjectData *CLibInfoMapObjectData::Renew(DWORD dwDataID, CInfoMapObjectData *pSrc)
{
	int i, nCount;
	PCInfoMapObjectData pRet, pInfo, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwDataID != dwDataID) {
			continue;
	}
		pInfo = (PCInfoMapObjectData)GetNew();
		pInfo->Copy(pSrc);
		pInfo->m_dwDataID = pInfoTmp->m_dwDataID;

                SAFE_DELETE(pInfoTmp);
                (*m_paInfo)[i] = pInfo;
		pRet = pInfo;
		break;
	}

	return pRet;
}

PCInfoBase CLibInfoMapObjectData::GetPtr(int nNo)
{
	return m_paInfo->at(nNo);
}

PCInfoBase CLibInfoMapObjectData::GetPtr(DWORD dwDataID)
{
	int i, nCount;
	PCInfoMapObjectData pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwDataID == dwDataID) {
			pRet = pInfoTmp;
			break;
	}
	}

	return pRet;
}

DWORD CLibInfoMapObjectData::GetDataSize(void)
{
	int i, j, nCount, nCount2;
	DWORD dwRet;
	PCInfoMapObjectData pInfo;

	dwRet = 0;

	dwRet += sizeof(int);	// データ数
	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->at(i);
		dwRet += sizeof(int);	// 要素数
		nCount2 = pInfo->GetElementCount();
		for (j = 0; j < nCount2; j ++) {
			dwRet += (strlen(pInfo->GetName(j)) + 1);	// 要素名
			dwRet += sizeof(DWORD);	// データサイズ
			dwRet += pInfo->GetDataSizeNo(j);	// データ
	}
	}

	return dwRet;
}

PBYTE CLibInfoMapObjectData::GetWriteData(PDWORD pdwSize)
{
	int i, j, nCount, nCount2;
	PBYTE pRet, pRetTmp, pTmp;
	DWORD dwSize, dwTmp;
	PCInfoMapObjectData pInfo;

	pRet	= NULL;
	dwSize	= GetDataSize();
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}

	pRet = ZeroNew(dwSize);
	pRetTmp = pRet;

	nCount = m_paInfo->size();
	CopyMemoryRenew(pRetTmp, &nCount, sizeof(nCount), pRetTmp);	// データ数
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->at(i);
		nCount2 = pInfo->GetElementCount();
		CopyMemoryRenew(pRetTmp, &nCount2, sizeof(nCount2), pRetTmp);	// 要素数

		for (j = 0; j < nCount2; j ++) {
			strcpyRenew((LPSTR)pRetTmp, pInfo->GetName(j), pRetTmp);	// 要素名
			dwTmp = pInfo->GetDataSizeNo(j);
			CopyMemoryRenew(pRetTmp, &dwTmp, sizeof(dwTmp), pRetTmp);	// データサイズ
			pTmp = pInfo->GetWriteData(j, &dwTmp);
			CopyMemoryRenew(pRetTmp, pTmp, dwTmp, pRetTmp);	// データ
			SAFE_DELETE_ARRAY(pTmp);
	}
	}

Exit:
	return pRet;
}

DWORD CLibInfoMapObjectData::ReadElementData(PBYTE pSrc)
{
	int i, j, nNo, nCount, nCount2;
	PBYTE pDst, pSrcTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoMapObjectData pInfo;
	CmyString strTmp;

	pDst	= NULL;
	dwSize	= 0;

	DeleteAll();

	pSrcTmp = pSrc;
	CopyMemoryRenew(&nCount, pSrcTmp, sizeof(nCount), pSrcTmp);	// データ数
	for (i = 0; i < nCount; i ++) {
		CopyMemoryRenew(&nCount2, pSrcTmp, sizeof(nCount2), pSrcTmp);	// 要素数

		pInfo = (PCInfoMapObjectData)GetNew();
		for (j = 0; j < nCount2; j ++) {
			StoreRenew(strTmp, (LPCSTR)pSrcTmp, pSrcTmp);	// 要素名
			CopyMemoryRenew(&dwSizeTmp, pSrcTmp, sizeof(dwSizeTmp), pSrcTmp);	// データサイズ
			nNo = pInfo->GetElementNo((LPCSTR)strTmp);
			if (nNo >= 0) {
				dwSizeTmp = pInfo->ReadElementData(pSrcTmp, nNo);
		}
			pSrcTmp += dwSizeTmp;
	}
		Add(pInfo);
	}

	return (DWORD)(pSrcTmp - pSrc);
}

DWORD CLibInfoMapObjectData::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoMapObjectData pInfo;

	dwRet = dwSize = 0;

	// データ数分のサイズ
	dwSize += sizeof(DWORD);

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapObjectData)GetPtr(i);

		dwSize += pInfo->GetSendDataSize();
	}

	dwRet = dwSize;
	return dwRet;
}

PBYTE CLibInfoMapObjectData::GetSendData(void)
{
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwCount, dwSize, dwSizeTmp, dwOffset;
	PCInfoMapObjectData pInfo;

	pRet = NULL;

	dwSize	= GetSendDataSize();
	pData	= ZeroNew(dwSize);
	dwOffset	= 0;

	// データ数を書き込み
	dwCount = (DWORD)GetCount();
	CopyMemory(pData, &dwCount, sizeof(dwCount));
	dwOffset += sizeof(dwCount);

	// 配置データを書き込み
	nCount = (int)dwCount;
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapObjectData)GetPtr(i);

		dwSizeTmp	= pInfo->GetSendDataSize();
		pDataTmp	= pInfo->GetSendData();
		CopyMemory(&pData[dwOffset], pDataTmp, dwSizeTmp);
		dwOffset += dwSizeTmp;

		SAFE_DELETE_ARRAY(pDataTmp);
	}

	pRet = pData;
	return pRet;
}

PBYTE CLibInfoMapObjectData::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	DWORD dwCount;
	PBYTE pRet, pDataTmp, pDataTmpBack;
	CInfoMapObjectData InfoTmp, *pInfo;

	pRet	= pSrc;
	pDataTmp	= pSrc;

	DeleteAll();

	// データ数を読み込み
	CopyMemory(&dwCount, pDataTmp, sizeof(dwCount));
	nCount	= (int)dwCount;
	pDataTmp	+= sizeof(dwCount);

	for (i = 0; i < nCount; i ++) {
		pDataTmpBack = pDataTmp;
		InfoTmp.SetSendData(pDataTmp);
		pInfo = (PCInfoMapObjectData)GetNew();
		pDataTmp = pDataTmpBack;
		pDataTmp = pInfo->SetSendData(pDataTmp);
		Add(pInfo);
	}

	pRet = pDataTmp;
	return pRet;
}

DWORD CLibInfoMapObjectData::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMapObjectData pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwDataID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
	}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

