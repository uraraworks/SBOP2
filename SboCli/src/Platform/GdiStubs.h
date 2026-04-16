/// @file GdiStubs.h
/// @brief 非Windows環境向け GDI描画関数スタブ（SDL/SdlFont連携）
/// @date 2026/04/07

#pragma once

#if !defined(_WIN32)

// -----------------------------------------------------------------------
// BeginPaint / EndPaint スタブ
// -----------------------------------------------------------------------

/// BeginPaint スタブ（PAINTSTRUCT をゼロクリアして NULL を返す）
inline HDC BeginPaint(HWND, LPPAINTSTRUCT pPaintStruct)
{
	if (pPaintStruct != NULL) {
		ZeroMemory(pPaintStruct, sizeof(PAINTSTRUCT));
	}
	return NULL;
}

/// EndPaint スタブ（何もしない）
inline BOOL EndPaint(HWND, const PAINTSTRUCT *)
{
	return TRUE;
}

// -----------------------------------------------------------------------
// フォント生成・選択・削除
// -----------------------------------------------------------------------

/// CreateFont: SdlFontCreate にマッピング
inline HFONT CreateFont(int nHeight, int /*nWidth*/, int /*nEscapement*/, int /*nOrientation*/,
	int nWeight, DWORD /*fdwItalic*/, DWORD /*fdwUnderline*/, DWORD /*fdwStrikeOut*/,
	DWORD /*fdwCharSet*/, DWORD /*fdwOutputPrecision*/, DWORD /*fdwClipPrecision*/,
	DWORD /*fdwQuality*/, DWORD /*fdwPitchAndFamily*/, LPCTSTR /*lpszFace*/)
{
	bool bold = (nWeight >= 700); // FW_BOLD = 700
	return (HFONT)SdlFontCreate(abs(nHeight), bold);
}

/// SelectObject: DC コンテキストにフォントを設定する
inline HGDIOBJ SelectObject(HDC hDC, HGDIOBJ hObject)
{
	SdlDCContext* ctx = SdlDCGet(hDC);
	if (ctx) {
		HGDIOBJ old = (HGDIOBJ)ctx->currentFont;
		ctx->currentFont = hObject;
		return old;
	}
	return hObject;
}

/// DeleteObject: SdlFontDestroy にマッピング
inline BOOL DeleteObject(HGDIOBJ hObj)
{
	SdlFontDestroy(hObj);
	return TRUE;
}

// -----------------------------------------------------------------------
// DC 生成・解放スタブ
// -----------------------------------------------------------------------

/// DeleteDC スタブ（何もしない）
inline BOOL DeleteDC(HDC)
{
	return TRUE;
}

/// CreateCompatibleDC スタブ（NULL を返す）
inline HDC CreateCompatibleDC(HDC)
{
	return NULL;
}

/// GetDC スタブ（NULL を返す）
inline HDC GetDC(HWND)
{
	return NULL;
}

/// ReleaseDC スタブ（何もしない）
inline int ReleaseDC(HWND, HDC)
{
	return 0;
}

// -----------------------------------------------------------------------
// DIBSection 生成
// -----------------------------------------------------------------------

/// CreateDIBSection: ヒープからピクセルバッファを確保して返す
inline HBITMAP CreateDIBSection(HDC, const BITMAPINFO *pBmpInfo, UINT, void **ppBits, HANDLE, DWORD)
{
	if ((pBmpInfo == NULL) || (ppBits == NULL)) {
		return NULL;
	}

	const int cx = (int)pBmpInfo->bmiHeader.biWidth;
	const int cy = (int)abs(pBmpInfo->bmiHeader.biHeight);
	const size_t nSize = (size_t)max(cx, 0) * (size_t)max(cy, 0) * 4;
	if (nSize == 0) {
		*ppBits = NULL;
		return NULL;
	}

	BYTE *pNewBits = new BYTE[nSize];
	ZeroMemory(pNewBits, nSize);
	*ppBits = pNewBits;
	return (HBITMAP)pNewBits;
}

// -----------------------------------------------------------------------
// テキスト描画関連
// -----------------------------------------------------------------------

