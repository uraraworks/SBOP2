/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:InfoCharSCORECli.cpp										 */
/* 内容			:キャラ情報(得点)クライアントクラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/09													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoCharSCORECli.h"


/* ========================================================================= */
/* 関数名	:CInfoCharSCORECli::CInfoCharSCORECli							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/09/09														 */
/* ========================================================================= */

CInfoCharSCORECli::CInfoCharSCORECli()
{
	m_bMotionDirection = FALSE;
	m_adwMotionID[CHARMOTIONID_STAND]	= CHARMOTIONLISTID_SIMPLE;	/* モーションID(立ち) */
	m_adwMotionID[CHARMOTIONID_WALK]	= CHARMOTIONLISTID_SIMPLE;	/* モーションID(歩き) */
}


/* ========================================================================= */
/* 関数名	:CInfoCharSCORECli::~CInfoCharSCORECli							 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/09/09														 */
/* ========================================================================= */

CInfoCharSCORECli::~CInfoCharSCORECli()
{
}


/* ========================================================================= */
/* 関数名	:CInfoCharSCORECli::ChgMoveState								 */
/* 内容		:移動状態変更													 */
/* 日付		:2007/09/09														 */
/* ========================================================================= */

void CInfoCharSCORECli::ChgMoveState(int nMoveState)
{
	int nAnimeBack;

	nAnimeBack = m_nAnime;
	CInfoCharCli::ChgMoveState (nMoveState);
	m_nAnime = nAnimeBack;
}

/* Copyright(C)URARA-works 2007 */
