/// @file LibInfoMapObject.cpp
/// @brief マップパーツ情報ライブラリクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/11/01
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "LibInfoMapObject.h"

CLibInfoMapObject::CLibInfoMapObject()
{
	m_paInfo = NULL;
}

CLibInfoMapObject::~CLibInfoMapObject()
{
	Destroy();
}

void CLibInfoMapObject::Create(void)
{
	m_paInfo = new ARRAYMAPOBJECT;
}

void CLibInfoMapObject::Destroy(void)
{
	DeleteAll();
	SAFE_DELETE(m_paInfo);
}

PCInfoBase CLibInfoMapObject::GetNew(void)
{
	return new CInfoMapObject;
}

int CLibInfoMapObject::GetCount(void)
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

void CLibInfoMapObject::Add(PCInfoBase pInfo)
{
	PCInfoMapObject pInfoMapObject;

	pInfoMapObject = (PCInfoMapObject)pInfo;
	if (pInfoMapObject->m_dwObjectID == 0) {
		pInfoMapObject->m_dwObjectID = GetNewID();
	}

	m_paInfo->Add(pInfoMapObject);
}

void CLibInfoMapObject::Delete(
	int nNo)	// [in] 配列番号
{
	PCInfoMapObject pInfo;

	pInfo = m_paInfo->at(nNo);
	SAFE_DELETE(pInfo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paInfo->size()))) {
		m_paInfo->erase(m_paInfo->begin() + nNo);
	}
}

void CLibInfoMapObject::Delete(
	DWORD dwObjectID)	// [in] マップオブジェクトID
{
	int i, nCount, nNo;
	PCInfoMapObject pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwObjectID != dwObjectID) {
			continue;
	}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete(nNo);
	}
}

void CLibInfoMapObject::DeleteAll(void)
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

void CLibInfoMapObject::Merge(CLibInfoMapObject *pSrc)
{
	int i, nCount;
	PCInfoMapObject pInfoTmp, pInfoSrc;

	nCount = pSrc->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoMapObject)pSrc->GetPtr(i);
		pInfoTmp = (PCInfoMapObject)GetPtr(pInfoSrc->m_dwObjectID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoMapObject)GetNew();
			pInfoTmp->Copy(pInfoSrc);
			Add(pInfoTmp);
	}
		pInfoTmp->Copy(pInfoSrc);
	}
}

PCInfoBase CLibInfoMapObject::GetPtr(int nNo)
{
	return m_paInfo->at(nNo);
}

PCInfoBase CLibInfoMapObject::GetPtr(
	DWORD dwObjectID)	// [in] マップオブジェクトID
{
	int i, nCount;
	PCInfoMapObject pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwObjectID != dwObjectID) {
			continue;
	}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}

DWORD CLibInfoMapObject::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwSize;
	PCInfoMapObject pInfoMapObject;

	dwSize = 0;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoMapObject = (PCInfoMapObject)GetPtr(i);
		dwSize += pInfoMapObject->GetSendDataSize();
	}
	// 終端用
	dwSize += sizeof(DWORD);

	return dwSize;
}

PBYTE CLibInfoMapObject::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataPos, pDataTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoMapObject pInfoMapObject;

	dwSize	= GetSendDataSize();
	pData	= new BYTE[dwSize];
	ZeroMemory(pData, dwSize);

	pDataPos = pData;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoMapObject = (PCInfoMapObject)GetPtr(i);

		dwSizeTmp	= pInfoMapObject->GetSendDataSize();
		pDataTmp	= pInfoMapObject->GetSendData();
		CopyMemoryRenew(pDataPos, pDataTmp, dwSizeTmp, pDataPos);
		SAFE_DELETE_ARRAY(pDataTmp);
	}

	return pData;
}

PBYTE CLibInfoMapObject::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp;
	DWORD dwTmp;
	PCInfoMapObject pInfoMapObject, pInfoMapObjectTmp;

	pDataTmp = pSrc;

	while (1) {
		CopyMemory(&dwTmp, pDataTmp, sizeof(DWORD));
		if (dwTmp == 0) {
			pDataTmp += sizeof(DWORD);
			break;
	}
		pInfoMapObjectTmp = (PCInfoMapObject)GetNew();
		pDataTmp = pInfoMapObjectTmp->SetSendData(pDataTmp);

		pInfoMapObject = (PCInfoMapObject)GetPtr(pInfoMapObjectTmp->m_dwObjectID);
		if (pInfoMapObject) {
			pInfoMapObject->Copy(pInfoMapObjectTmp);
			SAFE_DELETE(pInfoMapObjectTmp);
	} else {
			Add(pInfoMapObjectTmp);
	}
	}

	return pDataTmp;
}

DWORD CLibInfoMapObject::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMapObject pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwObjectID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
	}
	}

	return dwRet;
}

