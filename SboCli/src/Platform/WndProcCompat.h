/// @file WndProcCompat.h
/// @brief 非Windows環境向け ウィンドウ/メッセージ関数スタブ
/// @date 2026/04/07

#pragma once

#if !defined(_WIN32)

// -----------------------------------------------------------------------
// ウィンドウ操作スタブ
// -----------------------------------------------------------------------

/// ShowWindow スタブ（何もしない）
inline BOOL ShowWindow(HWND, int)
{
	return TRUE;
}

/// SetWindowPos スタブ（何もしない）
inline BOOL SetWindowPos(HWND, HWND, int, int, int, int, UINT)
{
	return TRUE;
}

/// GetWindowRect スタブ（RECT をゼロクリアして TRUE を返す）
inline BOOL GetWindowRect(HWND, LPRECT pRect)
{
	if (pRect == NULL) {
		return FALSE;
	}
	ZeroMemory(pRect, sizeof(RECT));
	return TRUE;
}

/// SetWindowLong スタブ（0 を返す）
inline LONG SetWindowLong(HWND, int, LONG)
{
	return 0;
}

/// GetWindowLong スタブ（0 を返す）
inline LONG GetWindowLong(HWND, int)
{
	return 0;
}

// -----------------------------------------------------------------------
// RECT 操作
// -----------------------------------------------------------------------

/// SetRect: RECT に値をセットする
inline BOOL SetRect(LPRECT pRect, int left, int top, int right, int bottom)
{
	if (pRect == NULL) {
		return FALSE;
	}
	pRect->left = left;
	pRect->top = top;
	pRect->right = right;
	pRect->bottom = bottom;
	return TRUE;
}

/// IntersectRect: 2つの RECT の交差領域を計算する
inline BOOL IntersectRect(LPRECT pDst, const RECT *pSrc1, const RECT *pSrc2)
{
	if ((pDst == NULL) || (pSrc1 == NULL) || (pSrc2 == NULL)) {
		return FALSE;
	}
	pDst->left = std::max(pSrc1->left, pSrc2->left);
	pDst->top = std::max(pSrc1->top, pSrc2->top);
	pDst->right = std::min(pSrc1->right, pSrc2->right);
	pDst->bottom = std::min(pSrc1->bottom, pSrc2->bottom);
	if ((pDst->left >= pDst->right) || (pDst->top >= pDst->bottom)) {
		ZeroMemory(pDst, sizeof(RECT));
		return FALSE;
	}
	return TRUE;
}

// -----------------------------------------------------------------------
// ウィンドウ状態確認スタブ
// -----------------------------------------------------------------------

/// IsWindowVisible スタブ（常に FALSE を返す）
inline BOOL IsWindowVisible(HWND)
{
	return FALSE;
}

/// IsIconic スタブ（常に FALSE を返す）
inline BOOL IsIconic(HWND)
{
	return FALSE;
}

// -----------------------------------------------------------------------
// フォーカス操作スタブ
// -----------------------------------------------------------------------

/// SetFocus スタブ（引数の HWND をそのまま返す）
inline HWND SetFocus(HWND hWnd)
{
	return hWnd;
}

/// GetFocus スタブ（NULL を返す）
inline HWND GetFocus(void)
{
	return NULL;
}

// -----------------------------------------------------------------------
// メッセージ送受信スタブ
// -----------------------------------------------------------------------

/// PostMessage スタブ（何もしない）
inline LRESULT PostMessage(HWND, UINT, WPARAM, LPARAM)
{
	return 0;
}

/// SendMessage スタブ（何もしない）
inline LRESULT SendMessage(HWND, UINT, WPARAM, LPARAM)
{
	return 0;
}

/// CallWindowProc: pWndProc が非 NULL なら呼び出す
inline LRESULT CallWindowProc(WNDPROC pWndProc, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (pWndProc == NULL) {
		return 0;
	}
	return pWndProc(hWnd, msg, wParam, lParam);
}

/// DefWindowProc スタブ（0 を返す）
inline LRESULT DefWindowProc(HWND, UINT, WPARAM, LPARAM)
{
	return 0;
}

// -----------------------------------------------------------------------
// タイマースタブ
// -----------------------------------------------------------------------

/// SetTimer スタブ（nIDEvent をそのまま返す）
inline UINT_PTR SetTimer(HWND, UINT_PTR nIDEvent, UINT, void *)
{
	return nIDEvent;
}

/// KillTimer スタブ（何もしない）
inline BOOL KillTimer(HWND, UINT_PTR)
{
	return TRUE;
}

// -----------------------------------------------------------------------
// メッセージループスタブ
// -----------------------------------------------------------------------

/// PeekMessage スタブ（常に FALSE を返す）
inline BOOL PeekMessage(LPMSG, HWND, UINT, UINT, UINT)
{
	return FALSE;
}

/// TranslateMessage スタブ（TRUE を返す）
inline BOOL TranslateMessage(const MSG *)
{
	return TRUE;
}

/// DispatchMessage スタブ（0 を返す）
inline LRESULT DispatchMessage(const MSG *)
{
	return 0;
}

/// PostQuitMessage スタブ（何もしない）
inline void PostQuitMessage(int)
{
}

// -----------------------------------------------------------------------
// ウィンドウ登録・生成・破棄スタブ
// -----------------------------------------------------------------------

/// InvalidateRect スタブ（何もしない）
inline BOOL InvalidateRect(HWND, const RECT *, BOOL)
{
	return TRUE;
}

/// UpdateWindow スタブ（何もしない）
inline BOOL UpdateWindow(HWND)
{
	return TRUE;
}

/// SetWindowText スタブ（何もしない）
inline BOOL SetWindowText(HWND, LPCTSTR)
{
	return TRUE;
}

// -----------------------------------------------------------------------
// シェル・タスクバー系スタブ
// -----------------------------------------------------------------------

/// FlashWindowEx スタブ（何もしない）
inline BOOL FlashWindowEx(PFLASHWINFO)
{
	return TRUE;
}

/// SHAppBarMessage スタブ（0 を返す）
inline UINT SHAppBarMessage(DWORD, PAPPBARDATA)
{
	return 0;
}

/// ShellExecute スタブ（NULL を返す）
inline HINSTANCE ShellExecute(HWND, LPCTSTR, LPCTSTR, LPCTSTR, LPCTSTR, int)
{
	return NULL;
}

#endif // !_WIN32
