/// @file stdafx.h
/// @brief プリコンパイルヘッダ 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/01/20
/// @copyright Copyright(C)URARA-works 2008

#pragma once


#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif
#include <windows.h>
#include <windowsx.h>
#include <process.h>
#include <shlwapi.h>

#else

// 非Windows（Linux の CMake ビルド）では共有の互換レイヤを読む
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cwchar>
#include <string>
// PlatformDefs.h が min/max をマクロ定義するため、標準ライブラリはそれより先に読む
#include <algorithm>
#include <cmath>
#include <map>
#include <mutex>
#include <random>
#include "../Common/Platform/PlatformDefs.h"
#include "../Common/Platform/CStringCompat.h"
#include "../Common/Platform/TCharCompat.h"

#endif // _WIN32

//{{AFX_INSERT_LOCATION}}
