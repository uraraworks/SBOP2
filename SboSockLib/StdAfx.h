/// @file stdafx.h
/// @brief プリコンパイルヘッダ 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/01/20
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#define WIN32_LEAN_AND_MEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windowsx.h>
#include <process.h>
#include <shlwapi.h>
#include <tchar.h>
#include <map>
using namespace std;

#include "myString.h"
#include "myArray.h"
#include "GlobalDefine.h"
#include "SBOGlobal.h"

//{{AFX_INSERT_LOCATION}}
