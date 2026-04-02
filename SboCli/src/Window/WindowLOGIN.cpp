/// @file WindowLOGIN.cpp
/// @brief ログイン情報ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/03
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "Img32.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#if defined(__EMSCRIPTEN__)
#include <emscripten/em_js.h>
#include <emscripten/emscripten.h>
#endif
#include "WindowLOGIN.h"

namespace {

enum {
	LOGIN_TEXT_MAX = 255,
};

#if defined(__EMSCRIPTEN__)
static CWindowLOGIN *g_pBrowserLoginWindow = NULL;

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
#endif

static LPCTSTR GetLoginLabelAccount(void)
{
	return _T("\u30a2\u30ab\u30a6\u30f3\u30c8");
}

static LPCTSTR GetLoginLabelPassword(void)
{
	return _T("\u30d1\u30b9\u30ef\u30fc\u30c9");
}

static LPCTSTR GetLoginLabelSavePassword(void)
{
	return _T("\u30d1\u30b9\u30ef\u30fc\u30c9\u3092\u8a18\u9332\u3059\u308b");
}

static LPCTSTR GetLoginLabelConnect(void)
{
	return _T("\u63a5\u7d9a");
}

static BOOL IsShiftPressed(void)
{
	SDL_Keymod keymod;

	keymod = SDL_GetModState();
	return (keymod & KMOD_SHIFT) ? TRUE : FALSE;
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
	SetRect(pRect, 16 * 11 - 2, 16 * 4 + 2, 16 * 11 - 2 + 54, 16 * 4 + 2 + 18);
}

static void GetWindowLOGINCheckBoxRect(const RECT &rcCheck, RECT *pRect)
{
	SetRect(pRect, rcCheck.left, rcCheck.top + 1, rcCheck.left + 12, rcCheck.top + 13);
}

static void OffsetWindowLOGINRect(const CWindowLOGIN *pWindow, RECT *pRect)
{
	const int nOffsetX = pWindow->m_ptViewPos.x + 32;
	const int nOffsetY = pWindow->m_ptViewPos.y + 32;

	pRect->left += nOffsetX;
	pRect->right += nOffsetX;
	pRect->top += nOffsetY;
	pRect->bottom += nOffsetY;
}

}

CWindowLOGIN::CWindowLOGIN()
{
	m_nID	= WINDOWTYPE_LOGIN;
	m_sizeWindow.cx	= 16 * 15;
	m_sizeWindow.cy	= 16 * 6;
	m_ptViewPos.x	= SCRSIZEX / 2 - m_sizeWindow.cx / 2;
	m_ptViewPos.y	= 280;
#if defined(__EMSCRIPTEN__)
	m_bEnabled = TRUE;
	m_bSavePassword = FALSE;
	m_nFocusIndex = LOGINFOCUS_ACCOUNT;
	m_bInput = TRUE;
#endif
	m_hWndAccount	= NULL;
	m_hWndPassword	= NULL;
	m_hWndSavePassword	= NULL;
	m_hWndConnect	= NULL;
	m_OrgWndProcAccount	= NULL;
	m_OrgWndProcPassword	= NULL;
	m_OrgWndProcSavePassword	= NULL;
	m_OrgWndProcConnect	= NULL;
}


CWindowLOGIN::~CWindowLOGIN()
{
#if !defined(__EMSCRIPTEN__)
	if (m_hWndAccount) {
		SetWindowLong(m_hWndAccount, GWL_WNDPROC, (LONG)m_OrgWndProcAccount);
		m_OrgWndProcAccount = NULL;
		SAFE_DESTROYWND(m_hWndAccount);
	}
	if (m_hWndPassword) {
		SetWindowLong(m_hWndPassword, GWL_WNDPROC, (LONG)m_OrgWndProcPassword);
		m_OrgWndProcPassword = NULL;
		SAFE_DESTROYWND(m_hWndPassword);
	}
	if (m_hWndSavePassword) {
		SetWindowLong(m_hWndSavePassword, GWL_WNDPROC, (LONG)m_OrgWndProcSavePassword);
		m_OrgWndProcPassword = NULL;
		SAFE_DESTROYWND(m_hWndSavePassword);
	}
	if (m_hWndConnect) {
		SetWindowLong(m_hWndConnect, GWL_WNDPROC, (LONG)m_OrgWndProcConnect);
		m_OrgWndProcConnect = NULL;
		SAFE_DESTROYWND(m_hWndConnect);
	}
#else
	if (g_pBrowserLoginWindow == this) {
		g_pBrowserLoginWindow = NULL;
	}
	HideBrowserDom();
	UpdateSDLTextInput();
#endif
}


