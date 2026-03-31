/// @file LibInfoSkill.cpp
/// @brief スキル情報ライブラリクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/04
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "InfoSkillMOVEATACK.h"
#include "InfoSkillHEAL.h"
#include "LibInfoSkill.h"

CLibInfoSkill::CLibInfoSkill()
{
	m_paInfo = NULL;
}

CLibInfoSkill::~CLibInfoSkill()
{
	Destroy();
}

void CLibInfoSkill::Create(void)
{
	m_paInfo = new ARRAYSKILLBASEINFO;
}

void CLibInfoSkill::Destroy(void)
{
	DeleteAll();
	SAFE_DELETE(m_paInfo);
}

PCInfoBase CLibInfoSkill::GetNew(void)
{
	return (PCInfoBase)GetNew(0, 0);
}

PCInfoBase CLibInfoSkill::GetNew(int nTypeMain, int nTypeSub)
{
	PCInfoSkillBase pInfo;

	pInfo = NULL;

	switch (nTypeMain) {
	case SKILLTYPEMAIN_NONE:	// 能力
		break;
	case SKILLTYPEMAIN_BATTLE:	// 戦闘
		switch (nTypeSub) {
		case SKILLTYPESUB_BATTLE_MOVEATACK:	// 移動して攻撃
			pInfo = new CInfoSkillMOVEATACK;
			break;
		case SKILLTYPESUB_BATTLE_HEAL:	// 回復
			pInfo = new CInfoSkillHEAL;
			break;
	}
		break;
	case SKILLTYPEMAIN_LIFE:	// 生活
		break;
	}

	if (pInfo == NULL) {
		pInfo = new CInfoSkillBase;
	}
	pInfo->m_nTypeMain = nTypeMain;
	pInfo->m_nTypeSub  = nTypeSub;

	return pInfo;
}

int CLibInfoSkill::GetCount(void)
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

void CLibInfoSkill::Add(PCInfoBase pInfo)
{
	PCInfoSkillBase pInfoSkillBase;

	pInfoSkillBase = (PCInfoSkillBase)pInfo;
	if (pInfoSkillBase->m_dwSkillID == 0) {
		pInfoSkillBase->m_dwSkillID = GetNewID();
	}

	m_paInfo->Add(pInfoSkillBase);
}

void CLibInfoSkill::Delete(
	int nNo)	// [in] 配列番号
{
	PCInfoSkillBase pInfo;

	pInfo = m_paInfo->at(nNo);
	SAFE_DELETE(pInfo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paInfo->size()))) {
		m_paInfo->erase(m_paInfo->begin() + nNo);
	}
}

void CLibInfoSkill::Delete(
	DWORD dwSkillID)	// [in] スキルID
{
	int i, nCount, nNo;
	PCInfoSkillBase pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwSkillID != dwSkillID) {
			continue;
	}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete(nNo);
	}
}

void CLibInfoSkill::DeleteAll(void)
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

void CLibInfoSkill::Merge(CLibInfoSkill *pSrc)
{
	int i, nCount;
	PCInfoSkillBase pInfoTmp, pInfoSrc;

	nCount = pSrc->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoSkillBase)pSrc->GetPtr(i);
		pInfoTmp = (PCInfoSkillBase)GetPtr(pInfoSrc->m_dwSkillID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoSkillBase)GetNew(pInfoSrc->m_nTypeMain, pInfoSrc->m_nTypeSub);
			pInfoTmp->Copy(pInfoSrc);
			Add(pInfoTmp);
	}
		pInfoTmp->Copy(pInfoSrc);
	}
}

CInfoSkillBase *CLibInfoSkill::Renew(CInfoSkillBase *pSrc)
{
	int i, nCount;
	PCInfoSkillBase pRet, pInfo, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwSkillID != pSrc->m_dwSkillID) {
			continue;
	}
		pInfo = (PCInfoSkillBase)GetNew(pSrc->m_nTypeMain, pSrc->m_nTypeSub);
		pInfo->Copy(pSrc);
		pInfo->m_dwSkillID = pInfoTmp->m_dwSkillID;

                SAFE_DELETE(pInfoTmp);
                (*m_paInfo)[i] = pInfo;
		pRet = pInfo;
		break;
	}

	return pRet;
}

PCInfoBase CLibInfoSkill::GetPtr(int nNo)
{
	return m_paInfo->at(nNo);
}

PCInfoBase CLibInfoSkill::GetPtr(
	DWORD dwSkillID)	// [in] スキルID
{
	int i, nCount;
	PCInfoSkillBase pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwSkillID != dwSkillID) {
			continue;
	}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}

PCInfoBase CLibInfoSkill::RenewInfo(int nNo)
{
	PCInfoSkillBase pInfoSkillBase, pInfoTmp;

	pInfoSkillBase = (PCInfoSkillBase)GetPtr(nNo);

        pInfoTmp = (PCInfoSkillBase)GetNew(pInfoSkillBase->m_nTypeMain, pInfoSkillBase->m_nTypeSub);
        pInfoTmp->Copy(pInfoSkillBase);
        (*m_paInfo)[nNo] = pInfoTmp;
	SAFE_DELETE(pInfoSkillBase);

	return pInfoTmp;
}

DWORD CLibInfoSkill::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwSize;
	PCInfoSkillBase pInfoSkillBase;

	dwSize = 0;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoSkillBase = (PCInfoSkillBase)GetPtr(i);
		dwSize += pInfoSkillBase->GetSendDataSize();
	}
	// 終端用
	dwSize += sizeof(DWORD);

	return dwSize;
}

PBYTE CLibInfoSkill::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataPos, pDataTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoSkillBase pInfoSkillBase;

	dwSize	= GetSendDataSize();
	pData	= new BYTE[dwSize];
	ZeroMemory(pData, dwSize);

	pDataPos = pData;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoSkillBase = (PCInfoSkillBase)GetPtr(i);

		dwSizeTmp	= pInfoSkillBase->GetSendDataSize();
		pDataTmp	= pInfoSkillBase->GetSendData();
		CopyMemoryRenew(pDataPos, pDataTmp, dwSizeTmp, pDataPos);
		SAFE_DELETE_ARRAY(pDataTmp);
	}

	return pData;
}

PBYTE CLibInfoSkill::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp, pDataTmpBack;
	DWORD dwTmp;
	CInfoSkillBase InfoTmp, *pInfoSkillBase, *pInfoSkillBaseTmp;

	pDataTmp = pSrc;

	while (1) {
		CopyMemory(&dwTmp, pDataTmp, sizeof(DWORD));
		if (dwTmp == 0) {
			pDataTmp += sizeof(DWORD);
			break;
	}
		pDataTmpBack = pDataTmp;
		InfoTmp.SetSendData(pDataTmp);
		pInfoSkillBaseTmp = (PCInfoSkillBase)GetNew(InfoTmp.m_nTypeMain, InfoTmp.m_nTypeSub);
		pDataTmp = pInfoSkillBaseTmp->SetSendData(pDataTmpBack);

		pInfoSkillBase = (PCInfoSkillBase)GetPtr(pInfoSkillBaseTmp->m_dwSkillID);
		if (pInfoSkillBase) {
			pInfoSkillBase->Copy(pInfoSkillBaseTmp);
			SAFE_DELETE(pInfoSkillBaseTmp);
	} else {
			Add(pInfoSkillBaseTmp);
	}
	}

	return pDataTmp;
}

DWORD CLibInfoSkill::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoSkillBase pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwSkillID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
	}
	}

	return dwRet;
}

