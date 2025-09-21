/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:stdafx.h													 */
/* 内容			:プリコンパイルヘッダ 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/09/24													 */
/* ========================================================================= */

#pragma once

#include <winsock2.h>
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif
#include "../Common/rpcsal_fallback.h"

#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef WINVER
#define WINVER 0x0A00
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif
// Ensure SAL/RPC SAL macros are available for Windows headers
#include <sdkddkver.h>
#include <rpc.h>
#include <rpcndr.h>
#include <rpcsal.h>
#include <sal.h>

// Fallback: if some toolchains pull older headers (e.g., DXSDK)
// and RPC SAL macros are missing, define them as no-ops so
// Windows SDK headers like objidlbase.h compile.
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
#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxdisp.h>        // MFC のオートメーション クラス
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxtempl.h>

//#pragma warning(disable:4192)
//#pragma warning(disable:4146)
//#pragma warning(disable:4049)

#define DIRECTINPUT_VERSION	DIRECTINPUT_HEADER_VERSION

#include <winsock2.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <imm.h>
#include <time.h>
#include <tchar.h>
#include <shlwapi.h>
#include <map>
using namespace std;

#include "GlobalDefine.h"
#include "myArray.h"
#include "myString.h"
#include "mySection.h"
#include "SBOGlobal.h"
#include "SboCli_priv.h"

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2006 */
