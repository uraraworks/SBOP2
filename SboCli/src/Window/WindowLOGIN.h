/// @file WindowLOGIN.h
/// @brief ログイン情報ウィンドウクラス 定義ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/03
/// @copyright Copyright(C)URARA-works 2006

#pragma once

#include "ILoginWindow.h"
#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

typedef class CWindowLOGIN : public CWindowBase, public ILoginWindow
{
public:
			CWindowLOGIN();	// コンストラクタ
	virtual ~CWindowLOGIN();	// デストラクタ

	virtual void	Create(CMgrData *pMgrData);	// 作成

	void	Draw(CImg32 *pDst);	// 描画
	virtual void	Enable(BOOL bEnable);	// 入力許可・禁止
	virtual void	SetShow(BOOL bShow);	// 表示するか設定
	virtual void	Save(void);	// アカウントとパスワードを保存
	virtual ILoginWindow	*GetLoginWindowInterface(void)	{ return this; }
	void	SetCheck(BOOL bCheck);	// パスワードを保存するチェックの設定
	BOOL	GetCheck(void);	// パスワードを保存するチェックの取得
	virtual LPCSTR	GetAccount(void) const	{ return m_strAccount;	}	// 入力されたアカウントを取得
	virtual LPCSTR	GetPassword(void) const	{ return m_strPassword;	}	// 入力されたパスワードを取得
	virtual BOOL	HandleSDLKeyDown(UINT vk);
	virtual void	HandleSDLTextInput(LPCSTR pszText);
	virtual BOOL	HandleSDLMouseLeftButtonDown(int x, int y);
	virtual BOOL	HandleKeyDown(UINT vk);
	virtual void	HandleTextInput(LPCSTR pszText);
	virtual BOOL	HandleMouseLeftButtonDown(int x, int y);
	void	SetFocusIndex(int nIndex);	// フォーカスを設定
#if defined(__EMSCRIPTEN__)
	void	UpdateBrowserDom(const RECT &rcAccount, const RECT &rcPassword, const RECT &rcCheck, const RECT &rcConnect);	// browser用DOMを更新
	void	HideBrowserDom(void);	// browser用DOMを非表示
	void	SetAccountFromBrowser(LPCSTR pszText);	// browser入力欄からアカウント反映
	void	SetPasswordFromBrowser(LPCSTR pszText);	// browser入力欄からパスワード反映
	void	SetSavePasswordFromBrowser(BOOL bCheck);	// browserチェック状態反映
	void	SubmitFromBrowser(void);	// browser接続実行
#endif
private:
	void	MakeWindow(void);	// ウィンドウ作成
	void	UpdateSDLTextInput(void);	// SDLテキスト入力状態を更新
	void	MoveFocus(int nStep);	// フォーカスを移動
	void	DeleteBackward(void);	// 1文字削除
	void	AppendText(LPCSTR pszText);	// テキスト追記
	BOOL	IsTextFieldFocus(void) const;	// テキスト入力欄が選択中か
	BOOL	IsInteractive(void) const;	// 操作可能か
	BOOL	HitTest(int x, int y, RECT &rcDst, int nFocusIndex) const;	// 当たり判定
	void	DrawTextField(HDC hDC, const RECT &rcField, LPCSTR pszText, BOOL bPassword, BOOL bFocused);	// 入力欄描画
	void	OnConnect(void);	// 接続ボタンハンドラ
	friend class CMainFrame;


public:
	CmyString	m_strAccount,	// 入力されたアカウント
				m_strPassword;	// 入力されたパスワード


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
} CWindowLOGIN, *PCWindowLOGIN;
