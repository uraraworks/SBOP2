/* Copyright(C)URARA-works 2025 */
/* ========================================================================= */
/* ファイル名	:SDLWindow.h												 */
/* 内容			:SDLウィンドウ ラッパー 定義ファイル						 */
/* 作成開始日	:2025/06/01													 */
/* 説明			:SDL_CreateWindow でウィンドウを生成し、					 */
/*				 SDL_GetWindowWMInfo 経由で HWND を取得・公開する。			 */
/*				 既存の GDI 描画コードはこの HWND を使って描画する。		 */
/* ========================================================================= */

#pragma once

/* SDL_syswm.h は SDL_GetWindowWMInfo に必要 */
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_syswm.h>

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CSDLWindow
{
public:
			CSDLWindow();					/* コンストラクタ */
	virtual ~CSDLWindow();					/* デストラクタ */

	/* ウィンドウ作成                                                     */
	/* pszTitle: タイトル（ANSI文字列）                                   */
	/* nWidth, nHeight: クライアント領域サイズ（SCRSIZEX/SCRSIZEY を渡す）*/
	BOOL	Create(const char *pszTitle, int nWidth, int nHeight);

	/* 後片付け */
	void	Destroy(void);

	/* Win32 HWNDを返す（既存コードに渡すために使用） */
	HWND	GetHWND(void) const;

	/* SDL_Windowポインタを返す（SDLAppが使用） */
	SDL_Window	*GetSDLWindow(void) const;

	/* ウィンドウの有効状態 */
	BOOL	IsValid(void) const;

private:
	SDL_Window	*m_pWindow;			/* SDLウィンドウ */
	HWND		m_hWnd;				/* Win32 HWNDキャッシュ */
};

/* Copyright(C)URARA-works 2025 */
