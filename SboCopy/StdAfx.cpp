/// @file stdafx.cpp
/// @brief 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/03/09
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"

#ifdef _DEBUG
	#pragma comment(lib, "../myLib/debug/myLib.lib")
#else
	#pragma comment(lib, "../myLib/Release/myLib.lib")
#endif

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "shlwapi.lib")