/// SetTextColor: DC コンテキストにテキスト色を設定する
inline COLORREF SetTextColor(HDC hDC, COLORREF color)
{
	SdlDCContext* ctx = SdlDCGet(hDC);
	if (ctx) {
		COLORREF old = (COLORREF)ctx->textColor;
		ctx->textColor = (unsigned long)color;
		return old;
	}
	return color;
}

/// SetBkMode スタブ（TRANSPARENT を返す）
inline int SetBkMode(HDC, int)
{
	return TRANSPARENT;
}

/// SetBkColor スタブ（color をそのまま返す）
inline COLORREF SetBkColor(HDC, COLORREF color)
{
	return color;
}

/// TextOut: SdlFontTextOut にマッピング（wchar_t 版）
inline BOOL TextOut(HDC hDC, int x, int y, LPCTSTR pStr, int nLen)
{
	return SdlFontTextOut(hDC, x, y, pStr, nLen) ? TRUE : FALSE;
}

/// TextOutW: SdlFontTextOut にマッピング（wchar_t 版）
inline BOOL TextOutW(HDC hDC, int x, int y, LPCWSTR pStr, int nLen)
{
	return SdlFontTextOut(hDC, x, y, pStr, nLen) ? TRUE : FALSE;
}

/// DrawText スタブ（何もしない）
inline int DrawText(HDC, LPCTSTR, int, LPRECT, UINT)
{
	return 0;
}

// -----------------------------------------------------------------------
// テキスト幅・高さ取得
// -----------------------------------------------------------------------

/// GetTextExtentPoint32: SdlFontGetTextExtent にマッピング（wchar_t 版）
inline BOOL GetTextExtentPoint32(HDC hDC, LPCTSTR lpString, int c, LPSIZE psizl)
{
	if (!psizl) return FALSE;
	SdlDCContext* ctx = SdlDCGet(hDC);
	if (ctx && ctx->currentFont) {
		int w = 0, h = 0;
		if (SdlFontGetTextExtent(ctx->currentFont, lpString, c, &w, &h)) {
			psizl->cx = w;
			psizl->cy = h;
			return TRUE;
		}
	}
	// フォールバック: 固定サイズ推定
	psizl->cx = c * 8;
	psizl->cy = 16;
	return TRUE;
}

/// GetTextExtentPoint32W: SdlFontGetTextExtent にマッピング（wchar_t 版）
inline BOOL GetTextExtentPoint32W(HDC hDC, LPCWSTR lpString, int c, LPSIZE psizl)
{
	if (!psizl) return FALSE;
	SdlDCContext* ctx = SdlDCGet(hDC);
	if (ctx && ctx->currentFont) {
		int w = 0, h = 0;
		if (SdlFontGetTextExtent(ctx->currentFont, lpString, c, &w, &h)) {
			psizl->cx = w;
			psizl->cy = h;
			return TRUE;
		}
	}
	// フォールバック: 固定サイズ推定（ワイド文字は倍幅想定）
	psizl->cx = c * 12;
	psizl->cy = 16;
	return TRUE;
}

/// GetTextExtentPoint32A: ANSI文字列をワイド変換して GetTextExtentPoint32 に委譲
inline BOOL GetTextExtentPoint32A(HDC hDC, LPCSTR lpString, int c, LPSIZE psizl)
{
	if (!psizl) return FALSE;
	// Win32 版の A 系 API は SJIS(CP932) を想定する。CP_ACP で変換する。
	if (lpString && c > 0) {
		int wlen = MultiByteToWideChar(CP_ACP, 0, lpString, c, NULL, 0);
		if (wlen > 0) {
			std::vector<wchar_t> wbuf(wlen);
			MultiByteToWideChar(CP_ACP, 0, lpString, c, wbuf.data(), wlen);
			return GetTextExtentPoint32(hDC, wbuf.data(), wlen, psizl);
		}
	}
	psizl->cx = c * 8;
	psizl->cy = 16;
	return TRUE;
}

// -----------------------------------------------------------------------
// ビットブロック転送
// -----------------------------------------------------------------------

/// StretchBlt スタブ（何もしない）
inline BOOL StretchBlt(HDC, int, int, int, int, HDC, int, int, int, int, DWORD)
{
	return TRUE;
}

#endif // !_WIN32
