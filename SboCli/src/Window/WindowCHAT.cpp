/// @file WindowCHAT.cpp
/// @brief チャット情報ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/02/03
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "PacketCHAR_REQ_CHAT.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrWindow.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "WindowCHAT.h"

namespace {

enum {
	CHAT_TEXT_MAX = 100,
};

}

CWindowCHAT::CWindowCHAT()
{
	m_bInput	= TRUE;
	m_bPushEnter	= FALSE;
	m_bPushEsc	= FALSE;
	m_bTextInputActive = FALSE;
	m_nID	= WINDOWTYPE_CHAT;
	m_nType	= 0;
	m_ptViewPos.y	= SCRSIZEY - 88;
	m_sizeWindow.cx	= SCRSIZEX - (m_ptViewPos.x * 2) - 6;
	m_sizeWindow.cy	= 16 + 14 + 6 + 4;
	m_ptViewPos.x	= SCRSIZEX / 2 - m_sizeWindow.cx / 2;
	m_hWndChat	= NULL;
	m_OrgWndProcChat	= NULL;
}


CWindowCHAT::~CWindowCHAT()
{
#if !defined(__EMSCRIPTEN__)
	HIMC hImc;

	if (m_hWndChat) {
		// IMEをオフにする
		hImc = ImmGetContext(m_hWndChat);
		ImmSetOpenStatus(hImc, FALSE);
		ImmReleaseContext(m_hWndChat, hImc);

		SetWindowLong(m_hWndChat, GWL_WNDPROC, (LONG)m_OrgWndProcChat);
		m_OrgWndProcChat = NULL;
		SAFE_DESTROYWND(m_hWndChat);
	}
#else
	UpdateSDLTextInput();
#endif
}


void CWindowCHAT::Create(CMgrData *pMgrData)
{
	int nResult;

	CWindowBase::Create(pMgrData);

	m_aArrayType.push_back(0);
	nResult = m_pMgrData->GetAdminLevel();
	if (nResult == ADMINLEVEL_ALL) {
		m_aArrayType.push_back(CHATTYPE_ADMIN);
	}
	m_nType = m_pMgrData->GetChatModeBack();
	if (m_nType >= (int)m_aArrayType.size()) {
		m_nType = 0;
	}

	m_bActive = TRUE;
	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(RGB(255, 0, 255));
	m_pDib->FillRect(0, 0, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib->GetColorKey());
	MakeWindow();
}


void CWindowCHAT::Draw(PCImg32 pDst)
{
	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	m_pDib->FillRect(0, 0, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib->GetColorKey());

	// タイトル
	m_pDib->BltFrom256(8, 0, 40, 16, m_pDibSystem, 48, 96);

	// 枠
	DrawInputFrame1(8 + 6, 16 + 6, m_sizeWindow.cx - 16 - 6 - 6, 14, 0);

	// チャット種別アイコン
	m_pDib->BltFrom256(50, 0, 16, 16, m_pDibSystem, 48 + (m_aArrayType[m_nType] * 16), 112);

#if defined(__EMSCRIPTEN__)
#endif

	m_dwTimeDrawStart = timeGetTime();

Exit:
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);

#if defined(__EMSCRIPTEN__)
	DrawBrowserText(8 + 8, 16 + 4, m_strChat, RGB(0, 0, 0), 12, FALSE, RGB(10, 10, 10), FALSE);
	if (m_bActive && (m_nCursorAnime == 0)) {
		int nCursorX;

		nCursorX = 8 + 8 + m_strChat.GetLength() * 8;
		if (nCursorX > m_sizeWindow.cx - 24) {
			nCursorX = m_sizeWindow.cx - 24;
		}
		DrawBrowserText(nCursorX, 16 + 4, _T("|"), RGB(0, 0, 0), 12, FALSE, RGB(10, 10, 10), FALSE);
	}
#endif
}


void CWindowCHAT::SetActive(BOOL bActive)
{
#if !defined(__EMSCRIPTEN__)
	HWND hWnd;
#endif

	CWindowBase::SetActive(bActive);

#if defined(__EMSCRIPTEN__)
	UpdateSDLTextInput();
	Redraw();
#else
	hWnd = m_hWndChat;

	EnableWindow(hWnd, bActive);
	if (bActive == FALSE) {
		hWnd = m_hWndMain;
	}
	SetFocus(hWnd);
#endif
}


