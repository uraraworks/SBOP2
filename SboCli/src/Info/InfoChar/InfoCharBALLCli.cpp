/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:InfoCharBALLCli.cpp										 */
/* 内容			:キャラ情報(ボール)クライアントクラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/02													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMotion.h"
#include "InfoCharBALLCli.h"


/* ========================================================================= */
/* 関数名	:CInfoCharBALLCli::CInfoCharBALLCli								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/09/02														 */
/* ========================================================================= */

CInfoCharBALLCli::CInfoCharBALLCli()
{
	m_dwMoveWait		= 6;
	m_bMotionDirection	= FALSE;

	m_adwMotionID[CHARMOTIONID_STAND]	= CHARMOTIONLISTID_STAND;	/* モーションID(立ち) */
	m_adwMotionID[CHARMOTIONID_WALK]	= CHARMOTIONLISTID_WALK;	/* モーションID(歩き) */
}


/* ========================================================================= */
/* 関数名	:CInfoCharBALLCli::~CInfoCharBALLCli							 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/09/02														 */
/* ========================================================================= */

CInfoCharBALLCli::~CInfoCharBALLCli()
{
}


/* ========================================================================= */
/* 関数名	:CInfoCharBALLCli::ChgMoveState									 */
/* 内容		:移動状態変更													 */
/* 日付		:2007/09/02														 */
/* ========================================================================= */

void CInfoCharBALLCli::ChgMoveState(int nMoveState)
{
	CInfoCharCli::ChgMoveState (nMoveState);

	switch (nMoveState) {
	case CHARMOVESTATE_MOVE:		/* 移動中 */
		m_nAnime = 0;
		m_dwLastTimeAnime = timeGetTime ();
		break;
	}
}

/* Copyright(C)URARA-works 2007 */
