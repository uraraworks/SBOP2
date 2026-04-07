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
#else
// POSIX / Emscripten
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <cstdint>
#include <string>
#include <vector>
#include <cwchar>
#include <cstdlib>
#include <cstdio>

// SDL2（PlatformDefs.h の CRITICAL_SECTION で必要）
#include <SDL2/SDL.h>

// プラットフォーム共通定義
#include "PlatformDefs.h"
#include "CStringCompat.h"
#include "TCharCompat.h"

// ソケット関連定義（PlatformDefs.h に無いもの）
typedef int SOCKET;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#define SD_BOTH SHUT_RDWR
#define closesocket(s) close(s)

#endif

#include <map>
using namespace std;

#include "myString.h"
#include "myArray.h"
#include "GlobalDefine.h"
#include "SBOGlobal.h"

//{{AFX_INSERT_LOCATION}}
