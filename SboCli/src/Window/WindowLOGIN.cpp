/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:WindowLOGIN.cpp											 */
/* 内容			:ログイン情報ウィンドウクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrWindow.h"
#include "MgrSound.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "WindowLOGIN.h"

/* ========================================================================= */
/* 関数名	:CWindowLOGIN::CWindowLOGIN										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2005/05/06														 */
/* ========================================================================= */

CWindowLOGIN::CWindowLOGIN()
{
	m_nID						= WINDOWTYPE_LOGIN;
	m_sizeWindow.cx				= 16 * 15;
	m_sizeWindow.cy				= 16 * 6;
	m_ptViewPos.x				= SCRSIZEX / 2 - m_sizeWindow.cx / 2;
	m_ptViewPos.y				= 280;
	m_hWndAccount				= NULL;
	m_hWndPassword				= NULL;
	m_hWndSavePassword			= NULL;
	m_hWndConnect				= NULL;
	m_OrgWndProcAccount			= NULL;
	m_OrgWndProcPassword		= NULL;
	m_OrgWndProcSavePassword	= NULL;
	m_OrgWndProcConnect			= NULL;
}


/* ========================================================================= */
/* 関数名	:CWindowLOGIN::~CWindowLOGIN									 */
/* 内容		:デストラクタ													 */
/* 日付		:2005/05/06														 */
/* ========================================================================= */

CWindowLOGIN::~CWindowLOGIN()
{
	if (m_hWndAccount) {
		SetWindowLong (m_hWndAccount, GWL_WNDPROC, (LONG)m_OrgWndProcAccount);
		m_OrgWndProcAccount = NULL;
		SAFE_DESTROYWND (m_hWndAccount);
	}
	if (m_hWndPassword) {
		SetWindowLong (m_hWndPassword, GWL_WNDPROC, (LONG)m_OrgWndProcPassword);
		m_OrgWndProcPassword = NULL;
		SAFE_DESTROYWND (m_hWndPassword);
	}
	if (m_hWndSavePassword) {
		SetWindowLong (m_hWndSavePassword, GWL_WNDPROC, (LONG)m_OrgWndProcSavePassword);
		m_OrgWndProcPassword = NULL;
		SAFE_DESTROYWND (m_hWndSavePassword);
	}
	if (m_hWndConnect) {
		SetWindowLong (m_hWndConnect, GWL_WNDPROC, (LONG)m_OrgWndProcConnect);
		m_OrgWndProcConnect = NULL;
		SAFE_DESTROYWND (m_hWndConnect);
	}
}


/* ========================================================================= */
/* 関数名	:CWindowLOGIN::Create											 */
/* 内容		:作成															 */
/* 日付		:2006/11/03														 */
/* ========================================================================= */

void CWindowLOGIN::Create(CMgrData *pMgrData)
{
	CWindowBase::Create (pMgrData);

	m_pDib->Create (m_sizeWindow.cx, m_sizeWindow.cy);
	m_pDib->SetColorKey (0);
	MakeWindow ();
}


/* ========================================================================= */
/* 関数名	:CWindowLOGIN::Draw												 */
/* 内容		:描画															 */
/* 日付		:2005/05/06														 */
/* ========================================================================= */

void CWindowLOGIN::Draw(PCImg32 pDst)
{
	HDC hDC;
	HFONT hFontOld;

	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	DrawFrame ();

	hDC			= m_pDib->Lock ();
	hFontOld	= (HFONT)SelectObject (hDC, m_hFont);
	SetBkMode (hDC, TRANSPARENT);

	TextOut2 (hDC, 16, 16, "アカウント:",	RGB (1, 1, 1));
	TextOut2 (hDC, 16, 42, "パスワード:",	RGB (1, 1, 1));

	SelectObject (hDC, hFontOld);
	m_pDib->Unlock ();

	/* 枠 */
	DrawInputFrame1 (16 * 8, 16, 8 * 12, 14, 0);
	DrawInputFrame1 (16 * 8, 42, 8 * 12, 14, 0);

	m_dwTimeDrawStart = timeGetTime ();

Exit:
	pDst->Blt (m_ptViewPos.x + 32, m_ptViewPos.y + 32, m_sizeWindow.cx, m_sizeWindow.cy, m_pDib, 0, 0, TRUE);
}


/* ========================================================================= */
/* 関数名	:CWindowLOGIN::Enable											 */
/* 内容		:入力許可・禁止													 */
/* 日付		:2005/05/29														 */
/* ========================================================================= */

void CWindowLOGIN::Enable(BOOL bEnable)
{
	EnableWindow (m_hWndAccount,		bEnable);
	EnableWindow (m_hWndPassword,		bEnable);
	EnableWindow (m_hWndSavePassword,	bEnable);
	EnableWindow (m_hWndConnect,		bEnable);
}


