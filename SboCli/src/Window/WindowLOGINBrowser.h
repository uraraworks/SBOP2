/// @file WindowLOGINBrowser.h
/// @brief ログイン情報ウィンドウクラス（Emscripten DOM overlay 版）定義ファイル

#pragma once

#ifdef __EMSCRIPTEN__

#include "ILoginWindow.h"
#include "WindowBase.h"

class CImg32;
class CMgrData;

typedef class CWindowLOGINBrowser : public CWindowBase, public ILoginWindow
{
public:
			CWindowLOGINBrowser();
	virtual ~CWindowLOGINBrowser();

	virtual void	Create(CMgrData *pMgrData);

	void	Draw(CImg32 *pDst);
	virtual void	Enable(BOOL bEnable);
	virtual void	SetShow(BOOL bShow);
	virtual void	Save(void);
	virtual ILoginWindow	*GetLoginWindowInterface(void)	{ return this; }
	virtual LPCSTR	GetAccount(void) const	{ return m_strAccount; }
	virtual LPCSTR	GetPassword(void) const	{ return m_strPassword; }
	virtual BOOL	HandleKeyDown(UINT vk)	{ return FALSE; }
	virtual void	HandleTextInput(LPCSTR pszText)	{}
	virtual BOOL	HandleMouseLeftButtonDown(int x, int y)	{ return FALSE; }
	virtual BOOL	TimerProc(void);

	void	SetFocusIndex(int nIndex);
	void	UpdateBrowserDom(const RECT &rcAccount, const RECT &rcPassword, const RECT &rcCheck, const RECT &rcConnect);
	void	HideBrowserDom(void);
	void	SetAccountFromBrowser(LPCSTR pszText);
	void	SetPasswordFromBrowser(LPCSTR pszText);
	void	SetSavePasswordFromBrowser(BOOL bCheck);
	void	SubmitFromBrowser(void);

public:
	CmyString	m_strAccount,
				m_strPassword;

private:
	enum {
		LOGINFOCUS_ACCOUNT = 0,
		LOGINFOCUS_PASSWORD,
		LOGINFOCUS_SAVEPASSWORD,
		LOGINFOCUS_CONNECT,
		LOGINFOCUS_MAX
	};
	BOOL	m_bEnabled;
	BOOL	m_bSavePassword;
	int		m_nFocusIndex;

	void	MakeWindow(void);
	void	OnConnect(void);
} CWindowLOGINBrowser, *PCWindowLOGINBrowser;

#endif // __EMSCRIPTEN__
