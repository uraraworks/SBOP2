/// @file WindowCHARNAME.cpp
/// @brief キャラ名入力ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/08
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "WindowCHARNAME.h"

CWindowCHARNAME::CWindowCHARNAME()
{
	m_bInput	= TRUE;
	m_bTextInputActive = FALSE;
	m_nID	= WINDOWTYPE_CHARNAME;
	m_ptViewPos.x	= 136 + 32;
	m_ptViewPos.y	= 180;
	m_sizeWindow.cx	= 16 * 2 + 8 * 15;
	m_sizeWindow.cy	= 16 * 2 + 16 * 3;
	m_hWndCharName	= NULL;
	m_OrgWndProcCharName	= NULL;
}


CWindowCHARNAME::~CWindowCHARNAME()
{
#if !defined(__EMSCRIPTEN__)
	HIMC hImc;

	if (m_hWndCharName) {
		// IMEをオフにする
		hImc = ImmGetContext(m_hWndCharName);
		ImmSetOpenStatus(hImc, FALSE);
		ImmReleaseContext(m_hWndCharName, hImc);

		SetWindowLong(m_hWndCharName, GWL_WNDPROC, (LONG)m_OrgWndProcCharName);
		m_OrgWndProcCharName = NULL;
		SAFE_DESTROYWND(m_hWndCharName);
	}
#else
	UpdateSDLTextInput();
#endif
}


void CWindowCHARNAME::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);

	m_bActive = TRUE;
	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);
	MakeWindow();
}


void CWindowCHARNAME::Draw(PCImg32 pDst)
{
	HDC hDC = NULL;
	HFONT hFontOld = NULL;

	if (m_dwTimeDrawStart == 0) {
		DrawFrame();
		DrawInputFrame1(16, 48, 8 * MAXLEN_CHARNAME, 14, 0);

#if !defined(__EMSCRIPTEN__)
	hDC	= m_pDib->Lock();
	hFontOld	= (HFONT)SelectObject(hDC, m_hFont14);
	SetBkMode(hDC, TRANSPARENT);

	TextOut4(hDC, 24, 16, _T("キャラクター名"), RGB(255, 127, 53));
	TextOut2(hDC, 18, 46, m_strName, RGB(0, 0, 0));
	if (m_bActive && (m_nCursorAnime == 0)) {
		int nCursorX;

		nCursorX = 18 + m_strName.GetLength() * 8;
		if (nCursorX > 16 + 8 * MAXLEN_CHARNAME - 6) {
			nCursorX = 16 + 8 * MAXLEN_CHARNAME - 6;
		}
		TextOut2(hDC, nCursorX, 46, _T("|"), RGB(0, 0, 0));
	}

	SelectObject(hDC, hFontOld);
	m_pDib->Unlock();
#endif

		m_dwTimeDrawStart = timeGetTime();
	}

Exit:
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);

#if defined(__EMSCRIPTEN__)
	DrawBrowserText(24, 16, _T("キャラクター名"), RGB(255, 255, 255), 14, TRUE, RGB(255, 127, 53), TRUE);
	DrawBrowserText(18, 46, m_strName, RGB(0, 0, 0), 14, FALSE, RGB(10, 10, 10), FALSE);
	if (m_bActive && (m_nCursorAnime == 0)) {
		int nCursorX;

		nCursorX = 18 + m_strName.GetLength() * 8;
		if (nCursorX > 16 + 8 * MAXLEN_CHARNAME - 6) {
			nCursorX = 16 + 8 * MAXLEN_CHARNAME - 6;
		}
		DrawBrowserText(nCursorX, 46, _T("|"), RGB(0, 0, 0), 14, FALSE, RGB(10, 10, 10), FALSE);
	}
#endif
}


void CWindowCHARNAME::SetActive(BOOL bActive)
{
#if !defined(__EMSCRIPTEN__)
	HWND hWnd;
#endif

	CWindowBase::SetActive(bActive);

#if defined(__EMSCRIPTEN__)
	UpdateSDLTextInput();
	Redraw();
#else
	hWnd = m_hWndCharName;

	EnableWindow(hWnd, bActive);
	if (bActive == FALSE) {
		hWnd = m_hWndMain;
	}
	SetFocus(hWnd);
#endif
}


BOOL CWindowCHARNAME::HandleSDLKeyDown(UINT vk)
{
#if defined(__EMSCRIPTEN__)
	if (!m_bActive) {
		return FALSE;
	}

	switch (vk) {
	case VK_RETURN:
		SubmitCharName();
		return TRUE;

	case VK_ESCAPE:
		m_bDelete = TRUE;
		UpdateSDLTextInput();
		return TRUE;

	case VK_BACK:
		DeleteBackward();
		return TRUE;
	}
#else
	UNREFERENCED_PARAMETER(vk);
#endif
	return FALSE;
}


void CWindowCHARNAME::HandleSDLTextInput(LPCSTR pszText)
{
#if defined(__EMSCRIPTEN__)
	if (!m_bActive) {
		return;
	}
	AppendText(pszText);
#else
	UNREFERENCED_PARAMETER(pszText);
#endif
}


BOOL CWindowCHARNAME::HandleSDLMouseLeftButtonDown(int x, int y)
{
#if defined(__EMSCRIPTEN__)
	RECT rcInput;

	SetRect(&rcInput, m_ptViewPos.x + 32 + 16, m_ptViewPos.y + 32 + 48, m_ptViewPos.x + 32 + 16 + 8 * MAXLEN_CHARNAME, m_ptViewPos.y + 32 + 48 + 14);
	if ((x >= rcInput.left) && (x < rcInput.right) && (y >= rcInput.top) && (y < rcInput.bottom)) {
		SetActive(TRUE);
		return TRUE;
	}
#else
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
#endif
	return FALSE;
}


