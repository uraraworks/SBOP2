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


/* ========================================================================= */
/* 関数名	:CInfoCharBATTLE1Svr::CInfoCharBATTLE1Svr						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

CInfoCharBATTLE1Svr::CInfoCharBATTLE1Svr()
{
	m_dwLastTiemAtack = 0;
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
		m_dwTargetCharID = 0;
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CInfoCharBATTLE1Svr::IsLogoutDelete							 */
/* 内容		:ログアウト時に削除するか判定									 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

BOOL CInfoCharBATTLE1Svr::IsLogoutDelete(void)
{
	return TRUE;
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
	int i, nCount, nDirection;
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
	/* 脱出可能かチェック */
	bResult = FALSE;
	RenewBlockMapArea (m_nMapX, m_nMapY, nDirection, TRUE);

	/* 進めるかチェック */
	nCount = m_aposBockMapArea.GetSize ();
	for (i = 0; i < nCount; i ++) {
		bResult |= !m_pInfoMap->IsMoveOut (m_aposBockMapArea[i].x, m_aposBockMapArea[i].y, nDirection);
	}
	bResult = !bResult;
	if (bResult) {
		bResult = FALSE;
		GetFrontPos (ptFront, nDirection, TRUE);
		RenewBlockMapArea (ptFront.x, ptFront.y, nDirection);
		nCount = m_aposBockMapArea.GetSize ();
		for (i = 0; i < nCount; i ++) {
			bResult |= !m_pInfoMap->IsMove (m_aposBockMapArea[i].x, m_aposBockMapArea[i].y, nDirection);
		}
		bResult = !bResult;
	}

	/* 進める？ */
	if (bResult) {
		m_nDirection = nDirection;
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
		dwTmp = timeGetTime () - m_dwLastTimeMove;
		if ((m_dwLastTimeMove != 0) && (dwTmp > 5000)) {
			/* 5秒以上移動していなかったらあきらめる */
			SetMoveState (CHARMOVESTATE_STAND);
			goto Exit;
		}
		m_dwLastTimeMove = dwTime;
		nMoveAverage = m_nMoveAverageBattle;
		if (nMoveAverage == 0) {
			nMoveAverage = m_nMoveAverage;
		}
		if (genrand () % 100 < 100 - nMoveAverage) {
			goto Exit;
		}
	}

	GetTargetPos (&m_ptTargetPos, ptPos, 1);
	xx = abs (m_ptTargetPos.x - m_nMapX);
	yy = abs (m_ptTargetPos.y - m_nMapY);
	if ((xx <= 2) && (yy <= 2)) {
		nDirection = GetDirection (m_ptTargetPos.x, m_ptTargetPos.y);
		if (nDirection >= 4) {
			if ((xx <= 1) && (yy <= 1)) {
				ptPos.x = -1;
			}
		} else {
			ptPos.x = -1;
		}
	}
	if (ptPos.x < 0) {
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
				SetMoveState (CHARMOVESTATE_STAND);
				goto Exit;
			}
		}
		if (dwTmp >= 1000) {
			m_bAtack = TRUE;
		}
		m_dwLastTimeMove = dwTime;

	} else {
		nDirection = GetDirection (ptPos.x, ptPos.y);
		if ((xx <= 1) && (yy <= 1)) {
			/* 4方向に変換 */
			nDirection = GetDrawDirection (nDirection);
		}

		/* 進める？ */
		m_bChgMoveCount = TRUE;
		m_nDirection = nDirection;
		m_dwLastTiemAtack = 0;
	}

Exit:
	return TRUE;
}

/* Copyright(C)URARA-works 2008 */