void CWindowLOGIN::Create(CMgrData *pMgrData)
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


void CWindowLOGIN::Draw(PCImg32 pDst)
{
	HDC hDC = NULL;
	HFONT hFontOld = NULL;
	RECT rcAccount, rcPassword, rcCheck, rcConnect;
	CString strCheck;

	GetWindowLOGINAccountRect(&rcAccount);
	GetWindowLOGINPasswordRect(&rcPassword);
	GetWindowLOGINCheckRect(&rcCheck);
	GetWindowLOGINConnectRect(&rcConnect);
	strCheck = m_bSavePassword ? _T("[x] ") : _T("[ ] ");
	strCheck += GetLoginLabelSavePassword();

	if (m_dwTimeDrawStart == 0) {
		DrawFrame();

#if !defined(__EMSCRIPTEN__)
	hDC	= m_pDib->Lock();
	hFontOld	= (HFONT)SelectObject(hDC, m_hFont);
	SetBkMode(hDC, TRANSPARENT);

	TextOut2(hDC, 16, 16, _T("アカウント:"), RGB(1, 1, 1));
	TextOut2(hDC, 16, 42, _T("パスワード:"), RGB(1, 1, 1));
	DrawInputFrame1(16 * 8, 16, 8 * 12, 14, 0);
	DrawInputFrame1(16 * 8, 42, 8 * 12, 14, 0);

	SelectObject(hDC, hFontOld);
	m_pDib->Unlock();
#endif
		m_dwTimeDrawStart = timeGetTime();
	}

Exit:
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);

#if defined(__EMSCRIPTEN__)
	g_pBrowserLoginWindow = this;
	UpdateBrowserDom(rcAccount, rcPassword, rcCheck, rcConnect);
#endif
}


void CWindowLOGIN::Enable(BOOL bEnable)
{
#if defined(__EMSCRIPTEN__)
	m_bEnabled = bEnable;
	UpdateSDLTextInput();
	if (!bEnable) {
		HideBrowserDom();
	}
	Redraw();
#else
	EnableWindow(m_hWndAccount,	bEnable);
	EnableWindow(m_hWndPassword,	bEnable);
	EnableWindow(m_hWndSavePassword,	bEnable);
	EnableWindow(m_hWndConnect,	bEnable);
#endif
}


void CWindowLOGIN::SetShow(BOOL bShow)
{
#if !defined(__EMSCRIPTEN__)
	int nCmdShow;
#endif

	CWindowBase::SetShow(bShow);

#if defined(__EMSCRIPTEN__)
	UpdateSDLTextInput();
	if (!bShow) {
		HideBrowserDom();
	}
	Redraw();
#else
	nCmdShow = (bShow) ? SW_SHOW : SW_HIDE;
	ShowWindow(m_hWndAccount,	bShow);
	ShowWindow(m_hWndPassword,	bShow);
	ShowWindow(m_hWndSavePassword,	bShow);
	ShowWindow(m_hWndConnect,	bShow);
#endif
}


void CWindowLOGIN::Save(void)
{
	BOOL bSave;

	m_pMgrData->SetLastAccount(m_strAccount);
	m_pMgrData->SetLastPassword("");

	bSave = GetCheck();
	m_pMgrData->SetSavePassword(bSave);
	if (bSave) {
		m_pMgrData->SetLastPassword(m_strPassword);
	}
	m_pMgrData->SaveIniData();
}


void CWindowLOGIN::SetCheck(BOOL bCheck)
{
#if defined(__EMSCRIPTEN__)
	m_bSavePassword = bCheck;
	Redraw();
#else
	WPARAM wParam;

	if (m_hWndSavePassword == NULL) {
		return;
	}

	wParam = BST_UNCHECKED;
	if (bCheck) {
		wParam = BST_CHECKED;
	}
	SendMessage(m_hWndSavePassword, BM_SETCHECK, wParam, 0);
#endif
}


BOOL CWindowLOGIN::GetCheck(void)
{
#if defined(__EMSCRIPTEN__)
	return m_bSavePassword;
#else
	LRESULT lResult;
	BOOL bRet;

	bRet = FALSE;
	if (m_hWndSavePassword == NULL) {
		goto Exit;
	}

	lResult = SendMessage(m_hWndSavePassword, BM_GETCHECK, 0, 0);
	if (lResult == BST_CHECKED) {
		bRet = TRUE;
	}

Exit:
	return bRet;
#endif
}


