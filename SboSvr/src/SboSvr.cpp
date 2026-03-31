/// @file SboSvr.cpp
/// @brief サーバーメイン処理 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/04
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "MainFrame.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArgs, int nWinMode)
{
	int nRet;
	CMainFrame MainFrame;

	nRet = MainFrame.MainLoop(hInstance);

	return nRet;
}
