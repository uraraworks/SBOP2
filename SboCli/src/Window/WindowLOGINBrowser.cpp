/// @file WindowLOGINBrowser.cpp
/// @brief ログイン情報ウィンドウクラス（Emscripten DOM overlay 版）実装ファイル

#include "StdAfx.h"

#ifdef __EMSCRIPTEN__

#include "Img32.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "MgrData.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <emscripten/em_js.h>
#include <emscripten/emscripten.h>
#include "WindowLOGINBrowser.h"

namespace {

enum {
	LOGIN_TEXT_MAX = 255,
};

// ログインコード方式(docs/login-code-auth-plan.md)へ移行済み。
// ここで送る情報はウィンドウの表示位置・サイズ・有効無効のみで、
// ボタンの文言や表示切り替え(端末トークン有無)は JS 側 (/api/account/me の結果)が持つ。
static CWindowLOGINBrowser *g_pBrowserLoginWindow = NULL;

EM_JS(void, SBOP2_UpdateLoginDom, (int visible, int enabled,
	int windowX, int windowY, int windowW, int windowH, int hasToken), {
	if (typeof window.SBOP2UpdateLoginOverlay !== 'function') {
		return;
	}
	window.SBOP2UpdateLoginOverlay({
		visible: !!visible,
		enabled: !!enabled,
		windowX: windowX,
		windowY: windowY,
		windowW: windowW,
		windowH: windowH,
		hasToken: !!hasToken
	});
});

EM_JS(void, SBOP2_HideLoginDom, (), {
	if (typeof window.SBOP2HideLoginOverlay === 'function') {
		window.SBOP2HideLoginOverlay();
	}
});

extern "C" {
// 以下4つは旧ID/PW入力方式の名残(S4で削除予定)。DOM側の入力欄は非表示・フォーカス不可に
// しているため、通常は呼ばれない。
EMSCRIPTEN_KEEPALIVE void SBOP2_BrowserLoginSetAccount(const char *pszText)
{
	if (g_pBrowserLoginWindow != NULL) {
		g_pBrowserLoginWindow->SetAccountFromBrowser(pszText);
	}
}

EMSCRIPTEN_KEEPALIVE void SBOP2_BrowserLoginSetPassword(const char *pszText)
{
	if (g_pBrowserLoginWindow != NULL) {
		g_pBrowserLoginWindow->SetPasswordFromBrowser(pszText);
	}
}

EMSCRIPTEN_KEEPALIVE void SBOP2_BrowserLoginSetSavePassword(int bCheck)
{
	if (g_pBrowserLoginWindow != NULL) {
		g_pBrowserLoginWindow->SetSavePasswordFromBrowser(bCheck ? TRUE : FALSE);
	}
}

EMSCRIPTEN_KEEPALIVE void SBOP2_BrowserLoginSetFocus(int nFocusIndex)
{
	if (g_pBrowserLoginWindow != NULL) {
		g_pBrowserLoginWindow->SetFocusIndex(nFocusIndex);
	}
}

// タイトル画面の「(○○さんとして)はじめる」ボタンから呼ばれる。
EMSCRIPTEN_KEEPALIVE void SBOP2_BrowserLoginConnect(void)
{
	if (g_pBrowserLoginWindow != NULL) {
		g_pBrowserLoginWindow->SubmitFromBrowser();
	}
}

// JS 側が /api/account/me の結果(端末トークンが有効か)を通知する。
EMSCRIPTEN_KEEPALIVE void SBOP2_BrowserLoginSetHasToken(int bHasToken)
{
	if (g_pBrowserLoginWindow != NULL) {
		g_pBrowserLoginWindow->SetHasTokenFromBrowser(bHasToken ? TRUE : FALSE);
	}
}
}

static void NormalizeLoginText(CmyString &strDst, LPCSTR pszText, BOOL bLowercase, BOOL bAsciiOnly = FALSE)
{
	CString strWide;
	CString strFiltered;
	int nLength;

	if (pszText == NULL) {
		pszText = "";
	}

	strWide = Utf8ToTString(pszText);
	nLength = strWide.GetLength();
	for (int i = 0; i < nLength; ++i) {
		TCHAR ch = strWide[i];

		if (ch < _T(' ')) {
			continue;
		}
		if (bLowercase && (ch >= _T('A')) && (ch <= _T('Z'))) {
			ch = (TCHAR)(ch - _T('A') + _T('a'));
		}
		if (bAsciiOnly) {
			// パスワードは ASCII の表示可能文字のみ。サーバー側の
			// PasswordHash::IsAcceptable() と同じ条件。
			if ((ch < _T('!')) || (ch > _T('~'))) {
				continue;
			}
		}
		strFiltered.AppendChar(ch);
		if (strFiltered.GetLength() >= LOGIN_TEXT_MAX) {
			break;
		}
	}

	TrimViewString(strDst, (LPCTSTR)strFiltered);
}

// ウィンドウの見た目サイズ(canvas 側の枠)。端末トークンありは
// 「としてはじめる/別のアカウントで遊ぶ/アカウント管理」の3ボタン、
// 無しは「はじめる」1ボタン+説明文なので、ボタン数に応じて縦幅を変える。
enum {
	LOGIN_WINDOW_CX = 16 * 16,
	LOGIN_WINDOW_CY_NO_TOKEN = 16 * 7,
	LOGIN_WINDOW_CY_HAS_TOKEN = 16 * 10,
};

}