/* ========================================================================= */
/* 関数名	:CWindowLOGIN::SetShow											 */
/* 内容		:表示するか設定													 */
/* 日付		:2005/06/26														 */
/* ========================================================================= */

void CWindowLOGIN::SetShow(BOOL bShow)
{
	int nCmdShow;

	CWindowBase::SetShow (bShow);

	nCmdShow = (bShow) ? SW_SHOW : SW_HIDE;
	ShowWindow (m_hWndAccount,		bShow);
	ShowWindow (m_hWndPassword,		bShow);
	ShowWindow (m_hWndSavePassword,	bShow);
	ShowWindow (m_hWndConnect,		bShow);
}


/* ========================================================================= */
/* 関数名	:CWindowLOGIN::Save												 */
/* 内容		:アカウントとパスワードを保存									 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

void CWindowLOGIN::Save(void)
{
	BOOL bSave;

	m_pMgrData->SetLastAccount (m_strAccount);
	m_pMgrData->SetLastPassword ("");

	bSave = GetCheck ();
	m_pMgrData->SetSavePassword (bSave);
	if (bSave) {
		m_pMgrData->SetLastPassword (m_strPassword);
	}
	m_pMgrData->SaveIniData ();
}


/* ========================================================================= */
/* 関数名	:CWindowLOGIN::SetCheck											 */
/* 内容		:パスワードを保存するチェックの設定								 */
/* 日付		:2007/07/03														 */
/* ========================================================================= */

void CWindowLOGIN::SetCheck(BOOL bCheck)
{
	WPARAM wParam;

	if (m_hWndSavePassword == NULL) {
		return;
	}

	wParam = BST_UNCHECKED;
	if (bCheck) {
		wParam = BST_CHECKED;
	}
	SendMessage (m_hWndSavePassword, BM_SETCHECK, wParam, 0);
}


/* ========================================================================= */
/* 関数名	:CWindowLOGIN::GetCheck											 */
/* 内容		:パスワードを保存するチェックの取得								 */
/* 日付		:2007/07/03														 */
/* ========================================================================= */

