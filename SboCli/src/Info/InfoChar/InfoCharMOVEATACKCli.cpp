/// @file InfoCharMOVEATACKCli.cpp
/// @brief キャラ情報(移動して攻撃)クライアントクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/17
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "InfoMapBase.h"
#include "InfoCharMOVEATACKCli.h"


CInfoCharMOVEATACKCli::CInfoCharMOVEATACKCli()
{
	m_bSkipMove	= FALSE;
	m_bMotionDirection	= FALSE;

	m_adwMotionID[CHARMOTIONID_STAND]	= CHARMOTIONLISTID_STAND_SIMPLE;	// モーションID(立ち)
	m_adwMotionID[CHARMOTIONID_WALK]	= CHARMOTIONLISTID_STAND_SIMPLE;	// モーションID(歩き)
	m_adwMotionID[CHARMOTIONID_BATTLESTAND]	= CHARMOTIONLISTID_STAND_SIMPLE;	// 戦闘立ち
}


CInfoCharMOVEATACKCli::~CInfoCharMOVEATACKCli()
{
}


void CInfoCharMOVEATACKCli::ChgMoveState(int nMoveState)
{
	BOOL bResult, bInitMove;

	if (nMoveState < 0) {
		return;
	}
	// 戦闘中
	if (nMoveState == CHARMOVESTATE_BATTLE) {
		nMoveState = CHARMOVESTATE_MOVE;
	}

	bResult = IsStateMove();
	if (bResult) {
		switch (nMoveState) {
		case CHARMOVESTATE_STAND:	// 立ち
		case CHARMOVESTATE_BATTLEATACK:	// 戦闘攻撃中
			AddMovePosQue(nMoveState, -1, -1, -1);
			return;
		case CHARMOVESTATE_MOVE:	// 移動中
			CInfoCharBase::SetMoveState(nMoveState);
			return;
		}
	}
	bInitMove = FALSE;
	switch (nMoveState) {
	case CHARMOVESTATE_STAND:	// 立ち
	case CHARMOVESTATE_MOVE:	// 移動中
		m_dwLastTimeMove = 0;
		break;
	case CHARMOVESTATE_DELETEREADY:	// 消去準備
	case CHARMOVESTATE_DELETE:	// 消去
		break;
	case CHARMOVESTATE_BATTLEMOVE:	// 戦闘移動中
	case CHARMOVESTATE_BATTLEATACK:	// 戦闘攻撃中
		if (nMoveState == CHARMOVESTATE_BATTLEATACK) {
			bInitMove = TRUE;
		}
		nMoveState = CHARMOVESTATE_MOVE;
		break;
	default:
		if (m_nMoveState == CHARMOVESTATE_MOVE) {
			return;
		}
		nMoveState = CHARMOVESTATE_MOVE;
		m_ptMove.x = m_ptMove.y = 0;
		m_dwLastTimeMove = 0;
		return;
	}

	CInfoCharCli::ChgMoveState(nMoveState);
	m_nAnime = m_nDirection;
	if (bInitMove) {
		m_ptMove.x = m_ptMove.y = 0;
	}
}


void CInfoCharMOVEATACKCli::SetViewState(int nViewState)
{
	CInfoCharCli::SetViewState(nViewState);

	if (m_nViewState == INFOCHARCLI_VIEWSTATE_FADEOUT) {
		m_nFadeLevel = 50;
		return;
	}
	// フェードイン・フェードアウトは無し
	m_dwLastTimeViewState = 0;
}


CInfoMotion *CInfoCharMOVEATACKCli::GetMotionInfo(int *pnCount)
{
	m_nAnime = m_nDirection;
	return CInfoCharCli::GetMotionInfo(pnCount);
}

