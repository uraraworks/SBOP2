/// @file WindowLOGINBrowser.cpp
/// @brief ログイン情報ウィンドウクラス（Emscripten DOM overlay 版）実装ファイル

#include "stdafx.h"

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

static CWindowLOGINBrowser *g_pBrowserLoginWindow = NULL;

EM_JS(void, SBOP2_UpdateLoginDom, (int visible, int enabled,
	int windowX, int windowY, int windowW, int windowH,
	int accountX, int accountY, int accountW, int accountH,
	int passwordX, int passwordY, int passwordW, int passwordH,
	int checkX, int checkY, int checkW, int checkH,
	int buttonX, int buttonY, int buttonW, int buttonH,
	const char *pszAccount, const char *pszPassword, int bSavePassword, int nFocusIndex), {
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
		accountX: accountX,
		accountY: accountY,
		accountW: accountW,
		accountH: accountH,
		passwordX: passwordX,
		passwordY: passwordY,
		passwordW: passwordW,
		passwordH: passwordH,
		checkX: checkX,
		checkY: checkY,
		checkW: checkW,
		checkH: checkH,
		buttonX: buttonX,
		buttonY: buttonY,
		buttonW: buttonW,
		buttonH: buttonH,
		account: UTF8ToString(pszAccount),
		password: UTF8ToString(pszPassword),
		savePassword: !!bSavePassword,
		focusIndex: nFocusIndex
	});
});

EM_JS(void, SBOP2_HideLoginDom, (), {
	if (typeof window.SBOP2HideLoginOverlay === 'function') {
		window.SBOP2HideLoginOverlay();
	}
});

extern "C" {
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

EMSCRIPTEN_KEEPALIVE void SBOP2_BrowserLoginConnect(void)
{
	if (g_pBrowserLoginWindow != NULL) {
		g_pBrowserLoginWindow->SubmitFromBrowser();
	}
}
}

static void GetWindowLOGINAccountRect(RECT *pRect)
{
	SetRect(pRect, 16 * 8, 16, 16 * 8 + 8 * 12, 16 + 14);
}

static void GetWindowLOGINPasswordRect(RECT *pRect)
{
	SetRect(pRect, 16 * 8, 42, 16 * 8 + 8 * 12, 42 + 14);
}

static void GetWindowLOGINCheckRect(RECT *pRect)
{
	SetRect(pRect, 16, 16 * 4 + 6, 16 + 6 * 23, 16 * 4 + 6 + 14);
}

static void GetWindowLOGINConnectRect(RECT *pRect)
{
	SetRect(pRect, 16 * 15 - 54 - 16, 16 * 5 + 6, 16 * 15 - 16, 16 * 5 + 6 + 18);
}

static void NormalizeLoginText(CmyString &strDst, LPCSTR pszText, BOOL bLowercase)
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
		strFiltered.AppendChar(ch);
		if (strFiltered.GetLength() >= LOGIN_TEXT_MAX) {
			break;
		}
	}

	TrimViewString(strDst, (LPCTSTR)strFiltered);
}

}

CWindowLOGINBrowser::CWindowLOGINBrowser()
{
	m_nID = WINDOWTYPE_LOGIN;
	m_sizeWindow.cx = 16 * 15;
	m_sizeWindow.cy = 16 * 7;
	m_ptViewPos.x = SCRSIZEX / 2 - m_sizeWindow.cx / 2;
	m_ptViewPos.y = 280;
	m_bEnabled = TRUE;
	m_bSavePassword = FALSE;
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

	strAccount = m_pMgrData->GetLastAccount();
	strPassword = m_pMgrData->GetLastPassword();
	m_strAccount = strAccount;
	m_strPassword = strPassword;
	MakeWindow();
}


void CWindowLOGINBrowser::Draw(PCImg32 pDst)
{
	RECT rcAccount, rcPassword, rcCheck, rcConnect;

	GetWindowLOGINAccountRect(&rcAccount);
	GetWindowLOGINPasswordRect(&rcPassword);
	GetWindowLOGINCheckRect(&rcCheck);
	GetWindowLOGINConnectRect(&rcConnect);

	g_pBrowserLoginWindow = this;
	UpdateBrowserDom(rcAccount, rcPassword, rcCheck, rcConnect);
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
	BOOL bSave;

	m_pMgrData->SetLastAccount(m_strAccount);
	m_pMgrData->SetLastPassword("");

	bSave = m_bSavePassword;
	m_pMgrData->SetSavePassword(bSave);
	if (bSave) {
		m_pMgrData->SetLastPassword(m_strPassword);
	}
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


void CWindowLOGINBrowser::UpdateBrowserDom(const RECT &rcAccount, const RECT &rcPassword, const RECT &rcCheck, const RECT &rcConnect)
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
		nWindowX + rcAccount.left,
		nWindowY + rcAccount.top,
		rcAccount.right - rcAccount.left,
		rcAccount.bottom - rcAccount.top,
		nWindowX + rcPassword.left,
		nWindowY + rcPassword.top,
		rcPassword.right - rcPassword.left,
		rcPassword.bottom - rcPassword.top,
		nWindowX + rcCheck.left,
		nWindowY + rcCheck.top,
		rcCheck.right - rcCheck.left,
		rcCheck.bottom - rcCheck.top,
		nWindowX + rcConnect.left,
		nWindowY + rcConnect.top,
		rcConnect.right - rcConnect.left,
		rcConnect.bottom - rcConnect.top,
		m_strAccount,
		m_strPassword,
		m_bSavePassword ? 1 : 0,
		m_nFocusIndex);
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
	NormalizeLoginText(m_strPassword, pszText, FALSE);
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


void CWindowLOGINBrowser::MakeWindow(void)
{
	BOOL bSave;
	bSave = m_pMgrData->GetSavePassword();
	m_bSavePassword = bSave;

	if (!bSave) {
		m_strPassword.Empty();
	}
	if (m_strAccount.IsEmpty()) {
		SetFocusIndex(LOGINFOCUS_ACCOUNT);
	} else if (m_strPassword.IsEmpty()) {
		SetFocusIndex(LOGINFOCUS_PASSWORD);
	} else {
		SetFocusIndex(LOGINFOCUS_CONNECT);
	}
}


void CWindowLOGINBrowser::OnConnect(void)
{
	if (m_strAccount.IsEmpty()) {
		return;
	}

	m_pMgrSound->PlaySound(SOUNDID_OK_PI73);
	Enable(FALSE);
	m_pMgrData->PostMainFrameMessage(MAINFRAMEMSG_CONNECT, 0);
}

#endif // __EMSCRIPTEN__
