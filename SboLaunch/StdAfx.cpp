/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:stdafx.cpp													 */
/* 内容			:実装ファイル												 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/01/27													 */
/* ========================================================================= */

#include "stdafx.h"

#ifdef _DEBUG
	#pragma comment(lib, "../Common/myLib/myZlib/zlibD.lib")
	#pragma comment(lib, "../myLib/debug/myLib.lib")
	#pragma comment(lib, "../SboSockLib/Debug/SboSockLib.lib")
#else
	#pragma comment(lib, "../Common/myLib/myZlib/zlib.lib")
	#pragma comment(lib, "../myLib/Release/myLib.lib")
	#pragma comment(lib, "../SboSockLib/Release/SboSockLib.lib")
#endif

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "shlwapi.lib")

/* Copyright(C)URARA-works 2008 */
