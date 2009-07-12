/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoCharBATTLE1Svr.cpp										 */
/* 内容			:キャラ情報(移動して攻撃)サーバークラス 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/12													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapBase.h"
#include "InfoCharBATTLE1Svr.h"
#include "LibInfoCharSvr.h"


/* ========================================================================= */
/* 関数名	:CInfoCharBATTLE1Svr::CInfoCharBATTLE1Svr						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

CInfoCharBATTLE1Svr::CInfoCharBATTLE1Svr()
{
	m_bDelete				= TRUE;
	m_dwLastTiemAtack		= 0;
	m_dwLastTimeBattleMove	= 0;
	m_pInfoCharTarget		= NULL;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBATTLE1Svr::~CInfoCharBATTLE1Svr						 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

CInfoCharBATTLE1Svr::~CInfoCharBATTLE1Svr()
{
}


/* ========================================================================= */
/* 関数名	:CInfoCharBATTLE1Svr::SetTarget									 */
/* 内容		:ターゲットキャラを設定											 */
/* 日付		:2008/08/09														 */
/* ========================================================================= */

void CInfoCharBATTLE1Svr::SetTarget(CInfoCharBase *pCharTarget)
{
	CInfoCharSvr::SetTarget(pCharTarget);
	m_pInfoCharTarget = (CInfoCharSvr *)pCharTarget;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBATTLE1Svr::SetMoveState								 */
/* 内容		:移動状態を変更													 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CInfoCharBATTLE1Svr::SetMoveState(int nMoveState)
{
	CInfoCharSvr::SetMoveState (nMoveState);

	switch (nMoveState) {
	case CHARMOVESTATE_BATTLE:
	case CHARMOVESTATE_BATTLEATACK:
		m_dwLastTiemAtack = timeGetTime ();
		break;
	default:
		m_dwLastTimeMove = timeGetTime ();
		m_dwLastTiemAtack = 0;
		m_dwTargetCharID = 0;
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CInfoCharBATTLE1Svr::TimerProc									 */
/* 内容		:時間処理														 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

BOOL CInfoCharBATTLE1Svr::TimerProc(DWORD dwTime)
{
	BOOL bRet;
	int nMoveState;

	bRet = FALSE;

	nMoveState = m_nMoveState;
	if (IsAtackTarget () == FALSE) {
		nMoveState = -1;
	}

	switch (nMoveState) {
	case CHARMOVESTATE_STAND:			/* 立ち */
		bRet |= TimerProcSTAND (dwTime);
		break;
	case CHARMOVESTATE_MOVE:			/* 移動中 */
		bRet |= TimerProcMOVE (dwTime);
		break;
	case CHARMOVESTATE_BATTLE:			/* 戦闘中 */
		bRet |= TimerProcBATTLE (dwTime);
		break;
	default:
		bRet |= CInfoCharSvr::TimerProc (dwTime);
		break;
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBATTLE1Svr::ProcAtack									 */
/* 内容		:処理(攻撃した時)												 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CInfoCharBATTLE1Svr::ProcAtack(void)
{
}


/* ========================================================================= */
/* 関数名	:CInfoCharBATTLE1Svr::ProcHit									 */
/* 内容		:処理(攻撃を受けた時)											 */
/* 日付		:2008/07/24														 */
/* 戻り値	:TRUE:以降の処理を続行する										 */
/* ========================================================================= */

BOOL CInfoCharBATTLE1Svr::ProcHit(CInfoCharSvr *pInfoChar)
{
	int anDirection[] = {1, 0, 3, 2, 6, 7, 4, 5};
	BOOL bRet;

	bRet = CInfoCharSvr::ProcHit (pInfoChar);

	if (IsAtackTarget ()) {
		/* 立ち状態で攻撃を受けた場合は戦闘モードに移行する */
		switch (m_nMoveState) {
		case CHARMOVESTATE_STAND:
		case CHARMOVESTATE_MOVE:
			m_nReserveChgMoveState = CHARMOVESTATE_BATTLE;
			break;
		}
		m_nDirection = anDirection[pInfoChar->m_nDirection];
		SetTarget (pInfoChar);
		m_bChgPos = TRUE;
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::ProcSWOON										 */
/* 内容		:行動処理(気絶)													 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

BOOL CInfoCharBATTLE1Svr::ProcSWOON(DWORD dwPara)
{
	if (m_nDropItemAverage != 0) {
		m_bDropItem = TRUE;
	}

//Todo:暫定
	m_nReserveChgEfect = 5;
	m_nReserveChgMoveState = CHARMOVESTATE_DELETE;

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBATTLE1Svr::TimerProcSTAND							 */
/* 内容		:時間処理(立ち)													 */
/* 日付		:2008/07/24														 */
/* ========================================================================= */

BOOL CInfoCharBATTLE1Svr::TimerProcSTAND(DWORD dwTime)
{
	int nDirection;
	DWORD dwTmp;
	BOOL bRet, bResult;
	POINT ptFront;

	bRet = FALSE;

	dwTmp = dwTime - m_dwLastTimeMove;
	/* 1ターンは100msにしておく */
	if (dwTmp < 100) {
		goto Exit;
	}

	m_dwLastTimeMove = dwTime;
	if (genrand () % 100 < 100 - m_nMoveAverage) {
		goto Exit;
	}

	if (m_pInfoMap == NULL) {
		goto Exit;
	}
	bResult = IsEnableMove ();
	if (bResult == FALSE) {
		goto Exit;
	}

	nDirection = genrand () % 4;
	bResult = IsMoveDirection (nDirection);

	/* 進める？ */
	if (bResult) {
		m_nDirection = nDirection;
		GetFrontPos (ptFront, m_nDirection, TRUE);
		SetPos (ptFront.x, ptFront.y);

		m_bChgPos = TRUE;
		SetMoveState (CHARMOVESTATE_MOVE);
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBATTLE1Svr::TimerProcMOVE								 */
/* 内容		:時間処理(移動中)												 */
/* 日付		:2008/07/24														 */
/* ========================================================================= */

BOOL CInfoCharBATTLE1Svr::TimerProcMOVE(DWORD dwTime)
{
	BOOL bRet;
	DWORD dwTmp, dwMoveWait;

	bRet = FALSE;
	if (m_nMoveState != CHARMOVESTATE_MOVE) {
		goto Exit;
	}

	dwMoveWait = GetMoveWait ();
	dwTmp = dwTime - m_dwLastTimeMove;
	/* 1歩分の時間が経過している？ */
	if (dwTmp < dwMoveWait * 16) {
		goto Exit;
	}
	m_dwLastTimeMove = dwTime;
	SetMoveState (CHARMOVESTATE_STAND);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CInfoCharBATTLE1Svr::TimerProcBATTLE							 */
/* 内容		:時間処理(戦闘中)												 */
/* 日付		:2008/07/24														 */
/* ========================================================================= */

BOOL CInfoCharBATTLE1Svr::TimerProcBATTLE(DWORD dwTime)
{
	int nDirection, xx, yy, nMoveAverage;
	BOOL bResult;
	DWORD dwTmp, dwMoveWait;
	POINT ptPos;
	SIZE sizeDistance;

	if (m_dwTargetCharID == 0) {
		SetMoveState (CHARMOVESTATE_STAND);
		goto Exit;
	}

	bResult = IsEnableMove ();
	if (bResult == FALSE) {
		goto Exit;
	}
	m_bRenewTargetPos = TRUE;
	dwTmp = dwTime - m_dwLastTimeMove;
	/* 1ターンは100msにしておく */
	if (dwTmp < 100) {
		goto Exit;
	}
	dwMoveWait = GetMoveWait ();
	dwTmp = dwTime - m_dwLastTimeMove;
	/* 1歩分の時間が経過している？ */
	if (dwTmp < dwMoveWait * 16) {
		goto Exit;
	}

	if (m_bChgMoveCount) {
		goto Exit;
	}

	/* 前回は移動？ */
	if (m_dwLastTiemAtack == 0) {
		dwTmp = timeGetTime () - m_dwLastTimeBattleMove;
		if ((m_dwLastTimeBattleMove != 0) && (dwTmp > 5000)) {
			/* 5秒以上移動していなかったらあきらめる */
			SetMoveState (CHARMOVESTATE_DELETE);
			goto Exit;
		}
		nMoveAverage = m_nMoveAverageBattle;
		if (nMoveAverage == 0) {
			nMoveAverage = m_nMoveAverage;
		}
		if (genrand () % 100 < 100 - nMoveAverage) {
			m_dwLastTimeMove = dwTime;
			goto Exit;
		}
	}

	GetTargetPos (&m_ptTargetPos, ptPos, 1);
	m_pLibInfoCharSvr->GetDistance (sizeDistance, this, m_pInfoCharTarget);
	xx = sizeDistance.cx;
	yy = sizeDistance.cy;
	if (xx + yy == 1) {
		ptPos.x = -1;
	}
	if (m_sizeSearchDistance.cx > 0) {
		if ((m_sizeSearchDistance.cx < xx) || (m_sizeSearchDistance.cy < yy)) {
			/* 索敵範囲外なのであきらめる */
			SetMoveState (CHARMOVESTATE_STAND);
			goto Exit;
		}
	}

	if (xx + yy == 1) {
		nDirection = GetDirection (m_ptTargetPos.x, m_ptTargetPos.y);
		/* 4方向に変換 */
		nDirection = GetDrawDirection (nDirection);
		if (m_nDirection != nDirection) {
			m_nDirection = nDirection;
			m_bChgPosRenew = TRUE;
		}
		dwTmp = timeGetTime () - m_dwLastTiemAtack;
		if (m_dwLastTiemAtack != 0) {
			if (dwTmp > 5000) {
				SetMoveState (CHARMOVESTATE_DELETE);
				goto Exit;
			}
		}
		if (dwTmp >= 1000) {
			m_bAtack = TRUE;
		}
		m_dwLastTimeMove = m_dwLastTimeBattleMove = dwTime;

	} else {
		nDirection = GetDirection (ptPos.x, ptPos.y);
		bResult = IsMoveDirection (nDirection);
		if ((xx <= 1) && (yy <= 1)) {
			/* 4方向に変換 */
			nDirection = GetDrawDirection (nDirection);
			if (bResult == FALSE) {
				SetMoveState (CHARMOVESTATE_STAND);
				goto Exit;
			}
		}

		/* 進める？ */
		if (bResult) {
			m_dwLastTimeBattleMove = dwTime;
			m_bChgMoveCount = TRUE;
			m_nDirection = nDirection;
			m_dwLastTiemAtack = 0;
		}
	}

Exit:
	return TRUE;
}

/* ========================================================================= */
/* 関数名	:CInfoCharBATTLE1Svr::IsMoveDirection							 */
/* 内容		:指定方向に進めるかチェック										 */
/* 日付		:2009/06/14														 */
/* 戻り値	:TRUE:進める													 */
/* ========================================================================= */

BOOL CInfoCharBATTLE1Svr::IsMoveDirection(int nDirection)
{
	int x, y;
	BOOL bResult, bRet;
	RECT rcMap;

	bRet = FALSE;

	bResult = IsEnableMove ();
	if (bResult == FALSE) {
		goto Exit;
	}

	/* 脱出可能かチェック */
	bResult = FALSE;
	GetFrontMapPosRect (rcMap, nDirection);
	if (rcMap.top < 0) {
		/* 範囲外に出る */
		goto Exit;
	}

	for (y = rcMap.top; y <= rcMap.bottom; y ++) {
		for (x = rcMap.left; x <= rcMap.right; x ++) {
			/* 通れない場合がTRUEになるよう反転しておく */
			bResult |= !m_pInfoMap->IsMoveOut (x, y, nDirection);
		}
	}
	if (bResult) {
		goto Exit;
	}

	/* 進めるかチェック */
	bResult = FALSE;
	for (y = rcMap.top; y <= rcMap.bottom; y ++) {
		for (x = rcMap.left; x <= rcMap.right; x ++) {
			/* 通れない場合がTRUEになるよう反転しておく */
			bResult |= !m_pInfoMap->IsMove (x, y, nDirection);
		}
	}
	if (bResult) {
		goto Exit;
	}

	bResult = m_pLibInfoCharSvr->IsBlockChar (this, nDirection, FALSE, TRUE);
	if (bResult) {
		goto Exit;
	}
	bRet = TRUE;
Exit:
	return bRet;
}

/* Copyright(C)URARA-works 2008 */
