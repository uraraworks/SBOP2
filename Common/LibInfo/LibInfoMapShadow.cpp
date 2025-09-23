/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LibInfoMapShadow.cpp										 */
/* 内容			:マップ影情報ライブラリクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMapShadow.h"


/* ========================================================================= */
/* 関数名	:CLibInfoMapShadow::CLibInfoMapShadow							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/06/05														 */
/* ========================================================================= */

CLibInfoMapShadow::CLibInfoMapShadow()
{
	m_paInfo = NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapShadow::~CLibInfoMapShadow							 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/06/05														 */
/* ========================================================================= */

CLibInfoMapShadow::~CLibInfoMapShadow()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapShadow::Create										 */
/* 内容		:作成															 */
/* 日付		:2007/06/05														 */
/* ========================================================================= */

void CLibInfoMapShadow::Create(void)
{
	m_paInfo = new ARRAYMAPSHADOW;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapShadow::Destroy										 */
/* 内容		:破棄															 */
/* 日付		:2007/06/05														 */
/* ========================================================================= */

void CLibInfoMapShadow::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapShadow::Proc										 */
/* 内容		:処理															 */
/* 戻り値	:TRUE:処理した FALS:処理していない								 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

BOOL CLibInfoMapShadow::Proc(void)
{
	BOOL bRet;
	int i, nCount;
	PCInfoMapShadow pInfo;

	bRet = FALSE;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapShadow)GetPtr (i);
		bRet |= pInfo->TimerProc (timeGetTime ());
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapShadow::GetNew										 */
/* 内容		:新規データを取得												 */
/* 日付		:2007/06/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapShadow::GetNew(void)
{
	return new CInfoMapShadow;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapShadow::GetCount									 */
/* 内容		:データ数を取得													 */
/* 日付		:2007/06/05														 */
/* ========================================================================= */

int CLibInfoMapShadow::GetCount(void)
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


/* ========================================================================= */
/* 関数名	:CLibInfoMapShadow::Add											 */
/* 内容		:追加															 */
/* 日付		:2007/06/05														 */
/* ========================================================================= */

void CLibInfoMapShadow::Add(PCInfoBase pInfo)
{
	PCInfoMapShadow pMapShadowInfo;

	pMapShadowInfo = (PCInfoMapShadow)pInfo;
	if (pMapShadowInfo->m_dwShadowID == 0) {
		pMapShadowInfo->m_dwShadowID = GetNewID ();
	}

	m_paInfo->Add (pMapShadowInfo);
	m_mapIDPtr.insert (pair<DWORD, PVOID>(pMapShadowInfo->m_dwShadowID, (PVOID)pMapShadowInfo));
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapShadow::RenewIDPtr									 */
/* 内容		:ID検索用マップを更新											 */
/* 日付		:2009/03/14														 */
/* ========================================================================= */

void CLibInfoMapShadow::RenewIDPtr(void)
{
	int i, nCount;
	PCInfoMapShadow pInfo;

	m_mapIDPtr.clear();

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapShadow)GetPtr (i);

		m_mapIDPtr.insert (pair<DWORD, PVOID>(pInfo->m_dwShadowID, (PVOID)pInfo));
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapShadow::Delete										 */
/* 内容		:削除															 */
/* 日付		:2007/06/05														 */
/* ========================================================================= */

void CLibInfoMapShadow::Delete(
	int nNo)		/* [in] 配列番号 */
{
	PCInfoMapShadow pInfo;

	pInfo = m_paInfo->at(nNo);
	SAFE_DELETE (pInfo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paInfo->size()))) {
		m_paInfo->erase (m_paInfo->begin () + nNo);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapShadow::Delete										 */
/* 内容		:削除															 */
/* 日付		:2007/06/05														 */
/* ========================================================================= */

void CLibInfoMapShadow::Delete(
	DWORD dwShadowID)		/* [in] マップ影ID */
{
	int i, nCount, nNo;
	PCInfoMapShadow pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwShadowID != dwShadowID) {
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
/* 関数名	:CLibInfoMapShadow::DeleteAll									 */
/* 内容		:全て削除														 */
/* 日付		:2007/06/05														 */
/* ========================================================================= */

void CLibInfoMapShadow::DeleteAll(void)
{
	int i, nCount;

	if (m_paInfo == NULL) {
		return;
	}

	nCount = m_paInfo->size();
	for (i = nCount - 1; i >= 0; i --) {
		Delete (i);
	}
	RenewIDPtr ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapShadow::Merge										 */
/* 内容		:取り込み														 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

void CLibInfoMapShadow::Merge(CLibInfoMapShadow *pSrc)
{
	int i, nCount;
	PCInfoMapShadow pInfoTmp, pInfoSrc;

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoMapShadow)pSrc->GetPtr (i);
		pInfoTmp = (PCInfoMapShadow)GetPtr (pInfoSrc->m_dwShadowID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoMapShadow)GetNew ();
			pInfoTmp->Copy (pInfoSrc);
			Add (pInfoTmp);
	} else {
			pInfoTmp->Copy (pInfoSrc);
	}
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapShadow::ResetAnime									 */
/* 内容		:アニメーション状態を初期化										 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

void CLibInfoMapShadow::ResetAnime(void)
{
	int i, nCount;
	PCInfoMapShadow pInfo;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapShadow)GetPtr (i);
		pInfo->m_byAnimeNo		= 0;
		pInfo->m_dwLastAnime	= timeGetTime ();
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapShadow::GetPtr										 */
/* 内容		:情報を取得														 */
/* 日付		:2007/06/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapShadow::GetPtr(int nNo)
{
	return m_paInfo->at(nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapShadow::GetPtr										 */
/* 内容		:情報を取得														 */
/* 日付		:2007/06/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapShadow::GetPtr(
	DWORD dwShadowID)		/* [in] マップ影ID */
{
	PCInfoBase pRet;

	pRet = NULL;
	if (dwShadowID == 0) {
		return pRet;
	}

	MapIDPtr::iterator ite;
	ite = m_mapIDPtr.find (dwShadowID);
	if (ite != m_mapIDPtr.end()) {
		pRet = (PCInfoBase)ite->second;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapShadow::GetPtr										 */
/* 内容		:情報を取得														 */
/* 日付		:2007/06/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapShadow::GetPtr(
	POINT *pPos)		/* [in] 表示位置 */
{
	int i, nCount;
	PCInfoMapShadow pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
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
/* 関数名	:CLibInfoMapShadow::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2007/04/29														 */
/* ========================================================================= */

DWORD CLibInfoMapShadow::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwSize;
	PCInfoMapShadow pInfoMapShadow;

	dwSize = 0;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapShadow = (PCInfoMapShadow)GetPtr (i);
		dwSize += pInfoMapShadow->GetSendDataSize ();
	}
	/* 終端用 */
	dwSize += sizeof (DWORD);

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapShadow::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2007/04/29														 */
/* ========================================================================= */

PBYTE CLibInfoMapShadow::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataPos, pDataTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoMapShadow pInfoMapShadow;

	dwSize	= GetSendDataSize ();
	pData	= new BYTE[dwSize];
	ZeroMemory (pData, dwSize);

	pDataPos = pData;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapShadow = (PCInfoMapShadow)GetPtr (i);

		dwSizeTmp	= pInfoMapShadow->GetSendDataSize ();
		pDataTmp	= pInfoMapShadow->GetSendData ();
		CopyMemoryRenew (pDataPos, pDataTmp, dwSizeTmp, pDataPos);
		SAFE_DELETE_ARRAY (pDataTmp);
	}

	return pData;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapShadow::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2007/04/29														 */
/* ========================================================================= */

PBYTE CLibInfoMapShadow::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp;
	DWORD dwTmp;
	PCInfoMapShadow pInfoMapShadow, pInfoMapShadowTmp;

	pDataTmp = pSrc;

	while (1) {
		CopyMemory (&dwTmp, pDataTmp, sizeof (DWORD));
		if (dwTmp == 0) {
			pDataTmp += sizeof (DWORD);
			break;
	}
		pInfoMapShadowTmp = (PCInfoMapShadow)GetNew ();
		pDataTmp = pInfoMapShadowTmp->SetSendData (pDataTmp);

		pInfoMapShadow = (PCInfoMapShadow)GetPtr (pInfoMapShadowTmp->m_dwShadowID);
		if (pInfoMapShadow) {
			pInfoMapShadow->Copy (pInfoMapShadowTmp);
			SAFE_DELETE (pInfoMapShadowTmp);
	} else {
			Add (pInfoMapShadowTmp);
	}
	}
	RenewIDPtr ();

	return pDataTmp;
}


/* ========================================================================= */
/* 関数名	:CLibInfoMapShadow::GetNewID									 */
/* 内容		:新しいIDを取得													 */
/* 日付		:2007/06/05														 */
/* ========================================================================= */

DWORD CLibInfoMapShadow::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMapShadow pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwShadowID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
	}
	}

	return dwRet;
}

/* Copyright(C)URARA-works 2007 */
