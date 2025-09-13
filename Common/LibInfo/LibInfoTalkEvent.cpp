/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:LibInfoTalkEvent.cpp										 */
/* 内容			:会話イベント情報ライブラリクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/27													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoTalkEvent.h"


/* ========================================================================= */
/* 関数名	:CLibInfoTalkEvent::CLibInfoTalkEvent							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

CLibInfoTalkEvent::CLibInfoTalkEvent()
{
	m_paInfo = NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoTalkEvent::~CLibInfoTalkEvent							 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

CLibInfoTalkEvent::~CLibInfoTalkEvent()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoTalkEvent::Create										 */
/* 内容		:作成															 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

void CLibInfoTalkEvent::Create(void)
{
	m_paInfo = new ARRAYTALKEVENTINFO;
}


/* ========================================================================= */
/* 関数名	:CLibInfoTalkEvent::Destroy										 */
/* 内容		:破棄															 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

void CLibInfoTalkEvent::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoTalkEvent::GetNew										 */
/* 内容		:新規データを取得												 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

PCInfoBase CLibInfoTalkEvent::GetNew(void)
{
	return (PCInfoBase)new CInfoTalkEvent;
}


/* ========================================================================= */
/* 関数名	:CLibInfoTalkEvent::GetCount									 */
/* 内容		:データ数を取得													 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

int CLibInfoTalkEvent::GetCount(void)
{
	int nRet;

	nRet = 0;

	if (m_paInfo == NULL) {
		goto Exit;
	}

	nRet = m_paInfo->GetSize ();
Exit:
	return nRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoTalkEvent::Add											 */
/* 内容		:追加															 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

void CLibInfoTalkEvent::Add(PCInfoBase pInfo)
{
	PCInfoTalkEvent pInfoTalkEvent;

	pInfoTalkEvent = (PCInfoTalkEvent)pInfo;
	if (pInfoTalkEvent->m_dwTalkEventID == 0) {
		pInfoTalkEvent->m_dwTalkEventID = GetNewID ();
	}

	m_paInfo->Add (pInfoTalkEvent);
}


/* ========================================================================= */
/* 関数名	:CLibInfoTalkEvent::Delete										 */
/* 内容		:削除															 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

void CLibInfoTalkEvent::Delete(
	int nNo)		/* [in] 配列番号 */
{
	PCInfoTalkEvent pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoTalkEvent::Delete										 */
/* 内容		:削除															 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

void CLibInfoTalkEvent::Delete(
	DWORD dwTalkEventID)		/* [in] 会話イベントID */
{
	int i, nCount, nNo;
	PCInfoTalkEvent pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwTalkEventID != dwTalkEventID) {
			continue;
		}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete (nNo);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoTalkEvent::DeleteAll									 */
/* 内容		:全て削除														 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

void CLibInfoTalkEvent::DeleteAll(void)
{
	int i, nCount;

	if (m_paInfo == NULL) {
		return;
	}

	nCount = m_paInfo->GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		Delete (i);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoTalkEvent::Merge										 */
/* 内容		:取り込み														 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

void CLibInfoTalkEvent::Merge(CLibInfoTalkEvent *pSrc)
{
	int i, nCount;
	PCInfoTalkEvent pInfoTmp, pInfoSrc;

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoTalkEvent)pSrc->GetPtr (i);
		pInfoTmp = (PCInfoTalkEvent)GetPtr (pInfoSrc->m_dwTalkEventID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoTalkEvent)GetNew ();
			pInfoTmp->Copy (pInfoSrc);
			Add (pInfoTmp);
		}
		pInfoTmp->Copy (pInfoSrc);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoTalkEvent::Renew										 */
/* 内容		:更新															 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

CInfoTalkEvent *CLibInfoTalkEvent::Renew(CInfoTalkEvent *pSrc)
{
	int i, nCount;
	PCInfoTalkEvent pRet, pInfo, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwTalkEventID != pSrc->m_dwTalkEventID) {
			continue;
		}
		pInfo = (PCInfoTalkEvent)GetNew ();
		pInfo->Copy (pSrc);
		pInfo->m_dwTalkEventID = pInfoTmp->m_dwTalkEventID;

		SAFE_DELETE (pInfoTmp);
		m_paInfo->SetAt (i, pInfo);
		pRet = pInfo;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoTalkEvent::GetPtr										 */
/* 内容		:情報を取得														 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

PCInfoBase CLibInfoTalkEvent::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoTalkEvent::GetPtr										 */
/* 内容		:情報を取得														 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

PCInfoBase CLibInfoTalkEvent::GetPtr(
	DWORD dwTalkEventID)		/* [in] 会話イベントID */
{
	int i, nCount;
	PCInfoTalkEvent pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwTalkEventID != dwTalkEventID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoTalkEvent::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

DWORD CLibInfoTalkEvent::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwSize;
	PCInfoTalkEvent pInfoTalkEvent;

	dwSize = 0;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoTalkEvent = (PCInfoTalkEvent)GetPtr (i);
		dwSize += pInfoTalkEvent->GetSendDataSize ();
	}
	/* 終端用 */
	dwSize += sizeof (DWORD);

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CLibInfoTalkEvent::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

PBYTE CLibInfoTalkEvent::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataPos, pDataTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoTalkEvent pInfoTalkEvent;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataPos = pData;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoTalkEvent = (PCInfoTalkEvent)GetPtr (i);

		dwSizeTmp	= pInfoTalkEvent->GetSendDataSize ();
		pDataTmp	= pInfoTalkEvent->GetSendData ();
		CopyMemoryRenew (pDataPos, pDataTmp, dwSizeTmp, pDataPos);
		SAFE_DELETE_ARRAY (pDataTmp);
	}

	return pData;
}


/* ========================================================================= */
/* 関数名	:CLibInfoTalkEvent::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

PBYTE CLibInfoTalkEvent::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp, pDataTmpBack;
	DWORD dwTmp;
	CInfoTalkEvent InfoTmp, *pInfoTalkEvent, *pInfoTalkEventTmp;

	pDataTmp = pSrc;

	while (1) {
		CopyMemory (&dwTmp, pDataTmp, sizeof (DWORD));
		if (dwTmp == 0) {
			pDataTmp += sizeof (DWORD);
			break;
		}
		pDataTmpBack = pDataTmp;
		InfoTmp.SetSendData (pDataTmp);
		pInfoTalkEventTmp = (PCInfoTalkEvent)GetNew ();
		pDataTmp = pInfoTalkEventTmp->SetSendData (pDataTmpBack);

		pInfoTalkEvent = (PCInfoTalkEvent)GetPtr (pInfoTalkEventTmp->m_dwTalkEventID);
		if (pInfoTalkEvent) {
			pInfoTalkEvent->Copy (pInfoTalkEventTmp);
			SAFE_DELETE (pInfoTalkEventTmp);
		} else {
			Add (pInfoTalkEventTmp);
		}
	}

	return pDataTmp;
}


/* ========================================================================= */
/* 関数名	:CLibInfoTalkEvent::GetNewID									 */
/* 内容		:新しいIDを取得													 */
/* 日付		:2008/12/27														 */
/* ========================================================================= */

DWORD CLibInfoTalkEvent::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoTalkEvent pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwTalkEventID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}

	return dwRet;
}

/* Copyright(C)URARA-works 2008 */
