/// @file Img32.cpp
/// @brief 32bitイメージクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/01/24
/// @copyright Copyright(C)URARA-works 2005

#include "stdafx.h"
#include <math.h>
#include "Img32.h"

// 定数の定義

#define HBITMAP_WITHOUT_GDI	((HBITMAP)-1)			// 非互換モード時のハンドル値
#define BYTES_PER_PIXEL		(4)						// 1ドットのバイト数
#define BITS_PER_PIXEL		(BYTES_PER_PIXEL * 8)	// 1ドットのビット数
#define	BYTES_PER_LINE(cx)	(cx * BYTES_PER_PIXEL)	// 1ラインのバイト数


// 構造体の定義

union TARGB {
	DWORD ARGB;
	struct {
		BYTE R, G, B, A;
	};
};

typedef struct _IMG32RGB {
	BYTE	b;
	BYTE	g;
	BYTE	r;
} IMG32RGB, *PIMG32RGB;


CImg32::CImg32(void)
{
	InitData();
}

CImg32::~CImg32()
{
	Destroy();
}

BOOL CImg32::Create(
	int cx,		// [in] 幅
	int cy)		// [in] 高さ
{
	BOOL bRet;
	HDC hDC;

	bRet = FALSE;

	if (m_hBmp) {
		goto Exit;
	}

	// ビットマップ情報を設定
	ZeroMemory(&m_bmpInfo, sizeof(m_bmpInfo));
	m_bmpInfo.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	m_bmpInfo.bmiHeader.biWidth       = cx;
	m_bmpInfo.bmiHeader.biHeight      = cy;
	m_bmpInfo.bmiHeader.biPlanes      = 1;
	m_bmpInfo.bmiHeader.biBitCount    = BYTES_PER_PIXEL * 8;
	m_bmpInfo.bmiHeader.biCompression = BI_RGB;

	// DIB セクションオブジェクトを作成
	hDC = GetDC(NULL);
	m_hBmp = CreateDIBSection(hDC, &m_bmpInfo, DIB_RGB_COLORS, (PVOID*)&m_pBits, 0, 0);
	ReleaseDC(NULL, hDC);

	if (m_hBmp == FALSE) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}

BOOL CImg32::CreateWithoutGdi(
	int cx,						// [in] 幅
	int cy,						// [in] 高さ
	LPBYTE pBits /* = NULL */)	// [in] ビットデータ
{
	BOOL bRet;

	bRet = FALSE;

	if (m_hBmp) {
		goto Exit;
	}

	// ビットマップ情報を設定
	ZeroMemory(&m_bmpInfo, sizeof(m_bmpInfo));
	m_bmpInfo.bmiHeader.biSize   = sizeof(BITMAPINFOHEADER);
	m_bmpInfo.bmiHeader.biWidth  = cx;
	m_bmpInfo.bmiHeader.biHeight = cy;

	// ビットデータが指定されている？
	if (pBits) {
		m_pBits = pBits;

	} else {
		DWORD dwSize;

		dwSize = BYTES_PER_LINE(cx) * cy;
		m_pBits = new BYTE [dwSize];
		if (m_pBits == NULL) {
			goto Exit;
		}
		ZeroMemory(m_pBits, dwSize);
	}

	// ダミーのハンドルを設定
	m_hBmp = HBITMAP_WITHOUT_GDI;

	bRet = TRUE;
Exit:
	return bRet;
}

BOOL CImg32::CreateWithoutGdi256(
	int cx,		// [in] 幅
	int cy)		// [in] 高さ
{
	BOOL bRet;

	bRet = FALSE;

	if (m_hBmp) {
		goto Exit;
	}

	m_byColorKey = 0;

	m_pPallet = new BYTE[256 * 3];
	ZeroMemory(m_pPallet, 256 * 3);

	m_pBits = new BYTE[cx * cy];
	ZeroMemory(m_pBits, cx * cy);

	// ビットマップ情報を設定
	ZeroMemory(&m_bmpInfo, sizeof(m_bmpInfo));
	m_bmpInfo.bmiHeader.biSize   = sizeof(BITMAPINFOHEADER);
	m_bmpInfo.bmiHeader.biWidth  = cx;
	m_bmpInfo.bmiHeader.biHeight = cy;

	// ダミーのハンドルを設定
	m_hBmp = HBITMAP_WITHOUT_GDI;

Exit:
	return bRet;
}

void CImg32::Destroy(void)
{
	Unlock();

	// DIB セクションオブジェクトを破棄
	if (m_hBmp) {
		if (m_hBmp != HBITMAP_WITHOUT_GDI) {
			DeleteObject(m_hBmp);

		} else {
			delete [] m_pBits;
		}
	}

	if (m_pPallet) {
		delete [] m_pPallet;
	}

	InitData();
}

void CImg32::Clear(void)
{
	int cx, cy, bpl;

	if (m_pBits == NULL) {
		return;
	}

	cx = Width();
	cy = Height();
	bpl = BYTES_PER_LINE(cx);
	if (IsMode256()) {
		bpl = cx;
	}
	ZeroMemory(m_pBits, bpl * cy);
}

void CImg32::FillRect(
	int x,				// [in] 始点(X)
	int y,				// [in] 始点(Y)
	int cx,				// [in] 幅
	int cy,				// [in] 高さ
	COLORREF clFill)	// [in] 塗りつぶす色
{
	int i, j;
	PDWORD dst;
	DWORD dwTmp, dwLineDst;
	PBYTE pBitsDst;

	if (m_pBits == NULL) {
		return;
	}

	x = min(x, Width());
	x = max(x, 0);
	y = min(y, Height());
	y = max(y, 0);
	if (x + cx > Width()) {
		cx = Width() - x;
	}
	if (y + cy > Height()) {
		cy = Height() - y;
	}

	pBitsDst = m_pBits;
	dwLineDst = BYTES_PER_LINE(Width());
	dwTmp = ColorrefToDword(clFill);

	x *= BYTES_PER_PIXEL;
	y = Height() - 1 - y;

	for (j = 0; j < cy; j ++) {
		dst = (PDWORD)(pBitsDst + dwLineDst * (y - j) + x);
		for (i = 0; i < cx; i ++) {
			*dst = dwTmp;
			dst ++;
		}
	}
}

