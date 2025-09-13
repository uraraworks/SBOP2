/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:InfoCharBATTLE2Svr.cpp										 */
/* 内容			:キャラ情報(移動して攻撃)サーバークラス 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/07/17													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapBase.h"
#include "InfoCharBATTLE2Svr.h"
#include "LibInfoCharSvr.h"


/* ========================================================================= */
/* 関数名	:CInfoCharBATTLE2Svr::CInfoCharBATTLE2Svr						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2009/07/17														 */
/* ========================================================================= */

CInfoCharBATTLE2Svr::CInfoCharBATTLE2Svr()
{
}


/* ========================================================================= */
/* 関数名	:CInfoCharBATTLE2Svr::~CInfoCharBATTLE2Svr						 */
/* 内容		:デストラクタ													 */
/* 日付		:2009/07/17														 */
/* ========================================================================= */

CInfoCharBATTLE2Svr::~CInfoCharBATTLE2Svr()
{
}


/* ========================================================================= */
/* 関数名	:CInfoCharBATTLE2Svr::TimerProcSTAND							 */
/* 内容		:時間処理(立ち)													 */
/* 日付		:2008/07/24														 */
/* ========================================================================= */

BOOL CInfoCharBATTLE2Svr::TimerProcSTAND(DWORD dwTime)
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
		if (m_dwTargetCharID == 0) {
			m_bChgTargetChar = TRUE;
		} else {
			m_nReserveChgMoveState = CHARMOVESTATE_BATTLE;
			m_bChgPos = TRUE;
		}
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

/* Copyright(C)URARA-works 2009 */
