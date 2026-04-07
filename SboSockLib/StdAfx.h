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
// TCHAR = wchar_t として定義しているので _UNICODE / UNICODE を定義
#if !defined(_UNICODE)
#define _UNICODE
#endif
#if !defined(UNICODE)
#define UNICODE
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <cstdint>

// Windows互換型定義
typedef int SOCKET;
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef int BOOL;
typedef int INT;
typedef unsigned int UINT;
typedef void* HANDLE;
typedef void* HWND;
typedef void* PVOID;
typedef void* LPVOID;
typedef const char* LPCSTR;
typedef char* LPSTR;
typedef BYTE* PBYTE;
typedef DWORD* PDWORD;
typedef const wchar_t* LPCTSTR;
typedef wchar_t* LPTSTR;
typedef wchar_t TCHAR;
typedef intptr_t WPARAM;
typedef intptr_t LPARAM;

#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#define SD_BOTH SHUT_RDWR
#define TRUE 1
#define FALSE 0
#define INFINITE 0xFFFFFFFF
#define ZeroMemory(p,n) memset((p),0,(n))
#define closesocket(s) close(s)
#define _T(x) L##x
#define TEXT(x) L##x
#define MAKEWORD(a,b) (((WORD)(a)) | (((WORD)(b)) << 8))
#define MAKELONG(a,b) ((LONG)(((WORD)(a)) | (((DWORD)((WORD)(b))) << 16)))
typedef long LONG;
typedef long long LONG_PTR;

#define WM_APP 0x8000
#define WM_CLOSE 0x0010
#endif

#include <map>
using namespace std;

#include "myString.h"
#include "myArray.h"
#include "GlobalDefine.h"
#include "SBOGlobal.h"

//{{AFX_INSERT_LOCATION}}
