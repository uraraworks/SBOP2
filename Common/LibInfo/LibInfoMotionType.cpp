/// @file LibInfoMotionType.cpp
/// @brief モーション情報ライブラリ基底クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/05/26
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "LibInfoMotionType.h"

CLibInfoMotionType::CLibInfoMotionType()
{
	m_dwNewIDTmp	= 0;
	m_paInfo	= NULL;
}

CLibInfoMotionType::~CLibInfoMotionType()
{
	Destroy();
}

void CLibInfoMotionType::Create(void)
{
	m_paInfo = new ARRAYMOTIONTYPEINFO;
}

void CLibInfoMotionType::Destroy(void)
{
	DeleteAll();
	SAFE_DELETE(m_paInfo);
}

PCInfoBase CLibInfoMotionType::GetNew(void)
{
	PCInfoMotionType pInfoMotion;

	pInfoMotion = new CInfoMotionType;

	return pInfoMotion;
}

PCInfoBase CLibInfoMotionType::GetPtr(int nNo)
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

PCInfoBase CLibInfoMotionType::GetPtr(DWORD dwMotionTypeID)
{
	int i, nCount;
	PCInfoMotionType pRet, pInfo;

	pRet = NULL;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->at(i);
		if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
			continue;
	}
		pRet = pInfo;
		break;
	}

	return pRet;
}

int CLibInfoMotionType::GetCount(void)
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

void CLibInfoMotionType::Add(PCInfoBase pInfo)
{
	PCInfoMotionType pMotionTypeInfo;

	pMotionTypeInfo = (PCInfoMotionType)pInfo;
	if (pMotionTypeInfo->m_dwMotionTypeID == 0) {
		pMotionTypeInfo->m_dwMotionTypeID = GetNewID();
	}

	m_paInfo->Add(pMotionTypeInfo);
}

void CLibInfoMotionType::Delete(
	int nNo)	// [in] 配列番号
{
	PCInfoMotionType pInfo;

	pInfo = m_paInfo->at(nNo);
	SAFE_DELETE(pInfo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paInfo->size()))) {
		m_paInfo->erase(m_paInfo->begin() + nNo);
	}
}

void CLibInfoMotionType::Delete(
	DWORD dwMotionTypeID)	// [in] モーション種別ID
{
	int i, nCount, nNo;
	PCInfoMotionType pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwMotionTypeID != dwMotionTypeID) {
			continue;
	}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete(nNo);
	}
}

void CLibInfoMotionType::DeleteAll(void)
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

DWORD CLibInfoMotionType::GetSendDataSize(DWORD dwMotionTypeID)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMotionType pInfo;

	dwRet = 0;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->at(i);
		if (dwMotionTypeID != 0) {
			if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
				continue;
		}
	}
		dwRet += pInfo->GetSendDataSize();
	}

	dwRet += sizeof(DWORD);	// 終端分

	return dwRet;
}

PBYTE CLibInfoMotionType::GetSendData(DWORD dwMotionTypeID)
{
	PBYTE pRet, pDataTmp, pPos;
	DWORD dwSize, dwSizeTmp;
	int i, nCount;
	PCInfoMotionType pInfo;

	pRet = NULL;

	dwSize = GetSendDataSize(dwMotionTypeID);
	if (dwSize == 0) {
		goto Exit;
	}
	pRet = ZeroNew(dwSize);
	pPos = pRet;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->at(i);
		if (dwMotionTypeID != 0) {
			if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
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

PBYTE CLibInfoMotionType::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;
	DWORD dwMotionTypeID;
	PCInfoMotionType pInfoMotion;

	pRet = pSrc;

	pDataTmp = pSrc;
	while (1) {
		CopyMemory(&dwMotionTypeID, pDataTmp, sizeof(dwMotionTypeID));
		if (dwMotionTypeID == 0) {
			pDataTmp += sizeof(DWORD);
			break;
	}
		pInfoMotion = (PCInfoMotionType)GetPtr(dwMotionTypeID);
		if (pInfoMotion == NULL) {
			pInfoMotion = new CInfoMotionType;
			pDataTmp = pInfoMotion->SetSendData(pDataTmp);
			Add(pInfoMotion);
	} else {
			pDataTmp = pInfoMotion->SetSendData(pDataTmp);
	}
	}
	pRet = pDataTmp;

	return pRet;
}

DWORD CLibInfoMotionType::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMotionType pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwMotionTypeID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
	}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

