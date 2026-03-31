/// @file RouteSearch.cpp
/// @brief 経路探索クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/06/13
/// @copyright Copyright(C)URARA-works 2009

#include "StdAfx.h"
#include "InfoMapBase.h"
#include "RouteSearch.h"

// 定数定義

// 検索状態
enum {
	SEARCHSTATE_READY = 0,		// 待機中
	SEARCHSTATE_SEARCH,			// 検索中
	SEARCHSTATE_SEARCHMOVE,		// 検索移動中
	SEARCHSTATE_FINISH			// 完了
};

CRouteSearch::CRouteSearch()
{
	m_pInfoMap	= NULL;
	m_pMap		= NULL;
	m_nState	= SEARCHSTATE_SEARCH;
	ZeroMemory(&m_sizeMap, sizeof(m_sizeMap));
	ZeroMemory(&m_rcSearch, sizeof(m_rcSearch));
}

CRouteSearch::~CRouteSearch()
{
	SAFE_DELETE_ARRAY(m_pMap);
}

void CRouteSearch::SetMapInfo(CInfoMapBase *pInfoMap)
{
	m_pInfoMap = pInfoMap;
}

void CRouteSearch::SetSearchArea(const RECT &rcSearch)
{
	// Phase 5: px単位の検索範囲をタイル座標へ変換（旧: /2 HALF_TILE→タイル）
	m_rcSearch = rcSearch;
	m_rcSearch.left   /= MAPPARTSSIZE;
	m_rcSearch.right  /= MAPPARTSSIZE;
	m_rcSearch.top    /= MAPPARTSSIZE;
	m_rcSearch.bottom /= MAPPARTSSIZE;

	m_sizeMap = m_pInfoMap->m_sizeMap;
//	m_sizeMap.cx = m_rcSearch.right - m_rcSearch.left + 1;
//	m_sizeMap.cy = m_rcSearch.bottom - m_rcSearch.top + 1;
	SAFE_DELETE_ARRAY(m_pMap);
	m_pMap = new WORD[m_sizeMap.cx * m_sizeMap.cy];
	ZeroMemory(m_pMap, m_sizeMap.cx * m_sizeMap.cy * sizeof(WORD));
}

void CRouteSearch::SetStartPos(int x, int y)
{
	// Phase 5: px単位の開始位置をタイル座標へ変換（旧: /2 HALF_TILE→タイル）
	x /= MAPPARTSSIZE;
	y /= MAPPARTSSIZE;
	m_ptStart.x = x;
	m_ptStart.y = y;
	m_ptNow.x = x;
	m_ptNow.y = y;
}

void CRouteSearch::SetEndPos(int x, int y)
{
	// Phase 5: px単位の終了位置をタイル座標へ変換（旧: /2 HALF_TILE→タイル）
	x /= MAPPARTSSIZE;
	y /= MAPPARTSSIZE;
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

	m_aResult.clear();
	m_nState = SEARCHSTATE_SEARCH;

	bResult = TRUE;
	while (bResult) {
		ProcSEARCH();
		bResult = ProcSEARCHMOVE();
	}

	return &m_aResult;
}

// ４方向検索

void CRouteSearch::ProcSEARCH(void)
{
	BOOL bResult;
	int nDirection, nNo, x, y,
		anDirection[] = {1, 0, 3, 2}, anPosX[] = {0, 0, -1, 1}, anPosY[] = {-1, 1, 0, 0};
	SEARCHINFO SearchInfo;

	// 各方向をチェック
	for (nDirection = 0; nDirection < 4; nDirection ++) {
		x = m_ptNow.x + anPosX[nDirection];
		y = m_ptNow.y + anPosY[nDirection];
		// 範囲外？
		if ((x < m_rcSearch.left) || (x > m_rcSearch.right) || (y < m_rcSearch.top) || (y > m_rcSearch.bottom)) {
			continue;
		}
		// 開始位置？
		if ((x == m_ptStart.x) && (y == m_ptStart.y)) {
			continue;
		}
		// 検索済み？
		if (m_pMap[y * m_sizeMap.cx + x] > 0) {
			continue;
		}
		// ぶつかる？
		bResult = m_pInfoMap->IsMove(x, y, nDirection);
		if (bResult == FALSE) {
			continue;
		}

		// 移動できそうなので検索情報を追加
		SearchInfo.pt.x			= x;
		SearchInfo.pt.y			= y;
		SearchInfo.byDirection	= anDirection[nDirection];
		SearchInfo.byState		= 0;
		SearchInfo.wScore		= abs(m_ptEnd.x - x) + abs(m_ptEnd.y - y) + abs(m_ptStart.x - x) + abs(m_ptStart.y - y);
		m_aInfo.push_back(SearchInfo);
		nNo = static_cast<int>(m_aInfo.size()) - 1;
		m_pMap[y * m_sizeMap.cx + x] = (WORD)(nNo + 1);
	}
}

// 検索処理(検索移動中)

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

	// スコアが最小のルートを検索
	nCount = m_aInfo.size();
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
	// 見つかった？
	if (nNo >= 0) {
		pInfo = &m_aInfo[nNo];
		nProcCount ++;
		pInfo->byState	= 1;
		m_ptNow.x		= pInfo->pt.x;
		m_ptNow.y		= pInfo->pt.y;

		// 最終位置？
		if ((pInfo->pt.x == m_ptEnd.x) && (pInfo->pt.y == m_ptEnd.y)) {
			// 集めたデータを整理
			InfoCleanup(pInfo->pt.x, pInfo->pt.y);
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

// 検索情報最適化

void CRouteSearch::InfoCleanup(
	int x,		// [in] 最終位置(横)
	int y)		// [in] 最終位置(縦)
{
	int i, nNo, nCount,
		anDirection[] = {1, 0, 3, 2}, anPosX[] = {0, 0, -1, 1}, anPosY[] = {-1, 1, 0, 0};
	PSEARCHINFO pInfo;
	ARRAYSEARCHINFO aSearchInfoTmp;

	// 最短ルートのみ残した配列を作成
	nCount = m_aInfo.size();
	for (i = 0; i < nCount; i ++) {
		nNo = m_pMap[m_sizeMap.cx * y + x] - 1;
		pInfo = &m_aInfo[nNo];

		aSearchInfoTmp.push_back(*pInfo);
		x += anPosX[pInfo->byDirection];
		y += anPosY[pInfo->byDirection];
		// 開始位置？
		if ((x == m_ptStart.x) && (y == m_ptStart.y)) {
			break;
		}
	}

	nCount = aSearchInfoTmp.size();
	for (i = nCount - 1; i >= 0; i --) {
		pInfo = &aSearchInfoTmp[i];

		SEARCHRESULT Result;
		Result.pt.x = pInfo->pt.x;
		Result.pt.y = pInfo->pt.y;
		if (i > 0) {
			// 次の位置への向きを設定
			Result.byDirection = anDirection[m_aInfo[i - 1].byDirection];

		} else {
			Result.byDirection = anDirection[pInfo->byDirection];
		}
		m_aResult.push_back(Result);
	}
}
