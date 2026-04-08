/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:stdafx.h													 */
/* 内容			:プリコンパイルヘッダ 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/09/24													 */
/* ========================================================================= */

#pragma once

#if !defined(_WIN32)

#include "../Common/rpcsal_fallback.h"
#include "src/Platform/BrowserCompat.h"
#include "GlobalDefine.h"
#include "myArray.h"
#include "myString.h"
#include "mySection.h"
#include "SBOGlobal.h"
#include "SboCli_priv.h"

#else

#include <winsock2.h>
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif
#include "../Common/rpcsal_fallback.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef WINVER
#define WINVER 0x0A00
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#include <sdkddkver.h>
#include <rpc.h>
#include <rpcndr.h>
#include <rpcsal.h>
#include <sal.h>

// RPC SAL macros のフォールバック定義
#ifndef __RPC__in_xcount_full
#define __RPC__in_xcount_full(_Size)
#endif
#ifndef __RPC__out_xcount_part
#define __RPC__out_xcount_part(_Size,_Length)
#endif
#ifndef __RPC__in_ecount_full
#define __RPC__in_ecount_full(_Count)
#endif
#ifndef __RPC__out_ecount_part
#define __RPC__out_ecount_part(_Count,_Length)
#endif

#include <SDL.h>
#include <windows.h>
// timeGetTime を SDL_GetTicks にマッピング（winmm.lib 依存を除去するため）
// inline 関数ではなくマクロにすることで dllimport 宣言との競合を回避する
#undef timeGetTime
#define timeGetTime() ((DWORD)SDL_GetTicks())
#include <tchar.h>
#include <shlwapi.h>
#include <map>
using namespace std;

#define DIRECTINPUT_VERSION	DIRECTINPUT_HEADER_VERSION

// CRect クラス（MFC除去後の互換定義）
struct CRect : public RECT {
    CRect() { left = top = right = bottom = 0; }
    CRect(LONG l, LONG t, LONG r, LONG b) { left = l; top = t; right = r; bottom = b; }
    CRect(const RECT &rc) { left = rc.left; top = rc.top; right = rc.right; bottom = rc.bottom; }
    LONG Width() const { return right - left; }
    LONG Height() const { return bottom - top; }
    bool IsRectEmpty() const { return (right <= left) || (bottom <= top); }
    void SetRect(LONG l, LONG t, LONG r, LONG b) { left = l; top = t; right = r; bottom = b; }
    void SetRectEmpty() { left = top = right = bottom = 0; }
    bool PtInRect(POINT pt) const {
        return (pt.x >= left) && (pt.x < right) && (pt.y >= top) && (pt.y < bottom);
    }
    CRect& operator=(const RECT &rc) {
        left = rc.left; top = rc.top; right = rc.right; bottom = rc.bottom; return *this;
    }
};

#include "GlobalDefine.h"
#include "myArray.h"
#include "myString.h"
#include "mySection.h"
#include "SBOGlobal.h"
#include "SboCli_priv.h"

#endif

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2006 */
