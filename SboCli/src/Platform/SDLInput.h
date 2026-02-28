/* Copyright(C)URARA-works 2025 */
/* ========================================================================= */
/* ファイル名	:SDLInput.h													 */
/* 内容			:SDL入力マッパー 定義ファイル								 */
/* 作成開始日	:2025/06/01													 */
/* 説明			:Phase 2 では GetKeyState() + Win32 HWNDフォーカスを使うため */
/*				 このクラスは SDL Scancode → Win32 VK コードのマッピング	 */
/*				 テーブルを提供するのみ。									 */
/*				 Phase 3 での SDL_GetKeyboardState() 完全移行時に使う。		 */
/* ========================================================================= */

#pragma once

#define SDL_MAIN_HANDLED
#include <SDL.h>

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

class CSDLInput
{
public:
	/* SDL_Scancode → Win32 仮想キーコード (VK_*) に変換する */
	/* 対応がない場合は 0 を返す                              */
	static int ScancodeToVK(SDL_Scancode scancode);

	/* Win32 仮想キーコード (VK_*) → SDL_Scancode に変換する */
	/* Phase 3 で SDL_GetKeyboardState() 移行時に使用        */
	/* 対応がない場合は SDL_SCANCODE_UNKNOWN を返す           */
	static SDL_Scancode VKToScancode(int vk);
};

/* Copyright(C)URARA-works 2025 */
