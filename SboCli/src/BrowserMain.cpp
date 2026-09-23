/// @file BrowserMain.cpp
/// @brief ブラウザ向け起動エントリポイント
/// @date 2026/04/01
/// @copyright Copyright(C)URARA-works 2026

#include "StdAfx.h"
#include "MainFrame.h"

#if defined(__EMSCRIPTEN__)
#include <emscripten/em_js.h>
#include <emscripten/emscripten.h>
#include <sstream>
#include <string>
#include "SboCli_priv.h"
#include "MgrData.h"
#include "MgrWindow.h"
#include "Window/WindowBase.h"
#include "InfoCharCli.h"
#include "LibInfoCharCli.h"

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

namespace {

// SBOP2_DebugGetStateJson() が参照する、実行中の唯一の CMainFrame インスタンス。
// main() の static ローカル変数を指すだけで、所有権は持たない。
CMainFrame *g_pDebugMainFrame = NULL;

// UTF-8文字列を JSON 文字列リテラルとして安全な形にエスケープする
std::string DebugJsonEscape(LPCSTR pszUtf8)
{
	std::string strOut;
	if (pszUtf8 == NULL) {
		return strOut;
	}
	for (const unsigned char *p = (const unsigned char *)pszUtf8; *p != '\0'; ++p) {
		switch (*p) {
		case '"':  strOut += "\\\""; break;
		case '\\': strOut += "\\\\"; break;
		case '\n': strOut += "\\n"; break;
		case '\r': strOut += "\\r"; break;
		case '\t': strOut += "\\t"; break;
		default:
			if (*p < 0x20) {
				char szBuf[8];
				snprintf(szBuf, sizeof(szBuf), "\\u%04x", (unsigned int)*p);
				strOut += szBuf;
			} else {
				strOut += (char)*p;
			}
			break;
		}
	}
	return strOut;
}

// ゲーム状態を表す文字列名を取得する(デバッグ表示用)
const char *DebugGameStateName(int nState)
{
	switch (nState) {
	case GAMESTATE_NONE:		return "NONE";
	case GAMESTATE_LOGO:		return "LOGO";
	case GAMESTATE_TITLE:		return "TITLE";
	case GAMESTATE_LOGIN:		return "LOGIN";
	case GAMESTATE_DISCONNECT:	return "DISCONNECT";
	case GAMESTATE_INFO:		return "INFO";
	case GAMESTATE_LOGINMENU:	return "LOGINMENU";
	case GAMESTATE_MAP:		return "MAP";
	default:					return "UNKNOWN";
	}
}

// ウィンドウ種別ID(WINDOWTYPE_*)を表す文字列名を取得する(デバッグ表示用)
const char *DebugWindowTypeName(int nID)
{
	switch (nID) {
	case WINDOWTYPE_NONE:								return "NONE";
	case WINDOWTYPE_LOGIN:								return "LOGIN";
	case WINDOWTYPE_MSG:								return "MSG";
	case WINDOWTYPE_LOGINMENU:							return "LOGINMENU";
	case WINDOWTYPE_CHARNAME:							return "CHARNAME";
	case WINDOWTYPE_SEX:								return "SEX";
	case WINDOWTYPE_MAKECHAR:							return "MAKECHAR";
	case WINDOWTYPE_ACCOUNTINFO:						return "ACCOUNTINFO";
	case WINDOWTYPE_STATUS:							return "STATUS";
	case WINDOWTYPE_CHAT:								return "CHAT";
	case WINDOWTYPE_FAMILYTYPE:						return "FAMILYTYPE";
	case WINDOWTYPE_HAIRTYPE:							return "HAIRTYPE";
	case WINDOWTYPE_HAIRCOLOR:							return "HAIRCOLOR";
	case WINDOWTYPE_EYECOLOR:							return "EYECOLOR";
	case WINDOWTYPE_STYLESELECT:						return "STYLESELECT";
	case WINDOWTYPE_NAMEINPUT:							return "NAMEINPUT";
	case WINDOWTYPE_SYSTEMMENU:							return "SYSTEMMENU";
	case WINDOWTYPE_SETCOLOR:							return "SETCOLOR";
	case WINDOWTYPE_SETBGMVOLUME:						return "SETBGMVOLUME";
	case WINDOWTYPE_SETSOUNDVOLUME:						return "SETSOUNDVOLUME";
	case WINDOWTYPE_SETDRAWMODE:						return "SETDRAWMODE";
	case WINDOWTYPE_ITEMMENU:							return "ITEMMENU";
	case WINDOWTYPE_ITEMMENU_SELECT:					return "ITEMMENU_SELECT";
	case WINDOWTYPE_OPTION:								return "OPTION";
	case WINDOWTYPE_OPTION_VIEWSET:						return "OPTION_VIEWSET";
	case WINDOWTYPE_OPTION_VOLUMESET:					return "OPTION_VOLUMESET";
	case WINDOWTYPE_OPTION_INPUTSET:					return "OPTION_INPUTSET";
	case WINDOWTYPE_OPTION_TASKBAR:						return "OPTION_TASKBAR";
	case WINDOWTYPE_OPTION_INPUTSET_SETDEVICE:			return "OPTION_INPUTSET_SETDEVICE";
	case WINDOWTYPE_OPTION_ACTIONSET:					return "OPTION_ACTIONSET";
	case WINDOWTYPE_OPTION_ACTIONSET_SLEEPTIMER:		return "OPTION_ACTIONSET_SLEEPTIMER";
	case WINDOWTYPE_COMMANDMENU:						return "COMMANDMENU";
	case WINDOWTYPE_CHAR_STATUS:						return "CHAR_STATUS";
	case WINDOWTYPE_CHAR_STATUS2:						return "CHAR_STATUS2";
	case WINDOWTYPE_CHAR_STATUS3:						return "CHAR_STATUS3";
	case WINDOWTYPE_CHAR_STATUS4:						return "CHAR_STATUS4";
	case WINDOWTYPE_TEXTMSG:							return "TEXTMSG";
	case WINDOWTYPE_SWOON:								return "SWOON";
	case WINDOWTYPE_SKILLMENU:							return "SKILLMENU";
	case WINDOWTYPE_PLACEINFORMATION:					return "PLACEINFORMATION";
	case WINDOWTYPE_DELCONFIRM:							return "DELCONFIRM";
	default:											return "UNKNOWN";
	}
}

} // namespace