BOOL CWindowLOGIN::HandleKeyDown(UINT vk)
{
#if defined(__EMSCRIPTEN__)
	if (!IsInteractive()) {
		return FALSE;
	}

	switch (vk) {
	case VK_TAB:
		MoveFocus(IsShiftPressed() ? -1 : 1);
		return TRUE;

	case VK_BACK:
		DeleteBackward();
		return TRUE;

	case VK_SPACE:
		if (m_nFocusIndex == LOGINFOCUS_SAVEPASSWORD) {
			m_bSavePassword = !m_bSavePassword;
			Redraw();
			return TRUE;
		}
		break;

	case VK_RETURN:
		if (m_nFocusIndex == LOGINFOCUS_SAVEPASSWORD) {
			m_bSavePassword = !m_bSavePassword;
			Redraw();
		}
		OnConnect();
		return TRUE;

	case VK_ESCAPE:
		return TRUE;
	}

	return IsTextFieldFocus();
#else
	UNREFERENCED_PARAMETER(vk);
	return FALSE;
#endif
}


BOOL CWindowLOGIN::HandleSDLKeyDown(UINT vk)
{
	return HandleKeyDown(vk);
}


void CWindowLOGIN::HandleTextInput(LPCSTR pszText)
{
#if defined(__EMSCRIPTEN__)
	if (!IsInteractive()) {
		return;
	}
	if (!IsTextFieldFocus()) {
		return;
	}

	AppendText(pszText);
#else
	UNREFERENCED_PARAMETER(pszText);
#endif
}


void CWindowLOGIN::HandleSDLTextInput(LPCSTR pszText)
{
	HandleTextInput(pszText);
}


BOOL CWindowLOGIN::HandleMouseLeftButtonDown(int x, int y)
{
#if defined(__EMSCRIPTEN__)
	RECT rcTmp;

	if (!IsInteractive()) {
		return FALSE;
	}

	if (HitTest(x, y, rcTmp, LOGINFOCUS_ACCOUNT)) {
		SetFocusIndex(LOGINFOCUS_ACCOUNT);
		return TRUE;
	}
	if (HitTest(x, y, rcTmp, LOGINFOCUS_PASSWORD)) {
		SetFocusIndex(LOGINFOCUS_PASSWORD);
		return TRUE;
	}
	if (HitTest(x, y, rcTmp, LOGINFOCUS_SAVEPASSWORD)) {
		SetFocusIndex(LOGINFOCUS_SAVEPASSWORD);
		m_bSavePassword = !m_bSavePassword;
		Redraw();
		return TRUE;
	}
	if (HitTest(x, y, rcTmp, LOGINFOCUS_CONNECT)) {
		SetFocusIndex(LOGINFOCUS_CONNECT);
		OnConnect();
		return TRUE;
	}
#else
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
#endif
	return FALSE;
}


BOOL CWindowLOGIN::HandleSDLMouseLeftButtonDown(int x, int y)
{
	return HandleMouseLeftButtonDown(x, y);
}


