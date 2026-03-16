/* Copyright(C)URARA-works 2025 */
/* ========================================================================= */
/* ファイル名	:SDLWindow.cpp												 */
/* 内容			:SDLウィンドウ ラッパー 実装ファイル						 */
/* 作成日		:2025/06/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "SDLWindow.h"


/* ========================================================================= */
/* 関数名	:CSDLWindow::CSDLWindow											 */
/* 内容		:コンストラクタ													 */
/* 日付		:2025/06/01														 */
/* ========================================================================= */

CSDLWindow::CSDLWindow()
{
	m_pWindow	= NULL;
	m_pRenderer	= NULL;
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
/* 内容		:SDLウィンドウを生成する										 */
/* 日付		:2025/06/01														 */
/* ========================================================================= */

BOOL CSDLWindow::Create(const char *pszTitle, int nWidth, int nHeight)
{
	m_pWindow = SDL_CreateWindow (
					pszTitle,
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					nWidth,
					nHeight,
					SDL_WINDOW_SHOWN);

	return (m_pWindow != NULL) ? TRUE : FALSE;
}


/* ========================================================================= */
/* 関数名	:CSDLWindow::CreateRenderer										 */
/* 内容		:SDL_Renderer を生成する										 */
/* 日付		:2025/06/01														 */
/* ========================================================================= */

BOOL CSDLWindow::CreateRenderer(void)
{
	if (m_pWindow == NULL) {
		return FALSE;
	}

	m_pRenderer = SDL_CreateRenderer (
					m_pWindow,
					-1,
					SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	return (m_pRenderer != NULL) ? TRUE : FALSE;
}


/* ========================================================================= */
/* 関数名	:CSDLWindow::Destroy											 */
/* 内容		:SDLウィンドウを破棄する										 */
/* 日付		:2025/06/01														 */
/* ========================================================================= */

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
}


/* ========================================================================= */
/* 関数名	:CSDLWindow::GetSDLWindow										 */
/* 内容		:SDL_Window ポインタを返す										 */
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
/* 内容		:ウィンドウが有効か判定する									 */
/* 日付		:2025/06/01														 */
/* ========================================================================= */

BOOL CSDLWindow::IsValid(void) const
{
	return (m_pWindow != NULL) ? TRUE : FALSE;
}

/* Copyright(C)URARA-works 2025 */
