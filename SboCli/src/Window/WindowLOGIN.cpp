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
#define SDL_MAIN_HANDLED
#include <SDL.h>
#if defined(__EMSCRIPTEN__)
#include <emscripten/em_js.h>
#include <emscripten/emscripten.h>
#endif
#include "WindowLOGIN.h"
#include "../Platform/SdlFont.h"

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
	m_sizeWindow.cy	= 16 * 7;
	m_ptViewPos.x	= SCRSIZEX / 2 - m_sizeWindow.cx / 2;
	m_ptViewPos.y	= 280;
	m_bEnabled = TRUE;
	m_bSavePassword = FALSE;
	m_nFocusIndex = LOGINFOCUS_ACCOUNT;
	m_nCursorPos = 0;
	m_bInput = TRUE;
}


CWindowLOGIN::~CWindowLOGIN()
{
#if defined(__EMSCRIPTEN__)
	if (g_pBrowserLoginWindow == this) {
		g_pBrowserLoginWindow = NULL;
	}
	HideBrowserDom();
#endif
	UpdateSDLTextInput();
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
	RECT rcAccount, rcPassword, rcCheck, rcConnect;
	CString strCheck;

	GetWindowLOGINAccountRect(&rcAccount);
	GetWindowLOGINPasswordRect(&rcPassword);
	GetWindowLOGINCheckRect(&rcCheck);
	GetWindowLOGINConnectRect(&rcConnect);
	strCheck = GetLoginLabelSavePassword();

	if (m_dwTimeDrawStart == 0) {
		DrawFrame();
		// 入力欄背景・チェックボックス枠・接続ボタン枠を自前描画（Windows / Emscripten 共通）
		DrawBrowserControls(rcAccount, rcPassword, rcCheck, rcConnect);
		hDC	= m_pDib->Lock();

		TextOut2(hDC, m_hFont, 16, 10, _T("アカウント:"), RGB(1, 1, 1));
		TextOut2(hDC, m_hFont, 16, 36, _T("パスワード:"), RGB(1, 1, 1));
		DrawTextField(hDC, rcAccount, m_strAccount, FALSE, (m_nFocusIndex == LOGINFOCUS_ACCOUNT));
		DrawTextField(hDC, rcPassword, m_strPassword, TRUE, (m_nFocusIndex == LOGINFOCUS_PASSWORD));
		{
			// チェックボックス枠の右端を取得してテキストをその右側に描画
			RECT rcCheckBox;
			GetWindowLOGINCheckBoxRect(rcCheck, &rcCheckBox);
			TextOut2(hDC, m_hFont, rcCheckBox.right + 4, rcCheck.top - 6, strCheck, RGB(1, 1, 1));
		}
		TextOut2(hDC, m_hFont, rcConnect.left + 12, rcConnect.top - 4, GetLoginLabelConnect(), RGB(1, 1, 1));

		m_pDib->Unlock();
		m_dwTimeDrawStart = timeGetTime();
	}
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);

#if defined(__EMSCRIPTEN__)
	g_pBrowserLoginWindow = this;
	UpdateBrowserDom(rcAccount, rcPassword, rcCheck, rcConnect);
#endif
}


void CWindowLOGIN::Enable(BOOL bEnable)
{
	m_bEnabled = bEnable;
	UpdateSDLTextInput();
	if (!bEnable) {
#if defined(__EMSCRIPTEN__)
		HideBrowserDom();
#endif
	}
	Redraw();
}


void CWindowLOGIN::SetShow(BOOL bShow)
{
	CWindowBase::SetShow(bShow);
	UpdateSDLTextInput();
	if (!bShow) {
#if defined(__EMSCRIPTEN__)
		HideBrowserDom();
#endif
	}
	Redraw();
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
	m_bSavePassword = bCheck;
	Redraw();
}


BOOL CWindowLOGIN::GetCheck(void)
{
	return m_bSavePassword;
}