void CWindowLOGIN::MakeWindow(void)
{
	BOOL bSave;
	CmyString strAccount, strPassword;

	strAccount  = m_pMgrData->GetLastAccount();
	strPassword = m_pMgrData->GetLastPassword();
	bSave = m_pMgrData->GetSavePassword();
	SetCheck(bSave);

#if defined(__EMSCRIPTEN__)
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
	UpdateSDLTextInput();
#else
	HINSTANCE hInstance;
	HWND hWndMain;
	HMODULE hThemeModule;
	typedef HRESULT(WINAPI *SETWINDOWTHEME)(HWND, LPCWSTR, LPCWSTR);
	SETWINDOWTHEME pfSetWindowTheme;

	hInstance	= m_pMgrData->GetInstance();
	hWndMain	= m_pMgrData->GetMainWindow();

	// アカウント入力欄
	m_hWndAccount = CreateWindow(_T("EDIT"), _T(""), WS_CHILD |  ES_LOWERCASE,
			m_ptViewPos.x + 16 * 8, m_ptViewPos.y + 16, 8 * 12, 14, hWndMain, NULL, hInstance, NULL);
	m_OrgWndProcAccount = (WNDPROC)GetWindowLong(m_hWndAccount, GWL_WNDPROC);
	SendMessage(m_hWndAccount, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
	ImmAssociateContext(m_hWndAccount, NULL);
	SetWindowLong(m_hWndAccount, GWL_USERDATA, (LONG)this);
	SetWindowLong(m_hWndAccount, GWL_WNDPROC, (LONG)AccountWndProc);
	SetWindowText(m_hWndAccount, strAccount);

	// パスワード入力欄
	m_hWndPassword = CreateWindow(_T("EDIT"), _T(""), WS_CHILD | ES_PASSWORD | ES_AUTOHSCROLL,
			m_ptViewPos.x + 16 * 8, m_ptViewPos.y + 42, 8 * 12, 14, hWndMain, NULL, hInstance, NULL);
	m_OrgWndProcPassword = (WNDPROC)GetWindowLong(m_hWndPassword, GWL_WNDPROC);
	SendMessage(m_hWndPassword, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
	ImmAssociateContext(m_hWndPassword, NULL);
	SetWindowLong(m_hWndPassword, GWL_USERDATA, (LONG)this);
	SetWindowLong(m_hWndPassword, GWL_WNDPROC, (LONG)PasswordWndProc);

	// パスワードを記録するチェック
	m_hWndSavePassword = CreateWindow(_T("BUTTON"), _T("パスワードを記録する"), WS_CHILD | BS_AUTOCHECKBOX,
			m_ptViewPos.x + 16, m_ptViewPos.y + 16 * 4 + 6, 6 * 23, 14, hWndMain, (HMENU)IDC_SAVEPASSWORD, hInstance, NULL);
	m_OrgWndProcSavePassword = (WNDPROC)GetWindowLong(m_hWndSavePassword, GWL_WNDPROC);
	SendMessage(m_hWndSavePassword, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
	SendMessage(m_hWndSavePassword, BM_SETCHECK, FALSE, 0);
	SetWindowLong(m_hWndSavePassword, GWL_USERDATA, (LONG)this);
	SetWindowLong(m_hWndSavePassword, GWL_WNDPROC, (LONG)SavePasswordWndProc);
	InvalidateRect(m_hWndSavePassword, NULL, TRUE);
	if (bSave) {
		SetWindowText(m_hWndPassword, strPassword);
	}

	// 接続ボタン
	m_hWndConnect = CreateWindow(_T("BUTTON"), _T("接続"), WS_CHILD | BS_PUSHBUTTON,
			m_ptViewPos.x + 16 * 11 - 2, m_ptViewPos.y + 16 * 4 + 2, 54, 18, hWndMain, (HMENU)IDC_CONNECT, hInstance, NULL);
	m_OrgWndProcConnect = (WNDPROC)GetWindowLong(m_hWndConnect, GWL_WNDPROC);
	SendMessage(m_hWndConnect, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
	SetWindowLong(m_hWndConnect, GWL_USERDATA, (LONG)this);
	SetWindowLong(m_hWndConnect, GWL_WNDPROC, (LONG)ConnectWndProc);

	hThemeModule = LoadLibrary(_T("UxTheme.dll"));
	if (hThemeModule) {
		pfSetWindowTheme = (SETWINDOWTHEME)GetProcAddress(hThemeModule, "SetWindowTheme");
		pfSetWindowTheme(m_hWndSavePassword, L" ", L" ");
		FreeLibrary(hThemeModule);
	}

	if (strAccount.IsEmpty()) {
		SetFocus(m_hWndAccount);
	} else if (strPassword.IsEmpty()) {
		SetFocus(m_hWndPassword);
	} else {
		SetFocus(m_hWndConnect);
	}
#endif
}


void CWindowLOGIN::UpdateSDLTextInput(void)
{
#if defined(__EMSCRIPTEN__)
	if (IsInteractive() && IsTextFieldFocus()) {
		SDL_StartTextInput();
	} else {
		SDL_StopTextInput();
	}
#endif
}


void CWindowLOGIN::MoveFocus(int nStep)
{
#if defined(__EMSCRIPTEN__)
	int nFocus;

	nFocus = m_nFocusIndex + nStep;
	if (nFocus < 0) {
		nFocus = LOGINFOCUS_MAX - 1;
	} else if (nFocus >= LOGINFOCUS_MAX) {
		nFocus = 0;
	}
	SetFocusIndex(nFocus);
#else
	UNREFERENCED_PARAMETER(nStep);
#endif
}


void CWindowLOGIN::SetFocusIndex(int nIndex)
{
#if defined(__EMSCRIPTEN__)
	if (nIndex < 0) {
		nIndex = 0;
	}
	if (nIndex >= LOGINFOCUS_MAX) {
		nIndex = LOGINFOCUS_MAX - 1;
	}

	m_nFocusIndex = nIndex;
	UpdateSDLTextInput();
	Redraw();
#else
	UNREFERENCED_PARAMETER(nIndex);
#endif
}


void CWindowLOGIN::DeleteBackward(void)
{
#if defined(__EMSCRIPTEN__)
	CmyString strTrimmed;
	CString strTmp;
	int nLength;

	if (!IsTextFieldFocus()) {
		return;
	}

	strTmp = (m_nFocusIndex == LOGINFOCUS_ACCOUNT) ? (LPCTSTR)m_strAccount : (LPCTSTR)m_strPassword;
	nLength = strTmp.GetLength();
	if (nLength <= 0) {
		return;
	}

	strTmp = strTmp.Left(nLength - 1);
	TrimViewString(strTrimmed, (LPCTSTR)strTmp);
	if (m_nFocusIndex == LOGINFOCUS_ACCOUNT) {
		m_strAccount = strTrimmed;
	} else {
		m_strPassword = strTrimmed;
	}
	Redraw();
#endif
}


void CWindowLOGIN::AppendText(LPCSTR pszText)
{
#if defined(__EMSCRIPTEN__)
	CmyString strCurrent;
	CmyString strAppend;
	CString strWide;
	CString strFiltered;
	int nAllow, nLength;

	if ((pszText == NULL) || (pszText[0] == '\0')) {
		return;
	}

	strCurrent = (m_nFocusIndex == LOGINFOCUS_ACCOUNT) ? m_strAccount : m_strPassword;
	nAllow = LOGIN_TEXT_MAX - strCurrent.GetLength();
	if (nAllow <= 0) {
		return;
	}

	strWide = Utf8ToTString(pszText);
	nLength = strWide.GetLength();
	for (int i = 0; i < nLength; ++i) {
		TCHAR ch = strWide[i];

		if (ch < _T(' ')) {
			continue;
		}
		if (m_nFocusIndex == LOGINFOCUS_ACCOUNT) {
			if ((ch >= _T('A')) && (ch <= _T('Z'))) {
				ch = (TCHAR)(ch - _T('A') + _T('a'));
			}
		}
		strFiltered.AppendChar(ch);
		if (strFiltered.GetLength() >= nAllow) {
			break;
		}
	}
	if (strFiltered.IsEmpty()) {
		return;
	}

	TrimViewString(strAppend, (LPCTSTR)strFiltered);
	strCurrent += (LPCTSTR)strAppend;
	if (m_nFocusIndex == LOGINFOCUS_ACCOUNT) {
		m_strAccount = strCurrent;
	} else {
		m_strPassword = strCurrent;
	}
	Redraw();
#else
	UNREFERENCED_PARAMETER(pszText);
#endif
}


BOOL CWindowLOGIN::IsTextFieldFocus(void) const
{
#if defined(__EMSCRIPTEN__)
	return ((m_nFocusIndex == LOGINFOCUS_ACCOUNT) || (m_nFocusIndex == LOGINFOCUS_PASSWORD)) ? TRUE : FALSE;
#else
	return FALSE;
#endif
}


BOOL CWindowLOGIN::IsInteractive(void) const
{
#if defined(__EMSCRIPTEN__)
	return (m_bShow && m_bEnabled) ? TRUE : FALSE;
#else
	return FALSE;
#endif
}


BOOL CWindowLOGIN::HitTest(int x, int y, RECT &rcDst, int nFocusIndex) const
{
#if defined(__EMSCRIPTEN__)
	switch (nFocusIndex) {
	case LOGINFOCUS_ACCOUNT:
		GetWindowLOGINAccountRect(&rcDst);
		OffsetWindowLOGINRect(this, &rcDst);
		break;
	case LOGINFOCUS_PASSWORD:
		GetWindowLOGINPasswordRect(&rcDst);
		OffsetWindowLOGINRect(this, &rcDst);
		break;
	case LOGINFOCUS_SAVEPASSWORD:
		GetWindowLOGINCheckRect(&rcDst);
		OffsetWindowLOGINRect(this, &rcDst);
		break;
	case LOGINFOCUS_CONNECT:
		GetWindowLOGINConnectRect(&rcDst);
		OffsetWindowLOGINRect(this, &rcDst);
		break;
	default:
		ZeroMemory(&rcDst, sizeof(rcDst));
		return FALSE;
	}

	return ((x >= rcDst.left) && (x < rcDst.right) && (y >= rcDst.top) && (y < rcDst.bottom)) ? TRUE : FALSE;
#else
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	UNREFERENCED_PARAMETER(nFocusIndex);
	ZeroMemory(&rcDst, sizeof(rcDst));
	return FALSE;
#endif
}


void CWindowLOGIN::DrawBrowserControls(const RECT &rcAccount, const RECT &rcPassword, const RECT &rcCheck, const RECT &rcConnect)
{
#if defined(__EMSCRIPTEN__)
	const BOOL bFocusAccount = (m_nFocusIndex == LOGINFOCUS_ACCOUNT);
	const BOOL bFocusPassword = (m_nFocusIndex == LOGINFOCUS_PASSWORD);
	const BOOL bFocusCheck = (m_nFocusIndex == LOGINFOCUS_SAVEPASSWORD);
	const BOOL bFocusConnect = (m_nFocusIndex == LOGINFOCUS_CONNECT);
	RECT rcCheckBox;

	DrawBrowserRect(rcAccount.left - 1, rcAccount.top - 1, rcAccount.right - rcAccount.left + 2, rcAccount.bottom - rcAccount.top + 2, RGB(120, 78, 42), FALSE, RGB(120, 78, 42), TRUE);
	DrawBrowserRect(rcAccount.left, rcAccount.top, rcAccount.right - rcAccount.left, rcAccount.bottom - rcAccount.top, RGB(255, 252, 245), TRUE, RGB(240, 214, 172), TRUE);
	if (bFocusAccount) {
		DrawBrowserRect(rcAccount.left - 2, rcAccount.top - 2, rcAccount.right - rcAccount.left + 4, rcAccount.bottom - rcAccount.top + 4, RGB(255, 255, 255), FALSE, RGB(255, 196, 92), TRUE);
	}

	DrawBrowserRect(rcPassword.left - 1, rcPassword.top - 1, rcPassword.right - rcPassword.left + 2, rcPassword.bottom - rcPassword.top + 2, RGB(120, 78, 42), FALSE, RGB(120, 78, 42), TRUE);
	DrawBrowserRect(rcPassword.left, rcPassword.top, rcPassword.right - rcPassword.left, rcPassword.bottom - rcPassword.top, RGB(255, 252, 245), TRUE, RGB(240, 214, 172), TRUE);
	if (bFocusPassword) {
		DrawBrowserRect(rcPassword.left - 2, rcPassword.top - 2, rcPassword.right - rcPassword.left + 4, rcPassword.bottom - rcPassword.top + 4, RGB(255, 255, 255), FALSE, RGB(255, 196, 92), TRUE);
	}

	GetWindowLOGINCheckBoxRect(rcCheck, &rcCheckBox);
	if (bFocusCheck) {
		DrawBrowserRect(rcCheckBox.left - 2, rcCheckBox.top - 2, (rcCheckBox.right - rcCheckBox.left) + 4, (rcCheckBox.bottom - rcCheckBox.top) + 4, RGB(255, 255, 255), FALSE, RGB(255, 196, 92), TRUE);
	}
	DrawBrowserRect(rcCheckBox.left, rcCheckBox.top, rcCheckBox.right - rcCheckBox.left, rcCheckBox.bottom - rcCheckBox.top, RGB(255, 250, 240), TRUE, RGB(120, 78, 42), TRUE);
	if (m_bSavePassword) {
		DrawBrowserRect(rcCheckBox.left + 3, rcCheckBox.top + 3, 5, 5, RGB(80, 56, 24), TRUE, RGB(80, 56, 24), FALSE);
	}

	DrawBrowserRect(rcConnect.left, rcConnect.top, rcConnect.right - rcConnect.left, rcConnect.bottom - rcConnect.top, RGB(235, 206, 158), TRUE, RGB(120, 78, 42), TRUE);
	DrawBrowserRect(rcConnect.left + 1, rcConnect.top + 1, rcConnect.right - rcConnect.left - 2, 1, RGB(255, 245, 224), TRUE, RGB(255, 245, 224), FALSE);
	DrawBrowserRect(rcConnect.left + 1, rcConnect.top + 1, 1, rcConnect.bottom - rcConnect.top - 2, RGB(255, 245, 224), TRUE, RGB(255, 245, 224), FALSE);
	DrawBrowserRect(rcConnect.left + 1, rcConnect.bottom - 2, rcConnect.right - rcConnect.left - 2, 1, RGB(154, 108, 58), TRUE, RGB(154, 108, 58), FALSE);
	DrawBrowserRect(rcConnect.right - 2, rcConnect.top + 1, 1, rcConnect.bottom - rcConnect.top - 2, RGB(154, 108, 58), TRUE, RGB(154, 108, 58), FALSE);
	if (bFocusConnect) {
		DrawBrowserRect(rcConnect.left - 2, rcConnect.top - 2, rcConnect.right - rcConnect.left + 4, rcConnect.bottom - rcConnect.top + 4, RGB(255, 255, 255), FALSE, RGB(255, 196, 92), TRUE);
	}
#else
	UNREFERENCED_PARAMETER(rcAccount);
	UNREFERENCED_PARAMETER(rcPassword);
	UNREFERENCED_PARAMETER(rcCheck);
	UNREFERENCED_PARAMETER(rcConnect);
#endif
}


void CWindowLOGIN::DrawTextField(HDC hDC, const RECT &rcField, LPCSTR pszText, BOOL bPassword, BOOL bFocused)
{
#if defined(__EMSCRIPTEN__)
	if (hDC != NULL) {
		DrawInputFrame1(rcField.left, rcField.top, rcField.right - rcField.left, rcField.bottom - rcField.top, bFocused ? 1 : 0);
	}
	UNREFERENCED_PARAMETER(pszText);
	UNREFERENCED_PARAMETER(bPassword);
	UNREFERENCED_PARAMETER(bFocused);
#else
	UNREFERENCED_PARAMETER(hDC);
	UNREFERENCED_PARAMETER(rcField);
	UNREFERENCED_PARAMETER(pszText);
	UNREFERENCED_PARAMETER(bPassword);
	UNREFERENCED_PARAMETER(bFocused);
#endif
}


#if defined(__EMSCRIPTEN__)
void CWindowLOGIN::UpdateBrowserDom(const RECT &rcAccount, const RECT &rcPassword, const RECT &rcCheck, const RECT &rcConnect)
{
	const int nWindowX = m_ptViewPos.x + 32;
	const int nWindowY = m_ptViewPos.y + 32;

	SBOP2_UpdateLoginDom(
		(IsInteractive() ? 1 : 0),
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


void CWindowLOGIN::HideBrowserDom(void)
{
	SBOP2_HideLoginDom();
}


void CWindowLOGIN::SetAccountFromBrowser(LPCSTR pszText)
{
	CmyString strTrimmed;
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
		if ((ch >= _T('A')) && (ch <= _T('Z'))) {
			ch = (TCHAR)(ch - _T('A') + _T('a'));
		}
		strFiltered.AppendChar(ch);
		if (strFiltered.GetLength() >= LOGIN_TEXT_MAX) {
			break;
		}
	}

	TrimViewString(strTrimmed, (LPCTSTR)strFiltered);
	m_strAccount = strTrimmed;
	Redraw();
}


void CWindowLOGIN::SetPasswordFromBrowser(LPCSTR pszText)
{
	CmyString strTrimmed;
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
		strFiltered.AppendChar(ch);
		if (strFiltered.GetLength() >= LOGIN_TEXT_MAX) {
			break;
		}
	}

	TrimViewString(strTrimmed, (LPCTSTR)strFiltered);
	m_strPassword = strTrimmed;
	Redraw();
}


void CWindowLOGIN::SetSavePasswordFromBrowser(BOOL bCheck)
{
	m_bSavePassword = bCheck;
	Redraw();
}


void CWindowLOGIN::SubmitFromBrowser(void)
{
	OnConnect();
}
#endif

#if !defined(__EMSCRIPTEN__)
BOOL CWindowLOGIN::HandleCtlColorStatic(WPARAM wParam, LPARAM lParam, LRESULT &lResult)
{
	int nID;
	HDC hDC;

	nID = GetWindowLong((HWND)lParam, GWL_ID);
	if (nID != IDC_SAVEPASSWORD) {
		return FALSE;
	}

	hDC = (HDC)wParam;
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(0, 0, 0));
	lResult = (LRESULT)GetStockObject(NULL_BRUSH);
	return TRUE;
}


LRESULT CALLBACK CWindowLOGIN::AccountWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PCWindowLOGIN pThis;
	LRESULT hResult;

	pThis	= (PCWindowLOGIN)GetWindowLong(hWnd, GWL_USERDATA);
	hResult	= -1;

	switch (message) {
	case WM_CHAR:
		if (wParam == VK_TAB) {
			if (IsShiftPressed()) {
				SetFocus(pThis->m_hWndConnect);
			} else {
				SetFocus(pThis->m_hWndPassword);
			}
			hResult = 0;

		} else if (wParam == VK_RETURN) {
			pThis->OnConnect();
			hResult = 0;
		}
		break;
	}

	if (hResult != 0) {
		hResult = CallWindowProc(pThis->m_OrgWndProcAccount, hWnd, message, wParam, lParam);
	}
	return hResult;
}


LRESULT CALLBACK CWindowLOGIN::PasswordWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PCWindowLOGIN pThis;
	LRESULT hResult;

	pThis	= (PCWindowLOGIN)GetWindowLong(hWnd, GWL_USERDATA);
	hResult	= -1;

	switch (message) {
	case WM_CHAR:
		if (wParam == VK_TAB) {
			if (IsShiftPressed()) {
				SetFocus(pThis->m_hWndAccount);
			} else {
				SetFocus(pThis->m_hWndSavePassword);
			}
			hResult = 0;

		} else if (wParam == VK_RETURN) {
			pThis->OnConnect();
			hResult = 0;
		}
		break;
	}

	if (hResult != 0) {
		hResult = CallWindowProc(pThis->m_OrgWndProcPassword, hWnd, message, wParam, lParam);
	}
	return hResult;
}


LRESULT CALLBACK CWindowLOGIN::SavePasswordWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PCWindowLOGIN pThis;
	LRESULT hResult;

	pThis	= (PCWindowLOGIN)GetWindowLong(hWnd, GWL_USERDATA);
	hResult	= -1;

	switch (message) {
	case WM_CHAR:
		if (wParam == VK_TAB) {
			if (IsShiftPressed()) {
				SetFocus(pThis->m_hWndPassword);
			} else {
				SetFocus(pThis->m_hWndConnect);
			}
			hResult = 0;

		} else if (wParam == VK_RETURN) {
			pThis->OnConnect();
			hResult = 0;
		}
		break;

	case WM_ERASEBKGND:
		{
			RECT rc;
			HDC hDC;
			HBRUSH hBrush, hBrushOld;

			hDC = GetDC(hWnd);
			hBrush = CreateSolidBrush(RGB(255, 235, 200));

			GetClientRect(hWnd, &rc);
			hBrushOld = (HBRUSH)SelectObject(hDC, hBrush);
			FillRect(hDC, &rc, hBrush);

			ReleaseDC(hWnd, hDC);
			SelectObject(hDC, hBrushOld);
			DeleteObject(hBrush);
			InvalidateRect(hWnd, NULL, FALSE);
			UpdateWindow(hWnd);
		}
		return 1;
	}

	if (hResult != 0) {
		hResult = CallWindowProc(pThis->m_OrgWndProcSavePassword, hWnd, message, wParam, lParam);
	}
	return hResult;
}


