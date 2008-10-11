/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:WindowOPTION_ACTIONSET_SLEEPTIMER.cpp						 */
/* 内容			:オプション-動作設定-おひるねタイマーの設定ウィンドウクラス 実装ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/21													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "WindowOPTION_ACTIONSET_SLEEPTIMER.h"


/* ========================================================================= */
/* 関数名	:CWindowOPTION_ACTIONSET_SLEEPTIMER::CWindowOPTION_ACTIONSET_SLEEPTIMER */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/07/21														 */
/* ========================================================================= */

CWindowOPTION_ACTIONSET_SLEEPTIMER::CWindowOPTION_ACTIONSET_SLEEPTIMER()
{
	m_nPosMax		= 0;
	m_bInput		= TRUE;
	m_nID			= WINDOWTYPE_OPTION_ACTIONSET_SLEEPTIMER;
	m_ptViewPos.x	= 8 * 16;
	m_ptViewPos.y	= 16 * 13;
	m_sizeWindow.cx	= 16 * 2 + 16 * 20;
	m_sizeWindow.cy	= 16 * 2 + 16 * 1;
}


/* ========================================================================= */
/* 関数名	:CWindowOPTION_ACTIONSET_SLEEPTIMER::~CWindowOPTION_ACTIONSET_SLEEPTIMER */
/* 内容		:デストラクタ													 */
/* 日付		:2008/07/21														 */
/* ========================================================================= */

CWindowOPTION_ACTIONSET_SLEEPTIMER::~CWindowOPTION_ACTIONSET_SLEEPTIMER()
{
}


/* ========================================================================= */
/* 関数名	:CWindowOPTION_ACTIONSET_SLEEPTIMER::Create						 */
/* 内容		:作成															 */
/* 日付		:2008/07/21														 */
/* ========================================================================= */

void CWindowOPTION_ACTIONSET_SLEEPTIMER::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_bActive = TRUE;
	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);
	m_nPos = 0;

	m_nSleepTimer = m_pMgrData->GetSleepTimer ();
}


/* ========================================================================= */
/* 関数名	:CWindowOPTION_ACTIONSET_SLEEPTIMER::Draw						 */
/* 内容		:描画															 */
/* 日付		:2008/07/21														 */
/* ========================================================================= */

void CWindowOPTION_ACTIONSET_SLEEPTIMER::Draw(PCImg32 pDst)
{
	int i, nLevel;
	HDC hDC;
	HFONT hFontOld;
	LPCSTR apszTmp[] = {"３分", "５分", "10分", "30分", "60分"};
	COLORREF clText;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame ();

	clText		= RGB (1, 1, 1);
	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont);
	SetBkMode (hDC, TRANSPARENT);

	TextOut2 (hDC, 32, 16 + 16 * 0, "なし", clText);
	for (i = 0; i < 5; i ++) {
		TextOut2 (hDC, 32 + 51 + (i * 51), 16 + 16 * 0, apszTmp[i], clText);
	}

	DrawCursor (8 + (m_nSleepTimer * 51), 16 + 16 * 0);

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	m_dwTimeDrawStart = timeGetTime ();

Exit:
	nLevel = 100;
	if (m_bActive == FALSE) {
		nLevel = 60;
	}
	pDst->BltLevel (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, nLevel, TRUE);
}


/* ========================================================================= */
/* 関数名	:CWindowOPTION_ACTIONSET_SLEEPTIMER::OnUp						 */
/* 内容		:キーハンドラ(↑)												 */
/* 日付		:2008/07/21														 */
/* ========================================================================= */

BOOL CWindowOPTION_ACTIONSET_SLEEPTIMER::OnUp(void)
{
	BOOL bRet;

	bRet = FALSE;

	m_nSleepTimer = 0;

	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	bRet = TRUE;
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowOPTION_ACTIONSET_SLEEPTIMER::OnDown						 */
/* 内容		:キーハンドラ(↓)												 */
/* 日付		:2008/07/21														 */
/* ========================================================================= */

BOOL CWindowOPTION_ACTIONSET_SLEEPTIMER::OnDown(void)
{
	BOOL bRet;

	bRet = FALSE;

	m_nSleepTimer = 5;

	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	bRet = TRUE;
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowOPTION_ACTIONSET_SLEEPTIMER::OnLeft						 */
/* 内容		:キーハンドラ(←)												 */
/* 日付		:2008/07/21														 */
/* ========================================================================= */

BOOL CWindowOPTION_ACTIONSET_SLEEPTIMER::OnLeft(void)
{
	m_nSleepTimer --;
	m_nSleepTimer = max (m_nSleepTimer, 0);

	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowOPTION_ACTIONSET_SLEEPTIMER::OnRight					 */
/* 内容		:キーハンドラ(→)												 */
/* 日付		:2008/07/21														 */
/* ========================================================================= */

BOOL CWindowOPTION_ACTIONSET_SLEEPTIMER::OnRight(void)
{
	m_nSleepTimer ++;
	m_nSleepTimer = min (m_nSleepTimer, 5);

	m_nCursorAnime = 0;
	m_dwLastTimeCursor = 0;
	m_pMgrSound->PlaySound (SOUNDID_CURSORMOVE);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CWindowOPTION_ACTIONSET_SLEEPTIMER::OnX						 */
/* 内容		:キーハンドラ(X)												 */
/* 日付		:2008/07/21														 */
/* ========================================================================= */

BOOL CWindowOPTION_ACTIONSET_SLEEPTIMER::OnX(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_bDelete = TRUE;
	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);
	PostMessage (m_hWndMain, WM_WINDOWMSG, m_nID, m_nSleepTimer);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowOPTION_ACTIONSET_SLEEPTIMER::OnZ						 */
/* 内容		:キーハンドラ(Z)												 */
/* 日付		:2008/07/21														 */
/* ========================================================================= */

BOOL CWindowOPTION_ACTIONSET_SLEEPTIMER::OnZ(BOOL bDown)
{
	BOOL bRet;

	bRet = FALSE;
	if (bDown) {
		goto Exit;
	}

	m_bDelete = TRUE;
	m_pMgrSound->PlaySound (SOUNDID_CANCEL);

	bRet = TRUE;
Exit:
	return bRet;
}

/* Copyright(C)URARA-works 2008 */
