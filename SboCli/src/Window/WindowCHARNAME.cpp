/// @file WindowCHARNAME.cpp
/// @brief キャラ名入力ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/08
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrKeyInput.h"
#include "MgrWindow.h"
#include "Platform/SdlFont.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "WindowCHARNAME.h"
#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif

namespace {

#if defined(__EMSCRIPTEN__)
static CWindowCHARNAME *g_pBrowserCharNameWindow = NULL;
#endif

static int GetFontTextWidthCN(HFONT hFont, LPCTSTR pszText)
{
	int nWidth, nHeight, nLen;

	if (pszText == NULL) {
		return 0;
	}
	nLen = (int)_tcslen(pszText);
	if (nLen <= 0) {
		return 0;
	}
	nWidth = 0;
	nHeight = 0;
	if (SdlFontGetTextExtent((void*)hFont, pszText, nLen, &nWidth, &nHeight)) {
		return nWidth;
	}
	return nLen * 8;
}

}

#if defined(__EMSCRIPTEN__)
extern "C" {
EMSCRIPTEN_KEEPALIVE void SBOP2_BrowserCharNameSetComposition(const char *pszText)
{
	if (g_pBrowserCharNameWindow != NULL) {
		g_pBrowserCharNameWindow->SetCompositionTextFromBrowser(pszText);
	}
}

EMSCRIPTEN_KEEPALIVE void SBOP2_BrowserCharNameCommitText(const char *pszText)
{
	if (g_pBrowserCharNameWindow != NULL) {
		g_pBrowserCharNameWindow->CommitTextFromBrowser(pszText);
	}
}
}
#endif

CWindowCHARNAME::CWindowCHARNAME()
{
	m_bInput	= TRUE;
	m_bTextInputActive = FALSE;
	m_dwSuppressSubmitUntil = 0;
	m_nID	= WINDOWTYPE_CHARNAME;
	m_ptViewPos.x	= 136 + 32;
	m_ptViewPos.y	= 180;
	m_sizeWindow.cx	= 16 * 2 + 8 * 15;
	m_sizeWindow.cy	= 16 * 2 + 16 * 3;
}


CWindowCHARNAME::~CWindowCHARNAME()
{
#if defined(__EMSCRIPTEN__)
	if (g_pBrowserCharNameWindow == this) {
		g_pBrowserCharNameWindow = NULL;
	}
#endif
	UpdateSDLTextInput();
}


void CWindowCHARNAME::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);
#if defined(__EMSCRIPTEN__)
	g_pBrowserCharNameWindow = this;
#endif

	m_bActive = TRUE;
	m_pDib->Create(m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey(0);
	MakeWindow();
}


