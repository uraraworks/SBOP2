/// @file BrowserMain.cpp
/// @brief ブラウザ向け起動エントリポイント
/// @date 2026/04/01
/// @copyright Copyright(C)URARA-works 2026

#include "stdafx.h"
#include "MainFrame.h"

#if defined(__EMSCRIPTEN__)
#include <emscripten/em_js.h>

// URL クエリ ?server=host:port で指定されたサーバーアドレスを取得する
// 戻り値: 取得成功=1, 未設定=0
EM_JS(int, SBOP2_GetBrowserServerAddr, (char *pBuf, int nBufSize), {
    if (typeof window.sbop2ServerAddr === 'string' && window.sbop2ServerAddr.length > 0) {
        stringToUTF8(window.sbop2ServerAddr, pBuf, nBufSize);
        return 1;
    }
    return 0;
});

// URL クエリ ?server=host:port で指定されたポート番号を取得する
// 戻り値: ポート番号 (>0), 未設定=0
EM_JS(int, SBOP2_GetBrowserServerPort, (), {
    if (typeof window.sbop2ServerPort === 'number' && window.sbop2ServerPort > 0) {
        return window.sbop2ServerPort | 0;
    }
    return 0;
});

int main(int argc, char **argv)
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	static CMainFrame s_MainFrame;
	return s_MainFrame.MainLoop(NULL);
}
#endif
