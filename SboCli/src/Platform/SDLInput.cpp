/// @file SDLInput.cpp
/// @brief SDL入力マッパー 実装ファイル
/// @date 2025/06/01
/// @copyright Copyright(C)URARA-works 2025

#include "stdafx.h"
#include "SDLInput.h"


int CSDLInput::ScancodeToVK(SDL_Scancode scancode)
{
	switch (scancode)
	{
	// 文字キー
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

	// 方向キー
	case SDL_SCANCODE_UP:		return VK_UP;
	case SDL_SCANCODE_DOWN:		return VK_DOWN;
	case SDL_SCANCODE_LEFT:		return VK_LEFT;
	case SDL_SCANCODE_RIGHT:	return VK_RIGHT;

	// 制御キー
	case SDL_SCANCODE_RETURN:	return VK_RETURN;
	case SDL_SCANCODE_ESCAPE:	return VK_ESCAPE;
	case SDL_SCANCODE_TAB:		return VK_TAB;
	case SDL_SCANCODE_LSHIFT:
	case SDL_SCANCODE_RSHIFT:	return VK_SHIFT;
	case SDL_SCANCODE_LCTRL:
	case SDL_SCANCODE_RCTRL:	return VK_CONTROL;
	case SDL_SCANCODE_SPACE:	return VK_SPACE;

	// @ キー（日本語配列では VK_OEM_3 に対応）
	case SDL_SCANCODE_GRAVE:	return VK_OEM_3;

	default:					return 0;
	}
}

SDL_Scancode CSDLInput::VKToScancode(int vk)
{
	switch (vk)
	{
	// 文字キー
	case 'A':			return SDL_SCANCODE_A;
	case 'B':			return SDL_SCANCODE_B;
	case 'C':			return SDL_SCANCODE_C;
	case 'F':			return SDL_SCANCODE_F;
	case 'G':			return SDL_SCANCODE_G;
	case 'H':			return SDL_SCANCODE_H;
	case 'I':			return SDL_SCANCODE_I;
	case 'J':			return SDL_SCANCODE_J;
	case 'K':			return SDL_SCANCODE_K;
	case 'L':			return SDL_SCANCODE_L;
	case 'M':			return SDL_SCANCODE_M;
	case 'N':			return SDL_SCANCODE_N;
	case 'P':			return SDL_SCANCODE_P;
	case 'R':			return SDL_SCANCODE_R;
	case 'S':			return SDL_SCANCODE_S;
	case 'V':			return SDL_SCANCODE_V;
	case 'X':			return SDL_SCANCODE_X;
	case 'Z':			return SDL_SCANCODE_Z;

	// 方向キー
	case VK_UP:			return SDL_SCANCODE_UP;
	case VK_DOWN:		return SDL_SCANCODE_DOWN;
	case VK_LEFT:		return SDL_SCANCODE_LEFT;
	case VK_RIGHT:		return SDL_SCANCODE_RIGHT;

	// 制御キー
	case VK_RETURN:		return SDL_SCANCODE_RETURN;
	case VK_ESCAPE:		return SDL_SCANCODE_ESCAPE;
	case VK_TAB:		return SDL_SCANCODE_TAB;
	case VK_SHIFT:		return SDL_SCANCODE_LSHIFT;		// 左Shiftを代表値として返す
	case VK_CONTROL:	return SDL_SCANCODE_LCTRL;		// 左Ctrlを代表値として返す
	case VK_SPACE:		return SDL_SCANCODE_SPACE;

	// @ キー（日本語配列）
	case VK_OEM_3:		return SDL_SCANCODE_GRAVE;

	default:			return SDL_SCANCODE_UNKNOWN;
	}
}

BOOL CSDLInput::IsVKPressed(int vk, const Uint8 *pKeyboardState)
{
	SDL_Scancode scancode;

	if (pKeyboardState == NULL) {
		return FALSE;
	}

	switch (vk)
	{
	case VK_SHIFT:
		return pKeyboardState[SDL_SCANCODE_LSHIFT] || pKeyboardState[SDL_SCANCODE_RSHIFT];

	case VK_CONTROL:
		return pKeyboardState[SDL_SCANCODE_LCTRL] || pKeyboardState[SDL_SCANCODE_RCTRL];

	default:
		scancode = VKToScancode(vk);
		if (scancode == SDL_SCANCODE_UNKNOWN) {
			return FALSE;
		}
		return pKeyboardState[scancode] ? TRUE : FALSE;
	}
}