BOOL CWindowLOGIN::HandleKeyDown(UINT vk)
{
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

	case VK_DELETE:
		if (IsTextFieldFocus()) {
			DeleteForward();
			return TRUE;
		}
		break;

	case VK_LEFT:
		if (IsTextFieldFocus()) {
			if (m_nCursorPos > 0) {
				m_nCursorPos--;
				m_nCursorAnime = 0;
				Redraw();
			}
			return TRUE;
		}
		break;

	case VK_RIGHT:
		if (IsTextFieldFocus()) {
			CString str = GetCurrentFieldText();
			if (m_nCursorPos < str.GetLength()) {
				m_nCursorPos++;
				m_nCursorAnime = 0;
				Redraw();
			}
			return TRUE;
		}
		break;

	case VK_HOME:
		if (IsTextFieldFocus()) {
			m_nCursorPos = 0;
			m_nCursorAnime = 0;
			Redraw();
			return TRUE;
		}
		break;

	case VK_END:
		if (IsTextFieldFocus()) {
			CString str = GetCurrentFieldText();
			m_nCursorPos = str.GetLength();
			m_nCursorAnime = 0;
			Redraw();
			return TRUE;
		}
		break;

	case VK_SPACE:
		if (m_nFocusIndex == LOGINFOCUS_SAVEPASSWORD) {
			SetCheck(!GetCheck());
			return TRUE;
		}
		break;

	case VK_RETURN:
		if (m_nFocusIndex == LOGINFOCUS_SAVEPASSWORD) {
			SetCheck(!GetCheck());
		}
		OnConnect();
		return TRUE;

	case VK_ESCAPE:
		return TRUE;
	}

	return IsTextFieldFocus();
}


BOOL CWindowLOGIN::HandleSDLKeyDown(UINT vk)
{
	return HandleKeyDown(vk);
}


void CWindowLOGIN::HandleTextInput(LPCSTR pszText)
{
	if (!IsInteractive()) {
		return;
	}
	if (!IsTextFieldFocus()) {
		return;
	}

	AppendText(pszText);
}


void CWindowLOGIN::HandleSDLTextInput(LPCSTR pszText)
{
	HandleTextInput(pszText);
}


BOOL CWindowLOGIN::HandleMouseLeftButtonDown(int x, int y)
{
	RECT rcTmp;

	if (!IsInteractive()) {
		return FALSE;
	}

	if (HitTest(x, y, rcTmp, LOGINFOCUS_ACCOUNT)) {
		SetFocusIndex(LOGINFOCUS_ACCOUNT);
		// クリックX座標からカーソル位置を計算（8px固定幅）
		{
			int nCharIndex = (x - rcTmp.left - 2) / 8;
			CString str = GetCurrentFieldText();
			if (nCharIndex < 0) nCharIndex = 0;
			if (nCharIndex > str.GetLength()) nCharIndex = str.GetLength();
			m_nCursorPos = nCharIndex;
		}
		return TRUE;
	}
	if (HitTest(x, y, rcTmp, LOGINFOCUS_PASSWORD)) {
		SetFocusIndex(LOGINFOCUS_PASSWORD);
		// クリックX座標からカーソル位置を計算（8px固定幅）
		{
			int nCharIndex = (x - rcTmp.left - 2) / 8;
			CString str = GetCurrentFieldText();
			if (nCharIndex < 0) nCharIndex = 0;
			if (nCharIndex > str.GetLength()) nCharIndex = str.GetLength();
			m_nCursorPos = nCharIndex;
		}
		return TRUE;
	}
	if (HitTest(x, y, rcTmp, LOGINFOCUS_SAVEPASSWORD)) {
		SetFocusIndex(LOGINFOCUS_SAVEPASSWORD);
		SetCheck(!GetCheck());
		return TRUE;
	}
	if (HitTest(x, y, rcTmp, LOGINFOCUS_CONNECT)) {
		SetFocusIndex(LOGINFOCUS_CONNECT);
		OnConnect();
		return TRUE;
	}
	return FALSE;
}


BOOL CWindowLOGIN::HandleSDLMouseLeftButtonDown(int x, int y)
{
	return HandleMouseLeftButtonDown(x, y);
}


void CWindowLOGIN::MakeWindow(void)
{
	BOOL bSave;
	bSave = m_pMgrData->GetSavePassword();
	SetCheck(bSave);

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
}


