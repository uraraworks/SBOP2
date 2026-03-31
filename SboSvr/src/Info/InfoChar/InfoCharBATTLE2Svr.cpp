/// @file InfoCharBATTLE2Svr.cpp
/// @brief キャラ情報(移動して攻撃)サーバークラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/07/17
/// @copyright Copyright(C)URARA-works 2009

#include "stdafx.h"
#include "InfoMapBase.h"
#include "InfoCharBATTLE2Svr.h"
#include "LibInfoCharSvr.h"

CInfoCharBATTLE2Svr::CInfoCharBATTLE2Svr()
{
}

CInfoCharBATTLE2Svr::~CInfoCharBATTLE2Svr()
{
}

BOOL CInfoCharBATTLE2Svr::TimerProcSTAND(DWORD dwTime)
{
	int nDirection;
	DWORD dwTmp;
	BOOL bRet, bResult;
	POINT ptFront;

	bRet = FALSE;

	dwTmp = dwTime - m_dwLastTimeMove;
	// 1ターンは100msにしておく
	if (dwTmp < 100) {
		goto Exit;
	}

	m_dwLastTimeMove = dwTime;
	if (genrand() % 100 < 100 - m_nMoveAverage) {
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
	bResult = IsEnableMove();
	if (bResult == FALSE) {
		goto Exit;
	}

	nDirection = genrand() % 4;
	bResult = IsMoveDirection(nDirection);

	// 進める？
	if (bResult) {
		m_nDirection = nDirection;
		GetFrontPos(ptFront, m_nDirection, TRUE);
		SetPos(ptFront.x, ptFront.y);

		m_bChgPos = TRUE;
		SetMoveState(CHARMOVESTATE_MOVE);
	}

	bRet = TRUE;
Exit:
	return bRet;
}