LRESULT CALLBACK CWindowLOGIN::ConnectWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PCWindowLOGIN pThis;
	LRESULT hResult;

	pThis	= (PCWindowLOGIN)GetWindowLong(hWnd, GWL_USERDATA);
	hResult	= -1;

	switch (message) {
	case WM_CHAR:
		if (wParam == VK_TAB) {
			if (IsShiftPressed()) {
				SetFocus(pThis->m_hWndSavePassword);
			} else {
				SetFocus(pThis->m_hWndAccount);
			}
			hResult = 0;
		}
		break;

	case WM_KEYUP:
		if ((wParam == VK_RETURN) || (wParam == VK_SPACE)) {
			pThis->OnConnect();
		}
		break;

	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
		pThis->OnConnect();
		break;
	}

	if (hResult != 0) {
		hResult = CallWindowProc(pThis->m_OrgWndProcConnect, hWnd, message, wParam, lParam);
	}
	return hResult;
}


void CWindowLOGIN::OnConnect(void)
{
	TCHAR szTmp[256];

	m_pMgrSound->PlaySound(SOUNDID_OK_PI73);

	GetWindowText(m_hWndAccount, szTmp, _countof(szTmp));
	TrimViewString(m_strAccount, szTmp);
	GetWindowText(m_hWndPassword, szTmp, _countof(szTmp));
	TrimViewString(m_strPassword, szTmp);
	if (m_strAccount.IsEmpty()) {
		return;
	}
	Enable(FALSE);

	m_pMgrData->PostMainFrameMessage(MAINFRAMEMSG_CONNECT, 0);
}
#else
BOOL CWindowLOGIN::HandleCtlColorStatic(WPARAM wParam, LPARAM lParam, LRESULT &lResult)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	UNREFERENCED_PARAMETER(lResult);
	return FALSE;
}


LRESULT CALLBACK CWindowLOGIN::AccountWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	return 0;
}


LRESULT CALLBACK CWindowLOGIN::PasswordWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	return 0;
}


LRESULT CALLBACK CWindowLOGIN::SavePasswordWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	return 0;
}


LRESULT CALLBACK CWindowLOGIN::ConnectWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	return 0;
}


void CWindowLOGIN::OnConnect(void)
{
	if (m_strAccount.IsEmpty()) {
		return;
	}

	m_pMgrSound->PlaySound(SOUNDID_OK_PI73);
	Enable(FALSE);
	m_pMgrData->PostMainFrameMessage(MAINFRAMEMSG_CONNECT, 0);
}
#endif
