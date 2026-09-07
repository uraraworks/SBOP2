/// @file StdAfx.h
/// @brief プリコンパイルヘッダ 定義ファイル
/// @copyright Copyright(C)URARA-works

#pragma once

#include "../Common/rpcsal_fallback.h"

#define WIN32_LEAN_AND_MEAN

#include <sdkddkver.h>
#include <rpc.h>
#include <rpcndr.h>
#include <rpcsal.h>
#include <sal.h>

#include <winsock2.h>
#include <windows.h>
#include <tchar.h>

// GlobalDefine.h が map を前提にしているため、SboSvr と同じ形で用意する
#include <map>
using namespace std;
