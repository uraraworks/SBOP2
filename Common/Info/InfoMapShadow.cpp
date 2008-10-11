/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoMapShadow.cpp											 */
/* 内容			:マップ影クラス 実装ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/04													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapShadow.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"byViewType",		/* 表示種別 */
	"byAnimeType",		/* アニメーション種別 */
	"byAnimeCount",		/* アニメーションコマ数 */
	"byLevel",			/* 透明度 */
	"wGrpID",			/* グラフィックID */
	"dwShadowID",		/* 影ID */
	"ptViewPos",		/* 編集画面での表示位置 */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoMapShadow::CInfoMapShadow									 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/06/04														 */
/* ========================================================================= */

CInfoMapShadow::CInfoMapShadow()
{
	m_byViewType		= 0;
	m_byAnimeType		= 0;
	m_byAnimeCount		= 0;
	m_byLevel			= 0;
	m_wGrpID			= 0;
	m_dwShadowID		= 0;
	ZeroMemory (&m_ptViewPos, sizeof (m_ptViewPos));

	m_byAnimeNo			= 0;
	m_dwLastAnime		= 0;

	m_paAnimeInfo = new ARRAYANIMEINFO;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoMapShadow::~CInfoMapShadow								 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/06/04														 */
/* ========================================================================= */

CInfoMapShadow::~CInfoMapShadow()
{
	if (m_paAnimeInfo) {
		DeleteAllAnime ();
	}

	SAFE_DELETE (m_paAnimeInfo);
}


/* ========================================================================= */
/* 関数名	:CInfoMapShadow::GetElementCount								 */
/* 内容		:要素数を取得													 */
/* 日付		:2007/06/04														 */
/* ========================================================================= */

int CInfoMapShadow::GetElementCount(void)
{
	int nRet;
	PCInfoAnime pAnimeTmp;

	pAnimeTmp = new CInfoAnime;

	nRet = m_nElementCount;
	nRet += pAnimeTmp->GetElementCount ();

	SAFE_DELETE (pAnimeTmp);
	return nRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapShadow::GetElementNo									 */
/* 内容		:要素番号を取得													 */
/* 日付		:2007/06/04														 */
/* ========================================================================= */

int CInfoMapShadow::GetElementNo(LPCSTR pszName)
{
	int i, nRet;
	char szTmp[128];
	LPCSTR pszTmp;

	nRet = -1;

	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp (s_aszName[i], pszName) == 0) {
			nRet = i;
			break;
		}
	}

	if (nRet < 0) {
		PCInfoAnime pAnimeTmp;

		pszTmp = strstr (pszName, PREFIX_INFOANIME);
		if (pszTmp == NULL) {
			goto Exit;
		}
		strcpy (szTmp, &pszName[strlen (PREFIX_INFOANIME)]);
		pAnimeTmp	= new CInfoAnime;
		nRet		= pAnimeTmp->GetElementNo (szTmp);
		SAFE_DELETE (pAnimeTmp);

		if (nRet < 0) {
			goto Exit;
		}
		nRet += i;
	}

Exit:
	return nRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapShadow::GetDataSize									 */
/* 内容		:データサイズを取得												 */
/* 日付		:2007/06/04														 */
/* ========================================================================= */

DWORD CInfoMapShadow::GetDataSize(void)
{
	int i, nCount;
	DWORD dwRet;

	dwRet = sizeof (m_byViewType)		+
			sizeof (m_byAnimeType)		+
			sizeof (m_byAnimeCount)		+
			sizeof (m_byLevel)			+
			sizeof (m_wGrpID)			+
			sizeof (m_dwShadowID)		+
			sizeof (m_ptViewPos);

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	nCount = m_paAnimeInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		PCInfoAnime pAnime;

		pAnime = m_paAnimeInfo->GetAt (i);
		dwRet += pAnime->GetDataSize ();
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapShadow::GetDataSizeNo									 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2007/06/04														 */
/* ========================================================================= */

DWORD CInfoMapShadow::GetDataSizeNo(int nNo)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoAnime pAnime;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_byViewType);			break;
	case 1:		dwRet = sizeof (m_byAnimeType);			break;
	case 2:		dwRet = sizeof (m_byAnimeCount);		break;
	case 3:		dwRet = sizeof (m_byLevel);				break;
	case 4:		dwRet = sizeof (m_wGrpID);				break;
	case 5:		dwRet = sizeof (m_dwShadowID);			break;
	case 6:		dwRet = sizeof (m_ptViewPos);			break;
	default:
		nCount = m_paAnimeInfo->GetSize ();
		for (i = 0; i < nCount; i ++) {
			pAnime	= m_paAnimeInfo->GetAt (i);
			dwRet	+= pAnime->GetDataSizeNo (nNo - m_nElementCount);
		}
		break;
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapShadow::GetName										 */
/* 内容		:要素名を取得													 */
/* 日付		:2007/06/04														 */
/* ========================================================================= */

LPCSTR CInfoMapShadow::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoMapShadow::GetWriteData									 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2007/06/04														 */
/* ========================================================================= */

PBYTE CInfoMapShadow::GetWriteData(int nNo, PDWORD pdwSize)
{
	int i, nCount;
	PBYTE pRet, pSrc;
	DWORD dwSize;
	PCInfoAnime pAnime;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:	pSrc = &m_byViewType;			break;
	case 1:	pSrc = &m_byAnimeType;			break;
	case 2:	pSrc = &m_byAnimeCount;			break;
	case 3:	pSrc = &m_byLevel;				break;
	case 4:	pSrc = (PBYTE)&m_wGrpID;		break;
	case 5:	pSrc = (PBYTE)&m_dwShadowID;	break;
	case 6:	pSrc = (PBYTE)&m_ptViewPos;		break;
	default:
		{
			PBYTE pTmp;

			pTmp = pRet;
			nCount = m_paAnimeInfo->GetSize ();
			for (i = 0; i < nCount; i ++) {
				PBYTE pSrcTmp;
				DWORD dwSizeTmp;

				pAnime	= m_paAnimeInfo->GetAt (i);
				pSrcTmp	= pAnime->GetWriteData (nNo - m_nElementCount, &dwSizeTmp);
				CopyMemoryRenew (pTmp, pSrcTmp, dwSizeTmp, pTmp);
				SAFE_DELETE_ARRAY (pSrcTmp);
			}
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
/* 関数名	:CInfoMapShadow::ReadElementData								 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2007/06/04														 */
/* ========================================================================= */

DWORD CInfoMapShadow::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	int i;
	PBYTE pDst, pSrcTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoAnime pAnime;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0: pDst = &m_byViewType;	dwSize = sizeof (m_byViewType);		break;
	case 1:	pDst = &m_byAnimeType;	dwSize = sizeof (m_byAnimeType);	break;
	case 2:
		dwSize = sizeof (BYTE);
		CopyMemory (&m_byAnimeCount, pSrc, dwSize);
		for (i = 0; i < m_byAnimeCount; i ++) {
			pAnime = new CInfoAnime;
			m_paAnimeInfo->Add (pAnime);
		}
		break;
	case 3:	pDst = &m_byLevel;				dwSize = sizeof (m_byLevel);		break;
	case 4:	pDst = (PBYTE)&m_wGrpID;		dwSize = sizeof (m_wGrpID);			break;
	case 5:	pDst = (PBYTE)&m_dwShadowID;	dwSize = sizeof (m_dwShadowID);		break;
	case 6:	pDst = (PBYTE)&m_ptViewPos;		dwSize = sizeof (m_ptViewPos);		break;
	default:
		pSrcTmp	= pSrc;
		for (i = 0; i < m_byAnimeCount; i ++) {
			pAnime		= m_paAnimeInfo->GetAt (i);
			dwSizeTmp	= pAnime->ReadElementData (pSrcTmp, nNo - m_nElementCount);
			dwSize		+= dwSizeTmp;
			pSrcTmp		+= dwSizeTmp;
		}
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoMapShadow::Copy											 */
/* 内容		:コピー															 */
/* 日付		:2007/06/04														 */
/* ========================================================================= */

void CInfoMapShadow::Copy(CInfoMapShadow *pSrc)
{
	PBYTE pTmp;

	pTmp = pSrc->GetSendData ();
	SetSendData (pTmp);
	SAFE_DELETE_ARRAY (pTmp);
}


/* ========================================================================= */
/* 関数名	:CInfoMapShadow::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2007/06/04														 */
/* ========================================================================= */

DWORD CInfoMapShadow::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoAnime pAnime;

	dwRet = sizeof (m_dwShadowID)		+
			sizeof (m_byViewType)		+
			sizeof (m_byAnimeType)		+
			sizeof (m_byAnimeCount)		+
			sizeof (m_byLevel)			+
			sizeof (m_wGrpID)			+
			sizeof (m_ptViewPos);

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	nCount = m_paAnimeInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pAnime = m_paAnimeInfo->GetAt (i);
		dwRet += pAnime->GetSendDataSize ();
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapShadow::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2007/06/04														 */
/* ========================================================================= */

PBYTE CInfoMapShadow::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataTmp, pDataAnimeTmp;
	DWORD dwSize, dwSizeAnime;
	PCInfoAnime pAnime;

	dwSize	= GetSendDataSize ();
	pData	= new BYTE[dwSize];
	ZeroMemory (pData, dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwShadowID,	sizeof (m_dwShadowID),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_byViewType,	sizeof (m_byViewType),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_byAnimeType,	sizeof (m_byAnimeType),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_byAnimeCount,	sizeof (m_byAnimeCount),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_byLevel,		sizeof (m_byLevel),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_wGrpID,		sizeof (m_wGrpID),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_ptViewPos,	sizeof (m_ptViewPos),		pDataTmp);

	nCount = m_paAnimeInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pAnime = m_paAnimeInfo->GetAt (i);
		pDataAnimeTmp	= pAnime->GetSendData ();
		dwSizeAnime		= pAnime->GetSendDataSize ();
		CopyMemoryRenew (pDataTmp, pDataAnimeTmp, dwSizeAnime, pDataTmp);
		SAFE_DELETE_ARRAY (pDataAnimeTmp);
	}

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoMapShadow::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2007/06/04														 */
/* ========================================================================= */

PBYTE CInfoMapShadow::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	PBYTE pDataTmp;
	PCInfoAnime pAnime;

	DeleteAllAnime ();
	pDataTmp = pSrc;

	CopyMemoryRenew (&m_dwShadowID,		pDataTmp, sizeof (m_dwShadowID),	pDataTmp);
	CopyMemoryRenew (&m_byViewType,		pDataTmp, sizeof (m_byViewType),	pDataTmp);
	CopyMemoryRenew (&m_byAnimeType,	pDataTmp, sizeof (m_byAnimeType),	pDataTmp);
	CopyMemoryRenew (&m_byAnimeCount,	pDataTmp, sizeof (m_byAnimeCount),	pDataTmp);
	CopyMemoryRenew (&m_byLevel,		pDataTmp, sizeof (m_byLevel),		pDataTmp);
	CopyMemoryRenew (&m_wGrpID,			pDataTmp, sizeof (m_wGrpID),		pDataTmp);
	CopyMemoryRenew (&m_ptViewPos,		pDataTmp, sizeof (m_ptViewPos),		pDataTmp);

	nCount = m_byAnimeCount;
	for (i = 0; i < nCount; i ++) {
		pAnime = new CInfoAnime;
		pAnime->SetSendData (pDataTmp);
		pDataTmp += pAnime->GetSendDataSize ();
		m_paAnimeInfo->Add (pAnime);
	}

	return pDataTmp;
}


/* ========================================================================= */
/* 関数名	:CInfoMapShadow::TimerProc										 */
/* 内容		:時間処理														 */
/* 戻り値	:TRUE:処理した FALS:処理していない								 */
/* 日付		:2007/06/04														 */
/* ========================================================================= */

BOOL CInfoMapShadow::TimerProc(DWORD dwTime)
{
	BOOL bRet;
	BYTE byAnimeNoBack;
	PCInfoAnime pAnime;

	bRet = FALSE;
	byAnimeNoBack = m_byAnimeNo;

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	pAnime = m_paAnimeInfo->GetAt (m_byAnimeNo);
	if (dwTime - m_dwLastAnime >= (DWORD)(pAnime->m_byWait * 10)) {
		if (m_dwLastAnime > 0) {
			m_byAnimeNo ++;
		}
		m_dwLastAnime = dwTime;
	}
	m_byAnimeNo = (m_byAnimeNo >= m_byAnimeCount) ? 0 : m_byAnimeNo;

Exit:
	if (byAnimeNoBack != m_byAnimeNo) {
		bRet = TRUE;
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapShadow::GetAnimeCount									 */
/* 内容		:アニメーションコマ数を取得										 */
/* 日付		:2007/06/04														 */
/* ========================================================================= */

int CInfoMapShadow::GetAnimeCount(void)
{
	return m_paAnimeInfo->GetSize ();
}


/* ========================================================================= */
/* 関数名	:CInfoMapShadow::AddAnime										 */
/* 内容		:アニメーションコマを追加										 */
/* 日付		:2007/06/04														 */
/* ========================================================================= */

void CInfoMapShadow::AddAnime(void)
{
	PCInfoAnime pInfo;

	pInfo = new CInfoAnime;
	m_paAnimeInfo->Add (pInfo);
	m_byAnimeCount = m_paAnimeInfo->GetSize ();
}


/* ========================================================================= */
/* 関数名	:CInfoMapShadow::DeleteAnime									 */
/* 内容		:アニメーションコマを削除										 */
/* 日付		:2007/06/04														 */
/* ========================================================================= */

void CInfoMapShadow::DeleteAnime(int nNo)
{
	PCInfoAnime pInfo;

	pInfo = m_paAnimeInfo->GetAt (nNo);
	m_paAnimeInfo->RemoveAt (nNo);
	SAFE_DELETE (pInfo);
	m_byAnimeCount = m_paAnimeInfo->GetSize ();
}


/* ========================================================================= */
/* 関数名	:CInfoMapShadow::DeleteAllAnime									 */
/* 内容		:アニメーションコマを全て削除									 */
/* 日付		:2007/06/04														 */
/* ========================================================================= */

void CInfoMapShadow::DeleteAllAnime(void)
{
	int i, nCount;

	nCount = m_paAnimeInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		DeleteAnime (0);
	}
}


/* ========================================================================= */
/* 関数名	:CInfoMapShadow::GetAnimePtr									 */
/* 内容		:アニメーションコマを取得										 */
/* 日付		:2007/06/04														 */
/* ========================================================================= */

PCInfoAnime CInfoMapShadow::GetAnimePtr(int nNo)
{
	int nCount;
	PCInfoAnime pRet;

	pRet = NULL;

	nCount = m_paAnimeInfo->GetSize ();
	if (nNo >= nCount) {
		goto Exit;
	}

	pRet = m_paAnimeInfo->GetAt (nNo);
Exit:
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
