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

// Common/Packet 配下のパケットクラス(PacketBase.cpp 等)や SBOGlobal.cpp が
// SAFE_DELETE_ARRAY マクロ・CmyString・CopyMemoryRenew 等を前提にしているため、
// SboSvr の StdAfx.h と同じ並びでここに読み込んでおく(このプロジェクトはPCH
// 無効のため、単なるヘッダインクルードとして働く)。
#include "GlobalDefine.h"
#include "myString.h"
#include "SBOGlobal.h"
