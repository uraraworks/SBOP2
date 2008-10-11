/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoCharATACKANIMESvr.cpp									 */
/* 内容			:キャラ情報(攻撃受けるとアニメーション)サーバークラス 実装ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/17													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoCharATACKANIMESvr.h"


/* ========================================================================= */
/* 関数名	:CInfoCharATACKANIMESvr::CInfoCharATACKANIMESvr					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

CInfoCharATACKANIMESvr::CInfoCharATACKANIMESvr()
{
}


/* ========================================================================= */
/* 関数名	:CInfoCharATACKANIMESvr::~CInfoCharATACKANIMESvr				 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

CInfoCharATACKANIMESvr::~CInfoCharATACKANIMESvr()
{
}


/* ========================================================================= */
/* 関数名	:CInfoCharATACKANIMESvr::ProcHit								 */
/* 内容		:処理(攻撃を受けた時)											 */
/* 日付		:2007/09/17														 */
/* 戻り値	:TRUE:以降の処理を続行する										 */
/* ========================================================================= */

BOOL CInfoCharATACKANIMESvr::ProcHit(CInfoCharSvr *pInfoChar)
{
//Todo:暫定
	if (m_wGrpIDNPC != 10) {
//		m_nReserveChgEfect = 1;
//		goto Exit;
	}

	SetMoveState (CHARMOVESTATE_ANIME);

//Exit:
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CInfoCharSvr::TimerProc										 */
/* 内容		:時間処理														 */
/* 日付		:2007/09/17														 */
/* ========================================================================= */

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

/* Copyright(C)URARA-works 2007 */
