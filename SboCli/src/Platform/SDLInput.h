/// @file SDLInput.h
/// @brief SDL入力マッパー 定義ファイル
/// @date 2025/06/01
/// @copyright Copyright(C)URARA-works 2025
/// @details Phase 2 では GetKeyState() + Win32 HWNDフォーカスを使うため
///          このクラスは SDL Scancode → Win32 VK コードのマッピング
///          テーブルを提供するのみ。
///          Phase 3 での SDL_GetKeyboardState() 完全移行時に使う。

#pragma once

#define SDL_MAIN_HANDLED
#include <SDL.h>

// クラス宣言

class CSDLInput
{
public:
	// SDL_Scancode → Win32 仮想キーコード (VK_*) に変換する
	// 対応がない場合は 0 を返す
	static int ScancodeToVK(SDL_Scancode scancode);

	// Win32 仮想キーコード (VK_*) → SDL_Scancode に変換する
	// Phase 3 で SDL_GetKeyboardState() 移行時に使用
	// 対応がない場合は SDL_SCANCODE_UNKNOWN を返す
	static SDL_Scancode VKToScancode(int vk);

	// Win32 仮想キーコード (VK_*) の押下状態を SDL キーボード状態から判定
	// 対応がない場合は FALSE を返す
	static BOOL IsVKPressed(int vk, const Uint8 *pKeyboardState);
};
