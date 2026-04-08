/// @file SboCli.cpp
/// @brief アプリケーション エントリポイント
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/09/24
/// @copyright Copyright(C)URARA-works 2006

// SDL_main マクロを無効化
#define SDL_MAIN_HANDLED

#include "stdafx.h"
#include "MainFrame.h"

#if defined(_WIN32)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	WSADATA wsaData;
	HANDLE hMutexMain;

	// winSockを初期化
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// 多重起動防止
	hMutexMain = CreateMutex(NULL, TRUE, _T("SboCli"));
	if ((hMutexMain == NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
		if (hMutexMain) {
			ReleaseMutex(hMutexMain);
			CloseHandle(hMutexMain);
		}
		return 0;
	}

	{
		CMainFrame MainFrame;
		MainFrame.MainLoop(hInstance);
	}

	ReleaseMutex(hMutexMain);
	CloseHandle(hMutexMain);
	WSACleanup();

	// SDLループ終了後、ソケットスレッド等が残存している場合でも確実に終了
	ExitProcess(0);
	return 0;
}

#endif
