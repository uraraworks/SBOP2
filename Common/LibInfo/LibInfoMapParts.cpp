/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LibInfoMapParts.cpp										 */
/* 内容			:マップパーツ情報ライブラリクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/28													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMapParts.h"


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::CLibInfoMapParts								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

CLibInfoMapParts::CLibInfoMapParts()
{
	m_dwNewIDTmp	= 0;
	m_paInfo		= NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::~CLibInfoMapParts							 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

CLibInfoMapParts::~CLibInfoMapParts()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::Create										 */
/* 内容		:作成															 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

void CLibInfoMapParts::Create(void)
{
	m_paInfo = new ARRAYMAPPARTS;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::Destroy										 */
/* 内容		:破棄															 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

void CLibInfoMapParts::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::Proc											 */
/* 内容		:処理															 */
/* 戻り値	:TRUE:処理した FALS:処理していない								 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

BOOL CLibInfoMapParts::Proc(void)
{
	BOOL bRet;
	int i, nCount;
	PCInfoMapParts pInfo;

	bRet = FALSE;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapParts)GetPtr (i);
		bRet |= pInfo->TimerProc (timeGetTime ());
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::GetNew										 */
/* 内容		:新規データを取得												 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapParts::GetNew(void)
{
	return new CInfoMapParts;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::GetCount										 */
/* 内容		:データ数を取得													 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

int CLibInfoMapParts::GetCount(void)
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
/* 関数名	:CLibInfoMapParts::Add											 */
/* 内容		:追加															 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

void CLibInfoMapParts::Add(PCInfoBase pInfo)
{
	PCInfoMapParts pMapPartsInfo;

	pMapPartsInfo = (PCInfoMapParts)pInfo;
	if (pMapPartsInfo->m_dwPartsID == 0) {
		pMapPartsInfo->m_dwPartsID = GetNewID ();
	}

	m_paInfo->Add (pMapPartsInfo);
	m_mapIDPtr.insert (pair<DWORD, PVOID>(pMapPartsInfo->m_dwPartsID, (PVOID)pMapPartsInfo));
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::Delete										 */
/* 内容		:削除															 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

void CLibInfoMapParts::Delete(
	int nNo)		/* [in] 配列番号 */
{
	PCInfoMapParts pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::Delete										 */
/* 内容		:削除															 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

void CLibInfoMapParts::Delete(
	DWORD dwPartsID)		/* [in] マップパーツID */
{
	int i, nCount, nNo;
	PCInfoMapParts pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwPartsID != dwPartsID) {
			continue;
		}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete (nNo);
	}
	RenewIDPtr ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::DeleteAll									 */
/* 内容		:全て削除														 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

void CLibInfoMapParts::DeleteAll(void)
{
	int i, nCount;

	if (m_paInfo == NULL) {
		return;
	}

	nCount = m_paInfo->GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		Delete (i);
	}
	m_dwNewIDTmp = 0;
	RenewIDPtr ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::Merge										 */
/* 内容		:取り込み														 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

void CLibInfoMapParts::Merge(CLibInfoMapParts *pSrc)
{
	int i, nCount;
	PCInfoMapParts pInfoTmp, pInfoSrc;

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoMapParts)pSrc->GetPtr (i);
		pInfoTmp = (PCInfoMapParts)GetPtr (pInfoSrc->m_dwPartsID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoMapParts)GetNew ();
			pInfoTmp->Copy (pInfoSrc);
			Add (pInfoTmp);
		} else {
			pInfoTmp->Copy (pInfoSrc);
		}
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::ResetAnime									 */
/* 内容		:アニメーション状態を初期化										 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

void CLibInfoMapParts::ResetAnime(void)
{
	int i, nCount;
	PCInfoMapParts pInfo;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapParts)GetPtr (i);
		pInfo->m_byAnimeNo		= 0;
		pInfo->m_dwLastAnime	= timeGetTime ();
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::GetPtr										 */
/* 内容		:情報を取得														 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapParts::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::GetPtr										 */
/* 内容		:情報を取得														 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapParts::GetPtr(
	DWORD dwPartsID)		/* [in] マップパーツID */
{
	PCInfoBase pRet;

	pRet = NULL;
	if (dwPartsID == 0) {
		return pRet;
	}

	MapIDPtr::iterator ite;
	ite = m_mapIDPtr.find (dwPartsID);
	if (ite != m_mapIDPtr.end()) {
		pRet = (PCInfoBase)ite->second;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::GetPtr										 */
/* 内容		:情報を取得														 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapParts::GetPtr(
	POINT *pPos)		/* [in] 表示位置 */
{
	int i, nCount;
	PCInfoMapParts pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (!((pInfoTmp->m_ptViewPos.x == pPos->x) &&
			(pInfoTmp->m_ptViewPos.y == pPos->y))) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2007/04/29														 */
/* ========================================================================= */

DWORD CLibInfoMapParts::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwSize;
	PCInfoMapParts pInfoMapParts;

	dwSize = 0;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapParts = (PCInfoMapParts)GetPtr (i);
		dwSize += pInfoMapParts->GetSendDataSize ();
	}
	/* 終端用 */
	dwSize += sizeof (DWORD);

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2007/04/29														 */
/* ========================================================================= */

PBYTE CLibInfoMapParts::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataPos, pDataTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoMapParts pInfoMapParts;

	dwSize	= GetSendDataSize ();
	pData	= new BYTE[dwSize];
	ZeroMemory (pData, dwSize);

	pDataPos = pData;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapParts = (PCInfoMapParts)GetPtr (i);

		dwSizeTmp	= pInfoMapParts->GetSendDataSize ();
		pDataTmp	= pInfoMapParts->GetSendData ();
		CopyMemoryRenew (pDataPos, pDataTmp, dwSizeTmp, pDataPos);
		SAFE_DELETE_ARRAY (pDataTmp);
	}

	return pData;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2007/04/29														 */
/* ========================================================================= */

PBYTE CLibInfoMapParts::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp;
	DWORD dwTmp;
	PCInfoMapParts pInfoMapParts, pInfoMapPartsTmp;

	pDataTmp = pSrc;

	while (1) {
		CopyMemory (&dwTmp, pDataTmp, sizeof (DWORD));
		if (dwTmp == 0) {
			pDataTmp += sizeof (DWORD);
			break;
		}
		pInfoMapPartsTmp = (PCInfoMapParts)GetNew ();
		pDataTmp = pInfoMapPartsTmp->SetSendData (pDataTmp);

		pInfoMapParts = (PCInfoMapParts)GetPtr (pInfoMapPartsTmp->m_dwPartsID);
		if (pInfoMapParts) {
			pInfoMapParts->Copy (pInfoMapPartsTmp);
			SAFE_DELETE (pInfoMapPartsTmp);
		} else {
			Add (pInfoMapPartsTmp);
		}
	}
	RenewIDPtr ();

	return pDataTmp;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::GetNewID										 */
/* 内容		:新しいIDを取得													 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

DWORD CLibInfoMapParts::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMapParts pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwPartsID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapParts::RenewIDPtr									 */
/* 内容		:ID検索用マップを更新											 */
/* 日付		:2009/03/14														 */
/* ========================================================================= */

void CLibInfoMapParts::RenewIDPtr(void)
{
	int i, nCount;
	PCInfoMapParts pInfoMapParts;

	m_mapIDPtr.clear();

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapParts = (PCInfoMapParts)GetPtr (i);

		m_mapIDPtr.insert (pair<DWORD, PVOID>(pInfoMapParts->m_dwPartsID, (PVOID)pInfoMapParts));
	}
}

/* Copyright(C)URARA-works 2007 */
