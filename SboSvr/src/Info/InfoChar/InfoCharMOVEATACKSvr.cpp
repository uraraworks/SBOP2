/// @file InfoCharMOVEATACKSvr.cpp
/// @brief キャラ情報(移動して攻撃)サーバークラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/17
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "InfoMapBase.h"
#include "InfoCharMOVEATACKSvr.h"

CInfoCharMOVEATACKSvr::CInfoCharMOVEATACKSvr()
{
	m_bHitQuit	= TRUE;
	m_bDistanceDelete	= FALSE;
	m_bDelete	= TRUE;
	m_dwLastAtackTime	= 0;
	m_dwQuitTime	= 0;
	m_dwHitEffectID	= 0;
	m_dwValue1	= 0;
	m_dwValue2	= 0;
	m_nMoveCount	= 0;
}

CInfoCharMOVEATACKSvr::~CInfoCharMOVEATACKSvr()
{
}

void CInfoCharMOVEATACKSvr::SetMoveState(int nMoveState)
{
	DWORD dwTime;

	if (m_nMoveState == nMoveState) {
		return;
	}
	dwTime = timeGetTime();;
	if (nMoveState == CHARMOVESTATE_BATTLEATACK) {
		m_dwLastAtackTime = dwTime;
	}
	// 戦闘中
	if (nMoveState == CHARMOVESTATE_BATTLE) {
		nMoveState = CHARMOVESTATE_MOVE;
	}
	CInfoCharSvr::SetMoveState(nMoveState);

	m_dwLastTimeMove = dwTime;
}

BOOL CInfoCharMOVEATACKSvr::TimerProc(DWORD dwTime)
{
	int x, y, nState;
	BOOL bRet, bResult, bDelete;
	POINT ptFront;
	DWORD dwTmp;
	RECT rcMap;

	bRet = CInfoCharBase::TimerProc(dwTime);
	bDelete = FALSE;

	bResult = IsEnableAtack();
	if (bResult == FALSE) {
		bDelete = TRUE;
	}
	if (m_dwQuitTime != 0) {
		if (dwTime >= m_dwQuitTime) {
			bDelete = TRUE;
		}
	}
	if (bDelete) {
		SetMoveState(CHARMOVESTATE_DELETE);
		goto Exit;
	}

	dwTmp = dwTime - m_dwLastTimeMove;
	if (dwTmp < m_dwMoveWait * 16) {
		goto Exit;
	}
//Todo:移動処理(とりあえず直進)
	m_dwLastTimeMove = dwTime;
	// 脱出可能かチェック
	bResult = FALSE;
	GetFrontMapPosRect(rcMap, m_nDirection);
	for (y = rcMap.top; y <= rcMap.bottom; y ++) {
		for (x = rcMap.left; x <= rcMap.right; x ++) {
			bResult |= !m_pInfoMap->IsMoveOut(x, y, m_nDirection);
		}
	}
	bResult = !bResult;
	if (bResult) {
		// 進めるかチェック
		bResult = FALSE;
		GetFrontMapPosRect(rcMap, m_nDirection);
		for (y = rcMap.top; y <= rcMap.bottom; y ++) {
			for (x = rcMap.left; x <= rcMap.right; x ++) {
				bResult |= !m_pInfoMap->IsMove(x, y, m_nDirection);
			}
		}
		bResult = !bResult;
		if (rcMap.top < 0) {
			bResult = FALSE;
		}
	}

	m_nMoveCount ++;
	if (m_nMoveCount >= (int)m_dwMoveCount) {
		m_nMoveCount = m_dwMoveCount;
		bResult = FALSE;
	}

	// ぶつかる？
	if (bResult == FALSE) {
		nState = CHARMOVESTATE_DELETE;
		if ((m_bDistanceDelete == FALSE) && (m_dwQuitTime != 0)) {
			if (dwTime - m_dwLastAtackTime < 1000) {
				goto Exit;
			}
			nState = CHARMOVESTATE_BATTLEATACK;
		}
		SetMoveState(nState);

	} else {
		m_bChgPos = TRUE;
		// 2回目以降？
		if (m_nMoveCount > 1) {
			GetFrontPos(ptFront, m_nDirection, TRUE);
			if ((m_nMapX == ptFront.x) && (m_nMapY == ptFront.y)) {
				m_bChgPos = FALSE;
			}
			m_nMapX = ptFront.x;
			m_nMapY = ptFront.y;
		}
		SetMoveState(CHARMOVESTATE_BATTLEATACK);
	}

Exit:
	return bRet;
}

void CInfoCharMOVEATACKSvr::ProcAtack(void)
{
	if (m_bHitQuit) {
		// 攻撃できたので終了
		SetMoveState(CHARMOVESTATE_DELETE);
	}
}

DWORD CInfoCharMOVEATACKSvr::GetHitEffectID(void)
{
	return m_dwHitEffectID;
}

DWORD CInfoCharMOVEATACKSvr::GetDamage(void)
{
	DWORD dwRet;

	if (m_dwValue2 - m_dwValue1 == 0) {
		return 0;
	}
	dwRet = m_dwValue1 + (genrand() % (m_dwValue2 - m_dwValue1));

	return dwRet;
}
