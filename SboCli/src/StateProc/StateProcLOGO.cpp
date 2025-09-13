/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名	:StateProcLOGO.cpp											 */
/* 内容			:状態処理クラス(ロゴ) 実装ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2005/05/29													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrData.h"
#include "MgrWindow.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "MgrSound.h"
#include "StateProcLOGO.h"


/* ========================================================================= */
/* 関数名	:CStateProcLOGO::CStateProcLOGO									 */
/* 内容		:コンストラクタ													 */
/* 日付		:2005/05/29														 */
/* ========================================================================= */

CStateProcLOGO::CStateProcLOGO()
{
	m_dwTimeWait = 0;
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGO::~CStateProcLOGO								 */
/* 内容		:デストラクタ													 */
/* 日付		:2005/05/29														 */
/* ========================================================================= */

CStateProcLOGO::~CStateProcLOGO()
{
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGO::Init											 */
/* 内容		:初期化															 */
/* 日付		:2005/05/29														 */
/* ========================================================================= */

void CStateProcLOGO::Init(void)
{
	m_pMgrDraw->SetLevel (1);
	m_pMgrDraw->SetFadeState (FADESTATE_FADEIN);
	m_pMgrLayer->MakeLOGO ();
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGO::TimerProc										 */
/* 内容		:時間処理														 */
/* 日付		:2005/05/29														 */
/* ========================================================================= */

BOOL CStateProcLOGO::TimerProc(void)
{
	BOOL bRet;

	bRet = FALSE;
	if (m_dwTimeWait == 0) {
		goto Exit;
	}

	if (timeGetTime () - m_dwTimeWait < 3000) {
		goto Exit;
	}

	m_pMgrDraw->SetFadeState (FADESTATE_FADEOUT);
	m_dwTimeWait = 0;

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGO::OnLButtonDown									 */
/* 内容		:メッセージハンドラ(WM_LBUTTONDOWN)								 */
/* 日付		:2005/05/29														 */
/* ========================================================================= */

void CStateProcLOGO::OnLButtonDown(int x, int y)
{
	m_pMgrDraw->SetFadeState (FADESTATE_FADEOUT);
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGO::OnX											 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2007/03/21														 */
/* ========================================================================= */

BOOL CStateProcLOGO::OnX(BOOL bDown)
{
	OnLButtonDown (0, 0);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGO::OnZ											 */
/* 内容		:キーハンドラ(Z)												 */
/* 日付		:2007/03/21														 */
/* ========================================================================= */

BOOL CStateProcLOGO::OnZ(BOOL bDown)
{
	OnLButtonDown (0, 0);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGO::OnMgrDrawEND_FADEIN							 */
/* 内容		:フェードイン完了												 */
/* 日付		:2005/05/29														 */
/* ========================================================================= */

void CStateProcLOGO::OnMgrDrawEND_FADEIN(DWORD dwPara)
{
	m_dwTimeWait = timeGetTime ();
	m_pMgrSound->PlaySound (SOUNDID_URARA_WORKS);
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGO::OnMgrDrawEND_FADEOUT							 */
/* 内容		:フェードアウト完了												 */
/* 日付		:2005/05/29														 */
/* ========================================================================= */

void CStateProcLOGO::OnMgrDrawEND_FADEOUT(DWORD dwPara)
{
	PostMessage (m_hWndMain, WM_MAINFRAME, MAINFRAMEMSG_CHGSTATE, GAMESTATE_LOGIN);
}

/* Copyright(C)URARA-works 2005 */