void CWindowLOGIN::UpdateSDLTextInput(void)
{
	if (IsInteractive() && IsTextFieldFocus()) {
		SDL_StartTextInput();
	} else {
		SDL_StopTextInput();
	}
}

void CWindowLOGIN::MoveFocus(int nStep)
{
	int nFocus;

	nFocus = m_nFocusIndex + nStep;
	if (nFocus < 0) {
		nFocus = LOGINFOCUS_MAX - 1;
	} else if (nFocus >= LOGINFOCUS_MAX) {
		nFocus = 0;
	}
	SetFocusIndex(nFocus);
}


void CWindowLOGIN::SetFocusIndex(int nIndex)
{
	if (nIndex < 0) {
		nIndex = 0;
	}
	if (nIndex >= LOGINFOCUS_MAX) {
		nIndex = LOGINFOCUS_MAX - 1;
	}

	m_nFocusIndex = nIndex;

	// フォーカス変更時はカーソルをテキスト末尾に設定
	if (nIndex == LOGINFOCUS_ACCOUNT || nIndex == LOGINFOCUS_PASSWORD) {
		CString str = GetCurrentFieldText();
		m_nCursorPos = str.GetLength();
	} else {
		m_nCursorPos = 0;
	}

	UpdateSDLTextInput();
	Redraw();
}


void CWindowLOGIN::DeleteBackward(void)
{
	CString str;

	if (!IsTextFieldFocus()) {
		return;
	}
	if (m_nCursorPos <= 0) {
		return;
	}

	str = GetCurrentFieldText();
	if (m_nCursorPos > str.GetLength()) {
		m_nCursorPos = str.GetLength();
	}
	if (m_nCursorPos <= 0) {
		return;
	}

	str.Delete(m_nCursorPos - 1, 1);
	m_nCursorPos--;
	SetCurrentFieldText(str);
	m_nCursorAnime = 0;
	Redraw();
}


CString CWindowLOGIN::GetCurrentFieldText(void) const
{
	if (m_nFocusIndex == LOGINFOCUS_ACCOUNT) {
		return CString((LPCTSTR)m_strAccount);
	}
	return CString((LPCTSTR)m_strPassword);
}


void CWindowLOGIN::SetCurrentFieldText(LPCTSTR pszText)
{
	CmyString strTrimmed;

	TrimViewString(strTrimmed, pszText);
	if (m_nFocusIndex == LOGINFOCUS_ACCOUNT) {
		m_strAccount = strTrimmed;
	} else {
		m_strPassword = strTrimmed;
	}
}


void CWindowLOGIN::DeleteForward(void)
{
	CString str;

	if (!IsTextFieldFocus()) {
		return;
	}

	str = GetCurrentFieldText();
	if (m_nCursorPos >= str.GetLength()) {
		return;
	}

	str.Delete(m_nCursorPos, 1);
	SetCurrentFieldText(str);
	m_nCursorAnime = 0;
	Redraw();
}


void CWindowLOGIN::AppendText(LPCSTR pszText)
{
	CmyString strAppend;
	CString strCurrent;
	CString strWide;
	CString strFiltered;
	int nAllow, nLength;

	if ((pszText == NULL) || (pszText[0] == '\0')) {
		return;
	}

	strCurrent = GetCurrentFieldText();
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

	// カーソル位置に挿入する
	if (m_nCursorPos < 0) {
		m_nCursorPos = 0;
	}
	if (m_nCursorPos > strCurrent.GetLength()) {
		m_nCursorPos = strCurrent.GetLength();
	}
	strCurrent.Insert(m_nCursorPos, (LPCTSTR)strAppend);
	m_nCursorPos += strAppend.GetLength();
	SetCurrentFieldText(strCurrent);
	m_nCursorAnime = 0;
	Redraw();
}


BOOL CWindowLOGIN::IsTextFieldFocus(void) const
{
	return ((m_nFocusIndex == LOGINFOCUS_ACCOUNT) || (m_nFocusIndex == LOGINFOCUS_PASSWORD)) ? TRUE : FALSE;
}


BOOL CWindowLOGIN::IsInteractive(void) const
{
	return (m_bShow && m_bEnabled) ? TRUE : FALSE;
}


