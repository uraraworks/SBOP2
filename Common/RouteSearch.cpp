/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名：	RouteSearch.cpp												 */
/* 内容：		経路探索クラス 実装ファイル									 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2009/06/13													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "RouteSearch.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* 検索状態 */
enum {
	SEARCHSTATE_READY = 0,			/* 待機中 */
	SEARCHSTATE_PREMOVE,			/* 検索前移動中 */
	SEARCHSTATE_SEARCH,				/* 検索中 */
	SEARCHSTATE_SEARCHMOVE,			/* 検索移動中 */
	SEARCHSTATE_MOVE,				/* 移動中 */
};


/* ========================================================================= */
/* 関数名	:CRouteSearch::CRouteSearch										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2009/06/13														 */
/* ========================================================================= */

CRouteSearch::CRouteSearch()
{
	m_ProcInfo.pSearchMap	= NULL;
	m_ProcInfo.pMap			= NULL;
	InitProcInfo();
}

CRouteSearch::~CRouteSearch()
{
	SAFE_DELETE_ARRAY (m_ProcInfo.pSearchMap);
	SAFE_DELETE_ARRAY (m_ProcInfo.pMap);
}

void CRouteSearch::SetStartPos(int x, int y)
{
	m_ProcInfo.ptStart.x = x;
	m_ProcInfo.ptStart.y = y;
}

void CRouteSearch::SetEndPos(int x, int y)
{
	m_ProcInfo.ptEnd.x = x;
	m_ProcInfo.ptEnd.y = y;
}

void CRouteSearch::SetSize(int cx, int cy)
{
	int nSize;

	m_ProcInfo.nState				/* 検索状態 */
	m_ProcInfo.nDirection			/* 向き */
	m_ProcInfo.nTurn				/* ターン */

	ZeroMemory (&m_ProcInfo.ptNow,		sizeof (m_ProcInfo.ptNow));		/* 現在位置 */
	ZeroMemory (&m_ProcInfo.ptStart,	sizeof (m_ProcInfo.ptStart));	/* 開始位置 */
	ZeroMemory (&m_ProcInfo.ptEnd,		sizeof (m_ProcInfo.ptEnd));		/* 最終位置 */
	ZeroMemory (&m_ProcInfo.sizeMap,	sizeof (m_ProcInfo.sizeMap));	/* 検索用マップサイズ */

	m_ProcInfo.sizeMap.cx = cx;
	m_ProcInfo.sizeMap.cy = cy;
	SAFE_DELETE_ARRAY (m_ProcInfo.pSearchMap);
	SAFE_DELETE_ARRAY (m_ProcInfo.pMap);
	nSize = cx * cy;
	m_ProcInfo.pSearchMap	= new BYTE[nSize];
	m_ProcInfo.pMap			= new BYTE[nSize];
	ZeroMemory (m_ProcInfo.pSearchMap, nSize);
	ZeroMemory (m_ProcInfo.pMap, nSize);
	m_ProcInfo.aInfo.RemoveAll ();
}

void CRouteSearch::SetBlock(int x, int y, BOOL bSet)
{
	if (x >= m_ProcInfo.sizeMap.cx) {
		return;
	}
	if (y >= m_ProcInfo.sizeMap.cy) {
		return;
	}
	m_ProcInfo.pMap[x + y * m_ProcInfo.sizeMap.cx] = (bSet) ? 1 : 0;
}

void CRouteSearch::Search(void)
{
	ChgState (SEARCHSTATE_PREMOVE);
	Proc ();
}

void CRouteSearch::InitProcInfo(void)
{
	SAFE_DELETE_ARRAY (m_ProcInfo.pMap);
	ZeroMemory (&m_ProcInfo, sizeof (m_ProcInfo));
}

