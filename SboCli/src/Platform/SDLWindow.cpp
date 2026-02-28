/* Copyright(C)URARA-works 2025 */
/* ========================================================================= */
/* ファイル名	:SDLWindow.cpp												 */
/* 内容			:SDLウィンドウ ラッパー 実装ファイル						 */
/* 作成開始日	:2025/06/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "SDLWindow.h"
#include "SboCli_priv.h"	/* SCRSIZEX / SCRSIZEY */


/* ========================================================================= */
/* 関数名	:CSDLWindow::CSDLWindow											 */
/* 内容		:コンストラクタ													 */
/* 日付		:2025/06/01														 */
/* ========================================================================= */

CSDLWindow::CSDLWindow()
{
	m_pWindow	= NULL;
	m_pRenderer	= NULL;
	m_hWnd		= NULL;
}


/* ========================================================================= */
/* 関数名	:CSDLWindow::~CSDLWindow										 */
/* 内容		:デストラクタ													 */
/* 日付		:2025/06/01														 */
/* ========================================================================= */

CSDLWindow::~CSDLWindow()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CSDLWindow::Create												 */
/* 内容		:SDLウィンドウを生成し、Win32 HWNDを取得する					 */
/* 日付		:2025/06/01														 */
/* ========================================================================= */

BOOL CSDLWindow::Create(const char *pszTitle, int nWidth, int nHeight)
{
	SDL_SysWMinfo wmInfo;

	/* SDLウィンドウ生成                                          */
	/* SDL_WINDOW_SHOWN: 初期表示                                 */
	/* SDL_WINDOW_RESIZABLE は付けない（固定サイズゲームのため）  */
	m_pWindow = SDL_CreateWindow (
					pszTitle,
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					nWidth,
					nHeight,
					SDL_WINDOW_SHOWN);

	if (m_pWindow == NULL) {
		return FALSE;
	}

	/* SDL_GetWindowWMInfo で Win32 HWND を取得 */
	SDL_VERSION (&wmInfo.version);
	if (!SDL_GetWindowWMInfo (m_pWindow, &wmInfo)) {
		SDL_DestroyWindow (m_pWindow);
		m_pWindow = NULL;
		return FALSE;
	}

	m_hWnd = wmInfo.info.win.window;

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CSDLWindow::Destroy											 */
/* 内容		:SDLウィンドウを破棄する										 */
/* 日付		:2025/06/01														 */
/* ========================================================================= */

/* ========================================================================= */
/* 関数名	:CSDLWindow::CreateRenderer										 */
/* 内容		:SDL_Renderer を生成する										 */
/* 日付		:2025/06/01														 */
/* 補足		:Create() でウィンドウ生成後に呼ぶこと							 */
/*			 アクセラレーション + VSync 有効で生成							 */
/* ========================================================================= */

BOOL CSDLWindow::CreateRenderer(void)
{
	if (m_pWindow == NULL) {
		return FALSE;
	}

	m_pRenderer = SDL_CreateRenderer (
					m_pWindow,
					-1,		/* 最初に利用可能なドライバを自動選択 */
					SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	return (m_pRenderer != NULL) ? TRUE : FALSE;
}


void CSDLWindow::Destroy(void)
{
	if (m_pRenderer != NULL) {
		SDL_DestroyRenderer (m_pRenderer);
		m_pRenderer = NULL;
	}
	if (m_pWindow != NULL) {
		SDL_DestroyWindow (m_pWindow);
		m_pWindow = NULL;
	}
	m_hWnd = NULL;
}


/* ========================================================================= */
/* 関数名	:CSDLWindow::GetHWND											 */
/* 内容		:Win32 HWNDを返す												 */
/* 日付		:2025/06/01														 */
/* ========================================================================= */

HWND CSDLWindow::GetHWND(void) const
{
	return m_hWnd;
}


/* ========================================================================= */
/* 関数名	:CSDLWindow::GetSDLWindow										 */
/* 内容		:SDL_Windowポインタを返す										 */
/* 日付		:2025/06/01														 */
/* ========================================================================= */

SDL_Window *CSDLWindow::GetSDLWindow(void) const
{
	return m_pWindow;
}


/* ========================================================================= */
/* 関数名	:CSDLWindow::GetRenderer										 */
/* 内容		:SDL_Renderer ポインタを返す									 */
/* 日付		:2025/06/01														 */
/* ========================================================================= */

SDL_Renderer *CSDLWindow::GetRenderer(void) const
{
	return m_pRenderer;
}


/* ========================================================================= */
/* 関数名	:CSDLWindow::IsValid											 */
/* 内容		:ウィンドウが有効かどうかを返す									 */
/* 日付		:2025/06/01														 */
/* ========================================================================= */

BOOL CSDLWindow::IsValid(void) const
{
	return (m_pWindow != NULL) ? TRUE : FALSE;
}

/* Copyright(C)URARA-works 2025 */
