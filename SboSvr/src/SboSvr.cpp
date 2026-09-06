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

// エントリポイントの選択のみ #ifdef を許容する
//
// 言語仕様上、実行環境ごとのエントリ関数(WinMain / main)は
// プラットフォーム層の関数呼び出しに隠す方法が無いため、
// このファイルのこの1箇所に限り #ifdef _WIN32 を用いる。
// SboSvrMain() の中身には #ifdef を持ち込まない。
#ifdef _WIN32

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArgs, int nWinMode)
{
	return SboSvrMain(hInstance, __argc, __argv);
}

#else

int main(int nArgc, char **ppszArgv)
{
	// 非Windows にはウィンドウが無いため hInstance 相当は使われない
	// (MainLoopWindow 内でのウィンドウ生成にのみ使われる値のため NULL でよい)
	return SboSvrMain(NULL, nArgc, ppszArgv);
}

#endif