void CRouteSearch::ChgState(int nState)
{
	m_ProcInfo.nState = nState;
	switch (m_ProcInfo.nState) {
	case SEARCHSTATE_READY:				/* 待機中 */
	case SEARCHSTATE_PREMOVE:			/* 検索前移動中 */
	case SEARCHSTATE_SEARCH:			/* 検索中 */
	case SEARCHSTATE_SEARCHMOVE:		/* 検索移動中 */
		break;
	}
}

void CRouteSearch::Proc(void)
{
	switch (m_ProcInfo.nState) {
	case SEARCHSTATE_READY:				/* 待機中 */
		break;
	case SEARCHSTATE_PREMOVE:			/* 検索前移動中 */
		ProcPREMOVE ();
		break;
	case SEARCHSTATE_SEARCH:			/* 検索中 */
		ProcSEARCH ();
		break;
	case SEARCHSTATE_SEARCHMOVE:		/* 検索移動中 */
		ProcSEARCHMOVE ();
		break;
	}
}

BOOL CRouteSearch::ProcSEARCH(void)
{
	int i, nNo, x, y, cx, cy,
		anDirection[] = {1, 0, 3, 2}, anPosX[] = {0, 0, -1, 1}, anPosY[] = {-1, 1, 0, 0};
	SEARCHINFO SearchInfo;
	POINT *pptNow, *pptStart, *pptEnd;

	cx			= m_ProcInfo.sizeMap.cx;
	cy			= m_ProcInfo.sizeMap.cy;
	pptNow		= &m_ProcInfo.ptNow;
	pptStart	= &m_ProcInfo.ptStart;
	pptEnd		= &m_ProcInfo.ptEnd;

	/* 各方向をチェック */
	for (i = 0; i < 4; i ++) {
		x = pptNow->x + anPosX[i];
		y = pptNow->y + anPosY[i];
		/* 範囲外？ */
		if (!((x >= 0) && (x < cx) && (y >= 0) && (y < cy))) {
			continue;
		}
		/* 開始位置？ */
		if ((x == pptStart->x) && (y == pptStart->y)) {
			continue;
		}
		/* 検索済み？ */
		if (m_ProcInfo.sizeMap.pSearchMap[y * cy + x] > 0) {
			continue;
		}
		/* ぶつかる？ */
		if (m_ProcInfo.sizeMap.pMap[y * cy + x] > 0) {
			continue;
		}

		/* 移動できそうなので検索情報を追加 */
		SearchInfo.x			= x;
		SearchInfo.y			= y;
		SearchInfo.byDirection	= anDirection[i];
		SearchInfo.byState		= 0;
		SearchInfo.wScore		= abs (pptEnd->x - x) + abs (pptEnd->y - y) + abs (pptStart->x - x) + abs (pptStart->y - y);
		nNo = m_ProcInfo.aInfo.Add (SearchInfo);
		m_ProcInfo.sizeMap.pSearchMap[y * cy + x] = (WORD)(nNo + 1);
	}
	ChgState (SEARCHSTATE_SEARCHMOVE);
	return TRUE;
}

/* ========================================================================= */
/* 関数名：	CWndMap::SearchProcSEARCHMOVE									 */
/* 内容：	検索処理(検索移動中)											 */
/* 日付：	2005/10/09														 */
/* ========================================================================= */

BOOL CRouteSearch::SearchProcSEARCHMOVE(void)
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
	nCount		= m_ProcInfo.aInfo.GetSize ();

	/* スコアが最小のルートを検索 */
	for (i = 0; i < nCount; i ++) {
		pInfo = &m_ProcInfo.aInfo[i];
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
		pInfo = &m_ProcInfo.aInfo[nNo];
		nProcCount ++;
		pInfo->byState		= 1;
		m_ProcInfo.ptNow.x	= pInfo->x;
		m_ProcInfo.ptNow.y	= pInfo->y;

		/* 最終位置？ */
		if ((pInfo->x == m_ProcInfo.ptEnd.x) && (pInfo->y == m_ProcInfo.ptEnd.y)) {
			ChgState (SEARCHSTATE_MOVE);
			/* 集めたデータを整理 */
			SearchInfoCleanup (pInfo->x, pInfo->y);
			goto Exit;
		}
	}
	if (nProcCount == 0) {
		ChgState (SEARCHSTATE_READY);
		goto Exit;
	}
	ChgState (SEARCHSTATE_SEARCH);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名：	CWndMap::SearchInfoCleanup										 */
