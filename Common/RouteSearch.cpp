/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:RouteSearch.cpp											 */
/* 内容			:経路探索クラス 実装ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/06/13													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoMapBase.h"
#include "RouteSearch.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* 検索状態 */
enum {
	SEARCHSTATE_READY = 0,			/* 待機中 */
	SEARCHSTATE_SEARCH,				/* 検索中 */
	SEARCHSTATE_SEARCHMOVE,			/* 検索移動中 */
	SEARCHSTATE_FINISH				/* 完了 */
};


/* ========================================================================= */
/* 関数名	:CRouteSearch::CRouteSearch										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2009/06/13														 */
/* ========================================================================= */

CRouteSearch::CRouteSearch()
{
	m_pInfoMap	= NULL;
	m_pMap		= NULL;
	m_nState	= SEARCHSTATE_SEARCH;
	ZeroMemory (&m_sizeMap, sizeof (m_sizeMap));
	ZeroMemory (&m_rcSearch, sizeof (m_rcSearch));
}

CRouteSearch::~CRouteSearch()
{
	SAFE_DELETE_ARRAY (m_pMap);
}

void CRouteSearch::SetMapInfo(CInfoMapBase *pInfoMap)
{
	m_pInfoMap = pInfoMap;
}

void CRouteSearch::SetSearchArea(const RECT &rcSearch)
{
	m_rcSearch = rcSearch;
	m_rcSearch.left /= 2;
	m_rcSearch.right /= 2;
	m_rcSearch.top /= 2;
	m_rcSearch.bottom /= 2;

	m_sizeMap = m_pInfoMap->m_sizeMap;
//	m_sizeMap.cx = m_rcSearch.right - m_rcSearch.left + 1;
//	m_sizeMap.cy = m_rcSearch.bottom - m_rcSearch.top + 1;
	SAFE_DELETE_ARRAY (m_pMap);
	m_pMap = new WORD[m_sizeMap.cx * m_sizeMap.cy];
	ZeroMemory (m_pMap, m_sizeMap.cx * m_sizeMap.cy * sizeof (WORD));
}

void CRouteSearch::SetStartPos(int x, int y)
{
	x /= 2;
	y /= 2;
	m_ptStart.x = x;
	m_ptStart.y = y;
	m_ptNow.x = x;
	m_ptNow.y = y;
}

void CRouteSearch::SetEndPos(int x, int y)
{
	x /= 2;
	y /= 2;
	m_ptEnd.x = x;
	m_ptEnd.y = y;
}

PARRAYSEARCHRESULT CRouteSearch::Search(void)
{
	BOOL bResult;

	if (m_pInfoMap == NULL) {
		return NULL;
	}
	if (m_pMap == NULL) {
		return NULL;
	}

	m_aResult.RemoveAll ();
	m_nState = SEARCHSTATE_SEARCH;

	bResult = TRUE;
	while (bResult) {
		ProcSEARCH ();
		bResult = ProcSEARCHMOVE ();
	}

	return &m_aResult;
}

/* ========================================================================= */
/* 関数名	:CRouteSearch::ProcSEARCH										 */
/* 内容		:４方向検索														 */
/* 日付		:2009/06/21														 */
/* ========================================================================= */

void CRouteSearch::ProcSEARCH(void)
{
	BOOL bResult;
	int nDirection, nNo, x, y,
		anDirection[] = {1, 0, 3, 2}, anPosX[] = {0, 0, -1, 1}, anPosY[] = {-1, 1, 0, 0};
	SEARCHINFO SearchInfo;

	/* 各方向をチェック */
	for (nDirection = 0; nDirection < 4; nDirection ++) {
		x = m_ptNow.x + anPosX[nDirection];
		y = m_ptNow.y + anPosY[nDirection];
		/* 範囲外？ */
		if ((x < m_rcSearch.left) || (x > m_rcSearch.right) || (y < m_rcSearch.top) || (y > m_rcSearch.bottom)) {
			continue;
		}
		/* 開始位置？ */
		if ((x == m_ptStart.x) && (y == m_ptStart.y)) {
			continue;
		}
		/* 検索済み？ */
		if (m_pMap[y * m_sizeMap.cx + x] > 0) {
			continue;
		}
		/* ぶつかる？ */
		bResult = m_pInfoMap->IsMove (x, y, nDirection);
		if (bResult == FALSE) {
			continue;
		}

		/* 移動できそうなので検索情報を追加 */
		SearchInfo.pt.x			= x;
		SearchInfo.pt.y			= y;
		SearchInfo.byDirection	= anDirection[nDirection];
		SearchInfo.byState		= 0;
		SearchInfo.wScore		= abs (m_ptEnd.x - x) + abs (m_ptEnd.y - y) + abs (m_ptStart.x - x) + abs (m_ptStart.y - y);
		nNo = m_aInfo.Add (SearchInfo);
		m_pMap[y * m_sizeMap.cx + x] = (WORD)(nNo + 1);
	}
}

