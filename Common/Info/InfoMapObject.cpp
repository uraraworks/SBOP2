/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoMapObject.cpp											 */
/* 内容			:マップパーツクラス 実装ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapObject.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_dwObjectID",		/* オブジェクトID */
	"m_dwAttr",			/* オブジェクトの属性 */
	"m_nHideY",			/* 隠れる上からのマス数 */
	"m_sizeGrp",		/* 画像サイズ */
	"m_bHit",			/* 当たり判定 */
	"m_strName",		/* オブジェクト名 */
	"m_pHit",			/* 当たり判定データ */
	"nAnimeCount",		/* オブジェクトアニメ数 */
	"anime_byWait",		/* 待ち時間(×１０ミリ秒) */
	"anime_byLevel",	/* 透明度 */
	"anime_pwGrpID",	/* 画像ID */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoMapObject::CInfoMapObject									 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

CInfoMapObject::CInfoMapObject()
{
	m_dwObjectID	= 0;		/* オブジェクトID */
	m_dwAttr		= 0;		/* オブジェクトの属性 */
	m_nHideY		= 0;		/* 隠れる上からのマス数 */
	m_bHit			= FALSE;	/* 当たり判定 */
	m_pHit			= NULL;		/* 当たり判定データ */
	ZeroMemory (&m_sizeGrp, sizeof (m_sizeGrp));		/* 画像サイズ */

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoMapObject::~CInfoMapObject								 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

CInfoMapObject::~CInfoMapObject()
{
	DeleteAllAnime ();
	SAFE_DELETE_ARRAY (m_pHit);
}


/* ========================================================================= */
/* 関数名	:CInfoMapObject::GetElementCount								 */
/* 内容		:要素数を取得													 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

int CInfoMapObject::GetElementCount(void)
{
	return m_nElementCount;
}


/* ========================================================================= */
/* 関数名	:CInfoMapObject::GetElementNo									 */
/* 内容		:要素番号を取得													 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

int CInfoMapObject::GetElementNo(LPCSTR pszName)
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
/* 関数名	:CInfoMapObject::GetDataSize									 */
/* 内容		:データサイズを取得												 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

DWORD CInfoMapObject::GetDataSize(void)
{
	int i;
	DWORD dwRet;

	dwRet = 0;
	for (i = 0; i < m_nElementCount; i ++) {
		dwRet += GetDataSizeNo (i);
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapObject::GetDataSizeNo									 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

DWORD CInfoMapObject::GetDataSizeNo(int nNo)
{
	int nTmp;
	DWORD dwRet;

	dwRet = 0;

	nTmp = m_aInfoAnime.GetSize ();

	switch (nNo) {
	case 0:		dwRet = sizeof (m_dwObjectID);			break;		/* オブジェクトID */
	case 1:		dwRet = sizeof (m_dwAttr);				break;		/* オブジェクトの属性 */
	case 2:		dwRet = sizeof (m_nHideY);				break;		/* 隠れる上からのマス数 */
	case 3:		dwRet = sizeof (m_sizeGrp);				break;		/* 画像サイズ */
	case 4:		dwRet = sizeof (m_bHit);				break;		/* 当たり判定 */
	case 5:		dwRet = m_strName.GetLength () + 1;		break;		/* オブジェクト名 */
	case 6:		dwRet = m_sizeGrp.cx * m_sizeGrp.cy;	break;		/* 当たり判定データ */
	case 7:		dwRet = sizeof (nTmp);					break;		/* オブジェクトアニメ数 */
	case 8:		dwRet = sizeof (BYTE) * nTmp;			break;		/* 待ち時間(×１０ミリ秒) */
	case 9:		dwRet = sizeof (BYTE) * nTmp;			break;		/* 透明度 */
	case 10:	/* 画像ID */
		dwRet = sizeof (WORD) * m_sizeGrp.cx * m_sizeGrp.cy * nTmp;
		break;
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapObject::GetName										 */
/* 内容		:要素名を取得													 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

LPCSTR CInfoMapObject::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoMapObject::GetWriteData									 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

PBYTE CInfoMapObject::GetWriteData(int nNo, PDWORD pdwSize)
{
	int i, nCount;
	PBYTE pRet, pSrc, pTmp;
	DWORD dwSize;
	PSTMAPOBJECTANIMEINFO pInfo;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;
	nCount		= m_aInfoAnime.GetSize ();

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:		pSrc = (PBYTE)&m_dwObjectID;			break;		/* オブジェクトID */
	case 1:		pSrc = (PBYTE)&m_dwAttr;				break;		/* オブジェクトの属性 */
	case 2:		pSrc = (PBYTE)&m_nHideY;				break;		/* 隠れる上からのマス数 */
	case 3:		pSrc = (PBYTE)&m_sizeGrp;				break;		/* 画像サイズ */
	case 4:		pSrc = (PBYTE)&m_bHit;					break;		/* 当たり判定 */
	case 5:		pSrc = (PBYTE)(LPCSTR)m_strName;		break;		/* オブジェクト名 */
	case 6:		/* 当たり判定データ */
		if (m_pHit) {
			CopyMemory (pRet, m_pHit, dwSize);
		} else {
			ZeroMemory (pRet, dwSize);
		}
		break;
	case 7:		/* オブジェクトアニメ数 */
		pSrc = (PBYTE)&nCount;
		break;
	case 8:		/* 待ち時間(×１０ミリ秒) */
		pTmp = pRet;
		for (i = 0; i < nCount; i ++) {
			pInfo = GetAnimePtr (i);
			CopyMemoryRenew (pTmp, &pInfo->byWait, sizeof (pInfo->byWait), pTmp);
		}
		break;
	case 9:		/* 透明度 */
		pTmp = pRet;
		for (i = 0; i < nCount; i ++) {
			pInfo = GetAnimePtr (i);
			CopyMemoryRenew (pTmp, &pInfo->byLevel, sizeof (pInfo->byLevel), pTmp);
		}
		break;
	case 10:	/* 画像ID */
		pTmp = pRet;
		for (i = 0; i < nCount; i ++) {
			pInfo = GetAnimePtr (i);
			dwSize = sizeof (WORD) * m_sizeGrp.cx * m_sizeGrp.cy;
			CopyMemoryRenew (pTmp, (PBYTE)pInfo->pwGrpID, dwSize, pTmp);
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
/* 関数名	:CInfoMapObject::ReadElementData								 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

DWORD CInfoMapObject::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	int i, nCount;
	PBYTE pDst, pSrcTmp;
	DWORD dwSize, dwSizeTmp;
	PSTMAPOBJECTANIMEINFO pInfo;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_dwObjectID;	dwSize = sizeof (m_dwObjectID);		break;		/* オブジェクトID */
	case 1:		pDst = (PBYTE)&m_dwAttr;		dwSize = sizeof (m_dwAttr);			break;		/* オブジェクトの属性 */
	case 2:		pDst = (PBYTE)&m_nHideY;		dwSize = sizeof (m_nHideY);			break;		/* 隠れる上からのマス数 */
	case 3:		pDst = (PBYTE)&m_sizeGrp;		dwSize = sizeof (m_sizeGrp);		break;		/* 画像サイズ */
	case 4:		pDst = (PBYTE)&m_bHit;			dwSize = sizeof (m_bHit);			break;		/* 当たり判定 */
	case 5:		/* オブジェクト名 */
		m_strName = (LPCSTR)pSrc;
		dwSize = m_strName.GetLength () + 1;
		break;
	case 6:		/* 当たり判定データ */
		SAFE_DELETE_ARRAY (m_pHit);
		dwSize = m_sizeGrp.cx * m_sizeGrp.cy;
		m_pHit = new BYTE[dwSize];
		CopyMemory (m_pHit, pSrc, dwSize);
		break;
	case 7:		/* オブジェクトアニメ数 */
		dwSize = sizeof (int);
		CopyMemory ((PBYTE)&nCount, pSrc, dwSize);
		for (i = 0; i < nCount; i ++) {
			AddAnime ();
		}
		break;
	case 8:		/* 待ち時間(×１０ミリ秒) */
		pSrcTmp = pSrc;
		nCount  = m_aInfoAnime.GetSize ();
		for (i = 0; i < nCount; i ++) {
			pInfo = GetAnimePtr (i);
			CopyMemoryRenew (&pInfo->byWait, pSrcTmp, sizeof (pInfo->byWait), pSrcTmp);
			dwSize += sizeof (pInfo->byWait);
		}
		break;
	case 9:		/* 透明度 */
		pSrcTmp = pSrc;
		nCount  = m_aInfoAnime.GetSize ();
		for (i = 0; i < nCount; i ++) {
			pInfo = GetAnimePtr (i);
			CopyMemoryRenew (&pInfo->byLevel, pSrcTmp, sizeof (pInfo->byLevel), pSrcTmp);
			dwSize += sizeof (pInfo->byLevel);
		}
		break;
	case 10:	/* 画像ID */
		pSrcTmp = pSrc;
		nCount  = m_aInfoAnime.GetSize ();
		for (i = 0; i < nCount; i ++) {
			pInfo = GetAnimePtr (i);
			dwSizeTmp = sizeof (WORD) * m_sizeGrp.cx * m_sizeGrp.cy;
			CopyMemoryRenew ((PBYTE)pInfo->pwGrpID, pSrcTmp, dwSizeTmp, pSrcTmp);
			dwSize += dwSizeTmp;
		}
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoMapObject::Copy											 */
/* 内容		:コピー															 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CInfoMapObject::Copy(CInfoMapObject *pSrc)
{
	PBYTE pTmp;

	pTmp = pSrc->GetSendData ();
	SetSendData (pTmp);
	SAFE_DELETE_ARRAY (pTmp);
}


/* ========================================================================= */
/* 関数名	:CInfoMapObject::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

DWORD CInfoMapObject::GetSendDataSize(void)
{
	/* 保存用データサイズと同じ */
	return GetDataSize ();
}


/* ========================================================================= */
/* 関数名	:CInfoMapObject::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

PBYTE CInfoMapObject::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataTmp;
	DWORD dwSize, dwSizeTmp;
	PSTMAPOBJECTANIMEINFO pInfo;

	dwSize	= GetSendDataSize ();
	pData	= new BYTE[dwSize];
	ZeroMemory (pData, dwSize);

	pDataTmp = pData;
	nCount   = m_aInfoAnime.GetSize ();

	CopyMemoryRenew (pDataTmp, &m_dwObjectID,	sizeof (m_dwObjectID),	pDataTmp);		/* オブジェクトID */
	CopyMemoryRenew (pDataTmp, &m_dwAttr,		sizeof (m_dwAttr),		pDataTmp);		/* オブジェクトの属性 */
	CopyMemoryRenew (pDataTmp, &m_nHideY,		sizeof (m_nHideY),		pDataTmp);		/* 隠れる上からのマス数 */
	CopyMemoryRenew (pDataTmp, &m_sizeGrp,		sizeof (m_sizeGrp),		pDataTmp);		/* 画像サイズ */
	CopyMemoryRenew (pDataTmp, &m_bHit,			sizeof (m_bHit),		pDataTmp);		/* 当たり判定 */
	CopyMemoryRenew (pDataTmp, &nCount,			sizeof (nCount),		pDataTmp);		/* オブジェクトアニメ数 */
	strcpyRenew ((LPSTR)pDataTmp, m_strName, pDataTmp);									/* オブジェクト名 */

	/* 当たり判定データ */
	dwSizeTmp = m_sizeGrp.cx * m_sizeGrp.cy;
	if (m_pHit) {
		CopyMemoryRenew (pDataTmp, m_pHit, dwSizeTmp, pDataTmp);
	} else {
		pDataTmp += dwSizeTmp;
	}

	dwSizeTmp = sizeof (WORD) * m_sizeGrp.cx * m_sizeGrp.cy;
	for (i = 0; i < nCount; i ++) {
		pInfo = GetAnimePtr (i);
		CopyMemoryRenew (pDataTmp, &pInfo->byWait,  sizeof (pInfo->byWait),  pDataTmp);	/* 待ち時間(×１０ミリ秒) */
		CopyMemoryRenew (pDataTmp, &pInfo->byLevel, sizeof (pInfo->byLevel), pDataTmp);	/* 透明度 */
		CopyMemoryRenew (pDataTmp, (PBYTE)pInfo->pwGrpID, dwSizeTmp, pDataTmp);			/* 画像ID */
	}

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoMapObject::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

PBYTE CInfoMapObject::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	PBYTE pDataTmp;
	DWORD dwSizeTmp;
	PSTMAPOBJECTANIMEINFO pInfo;

	DeleteAllAnime ();
	pDataTmp = pSrc;

	CopyMemoryRenew (&m_dwObjectID,	pDataTmp, sizeof (m_dwObjectID),	pDataTmp);		/* オブジェクトID */
	CopyMemoryRenew (&m_dwAttr,		pDataTmp, sizeof (m_dwAttr),		pDataTmp);		/* オブジェクトの属性 */
	CopyMemoryRenew (&m_nHideY,		pDataTmp, sizeof (m_nHideY),		pDataTmp);		/* 隠れる上からのマス数 */
	CopyMemoryRenew (&m_sizeGrp,	pDataTmp, sizeof (m_sizeGrp),		pDataTmp);		/* 画像サイズ */
	CopyMemoryRenew (&m_bHit,		pDataTmp, sizeof (m_bHit),			pDataTmp);		/* 当たり判定 */
	CopyMemoryRenew (&nCount,		pDataTmp, sizeof (nCount),			pDataTmp);		/* オブジェクトアニメ数 */
	StoreRenew (m_strName, (LPCSTR)pDataTmp, pDataTmp);									/* オブジェクト名 */

	/* 当たり判定データ */
	SAFE_DELETE_ARRAY (m_pHit);
	dwSizeTmp = m_sizeGrp.cx * m_sizeGrp.cy;
	m_pHit = new BYTE[dwSizeTmp];
	CopyMemoryRenew (m_pHit, pDataTmp, dwSizeTmp, pDataTmp);

	for (i = 0; i < nCount; i ++) {
		AddAnime ();
	}
	dwSizeTmp = sizeof (WORD) * m_sizeGrp.cx * m_sizeGrp.cy;
	for (i = 0; i < nCount; i ++) {
		pInfo = GetAnimePtr (i);
		CopyMemoryRenew (&pInfo->byWait,		pDataTmp, sizeof (pInfo->byWait),  pDataTmp);	/* 待ち時間(×１０ミリ秒) */
		CopyMemoryRenew (&pInfo->byLevel,		pDataTmp, sizeof (pInfo->byLevel), pDataTmp);	/* 透明度 */
		CopyMemoryRenew ((PBYTE)pInfo->pwGrpID, pDataTmp, dwSizeTmp, pDataTmp);					/* 画像ID */
	}

	return pDataTmp;
}


