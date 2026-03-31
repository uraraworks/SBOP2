/// @file LibInfoTalkEvent.cpp
/// @brief 会話イベント情報ライブラリクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/27
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "LibInfoTalkEvent.h"

CLibInfoTalkEvent::CLibInfoTalkEvent()
{
	m_paInfo = NULL;
}

CLibInfoTalkEvent::~CLibInfoTalkEvent()
{
	Destroy();
}

void CLibInfoTalkEvent::Create(void)
{
	m_paInfo = new ARRAYTALKEVENTINFO;
}

void CLibInfoTalkEvent::Destroy(void)
{
	DeleteAll();
	SAFE_DELETE(m_paInfo);
}

PCInfoBase CLibInfoTalkEvent::GetNew(void)
{
	return (PCInfoBase)new CInfoTalkEvent;
}

int CLibInfoTalkEvent::GetCount(void)
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

void CLibInfoTalkEvent::Add(PCInfoBase pInfo)
{
	PCInfoTalkEvent pInfoTalkEvent;

	pInfoTalkEvent = (PCInfoTalkEvent)pInfo;
	if (pInfoTalkEvent->m_dwTalkEventID == 0) {
		pInfoTalkEvent->m_dwTalkEventID = GetNewID();
	}

	m_paInfo->Add(pInfoTalkEvent);
}

void CLibInfoTalkEvent::Delete(
	int nNo)	// [in] 配列番号
{
	PCInfoTalkEvent pInfo;

	pInfo = m_paInfo->at(nNo);
	SAFE_DELETE(pInfo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paInfo->size()))) {
		m_paInfo->erase(m_paInfo->begin() + nNo);
	}
}

void CLibInfoTalkEvent::Delete(
	DWORD dwTalkEventID)	// [in] 会話イベントID
{
	int i, nCount, nNo;
	PCInfoTalkEvent pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwTalkEventID != dwTalkEventID) {
			continue;
	}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete(nNo);
	}
}

void CLibInfoTalkEvent::DeleteAll(void)
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

void CLibInfoTalkEvent::Merge(CLibInfoTalkEvent *pSrc)
{
	int i, nCount;
	PCInfoTalkEvent pInfoTmp, pInfoSrc;

	nCount = pSrc->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoTalkEvent)pSrc->GetPtr(i);
		pInfoTmp = (PCInfoTalkEvent)GetPtr(pInfoSrc->m_dwTalkEventID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoTalkEvent)GetNew();
			pInfoTmp->Copy(pInfoSrc);
			Add(pInfoTmp);
	}
		pInfoTmp->Copy(pInfoSrc);
	}
}

CInfoTalkEvent *CLibInfoTalkEvent::Renew(CInfoTalkEvent *pSrc)
{
	int i, nCount;
	PCInfoTalkEvent pRet, pInfo, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwTalkEventID != pSrc->m_dwTalkEventID) {
			continue;
	}
		pInfo = (PCInfoTalkEvent)GetNew();
		pInfo->Copy(pSrc);
		pInfo->m_dwTalkEventID = pInfoTmp->m_dwTalkEventID;

                SAFE_DELETE(pInfoTmp);
                (*m_paInfo)[i] = pInfo;
		pRet = pInfo;
		break;
	}

	return pRet;
}

PCInfoBase CLibInfoTalkEvent::GetPtr(int nNo)
{
	return m_paInfo->at(nNo);
}

PCInfoBase CLibInfoTalkEvent::GetPtr(
	DWORD dwTalkEventID)	// [in] 会話イベントID
{
	int i, nCount;
	PCInfoTalkEvent pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwTalkEventID != dwTalkEventID) {
			continue;
	}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}

DWORD CLibInfoTalkEvent::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwSize;
	PCInfoTalkEvent pInfoTalkEvent;

	dwSize = 0;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoTalkEvent = (PCInfoTalkEvent)GetPtr(i);
		dwSize += pInfoTalkEvent->GetSendDataSize();
	}
	// 終端用
	dwSize += sizeof(DWORD);

	return dwSize;
}

PBYTE CLibInfoTalkEvent::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataPos, pDataTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoTalkEvent pInfoTalkEvent;

	dwSize	= GetSendDataSize();
	pData	= ZeroNew(dwSize);

	pDataPos = pData;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoTalkEvent = (PCInfoTalkEvent)GetPtr(i);

		dwSizeTmp	= pInfoTalkEvent->GetSendDataSize();
		pDataTmp	= pInfoTalkEvent->GetSendData();
		CopyMemoryRenew(pDataPos, pDataTmp, dwSizeTmp, pDataPos);
		SAFE_DELETE_ARRAY(pDataTmp);
	}

	return pData;
}

PBYTE CLibInfoTalkEvent::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp, pDataTmpBack;
	DWORD dwTmp;
	CInfoTalkEvent InfoTmp, *pInfoTalkEvent, *pInfoTalkEventTmp;

	pDataTmp = pSrc;

	while (1) {
		CopyMemory(&dwTmp, pDataTmp, sizeof(DWORD));
		if (dwTmp == 0) {
			pDataTmp += sizeof(DWORD);
			break;
	}
		pDataTmpBack = pDataTmp;
		InfoTmp.SetSendData(pDataTmp);
		pInfoTalkEventTmp = (PCInfoTalkEvent)GetNew();
		pDataTmp = pInfoTalkEventTmp->SetSendData(pDataTmpBack);

		pInfoTalkEvent = (PCInfoTalkEvent)GetPtr(pInfoTalkEventTmp->m_dwTalkEventID);
		if (pInfoTalkEvent) {
			pInfoTalkEvent->Copy(pInfoTalkEventTmp);
			SAFE_DELETE(pInfoTalkEventTmp);
	} else {
			Add(pInfoTalkEventTmp);
	}
	}

	return pDataTmp;
}

DWORD CLibInfoTalkEvent::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoTalkEvent pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwTalkEventID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
	}
	}

	return dwRet;
}

