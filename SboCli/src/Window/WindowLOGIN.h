/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:WindowLOGIN.h												 */
/* 内容			:ログイン情報ウィンドウクラス 定義ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/03													 */
/* ========================================================================= */

#pragma once

#include "WindowBase.h"

class CImg32;
class CMgrData;
class CMgrGrpData;

/* ========================================================================= */
/* クラス宣言																 */
/* ========================================================================= */

typedef class CWindowLOGIN : public CWindowBase
{
public:
			CWindowLOGIN();									/* コンストラクタ */
	virtual ~CWindowLOGIN();								/* デストラクタ */

	void	Create		(CMgrData *pMgrData);						/* 作成 */

	void	Draw		(CImg32 *pDst);								/* 描画 */
	void	Enable		(BOOL bEnable);								/* 入力許可・禁止 */
	void	SetShow		(BOOL bShow);								/* 表示するか設定 */
	void	Save		(void);										/* アカウントとパスワードを保存 */
	void	SetCheck	(BOOL bCheck);								/* パスワードを保存するチェックの設定 */
	BOOL	GetCheck	(void);										/* パスワードを保存するチェックの取得 */
	LPCSTR	GetAccount	(void)	{ return m_strAccount;	}			/* 入力されたアカウントを取得 */
	LPCSTR	GetPassword	(void)	{ return m_strPassword;	}			/* 入力されたパスワードを取得 */


private:
	void	MakeWindow	(void);										/* ウィンドウ作成 */
	static LRESULT CALLBACK AccountWndProc		(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	/* アカウント入力欄プロシージャ */
	static LRESULT CALLBACK PasswordWndProc		(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	/* パスワード入力欄プロシージャ */
	static LRESULT CALLBACK SavePasswordWndProc	(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	/* パスワードを記録するチェックプロシージャ */
	static LRESULT CALLBACK ConnectWndProc		(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	/* 接続ボタンプロシージャ */
	void	OnConnect	(void);										/* 接続ボタンハンドラ */


public:
	CmyString	m_strAccount,				/* 入力されたアカウント */
				m_strPassword;				/* 入力されたパスワード */


private:
	HWND	m_hWndAccount,					/* アカウント入力欄 */
			m_hWndPassword,					/* パスワード入力欄 */
			m_hWndSavePassword,				/* パスワードを記録するチェック */
			m_hWndConnect;					/* 接続ボタン */

	WNDPROC	m_OrgWndProcAccount,			/* アカウント入力欄の元ウィンドウプロシージャ */
			m_OrgWndProcPassword,			/* パスワード入力欄の元ウィンドウプロシージャ */
			m_OrgWndProcSavePassword,		/* パスワードを記録するチェックの元ウィンドウプロシージャ */
			m_OrgWndProcConnect;			/* 接続ボタンの元ウィンドウプロシージャ */
} CWindowLOGIN, *PCWindowLOGIN;

/* Copyright(C)URARA-works 2006 */