void CImg32::XorRect(
	int x,		// [in] 始点(X)
	int y,		// [in] 始点(Y)
	int cx,		// [in] 幅
	int cy)		// [in] 高さ
{
	int i, j;
	PDWORD dst;
	DWORD dwLineDst;
	PBYTE pBitsDst;

	if (m_pBits == NULL) {
		return;
	}

	x = min(x, Width());
	x = max(x, 0);
	y = min(y, Height());
	y = max(y, 0);
	if (x + cx > Width()) {
		cx = Width() - x;
	}
	if (y + cy > Height()) {
		cy = Height() - y;
	}

	pBitsDst = m_pBits;
	dwLineDst = BYTES_PER_LINE(Width());

	x *= BYTES_PER_PIXEL;
	y = Height() - 1 - y;

	for (j = 0; j < cy; j ++) {
		dst = (PDWORD)(pBitsDst + dwLineDst * (y - j) + x);
		for (i = 0; i < cx; i ++) {
			*dst ^= (DWORD)-1;
			dst ++;
		}
	}
}

void CImg32::Rectangle(
	int x,				// [in] 始点(X)
	int y,				// [in] 始点(Y)
	int cx,				// [in] 幅
	int cy,				// [in] 高さ
	COLORREF clDraw)	// [in] 描画色(-1で色反転)
{
	int i, j;
	PDWORD dst;
	DWORD dwTmp, dwLineDst;
	PBYTE pBitsDst;

	if (m_pBits == NULL) {
		return;
	}

	x = min(x, Width());
	x = max(x, 0);
	y = min(y, Height());
	y = max(y, 0);
	if (x + cx > Width()) {
		cx = Width() - x;
	}
	if (y + cy > Height()) {
		cy = Height() - y;
	}

	pBitsDst = m_pBits;
	dwLineDst = BYTES_PER_LINE(Width());
	dwTmp = ColorrefToDword(clDraw);
	if (clDraw == -1) {
		dwTmp = -1;
	}

	x *= BYTES_PER_PIXEL;
	y = Height() - 1 - y;

	for (j = 0; j < cy; j ++) {
		dst = (PDWORD)(pBitsDst + dwLineDst * (y - j) + x);
		if (j > 0 && j < cy - 1) {
			if (dwTmp != -1) {
				*dst = dwTmp;
				dst[cx - 1] = dwTmp;
			} else {
				*dst ^= 0xFFFFFFFF;
				dst[cx - 1] ^= 0xFFFFFFFF;
			}
		} else {
			for (i = 0; i < cx; i ++) {
				if (dwTmp != -1) {
					*dst = dwTmp;
				} else {
					*dst ^= 0xFFFFFFFF;
				}
				dst ++;
			}
		}
	}
}

void CImg32::Circle(
	int x,				// [in] 始点(X)
	int y,				// [in] 始点(Y)
	int r,				// [in] 半径
	COLORREF clDraw)	// [in] 描画色(-1で色反転)
{
	int i, j, xx, yy, xxx, yyy, mx, my, cx, cy;
	PDWORD dst;
	DWORD dwTmp, dwLineDst;
	PBYTE pBitsDst;

	if (m_pBits == NULL) {
		return;
	}

	cx = r * 2;
	cy = r * 2;

	x = min(x, Width());
	x = max(x, 0);
	y = min(y, Height());
	y = max(y, 0);
	if (x + cx > Width()) {
		cx = Width() - x;
	}
	if (y + cy > Height()) {
		cy = Height() - y;
	}
	// 中心点の設定
	mx = x + cx / 2;
	my = y + cy / 2;

	pBitsDst = m_pBits;
	dwLineDst = BYTES_PER_LINE(Width());
	dwTmp = ColorrefToDword(clDraw);
	if (clDraw == -1) {
		dwTmp = -1;
	}

	xxx = x * BYTES_PER_PIXEL;
	yyy = Height() - 1 - y;

	for (j = 0; j < cy; j ++) {
		yy = y + j;
		dst = (PDWORD)(pBitsDst + dwLineDst * (yyy - j) + xxx);
		for (i = 0; i < cx; i ++) {
			xx = x + i;
			if ((xx - mx) * (xx - mx) + (yy - my) * (yy - my) <= r * r) {
				dst[i] = dwTmp;
			}
		}
	}
}

