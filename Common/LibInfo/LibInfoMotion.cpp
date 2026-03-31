/// @file LibInfoMotion.cpp
/// @brief モーション情報ライブラリ基底クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/10/29
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "InfoMotion.h"
#include "LibInfoMotion.h"

CLibInfoMotion::CLibInfoMotion()
{
	m_dwNewIDTmp	= 0;
	m_paInfo	= NULL;
}

CLibInfoMotion::~CLibInfoMotion()
{
	Destroy();
}

void CLibInfoMotion::Create(void)
{
	m_paInfo = new ARRAYMOTIONINFO;
}

void CLibInfoMotion::Destroy(void)
{
	DeleteAll();
	SAFE_DELETE(m_paInfo);
}

PCInfoBase CLibInfoMotion::GetNew(void)
{
	PCInfoMotion pInfoMotion;

	pInfoMotion = new CInfoMotion;

	return pInfoMotion;
}

PCInfoBase CLibInfoMotion::GetPtr(int nNo)
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

void CLibInfoMotion::Revice(void)
{
	int i, j, nCount;
	PCInfoMotion pInfoTmp;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = (PCInfoMotion)GetPtr(i);

		if (pInfoTmp->m_anDrawList.size() > 0) {
			continue;
	}
		for (j = 0; j < 4; j ++) {
			pInfoTmp->m_anDrawList.push_back(j);
	}
	}
}

int CLibInfoMotion::GetCount(void)
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

void CLibInfoMotion::Add(PCInfoBase pInfo)
{
	PCInfoMotion pItemInfo;

	pItemInfo = (PCInfoMotion)pInfo;
	if (pItemInfo->m_dwMotionID == 0) {
		pItemInfo->m_dwMotionID = GetNewID();
	}

	m_paInfo->Add(pItemInfo);
}

void CLibInfoMotion::Add(PCInfoBase pInfo, int nNo)
{
	PCInfoMotion pItemInfo;

	pItemInfo = (PCInfoMotion)pInfo;
	if (pItemInfo->m_dwMotionID == 0) {
		pItemInfo->m_dwMotionID = GetNewID();
	}

	m_paInfo->insert(m_paInfo->begin() + nNo, pItemInfo);
}

void CLibInfoMotion::AddType(DWORD dwMotionTypeID)
{
	int i;
	PCInfoMotion pInfoTmp;

	for (i = CHARMOTIONLISTID_NONE + 1; i < CHARMOTIONLISTID_MAX; i ++) {
		pInfoTmp = (PCInfoMotion)GetNew();
		pInfoTmp->m_dwMotionListID	= i;
		pInfoTmp->m_dwMotionTypeID	= dwMotionTypeID;
		Add(pInfoTmp);
	}
}

void CLibInfoMotion::Delete(
	int nNo)	// [in] 配列番号
{
	PCInfoMotion pInfo;

	pInfo = m_paInfo->at(nNo);
	SAFE_DELETE(pInfo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paInfo->size()))) {
		m_paInfo->erase(m_paInfo->begin() + nNo);
	}
}

void CLibInfoMotion::Delete(
	DWORD dwMotionID)	// [in] モーションID
{
	int i, nCount, nNo;
	PCInfoMotion pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwMotionID != dwMotionID) {
			continue;
	}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete(nNo);
	}
}

void CLibInfoMotion::DeleteAll(void)
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

void CLibInfoMotion::SetList(DWORD dwMotionTypeID, DWORD dwMotionListID, CLibInfoMotion *pSrc)
{
	int i, nCount;
	PCInfoMotion pInfo, pInfoTmp;

	// まずは対象となるリストIDの情報を削除
	nCount = m_paInfo->size();
	for (i = nCount - 1; i >= 0; i --) {
		pInfo = m_paInfo->at(i);
		if (dwMotionTypeID != 0) {
			if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
				continue;
		}
	}
		if (dwMotionListID != 0) {
			if (pInfo->m_dwMotionListID != dwMotionListID) {
				continue;
		}
	}
		Delete(i);
	}

	nCount = pSrc->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMotion)pSrc->GetPtr(i);
		if (dwMotionTypeID != 0) {
			if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
				continue;
		}
	}
		if (dwMotionListID != 0) {
			if (pInfo->m_dwMotionListID != dwMotionListID) {
				continue;
		}
	}
		pInfoTmp = new CInfoMotion;
		pInfoTmp->Copy(pInfo);
		Add(pInfoTmp);
	}
}

