/// @file MainFrameRecvProcVERSION.cpp
/// @brief クライアントメインフレーム(バージョン系受信処理) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/05
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "Window/ILoginWindow.h"
#include "WindowLOGIN.h"
#include "MacAddr.h"
#include "MgrWindow.h"
#include "MainFrame.h"

#if defined(__EMSCRIPTEN__)
#include <emscripten/em_js.h>

namespace {

// ブラウザ版: localStorage の端末トークン(sbop2_device_token)を読み出す。
// 無ければ空文字を返す。戻り値は Emscripten ヒープ上の malloc バッファ(呼び出し元が free すること)。
// トークンの値は SDL_Log 等に出さないこと。
EM_JS(char *, SBOP2_ReadDeviceTokenFromBrowser, (), {
	var token = "";
	try {
		token = window.localStorage.getItem("sbop2_device_token") || "";
	} catch (e) {
		token = "";
	}
	var nLen = lengthBytesUTF8(token) + 1;
	var pBuf = _malloc(nLen);
	stringToUTF8(token, pBuf, nLen);
	return pBuf;
});

// ブラウザ版: 端末トークンが無いのでアカウントページへ移動する。
EM_JS(void, SBOP2_GoToAccountPage, (), {
	window.location.href = '/account/';
});

}
#endif // __EMSCRIPTEN__

void CMainFrame::RecvProcVERSION(BYTE byCmdSub, PBYTE pData)
{
	switch (byCmdSub) {
	case SBOCOMMANDID_SUB_VERSION_RES_VERSIONCHECK: RecvProcVERSION_RES_VERSIONCHECK(pData); break; // バージョンチェック応答
	case SBOCOMMANDID_SUB_VERSION_RES_FILELISTCHECK: RecvProcVERSION_RES_FILELISTCHECK(pData); break; // ファイルリストチェック応答
	}
}


void CMainFrame::RecvProcVERSION_RES_VERSIONCHECK(PBYTE pData)
{
	int nCount = 0;
	BYTE abyTmp[10];
	ILoginWindow *pWindow;
	CPacketVERSION_RES_VERSIONCHECK Packet;
	CPacketCONNECT_REQ_LOGIN PacketLOGIN;
	CMacAddr MacAddr;

	Packet.Set(pData);

	switch (Packet.m_nResult) {
	case VERSIONCHECKRES_OK: // 問題無し
		pWindow = m_pMgrWindow->GetLoginWindow();
		if (pWindow == NULL) {
			break;
		}
#if defined(__EMSCRIPTEN__)
		// ブラウザ版: ログインコード方式。端末トークンがあれば REQ_LOGIN_TOKEN を送る。
		// 無ければアカウントページへ移動する(旧 REQ_LOGIN は送らない)。
		{
			CPacketCONNECT_REQ_LOGIN_TOKEN PacketLOGIN_TOKEN;
			char *pszDeviceToken = SBOP2_ReadDeviceTokenFromBrowser();
			if ((pszDeviceToken != NULL) && (pszDeviceToken[0] != '\0')) {
				ZeroMemory(abyTmp, sizeof(abyTmp));
				PacketLOGIN_TOKEN.Make(pszDeviceToken, abyTmp);
				m_pSock->Send(&PacketLOGIN_TOKEN);
			} else {
				SBOP2_GoToAccountPage();
			}
			if (pszDeviceToken != NULL) {
				free(pszDeviceToken);
			}
		}
#else
		nCount = MacAddr.GetCount();
		for (int i = 0; i < nCount; i++) {
			ZeroMemory(abyTmp, sizeof(abyTmp));
			BOOL bResult = MacAddr.Get(abyTmp, i);
			if (bResult) {
				break;
			}
		}
		PacketLOGIN.Make(pWindow->GetAccount(), pWindow->GetPassword(), abyTmp);
		m_pSock->Send(&PacketLOGIN);
#endif
		break;

	case VERSIONCHECKRES_NG_VERSION: // バージョン不一致
		m_pSock->Destroy();
		DisConnectProc(DISCONNECTID_VERSION);
		break;
	}
}


void CMainFrame::RecvProcVERSION_RES_FILELISTCHECK(PBYTE pData)
{
	CPacketVERSION_RES_FILELISTCHECK Packet;

	Packet.Set(pData);

	switch (Packet.m_nResult) {
	case FILELISTCHECKRES_OK: // 問題無し
		break;
	case FILELISTCHECKRES_NG: // 不一致
		break;
	}
}
