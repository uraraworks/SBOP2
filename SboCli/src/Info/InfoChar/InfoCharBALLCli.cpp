/// @file InfoCharBALLCli.cpp
/// @brief キャラ情報(ボール)クライアントクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/02
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "InfoMotion.h"
#include "InfoCharBALLCli.h"


CInfoCharBALLCli::CInfoCharBALLCli()
{
	m_dwMoveWait	= 6;
	m_bMotionDirection	= FALSE;

	m_adwMotionID[CHARMOTIONID_STAND]	= CHARMOTIONLISTID_STAND;	// モーションID(立ち)
	m_adwMotionID[CHARMOTIONID_WALK]	= CHARMOTIONLISTID_WALK;	// モーションID(歩き)
}


CInfoCharBALLCli::~CInfoCharBALLCli()
{
}


void CInfoCharBALLCli::ChgMoveState(int nMoveState)
{
	CInfoCharCli::ChgMoveState(nMoveState);

	switch (nMoveState) {
	case CHARMOVESTATE_MOVE:	// 移動中
		m_nAnime = 0;
		m_dwLastTimeAnime = timeGetTime();
		break;
	}
}