BOOL CWindowLOGIN::HitTest(int x, int y, RECT &rcDst, int nFocusIndex) const
{
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
}

void CWindowLOGIN::DrawBrowserControls(const RECT &rcAccount, const RECT &rcPassword, const RECT &rcCheck, const RECT &rcConnect)
{
	const BOOL bFocusAccount = (m_nFocusIndex == LOGINFOCUS_ACCOUNT);
	const BOOL bFocusPassword = (m_nFocusIndex == LOGINFOCUS_PASSWORD);
	const BOOL bFocusCheck = (m_nFocusIndex == LOGINFOCUS_SAVEPASSWORD);
	const BOOL bFocusConnect = (m_nFocusIndex == LOGINFOCUS_CONNECT);
	RECT rcCheckBox;

	// アカウント入力欄
	DrawBrowserRect(rcAccount.left - 1, rcAccount.top - 1, rcAccount.right - rcAccount.left + 2, rcAccount.bottom - rcAccount.top + 2, RGB(120, 78, 42), FALSE, RGB(120, 78, 42), TRUE);
	DrawBrowserRect(rcAccount.left, rcAccount.top, rcAccount.right - rcAccount.left, rcAccount.bottom - rcAccount.top, RGB(255, 252, 245), TRUE, RGB(240, 214, 172), TRUE);
	if (bFocusAccount) {
		DrawBrowserRect(rcAccount.left - 2, rcAccount.top - 2, rcAccount.right - rcAccount.left + 4, rcAccount.bottom - rcAccount.top + 4, RGB(255, 255, 255), FALSE, RGB(255, 196, 92), TRUE);
	}

	// パスワード入力欄
	DrawBrowserRect(rcPassword.left - 1, rcPassword.top - 1, rcPassword.right - rcPassword.left + 2, rcPassword.bottom - rcPassword.top + 2, RGB(120, 78, 42), FALSE, RGB(120, 78, 42), TRUE);
	DrawBrowserRect(rcPassword.left, rcPassword.top, rcPassword.right - rcPassword.left, rcPassword.bottom - rcPassword.top, RGB(255, 252, 245), TRUE, RGB(240, 214, 172), TRUE);
	if (bFocusPassword) {
		DrawBrowserRect(rcPassword.left - 2, rcPassword.top - 2, rcPassword.right - rcPassword.left + 4, rcPassword.bottom - rcPassword.top + 4, RGB(255, 255, 255), FALSE, RGB(255, 196, 92), TRUE);
	}

	// チェックボックス
	GetWindowLOGINCheckBoxRect(rcCheck, &rcCheckBox);
	if (bFocusCheck) {
		DrawBrowserRect(rcCheckBox.left - 2, rcCheckBox.top - 2, (rcCheckBox.right - rcCheckBox.left) + 4, (rcCheckBox.bottom - rcCheckBox.top) + 4, RGB(255, 255, 255), FALSE, RGB(255, 196, 92), TRUE);
	}
	DrawBrowserRect(rcCheckBox.left, rcCheckBox.top, rcCheckBox.right - rcCheckBox.left, rcCheckBox.bottom - rcCheckBox.top, RGB(255, 250, 240), TRUE, RGB(120, 78, 42), TRUE);
	if (m_bSavePassword) {
		DrawBrowserRect(rcCheckBox.left + 3, rcCheckBox.top + 3, 5, 5, RGB(80, 56, 24), TRUE, RGB(80, 56, 24), FALSE);
	}

	// 接続ボタン
	DrawBrowserRect(rcConnect.left, rcConnect.top, rcConnect.right - rcConnect.left, rcConnect.bottom - rcConnect.top, RGB(235, 206, 158), TRUE, RGB(120, 78, 42), TRUE);
	DrawBrowserRect(rcConnect.left + 1, rcConnect.top + 1, rcConnect.right - rcConnect.left - 2, 1, RGB(255, 245, 224), TRUE, RGB(255, 245, 224), FALSE);	// 上ハイライト
	DrawBrowserRect(rcConnect.left + 1, rcConnect.top + 1, 1, rcConnect.bottom - rcConnect.top - 2, RGB(255, 245, 224), TRUE, RGB(255, 245, 224), FALSE);	// 左ハイライト
	DrawBrowserRect(rcConnect.left + 1, rcConnect.bottom - 2, rcConnect.right - rcConnect.left - 2, 1, RGB(154, 108, 58), TRUE, RGB(154, 108, 58), FALSE);	// 下影
	DrawBrowserRect(rcConnect.right - 2, rcConnect.top + 1, 1, rcConnect.bottom - rcConnect.top - 2, RGB(154, 108, 58), TRUE, RGB(154, 108, 58), FALSE);	// 右影
	if (bFocusConnect) {
		DrawBrowserRect(rcConnect.left - 2, rcConnect.top - 2, rcConnect.right - rcConnect.left + 4, rcConnect.bottom - rcConnect.top + 4, RGB(255, 255, 255), FALSE, RGB(255, 196, 92), TRUE);
	}
}


