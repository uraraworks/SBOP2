/* Copyright(C)URARA-works 2025 */
/* ========================================================================= */
/* ファイル名	:SDLInput.cpp												 */
/* 内容			:SDL入力マッパー 実装ファイル								 */
/* 作成開始日	:2025/06/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "SDLInput.h"


/* ========================================================================= */
/* 関数名	:CSDLInput::ScancodeToVK										 */
/* 内容		:SDL_Scancode を Win32 仮想キーコードに変換する					 */
/* 日付		:2025/06/01														 */
/* 補足		:CMgrKeyInput が使用するキーコード（MgrKeyInput.cpp のテーブル）*/
/*			 に対応するものだけマッピングすれば十分							 */
/* ========================================================================= */

int CSDLInput::ScancodeToVK(SDL_Scancode scancode)
{
	switch (scancode)
	{
	/* 文字キー */
	case SDL_SCANCODE_A:		return 'A';
	case SDL_SCANCODE_B:		return 'B';
	case SDL_SCANCODE_C:		return 'C';
	case SDL_SCANCODE_F:		return 'F';
	case SDL_SCANCODE_G:		return 'G';
	case SDL_SCANCODE_H:		return 'H';
	case SDL_SCANCODE_I:		return 'I';
	case SDL_SCANCODE_J:		return 'J';
	case SDL_SCANCODE_K:		return 'K';
	case SDL_SCANCODE_L:		return 'L';
	case SDL_SCANCODE_M:		return 'M';
	case SDL_SCANCODE_N:		return 'N';
	case SDL_SCANCODE_P:		return 'P';
	case SDL_SCANCODE_R:		return 'R';
	case SDL_SCANCODE_S:		return 'S';
	case SDL_SCANCODE_V:		return 'V';
	case SDL_SCANCODE_X:		return 'X';
	case SDL_SCANCODE_Z:		return 'Z';

	/* 方向キー */
	case SDL_SCANCODE_UP:		return VK_UP;
	case SDL_SCANCODE_DOWN:		return VK_DOWN;
	case SDL_SCANCODE_LEFT:		return VK_LEFT;
	case SDL_SCANCODE_RIGHT:	return VK_RIGHT;

	/* 制御キー */
	case SDL_SCANCODE_RETURN:	return VK_RETURN;
	case SDL_SCANCODE_ESCAPE:	return VK_ESCAPE;
	case SDL_SCANCODE_TAB:		return VK_TAB;
	case SDL_SCANCODE_LSHIFT:
	case SDL_SCANCODE_RSHIFT:	return VK_SHIFT;
	case SDL_SCANCODE_LCTRL:
	case SDL_SCANCODE_RCTRL:	return VK_CONTROL;
	case SDL_SCANCODE_SPACE:	return VK_SPACE;

	/* @ キー（日本語配列では VK_OEM_3 に対応） */
	case SDL_SCANCODE_GRAVE:	return VK_OEM_3;

	default:					return 0;
	}
}

/* Copyright(C)URARA-works 2025 */
