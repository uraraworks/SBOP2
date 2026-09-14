/// @file InfoCharBALLCli.cpp
/// @brief キャラ情報(ボール)クライアントクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/02
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
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
	BOOL bWasMove;

	bWasMove = IsStateMove();
	CInfoCharCli::ChgMoveState(nMoveState);

	switch (nMoveState) {
	case CHARMOVESTATE_MOVE:	// 移動中
		if (!bWasMove) {
			// MOVE 以外から MOVE に切り替わった時だけコマを戻す。見る側は
			// サーバーからほぼ毎パケットで MOVE を受け取るため、ここを毎回
			// リセットすると常に1コマ目に固定され転がって見えなくなる。
			m_nAnime = 0;
			m_dwLastTimeAnime = timeGetTime();
		}
		break;
	}
}

