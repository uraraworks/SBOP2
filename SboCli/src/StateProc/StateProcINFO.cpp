/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名	:StateProcINFO.cpp											 */
/* 内容			:状態処理クラス(お知らせ) 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2005/08/16													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrData.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "StateProcINFO.h"


/* ========================================================================= */
/* 関数名	:CStateProcINFO::CStateProcINFO									 */
/* 内容		:コンストラクタ													 */
/* 日付		:2005/08/16														 */
/* ========================================================================= */

CStateProcINFO::CStateProcINFO()
{
}


/* ========================================================================= */
/* 関数名	:CStateProcINFO::~CStateProcINFO								 */
/* 内容		:デストラクタ													 */
/* 日付		:2005/08/16														 */
/* ========================================================================= */

CStateProcINFO::~CStateProcINFO()
{
}


/* ========================================================================= */
/* 関数名	:CStateProcINFO::Init											 */
/* 内容		:初期化															 */
/* 日付		:2005/08/16														 */
/* ========================================================================= */

void CStateProcINFO::Init(void)
{
	m_pMgrLayer->MakeTITLE ();
	m_pMgrLayer->MakeINFO ();
}


/* ========================================================================= */
/* 関数名	:CStateProcINFO::OnLButtonDown									 */
/* 内容		:メッセージハンドラ(WM_LBUTTONDOWN)								 */
/* 日付		:2005/08/16														 */
/* ========================================================================= */

void CStateProcINFO::OnLButtonDown(int x, int y)
{
	PostMessage (m_hWndMain, WM_MAINFRAME, MAINFRAMEMSG_CHGSTATE, GAMESTATE_LOGINMENU);
}


/* ========================================================================= */
/* 関数名	:CStateProcINFO::OnX											 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2005/08/21														 */
/* ========================================================================= */

BOOL CStateProcINFO::OnX(BOOL bDown)
{
	if (bDown == TRUE) {
		goto Exit;
	}

	OnLButtonDown (0, 0);

Exit:
	return FALSE;
}

/* Copyright(C)URARA-works 2005 */