/* 内容：	検索情報最適化													 */
/* 日付：	2005/10/09														 */
/* ========================================================================= */

void CRouteSearch::SearchInfoCleanup(
	int x,		/* [in] 最終位置(横) */
	int y)		/* [in] 最終位置(縦) */
{
	int i, nNo, nCount, cx, cy,
		anDirection[] = {1, 0, 3, 2}, anPosX[] = {0, 0, -1, 1}, anPosY[] = {-1, 1, 0, 0};
	PSEARCHINFO pInfo;
	ARRAYSEARCHINFO aSearchInfoTmp;

	cx = m_ProcInfo.sizeMap.cx;
	cy = m_ProcInfo.sizeMap.cy;

	/* 最短ルートのみ残した配列を作成 */
	nCount = m_ProcInfo.aInfo.GetSize ();
	for (i = 0; i < nCount; i ++) {
		nNo = m_ProcInfo.aInfo.pSearchMap[cx * y + x] - 1;
		pInfo = &m_ProcInfo.aInfo[nNo];

		aSearchInfoTmp.Add (*pInfo);
		x += anPosX[pInfo->byDirection];
		y += anPosY[pInfo->byDirection];
		/* 開始位置？ */
		if ((x == m_ProcInfo.ptStart.x) && (y == m_ProcInfo.ptStart.y)) {
			m_ProcInfo.nDirection = anDirection[pInfo->byDirection];
			break;
		}
	}

	/* 本データを更新 */
	ZeroMemory (m_ProcInfo.pSearchMap, cx * cy * sizeof (WORD));
	m_ProcInfo.aInfo.RemoveAll ();
	m_ProcInfo.aInfo.Copy (aSearchInfoTmp);

	nCount = m_ProcInfo.aInfo.GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		pInfo = &m_ProcInfo.aInfo[i];
		m_ProcInfo.pSearchMap[cy * pInfo->y + pInfo->x] = (WORD)(i + 1);

		if (i > 0) {
			/* 次の位置への向きを設定 */
			pInfo->byDirection = anDirection[m_ProcInfo.aInfo[i - 1].byDirection];

		} else {
			pInfo->byDirection = anDirection[pInfo->byDirection];
		}
	}
}

#if 0
/* ========================================================================= */
/* 関数名：	CWndMap::SearchProcMOVE											 */
/* 内容：	検索処理(移動中)												 */
/* 日付：	2005/10/09														 */
/* ========================================================================= */

BOOL CRouteSearch::SearchProcMOVE(void)
{
	int nNo,
		anPosX[] = {0, 0, -1, 1}, anPosY[] = {-1, 1, 0, 0};
	PSEARCHINFO pInfo;
	POINT *pptNow, *pptEnd;

	pptNow	= &m_ProcInfo.ptNow;
	pptEnd	= &m_ProcInfo.ptEnd;

	/* 最終位置？ */
	if ((pptNow->x == pptEnd->x) && (pptNow->y == pptEnd->y)) {
		return TRUE;
	}

	nNo = m_ProcInfo.sizeMap.pSearchMap[m_ProcInfo.sizeMap.cx * pptNow->y + pptNow->x] - 1;
	if (nNo >= 0) {
		pInfo = &m_ProcInfo.aInfo[nNo];
		m_ProcInfo.nDirection = pInfo->byDirection;
	}

	/* 次の位置へ移動 */
	pptNow->x		+= anPosX[m_ProcInfo.nDirection];
	pptNow->y		+= anPosY[m_ProcInfo.nDirection];
	m_nDirection	= m_ProcInfo.nDirection;

	return TRUE;
}
#endif
/* Copyright(C)URARA-works 2009 */
