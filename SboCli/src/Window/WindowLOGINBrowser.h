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
	void	UpdateBrowserDom(void);
	void	HideBrowserDom(void);
	void	SetAccountFromBrowser(LPCSTR pszText);
	void	SetPasswordFromBrowser(LPCSTR pszText);
	void	SetSavePasswordFromBrowser(BOOL bCheck);
	void	SubmitFromBrowser(void);
	// ログインコード方式(docs/login-code-auth-plan.md): JS 側が /api/account/me の結果を
	// 見て、端末トークンが有効かどうかをここへ通知する。無効なまま接続要求が来たときは
	// OnConnect() 側で弾く(旧「アカウント名が空なら何もしない」判定の置き換え)。
	void	SetHasTokenFromBrowser(BOOL bHasToken);

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
	BOOL	m_bHasToken;	// 端末トークンが有効か(JS からの通知。既定は FALSE)
	int		m_nFocusIndex;

	void	MakeWindow(void);
	void	OnConnect(void);
	void	UpdateWindowSizeForToken(void);	// トークン有無に応じてウィンドウの見た目のサイズを調整
} CWindowLOGINBrowser, *PCWindowLOGINBrowser;

#endif // __EMSCRIPTEN__
