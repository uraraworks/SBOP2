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
#include <cwchar>
#include <string>
#include <vector>
#include <map>

// 共有の型定義と文字列クラス
#include "../../../Common/Platform/PlatformDefs.h"
#include "../../../Common/Platform/CStringCompat.h"
#include "../../../Common/Platform/TCharCompat.h"

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

#endif // !_WIN32
