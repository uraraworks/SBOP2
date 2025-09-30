/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoEffect.cpp												 */
/* 内容			:エフェクトクラス 実装ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoEffect.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"byAnimeCount",		/* アニメーションコマ数 */
	"dwEffectID",		/* エフェクトID */
	"dwSoundID",		/* 効果音ID */
	"bLoop",			/* ループ判定 */
	"m_bLoopSound",		/* ループ時に効果音を再生する */
	"m_dwGrpIDMain",	/* 画像メインID */
	"m_strName",		/* エフェクト名 */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoEffect::CInfoEffect										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

CInfoEffect::CInfoEffect()
{
	m_bLoop				= FALSE;
	m_bLoopSound		= FALSE;
	m_byAnimeCount		= 0;
	m_dwEffectID		= 0;
	m_dwSoundID			= 0;
	m_dwGrpIDMain		= GRPIDMAIN_EFFECT32;

	m_bAnimeEnd			= FALSE;
	m_byAnimeNo			= 0;
	m_dwLastAnime		= 0;

	m_paAnimeInfo = new ARRAYANIMEINFO;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoEffect::~CInfoEffect										 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

CInfoEffect::~CInfoEffect()
{
	if (m_paAnimeInfo) {
		DeleteAllAnime ();
	}

	SAFE_DELETE (m_paAnimeInfo);
}


/* ========================================================================= */
/* 関数名	:CInfoEffect::GetElementCount									 */
/* 内容		:要素数を取得													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

int CInfoEffect::GetElementCount(void)
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
/* 関数名	:CInfoEffect::GetElementNo										 */
/* 内容		:要素番号を取得													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

int CInfoEffect::GetElementNo(LPCSTR pszName)
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
/* 関数名	:CInfoEffect::GetDataSize										 */
/* 内容		:データサイズを取得												 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

DWORD CInfoEffect::GetDataSize(void)
{
	int i, nCount;
	DWORD dwRet;

	dwRet = sizeof (m_bLoop)		+
			sizeof (m_bLoopSound)	+
			sizeof (m_byAnimeCount)	+
			sizeof (m_dwEffectID)	+
			sizeof (m_dwSoundID)	+
			sizeof (m_dwGrpIDMain)	+
			(m_strName.GetStoreLength () + 1);

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	nCount = m_paAnimeInfo->size();
	for (i = 0; i < nCount; i ++) {
		PCInfoAnime pAnime;

		pAnime = m_paAnimeInfo->at(i);
		dwRet += pAnime->GetDataSize ();
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoEffect::GetDataSizeNo										 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

DWORD CInfoEffect::GetDataSizeNo(int nNo)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoAnime pAnime;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_byAnimeCount);		break;	/* アニメーションコマ数 */
	case 1:		dwRet = sizeof (m_dwEffectID);			break;	/* エフェクトID */
	case 2:		dwRet = sizeof (m_dwSoundID);			break;	/* 効果音ID */
	case 3:		dwRet = sizeof (m_bLoop);				break;	/* ループ判定 */
	case 4:		dwRet = sizeof (m_bLoopSound);			break;	/* ループ時に効果音を再生する */
	case 5:		dwRet = sizeof (m_dwGrpIDMain);			break;	/* 画像メインID */
	case 6:		dwRet = (m_strName.GetStoreLength () + 1);	break;	/* エフェクト名 */
	default:
		nCount = m_paAnimeInfo->size();
		for (i = 0; i < nCount; i ++) {
			pAnime	= m_paAnimeInfo->at(i);
			dwRet	+= pAnime->GetDataSizeNo (nNo - m_nElementCount);
		}
		break;
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoEffect::GetName											 */
/* 内容		:要素名を取得													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

LPCSTR CInfoEffect::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoEffect::GetWriteData										 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

PBYTE CInfoEffect::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:		pSrc = &m_byAnimeCount;				break;	/* アニメーションコマ数 */
	case 1:		pSrc = (PBYTE)&m_dwEffectID;		break;	/* エフェクトID */
	case 2:		pSrc = (PBYTE)&m_dwSoundID;			break;	/* 効果音ID */
	case 3:		pSrc = (PBYTE)&m_bLoop;				break;	/* ループ判定 */
	case 4:		pSrc = (PBYTE)&m_bLoopSound;		break;	/* ループ時に効果音を再生する */
	case 5:		pSrc = (PBYTE)&m_dwGrpIDMain;		break;	/* 画像メインID */
	case 6:		pSrc = (PBYTE)(LPCSTR)m_strName;	break;	/* エフェクト名 */
	default:
		{
			PBYTE pTmp;

			pTmp = pRet;
			nCount = m_paAnimeInfo->size();
			for (i = 0; i < nCount; i ++) {
				PBYTE pSrcTmp;
				DWORD dwSizeTmp;

				pAnime	= m_paAnimeInfo->at(i);
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
/* 関数名	:CInfoEffect::ReadElementData									 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

DWORD CInfoEffect::ReadElementData(
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
	case 0:
		dwSize = sizeof (BYTE);
		CopyMemory (&m_byAnimeCount, pSrc, dwSize);
		for (i = 0; i < m_byAnimeCount; i ++) {
			pAnime = new CInfoAnime;
			m_paAnimeInfo->Add (pAnime);
		}
		break;
	case 1:	pDst = (PBYTE)&m_dwEffectID;	dwSize = sizeof (m_dwEffectID);		break;	/* エフェクトID */
	case 2:	pDst = (PBYTE)&m_dwSoundID;		dwSize = sizeof (m_dwSoundID);		break;	/* 効果音ID */
	case 3:	pDst = (PBYTE)&m_bLoop;			dwSize = sizeof (m_bLoop);			break;	/* ループ判定 */
	case 4:	pDst = (PBYTE)&m_bLoopSound;	dwSize = sizeof (m_bLoopSound);		break;	/* ループ時に効果音を再生する */
	case 5:	pDst = (PBYTE)&m_dwGrpIDMain;	dwSize = sizeof (m_dwGrpIDMain);	break;	/* 画像メインID */
	case 6: 																		  	/* エフェクト名 */
		m_strName = (LPCTSTR)AnsiToTString ((LPCSTR)pSrc, 932);
		dwSize = (DWORD)(strlen ((LPCSTR)pSrc) + 1);
		break;
	default:
		pSrcTmp	= pSrc;
		for (i = 0; i < m_byAnimeCount; i ++) {
			pAnime		= m_paAnimeInfo->at(i);
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
/* 関数名	:CInfoEffect::Copy												 */
/* 内容		:コピー															 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

void CInfoEffect::Copy(CInfoEffect *pSrc)
{
	PBYTE pTmp;

	pTmp = pSrc->GetSendData ();
	SetSendData (pTmp);
	SAFE_DELETE_ARRAY (pTmp);
}


/* ========================================================================= */
/* 関数名	:CInfoEffect::GetSendDataSize									 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

DWORD CInfoEffect::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoAnime pAnime;

	dwRet = sizeof (m_bLoop)		+
			sizeof (m_bLoopSound)	+
			sizeof (m_dwEffectID)	+
			sizeof (m_dwSoundID)	+
			sizeof (m_dwGrpIDMain)	+
			sizeof (m_byAnimeCount)	+
			(m_strName.GetStoreLength () + 1);

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	nCount = m_paAnimeInfo->size();
	for (i = 0; i < nCount; i ++) {
		pAnime = m_paAnimeInfo->at(i);
		dwRet += pAnime->GetSendDataSize ();
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoEffect::GetSendData										 */
/* 内容		:送信データを取得												 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

PBYTE CInfoEffect::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataTmp, pDataAnimeTmp;
	DWORD dwSize, dwSizeAnime;
	PCInfoAnime pAnime;

	dwSize	= GetSendDataSize ();
	pData	= new BYTE[dwSize];
	ZeroMemory (pData, dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwEffectID,	sizeof (m_dwEffectID),		pDataTmp);	/* エフェクトID */
	CopyMemoryRenew (pDataTmp, &m_dwSoundID,	sizeof (m_dwSoundID),		pDataTmp);	/* 効果音ID */
	CopyMemoryRenew (pDataTmp, &m_dwGrpIDMain,	sizeof (m_dwGrpIDMain),		pDataTmp);	/* 画像メインID */
	CopyMemoryRenew (pDataTmp, &m_byAnimeCount,	sizeof (m_byAnimeCount),	pDataTmp);	/* アニメーションコマ数 */
	CopyMemoryRenew (pDataTmp, &m_bLoop,		sizeof (m_bLoop),			pDataTmp);	/* ループ判定 */
	CopyMemoryRenew (pDataTmp, &m_bLoopSound,	sizeof (m_bLoopSound),		pDataTmp);	/* ループ時に効果音を再生する */
	strcpyRenew ((LPSTR)pDataTmp, m_strName, pDataTmp); 							  	/* エフェクト名 */

	nCount = m_paAnimeInfo->size();
	for (i = 0; i < nCount; i ++) {
		pAnime = m_paAnimeInfo->at(i);
		pDataAnimeTmp	= pAnime->GetSendData ();
		dwSizeAnime		= pAnime->GetSendDataSize ();
		CopyMemoryRenew (pDataTmp, pDataAnimeTmp, dwSizeAnime, pDataTmp);
		SAFE_DELETE_ARRAY (pDataAnimeTmp);
	}

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoEffect::SetSendData										 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

PBYTE CInfoEffect::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	PBYTE pDataTmp;
	PCInfoAnime pAnime;

	DeleteAllAnime ();
	pDataTmp = pSrc;

	CopyMemoryRenew (&m_dwEffectID,		pDataTmp, sizeof (m_dwEffectID),	pDataTmp);	/* エフェクトID */
	CopyMemoryRenew (&m_dwSoundID,		pDataTmp, sizeof (m_dwSoundID),		pDataTmp);	/* 効果音ID */
	CopyMemoryRenew (&m_dwGrpIDMain,	pDataTmp, sizeof (m_dwGrpIDMain),	pDataTmp);	/* 画像メインID */
	CopyMemoryRenew (&m_byAnimeCount,	pDataTmp, sizeof (m_byAnimeCount),	pDataTmp);	/* アニメーションコマ数 */
	CopyMemoryRenew (&m_bLoop,			pDataTmp, sizeof (m_bLoop),			pDataTmp);	/* ループ判定 */
	CopyMemoryRenew (&m_bLoopSound,		pDataTmp, sizeof (m_bLoopSound),	pDataTmp);	/* ループ時に効果音を再生する */
	StoreRenew (m_strName, (LPCSTR)pDataTmp, pDataTmp); 							 	/* エフェクト名 */

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
/* 関数名	:CInfoEffect::TimerProc											 */
/* 内容		:時間処理														 */
/* 戻り値	:TRUE:処理した FALSE:処理していない								 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

BOOL CInfoEffect::TimerProc(DWORD dwTime)
{
	BOOL bRet;
	BYTE byAnimeNoBack;
	DWORD dwTmp;
	PCInfoAnime pAnime;

	bRet = FALSE;
	byAnimeNoBack = m_byAnimeNo;

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	pAnime = m_paAnimeInfo->at(m_byAnimeNo);
	dwTmp  = (DWORD)pAnime->m_byWait * 10;
	if (dwTime - m_dwLastAnime >= dwTmp) {
		if (m_dwLastAnime > 0) {
			m_byAnimeNo ++;
		}
		m_dwLastAnime = dwTime;
	}
	if (m_byAnimeNo >= m_byAnimeCount) {
		if (m_bLoop) {
			m_byAnimeNo = 0;
		} else {
			m_byAnimeNo = m_byAnimeCount - 1;
			m_bAnimeEnd = TRUE;
		}
	}

Exit:
	if (byAnimeNoBack != m_byAnimeNo) {
		bRet = TRUE;
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoEffect::GetAnimeCount										 */
/* 内容		:アニメーションコマ数を取得										 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

int CInfoEffect::GetAnimeCount(void)
{
	return m_paAnimeInfo->size();
}


/* ========================================================================= */
/* 関数名	:CInfoEffect::AddAnime											 */
/* 内容		:アニメーションコマを追加										 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

void CInfoEffect::AddAnime(void)
{
	PCInfoAnime pInfo;

	pInfo = new CInfoAnime;
	m_paAnimeInfo->Add (pInfo);
	m_byAnimeCount = static_cast<BYTE>(m_paAnimeInfo->size());
}


/* ========================================================================= */
/* 関数名	:CInfoEffect::DeleteAnime										 */
/* 内容		:アニメーションコマを削除										 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

void CInfoEffect::DeleteAnime(int nNo)
{
	PCInfoAnime pInfo;

	pInfo = m_paAnimeInfo->at(nNo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paAnimeInfo->size()))) { m_paAnimeInfo->erase (m_paAnimeInfo->begin () + nNo); }
	SAFE_DELETE (pInfo);
	m_byAnimeCount = static_cast<BYTE>(m_paAnimeInfo->size());
}


/* ========================================================================= */
/* 関数名	:CInfoEffect::DeleteAllAnime									 */
/* 内容		:アニメーションコマを全て削除									 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

void CInfoEffect::DeleteAllAnime(void)
{
	int i, nCount;

	nCount = m_paAnimeInfo->size();
	for (i = 0; i < nCount; i ++) {
		DeleteAnime (0);
	}
}


/* ========================================================================= */
/* 関数名	:CInfoEffect::GetAnimePtr										 */
/* 内容		:アニメーションコマを取得										 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

PCInfoAnime CInfoEffect::GetAnimePtr(int nNo)
{
	int nCount;
	PCInfoAnime pRet;

	pRet = NULL;

	nCount = m_paAnimeInfo->size();
	if ((nNo < 0) || (nNo >= nCount)) {
		goto Exit;
	}

	pRet = m_paAnimeInfo->at(nNo);
Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoEffect::GetGrpID											 */
/* 内容		:現在の画像IDを取得												 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

WORD CInfoEffect::GetGrpID(void)
{
	WORD wRet;
	PCInfoAnime pAnime;

	wRet = 0;

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	pAnime = m_paAnimeInfo->at(m_byAnimeNo);
	if (pAnime == NULL) {
		goto Exit;
	}

	wRet = pAnime->m_wGrpIDBase;
Exit:
	return wRet;
}


/* ========================================================================= */
/* 関数名	:CInfoEffect::GetLevel											 */
/* 内容		:現在の透明度を取得												 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

BYTE CInfoEffect::GetLevel(void)
{
	BYTE byRet;
	PCInfoAnime pAnime;

	byRet = 0;

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	pAnime = m_paAnimeInfo->at(m_byAnimeNo);
	if (pAnime == NULL) {
		goto Exit;
	}

	byRet = pAnime->m_byLevel;
Exit:
	return byRet;
}


/* ========================================================================= */
/* 関数名	:CInfoEffect::IsAnimeEnd										 */
/* 内容		:アニメーション終了か判定										 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

BOOL CInfoEffect::IsAnimeEnd(void)
{
	return m_bAnimeEnd;
}

/* Copyright(C)URARA-works 2007 */
