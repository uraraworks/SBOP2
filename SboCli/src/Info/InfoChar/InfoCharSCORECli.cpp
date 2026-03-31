/// @file InfoCharSCORECli.cpp
/// @brief キャラ情報(得点)クライアントクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/09
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "InfoCharSCORECli.h"


CInfoCharSCORECli::CInfoCharSCORECli()
{
	m_bMotionDirection = FALSE;
	m_adwMotionID[CHARMOTIONID_STAND]	= CHARMOTIONLISTID_SIMPLE;	// モーションID(立ち)
	m_adwMotionID[CHARMOTIONID_WALK]	= CHARMOTIONLISTID_SIMPLE;	// モーションID(歩き)
}


CInfoCharSCORECli::~CInfoCharSCORECli()
{
}


void CInfoCharSCORECli::ChgMoveState(int nMoveState)
{
	int nAnimeBack;

	nAnimeBack = m_nAnime;
	CInfoCharCli::ChgMoveState(nMoveState);
	m_nAnime = nAnimeBack;
}

