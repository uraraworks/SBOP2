/// @file WindowLOGINNull.h
/// @brief タイトル表示用のダミーログインUIクラス 定義ファイル
/// @author Codex
/// @date 2026/04/01

#pragma once

#include "ILoginWindow.h"
#include "WindowBase.h"

class CWindowLOGINNull : public CWindowBase, public ILoginWindow
{
public:
			CWindowLOGINNull();
	virtual ~CWindowLOGINNull();

	virtual void	Create(CMgrData *pMgrData);
	virtual void	Enable(BOOL bEnable);
	virtual void	SetShow(BOOL bShow);
	virtual void	Save(void);
	virtual ILoginWindow	*GetLoginWindowInterface(void)	{ return this; }
	virtual LPCSTR	GetAccount(void) const;
	virtual LPCSTR	GetPassword(void) const;
	virtual BOOL	HandleKeyDown(UINT vk);
	virtual void	HandleTextInput(LPCSTR pszText);
	virtual BOOL	HandleMouseLeftButtonDown(int x, int y);
};