void CWindowCHARNAME::MakeWindow(void)
{
#if defined(__EMSCRIPTEN__)
	UpdateSDLTextInput();
#else
	HINSTANCE hInstance;
	HWND hWndMain;
	HIMC hImc;

	hInstance	= m_pMgrData->GetInstance();
	hWndMain	= m_pMgrData->GetMainWindow();

	// キャラ名入力欄
	m_hWndCharName = CreateWindow(_T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
			m_ptViewPos.x + 16, m_ptViewPos.y + 48, 8 * MAXLEN_CHARNAME, 14, hWndMain, NULL, hInstance, NULL);
	m_OrgWndProcCharName = (WNDPROC)GetWindowLong(m_hWndCharName, GWL_WNDPROC);
	SendMessage(m_hWndCharName, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
	SetWindowLong(m_hWndCharName, GWL_USERDATA, (LONG)this);
	SetWindowLong(m_hWndCharName, GWL_WNDPROC, (LONG)CharNameWndProc);

	// IMEをオンにする
	hImc = ImmGetContext(m_hWndCharName);
	ImmSetOpenStatus(hImc, TRUE);
	ImmReleaseContext(m_hWndCharName, hImc);

	SetFocus(m_hWndCharName);
#endif
}


void CWindowCHARNAME::UpdateSDLTextInput(void)
{
#if defined(__EMSCRIPTEN__)
	if (m_bActive && (!m_bDelete)) {
		SDL_StartTextInput();
		m_bTextInputActive = TRUE;
	} else if (m_bTextInputActive) {
		SDL_StopTextInput();
		m_bTextInputActive = FALSE;
	}
#endif
}


void CWindowCHARNAME::DeleteBackward(void)
{
#if defined(__EMSCRIPTEN__)
	CString strTmp;

	if (m_strName.IsEmpty()) {
		return;
	}

	strTmp = (LPCTSTR)m_strName;
	strTmp = strTmp.Left(strTmp.GetLength() - 1);
	TrimViewString(m_strName, (LPCTSTR)strTmp);
	Redraw();
#endif
}


void CWindowCHARNAME::AppendText(LPCSTR pszText)
{
#if defined(__EMSCRIPTEN__)
	CString strWide;
	CString strFiltered;
	CmyString strAppend;
	int nAllow, nLength;

	if ((pszText == NULL) || (pszText[0] == '\0')) {
		return;
	}

	nAllow = MAXLEN_CHARNAME - m_strName.GetLength();
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
		strFiltered.AppendChar(ch);
		if (strFiltered.GetLength() >= nAllow) {
			break;
		}
	}
	if (strFiltered.IsEmpty()) {
		return;
	}

	TrimViewString(strAppend, (LPCTSTR)strFiltered);
	m_strName += (LPCTSTR)strAppend;
	Redraw();
#else
	UNREFERENCED_PARAMETER(pszText);
#endif
}


void CWindowCHARNAME::SubmitCharName(void)
{
#if defined(__EMSCRIPTEN__)
	CString strTmp;

	if (m_strName.IsEmpty()) {
		return;
	}

	strTmp = (LPCTSTR)m_strName;
	if (strTmp.GetLength() > MAXLEN_CHARNAME) {
		strTmp = strTmp.Left(MAXLEN_CHARNAME);
		TrimViewString(m_strName, (LPCTSTR)strTmp);
	}
	m_pMgrData->PostWindowMessage(WINDOWTYPE_CHARNAME, 0);
#endif
}


#if !defined(__EMSCRIPTEN__)
LRESULT CALLBACK CWindowCHARNAME::CharNameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int nCount;
	TCHAR szTmp[128];
	LRESULT hResult;
	HIMC hImc;
	PCWindowCHARNAME pThis;

	pThis	= (PCWindowCHARNAME)GetWindowLong(hWnd, GWL_USERDATA);
	hResult	= -1;

	switch (message) {
	case WM_CHAR:
		if (wParam == VK_RETURN) {
			GetWindowText(hWnd, szTmp, _countof(szTmp));
			nCount = static_cast<int>(_tcslen(szTmp));
			if (nCount > 0) {
				if (nCount >= MAXLEN_CHARNAME) {
#ifdef _UNICODE
					szTmp[MAXLEN_CHARNAME] = _T('\0');
#else
					if (IsDBCSLeadByte((BYTE)szTmp[MAXLEN_CHARNAME - 1])) {
						szTmp[MAXLEN_CHARNAME - 1] = 0;
					} else {
						szTmp[MAXLEN_CHARNAME] = 0;
					}
#endif
				}
				pThis->m_strName = szTmp;
				TrimViewString(pThis->m_strName, szTmp);
				pThis->m_pMgrData->PostWindowMessage(WINDOWTYPE_CHARNAME, 0);
			}
			// IMEをオフにする
			hImc = ImmGetContext(pThis->m_hWndCharName);
			ImmSetOpenStatus(hImc, FALSE);
			ImmReleaseContext(pThis->m_hWndCharName, hImc);
			hResult = 0;

		} else if (wParam == VK_ESCAPE) {
			pThis->m_bDelete = TRUE;
			hResult = 0;
		}
		break;
	}

	if (hResult != 0) {
		hResult = CallWindowProc(pThis->m_OrgWndProcCharName, hWnd, message, wParam, lParam);
	}
	return hResult;
}
#else
LRESULT CALLBACK CWindowCHARNAME::CharNameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	return 0;
}
#endif
