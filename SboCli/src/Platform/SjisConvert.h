/// @file SjisConvert.h
/// @brief SJIS→wchar_t 変換関数の宣言
/// @date 2026/04/14

#pragma once

#include <string>

/// SJIS (CP932) バイト列を wstring (UTF-32/UTF-16) に変換する
/// Windows では MultiByteToWideChar、Emscripten では TextDecoder('shift-jis') を使用する
/// @param pszSrc   変換元の SJIS 文字列（NULL 終端または nSrcLen 指定）
/// @param nSrcLen  変換するバイト数。負値の場合は strlen で自動計算
/// @return 変換後の wstring。変換失敗時は空文字列
std::wstring SjisToWstring(const char* pszSrc, int nSrcLen);

/// wstring を SJIS (CP932) バイト列に変換する
/// Windows では WideCharToMultiByte、Emscripten では事前構築した逆引きマップを使用する
/// @param pszSrc   変換元の wstring（NULL 終端または nSrcLen 指定）
/// @param nSrcLen  変換する wchar_t 数。負値の場合は wcslen で自動計算
/// @return 変換後の SJIS バイト列を格納した std::string。変換失敗時は空文字列
std::string WstringToSjis(const wchar_t* pszSrc, int nSrcLen);
