/// @file ILoginWindow.h
/// @brief ログイン画面UIの抽象インターフェース
/// @author Codex
/// @date 2026/04/01

#pragma once

class CMgrData;

class ILoginWindow
{
public:
	virtual ~ILoginWindow() {}

	virtual void Create(CMgrData *pMgrData) = 0;
	virtual void Enable(BOOL bEnable) = 0;
	virtual void SetShow(BOOL bShow) = 0;
	virtual void Save(void) = 0;
	virtual LPCSTR GetAccount(void) const = 0;
	virtual LPCSTR GetPassword(void) const = 0;
	virtual BOOL HandleKeyDown(UINT vk) = 0;
	virtual void HandleTextInput(LPCSTR pszText) = 0;
	virtual BOOL HandleMouseLeftButtonDown(int x, int y) = 0;
};
