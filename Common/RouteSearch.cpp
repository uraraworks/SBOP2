/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:RouteSearch.cpp											 */
/* ���e			:�o�H�T���N���X �����t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/06/13													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoMapBase.h"
#include "RouteSearch.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* ������� */
enum {
	SEARCHSTATE_READY = 0,			/* �ҋ@�� */
	SEARCHSTATE_SEARCH,				/* ������ */
	SEARCHSTATE_SEARCHMOVE,			/* �����ړ��� */
	SEARCHSTATE_FINISH				/* ���� */
};


/* ========================================================================= */
/* �֐���	:CRouteSearch::CRouteSearch										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2009/06/13														 */
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
/* �֐���	:CRouteSearch::ProcSEARCH										 */
/* ���e		:�S��������														 */
/* ���t		:2009/06/21														 */
/* ========================================================================= */

void CRouteSearch::ProcSEARCH(void)
{
	BOOL bResult;
	int nDirection, nNo, x, y,
		anDirection[] = {1, 0, 3, 2}, anPosX[] = {0, 0, -1, 1}, anPosY[] = {-1, 1, 0, 0};
	SEARCHINFO SearchInfo;

	/* �e�������`�F�b�N */
	for (nDirection = 0; nDirection < 4; nDirection ++) {
		x = m_ptNow.x + anPosX[nDirection];
		y = m_ptNow.y + anPosY[nDirection];
		/* �͈͊O�H */
		if ((x < m_rcSearch.left) || (x > m_rcSearch.right) || (y < m_rcSearch.top) || (y > m_rcSearch.bottom)) {
			continue;
		}
		/* �J�n�ʒu�H */
		if ((x == m_ptStart.x) && (y == m_ptStart.y)) {
			continue;
		}
		/* �����ς݁H */
		if (m_pMap[y * m_sizeMap.cx + x] > 0) {
			continue;
		}
		/* �Ԃ���H */
		bResult = m_pInfoMap->IsMove (x, y, nDirection);
		if (bResult == FALSE) {
			continue;
		}

		/* �ړ��ł������Ȃ̂Ō�������ǉ� */
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
/* �֐���	:CRouteSearch::ProcSEARCHMOVE									 */
/* ���e		:��������(�����ړ���)											 */
/* ���t		:2009/06/21														 */
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

	/* �X�R�A���ŏ��̃��[�g������ */
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
	/* ���������H */
	if (nNo >= 0) {
		pInfo = &m_aInfo[nNo];
		nProcCount ++;
		pInfo->byState	= 1;
		m_ptNow.x		= pInfo->pt.x;
		m_ptNow.y		= pInfo->pt.y;

		/* �ŏI�ʒu�H */
		if ((pInfo->pt.x == m_ptEnd.x) && (pInfo->pt.y == m_ptEnd.y)) {
			/* �W�߂��f�[�^�𐮗� */
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
/* �֐����F	CWndMap::InfoCleanup											 */
/* ���e�F	�������œK��													 */
/* ���t�F	2005/10/09														 */
/* ========================================================================= */

void CRouteSearch::InfoCleanup(
	int x,		/* [in] �ŏI�ʒu(��) */
	int y)		/* [in] �ŏI�ʒu(�c) */
{
	int i, nNo, nCount,
		anDirection[] = {1, 0, 3, 2}, anPosX[] = {0, 0, -1, 1}, anPosY[] = {-1, 1, 0, 0};
	PSEARCHINFO pInfo;
	ARRAYSEARCHINFO aSearchInfoTmp;

	/* �ŒZ���[�g�̂ݎc�����z����쐬 */
	nCount = m_aInfo.GetSize ();
	for (i = 0; i < nCount; i ++) {
		nNo = m_pMap[m_sizeMap.cx * y + x] - 1;
		pInfo = &m_aInfo[nNo];

		aSearchInfoTmp.Add (*pInfo);
		x += anPosX[pInfo->byDirection];
		y += anPosY[pInfo->byDirection];
		/* �J�n�ʒu�H */
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
			/* ���̈ʒu�ւ̌�����ݒ� */
			Result.byDirection = anDirection[m_aInfo[i - 1].byDirection];

		} else {
			Result.byDirection = anDirection[pInfo->byDirection];
		}
		m_aResult.Add (Result);
	}
}