/// @brief デバッグ用: 現在のゲーム状態をJSON文字列(UTF-8)で取得する
/// @details window.sbop2Debug (?debug=1限定) から Module.ccall(..., 'string', 'string', []) で呼ぶ。
///          戻り値ポインタは次回呼び出しまで有効な静的バッファを指す。
extern "C" EMSCRIPTEN_KEEPALIVE const char *SBOP2_DebugGetStateJson(void)
{
	static std::string s_strJson;
	std::ostringstream oss;

	CMainFrame *pMainFrame = g_pDebugMainFrame;
	CMgrData *pMgrData = (pMainFrame != NULL) ? pMainFrame->GetMgrData() : NULL;
	int nGameState = (pMainFrame != NULL) ? pMainFrame->GetGameState() : GAMESTATE_NONE;
	CMgrWindow *pMgrWindow = (pMgrData != NULL) ? pMgrData->GetMgrWindow() : NULL;

	oss << "{";

	oss << "\"gameState\":" << nGameState << ",";
	oss << "\"gameStateName\":\"" << DebugGameStateName(nGameState) << "\",";

	// 開いているウィンドウの一覧
	oss << "\"windows\":[";
	if (pMgrWindow != NULL) {
		int nCount = pMgrWindow->GetWindowCount();
		for (int i = 0; i < nCount; ++i) {
			CWindowBase *pWindow = pMgrWindow->GetWindowAt(i);
			if (pWindow == NULL) {
				continue;
			}
			if (i > 0) {
				oss << ",";
			}
			oss << "{"
				<< "\"id\":" << pWindow->GetID() << ","
				<< "\"name\":\"" << DebugWindowTypeName(pWindow->GetID()) << "\","
				<< "\"active\":" << (pWindow->IsActive() ? "true" : "false") << ","
				<< "\"show\":" << (pWindow->IsShow() ? "true" : "false") << ","
				<< "\"pos\":" << pWindow->m_nPos
				<< "}";
		}
	}
	oss << "],";

	// キー入力を受け付けているウィンドウ(アクティブウィンドウ)
	CWindowBase *pActiveWindow = (pMgrWindow != NULL) ? pMgrWindow->GetActiveWindow() : NULL;
	if (pActiveWindow != NULL) {
		oss << "\"activeWindow\":{"
			<< "\"id\":" << pActiveWindow->GetID() << ","
			<< "\"name\":\"" << DebugWindowTypeName(pActiveWindow->GetID()) << "\""
			<< "},";
	} else {
		oss << "\"activeWindow\":null,";
	}
	oss << "\"keyInput\":" << ((pMgrWindow != NULL && pMgrWindow->IsKeyInput()) ? "true" : "false") << ",";

	// プレイヤーキャラ(マップにいる時のみ非null)
	CInfoCharCli *pPlayer = (pMgrData != NULL) ? pMgrData->GetPlayerChar() : NULL;
	if (pPlayer != NULL) {
		oss << "\"player\":{"
			<< "\"name\":\"" << DebugJsonEscape(pPlayer->m_strCharName.GetUtf8Pointer()) << "\","
			<< "\"mapID\":" << pPlayer->m_dwMapID << ","
			<< "\"x\":" << pPlayer->m_nMapX << ","
			<< "\"y\":" << pPlayer->m_nMapY << ","
			<< "\"direction\":" << pPlayer->m_nDirection
			<< "},";
	} else {
		oss << "\"player\":null,";
	}

	// 直近のシステムメッセージ(取れる範囲で末尾5件)
	oss << "\"systemMessages\":[";
	if (pMgrData != NULL) {
		int nMsgCount = pMgrData->GetSystemMsgCount();
		int nStart = (nMsgCount > 5) ? (nMsgCount - 5) : 0;
		bool bFirst = true;
		for (int i = nStart; i < nMsgCount; ++i) {
			PSTSYSTEMMSG pMsg = pMgrData->GetSystemMsg(i);
			if (pMsg == NULL) {
				continue;
			}
			if (!bFirst) {
				oss << ",";
			}
			bFirst = false;
			oss << "\"" << DebugJsonEscape(pMsg->strMsg.GetUtf8Pointer()) << "\"";
		}
	}
	oss << "]";

	oss << "}";

	s_strJson = oss.str();
	return s_strJson.c_str();
}

