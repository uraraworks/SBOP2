/// @file BrowserCompat.h
/// @brief 非Windows環境向けの Windows 互換定義（ハブヘッダ）
/// @date 2026/04/01
/// @copyright Copyright(C)URARA-works 2026

#pragma once

#if !defined(_WIN32)

#include <SDL.h>
#include "Platform/SdlFont.h"
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cerrno>
#include <cwctype>
#include <cwchar>
#include <ctime>
#include <cmath>
#include <codecvt>
#include <cstdio>
#include <algorithm>
#include <locale>
#include <map>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

// 型定義・構造体・定数・マクロ（依存関係の起点）
#include "PlatformDefs.h"
// CStringTCompat テンプレートクラス
#include "CStringCompat.h"
// TCHAR系文字列操作・MultiByteToWideChar 等
#include "TCharCompat.h"
// GDI描画関数スタブ（SdlFont 連携）
#include "GdiStubs.h"
// ウィンドウ/メッセージ関数スタブ
#include "WndProcCompat.h"
// その他Win32 APIスタブ群
#include "Win32ApiStubs.h"

#endif // !_WIN32
