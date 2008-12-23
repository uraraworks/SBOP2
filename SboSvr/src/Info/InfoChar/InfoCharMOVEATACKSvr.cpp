/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoCharMOVEATACKSvr.cpp									 */
/* 内容			:キャラ情報(移動して攻撃)サーバークラス 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/17													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapBase.h"
#include "InfoCharMOVEATACKSvr.h"


/* ========================================================================= */
/* 関数名	:CInfoCharMOVEATACKSvr::CInfoCharMOVEATACKSvr					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

CInfoCharMOVEATACKSvr::CInfoCharMOVEATACKSvr()
{
	m_bDelete		= TRUE;
	m_nMoveCount	= 0;
}


/* ========================================================================= */
/* 関数名	:CInfoCharMOVEATACKSvr::~CInfoCharMOVEATACKSvr					 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

CInfoCharMOVEATACKSvr::~CInfoCharMOVEATACKSvr()
{
}


/* ========================================================================= */
/* 関数名	:CInfoCharMOVEATACKSvr::SetMoveState							 */
/* 内容		:移動状態を変更													 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

void CInfoCharMOVEATACKSvr::SetMoveState(int nMoveState)
{
	if (m_nMoveState == nMoveState) {
		return;
	}
	/* 戦闘中 */
	if (nMoveState == CHARMOVESTATE_BATTLE) {
		nMoveState = CHARMOVESTATE_MOVE;
	}
	CInfoCharSvr::SetMoveState (nMoveState);

	m_dwLastTimeMove = timeGetTime ();
}


/* ========================================================================= */
/* 関数名	:CInfoCharMOVEATACKSvr::TimerProc								 */
/* 内容		:時間処理														 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

BOOL CInfoCharMOVEATACKSvr::TimerProc(DWORD dwTime)
{
	int i, nCount;
	BOOL bRet, bResult;
	POINT ptFront;
	DWORD dwTmp;

	bRet = CInfoCharBase::TimerProc (dwTime);

	bResult = IsEnableAtack ();
	if (bResult == FALSE) {
		SetMoveState (CHARMOVESTATE_DELETE);
		goto Exit;
	}

	dwTmp = dwTime - m_dwLastTimeMove;
	if (dwTmp < m_dwMoveWait * 16) {
		goto Exit;
	}
//Todo:移動処理(とりあえず直進)
	m_dwLastTimeMove = dwTime;
	/* 脱出可能かチェック */
	bResult = FALSE;
	RenewBlockMapArea (m_nMapX, m_nMapY, m_nDirection, TRUE);

	nCount = m_aposBockMapArea.GetSize ();
	for (i = 0; i < nCount; i ++) {
		bResult |= !m_pInfoMap->IsMoveOut (m_aposBockMapArea[i].x, m_aposBockMapArea[i].y, m_nDirection);
	}
	bResult = !bResult;
	if (bResult) {
		/* 進めるかチェック */
		bResult = FALSE;
		GetFrontPos (ptFront, m_nDirection, TRUE);
		RenewBlockMapArea (ptFront.x, ptFront.y, m_nDirection);
		nCount = m_aposBockMapArea.GetSize ();
		for (i = 0; i < nCount; i ++) {
			bResult |= !m_pInfoMap->IsMove (m_aposBockMapArea[i].x, m_aposBockMapArea[i].y, m_nDirection);
		}
		bResult = !bResult;
	}

	m_nMoveCount ++;
	if (m_nMoveCount >= (int)m_dwMoveCount) {
		bResult = FALSE;
	}

	/* ぶつかる？ */
	if (bResult == FALSE) {
		SetMoveState (CHARMOVESTATE_DELETE);

	} else {
		/* 2回目以降？ */
		if (m_nMoveCount > 1) {
			m_nMapX = ptFront.x;
			m_nMapY = ptFront.y;
		}
		m_bChgPos = TRUE;
		SetMoveState (CHARMOVESTATE_BATTLEATACK);
	}

Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharMOVEATACKSvr::ProcAtack								 */
/* 内容		:処理(攻撃した時)												 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

void CInfoCharMOVEATACKSvr::ProcAtack(void)
{
	/* 攻撃できたので終了 */
	SetMoveState (CHARMOVESTATE_DELETE);
}

/* Copyright(C)URARA-works 2007 */