/// @brief デバッグ用: 自キャラ以外のキャラの移動同期状態をJSON配列(UTF-8)で取得する
/// @details 他プレイヤーの見え方(カクつき・後退)を毎フレーム計測するための軽量版。
///          window.sbop2Debug.others() から呼ぶ。戻り値は次回呼び出しまで有効な静的バッファ。
extern "C" EMSCRIPTEN_KEEPALIVE const char *SBOP2_DebugGetOthersJson(void)
{
	static std::string s_strJson;
	std::ostringstream oss;

	CMainFrame *pMainFrame = g_pDebugMainFrame;
	CMgrData *pMgrData = (pMainFrame != NULL) ? pMainFrame->GetMgrData() : NULL;
	CInfoCharCli *pPlayer = (pMgrData != NULL) ? pMgrData->GetPlayerChar() : NULL;
	CLibInfoCharCli *pLibInfoChar = (pMgrData != NULL) ? pMgrData->GetLibInfoChar() : NULL;

	oss << "{\"t\":" << SDL_GetTicks() << ",\"chars\":[";
	if (pLibInfoChar != NULL) {
		int nCount = pLibInfoChar->GetCount();
		bool bFirst = true;
		for (int i = 0; i < nCount; ++i) {
			CInfoCharCli *pChar = (CInfoCharCli *)pLibInfoChar->GetPtr(i);
			if ((pChar == NULL) || (pChar == pPlayer)) {
				continue;
			}
			POINT ptDraw;
			pChar->GetDrawMapPos(ptDraw);
			if (!bFirst) {
				oss << ",";
			}
			bFirst = false;
			oss << "{"
				<< "\"id\":" << pChar->m_dwCharID << ","
				<< "\"npc\":" << (pChar->IsNPC() ? 1 : 0) << ","
				<< "\"x\":" << pChar->m_nMapX << ","
				<< "\"y\":" << pChar->m_nMapY << ","
				<< "\"dx\":" << ptDraw.x << ","
				<< "\"dy\":" << ptDraw.y << ","
				<< "\"dir\":" << pChar->m_nDirection << ","
				<< "\"st\":" << pChar->m_nMoveState << ","
				<< "\"pred\":" << (pChar->m_bPredictedMove ? 1 : 0) << ","
				<< "\"sx\":" << pChar->m_nPredictSyncX << ","
				<< "\"sy\":" << pChar->m_nPredictSyncY << ","
				<< "\"rt\":" << pChar->m_dwPredictRecvTime << ","
				<< "\"spd\":" << pChar->m_nPredictSpeed
				<< "}";
		}
	}
	oss << "]}";

	s_strJson = oss.str();
	return s_strJson.c_str();
}

int main(int argc, char **argv)
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	static CMainFrame s_MainFrame;
	g_pDebugMainFrame = &s_MainFrame;
	return s_MainFrame.MainLoop(NULL);
}
#endif
