/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:StateProcLOGIN.cpp											 */
/* 内容			:状態処理クラス(ログイン画面) 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "WindowLOGIN.h"
#include "LayerTitle.h"
#include "MgrData.h"
#include "MgrSound.h"
#include "MgrWindow.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "StateProcLOGIN.h"

/* ========================================================================= */
/* 関数名	:CStateProcLOGIN::CStateProcLOGIN								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2005/05/29														 */
/* ========================================================================= */

CStateProcLOGIN::CStateProcLOGIN()
{
	m_bFadeIn = FALSE;
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGIN::~CStateProcLOGIN								 */
/* 内容		:デストラクタ													 */
/* 日付		:2005/05/29														 */
/* ========================================================================= */

CStateProcLOGIN::~CStateProcLOGIN()
{
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGIN::Init											 */
/* 内容		:初期化															 */
/* 日付		:2005/05/29														 */
/* ========================================================================= */

void CStateProcLOGIN::Init(void)
{
	PCWindowLOGIN pWindow;

	m_pMgrWindow->	MakeWindowLOGIN ();
	m_pMgrDraw->	SetFadeState (FADESTATE_FADEIN);
	m_pMgrLayer->	MakeTITLE ();

	pWindow = (PCWindowLOGIN)m_pMgrWindow->GetWindow (WINDOWTYPE_LOGIN);
	pWindow->SetShow (FALSE);

	m_pMgrSound->PlayBGM (BGMID_HISYOU);
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGIN::TimerProc										 */
/* 内容		:時間処理														 */
/* 日付		:2007/03/26														 */
/* ========================================================================= */

BOOL CStateProcLOGIN::TimerProc(void)
{
	PCLayerTitle pLayer;
	PCWindowLOGIN pWindow;

	if (m_bFadeIn) {
		return FALSE;
	}

	pLayer = (PCLayerTitle)m_pMgrLayer->Get (LAYERTYPE_TITLE);
	if (pLayer->IsFadeInEnd ()) {
		m_bFadeIn = TRUE;

		pWindow = (PCWindowLOGIN)m_pMgrWindow->GetWindow (WINDOWTYPE_LOGIN);
		pWindow->SetShow (TRUE);
	}

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGIN::OnLButtonDown									 */
/* 内容		:メッセージハンドラ(WM_LBUTTONDOWN)								 */
/* 日付		:2007/04/02														 */
/* ========================================================================= */

void CStateProcLOGIN::OnLButtonDown(int x, int y)
{
	PCLayerTitle pLayer;

	pLayer = (PCLayerTitle)m_pMgrLayer->Get (LAYERTYPE_TITLE);
	pLayer->EndFadeIn ();
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGIN::OnX											 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2007/04/02														 */
/* ========================================================================= */

BOOL CStateProcLOGIN::OnX(BOOL bDown)
{
	OnLButtonDown (0, 0);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CStateProcLOGIN::OnMgrDrawSTART_FADEIN							 */
/* 内容		:フェードイン開始												 */
/* 日付		:2007/03/26														 */
/* ========================================================================= */

void CStateProcLOGIN::OnMgrDrawSTART_FADEIN(DWORD dwPara)
{
	PCLayerTitle pLayer;

	pLayer = (PCLayerTitle)m_pMgrLayer->Get (LAYERTYPE_TITLE);
	pLayer->StartFadeIn ();
}

/* Copyright(C)URARA-works 2006 */
