/// @file SboSvr.cpp
/// @brief サーバーメイン処理 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/04
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "MainFrame.h"

// 起動引数を照合する
//
// --xxx / -xxx / /xxx のいずれも受け付ける。

static BOOL IsArg(LPCSTR pszArg, LPCSTR pszName)
{
	char szTmp[64];

	if (pszArg == NULL) {
		return FALSE;
	}

	_snprintf_s(szTmp, sizeof (szTmp), _TRUNCATE, "--%s", pszName);
	if (_stricmp(pszArg, szTmp) == 0) {
		return TRUE;
	}
	_snprintf_s(szTmp, sizeof (szTmp), _TRUNCATE, "-%s", pszName);
	if (_stricmp(pszArg, szTmp) == 0) {
		return TRUE;
	}
	_snprintf_s(szTmp, sizeof (szTmp), _TRUNCATE, "/%s", pszName);
	if (_stricmp(pszArg, szTmp) == 0) {
		return TRUE;
	}
	return FALSE;
}

static BOOL HasArg(int nArgc, char **ppszArgv, LPCSTR pszName)
{
	int i;

	for (i = 1; i < nArgc; i ++) {
		if (IsArg(ppszArgv[i], pszName)) {
			return TRUE;
		}
	}
	return FALSE;
}

// 移植しやすいエントリポイント
//
// 非Windows へ移す際はここを main() から呼べばよい。

static int SboSvrMain(HINSTANCE hInstance, int nArgc, char **ppszArgv)
{
	// 稼働中のヘッドレスサーバーを停止させる
	if (HasArg(nArgc, ppszArgv, "stop")) {
		return (CMainFrame::RequestStopRunningServer() != FALSE)
				? SBOSVR_EXIT_OK : SBOSVR_EXIT_ERROR;
	}

	{
		CMainFrame MainFrame;

		return MainFrame.MainLoop(hInstance, HasArg(nArgc, ppszArgv, "headless"));
	}
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArgs, int nWinMode)
{
	return SboSvrMain(hInstance, __argc, __argv);
}
