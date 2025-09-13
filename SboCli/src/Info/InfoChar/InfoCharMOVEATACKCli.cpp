/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoCharMOVEATACKCli.cpp									 */
/* 内容			:キャラ情報(移動して攻撃)クライアントクラス 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/17													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapBase.h"
#include "InfoCharMOVEATACKCli.h"


/* ========================================================================= */
/* 関数名	:CInfoCharMOVEATACKCli::CInfoCharMOVEATACKCli					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

CInfoCharMOVEATACKCli::CInfoCharMOVEATACKCli()
{
	m_bSkipMove			= FALSE;
	m_bMotionDirection	= FALSE;

	m_adwMotionID[CHARMOTIONID_STAND]		= CHARMOTIONLISTID_STAND_SIMPLE;	/* モーションID(立ち) */
	m_adwMotionID[CHARMOTIONID_WALK]		= CHARMOTIONLISTID_STAND_SIMPLE;	/* モーションID(歩き) */
	m_adwMotionID[CHARMOTIONID_BATTLESTAND]	= CHARMOTIONLISTID_STAND_SIMPLE;	/* 戦闘立ち */
}


/* ========================================================================= */
/* 関数名	:CInfoCharMOVEATACKCli::~CInfoCharMOVEATACKCli					 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

CInfoCharMOVEATACKCli::~CInfoCharMOVEATACKCli()
{
}


/* ========================================================================= */
/* 関数名	:CInfoCharMOVEATACKCli::ChgMoveState							 */
/* 内容		:移動状態変更													 */
/* 日付		:2007/09/09														 */
/* ========================================================================= */

void CInfoCharMOVEATACKCli::ChgMoveState(int nMoveState)
{
	BOOL bResult, bInitMove;

	if (nMoveState < 0) {
		return;
	}
	/* 戦闘中 */
	if (nMoveState == CHARMOVESTATE_BATTLE) {
		nMoveState = CHARMOVESTATE_MOVE;
	}

	bResult = IsStateMove ();
	if (bResult) {
		switch (nMoveState) {
		case CHARMOVESTATE_STAND:				/* 立ち */
		case CHARMOVESTATE_BATTLEATACK:			/* 戦闘攻撃中 */
			AddMovePosQue (nMoveState, -1, -1, -1);
			return;
		case CHARMOVESTATE_MOVE:				/* 移動中 */
			CInfoCharBase::SetMoveState (nMoveState);
			return;
		}
	}
	bInitMove = FALSE;
	switch (nMoveState) {
	case CHARMOVESTATE_STAND:				/* 立ち */
	case CHARMOVESTATE_MOVE:				/* 移動中 */
		m_dwLastTimeMove = 0;
		break;
	case CHARMOVESTATE_DELETEREADY:			/* 消去準備 */
	case CHARMOVESTATE_DELETE:				/* 消去 */
		break;
	case CHARMOVESTATE_BATTLEMOVE:			/* 戦闘移動中 */
	case CHARMOVESTATE_BATTLEATACK:			/* 戦闘攻撃中 */
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

	CInfoCharCli::ChgMoveState (nMoveState);
	m_nAnime = m_nDirection;
	if (bInitMove) {
		m_ptMove.x = m_ptMove.y = 0;
	}
}


/* ========================================================================= */
/* 関数名	:CInfoCharMOVEATACKCli::SetViewState							 */
/* 内容		:表示状態設定													 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

void CInfoCharMOVEATACKCli::SetViewState(int nViewState)
{
	CInfoCharCli::SetViewState (nViewState);

	if (m_nViewState == INFOCHARCLI_VIEWSTATE_FADEOUT) {
		m_nFadeLevel = 50;
		return;
	}
	/* フェードイン・フェードアウトは無し */
	m_dwLastTimeViewState = 0;
}


/* ========================================================================= */
/* 関数名	:CInfoCharMOVEATACKCli::GetMotionInfo							 */
/* 内容		:モーション情報を取得											 */
/* 日付		:2007/12/23														 */
/* ========================================================================= */

CInfoMotion *CInfoCharMOVEATACKCli::GetMotionInfo(int *pnCount)
{
	m_nAnime = m_nDirection;
	return CInfoCharCli::GetMotionInfo (pnCount);
}

/* Copyright(C)URARA-works 2007 */
