/// @file SDLWindow.h
/// @brief SDLウィンドウ ラッパー 定義ファイル
/// @date 2025/06/01
/// @copyright Copyright(C)URARA-works 2025
/// @details SDL_CreateWindow でウィンドウを生成し、
///          SDL_Renderer も管理する。
///          Win32 HWND は保持せず、必要な箇所で
///          SDL_GetWindowWMInfo から取得する。

#pragma once

#define SDL_MAIN_HANDLED
#include <SDL.h>

class CSDLWindow
{
public:
			CSDLWindow();
	virtual ~CSDLWindow();

	BOOL	Create(const char *pszTitle, int nWidth, int nHeight);
	BOOL	CreateRenderer(void);
	void	Destroy(void);

	SDL_Window		*GetSDLWindow(void) const;
	SDL_Renderer	*GetRenderer(void) const;
	BOOL			IsValid(void) const;

private:
	SDL_Window		*m_pWindow;
	SDL_Renderer	*m_pRenderer;
};