BOOL CLibInfoMotion::IsDirection(DWORD dwMotionListID)
{
	BOOL bRet;

	bRet = TRUE;
	switch (dwMotionListID) {
	case CHARMOTIONLISTID_STAND:	// 立ち
	case CHARMOTIONLISTID_STAND_SIMPLE:	// 立ち(上下左右1コマずつ)
	case CHARMOTIONLISTID_WALK:	// 歩き(方向無し)
	case CHARMOTIONLISTID_DESTROY:	// 破壊
	case CHARMOTIONLISTID_SIMPLE:	// 16コマ連続
		bRet = FALSE;
		break;
	}

	return bRet;
}

DWORD CLibInfoMotion::GetWaitTime(DWORD dwMotionTypeID, DWORD dwMotionListID)
{
	int i, nCount;
	DWORD dwRet;
	ARRAYMOTIONINFO apMotionInfo;

	dwRet = 0;
	GetMotionInfo(dwMotionTypeID, dwMotionListID, apMotionInfo);

	nCount = apMotionInfo.size();
	for (i = 0; i < nCount; i ++) {
		dwRet += apMotionInfo[i]->m_byWait;
	}
	dwRet *= 10;

	return dwRet;
}

void CLibInfoMotion::GetMotionInfo(DWORD dwMotionTypeID, DWORD dwMotionListID, ARRAYMOTIONINFO &aDst)
{
	int i, nCount;
	PCInfoMotion pInfo;

	if (dwMotionListID == 0) {
		return;
	}
	aDst.clear();

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMotion)GetPtr(i);
		if (dwMotionTypeID != 0) {
			if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
				continue;
		}
	}
		if (dwMotionListID != 0) {
			if (pInfo->m_dwMotionListID != dwMotionListID) {
				continue;
		}
	}
		aDst.push_back(pInfo);
	}
}

DWORD CLibInfoMotion::GetSendDataSize(DWORD dwMotionTypeID, DWORD dwMotionListID)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMotion pInfo;

	dwRet = 0;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->at(i);
		if (dwMotionTypeID != 0) {
			if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
				continue;
		}
	}
		if (dwMotionListID != 0) {
			if (pInfo->m_dwMotionListID != dwMotionListID) {
				continue;
		}
	}
		dwRet += pInfo->GetSendDataSize();
	}

	dwRet += sizeof(DWORD);	// 終端分

	return dwRet;
}

PBYTE CLibInfoMotion::GetSendData(DWORD dwMotionTypeID, DWORD dwMotionListID)
{
	PBYTE pRet, pDataTmp, pPos;
	DWORD dwSize, dwSizeTmp;
	int i, nCount;
	PCInfoMotion pInfo;

	pRet = NULL;

	dwSize = GetSendDataSize(dwMotionTypeID, dwMotionListID);
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
		if (dwMotionListID != 0) {
			if (pInfo->m_dwMotionListID != dwMotionListID) {
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

PBYTE CLibInfoMotion::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;
	DWORD dwMotionID;
	PCInfoMotion pInfoMotion;

	pRet = pSrc;

	pDataTmp = pSrc;
	while (1) {
		CopyMemory(&dwMotionID, pDataTmp, sizeof(dwMotionID));
		if (dwMotionID == 0) {
			pDataTmp += sizeof(DWORD);
			break;
	}
		pInfoMotion = new CInfoMotion;
		pDataTmp = pInfoMotion->SetSendData(pDataTmp);
		Add(pInfoMotion);
	}
	pRet = pDataTmp;

	return pRet;
}

DWORD CLibInfoMotion::GetSendDataSizeType(DWORD dwMotionTypeID)
{
	return 0;
}

PBYTE CLibInfoMotion::GetSendDataType(DWORD dwMotionTypeID)
{
	return NULL;
}

PBYTE CLibInfoMotion::SetSendDataType(PBYTE pSrc)
{
	return NULL;
}

DWORD CLibInfoMotion::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMotion pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwMotionID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
	}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