BOOL CWindowLOGIN::GetCheck(void)
{
	LRESULT lResult;
	BOOL bRet;

	bRet = FALSE;
	if (m_hWndSavePassword == NULL) {
		goto Exit;
	}

	lResult = SendMessage (m_hWndSavePassword, BM_GETCHECK, 0, 0);
	if (lResult == BST_CHECKED) {
		bRet = TRUE;
	}

Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWindowLOGIN::MakeWindow										 */
/* 内容		:ウィンドウ作成													 */
/* 日付		:2005/05/14														 */
/* ========================================================================= */

void CWindowLOGIN::MakeWindow(void)
{
	BOOL bSave;
	HINSTANCE hInstance;
	HWND hWndMain;
	HMODULE hThemeModule;
	typedef HRESULT (WINAPI *SETWINDOWTHEME)(HWND, LPCWSTR, LPCWSTR);
	SETWINDOWTHEME pfSetWindowTheme;
	CmyString strAccount, strPassword;

	hInstance	= m_pMgrData->GetInstance ();
	hWndMain	= m_pMgrData->GetMainWindow ();

	strAccount  = m_pMgrData->GetLastAccount ();
	strPassword = m_pMgrData->GetLastPassword ();

	/* アカウント入力欄 */
	m_hWndAccount = CreateWindow ("EDIT", "", WS_CHILD |  ES_LOWERCASE,
			m_ptViewPos.x + 16 * 8, m_ptViewPos.y + 16, 8 * 12, 14, hWndMain, NULL, hInstance, NULL);
	m_OrgWndProcAccount = (WNDPROC)GetWindowLong (m_hWndAccount, GWL_WNDPROC);
	SendMessage			(m_hWndAccount, WM_SETFONT, (WPARAM)GetStockObject (DEFAULT_GUI_FONT), 0);
	ImmAssociateContext	(m_hWndAccount, NULL);
	SetWindowLong		(m_hWndAccount, GWL_USERDATA, (LONG)this);
	SetWindowLong		(m_hWndAccount, GWL_WNDPROC, (LONG)AccountWndProc);
	SetWindowText (m_hWndAccount, strAccount);

	/* パスワード入力欄 */
	m_hWndPassword = CreateWindow ("EDIT", "", WS_CHILD | ES_PASSWORD | ES_AUTOHSCROLL,
			m_ptViewPos.x + 16 * 8, m_ptViewPos.y + 42, 8 * 12, 14, hWndMain, NULL, hInstance, NULL);
	m_OrgWndProcPassword = (WNDPROC)GetWindowLong (m_hWndPassword, GWL_WNDPROC);
	SendMessage			(m_hWndPassword, WM_SETFONT, (WPARAM)GetStockObject (DEFAULT_GUI_FONT), 0);
	ImmAssociateContext	(m_hWndPassword, NULL);
	SetWindowLong		(m_hWndPassword, GWL_USERDATA, (LONG)this);
	SetWindowLong		(m_hWndPassword, GWL_WNDPROC, (LONG)PasswordWndProc);

	/* パスワードを記録するチェック */
	m_hWndSavePassword = CreateWindow ("BUTTON", "パスワードを記録する", WS_CHILD | BS_AUTOCHECKBOX,
			m_ptViewPos.x + 16, m_ptViewPos.y + 16 * 4 + 6, 6 * 23, 14, hWndMain, (HMENU)IDC_SAVEPASSWORD, hInstance, NULL);
	m_OrgWndProcSavePassword = (WNDPROC)GetWindowLong (m_hWndSavePassword, GWL_WNDPROC);
	SendMessage		(m_hWndSavePassword, WM_SETFONT, (WPARAM)GetStockObject (DEFAULT_GUI_FONT), 0);
	SendMessage		(m_hWndSavePassword, BM_SETCHECK, FALSE, 0);
	SetWindowLong	(m_hWndSavePassword, GWL_USERDATA, (LONG)this);
	SetWindowLong	(m_hWndSavePassword, GWL_WNDPROC, (LONG)SavePasswordWndProc);
	InvalidateRect	(m_hWndSavePassword, NULL, TRUE);
	bSave = m_pMgrData->GetSavePassword ();
	if (bSave) {
		SetCheck (bSave);
		SetWindowText (m_hWndPassword, strPassword);
	}

	/* 接続ボタン */
	m_hWndConnect = CreateWindow ("BUTTON", "接続", WS_CHILD | BS_PUSHBUTTON,
			m_ptViewPos.x + 16 * 11 - 2, m_ptViewPos.y + 16 * 4 + 2, 54, 18, hWndMain, (HMENU)IDC_CONNECT, hInstance, NULL);
	m_OrgWndProcConnect = (WNDPROC)GetWindowLong (m_hWndConnect, GWL_WNDPROC);
	SendMessage		(m_hWndConnect, WM_SETFONT, (WPARAM)GetStockObject (DEFAULT_GUI_FONT), 0);
	SetWindowLong	(m_hWndConnect, GWL_USERDATA, (LONG)this);
	SetWindowLong	(m_hWndConnect, GWL_WNDPROC, (LONG)ConnectWndProc);

	hThemeModule = LoadLibrary("UxTheme.dll");
	if (hThemeModule) {
		pfSetWindowTheme = (SETWINDOWTHEME)GetProcAddress(hThemeModule, "SetWindowTheme");
		pfSetWindowTheme (m_hWndSavePassword, L" ", L" ");
		FreeLibrary (hThemeModule);
	}

	if (strAccount.IsEmpty ()) {
		SetFocus (m_hWndAccount);
	} else if (strPassword.IsEmpty ()) {
		SetFocus (m_hWndPassword);
	} else {
		SetFocus (m_hWndConnect);
	}
}


/* ========================================================================= */
/* 関数名	:CWindowLOGIN::AccountWndProc									 */
/* 内容		:アカウント名入力欄メッセージ処理								 */
/* 日付		:2005/05/14														 */
/* ========================================================================= */

LRESULT CALLBACK CWindowLOGIN::AccountWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PCWindowLOGIN pThis;
	LRESULT hResult;

	pThis	= (PCWindowLOGIN)GetWindowLong (hWnd, GWL_USERDATA);
	hResult	= -1;

	switch (message) {
	case WM_CHAR:
		if (wParam == VK_TAB) {
			if (GetKeyState (VK_SHIFT) & 0x8000) {
				SetFocus (pThis->m_hWndConnect);
			} else {
				SetFocus (pThis->m_hWndPassword);
			}
			hResult = 0;

		} else if (wParam == VK_RETURN) {
			pThis->OnConnect ();
			hResult = 0;
		}
		break;
	}

	if (hResult != 0) {
		hResult = CallWindowProc (pThis->m_OrgWndProcAccount, hWnd, message, wParam, lParam);
	}
	return hResult;
}


/* ========================================================================= */
/* 関数名	:CWindowLOGIN::PasswordWndProc									 */
/* 内容		:パスワード名入力欄メッセージ処理								 */
/* 日付		:2005/05/14														 */
/* ========================================================================= */

