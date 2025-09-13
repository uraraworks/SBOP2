/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:stdafx.h													 */
/* 内容			:プリコンパイルヘッダ 定義ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/01/27													 */
/* ========================================================================= */

#pragma once

#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。
#define WINVER 0x0602
#define _WIN32_WINNT 0x0602

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxdisp.h>        // MFC のオートメーション クラス
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxtempl.h>

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
#include "SBOGlobal.h"
#include "SboLaunch_priv.h"

//{{AFX_INSERT_LOCATION}}

/* Copyright(C)URARA-works 2008 */
