/// @file LibInfoMapBase.cpp
/// @brief マップ情報基底クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/10/15
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "LibInfoMapParts.h"
#include "LibInfoMapBase.h"

CLibInfoMapBase::CLibInfoMapBase()
{
	m_paInfo = NULL;
	m_pLibInfoMapParts = NULL;
}

CLibInfoMapBase::~CLibInfoMapBase()
{
	Destroy();
}

void CLibInfoMapBase::Create(CLibInfoMapParts *pLibInfoMapParts)
{
	m_paInfo = new ARRAYINFOMAPBASE;

	m_pLibInfoMapParts = pLibInfoMapParts;
}

void CLibInfoMapBase::Destroy(void)
{
	DeleteAll();
	SAFE_DELETE(m_paInfo);
}

PCInfoBase CLibInfoMapBase::GetNew(void)
{
	PCInfoMapBase pInfoMapBase;

	pInfoMapBase = new CInfoMapBase;
	pInfoMapBase->Create(m_pLibInfoMapParts);

	return pInfoMapBase;
}

void CLibInfoMapBase::RenewMapEvent(void)
{
	int i, nCount;
	PCInfoMapBase pInfoMapBase;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoMapBase = (PCInfoMapBase)GetPtr(i);
		pInfoMapBase->RenewMapEvent();
	}
}

void CLibInfoMapBase::RenewHitTmp(void)
{
	int i, nCount;
	PCInfoMapBase pInfoMapBase;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoMapBase = (PCInfoMapBase)GetPtr(i);
		pInfoMapBase->RenewHitTmp();
	}
}

void CLibInfoMapBase::SetMapObject(CLibInfoMapObject *pLibInfo)
{
	int i, nCount;
	PCInfoMapBase pInfoMapBase;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoMapBase = (PCInfoMapBase)GetPtr(i);
		pInfoMapBase->m_pLibInfoMapObject = pLibInfo;
	}
}

int CLibInfoMapBase::GetCount(void)
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

void CLibInfoMapBase::Add(PCInfoBase pInfo)
{
	PCInfoMapBase pMapInfo;

	pMapInfo = (PCInfoMapBase)pInfo;
	if (pMapInfo->m_dwMapID == 0) {
		pMapInfo->m_dwMapID = GetNewID();
	}

	m_paInfo->Add(pMapInfo);
}

void CLibInfoMapBase::Delete(
	int nNo)	// [in] 配列番号
{
	PCInfoMapBase pInfo;

	pInfo = m_paInfo->at(nNo);
	SAFE_DELETE(pInfo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paInfo->size()))) {
		m_paInfo->erase(m_paInfo->begin() + nNo);
	}
}

void CLibInfoMapBase::Delete(
	DWORD dwMapID)	// [in] マップID
{
	int i, nCount, nNo;
	PCInfoMapBase pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwMapID != dwMapID) {
			continue;
	}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete(nNo);
	}
}

void CLibInfoMapBase::DeleteAll(void)
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

void CLibInfoMapBase::DeleteParts(DWORD dwPartsID)
{
	int i, nCount;
	PCInfoMapBase pInfoTmp;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		pInfoTmp->DeleteParts(dwPartsID);
	}
}

void CLibInfoMapBase::DeleteShadow(DWORD dwShadowID)
{
	int i, nCount;
	PCInfoMapBase pInfoTmp;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		pInfoTmp->DeleteShadow(dwShadowID);
	}
}

PCInfoBase CLibInfoMapBase::GetPtr(int nNo)
{
	return m_paInfo->at(nNo);
}

PCInfoBase CLibInfoMapBase::GetPtr(
	DWORD dwMapID)	// [in] マップID
{
	int i, nCount;
	PCInfoMapBase pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwMapID != dwMapID) {
			continue;
	}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}

DWORD CLibInfoMapBase::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMapBase pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwMapID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
	}
	}

	return dwRet;
}