void CWindowCHARNAME::Draw(PCImg32 pDst)
{
	HDC hDC = NULL;

	if (m_dwTimeDrawStart == 0) {
		DrawFrame();
		DrawInputFrame1(16, 48, 8 * MAXLEN_CHARNAME, 14, 0);

		// 入力欄内側を白で塗りつぶす（ブラウザ版で黒下地が透けて黒文字が見えなくなる対策）
		m_pDib->FillRect(16, 48, 8 * MAXLEN_CHARNAME, 14, RGB(255, 255, 255));

		hDC	= m_pDib->Lock();

		TextOut4(hDC, m_hFont14, 24, 16, _T("キャラクター名"), RGB(255, 127, 53));
		TextOut2(hDC, m_hFont14, 18, 46, m_strName, RGB(0, 0, 0));
		if (!m_strComposition.IsEmpty()) {
			int nCompositionX;

			nCompositionX = 18 + GetFontTextWidthCN(m_hFont14, m_strName);
			if (nCompositionX > 16 + 8 * MAXLEN_CHARNAME - 6) {
				nCompositionX = 16 + 8 * MAXLEN_CHARNAME - 6;
			}
			TextOut2(hDC, m_hFont14, nCompositionX, 46, m_strComposition, RGB(40, 80, 180));
		}
		if (m_bActive && (m_nCursorAnime == 0)) {
			int nCursorX;
			CString strCursorBase;

			strCursorBase = (LPCTSTR)m_strName;
			strCursorBase += (LPCTSTR)m_strComposition;
			nCursorX = 18 + GetFontTextWidthCN(m_hFont14, strCursorBase);
			if (nCursorX > 16 + 8 * MAXLEN_CHARNAME - 6) {
				nCursorX = 16 + 8 * MAXLEN_CHARNAME - 6;
			}
			TextOut2(hDC, m_hFont14, nCursorX, 46, _T("|"), RGB(0, 0, 0));
		}

		m_pDib->Unlock();

		m_dwTimeDrawStart = timeGetTime();
	}

Exit:
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


void CWindowCHARNAME::SetActive(BOOL bActive)
{
	CWindowBase::SetActive(bActive);
#if defined(__EMSCRIPTEN__)
	if (bActive && (!m_bDelete)) {
		g_pBrowserCharNameWindow = this;
	} else if (g_pBrowserCharNameWindow == this) {
		g_pBrowserCharNameWindow = NULL;
	}
#endif
	UpdateSDLTextInput();
	Redraw();
}


BOOL CWindowCHARNAME::HandleSDLKeyDown(UINT vk)
{
	if (!m_bActive) {
		return FALSE;
	}

	// IME 変換中はキーを IME 側に取られているので、確定/取消以外は無視する
	if (!m_strComposition.IsEmpty()) {
		switch (vk) {
		case VK_RETURN:
		case VK_BACK:
		case VK_UP:
		case VK_DOWN:
			return TRUE;

		case VK_ESCAPE:
			m_strComposition.Empty();
			Redraw();
			return TRUE;
		}
	}

	switch (vk) {
	case VK_RETURN:
		// browser IME 確定直後の Enter で名前が即送信されないよう抑制期間を見る
		if ((m_dwSuppressSubmitUntil != 0) && (SDL_GetTicks() < m_dwSuppressSubmitUntil)) {
			return TRUE;
		}
		m_dwSuppressSubmitUntil = 0;
		SubmitCharName();
		return TRUE;

	case VK_ESCAPE:
		m_bDelete = TRUE;
		m_strComposition.Empty();
		UpdateSDLTextInput();
		m_pMgrData->GetMgrKeyInput()->Reset();
		return TRUE;

	case VK_BACK:
		DeleteBackward();
		return TRUE;
	}
	return FALSE;
}


void CWindowCHARNAME::HandleSDLTextInput(LPCSTR pszText)
{
	if (!m_bActive) {
		return;
	}
	m_strComposition.Empty();
	AppendText(pszText);
}


void CWindowCHARNAME::HandleSDLTextEditing(LPCSTR pszText)
{
	if (!m_bActive) {
		return;
	}
	SetCompositionText(pszText);
}


void CWindowCHARNAME::SetCompositionTextFromBrowser(LPCSTR pszText)
{
	if (!m_bActive) {
		return;
	}
	SetCompositionText(pszText);
}


void CWindowCHARNAME::CommitTextFromBrowser(LPCSTR pszText)
{
	if (!m_bActive) {
		return;
	}
	m_strComposition.Empty();
	m_dwSuppressSubmitUntil = SDL_GetTicks() + 250;
	AppendText(pszText);
}


BOOL CWindowCHARNAME::HandleSDLMouseLeftButtonDown(int x, int y)
{
	RECT rcInput;

	SetRect(&rcInput, m_ptViewPos.x + 32 + 16, m_ptViewPos.y + 32 + 48, m_ptViewPos.x + 32 + 16 + 8 * MAXLEN_CHARNAME, m_ptViewPos.y + 32 + 48 + 14);
	if ((x >= rcInput.left) && (x < rcInput.right) && (y >= rcInput.top) && (y < rcInput.bottom)) {
		SetActive(TRUE);
		return TRUE;
	}
	return FALSE;
}


void CWindowCHARNAME::MakeWindow(void)
{
	UpdateSDLTextInput();
}


void CWindowCHARNAME::UpdateSDLTextInput(void)
{
	if (m_bActive && (!m_bDelete)) {
		SDL_StartTextInput();
		m_bTextInputActive = TRUE;
	} else if (m_bTextInputActive) {
		SDL_StopTextInput();
		m_bTextInputActive = FALSE;
	}
}


void CWindowCHARNAME::DeleteBackward(void)
{
	CString strTmp;

	if (m_strName.IsEmpty()) {
		return;
	}

	strTmp = (LPCTSTR)m_strName;
	strTmp = strTmp.Left(strTmp.GetLength() - 1);
	TrimViewString(m_strName, (LPCTSTR)strTmp);
	Redraw();
}


void CWindowCHARNAME::AppendText(LPCSTR pszText)
{
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
}


void CWindowCHARNAME::SetCompositionText(LPCSTR pszText)
{
	CmyString strComposition;
	CString strWide;

	m_strComposition.Empty();
	if ((pszText != NULL) && (pszText[0] != '\0')) {
		strWide = Utf8ToTString(pszText);
		TrimViewString(strComposition, (LPCTSTR)strWide);
		m_strComposition = strComposition;
	}
	Redraw();
}


void CWindowCHARNAME::SubmitCharName(void)
{
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
}