void CWindowLOGIN::DrawTextField(HDC hDC, const RECT &rcField, LPCSTR pszText, BOOL bPassword, BOOL bFocused)
{
	CString strDraw;
	int nCursorX;

	if (hDC == NULL) {
		return;
	}

	DrawInputFrame1(rcField.left, rcField.top, rcField.right - rcField.left, rcField.bottom - rcField.top, bFocused ? 0 : 1);
	if (bFocused) {
		m_pDib->FillRect(rcField.left, rcField.top, rcField.right - rcField.left, rcField.bottom - rcField.top, RGB(255, 255, 255));
	}
	strDraw = Utf8ToTString(pszText);
	if (bPassword) {
		strDraw = _T("");
		for (int i = 0; pszText[i] != _T('\0'); ++i) {
			strDraw += _T('*');
		}
	}

	TextOut2(hDC, m_hFont, rcField.left + 2, rcField.top - 5, strDraw, RGB(0, 0, 0));
	if (bFocused && (m_nCursorAnime == 0)) {
		// カーソル位置に基づいて描画位置を計算
		int nDrawCursorPos = m_nCursorPos;
		if (nDrawCursorPos > strDraw.GetLength()) {
			nDrawCursorPos = strDraw.GetLength();
		}
		if (nDrawCursorPos == 0) {
			nCursorX = rcField.left + 2;
		} else {
			// SdlFontGetTextExtent で実測した文字列幅を使用
			int nTextWidth = 0;
			int nTextHeight = 0;
			if (SdlFontGetTextExtent((void*)m_hFont, strDraw, nDrawCursorPos, &nTextWidth, &nTextHeight)) {
				nCursorX = rcField.left + 2 + nTextWidth;
			} else {
				// フォント取得失敗時の暫定フォールバック (半角幅 7px 想定)
				nCursorX = rcField.left + 2 + nDrawCursorPos * 7;
			}
		}
		if (nCursorX > rcField.right - 8) {
			nCursorX = rcField.right - 8;
		}
		TextOut2(hDC, m_hFont, nCursorX, rcField.top - 5, _T("|"), RGB(0, 0, 0));
	}
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
	NormalizeLoginText(m_strAccount, pszText, TRUE);
	Redraw();
}


void CWindowLOGIN::SetPasswordFromBrowser(LPCSTR pszText)
{
	NormalizeLoginText(m_strPassword, pszText, FALSE);
	Redraw();
}


void CWindowLOGIN::SetSavePasswordFromBrowser(BOOL bCheck)
{
	SetCheck(bCheck);
}


void CWindowLOGIN::SubmitFromBrowser(void)
{
	OnConnect();
}
#endif

BOOL CWindowLOGIN::TimerProc(void)
{
	int nOldAnime;

	nOldAnime = m_nCursorAnime;
	CWindowBase::TimerProc();

	// カーソル点滅状態が変化し、かつテキスト入力欄にフォーカスがある場合は再描画
	if (nOldAnime != m_nCursorAnime && IsTextFieldFocus()) {
		m_dwTimeDrawStart = 0;	// 再描画を強制
		return TRUE;
	}
	return (m_dwTimeDrawStart == 0) ? TRUE : FALSE;
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