void CImg32::CircleGradation(
	int x,				// [in] 始点(X)
	int y,				// [in] 始点(Y)
	int r,				// [in] 半径
	int rr,				// [in] グラデーションしない半径
	COLORREF clDraw)	// [in] 描画色(-1で色反転)
{
	int i, j, xx, yy, xxx, yyy, mx, my, cx, cy, nTmp, nLevel;
	double dTmp, dr;
	PDWORD dst;
	DWORD dwTmp, dwLineDst;
	BYTE byLevel;
	PBYTE pBitsDst;

	if (m_pBits == NULL) {
		return;
	}

	cx = r * 2;
	cy = r * 2;

	x = min(x, Width());
	x = max(x, 0);
	y = min(y, Height());
	y = max(y, 0);
	if (x + cx > Width()) {
		cx = Width() - x;
	}
	if (y + cy > Height()) {
		cy = Height() - y;
	}
	// 中心点の設定
	mx = x + cx / 2;
	my = y + cy / 2;

	pBitsDst = m_pBits;
	dwLineDst = BYTES_PER_LINE(Width());
	dwTmp = ColorrefToDword(clDraw);
	if (clDraw == -1) {
		dwTmp = -1;
	}

	xxx = x * BYTES_PER_PIXEL;
	yyy = Height() - 1 - y;
	dr = r - rr;

	for (j = 0; j < cy; j ++) {
		yy = y + j;
		dst = (PDWORD)(pBitsDst + dwLineDst * (yyy - j) + xxx);
		for (i = 0; i < cx; i ++) {
			xx = x + i;
			if ((xx - mx) * (xx - mx) + (yy - my) * (yy - my) <= r * r) {
				TARGB tTmp;
				dTmp = (abs(j - r) * abs(j - r) + abs(i - r) * abs(i - r));
				dTmp = max(dTmp, 1);
				dTmp = sqrt(dTmp);
				if ((int)dTmp < rr) {
					nLevel = 0;
				} else {
					nLevel = (int)((dTmp - rr) * 100 / dr);
					nLevel = min(nLevel, 100);
					nLevel = max(nLevel, 1);
				}
				nLevel = 100 - nLevel;
				byLevel = PercentToHex((BYTE)nLevel);

				tTmp.ARGB = dwTmp;
//				nTmp = (int)((TARGB*)&dst[i])->R + ((int)tTmp.R + ((int)(-tTmp.R) * ((int)byLevel + 1) >> 8)) / 2 * 2;
//				nTmp = max(nTmp, 0);
//				tTmp.R = min(nTmp, 100);
//				nTmp = ((TARGB*)&dst[i])->G + (tTmp.G + ((int)(-tTmp.G) * (byLevel + 1) >> 8)) / 2 * 2;
//				nTmp = max(nTmp, 0);
//				tTmp.G = min(nTmp, 100);
				nTmp = ((TARGB*)&dst[i])->B + (tTmp.B + ((int)(-tTmp.B) * (byLevel + 1) >> 8)) / 2 * 2;
				nTmp = max(nTmp, 0);
				tTmp.B = min(nTmp, 100);
				dst[i] = tTmp.ARGB;
			}
		}
	}
}

void CImg32::ChgLevel(
	int x,			// [in] 始点(X)
	int y,			// [in] 始点(Y)
	int cx,			// [in] 幅
	int cy,			// [in] 高さ
	BYTE byLevel)	// [in] 明度レベル
{
	int i, j;
	PDWORD dst;
	DWORD dwLineDst;
	PBYTE pBitsDst;

	if (m_pBits == NULL) {
		return;
	}

	x = min(x, Width());
	x = max(x, 0);
	y = min(y, Height());
	y = max(y, 0);
	if (x + cx > Width()) {
		cx = Width() - x;
	}
	if (y + cy > Height()) {
		cy = Height() - y;
	}

	pBitsDst = m_pBits;
	dwLineDst = BYTES_PER_LINE(Width());
	byLevel = PercentToHex(byLevel);

	x *= BYTES_PER_PIXEL;
	y = Height() - 1 - y;

	for (j = 0; j < cy; j ++) {
		dst = (PDWORD)(pBitsDst + dwLineDst * (y - j) + x);
		for (i = 0; i < cx; i ++) {
			TARGB tTmp;

			tTmp.ARGB = *dst;
			tTmp.R = tTmp.R + ((int)(-tTmp.R) * (byLevel + 1) >> 8);
			tTmp.G = tTmp.G + ((int)(-tTmp.G) * (byLevel + 1) >> 8);
			tTmp.B = tTmp.B + ((int)(-tTmp.B) * (byLevel + 1) >> 8);
			*dst = tTmp.ARGB;
			dst ++;
		}
	}
}

void CImg32::ChgColor(
	int x,			// [in] 始点(X)
	int y,			// [in] 始点(Y)
	int cx,			// [in] 幅
	int cy,			// [in] 高さ
	COLORREF clChg)	// [in] 色素
{
	int i, j;
	PDWORD dst;
	DWORD dwLineDst;
	PBYTE pBitsDst;
	TARGB tChg;

	if (m_pBits == NULL) {
		return;
	}

	if (cx == 0) {
		cx = Width();
	}
	if (cy == 0) {
		cy = Height();
	}

	x = min(x, Width());
	x = max(x, 0);
	y = min(y, Height());
	y = max(y, 0);
	if (x + cx > Width()) {
		cx = Width() - x;
	}
	if (y + cy > Height()) {
		cy = Height() - y;
	}

	pBitsDst = m_pBits;
	dwLineDst = BYTES_PER_LINE(Width());
	tChg.ARGB = ColorrefToDword(clChg);

	x *= BYTES_PER_PIXEL;
	y = Height() - 1 - y;

	for (j = 0; j < cy; j ++) {
		dst = (PDWORD)(pBitsDst + dwLineDst * (y - j) + x);
		for (i = 0; i < cx; i ++) {
			TARGB tTmp;
			BYTE byTmp;

			tTmp.ARGB = *dst;
			byTmp = (BYTE)(0.30 * tTmp.B + 0.59 * tTmp.G + 0.11 * tTmp.R);

			tTmp.R = (BYTE)(double)(((double)byTmp / 255) * tChg.R);
			tTmp.G = (BYTE)(double)(((double)byTmp / 255) * tChg.G);
			tTmp.B = (BYTE)(double)(((double)byTmp / 255) * tChg.B);
			*dst = tTmp.ARGB;
			dst ++;
		}
	}
}

