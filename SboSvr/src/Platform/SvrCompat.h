/// @file SvrCompat.h
/// @brief サーバー向け 非Windows 互換ヘッダ
/// @copyright Copyright(C)URARA-works
///
/// SboSvr/StdAfx.h が非Windows で読む側。Windows ヘッダの代わりに、
/// Common/Platform の共有互換レイヤと、サーバーが必要とする最小限の
/// 補完を提供する。
///
/// クライアント(SboCli)の BrowserCompat.h と役割は同じだが、
/// 描画・ウィンドウ系のスタブ(GdiStubs / WndProcCompat)は取り込まない。
/// サーバーはヘッドレスで動くため不要で、持ち込むと SDL 依存が付いてくる。

#pragma once

#if !defined(_WIN32)

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cstdarg>
#include <cwchar>
#include <string>
#include <vector>
#include <map>

// 共有の型定義と文字列クラス
#include "../../../Common/Platform/PlatformDefs.h"
#include "../../../Common/Platform/CStringCompat.h"
#include "../../../Common/Platform/TCharCompat.h"

// GetTickMs() を timeGetTime() の置き換えに使うため
#include "SvrPlatform.h"

// POSIX のソケット
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

// -----------------------------------------------------------------------
// ソケットまわりの名前合わせ
// -----------------------------------------------------------------------

typedef int SOCKET;

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

typedef struct sockaddr_in	SOCKADDR_IN;
typedef struct sockaddr		SOCKADDR;
typedef struct sockaddr		*LPSOCKADDR;
typedef struct in_addr		IN_ADDR;

inline int closesocket(SOCKET s)
{
	return close(s);
}

/// ioctlsocket(FIONBIO) 相当。非ブロッキングの設定だけ使う。
inline int ioctlsocket(SOCKET s, long cmd, unsigned long *argp)
{
	if ((cmd == FIONBIO) && (argp != NULL)) {
		int nFlags = fcntl(s, F_GETFL, 0);
		if (nFlags < 0) {
			return -1;
		}
		if (*argp) {
			nFlags |= O_NONBLOCK;
		} else {
			nFlags &= ~O_NONBLOCK;
		}
		return (fcntl(s, F_SETFL, nFlags) < 0) ? -1 : 0;
	}
	return -1;
}

inline int WSAGetLastError(void)
{
	return errno;
}

#ifndef WSAEWOULDBLOCK
#define WSAEWOULDBLOCK EWOULDBLOCK
#endif
#ifndef WSAEINTR
#define WSAEINTR EINTR
#endif
#ifndef SD_BOTH
#define SD_BOTH SHUT_RDWR
#endif
#ifndef SD_SEND
#define SD_SEND SHUT_WR
#endif

// -----------------------------------------------------------------------
// サーバーが使う細かい補完
// -----------------------------------------------------------------------

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef ZeroMemory
#define ZeroMemory(p, n)   std::memset((p), 0, (n))
#endif
#ifndef CopyMemory
#define CopyMemory(d, s, n) std::memcpy((d), (s), (n))
#endif

/// _TRUNCATE: 文字列切り詰め系 API(_snprintf_s 等)の第3引数として渡す値。
/// 実体は使われず(呼び出し先で無視される)、識別子として存在すればよい。
#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif

/// SetRect: RECT に値をセットする(Common 側で使用)
inline BOOL SetRect(LPRECT pRect, int left, int top, int right, int bottom)
{
	if (pRect == NULL) {
		return FALSE;
	}
	pRect->left = left;
	pRect->top = top;
	pRect->right = right;
	pRect->bottom = bottom;
	return TRUE;
}

/// timeGetTime: GetTickCount() 相当。サーバー側では SboPlatform::GetTickMs()
/// (起動からの単調増加ミリ秒)へ委譲する。
inline DWORD timeGetTime(void)
{
	return (DWORD)SboPlatform::GetTickMs();
}

/// IsDBCSLeadByte: 非Windows環境では常に FALSE を返す。
///
/// SBOP2 は通信・DB を UTF-8 へ完全移行済み(project_utf8_migration 参照)で、
/// CP932 前提の DBCS 判定は不要になっているため。
inline BOOL IsDBCSLeadByte(BYTE)
{
	return FALSE;
}

/// _vsnprintf: vsnprintf にマッピング
#ifndef _vsnprintf
#define _vsnprintf vsnprintf
#endif

/// _snprintf_s: ANSI(char)版の書式整形。第3引数(切り詰めサイズ指定)は
/// 無視し、常に vsnprintf 相当(バッファサイズで切り詰め)で動く。
inline int _snprintf_s(char *pszDst, size_t nDstCount, size_t /*nMaxCount*/, const char *pszFormat, ...)
{
	if ((pszDst == NULL) || (nDstCount == 0) || (pszFormat == NULL)) {
		return -1;
	}
	va_list args;
	va_start(args, pszFormat);
	int nRet = vsnprintf(pszDst, nDstCount, pszFormat, args);
	va_end(args);
	return nRet;
}

#endif // !_WIN32
