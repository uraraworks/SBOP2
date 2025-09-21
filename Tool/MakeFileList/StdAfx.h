/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:stdafx.h													 */
/* 内容			:プリコンパイルヘッダ 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/08/26													 */
/* ========================================================================= */

#pragma once

// no rpcsal fallback needed here

#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。
#define WINVER 0x0602
#define _WIN32_WINNT 0x0602

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>
#include <shlwapi.h>
#include <map>
using namespace std;

#include "GlobalDefine.h"
#include "SBOGlobal.h"
#include "myString.h"

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2007 */
