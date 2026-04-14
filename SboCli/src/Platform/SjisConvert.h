/// @file SjisConvert.h
/// @brief Emscripten 環境向け SJIS→wchar_t 変換関数の宣言
/// @date 2026/04/14

#pragma once

#if !defined(_WIN32)

#include <string>

/// SJIS (CP932) バイト列を wstring (UTF-32/UTF-16) に変換する
/// Emscripten 環境では ブラウザの TextDecoder('shift-jis') を使用する
/// @param pszSrc   変換元の SJIS 文字列（NULL 終端または nSrcLen 指定）
/// @param nSrcLen  変換するバイト数。負値の場合は strlen で自動計算
/// @return 変換後の wstring。変換失敗時は空文字列
std::wstring SjisToWstring(const char* pszSrc, int nSrcLen);

#endif // !_WIN32
