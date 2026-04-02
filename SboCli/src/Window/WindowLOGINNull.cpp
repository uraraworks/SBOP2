/// @file WindowLOGINNull.cpp
/// @brief タイトル表示用のダミーログインUIクラス 実装ファイル
/// @author Codex
/// @date 2026/04/01

#include "stdafx.h"
#include "MgrData.h"
#include "MgrWindow.h"
#include "WindowLOGINNull.h"

CWindowLOGINNull::CWindowLOGINNull()
{
	m_nID = WINDOWTYPE_LOGIN;
	m_bInput = FALSE;
	m_bShow = FALSE;
}

CWindowLOGINNull::~CWindowLOGINNull()
{
}

void CWindowLOGINNull::Create(CMgrData *pMgrData)
{
	CWindowBase::Create(pMgrData);
}

void CWindowLOGINNull::Enable(BOOL bEnable)
{
}

void CWindowLOGINNull::SetShow(BOOL bShow)
{
	CWindowBase::SetShow(bShow);
}

void CWindowLOGINNull::Save(void)
{
}

LPCSTR CWindowLOGINNull::GetAccount(void) const
{
	return "";
}

LPCSTR CWindowLOGINNull::GetPassword(void) const
{
	return "";
}

BOOL CWindowLOGINNull::HandleKeyDown(UINT vk)
{
	UNREFERENCED_PARAMETER(vk);
	return FALSE;
}

void CWindowLOGINNull::HandleTextInput(LPCSTR pszText)
{
	UNREFERENCED_PARAMETER(pszText);
}

BOOL CWindowLOGINNull::HandleMouseLeftButtonDown(int x, int y)
{
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	return FALSE;
}
