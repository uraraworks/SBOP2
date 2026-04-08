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
}


CWindowCHAT::~CWindowCHAT()
{
	UpdateSDLTextInput();
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
	HDC hDC;

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

	hDC = m_pDib->Lock();
	TextOut2(hDC, m_hFont14, 8 + 8, 16 + 4, m_strChat, RGB(0, 0, 0));
	if (m_bActive && (m_nCursorAnime == 0)) {
		int nCursorX;

		nCursorX = 8 + 8 + m_strChat.GetLength() * 8;
		if (nCursorX > m_sizeWindow.cx - 24) {
			nCursorX = m_sizeWindow.cx - 24;
		}
		TextOut2(hDC, m_hFont14, nCursorX, 16 + 4, _T("|"), RGB(0, 0, 0));
	}
	m_pDib->Unlock();

	m_dwTimeDrawStart = timeGetTime();

Exit:
	pDst->Blt(m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


void CWindowCHAT::SetActive(BOOL bActive)
{
	CWindowBase::SetActive(bActive);
	UpdateSDLTextInput();
	Redraw();
}


BOOL CWindowCHAT::HandleSDLKeyDown(UINT vk)
{
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
	return FALSE;
}


void CWindowCHAT::HandleSDLTextInput(LPCSTR pszText)
{
	if (!m_bActive) {
		return;
	}
	AppendText(pszText);
}


BOOL CWindowCHAT::HandleSDLMouseLeftButtonDown(int x, int y)
{
	RECT rcInput;

	SetRect(&rcInput, m_ptViewPos.x + 32 + 8 + 6, m_ptViewPos.y + 32 + 16 + 6, m_ptViewPos.x + 32 + m_sizeWindow.cx - 16 - 6, m_ptViewPos.y + 32 + 16 + 6 + 14);
	if ((x >= rcInput.left) && (x < rcInput.right) && (y >= rcInput.top) && (y < rcInput.bottom)) {
		SetActive(TRUE);
		return TRUE;
	}
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
	UpdateSDLTextInput();
}


void CWindowCHAT::UpdateSDLTextInput(void)
{
	if (m_bActive && (!m_bDelete)) {
		SDL_StartTextInput();
		m_bTextInputActive = TRUE;
	} else if (m_bTextInputActive) {
		SDL_StopTextInput();
		m_bTextInputActive = FALSE;
	}
}


void CWindowCHAT::DeleteBackward(void)
{
	CString strTmp;

	if (m_strChat.IsEmpty()) {
		return;
	}

	strTmp = (LPCTSTR)m_strChat;
	strTmp = strTmp.Left(strTmp.GetLength() - 1);
	TrimViewString(m_strChat, (LPCTSTR)strTmp);
	Redraw();
}


void CWindowCHAT::AppendText(LPCSTR pszText)
{
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
}


void CWindowCHAT::SubmitChat(void)
{
	if (!m_strChat.IsEmpty()) {
		m_pMgrData->SetChatModeBack(m_nType);
		m_pMgrData->PostWindowMessage(WINDOWTYPE_CHAT, 0);
	} else {
		m_pMgrData->SetChatModeBack(m_nType);
		m_bDelete = TRUE;
		UpdateSDLTextInput();
	}
}
