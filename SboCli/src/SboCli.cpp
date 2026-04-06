/// @file SboCli.cpp
/// @brief アプリケーションクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/09/24
/// @copyright Copyright(C)URARA-works 2006

// SDL_main マクロを無効化（MFCのエントリポイントと競合させない）
#define SDL_MAIN_HANDLED

#include "stdafx.h"
#include "MainFrame.h"
#include "SboCli.h"

#if defined(_WIN32)
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

BEGIN_MESSAGE_MAP(CSboCliApp, CWinApp)
	//{{AFX_MSG_MAP(CSboCliApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

// 唯一の CSboCliApp オブジェクト
CSboCliApp theApp;


CSboCliApp::CSboCliApp()
{
}


BOOL CSboCliApp::InitInstance()
{
	BOOL bRet;
	WSADATA wsaData;
	CMainFrame MainFrame;
	HANDLE hMutexMain;

	bRet = FALSE;
	hMutexMain = NULL;

	// winSockを初期化
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	CWinApp::InitInstance();
	AfxEnableControlContainer();

#if 1
	hMutexMain = CreateMutex(NULL, TRUE, _T("SboCli"));
	if ((hMutexMain == NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
		goto Exit;
	}
#endif

	MainFrame.MainLoop(GetModuleHandle(NULL));

	bRet = TRUE;
Exit:
	ReleaseMutex(hMutexMain);
	CloseHandle(hMutexMain);

	// SDLループ終了後、ソケットスレッド等が残存している場合でも確実に終了
	ExitProcess(0);
	return FALSE;
}
#endif