void CImg32::SetLevel(CImg32 *pSrc)
{
	int i, j, x, y, cx, cy;
	PDWORD dst, psrc;
	DWORD dwLineDst;
	BYTE byLevel;
	PBYTE pBitsDst, pBitsSrc;

	if (m_pBits == NULL) {
		return;
	}

	x = y = 0;
	cx = Width();
	cy = Height();

	pBitsSrc = pSrc->GetBits();
	pBitsDst = m_pBits;
	dwLineDst = BYTES_PER_LINE(Width());

	x *= BYTES_PER_PIXEL;
	y = Height() - 1 - y;

	for (j = 0; j < cy; j ++) {
		dst = (PDWORD)(pBitsDst + dwLineDst * (y - j) + x);
		psrc = (PDWORD)(pBitsSrc + dwLineDst * (y - j) + x);
		for (i = 0; i < cx; i ++) {
			TARGB tTmp, *ptDst;

			ptDst = (TARGB *)&dst[i];
			byLevel = PercentToHex(((TARGB *)(&psrc[i]))->B);
			if (byLevel == 255) {
				dst[i] = 0;
				continue;
			}
			tTmp.ARGB = dst[i];
			ptDst->R = tTmp.R + ((int)(-tTmp.R) * (byLevel + 1) >> 8);
			ptDst->G = tTmp.G + ((int)(-tTmp.G) * (byLevel + 1) >> 8);
			ptDst->B = tTmp.B + ((int)(-tTmp.B) * (byLevel + 1) >> 8);
		}
	}
}

void CImg32::Blt(
	int dx,			// [in] 始点(X)
	int dy,			// [in] 始点(Y)
	int cx,			// [in] 幅
	int cy,			// [in] 高さ
	CImg32 *pSrc,	// [in] 転送元
	int sx,			// [in] 転送元の始点(X)
	int sy,			// [in] 転送元の始点(Y)
	BOOL bColorKey)	// [in] カラーキーを使用
{
	int i, j;
	PDWORD dst, src;
	DWORD dwKey, dwLineDst, dwLineSrc;
	PBYTE pBitsDst, pBitsSrc;

	if ((m_pBits == NULL) || (pSrc == NULL)) {
		return;
	}

	if (dx < 0) {
		sx += (0 - dx);
		cx -= (0 - dx);
		dx = 0;
	}
	if (dy < 0) {
		sy += (0 - dy);
		cy -= (0 - dy);
		dy = 0;
	}

	// 座標をクリッピング
	ClipPos(dx, dy, cx, cy, pSrc, sx, sy);

	pBitsDst = m_pBits;
	pBitsSrc = pSrc->GetBits();
	dwLineDst = BYTES_PER_LINE(Width());
	dwLineSrc = BYTES_PER_LINE(pSrc->Width());
	dwKey = pSrc->GetColorKey();

	dx *= BYTES_PER_PIXEL;
	dy = Height() - 1 - dy;
	sx *= BYTES_PER_PIXEL;
	sy = pSrc->Height() - 1 - sy;

	// カラーキーを使用する？
	if (bColorKey) {
		for (j = 0; j < cy; j ++) {
			dst = (PDWORD)(pBitsDst + dwLineDst * (dy - j) + dx);
			src = (PDWORD)(pBitsSrc + dwLineSrc * (sy - j) + sx);
			for (i = 0; i < cx; i ++) {
				if (dwKey != *src) {
					*dst = *src;
				}
				dst ++;
				src ++;
			}
		}

	} else {
		for (j = 0; j < cy; j ++) {
			CopyMemory(
				pBitsDst + dwLineDst * (dy - j) + dx,
				pBitsSrc + dwLineSrc * (sy - j) + sx,
				cx * BYTES_PER_PIXEL);
		}
	}
}

void CImg32::BltLevel(
	int dx,			// [in] 始点(X)
	int dy,			// [in] 始点(Y)
	int cx,			// [in] 幅
	int cy,			// [in] 高さ
	CImg32 *pSrc,	// [in] 転送元
	int sx,			// [in] 転送元の始点(X)
	int sy,			// [in] 転送元の始点(Y)
	BYTE byLevel,	// [in] 明度
	BOOL bColorKey)	// [in] カラーキーを使用
{
	int i, j;
	DWORD dwKey, dwLineDst, dwLineSrc;
	PBYTE pBitsDst, pBitsSrc, pbySrc, pbyDst;

	if (byLevel == 100) {
		Blt(dx, dy, cx, cy, pSrc, sx, sy, bColorKey);
		return;
	}

	if ((m_pBits == NULL) || (pSrc == NULL)) {
		return;
	}

	// 座標をクリッピング
	ClipPos(dx, dy, cx, cy, pSrc, sx, sy);

	pBitsDst = m_pBits;
	pBitsSrc = pSrc->GetBits();
	dwLineDst = BYTES_PER_LINE(Width());
	dwLineSrc = BYTES_PER_LINE(pSrc->Width());
	dwKey = pSrc->GetColorKey();
	byLevel = PercentToHex(byLevel);

	dx *= BYTES_PER_PIXEL;
	dy = Height() - 1 - dy;
	sx *= BYTES_PER_PIXEL;
	sy = pSrc->Height() - 1 - sy;

	// カラーキーを使用する？
	if (bColorKey) {
		for (j = 0; j < cy; j ++) {
			pbyDst = pBitsDst + dwLineDst * (dy - j) + dx;
			pbySrc = pBitsSrc + dwLineSrc * (sy - j) + sx;
			for (i = 0; i < cx; i ++) {
				if (memcmp(&dwKey, pbySrc, BYTES_PER_PIXEL) != 0) {
					pbyDst[2] = pbySrc[2] + ((int)(-pbySrc[2]) * (byLevel + 1) >> 8);
					pbyDst[0] = pbySrc[0] + ((int)(-pbySrc[0]) * (byLevel + 1) >> 8);
					pbyDst[1] = pbySrc[1] + ((int)(-pbySrc[1]) * (byLevel + 1) >> 8);
				}
				pbyDst += BYTES_PER_PIXEL;
				pbySrc += BYTES_PER_PIXEL;
			}
		}

	} else {
		for (j = 0; j < cy; j ++) {
			pbyDst = pBitsDst + dwLineDst * (dy - j) + dx;
			pbySrc = pBitsSrc + dwLineSrc * (sy - j) + sx;
			for (i = 0; i < cx; i ++) {
				pbyDst[2] = pbySrc[2] + ((int)(-pbySrc[2]) * (byLevel + 1) >> 8);
				pbyDst[0] = pbySrc[0] + ((int)(-pbySrc[0]) * (byLevel + 1) >> 8);
				pbyDst[1] = pbySrc[1] + ((int)(-pbySrc[1]) * (byLevel + 1) >> 8);

				pbyDst += BYTES_PER_PIXEL;
				pbySrc += BYTES_PER_PIXEL;
			}
		}
	}
}

