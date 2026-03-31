/// @file InfoCharATACKANIMESvr.cpp
/// @brief キャラ情報(攻撃受けるとアニメーション)サーバークラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/17
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "InfoCharATACKANIMESvr.h"

CInfoCharATACKANIMESvr::CInfoCharATACKANIMESvr()
{
}

CInfoCharATACKANIMESvr::~CInfoCharATACKANIMESvr()
{
}

BOOL CInfoCharATACKANIMESvr::ProcHit(CInfoCharSvr *pInfoChar)
{
//Todo:暫定
	if (m_wGrpIDNPC != 10) {
//		m_nReserveChgEfect = 1;
//		goto Exit;
	}

	SetMoveState(CHARMOVESTATE_ANIME);

//Exit:
	return FALSE;
}

BOOL CInfoCharATACKANIMESvr::TimerProc(DWORD dwTime)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_bChgMoveState) {
		goto Exit;
	}
	if (m_nMoveState == CHARMOVESTATE_ANIME) {
		m_nMoveState = CHARMOVESTATE_STAND;
	}

Exit:
	return bRet;
}
