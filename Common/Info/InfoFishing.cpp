/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoFishing.cpp											 */
/* 内容			:釣り情報クラス 実装ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/02													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoFishing.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_dwFishingID",	/* 釣りID */
	"m_nAverage",		/* 釣れる確率 */
	"m_strName",		/* 釣り場名 */
	"nArrayCount",		/* 釣り情報パラメータ数 */
	"a_dwItemTypeID",	/* アイテム種別ID */
	"a_nAverage",		/* 釣りきる確率 */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoFishing::CInfoFishing										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

CInfoFishing::CInfoFishing()
{
	m_dwFishingID	= 0;
	m_nAverage		= 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoFishing::~CInfoFishing									 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

CInfoFishing::~CInfoFishing()
{
}


/* ========================================================================= */
/* 関数名	:CInfoFishing::GetElementNo										 */
/* 内容		:要素番号を取得													 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

int CInfoFishing::GetElementNo(LPCSTR pszName)
{
	int i, nRet;

	nRet = -1;

	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp (s_aszName[i], pszName) == 0) {
			nRet = i;
			break;
		}
	}

	return nRet;
}


/* ========================================================================= */
/* 関数名	:CInfoFishing::GetDataSize										 */
/* 内容		:データサイズを取得												 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

DWORD CInfoFishing::GetDataSize(void)
{
	int nCount;
	DWORD dwRet;

	dwRet = 0;
	nCount = m_apParam.size();

	dwRet += sizeof (m_dwFishingID);		/* 釣りID */
	dwRet += sizeof (m_nAverage);			/* 釣れる確率 */
	dwRet += (m_strName.GetUtf8Length () + 1);	/* 釣り場名 */
	dwRet += sizeof (int);					/* 釣り情報パラメータ数 */
	/* 釣り情報パラメータ */
	dwRet += (nCount * sizeof (DWORD));		/* アイテム種別ID */
	dwRet += (nCount * sizeof (int));		/* 釣りきる確率 */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoFishing::GetDataSizeNo									 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

DWORD CInfoFishing::GetDataSizeNo(int nNo)
{
	int nCount;
	DWORD dwRet;

	dwRet = 0;
	nCount = m_apParam.size();

	switch (nNo) {
	case 0:		dwRet = sizeof (m_dwFishingID);			break;	/* 釣りID */
	case 1:		dwRet = sizeof (m_nAverage);			break;	/* 釣れる確率 */
	case 2:		dwRet = (m_strName.GetUtf8Length () + 1);	break;	/* 釣り場名 */
	case 3:		dwRet = nCount * sizeof (int);			break;	/* 釣り情報パラメータ数 */
	case 4:		dwRet = nCount * sizeof (DWORD);		break;	/* アイテム種別ID */
	case 5:		dwRet = nCount * sizeof (int);			break;	/* 釣りきる確率 */
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoFishing::GetName											 */
/* 内容		:要素名を取得													 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

LPCSTR CInfoFishing::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoFishing::GetWriteData										 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

PBYTE CInfoFishing::GetWriteData(int nNo, PDWORD pdwSize)
{
	int i, nCount, *pnTmp;
	PBYTE pRet, pSrc;
	DWORD dwSize, *pdwTmp;
	PINFOFISHINGPARAM pInfo;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;
	nCount		= m_apParam.size();

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:		pSrc = (PBYTE)&m_dwFishingID;		break;	/* 釣りID */
	case 1:		pSrc = (PBYTE)&m_nAverage;			break;	/* 釣れる確率 */
	case 2:		pSrc = (PBYTE)(LPCSTR)m_strName;	break;	/* 釣り場名 */
	case 3:		pSrc = (PBYTE)&nCount;				break;	/* 釣り情報パラメータ数 */
	case 4:		/* アイテム種別ID */
		pdwTmp = (PDWORD)pRet;
		for (i = 0; i < nCount; i ++) {
			pInfo = GetPtr (i);
			pdwTmp[i] = pInfo->dwItemTypeID;
		}
		break;
	case 5:		/* 釣りきる確率 */
		pnTmp = (int *)pRet;
		for (i = 0; i < nCount; i ++) {
			pInfo = GetPtr (i);
			pnTmp[i] = pInfo->nAverage;
		}
		break;
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoFishing::ReadElementData									 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

DWORD CInfoFishing::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	int i, nCount, *pnTmp;
	PBYTE pDst;
	DWORD dwSize, *pdwTmp;
	PINFOFISHINGPARAM pInfo;

	pDst	= NULL;
	dwSize	= GetDataSizeNo (nNo);
	nCount	= m_apParam.size();

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_dwFishingID;		break;	/* 釣りID */
	case 1:		pDst = (PBYTE)&m_nAverage;			break;	/* 釣れる確率 */
	case 2:		/* 釣り場名 */
		m_strName = (LPCTSTR)AnsiToTString ((LPCSTR)pSrc, 932);
		dwSize = (DWORD)(strlen ((LPCSTR)pSrc) + 1);
		break;
	case 3:		/* 釣り情報パラメータ数 */
		CopyMemory ((PBYTE)&nCount, pSrc, dwSize);
		for (i = 0; i < nCount; i ++) {
			AddParam (0, 0);
		}
		break;
	case 4:		/* アイテム種別ID */
		pdwTmp = (PDWORD)pSrc;
		for (i = 0; i < nCount; i ++) {
			pInfo = GetPtr (i);
			pInfo->dwItemTypeID = pdwTmp[i];
		}
		break;
	case 5:		/* 釣りきる確率 */
		pnTmp = (int *)pSrc;
		for (i = 0; i < nCount; i ++) {
			pInfo = GetPtr (i);
			pInfo->nAverage = pnTmp[i];
		}
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoFishing::GetSendDataSize									 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

DWORD CInfoFishing::GetSendDataSize(void)
{
	return GetDataSize ();
}


/* ========================================================================= */
/* 関数名	:CInfoFishing::GetSendData										 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

PBYTE CInfoFishing::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PINFOFISHINGPARAM pInfo;

	nCount	= m_apParam.size();
	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwFishingID,	sizeof (m_dwFishingID),	pDataTmp);	/* 釣りID */
	CopyMemoryRenew (pDataTmp, &m_nAverage,		sizeof (m_nAverage),	pDataTmp);	/* 釣れる確率 */
	strcpyRenew ((LPSTR)pDataTmp, m_strName, pDataTmp);								/* 釣り場名 */
	CopyMemoryRenew (pDataTmp, &nCount,			sizeof (nCount),		pDataTmp);	/* 釣り情報パラメータ数 */
	for (i = 0; i < nCount; i ++) {
		pInfo = GetPtr (i);
		CopyMemoryRenew (pDataTmp, &pInfo->dwItemTypeID,	sizeof (pInfo->dwItemTypeID),	pDataTmp);	/* アイテム種別ID */
		CopyMemoryRenew (pDataTmp, &pInfo->nAverage,		sizeof (pInfo->nAverage),		pDataTmp);	/* 釣りきる確率 */
	}

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoFishing::SetSendData										 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

PBYTE CInfoFishing::SetSendData(PBYTE pSrc)
{
	int i, nCount, nAverage;
	DWORD dwItemTypeID;
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwFishingID,	pDataTmp, sizeof (m_dwFishingID),	pDataTmp);	/* 釣りID */
	CopyMemoryRenew (&m_nAverage,		pDataTmp, sizeof (m_nAverage),		pDataTmp);	/* 釣れる確率 */
	StoreRenew (m_strName, (LPCSTR)pDataTmp, pDataTmp);									/* 釣り場名 */
	CopyMemoryRenew (&nCount,			pDataTmp, sizeof (nCount),			pDataTmp);	/* 釣り情報パラメータ数 */

	DeleteAllParam ();
	for (i = 0; i < nCount; i ++) {
		CopyMemoryRenew (&dwItemTypeID,	pDataTmp, sizeof (dwItemTypeID),	pDataTmp);	/* アイテム種別ID */
		CopyMemoryRenew (&nAverage,		pDataTmp, sizeof (nAverage),		pDataTmp);	/* 釣りきる確率 */
		AddParam (dwItemTypeID, nAverage);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoFishing::GetCount											 */
/* 内容		:釣り情報パラメータ数を取得										 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

int CInfoFishing::GetCount(void)
{
	return static_cast<int>(m_apParam.size());
}


/* ========================================================================= */
/* 関数名	:CInfoFishing::AddParam											 */
/* 内容		:釣り情報パラメータを追加										 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

void CInfoFishing::AddParam(
	DWORD dwItemTypeID,		/* [in] アイテム種別ID */
	int nAverage)			/* [in] 釣りきる確率 */
{
	PINFOFISHINGPARAM pInfo;

	pInfo = new INFOFISHINGPARAM;
	pInfo->dwItemTypeID	= dwItemTypeID;
	pInfo->nAverage		= nAverage;

	m_apParam.push_back (pInfo);
}


/* ========================================================================= */
/* 関数名	:CInfoFishing::DeleteParam										 */
/* 内容		:釣り情報パラメータを削除										 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

void CInfoFishing::DeleteParam(int nNo)
{
	PINFOFISHINGPARAM pInfo;

	if ((nNo < 0) || (nNo >= static_cast<int>(m_apParam.size()))) {
		return;
	}
	pInfo = m_apParam[nNo];
	SAFE_DELETE (pInfo);
	m_apParam.erase (m_apParam.begin () + nNo);
}


/* ========================================================================= */
/* 関数名	:CInfoFishing::DeleteAllParam									 */
/* 内容		:釣り情報パラメータを全て削除									 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

void CInfoFishing::DeleteAllParam(void)
{
	int i, nCount;

	nCount = GetCount ();
	for (i = nCount - 1; i >= 0; i --) {
		DeleteParam (i);
	}
}


/* ========================================================================= */
/* 関数名	:CInfoFishing::DeleteAllParam									 */
/* 内容		:釣り情報パラメータを全て削除									 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

PINFOFISHINGPARAM CInfoFishing::GetPtr(int nNo)
{
	PINFOFISHINGPARAM pRet;

	pRet = NULL;
	if ((nNo < 0) || (nNo >= static_cast<int>(m_apParam.size()))) {
		goto Exit;
	}

	pRet = m_apParam[nNo];
Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoFishing::Copy												 */
/* 内容		:コピー															 */
/* 日付		:2008/06/02														 */
/* ========================================================================= */

void CInfoFishing::Copy(CInfoFishing *pSrc)
{
	int i, nCount;
	PINFOFISHINGPARAM pInfo;

	m_dwFishingID	= pSrc->m_dwFishingID;
	m_nAverage		= pSrc->m_nAverage;
	m_strName		= pSrc->m_strName;
	nCount			= pSrc->GetCount ();

	DeleteAllParam ();
	for (i = 0; i < nCount; i ++) {
		pInfo = pSrc->GetPtr (i);
		AddParam (pInfo->dwItemTypeID, pInfo->nAverage);
	}
}

/* Copyright(C)URARA-works 2008 */