LRESULT CALLBACK CWindowLOGIN::PasswordWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PCWindowLOGIN pThis;
	LRESULT hResult;

	pThis	= (PCWindowLOGIN)GetWindowLong (hWnd, GWL_USERDATA);
	hResult	= -1;

	switch (message) {
	case WM_CHAR:
		if (wParam == VK_TAB) {
			if (GetKeyState (VK_SHIFT) & 0x8000) {
				SetFocus (pThis->m_hWndAccount);
			} else {
				SetFocus (pThis->m_hWndSavePassword);
			}
			hResult = 0;

		} else if (wParam == VK_RETURN) {
			pThis->OnConnect ();
			hResult = 0;
		}
		break;
	}

	if (hResult != 0) {
		hResult = CallWindowProc (pThis->m_OrgWndProcPassword, hWnd, message, wParam, lParam);
	}
	return hResult;
}


/* ========================================================================= */
/* 関数名	:CWindowLOGIN::SavePasswordWndProc								 */
/* 内容		:パスワードを記録するチェックメッセージ処理						 */
/* 日付		:2005/05/29														 */
/* ========================================================================= */

LRESULT CALLBACK CWindowLOGIN::SavePasswordWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PCWindowLOGIN pThis;
	LRESULT hResult;

	pThis	= (PCWindowLOGIN)GetWindowLong (hWnd, GWL_USERDATA);
	hResult	= -1;

	switch (message) {
	case WM_CHAR:
		if (wParam == VK_TAB) {
			if (GetKeyState (VK_SHIFT) & 0x8000) {
				SetFocus (pThis->m_hWndPassword);
			} else {
				SetFocus (pThis->m_hWndConnect);
			}
			hResult = 0;

		} else if (wParam == VK_RETURN) {
			pThis->OnConnect ();
			hResult = 0;
		}
		break;

	case WM_ERASEBKGND:
		{
			RECT rc;
			HDC hDC;
			HBRUSH hBrush, hBrushOld;

			hDC = GetDC (hWnd);
			hBrush = CreateSolidBrush (RGB (255, 235, 200));

			GetClientRect (hWnd, &rc);
			hBrushOld = (HBRUSH)SelectObject (hDC, hBrush);
			FillRect (hDC, &rc, hBrush);

			ReleaseDC (hWnd, hDC);
			SelectObject (hDC, hBrushOld);
			DeleteObject (hBrush);
			InvalidateRect (hWnd, NULL, FALSE);
			UpdateWindow (hWnd);
		}
		return 1;
	}

	if (hResult != 0) {
		hResult = CallWindowProc (pThis->m_OrgWndProcSavePassword, hWnd, message, wParam, lParam);
	}
	return hResult;
}


/* ========================================================================= */
/* 関数名	:CWindowLOGIN::ConnectWndProc									 */
/* 内容		:接続ボタン用メッセージ処理										 */
/* 日付		:2005/05/14														 */
/* ========================================================================= */

LRESULT CALLBACK CWindowLOGIN::ConnectWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PCWindowLOGIN pThis;
	LRESULT hResult;

	pThis	= (PCWindowLOGIN)GetWindowLong (hWnd, GWL_USERDATA);
	hResult	= -1;

	switch (message) {
	case WM_CHAR:
 		if (wParam == VK_TAB) {
			if (GetKeyState (VK_SHIFT) & 0x8000) {
				SetFocus (pThis->m_hWndSavePassword);
			} else {
				SetFocus (pThis->m_hWndAccount);
			}
			hResult = 0;
		}
		break;

	case WM_KEYUP:
		if ((wParam == VK_RETURN) || (wParam == VK_SPACE)) {
			pThis->OnConnect ();
		}
		break;

	case WM_COMMAND:
		pThis->OnConnect ();
		break;
	}

	if (hResult != 0) {
		hResult = CallWindowProc (pThis->m_OrgWndProcConnect, hWnd, message, wParam, lParam);
	}
	return hResult;
}


/* ========================================================================= */
/* 関数名	:CWindowLOGIN::OnConnect										 */
/* 内容		:接続ボタンハンドラ												 */
/* 日付		:2005/05/14														 */
/* ========================================================================= */

void CWindowLOGIN::OnConnect(void)
{
	char szTmp[256];

	m_pMgrSound->PlaySound (SOUNDID_OK_PI73);

	GetWindowText (m_hWndAccount, szTmp, sizeof (szTmp));
	TrimViewString (m_strAccount, szTmp);
	GetWindowText (m_hWndPassword, szTmp, sizeof (szTmp));
	TrimViewString (m_strPassword, szTmp);
	if (m_strAccount.IsEmpty ()) {
		return;
	}
	Enable (FALSE);

	PostMessage (m_hWndMain, WM_MAINFRAME, MAINFRAMEMSG_CONNECT, 0);
}

/* Copyright(C)URARA-works 2006 */