void CImg32::BltAlpha(
	int dx,			// [in] 始点(X)
	int dy,			// [in] 始点(Y)
	int cx,			// [in] 幅
	int cy,			// [in] 高さ
	CImg32 *pSrc,	// [in] 転送元
	int sx,			// [in] 転送元の始点(X)
	int sy,			// [in] 転送元の始点(Y)
	BYTE byLevel,	// [in] 割合
	BOOL bColorKey)	// [in] カラーキーを使用
{
	int i, j;
	DWORD dwKey, dwLineDst, dwLineSrc;
	PBYTE pBitsDst, pBitsSrc, pbySrc, pbyDst;

	if ((m_pBits == NULL) || (pSrc == NULL)) {
		return;
	}

	// 座標をクリッピング
	ClipPos(dx, dy, cx, cy, pSrc, sx, sy);

	pBitsDst = m_pBits;
	pBitsSrc = pSrc->GetBits();
	dwLineDst = BYTES_PER_LINE(Width());
	dwLineSrc = BYTES_PER_LINE(pSrc->Width());
	dwKey = pSrc->GetColorKey();
	byLevel = PercentToHex(byLevel);

	dx *= BYTES_PER_PIXEL;
	dy = Height() - 1 - dy;
	sx *= BYTES_PER_PIXEL;
	sy = pSrc->Height() - 1 - sy;

	// カラーキーを使用する？
	if (bColorKey) {
		for (j = 0; j < cy; j ++) {
			pbyDst = pBitsDst + dwLineDst * (dy - j) + dx;
			pbySrc = pBitsSrc + dwLineSrc * (sy - j) + sx;
			for (i = 0; i < cx; i ++) {
				if (memcmp(&dwKey, pbySrc, BYTES_PER_PIXEL)) {
					pbyDst[2] = pbyDst[2] + ((int)(pbySrc[2] - pbyDst[2]) * (byLevel + 1) >> 8);
					pbyDst[0] = pbyDst[0] + ((int)(pbySrc[0] - pbyDst[0]) * (byLevel + 1) >> 8);
					pbyDst[1] = pbyDst[1] + ((int)(pbySrc[1] - pbyDst[1]) * (byLevel + 1) >> 8);
				}
				pbyDst += BYTES_PER_PIXEL;
				pbySrc += BYTES_PER_PIXEL;
			}
		}

	} else {
		for (j = 0; j < cy; j ++) {
			pbyDst = pBitsDst + dwLineDst * (dy - j) + dx;
			pbySrc = pBitsSrc + dwLineSrc * (sy - j) + sx;
			for (i = 0; i < cx; i ++) {
				pbyDst[2] = pbyDst[2] + ((int)(pbySrc[2] - pbyDst[2]) * (byLevel + 1) >> 8);
				pbyDst[0] = pbyDst[0] + ((int)(pbySrc[0] - pbyDst[0]) * (byLevel + 1) >> 8);
				pbyDst[1] = pbyDst[1] + ((int)(pbySrc[1] - pbyDst[1]) * (byLevel + 1) >> 8);

				pbyDst += BYTES_PER_PIXEL;
				pbySrc += BYTES_PER_PIXEL;
			}
		}
	}
}

void CImg32::BltAlpha2(
	int dx,				// [in] 始点(X)
	int dy,				// [in] 始点(Y)
	int cx,				// [in] 幅
	int cy,				// [in] 高さ
	CImg32 *pSrc,		// [in] 転送元
	int sx,				// [in] 転送元の始点(X)
	int sy,				// [in] 転送元の始点(Y)
	BYTE byLevelAlpha,	// [in] 割合
	BOOL bColorKey)		// [in] カラーキーを使用
{
	int i, j;
	DWORD dwKey, dwLineDst, dwLineSrc;
	PBYTE pBitsDst, pBitsSrc, pbySrc, pbyDst;

	if ((m_pBits == NULL) || (pSrc == NULL)) {
		return;
	}

	// 座標をクリッピング
	ClipPos(dx, dy, cx, cy, pSrc, sx, sy);

	pBitsDst = m_pBits;
	pBitsSrc = pSrc->GetBits();
	dwLineDst = BYTES_PER_LINE(Width());
	dwLineSrc = BYTES_PER_LINE(pSrc->Width());
	dwKey = pSrc->GetColorKey();
	byLevelAlpha = PercentToHex(byLevelAlpha);

	dx *= BYTES_PER_PIXEL;
	dy = Height() - 1 - dy;
	sx *= BYTES_PER_PIXEL;
	sy = pSrc->Height() - 1 - sy;

	// カラーキーを使用する？
	if (bColorKey) {
		for (j = 0; j < cy; j ++) {
			pbyDst = pBitsDst + dwLineDst * (dy - j) + dx;
			pbySrc = pBitsSrc + dwLineSrc * (sy - j) + sx;
			for (i = 0; i < cx; i ++) {
				if (memcmp(&dwKey, pbySrc, BYTES_PER_PIXEL) == 0) {
					pbyDst[2] = pbyDst[2] + ((int)(pbySrc[2] - pbyDst[2]) * (byLevelAlpha + 1) >> 8);
					pbyDst[0] = pbyDst[0] + ((int)(pbySrc[0] - pbyDst[0]) * (byLevelAlpha + 1) >> 8);
					pbyDst[1] = pbyDst[1] + ((int)(pbySrc[1] - pbyDst[1]) * (byLevelAlpha + 1) >> 8);
				} else {
					CopyMemory(pbyDst, pbySrc, BYTES_PER_PIXEL);
				}
				pbyDst += BYTES_PER_PIXEL;
				pbySrc += BYTES_PER_PIXEL;
			}
		}

	} else {
		for (j = 0; j < cy; j ++) {
			pbyDst = pBitsDst + dwLineDst * (dy - j) + dx;
			pbySrc = pBitsSrc + dwLineSrc * (sy - j) + sx;
			for (i = 0; i < cx; i ++) {
				pbyDst[2] = pbyDst[2] + ((int)(pbySrc[2] - pbyDst[2]) * (byLevelAlpha + 1) >> 8);
				pbyDst[0] = pbyDst[0] + ((int)(pbySrc[0] - pbyDst[0]) * (byLevelAlpha + 1) >> 8);
				pbyDst[1] = pbyDst[1] + ((int)(pbySrc[1] - pbyDst[1]) * (byLevelAlpha + 1) >> 8);

				pbyDst += BYTES_PER_PIXEL;
				pbySrc += BYTES_PER_PIXEL;
			}
		}
	}
}