/* ========================================================================= */
/* 関数名	:CRouteSearch::ProcSEARCHMOVE									 */
/* 内容		:検索処理(検索移動中)											 */
/* 日付		:2009/06/21														 */
/* ========================================================================= */

BOOL CRouteSearch::ProcSEARCHMOVE(void)
{
	BOOL bRet;
	WORD wScore;
	int i, nNo, nCount, nProcCount,
		anDirection[] = {1, 0, 3, 2}, anPosX[] = {0, 0, -1, 1}, anPosY[] = {-1, 1, 0, 0};
	PSEARCHINFO pInfo;

	bRet		= FALSE;
	nNo			= -1;
	wScore		= 0xFFFF;
	nProcCount	= 0;

	/* スコアが最小のルートを検索 */
	nCount = m_aInfo.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = &m_aInfo[i];
		if (pInfo->byState > 0) {
			continue;
		}
		if (pInfo->wScore < wScore) {
			wScore = pInfo->wScore;
			nNo = i;
		}
	}
	/* 見つかった？ */
	if (nNo >= 0) {
		pInfo = &m_aInfo[nNo];
		nProcCount ++;
		pInfo->byState	= 1;
		m_ptNow.x		= pInfo->pt.x;
		m_ptNow.y		= pInfo->pt.y;

		/* 最終位置？ */
		if ((pInfo->pt.x == m_ptEnd.x) && (pInfo->pt.y == m_ptEnd.y)) {
			/* 集めたデータを整理 */
			InfoCleanup (pInfo->pt.x, pInfo->pt.y);
			m_nState = SEARCHSTATE_FINISH;
			goto Exit;
		}
	}
	if (nProcCount == 0) {
		m_nState = SEARCHSTATE_READY;
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名：	CWndMap::InfoCleanup											 */
/* 内容：	検索情報最適化													 */
/* 日付：	2005/10/09														 */
/* ========================================================================= */

void CRouteSearch::InfoCleanup(
	int x,		/* [in] 最終位置(横) */
	int y)		/* [in] 最終位置(縦) */
{
	int i, nNo, nCount,
		anDirection[] = {1, 0, 3, 2}, anPosX[] = {0, 0, -1, 1}, anPosY[] = {-1, 1, 0, 0};
	PSEARCHINFO pInfo;
	ARRAYSEARCHINFO aSearchInfoTmp;

	/* 最短ルートのみ残した配列を作成 */
	nCount = m_aInfo.GetSize ();
	for (i = 0; i < nCount; i ++) {
		nNo = m_pMap[m_sizeMap.cx * y + x] - 1;
		pInfo = &m_aInfo[nNo];

		aSearchInfoTmp.Add (*pInfo);
		x += anPosX[pInfo->byDirection];
		y += anPosY[pInfo->byDirection];
		/* 開始位置？ */
		if ((x == m_ptStart.x) && (y == m_ptStart.y)) {
			break;
		}
	}

	nCount = aSearchInfoTmp.GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		pInfo = &aSearchInfoTmp[i];

		SEARCHRESULT Result;
		Result.pt.x = pInfo->pt.x;
		Result.pt.y = pInfo->pt.y;
		if (i > 0) {
			/* 次の位置への向きを設定 */
			Result.byDirection = anDirection[m_aInfo[i - 1].byDirection];

		} else {
			Result.byDirection = anDirection[pInfo->byDirection];
		}
		m_aResult.Add (Result);
	}
}