BOOL CWindowCHAT::HandleSDLKeyDown(UINT vk)
{
#if defined(__EMSCRIPTEN__)
	int nCount;

	if (!m_bActive) {
		return FALSE;
	}

	nCount = (int)m_aArrayType.size();
	switch (vk) {
	case VK_RETURN:
		SubmitChat();
		return TRUE;

	case VK_ESCAPE:
		m_pMgrData->SetChatModeBack(m_nType);
		m_bDelete = TRUE;
		UpdateSDLTextInput();
		return TRUE;

	case VK_UP:
		m_nType --;
		if (m_nType < 0) {
			m_nType = nCount - 1;
		}
		Redraw();
		return TRUE;

	case VK_DOWN:
		m_nType ++;
		if (m_nType >= nCount) {
			m_nType = 0;
		}
		Redraw();
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


void CWindowCHAT::HandleSDLTextInput(LPCSTR pszText)
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


BOOL CWindowCHAT::HandleSDLMouseLeftButtonDown(int x, int y)
{
#if defined(__EMSCRIPTEN__)
	RECT rcInput;

	SetRect(&rcInput, m_ptViewPos.x + 32 + 8 + 6, m_ptViewPos.y + 32 + 16 + 6, m_ptViewPos.x + 32 + m_sizeWindow.cx - 16 - 6, m_ptViewPos.y + 32 + 16 + 6 + 14);
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


int CWindowCHAT::GetType(void)
{
	int nRet;

	nRet = m_aArrayType[m_nType];

	return nRet;
}


void CWindowCHAT::MakeWindow(void)
{
#if defined(__EMSCRIPTEN__)
	UpdateSDLTextInput();
#else
	HINSTANCE hInstance;
	HWND hWndMain;
	HIMC hImc;

	hInstance	= m_pMgrData->GetInstance();
	hWndMain	= m_pMgrData->GetMainWindow();

	m_hWndChat = CreateWindow(_T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
			m_ptViewPos.x + 8 + 6, m_ptViewPos.y + 16 + 6, m_sizeWindow.cx - (16 + 6 + 6), 14, hWndMain, NULL, hInstance, NULL);
	m_OrgWndProcChat = (WNDPROC)GetWindowLong(m_hWndChat, GWL_WNDPROC);
	SendMessage(m_hWndChat, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
	SetWindowLong(m_hWndChat, GWL_USERDATA, (LONG)this);
	SetWindowLong(m_hWndChat, GWL_WNDPROC, (LONG)ChatWndProcEntry);

	// IMEをオンにする
	hImc = ImmGetContext(m_hWndChat);
	ImmSetOpenStatus(hImc, TRUE);
	ImmReleaseContext(m_hWndChat, hImc);

	SetFocus(m_hWndChat);
#endif
}


void CWindowCHAT::UpdateSDLTextInput(void)
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


void CWindowCHAT::DeleteBackward(void)
{
#if defined(__EMSCRIPTEN__)
	CString strTmp;

	if (m_strChat.IsEmpty()) {
		return;
	}

	strTmp = (LPCTSTR)m_strChat;
	strTmp = strTmp.Left(strTmp.GetLength() - 1);
	TrimViewString(m_strChat, (LPCTSTR)strTmp);
	Redraw();
#endif
}


void CWindowCHAT::AppendText(LPCSTR pszText)
{
#if defined(__EMSCRIPTEN__)
	CString strWide;
	CString strFiltered;
	CmyString strAppend;
	int nAllow, nLength;

	if ((pszText == NULL) || (pszText[0] == '\0')) {
		return;
	}

	nAllow = CHAT_TEXT_MAX - m_strChat.GetLength();
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
	m_strChat += (LPCTSTR)strAppend;
	Redraw();
#else
	UNREFERENCED_PARAMETER(pszText);
#endif
}


void CWindowCHAT::SubmitChat(void)
{
#if defined(__EMSCRIPTEN__)
	if (!m_strChat.IsEmpty()) {
		m_pMgrData->SetChatModeBack(m_nType);
		m_pMgrData->PostWindowMessage(WINDOWTYPE_CHAT, 0);
	} else {
		m_bDelete = TRUE;
		UpdateSDLTextInput();
	}
#endif
}


#if !defined(__EMSCRIPTEN__)
LRESULT CALLBACK CWindowCHAT::ChatWndProcEntry(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT hResult;
	PCWindowCHAT pThis;

	pThis	= (PCWindowCHAT)GetWindowLong(hWnd, GWL_USERDATA);
	hResult	= -1;

	if (pThis) {
		hResult = pThis->ChatWndProc(hWnd, message, wParam, lParam);
	}
	return hResult;
}


LRESULT CWindowCHAT::ChatWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT hResult = -1;

	switch (message) {
	case WM_CHAR:
		switch (wParam) {
		case VK_RETURN:
		case VK_ESCAPE:
			hResult = 0;
			break;
		}
		break;

	case WM_KEYDOWN:
		{
			int nCount;
			nCount = m_aArrayType.size();

			switch (wParam) {
			case VK_RETURN:
				m_bPushEnter = TRUE;
				hResult = 0;
				break;
			case VK_ESCAPE:
				m_bPushEsc = TRUE;
				hResult = 0;
				break;
			case VK_UP:
				m_nType --;
				if (m_nType < 0) {
					m_nType = nCount - 1;
				}
				m_dwTimeDrawStart = 0;
				hResult = 0;
				break;

			case VK_DOWN:
				m_nType ++;
				if (m_nType >= nCount) {
					m_nType = 0;
				}
				m_dwTimeDrawStart = 0;
				hResult = 0;
				break;
			}
		}
		break;

	case WM_KEYUP:
		switch (wParam) {
		case VK_RETURN:
			{
				TCHAR szTmp[256], szTmp2[256];

				if (m_bPushEnter == FALSE) {
					break;
				}
				GetWindowText(hWnd, szTmp, _countof(szTmp));
				if (_tcslen(szTmp) > 0) {
					ZeroMemory(szTmp2, sizeof(szTmp2));
					_tcsnccat(szTmp2, szTmp, 100);
					TrimViewString(m_strChat, szTmp2);
					m_pMgrData->SetChatModeBack(m_nType);
					m_pMgrData->PostWindowMessage(WINDOWTYPE_CHAT, 0);

				} else {
					m_bDelete = TRUE;
				}
				m_bPushEnter = FALSE;
				hResult = 0;
			}
			break;
		case VK_ESCAPE:
			if (m_bPushEsc == FALSE) {
				break;
			}
			m_bPushEsc = FALSE;
			m_bDelete  = TRUE;
			m_pMgrData->SetChatModeBack(m_nType);
			hResult = 0;
			break;
		}
		break;
	}

	if (hResult != 0) {
		hResult = CallWindowProc(m_OrgWndProcChat, hWnd, message, wParam, lParam);
	}
	return hResult;
}
#else
LRESULT CALLBACK CWindowCHAT::ChatWndProcEntry(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	return 0;
}


LRESULT CWindowCHAT::ChatWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	return 0;
}
#endif