void CImg32::BltPlus(
	int dx,					// [in] 始点(X)
	int dy,					// [in] 始点(Y)
	int cx,					// [in] 幅
	int cy,					// [in] 高さ
	CImg32 *pSrc,			// [in] 転送元
	int sx,					// [in] 転送元の始点(X)
	int sy,					// [in] 転送元の始点(Y)
	int nMax,				// [in] 最大値
	BOOL bColorKey)/*FALSE*/	// [in] カラーキーを使用
{
	int i, j;
	PDWORD dst, src;
	DWORD dwKey, dwLineDst, dwLineSrc;
	PBYTE pBitsDst, pBitsSrc;

	if ((m_pBits == NULL) || (pSrc == NULL)) {
		return;
	}

	// 座標をクリッピング
	ClipPos(dx, dy, cx, cy, pSrc, sx, sy);

	pBitsDst = m_pBits;
	pBitsSrc = pSrc->GetBits();
	dwLineDst = BYTES_PER_LINE(Width());
	dwLineSrc = BYTES_PER_LINE(pSrc->Width());
	dwKey = pSrc->GetColorKey();

	dx *= BYTES_PER_PIXEL;
	dy = Height() - 1 - dy;
	sx *= BYTES_PER_PIXEL;
	sy = pSrc->Height() - 1 - sy;

	for (j = 0; j < cy; j ++) {
		dst = (PDWORD)(pBitsDst + dwLineDst * (dy - j) + dx);
		src = (PDWORD)(pBitsSrc + dwLineSrc * (sy - j) + sx);
		for (i = 0; i < cx; i ++) {
			if ((bColorKey == FALSE) || (bColorKey && dwKey != src[i])) {
				TARGB tSrc, tDst, *ptDst;
				tSrc.ARGB = src[i];
				tDst.ARGB = dst[i];
				ptDst = (TARGB *)&dst[i];
				if (tSrc.R != 0) {
					ptDst->R = min(tDst.R + tSrc.R, nMax);
				}
				if (tSrc.G != 0) {
					ptDst->G = min(tDst.G + tSrc.G, nMax);
				}
				if (tSrc.B != 0) {
					ptDst->B = min(tDst.B + tSrc.B, nMax);
				}
			}
		}
	}
}

void CImg32::BltTurnRightAndLeft(
	int dx,			// [in] 始点(X)
	int dy,			// [in] 始点(Y)
	int cx,			// [in] 幅
	int cy,			// [in] 高さ
	CImg32 *pSrc,	// [in] 転送元
	int sx,			// [in] 転送元の始点(X)
	int sy,			// [in] 転送元の始点(Y)
	BOOL bColorKey)	// [in] カラーキーを使用
{
	int i, j;
	PDWORD dst, src;
	DWORD dwKey, dwLineDst, dwLineSrc;
	PBYTE pBitsDst, pBitsSrc;

	if ((m_pBits == NULL) || (pSrc == NULL)) {
		return;
	}

	// 座標をクリッピング
	ClipPos(dx, dy, cx, cy, pSrc, sx, sy);

	pBitsDst = m_pBits;
	pBitsSrc = pSrc->GetBits();
	dwLineDst = BYTES_PER_LINE(Width());
	dwLineSrc = BYTES_PER_LINE(pSrc->Width());
	dwKey = pSrc->GetColorKey();

	dx *= BYTES_PER_PIXEL;
	dy = Height() - 1 - dy;
	sx *= BYTES_PER_PIXEL;
	sy = pSrc->Height() - 1 - sy;

	// カラーキーを使用する？
	if (bColorKey) {
		for (j = 0; j < cy; j ++) {
			dst = (PDWORD)(pBitsDst + dwLineDst * (dy - j) + dx);
			src = (PDWORD)(pBitsSrc + dwLineSrc * (sy - j) + (sx + cx - BYTES_PER_PIXEL));
			for (i = 0; i < cx; i ++) {
				if (dwKey != *src) {
					*dst = *src;
				}
				dst ++;
				src --;
			}
		}

	} else {
		for (j = 0; j < cy; j ++) {
			dst = (PDWORD)(pBitsDst + dwLineDst * (dy - j) + dx);
			src = (PDWORD)(pBitsSrc + dwLineSrc * (sy - j) + (sx + cx - BYTES_PER_PIXEL));
			for (i = 0; i < cx; i ++) {
				*dst = *src;

				dst ++;
				src --;
			}
		}
	}
}

