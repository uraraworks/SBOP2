/// @file stdafx.h
/// @brief プリコンパイルヘッダ 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/04
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "../Common/rpcsal_fallback.h"

#define WIN32_LEAN_AND_MEAN

// Ensure SAL/RPC SAL macros are available for Windows headers
#include <sdkddkver.h>
#include <rpc.h>
#include <rpcndr.h>
#include <rpcsal.h>
#include <sal.h>
		// Windows ヘッダーから殆ど使用されないスタッフを除外します

#include <winsock2.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <shlwapi.h>
#include <tchar.h>
#include <map>
using namespace std;

#include "GlobalDefine.h"
#include "myArray.h"
#include "myString.h"
#include "mySection.h"
#include "SBOGlobal.h"
#include "SboSvr_priv.h"

//{{AFX_INSERT_LOCATION}}
