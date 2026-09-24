/// @file stdafx.h
/// @brief プリコンパイルヘッダ 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/01/20
/// @copyright Copyright(C)URARA-works 2008

#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windowsx.h>
#include <process.h>
#include <shlwapi.h>
#include <tchar.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#else
// 非Windows（Linux の CMake ビルド）
// SboSockLib はサーバー側（SboSvr / SboSvrTest）にリンクされるライブラリで、
// Common 配下の Info / LibInfo も取り込むため、SboSvr と同じ互換ヘッダを読む。
// （timeGetTime / SetRect / CreateDirectory などの補完と、ソケット定義を含む）
#include "../SboSvr/src/Platform/SvrCompat.h"
#include <netdb.h>
#endif

#include <map>
using namespace std;

#include "myString.h"
#include "myArray.h"
#include "GlobalDefine.h"
#include "SBOGlobal.h"

//{{AFX_INSERT_LOCATION}}