void CImg32::BltTurnUpAndDown(
	int dx,			// [in] 始点(X)
	int dy,			// [in] 始点(Y)
	int cx,			// [in] 幅
	int cy,			// [in] 高さ
	CImg32 *pSrc,	// [in] 転送元
	int sx,			// [in] 転送元の始点(X)
	int sy,			// [in] 転送元の始点(Y)
	BOOL bColorKey)	// [in] カラーキーを使用
{
	int i, j;
	PDWORD dst, src;
	DWORD dwKey, dwLineDst, dwLineSrc;
	PBYTE pBitsDst, pBitsSrc;

	if ((m_pBits == NULL) || (pSrc == NULL)) {
		return;
	}

	// 座標をクリッピング
	ClipPos(dx, dy, cx, cy, pSrc, sx, sy);

	pBitsDst = m_pBits;
	pBitsSrc = pSrc->GetBits();
	dwLineDst = BYTES_PER_LINE(Width());
	dwLineSrc = BYTES_PER_LINE(pSrc->Width());
	dwKey = pSrc->GetColorKey();

	dx *= BYTES_PER_PIXEL;
	dy = Height() - 1 - dy;
	sx *= BYTES_PER_PIXEL;
	sy = pSrc->Height() - 1 - sy;

	// カラーキーを使用する？
	if (bColorKey) {
		for (j = 0; j < cy; j ++) {
			dst = (PDWORD)(pBitsDst + dwLineDst * (dy - j) + dx);
			src = (PDWORD)(pBitsSrc + dwLineSrc * (sy - cy + j - 1) + sx);
			for (i = 0; i < cx; i ++) {
				if (dwKey != *src) {
					*dst = *src;
				}
				dst ++;
				src ++;
			}
		}

	} else {
		for (j = 0; j < cy; j ++) {
			dst = (PDWORD)(pBitsDst + dwLineDst * (dy - j) + dx);
			src = (PDWORD)(pBitsSrc + dwLineSrc * (sy - cy + j - 1) + sx);
			for (i = 0; i < cx; i ++) {
				*dst = *src;

				dst ++;
				src ++;
			}
		}
	}
}

HDC CImg32::Lock(void)
{
	HDC hDC, hRet;

	hRet = NULL;
	// オブジェクトが不正
	if ((m_hBmp == NULL) || (m_hBmp == HBITMAP_WITHOUT_GDI)) {
		goto Exit;
	}

	// すでにロックしている
	if (m_hDC) {
		hRet = m_hDC;
		goto Exit;
	}

	// コンパチブル DC を作成して、ビットマップを選択
	hDC = GetDC(NULL);
	m_hDC = CreateCompatibleDC(hDC);
	ReleaseDC(NULL, hDC);
	if (m_hDC) {
		m_hBmpBack = (HBITMAP)SelectObject(m_hDC, m_hBmp);
	}
	hRet = m_hDC;

Exit:
	return hRet;
}

void CImg32::Unlock(void)
{
	if (m_hDC == NULL) {
		return;
	}

	SelectObject(m_hDC, m_hBmpBack);
	DeleteDC(m_hDC);
	m_hDC = NULL;
}

HBITMAP CImg32::GetSafeHandle(void)
{
	return m_hBmp;
}

int CImg32::Width(void)
{
	int nRet;

	nRet = 0;
	if (m_hBmp == NULL) {
		goto Exit;
	}

	nRet = m_bmpInfo.bmiHeader.biWidth;

Exit:
	return nRet;
}

int CImg32::Height(void)
{
	int nRet;

	nRet = 0;
	if (m_hBmp == NULL) {
		goto Exit;
	}

	nRet = m_bmpInfo.bmiHeader.biHeight;

Exit:
	return nRet;
}

int CImg32::GetImageSize(void)
{
	int nRet, cx, cy;

	nRet = 0;
	if (m_hBmp == NULL) {
		goto Exit;
	}

	cx = Width();
	cy = Height();
	nRet = BYTES_PER_LINE(cx) * cy;

Exit:
	return nRet;
}

LPBYTE CImg32::GetBits(void)
{
	return m_pBits;
}

void CImg32::SetColorKey(COLORREF clKey)
{
	m_dwColorKey = ColorrefToDword(clKey);
}

COLORREF CImg32::GetColorKey(void)
{
	COLORREF clRet;
	TARGB tTmp;

	tTmp.ARGB = m_dwColorKey;
	clRet = RGB(tTmp.R, tTmp.G, tTmp.B);

	return clRet;
}

BOOL CImg32::IsMode256(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_pPallet) {
		// パレットがあったら256色画像とする
		bRet = TRUE;
	}

	return bRet;
}

void CImg32::SetColorKeyNo(BYTE byColorKeyNo)
{
	m_byColorKey = byColorKeyNo;
}

BYTE CImg32::GetColorKeyNo(void)
{
	return m_byColorKey;
}

void CImg32::SetPallet(BYTE byNo, BYTE r, BYTE g, BYTE b)
{
	PIMG32RGB pPallet;

	pPallet = (PIMG32RGB)&m_pPallet[byNo * 3];
	pPallet->r = r;
	pPallet->g = g;
	pPallet->b = b;
}

PBYTE CImg32::GetPallet(BYTE byNo)
{
	return &m_pPallet[byNo * 3];
}

void CImg32::FillRect256(int x, int y, int cx, int cy, BYTE byPallet)
{
	int i, j;
	DWORD dwLineDst;
	PBYTE pTmp;

	if (m_pBits == NULL) {
		return;
	}

	x = min(x, Width());
	x = max(x, 0);
	y = min(y, Height());
	y = max(y, 0);
	if (x + cx > Width()) {
		cx = Width() - x;
	}
	if (y + cy > Height()) {
		cy = Height() - y;
	}

	dwLineDst = Width();
	y = Height() - 1 - y;

	for (j = 0; j < cy; j ++) {
		pTmp = (m_pBits + dwLineDst * (y - j) + x);
		for (i = 0; i < cx; i ++) {
			pTmp[i] = byPallet;
		}
	}
}

