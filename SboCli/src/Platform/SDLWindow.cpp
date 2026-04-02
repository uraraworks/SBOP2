/// @file SDLWindow.cpp
/// @brief SDLウィンドウ ラッパー 実装ファイル
/// @date 2025/06/01
/// @copyright Copyright(C)URARA-works 2025

#include "stdafx.h"
#include "SDLWindow.h"


CSDLWindow::CSDLWindow()
{
	m_pWindow = NULL;
	m_pRenderer = NULL;
}

CSDLWindow::~CSDLWindow()
{
	Destroy();
}

BOOL CSDLWindow::Create(const char *pszTitle, int nWidth, int nHeight)
{
	m_pWindow = SDL_CreateWindow(
					pszTitle,
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					nWidth,
					nHeight,
					SDL_WINDOW_SHOWN);

	return (m_pWindow != NULL) ? TRUE : FALSE;
}

BOOL CSDLWindow::CreateRenderer(void)
{
	Uint32 dwRendererFlags;

	if (m_pWindow == NULL) {
		return FALSE;
	}

#if defined(__EMSCRIPTEN__)
	dwRendererFlags = SDL_RENDERER_ACCELERATED;
#else
	dwRendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
#endif

	m_pRenderer = SDL_CreateRenderer(
					m_pWindow,
					-1,
					dwRendererFlags);
#if defined(__EMSCRIPTEN__)
	if (m_pRenderer == NULL) {
		m_pRenderer = SDL_CreateRenderer(
						m_pWindow,
						-1,
						SDL_RENDERER_SOFTWARE);
	}
	return (m_pRenderer != NULL) ? TRUE : FALSE;
#else
	if (m_pRenderer == NULL) {
		m_pRenderer = SDL_CreateRenderer(
						m_pWindow,
						-1,
						SDL_RENDERER_SOFTWARE);
	}

	return (m_pRenderer != NULL) ? TRUE : FALSE;
#endif
}

void CSDLWindow::Destroy(void)
{
	if (m_pRenderer != NULL) {
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = NULL;
	}
	if (m_pWindow != NULL) {
		SDL_DestroyWindow(m_pWindow);
		m_pWindow = NULL;
	}
}

SDL_Window *CSDLWindow::GetSDLWindow(void) const
{
	return m_pWindow;
}

SDL_Renderer *CSDLWindow::GetRenderer(void) const
{
	return m_pRenderer;
}

BOOL CSDLWindow::IsValid(void) const
{
	return (m_pWindow != NULL) ? TRUE : FALSE;
}
