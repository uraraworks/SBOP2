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
#include "TextRenderer.h"

CWindowCHARNAME::CWindowCHARNAME()
{
	m_bInput	= TRUE;
	m_bTextInputActive = FALSE;
	m_nID	= WINDOWTYPE_CHARNAME;
	m_ptViewPos.x	= 136 + 32;
	m_ptViewPos.y	= 180;
	m_sizeWindow.cx	= 16 * 2 + 8 * 15;
	m_sizeWindow.cy	= 16 * 2 + 16 * 3;
}


CWindowCHARNAME::~CWindowCHARNAME()
{
	UpdateSDLTextInput();
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
	if (m_dwTimeDrawStart == 0) {
		DrawFrame();
		DrawInputFrame1(16, 48, 8 * MAXLEN_CHARNAME, 14, 0);

		TextOut4(m_pDib, FONTID_PGOTHIC_14_BOLD, 24, 16, _T("キャラクター名"), RGB(255, 127, 53));
		TextOut2(m_pDib, FONTID_PGOTHIC_14_BOLD, 18, 46, m_strName, RGB(0, 0, 0));
		if (m_bActive && (m_nCursorAnime == 0)) {
			int nCursorX;

			nCursorX = 18 + m_strName.GetLength() * 8;
			if (nCursorX > 16 + 8 * MAXLEN_CHARNAME - 6) {
				nCursorX = 16 + 8 * MAXLEN_CHARNAME - 6;
			}
			TextOut2(m_pDib, FONTID_PGOTHIC_14_BOLD, nCursorX, 46, _T("|"), RGB(0, 0, 0));
		}

		m_dwTimeDrawStart = timeGetTime();
	}

Exit:
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


void CWindowCHARNAME::SetActive(BOOL bActive)
{
	CWindowBase::SetActive(bActive);
	UpdateSDLTextInput();
	Redraw();
}


BOOL CWindowCHARNAME::HandleSDLKeyDown(UINT vk)
{
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
	return FALSE;
}


void CWindowCHARNAME::HandleSDLTextInput(LPCSTR pszText)
{
	if (!m_bActive) {
		return;
	}
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