CWindowLOGINBrowser::CWindowLOGINBrowser()
{
	m_nID = WINDOWTYPE_LOGIN;
	m_sizeWindow.cx = LOGIN_WINDOW_CX;
	m_sizeWindow.cy = LOGIN_WINDOW_CY_NO_TOKEN;
	m_ptViewPos.x = SCRSIZEX / 2 - m_sizeWindow.cx / 2;
	m_ptViewPos.y = 280;
	m_bEnabled = TRUE;
	m_bSavePassword = FALSE;
	m_bHasToken = FALSE;
	m_nFocusIndex = LOGINFOCUS_ACCOUNT;
	m_bInput = TRUE;
}


CWindowLOGINBrowser::~CWindowLOGINBrowser()
{
	if (g_pBrowserLoginWindow == this) {
		g_pBrowserLoginWindow = NULL;
	}
	HideBrowserDom();
}


void CWindowLOGINBrowser::Create(CMgrData *pMgrData)
{
	CmyString strAccount, strPassword;

	CWindowBase::Create(pMgrData);

	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);
	strAccount = m_pMgrData->GetLastAccount();
	strPassword = m_pMgrData->GetLastPassword();
	m_strAccount = strAccount;
	m_strPassword = strPassword;
	MakeWindow();
}


void CWindowLOGINBrowser::Draw(PCImg32 pDst)
{
	if (m_dwTimeDrawStart == 0) {
		DrawFrame(0);
		m_dwTimeDrawStart = timeGetTime();
	}
	// 無効化中（接続要求の送信〜状態遷移までの間など）は DOM オーバーレイが隠れるため、
	// canvas 側の枠だけ残ると空っぽの背景に見える。canvas 描画もまとめてスキップする。
	if (m_bEnabled) {
		pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
	}

	g_pBrowserLoginWindow = this;
	UpdateBrowserDom();
}


void CWindowLOGINBrowser::Enable(BOOL bEnable)
{
	m_bEnabled = bEnable;
	if (!bEnable) {
		HideBrowserDom();
	}
	Redraw();
}


void CWindowLOGINBrowser::SetShow(BOOL bShow)
{
	CWindowBase::SetShow(bShow);
	if (!bShow) {
		HideBrowserDom();
	}
	Redraw();
}


void CWindowLOGINBrowser::Save(void)
{
	// ログインコード方式へ移行済み。パスワードは保存しない
	// (旧チェックボックスは非表示・入力不可のため、常に空にする)。
	m_pMgrData->SetLastAccount(m_strAccount);
	m_pMgrData->SetLastPassword("");
	m_pMgrData->SetSavePassword(FALSE);
	m_pMgrData->SaveIniData();
}


