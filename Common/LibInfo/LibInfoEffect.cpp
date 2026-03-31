/// @file LibInfoEffect.cpp
/// @brief エフェクト情報ライブラリクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/26
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "LibInfoEffect.h"

CLibInfoEffect::CLibInfoEffect()
{
	m_paInfo = NULL;
}

CLibInfoEffect::~CLibInfoEffect()
{
	Destroy();
}

void CLibInfoEffect::Create(void)
{
	m_paInfo = new ARRAYEFFECT;
}

void CLibInfoEffect::Destroy(void)
{
	DeleteAll();
	SAFE_DELETE(m_paInfo);
}

BOOL CLibInfoEffect::Proc(void)
{
	BOOL bRet;
	int i, nCount;
	PCInfoEffect pInfo;

	bRet = FALSE;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoEffect)GetPtr(i);
		bRet |= pInfo->TimerProc(timeGetTime());
	}

	return bRet;
}

PCInfoBase CLibInfoEffect::GetNew(void)
{
	return new CInfoEffect;
}

int CLibInfoEffect::GetCount(void)
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

void CLibInfoEffect::Add(PCInfoBase pInfo)
{
	PCInfoEffect pEffectInfo;

	pEffectInfo = (PCInfoEffect)pInfo;
	if (pEffectInfo->m_dwEffectID == 0) {
		pEffectInfo->m_dwEffectID = GetNewID();
	}

	m_paInfo->Add(pEffectInfo);
}

void CLibInfoEffect::Delete(
	int nNo)	// [in] 配列番号
{
	PCInfoEffect pInfo;

	pInfo = m_paInfo->at(nNo);
	SAFE_DELETE(pInfo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paInfo->size()))) {
		m_paInfo->erase(m_paInfo->begin() + nNo);
	}
}

void CLibInfoEffect::Delete(
	DWORD dwEffectID)	// [in] エフェクトID
{
	int i, nCount, nNo;
	PCInfoEffect pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwEffectID != dwEffectID) {
			continue;
	}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete(nNo);
	}
}

void CLibInfoEffect::DeleteAll(void)
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

void CLibInfoEffect::GetName(
	DWORD dwEffectID,	// [in]  エフェクトID
	CmyString &strDst)	// [out] エフェクト名
{
	int i, nCount;
	PCInfoEffect pInfo;

	strDst.Empty();

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->at(i);
		if (pInfo->m_dwEffectID != dwEffectID) {
			continue;
	}
		strDst = pInfo->m_strName;
		break;
	}
}

PCInfoBase CLibInfoEffect::GetPtr(int nNo)
{
	PCInfoBase pRet;

	pRet = NULL;
	if ((nNo < 0) || (nNo >= static_cast<int>(m_paInfo->size()))) {
		goto Exit;
	}

	pRet = m_paInfo->at(nNo);
Exit:
	return pRet;
}

PCInfoBase CLibInfoEffect::GetPtr(
	DWORD dwEffectID)	// [in] エフェクトID
{
	int i, nCount;
	PCInfoEffect pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwEffectID != dwEffectID) {
			continue;
	}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}

DWORD CLibInfoEffect::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwSize;
	PCInfoEffect pInfoEffect;

	dwSize = 0;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoEffect = (PCInfoEffect)GetPtr(i);
		dwSize += pInfoEffect->GetSendDataSize();
	}
	// 終端用
	dwSize += sizeof(DWORD);

	return dwSize;
}

PBYTE CLibInfoEffect::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataPos, pDataTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoEffect pInfoEffect;

	dwSize	= GetSendDataSize();
	pData	= new BYTE[dwSize];
	ZeroMemory(pData, dwSize);

	pDataPos = pData;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoEffect = (PCInfoEffect)GetPtr(i);

		dwSizeTmp	= pInfoEffect->GetSendDataSize();
		pDataTmp	= pInfoEffect->GetSendData();
		CopyMemoryRenew(pDataPos, pDataTmp, dwSizeTmp, pDataPos);
		SAFE_DELETE_ARRAY(pDataTmp);
	}

	return pData;
}

PBYTE CLibInfoEffect::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp;
	DWORD dwTmp;
	PCInfoEffect pInfoEffect, pInfoEffectTmp;

	pDataTmp = pSrc;

	while (1) {
		CopyMemory(&dwTmp, pDataTmp, sizeof(DWORD));
		if (dwTmp == 0) {
			pDataTmp += sizeof(DWORD);
			break;
	}
		pInfoEffectTmp = (PCInfoEffect)GetNew();
		pDataTmp = pInfoEffectTmp->SetSendData(pDataTmp);

		pInfoEffect = (PCInfoEffect)GetPtr(pInfoEffectTmp->m_dwEffectID);
		if (pInfoEffect) {
			pInfoEffect->Copy(pInfoEffectTmp);
			SAFE_DELETE(pInfoEffectTmp);
	} else {
			Add(pInfoEffectTmp);
	}
	}

	return pDataTmp;
}

DWORD CLibInfoEffect::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoEffect pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwEffectID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
	}
	}

	return dwRet;
}