/* ========================================================================= */
/* 関数名	:CInfoMapObject::GetAnimeCount									 */
/* 内容		:アニメーションコマ数を取得										 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

int CInfoMapObject::GetAnimeCount(void)
{
	return m_aInfoAnime.GetSize ();
}


/* ========================================================================= */
/* 関数名	:CInfoMapObject::AddAnime										 */
/* 内容		:アニメーションコマを追加										 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CInfoMapObject::AddAnime(void)
{
	PSTMAPOBJECTANIMEINFO pInfo;

	pInfo = new STMAPOBJECTANIMEINFO;
	pInfo->byWait	= 0;		/* 待ち時間(×１０ミリ秒) */
	pInfo->byLevel	= 0;		/* 透明度 */
	pInfo->pwGrpID = new WORD[m_sizeGrp.cx * m_sizeGrp.cy];
	ZeroMemory ((PBYTE)pInfo->pwGrpID, sizeof (WORD) * m_sizeGrp.cx * m_sizeGrp.cy);

	m_aInfoAnime.Add (pInfo);
}


/* ========================================================================= */
/* 関数名	:CInfoMapObject::DeleteAnime									 */
/* 内容		:アニメーションコマを削除										 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CInfoMapObject::DeleteAnime(int nNo)
{
	PSTMAPOBJECTANIMEINFO pInfo;

	pInfo = GetAnimePtr (nNo);
	SAFE_DELETE_ARRAY (pInfo->pwGrpID);
	SAFE_DELETE (pInfo);

	m_aInfoAnime.RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CInfoMapObject::DeleteAllAnime									 */