void CImg32::BltFrom256(
	int dx,			// [in] 始点(X)
	int dy,			// [in] 始点(Y)
	int cx,			// [in] 幅
	int cy,			// [in] 高さ
	CImg32 *pSrc,	// [in] 転送元
	int sx,			// [in] 転送元の始点(X)
	int sy,			// [in] 転送元の始点(Y)
	BOOL bColorKey)	// [in] カラーキーを使用
{
	int i, j;
	BYTE byKey;
	DWORD dwLineDst, dwLineSrc;
	PBYTE pBitsDst, pBitsSrc, dst, src;
	PIMG32RGB pPallet;

	if ((m_pBits == NULL) || (pSrc == NULL)) {
		return;
	}

	// 座標をクリッピング
	ClipPos(dx, dy, cx, cy, pSrc, sx, sy);

	pBitsDst = m_pBits;
	pBitsSrc = pSrc->GetBits();
	dwLineDst = BYTES_PER_LINE(Width());
	dwLineSrc = pSrc->Width();
	byKey = pSrc->GetColorKeyNo();

	dx *= BYTES_PER_PIXEL;
	dy = Height() - 1 - dy;
	sy = pSrc->Height() - 1 - sy;
	pPallet = (PIMG32RGB)pSrc->GetPallet(0);

	for (j = 0; j < cy; j ++) {
		dst = (pBitsDst + dwLineDst * (dy - j) + dx);
		src = (pBitsSrc + dwLineSrc * (sy - j) + sx);
		for (i = 0; i < cx; i ++) {
			if ((bColorKey == FALSE) || (byKey != src[i])) {
				CopyMemory(dst, &pPallet[src[i]], 3);
			}
			dst += 4;
		}
	}
}

void CImg32::BltAlphaFrom256(
	int dx,			// [in] 始点(X)
	int dy,			// [in] 始点(Y)
	int cx,			// [in] 幅
	int cy,			// [in] 高さ
	CImg32 *pSrc,	// [in] 転送元
	int sx,			// [in] 転送元の始点(X)
	int sy,			// [in] 転送元の始点(Y)
	BYTE byLevel,	// [in] 割合
	BOOL bColorKey)	// [in] カラーキーを使用
{
	int i, j;
	BYTE byKey;
	DWORD dwLineDst, dwLineSrc;
	PBYTE pBitsDst, pBitsSrc, dst, src;
	PIMG32RGB pPallet;

	if ((m_pBits == NULL) || (pSrc == NULL)) {
		return;
	}

	// 座標をクリッピング
	ClipPos(dx, dy, cx, cy, pSrc, sx, sy);

	pBitsDst = m_pBits;
	pBitsSrc = pSrc->GetBits();
	dwLineDst = BYTES_PER_LINE(Width());
	dwLineSrc = pSrc->Width();
	byKey = pSrc->GetColorKeyNo();
	byLevel = PercentToHex(byLevel);

	dx *= BYTES_PER_PIXEL;
	dy = Height() - 1 - dy;
	sy = pSrc->Height() - 1 - sy;
	pPallet = (PIMG32RGB)pSrc->GetPallet(0);

	for (j = 0; j < cy; j ++) {
		dst = (pBitsDst + dwLineDst * (dy - j) + dx);
		src = (pBitsSrc + dwLineSrc * (sy - j) + sx);
		for (i = 0; i < cx; i ++) {
			if (byKey != src[i]) {
				dst[0] = dst[0] + ((int)(pPallet[src[i]].b - dst[0]) * (byLevel + 1) >> 8);
				dst[1] = dst[1] + ((int)(pPallet[src[i]].g - dst[1]) * (byLevel + 1) >> 8);
				dst[2] = dst[2] + ((int)(pPallet[src[i]].r - dst[2]) * (byLevel + 1) >> 8);
			}
			dst += 4;
		}
	}
}

void CImg32::InitData(void)
{
	m_hBmp = NULL;
	m_hBmpBack = NULL;
	m_pBits = NULL;
	m_hDC = NULL;
	m_dwColorKey = 0;
	m_pPallet = NULL;
	ZeroMemory(&m_bmpInfo, sizeof(m_bmpInfo));
}

DWORD CImg32::ColorrefToDword(COLORREF cl)
{
	DWORD dwRet;

	dwRet  = (cl & 0x000000FF) << 16;
	dwRet |= (cl & 0x0000FF00) << 0;
	dwRet |= (cl & 0x00FF0000) >> 16;

	return dwRet;
}

BYTE CImg32::PercentToHex(BYTE byPercent)
{
	BYTE byRet;

	byRet = 255 - (byPercent * 255 / 100);

	return byRet;
}

void CImg32::ClipPos(
	int &dx,		// [in,out] 始点(X)
	int &dy,		// [in,out] 始点(Y)
	int &cx,		// [in,out] 幅
	int &cy,		// [in,out] 高さ
	CImg32 *pSrc,	// [in]     転送元
	int &sx,		// [in,out] 転送元の始点(X)
	int &sy)		// [in,out] 転送元の始点(Y)
{
	int dxBack, dyBack, cxBack, cyBack, sxBack, syBack;
	RECT rc1, rc2, rcTmp;

	dxBack = dx;
	dyBack = dy;
	cxBack = cx;
	cyBack = cy;
	sxBack = sx;
	syBack = sy;

	// 転送元をクリッピング
	SetRect(&rc1, sx, sy, sx + cx, sy + cy);
	SetRect(&rc2, 0, 0, pSrc->Width(), pSrc->Height());
	IntersectRect(&rcTmp, &rc1, &rc2);
	sx = rcTmp.left;
	sy = rcTmp.top;
	cx = rcTmp.right - rcTmp.left;
	cy = rcTmp.bottom - rcTmp.top;

	// 転送先をクリッピング
	SetRect(&rc1, dx, dy, dx + cx, dy + cy);
	SetRect(&rc2, 0, 0, Width(), Height());
	IntersectRect(&rcTmp, &rc1, &rc2);
	dx = rcTmp.left;
	dy = rcTmp.top;
	cx = rcTmp.right - rcTmp.left;
	cy = rcTmp.bottom - rcTmp.top;

	if (dy > dyBack) {
		sy += (cyBack - cy);
	}
	if (dx > dxBack) {
		sx += (cxBack - cx);
	}
}