BOOL CWindowLOGINBrowser::TimerProc(void)
{
	CWindowBase::TimerProc();
	return (m_dwTimeDrawStart == 0) ? TRUE : FALSE;
}


void CWindowLOGINBrowser::SetFocusIndex(int nIndex)
{
	if (nIndex < 0) {
		nIndex = 0;
	}
	if (nIndex >= LOGINFOCUS_MAX) {
		nIndex = LOGINFOCUS_MAX - 1;
	}
	m_nFocusIndex = nIndex;
	Redraw();
}


void CWindowLOGINBrowser::UpdateBrowserDom(void)
{
	const int nWindowX = m_ptViewPos.x + 32;
	const int nWindowY = m_ptViewPos.y + 32;
	const BOOL bVisible = (m_bShow && m_bEnabled);

	SBOP2_UpdateLoginDom(
		(bVisible ? 1 : 0),
		m_bEnabled ? 1 : 0,
		nWindowX,
		nWindowY,
		m_sizeWindow.cx,
		m_sizeWindow.cy,
		m_bHasToken ? 1 : 0);
}


void CWindowLOGINBrowser::HideBrowserDom(void)
{
	SBOP2_HideLoginDom();
}


void CWindowLOGINBrowser::SetAccountFromBrowser(LPCSTR pszText)
{
	NormalizeLoginText(m_strAccount, pszText, TRUE);
	Redraw();
}


void CWindowLOGINBrowser::SetPasswordFromBrowser(LPCSTR pszText)
{
	NormalizeLoginText(m_strPassword, pszText, FALSE, TRUE);
	Redraw();
}


void CWindowLOGINBrowser::SetSavePasswordFromBrowser(BOOL bCheck)
{
	m_bSavePassword = bCheck;
	Redraw();
}


void CWindowLOGINBrowser::SubmitFromBrowser(void)
{
	OnConnect();
}


void CWindowLOGINBrowser::SetHasTokenFromBrowser(BOOL bHasToken)
{
	if (m_bHasToken == bHasToken) {
		return;
	}
	m_bHasToken = bHasToken;
	UpdateWindowSizeForToken();
	Redraw();
}


void CWindowLOGINBrowser::UpdateWindowSizeForToken(void)
{
	int cyNew, cyOld, nCenterY;

	cyNew = m_bHasToken ? LOGIN_WINDOW_CY_HAS_TOKEN : LOGIN_WINDOW_CY_NO_TOKEN;
	cyOld = m_sizeWindow.cy;
	if (cyOld == cyNew) {
		return;
	}
	// ボタン数の増減で縦幅が変わっても、見た目の中心が動かないように位置を調整する。
	nCenterY = m_ptViewPos.y + cyOld / 2;
	m_sizeWindow.cy = cyNew;
	m_ptViewPos.y = nCenterY - cyNew / 2;
	if (m_pDib != NULL) {
		m_pDib->Destroy();
		m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
		m_pDib->SetColorKey(0);
	}
	// 枠を描き直す
	m_dwTimeDrawStart = 0;
}


void CWindowLOGINBrowser::MakeWindow(void)
{
	BOOL bSave;
	bSave = m_pMgrData->GetSavePassword();
	m_bSavePassword = bSave;

	if (!bSave) {
		m_strPassword.Empty();
	}
	SetFocusIndex(LOGINFOCUS_ACCOUNT);
}


void CWindowLOGINBrowser::OnConnect(void)
{
	// ログインコード方式(docs/login-code-auth-plan.md): 端末トークンが有効なときだけ接続する。
	// 旧「アカウント名が空なら何もしない」判定の置き換え。
	if (!m_bHasToken) {
		return;
	}

	m_pMgrSound->PlaySound(SOUNDID_OK_PI73);
	Enable(FALSE);
	m_pMgrData->PostMainFrameMessage(MAINFRAMEMSG_CONNECT, 0);
}

#endif // __EMSCRIPTEN__
