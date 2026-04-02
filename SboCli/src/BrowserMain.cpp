/// @file BrowserMain.cpp
/// @brief ブラウザ向け起動エントリポイント
/// @date 2026/04/01
/// @copyright Copyright(C)URARA-works 2026

#include "stdafx.h"
#include "MainFrame.h"

#if defined(__EMSCRIPTEN__)
int main(int argc, char **argv)
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	static CMainFrame s_MainFrame;
	return s_MainFrame.MainLoop(NULL);
}
#endif