/* 内容		:アニメーションコマを全て削除									 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CInfoMapObject::DeleteAllAnime(void)
{
	int i, nCount;

	nCount = m_aInfoAnime.GetSize ();
	for (i = 0; i < nCount; i ++) {
		DeleteAnime (0);
	}
}


/* ========================================================================= */
/* 関数名	:CInfoMapObject::RenewGrpSize									 */
/* 内容		:画像サイズを更新												 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CInfoMapObject::RenewGrpSize(int cx/*-1*/, int cy/*-1*/)
{
	int i, nCount, x, y, xx, yy;
	PBYTE pTmp;
	PWORD pwTmp;
	PSTMAPOBJECTANIMEINFO pInfo;

	if ((m_sizeGrp.cx == cx) && (m_sizeGrp.cy == cy)) {
		return;
	}
	if (cx == -1) {
		cx = m_sizeGrp.cx;
	}
	if (cy == -1) {
		cy = m_sizeGrp.cy;
	}

	xx = m_sizeGrp.cx;
	yy = m_sizeGrp.cy;
	xx = min (xx, cx);
	yy = min (yy, cy);

	/* 当たり判定データ */
	pTmp = new BYTE[cx * cy];
	ZeroMemory (pTmp, cx * cy);
	if (m_pHit) {
		for (y = 0; y < yy; y ++) {
			for (x = 0; x < xx; x ++) {
				pTmp[cx * y + x] = m_pHit[m_sizeGrp.cx * y + x];
			}
		}
	}
	SAFE_DELETE_ARRAY (m_pHit);
	m_pHit = pTmp;

	/* アニメ毎の設定 */
	nCount = m_aInfoAnime.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = GetAnimePtr (i);

		pwTmp = new WORD[cx * cy];
		ZeroMemory ((PBYTE)pwTmp, sizeof (WORD) * cx * cy);

		for (y = 0; y < yy; y ++) {
			for (x = 0; x < xx; x ++) {
				pwTmp[cx * y + x] = pInfo->pwGrpID[m_sizeGrp.cx * y + x];
			}
		}
		SAFE_DELETE_ARRAY (pInfo->pwGrpID);
		pInfo->pwGrpID = pwTmp;
	}
}


/* ========================================================================= */
/* 関数名	:CInfoMapObject::SetGrpID										 */
/* 内容		:画像IDを設定													 */
/* 日付		:2008/11/02														 */
/* ========================================================================= */

void CInfoMapObject::SetGrpID(int nNo, int x, int y, WORD wGrpID)
{
	PSTMAPOBJECTANIMEINFO pInfo;

	pInfo = GetAnimePtr (nNo);
	if (pInfo == NULL) {
		return;
	}
	if (x >= m_sizeGrp.cx) {
		return;
	}
	if (y >= m_sizeGrp.cy) {
		return;
	}

	pInfo->pwGrpID[y * m_sizeGrp.cx + x] = wGrpID;
}


/* ========================================================================= */
/* 関数名	:CInfoMapObject::GetAnimePtr									 */
/* 内容		:アニメーションコマを取得										 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

PSTMAPOBJECTANIMEINFO CInfoMapObject::GetAnimePtr(int nNo)
{
	int nCount;
	PSTMAPOBJECTANIMEINFO pRet;

	pRet = NULL;

	nCount = m_aInfoAnime.GetSize ();
	if (nNo >= nCount) {
		goto Exit;
	}

	pRet = m_aInfoAnime.GetAt (nNo);
Exit:
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
