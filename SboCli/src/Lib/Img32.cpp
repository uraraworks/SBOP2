/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C�����F	Img32.cpp													 */
/* ���e�F		32bit�C���[�W�N���X �����t�@�C��							 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2005/01/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include <math.h>
#include "Img32.h"

/* ========================================================================= */
/* �萔�̒�`																 */
/* ========================================================================= */

#define HBITMAP_WITHOUT_GDI	((HBITMAP)-1)						/* ��݊����[�h���̃n���h���l */
#define BYTES_PER_PIXEL		(4)									/* 1�h�b�g�̃o�C�g�� */
#define BITS_PER_PIXEL		(BYTES_PER_PIXEL * 8)				/* 1�h�b�g�̃r�b�g�� */
#define	BYTES_PER_LINE(cx)	(cx * BYTES_PER_PIXEL)				/* 1���C���̃o�C�g�� */


/* ========================================================================= */
/* �\���̂̒�`																 */
/* ========================================================================= */

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


/* ========================================================================= */
/* �֐����F	CImg32::CImg32													 */
/* ���e�F	�R���X�g���N�^													 */
/* ���t�F	2005/01/24														 */
/* ========================================================================= */
CImg32::CImg32(void)
{
	InitData ();
}


/* ========================================================================= */
/* �֐����F	CImg32::~CImg32													 */
/* ���e�F	�f�X�g���N�^													 */
/* ���t�F	2005/01/24														 */
/* ========================================================================= */
CImg32::~CImg32()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐����F	CImg32::Create													 */
/* ���e�F	�r�b�g�}�b�v���쐬												 */
/* ���t�F	2005/01/24														 */
/* ========================================================================= */
BOOL CImg32::Create(
	int cx,		/* [in] �� */
	int cy)		/* [in] ���� */
{
	BOOL bRet;
	HDC hDC;

	bRet = FALSE;

	if (m_hBmp) {
		goto Exit;
	}

	/* �r�b�g�}�b�v����ݒ� */
	ZeroMemory (&m_bmpInfo, sizeof (m_bmpInfo));
	m_bmpInfo.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	m_bmpInfo.bmiHeader.biWidth			= cx;
	m_bmpInfo.bmiHeader.biHeight		= cy;
	m_bmpInfo.bmiHeader.biPlanes		= 1;
	m_bmpInfo.bmiHeader.biBitCount		= BYTES_PER_PIXEL * 8;
	m_bmpInfo.bmiHeader.biCompression	= BI_RGB;

	/* DIB �Z�N�V�����I�u�W�F�N�g���쐬 */
	hDC = GetDC (NULL);
	m_hBmp = CreateDIBSection (hDC, &m_bmpInfo, DIB_RGB_COLORS, (PVOID*)&m_pBits, 0, 0);
	ReleaseDC (NULL, hDC);

	if (m_hBmp == FALSE) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐����F	CImg32::CreateWithoutGdi										 */
/* ���e�F	�r�b�g�}�b�v���쐬												 */
/* ���t�F	2005/01/24														 */
/* ========================================================================= */
BOOL CImg32::CreateWithoutGdi(
	int cx,							/* [in] �� */
	int cy,							/* [in] ���� */
	LPBYTE pBits /* = NULL */)		/* [in] �r�b�g�f�[�^ */
{
	BOOL bRet;

	bRet = FALSE;

	if (m_hBmp) {
		goto Exit;
	}

	/* �r�b�g�}�b�v����ݒ� */
	ZeroMemory (&m_bmpInfo, sizeof (m_bmpInfo));
	m_bmpInfo.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	m_bmpInfo.bmiHeader.biWidth		= cx;
	m_bmpInfo.bmiHeader.biHeight	= cy;

	/* �r�b�g�f�[�^���w�肳��Ă���H */
	if (pBits) {
		m_pBits = pBits;

	} else {
		DWORD dwSize;

		dwSize = BYTES_PER_LINE(cx) * cy;
		m_pBits = new BYTE [dwSize];
		if (m_pBits == NULL) {
			goto Exit;
		}
		ZeroMemory (m_pBits, dwSize);
	}

	/* �_�~�[�̃n���h����ݒ� */
	m_hBmp = HBITMAP_WITHOUT_GDI;

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐����F	CImg32::CreateWithoutGdi256										 */
/* ���e�F	GDI��݊��ō쐬(256�F)											 */
/* ���t�F	2008/07/04														 */
/* ========================================================================= */
BOOL CImg32::CreateWithoutGdi256(
	int cx,		/* [in] �� */
	int cy)		/* [in] ���� */
{
	BOOL bRet;

	bRet = FALSE;

	if (m_hBmp) {
		goto Exit;
	}

	m_byColorKey = 0;

	m_pPallet = new BYTE[256 * 3];
	ZeroMemory (m_pPallet, 256 * 3);

	m_pBits = new BYTE[cx * cy];
	ZeroMemory (m_pBits, cx * cy);

	/* �r�b�g�}�b�v����ݒ� */
	ZeroMemory (&m_bmpInfo, sizeof (m_bmpInfo));
	m_bmpInfo.bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
	m_bmpInfo.bmiHeader.biWidth		= cx;
	m_bmpInfo.bmiHeader.biHeight	= cy;

	/* �_�~�[�̃n���h����ݒ� */
	m_hBmp = HBITMAP_WITHOUT_GDI;

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐����F	CImg32::Destroy													 */
/* ���e�F	�j��															 */
/* ���t�F	2005/01/24														 */
/* ========================================================================= */
void CImg32::Destroy(void)
{
	Unlock ();

	/* DIB �Z�N�V�����I�u�W�F�N�g��j�� */
	if (m_hBmp) {
		if (m_hBmp != HBITMAP_WITHOUT_GDI) {
			DeleteObject (m_hBmp);

		} else {
			delete [] m_pBits;
		}
	}

	if (m_pPallet) {
		delete [] m_pPallet;
	}

	InitData ();
}


/* ========================================================================= */
/* �֐����F	CImg32::Clear													 */
/* ���e�F	�N���A															 */
/* ���t�F	2005/01/26														 */
/* ========================================================================= */
void CImg32::Clear(void)
{
	int cx, cy, bpl;

	if (m_pBits == NULL) {
		return;
	}

	cx = Width ();
	cy = Height ();
	bpl = BYTES_PER_LINE(cx);
	if (IsMode256 ()) {
		bpl = cx;
	}
	ZeroMemory (m_pBits, bpl * cy);
}


/* ========================================================================= */
/* �֐����F	CImg32::FillRect												 */
/* ���e�F	��`�h��Ԃ�													 */
/* ���t�F	2005/01/26														 */
/* ========================================================================= */
void CImg32::FillRect(
	int x,					/* [in] �n�_(X) */
	int y,					/* [in] �n�_(Y) */
	int cx,					/* [in] �� */
	int cy,					/* [in] ���� */
	COLORREF clFill)		/* [in] �h��Ԃ��F */
{
	int i, j;
	PDWORD dst;
	DWORD dwTmp, dwLineDst;
	PBYTE pBitsDst;

	if (m_pBits == NULL) {
		return;
	}

	x = min (x, Width ());
	x = max (x, 0);
	y = min (y, Height ());
	y = max (y, 0);
	if (x + cx > Width ()) {
		cx = Width () - x;
	}
	if (y + cy > Height ()) {
		cy = Height () - y;
	}

	pBitsDst	= m_pBits;
	dwLineDst	= BYTES_PER_LINE(Width ());
	dwTmp = ColorrefToDword (clFill);

	x *= BYTES_PER_PIXEL;
	y = Height () - 1 - y;

	for (j = 0; j < cy; j ++) {
		dst = (PDWORD)(pBitsDst + dwLineDst * (y - j) + x);
		for (i = 0; i < cx; i ++) {
			*dst = dwTmp;
			dst ++;
		}
	}
}


/* ========================================================================= */
/* �֐���	:CImg32::Rectangle												 */
/* ���e		:��`															 */
/* ���t		:2008/04/14														 */
/* ========================================================================= */
void CImg32::Rectangle(
	int x,					/* [in] �n�_(X) */
	int y,					/* [in] �n�_(Y) */
	int cx,					/* [in] �� */
	int cy,					/* [in] ���� */
	COLORREF clDraw)		/* [in] �`��F(-1�ŐF���]) */
{
	int i, j;
	PDWORD dst;
	DWORD dwTmp, dwLineDst;
	PBYTE pBitsDst;

	if (m_pBits == NULL) {
		return;
	}

	x = min (x, Width ());
	x = max (x, 0);
	y = min (y, Height ());
	y = max (y, 0);
	if (x + cx > Width ()) {
		cx = Width () - x;
	}
	if (y + cy > Height ()) {
		cy = Height () - y;
	}

	pBitsDst	= m_pBits;
	dwLineDst	= BYTES_PER_LINE(Width ());
	dwTmp = ColorrefToDword (clDraw);
	if (clDraw == -1) {
		dwTmp = -1;
	}

	x *= BYTES_PER_PIXEL;
	y = Height () - 1 - y;

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


/* ========================================================================= */
/* �֐���	:CImg32::Circle													 */
/* ���e		:�~																 */
/* ���t		:2008/09/14														 */
/* ========================================================================= */
void CImg32::Circle(
	int x,					/* [in] �n�_(X) */
	int y,					/* [in] �n�_(Y) */
	int r,					/* [in] ���a */
	COLORREF clDraw)		/* [in] �`��F(-1�ŐF���]) */
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

	x = min (x, Width ());
	x = max (x, 0);
	y = min (y, Height ());
	y = max (y, 0);
	if (x + cx > Width ()) {
		cx = Width () - x;
	}
	if (y + cy > Height ()) {
		cy = Height () - y;
	}
	/* ���S�_�̐ݒ� */
	mx = x + cx / 2;
	my = y + cy / 2;

	pBitsDst	= m_pBits;
	dwLineDst	= BYTES_PER_LINE(Width ());
	dwTmp = ColorrefToDword (clDraw);
	if (clDraw == -1) {
		dwTmp = -1;
	}

	xxx = x * BYTES_PER_PIXEL;
	yyy = Height () - 1 - y;

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


/* ========================================================================= */
/* �֐���	:CImg32::CircleGradation										 */
/* ���e		:�~(�O���f�[�V����)												 */
/* ���t		:2008/09/20														 */
/* ========================================================================= */
void CImg32::CircleGradation(
	int x,					/* [in] �n�_(X) */
	int y,					/* [in] �n�_(Y) */
	int r,					/* [in] ���a */
	int rr,					/* [in] �O���f�[�V�������Ȃ����a */
	COLORREF clDraw)		/* [in] �`��F(-1�ŐF���]) */
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

	x = min (x, Width ());
	x = max (x, 0);
	y = min (y, Height ());
	y = max (y, 0);
	if (x + cx > Width ()) {
		cx = Width () - x;
	}
	if (y + cy > Height ()) {
		cy = Height () - y;
	}
	/* ���S�_�̐ݒ� */
	mx = x + cx / 2;
	my = y + cy / 2;

	pBitsDst	= m_pBits;
	dwLineDst	= BYTES_PER_LINE(Width ());
	dwTmp = ColorrefToDword (clDraw);
	if (clDraw == -1) {
		dwTmp = -1;
	}

	xxx = x * BYTES_PER_PIXEL;
	yyy = Height () - 1 - y;
	dr = r - rr;

	for (j = 0; j < cy; j ++) {
		yy = y + j;
		dst = (PDWORD)(pBitsDst + dwLineDst * (yyy - j) + xxx);
		for (i = 0; i < cx; i ++) {
			xx = x + i;
			if ((xx - mx) * (xx - mx) + (yy - my) * (yy - my) <= r * r) {
				TARGB tTmp;
				dTmp = (abs (j - r) * abs (j - r) + abs (i - r) * abs (i - r));
				dTmp = max (dTmp, 1);
				dTmp = sqrt (dTmp);
				if ((int)dTmp < rr) {
					nLevel = 0;
				} else {
					nLevel = (int)((dTmp - rr) * 100 / dr);
					nLevel = min (nLevel, 100);
					nLevel = max (nLevel, 1);
				}
				nLevel = 100 - nLevel;
				byLevel = PercentToHex ((BYTE)nLevel);

				tTmp.ARGB = dwTmp;
//				nTmp = (int)((TARGB*)&dst[i])->R + ((int)tTmp.R + ((int)(-tTmp.R) * ((int)byLevel + 1) >> 8)) / 2 * 2;
//				nTmp = max (nTmp, 0);
//				tTmp.R = min (nTmp, 100);
//				nTmp = ((TARGB*)&dst[i])->G + (tTmp.G + ((int)(-tTmp.G) * (byLevel + 1) >> 8)) / 2 * 2;
//				nTmp = max (nTmp, 0);
//				tTmp.G = min (nTmp, 100);
				nTmp = ((TARGB*)&dst[i])->B + (tTmp.B + ((int)(-tTmp.B) * (byLevel + 1) >> 8)) / 2 * 2;
				nTmp = max (nTmp, 0);
				tTmp.B = min (nTmp, 100);
				dst[i] = tTmp.ARGB;
			}
		}
	}
}


/* ========================================================================= */
/* �֐����F	CImg32::ChgLevel												 */
/* ���e�F	���x��ύX														 */
/* ���t�F	2005/01/26														 */
/* ========================================================================= */
void CImg32::ChgLevel(
	int x,				/* [in] �n�_(X) */
	int y,				/* [in] �n�_(Y) */
	int cx,				/* [in] �� */
	int cy,				/* [in] ���� */
	BYTE byLevel)		/* [in] ���x���x�� */
{
	int i, j;
	PDWORD dst;
	DWORD dwLineDst;
	PBYTE pBitsDst;

	if (m_pBits == NULL) {
		return;
	}

	x = min (x, Width ());
	x = max (x, 0);
	y = min (y, Height ());
	y = max (y, 0);
	if (x + cx > Width ()) {
		cx = Width () - x;
	}
	if (y + cy > Height ()) {
		cy = Height () - y;
	}

	pBitsDst	= m_pBits;
	dwLineDst	= BYTES_PER_LINE(Width ());
	byLevel = PercentToHex (byLevel);

	x *= BYTES_PER_PIXEL;
	y = Height () - 1 - y;

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


/* ========================================================================= */
/* �֐����F	CImg32::ChgColor												 */
/* ���e�F	�F�f��ύX														 */
/* ���t�F	2005/01/26														 */
/* ========================================================================= */
void CImg32::ChgColor(
	int x,				/* [in] �n�_(X) */
	int y,				/* [in] �n�_(Y) */
	int cx,				/* [in] �� */
	int cy,				/* [in] ���� */
	COLORREF clChg)		/* [in] �F�f */
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
		cx = Width ();
	}
	if (cy == 0) {
		cy = Height ();
	}

	x = min (x, Width ());
	x = max (x, 0);
	y = min (y, Height ());
	y = max (y, 0);
	if (x + cx > Width ()) {
		cx = Width () - x;
	}
	if (y + cy > Height ()) {
		cy = Height () - y;
	}

	pBitsDst	= m_pBits;
	dwLineDst	= BYTES_PER_LINE(Width ());
	tChg.ARGB = ColorrefToDword (clChg);

	x *= BYTES_PER_PIXEL;
	y = Height () - 1 - y;

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


/* ========================================================================= */
/* �֐���	:CImg32::SetLevel												 */
/* ���e		:�w��摜��R�l�𖾓x�Ƃ��Ĕ��f									 */
/* ���t		:2008/09/20														 */
/* ========================================================================= */
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
	cx = Width ();
	cy = Height ();

	pBitsSrc	= pSrc->GetBits ();
	pBitsDst	= m_pBits;
	dwLineDst	= BYTES_PER_LINE(Width ());

	x *= BYTES_PER_PIXEL;
	y = Height () - 1 - y;

	for (j = 0; j < cy; j ++) {
		dst = (PDWORD)(pBitsDst + dwLineDst * (y - j) + x);
		psrc = (PDWORD)(pBitsSrc + dwLineDst * (y - j) + x);
		for (i = 0; i < cx; i ++) {
			TARGB tTmp, *ptDst;

			ptDst = (TARGB *)&dst[i];
			byLevel = PercentToHex (((TARGB *)(&psrc[i]))->B);
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


/* ========================================================================= */
/* �֐����F	CImg32::Blt														 */
/* ���e�F	��`�]��														 */
/* ���t�F	2005/01/26														 */
/* ========================================================================= */
void CImg32::Blt(
	int dx,				/* [in] �n�_(X) */
	int dy,				/* [in] �n�_(Y) */
	int cx,				/* [in] �� */
	int cy,				/* [in] ���� */
	CImg32 *pSrc,		/* [in] �]���� */
	int sx,				/* [in] �]�����̎n�_(X) */
	int sy,				/* [in] �]�����̎n�_(Y) */
	BOOL bColorKey)		/* [in] �J���[�L�[���g�p */
{
	int i, j;
	PDWORD dst, src;
	DWORD dwKey, dwLineDst, dwLineSrc;
	PBYTE pBitsDst, pBitsSrc;

	if ((m_pBits == NULL) || (pSrc == NULL)) {
		return;
	}

	/* ���W���N���b�s���O */
	ClipPos (dx, dy, cx, cy, pSrc, sx, sy);

	pBitsDst	= m_pBits;
	pBitsSrc	= pSrc->GetBits ();
	dwLineDst	= BYTES_PER_LINE(Width ());
	dwLineSrc	= BYTES_PER_LINE(pSrc->Width ());
	dwKey		= pSrc->GetColorKey ();

	dx	*= BYTES_PER_PIXEL;
	dy	= Height () - 1 - dy;
	sx	*= BYTES_PER_PIXEL;
	sy	= pSrc->Height () - 1 - sy;

	/* �J���[�L�[���g�p����H */
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
			CopyMemory (
				pBitsDst + dwLineDst * (dy - j) + dx,
				pBitsSrc + dwLineSrc * (sy - j) + sx,
				cx * BYTES_PER_PIXEL);
		}
	}
}


/* ========================================================================= */
/* �֐����F	CImg32::BltLevel												 */
/* ���e�F	���x��ς��Ȃ����`�]��										 */
/* ���t�F	2005/01/26														 */
/* ========================================================================= */
void CImg32::BltLevel(
	int dx,				/* [in] �n�_(X) */
	int dy,				/* [in] �n�_(Y) */
	int cx,				/* [in] �� */
	int cy,				/* [in] ���� */
	CImg32 *pSrc,		/* [in] �]���� */
	int sx,				/* [in] �]�����̎n�_(X) */
	int sy,				/* [in] �]�����̎n�_(Y) */
	BYTE byLevel,		/* [in] ���x */
	BOOL bColorKey)		/* [in] �J���[�L�[���g�p */
{
	int i, j;
	DWORD dwKey, dwLineDst, dwLineSrc;
	PBYTE pBitsDst, pBitsSrc, pbySrc, pbyDst;

	if (byLevel == 100) {
		Blt (dx, dy, cx, cy, pSrc, sx, sy, bColorKey);
		return;
	}

	if ((m_pBits == NULL) || (pSrc == NULL)) {
		return;
	}

	/* ���W���N���b�s���O */
	ClipPos (dx, dy, cx, cy, pSrc, sx, sy);

	pBitsDst	= m_pBits;
	pBitsSrc	= pSrc->GetBits ();
	dwLineDst	= BYTES_PER_LINE(Width ());
	dwLineSrc	= BYTES_PER_LINE(pSrc->Width ());
	dwKey		= pSrc->GetColorKey ();
	byLevel		= PercentToHex (byLevel);

	dx	*= BYTES_PER_PIXEL;
	dy	= Height () - 1 - dy;
	sx	*= BYTES_PER_PIXEL;
	sy	= pSrc->Height () - 1 - sy;

	/* �J���[�L�[���g�p����H */
	if (bColorKey) {
		for (j = 0; j < cy; j ++) {
			pbyDst = pBitsDst + dwLineDst * (dy - j) + dx;
			pbySrc = pBitsSrc + dwLineSrc * (sy - j) + sx;
			for (i = 0; i < cx; i ++) {
				if (memcmp (&dwKey, pbySrc, BYTES_PER_PIXEL) != 0) {
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


/* ========================================================================= */
/* �֐����F	CImg32::BltAlpha												 */
/* ���e�F	�������킹��`�]��												 */
/* ���t�F	2005/01/26														 */
/* ========================================================================= */
void CImg32::BltAlpha(
	int dx,				/* [in] �n�_(X) */
	int dy,				/* [in] �n�_(Y) */
	int cx,				/* [in] �� */
	int cy,				/* [in] ���� */
	CImg32 *pSrc,		/* [in] �]���� */
	int sx,				/* [in] �]�����̎n�_(X) */
	int sy,				/* [in] �]�����̎n�_(Y) */
	BYTE byLevel,		/* [in] ���� */
	BOOL bColorKey)		/* [in] �J���[�L�[���g�p */
{
	int i, j;
	DWORD dwKey, dwLineDst, dwLineSrc;
	PBYTE pBitsDst, pBitsSrc, pbySrc, pbyDst;

	if ((m_pBits == NULL) || (pSrc == NULL)) {
		return;
	}

	/* ���W���N���b�s���O */
	ClipPos (dx, dy, cx, cy, pSrc, sx, sy);

	pBitsDst	= m_pBits;
	pBitsSrc	= pSrc->GetBits ();
	dwLineDst	= BYTES_PER_LINE(Width ());
	dwLineSrc	= BYTES_PER_LINE(pSrc->Width ());
	dwKey		= pSrc->GetColorKey ();
	byLevel		= PercentToHex (byLevel);

	dx	*= BYTES_PER_PIXEL;
	dy	= Height () - 1 - dy;
	sx	*= BYTES_PER_PIXEL;
	sy	= pSrc->Height () - 1 - sy;

	/* �J���[�L�[���g�p����H */
	if (bColorKey) {
		for (j = 0; j < cy; j ++) {
			pbyDst = pBitsDst + dwLineDst * (dy - j) + dx;
			pbySrc = pBitsSrc + dwLineSrc * (sy - j) + sx;
			for (i = 0; i < cx; i ++) {
				if (memcmp (&dwKey, pbySrc, BYTES_PER_PIXEL)) {
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


/* ========================================================================= */
/* �֐����F	CImg32::BltAlpha2												 */
/* ���e�F	�J���[�L�[�̂ݍ������킹��`�]��								 */
/* ���t�F	2005/01/26														 */
/* ========================================================================= */
void CImg32::BltAlpha2(
	int dx,				/* [in] �n�_(X) */
	int dy,				/* [in] �n�_(Y) */
	int cx,				/* [in] �� */
	int cy,				/* [in] ���� */
	CImg32 *pSrc,		/* [in] �]���� */
	int sx,				/* [in] �]�����̎n�_(X) */
	int sy,				/* [in] �]�����̎n�_(Y) */
	BYTE byLevelAlpha,	/* [in] ���� */
	BOOL bColorKey)		/* [in] �J���[�L�[���g�p */
{
	int i, j;
	DWORD dwKey, dwLineDst, dwLineSrc;
	PBYTE pBitsDst, pBitsSrc, pbySrc, pbyDst;

	if ((m_pBits == NULL) || (pSrc == NULL)) {
		return;
	}

	/* ���W���N���b�s���O */
	ClipPos (dx, dy, cx, cy, pSrc, sx, sy);

	pBitsDst		= m_pBits;
	pBitsSrc		= pSrc->GetBits ();
	dwLineDst		= BYTES_PER_LINE(Width ());
	dwLineSrc		= BYTES_PER_LINE(pSrc->Width ());
	dwKey			= pSrc->GetColorKey ();
	byLevelAlpha	= PercentToHex (byLevelAlpha);

	dx	*= BYTES_PER_PIXEL;
	dy	= Height () - 1 - dy;
	sx	*= BYTES_PER_PIXEL;
	sy	= pSrc->Height () - 1 - sy;

	/* �J���[�L�[���g�p����H */
	if (bColorKey) {
		for (j = 0; j < cy; j ++) {
			pbyDst = pBitsDst + dwLineDst * (dy - j) + dx;
			pbySrc = pBitsSrc + dwLineSrc * (sy - j) + sx;
			for (i = 0; i < cx; i ++) {
				if (memcmp (&dwKey, pbySrc, BYTES_PER_PIXEL) == 0) {
					pbyDst[2] = pbyDst[2] + ((int)(pbySrc[2] - pbyDst[2]) * (byLevelAlpha + 1) >> 8);
					pbyDst[0] = pbyDst[0] + ((int)(pbySrc[0] - pbyDst[0]) * (byLevelAlpha + 1) >> 8);
					pbyDst[1] = pbyDst[1] + ((int)(pbySrc[1] - pbyDst[1]) * (byLevelAlpha + 1) >> 8);
				} else {
					CopyMemory (pbyDst, pbySrc, BYTES_PER_PIXEL);
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


/* ========================================================================= */
/* �֐����F	CImg32::BltPlus													 */
/* ���e�F	��`�]��														 */
/* ���t�F	2005/01/26														 */
/* ========================================================================= */
void CImg32::BltPlus(
	int dx,						/* [in] �n�_(X) */
	int dy,						/* [in] �n�_(Y) */
	int cx,						/* [in] �� */
	int cy,						/* [in] ���� */
	CImg32 *pSrc,				/* [in] �]���� */
	int sx,						/* [in] �]�����̎n�_(X) */
	int sy,						/* [in] �]�����̎n�_(Y) */
	int nMax,					/* [in] �ő�l */
	BOOL bColorKey)/*FALSE*/	/* [in] �J���[�L�[���g�p */
{
	int i, j;
	PDWORD dst, src;
	DWORD dwKey, dwLineDst, dwLineSrc;
	PBYTE pBitsDst, pBitsSrc;

	if ((m_pBits == NULL) || (pSrc == NULL)) {
		return;
	}

	/* ���W���N���b�s���O */
	ClipPos (dx, dy, cx, cy, pSrc, sx, sy);

	pBitsDst	= m_pBits;
	pBitsSrc	= pSrc->GetBits ();
	dwLineDst	= BYTES_PER_LINE(Width ());
	dwLineSrc	= BYTES_PER_LINE(pSrc->Width ());
	dwKey		= pSrc->GetColorKey ();

	dx	*= BYTES_PER_PIXEL;
	dy	= Height () - 1 - dy;
	sx	*= BYTES_PER_PIXEL;
	sy	= pSrc->Height () - 1 - sy;

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
					ptDst->R = min (tDst.R + tSrc.R, nMax);
				}
				if (tSrc.G != 0) {
					ptDst->G = min (tDst.G + tSrc.G, nMax);
				}
				if (tSrc.B != 0) {
					ptDst->B = min (tDst.B + tSrc.B, nMax);
				}
			}
		}
	}
}


/* ========================================================================= */
/* �֐����F	CImg32::BltTurnRightAndLeft										 */
/* ���e�F	���E���]���ċ�`�]��											 */
/* ���t�F	2005/01/27														 */
/* ========================================================================= */
void CImg32::BltTurnRightAndLeft(
	int dx,				/* [in] �n�_(X) */
	int dy,				/* [in] �n�_(Y) */
	int cx,				/* [in] �� */
	int cy,				/* [in] ���� */
	CImg32 *pSrc,		/* [in] �]���� */
	int sx,				/* [in] �]�����̎n�_(X) */
	int sy,				/* [in] �]�����̎n�_(Y) */
	BOOL bColorKey)		/* [in] �J���[�L�[���g�p */
{
	int i, j;
	PDWORD dst, src;
	DWORD dwKey, dwLineDst, dwLineSrc;
	PBYTE pBitsDst, pBitsSrc;

	if ((m_pBits == NULL) || (pSrc == NULL)) {
		return;
	}

	/* ���W���N���b�s���O */
	ClipPos (dx, dy, cx, cy, pSrc, sx, sy);

	pBitsDst	= m_pBits;
	pBitsSrc	= pSrc->GetBits ();
	dwLineDst	= BYTES_PER_LINE(Width ());
	dwLineSrc	= BYTES_PER_LINE(pSrc->Width ());
	dwKey		= pSrc->GetColorKey ();

	dx	*= BYTES_PER_PIXEL;
	dy	= Height () - 1 - dy;
	sx	*= BYTES_PER_PIXEL;
	sy	= pSrc->Height () - 1 - sy;

	/* �J���[�L�[���g�p����H */
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


/* ========================================================================= */
/* �֐����F	CImg32::BltTurnUpAndDown										 */
/* ���e�F	�㉺���]���ċ�`�]��											 */
/* ���t�F	2005/01/27														 */
/* ========================================================================= */
void CImg32::BltTurnUpAndDown(
	int dx,				/* [in] �n�_(X) */
	int dy,				/* [in] �n�_(Y) */
	int cx,				/* [in] �� */
	int cy,				/* [in] ���� */
	CImg32 *pSrc,		/* [in] �]���� */
	int sx,				/* [in] �]�����̎n�_(X) */
	int sy,				/* [in] �]�����̎n�_(Y) */
	BOOL bColorKey)		/* [in] �J���[�L�[���g�p */
{
	int i, j;
	PDWORD dst, src;
	DWORD dwKey, dwLineDst, dwLineSrc;
	PBYTE pBitsDst, pBitsSrc;

	if ((m_pBits == NULL) || (pSrc == NULL)) {
		return;
	}

	/* ���W���N���b�s���O */
	ClipPos (dx, dy, cx, cy, pSrc, sx, sy);

	pBitsDst	= m_pBits;
	pBitsSrc	= pSrc->GetBits ();
	dwLineDst	= BYTES_PER_LINE(Width ());
	dwLineSrc	= BYTES_PER_LINE(pSrc->Width ());
	dwKey		= pSrc->GetColorKey ();

	dx	*= BYTES_PER_PIXEL;
	dy	= Height () - 1 - dy;
	sx	*= BYTES_PER_PIXEL;
	sy	= pSrc->Height () - 1 - sy;

	/* �J���[�L�[���g�p����H */
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


/* ========================================================================= */
/* �֐����F	CImg32::Lock													 */
/* ���e�F	�摜�����b�N													 */
/* ���t�F	2005/01/26														 */
/* ========================================================================= */
HDC CImg32::Lock(void)
{
	HDC hDC, hRet;

	hRet = NULL;
	/* �I�u�W�F�N�g���s�� */
	if ((m_hBmp == NULL )|| (m_hBmp == HBITMAP_WITHOUT_GDI)) {
		goto Exit;
	}

	/* ���łɃ��b�N���Ă��� */
	if (m_hDC) {
		hRet = m_hDC;
		goto Exit;
	}

	/* �R���p�`�u�� DC ���쐬���āA�r�b�g�}�b�v��I�� */
	hDC = GetDC (NULL);
	m_hDC = CreateCompatibleDC (hDC);
	ReleaseDC (NULL, hDC);
	if (m_hDC) {
		m_hBmpBack = (HBITMAP)SelectObject (m_hDC, m_hBmp);
	}
	hRet = m_hDC;

Exit:
	return hRet;
}


/* ========================================================================= */
/* �֐����F	CImg32::Unlock													 */
/* ���e�F	���b�N����														 */
/* ���t�F	2005/01/26														 */
/* ========================================================================= */
void CImg32::Unlock(void)
{
	if (m_hDC == NULL) {
		return;
	}

	SelectObject (m_hDC, m_hBmpBack);
	DeleteDC (m_hDC);
	m_hDC = NULL;
}


/* ========================================================================= */
/* �֐����F	CImg32::GetSafeHandle											 */
/* ���e�F	�r�b�g�}�b�v�n���h�����擾										 */
/* ���t�F	2005/01/26														 */
/* ========================================================================= */
HBITMAP CImg32::GetSafeHandle(void)
{
	return m_hBmp;
}


/* ========================================================================= */
/* �֐����F	CImg32::Width													 */
/* ���e�F	�����擾														 */
/* ���t�F	2005/01/26														 */
/* ========================================================================= */
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


/* ========================================================================= */
/* �֐����F	CImg32::Height													 */
/* ���e�F	�������擾														 */
/* ���t�F	2005/01/26														 */
/* ========================================================================= */
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


/* ========================================================================= */
/* �֐����F	CImg32::GetImageSize											 */
/* ���e�F	�f�[�^�T�C�Y���擾												 */
/* ���t�F	2005/01/26														 */
/* ========================================================================= */
int CImg32::GetImageSize(void)
{
	int nRet, cx, cy;

	nRet = 0;
	if (m_hBmp == NULL) {
		goto Exit;
	}

	cx = Width ();
	cy = Height ();
	nRet = BYTES_PER_LINE(cx) * cy;

Exit:
	return nRet;
}


/* ========================================================================= */
/* �֐����F	CImg32::GetBits													 */
/* ���e�F	�r�b�g�f�[�^�̃A�h���X���擾									 */
/* ���t�F	2005/01/26														 */
/* ========================================================================= */
LPBYTE CImg32::GetBits(void)
{
	return m_pBits;
}


/* ========================================================================= */
/* �֐����F	CImg32::SetColorKey												 */
/* ���e�F	�J���[�L�[��ݒ�												 */
/* ���t�F	2005/01/26														 */
/* ========================================================================= */
void CImg32::SetColorKey(COLORREF clKey)
{
	m_dwColorKey = ColorrefToDword (clKey);
}


/* ========================================================================= */
/* �֐����F	CImg32::GetColorKey												 */
/* ���e�F	�J���[�L�[���擾												 */
/* ���t�F	2005/01/26														 */
/* ========================================================================= */
COLORREF CImg32::GetColorKey(void)
{
	COLORREF clRet;
	TARGB tTmp;

	tTmp.ARGB = m_dwColorKey;
	clRet = RGB (tTmp.R, tTmp.G, tTmp.B);

	return clRet;
}


/* ========================================================================= */
/* �֐����F	CImg32::IsMode256												 */
/* ���e�F	256�F�摜������													 */
/* ���t�F	2008/07/04														 */
/* ========================================================================= */
BOOL CImg32::IsMode256(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (m_pPallet) {
		/* �p���b�g����������256�F�摜�Ƃ��� */
		bRet = TRUE;
	}

	return bRet;
}


/* ========================================================================= */
/* �֐����F	CImg32::SetColorKeyNo											 */
/* ���e�F	�J���[�L�[��ݒ�												 */
/* ���t�F	2008/07/04														 */
/* ========================================================================= */
void CImg32::SetColorKeyNo(BYTE byColorKeyNo)
{
	m_byColorKey = byColorKeyNo;
}


/* ========================================================================= */
/* �֐����F	CImg32::GetColorKeyNo											 */
/* ���e�F	�J���[�L�[���擾												 */
/* ���t�F	2008/07/04														 */
/* ========================================================================= */
BYTE CImg32::GetColorKeyNo(void)
{
	return m_byColorKey;
}


/* ========================================================================= */
/* �֐����F	CImg32::SetPallet												 */
/* ���e�F	�p���b�g��ݒ�													 */
/* ���t�F	2008/07/04														 */
/* ========================================================================= */
void CImg32::SetPallet(BYTE byNo, BYTE r, BYTE g, BYTE b)
{
	PIMG32RGB pPallet;

	pPallet = (PIMG32RGB)&m_pPallet[byNo * 3];
	pPallet->r = r;
	pPallet->g = g;
	pPallet->b = b;
}


/* ========================================================================= */
/* �֐����F	CImg32::GetPallet												 */
/* ���e�F	�p���b�g���擾													 */
/* ���t�F	2008/07/04														 */
/* ========================================================================= */
PBYTE CImg32::GetPallet(BYTE byNo)
{
	return &m_pPallet[byNo * 3];
}


/* ========================================================================= */
/* �֐����F	CImg32::FillRect256												 */
/* ���e�F	��`�h��Ԃ�													 */
/* ���t�F	2008/07/04														 */
/* ========================================================================= */
void CImg32::FillRect256(int x, int y, int cx, int cy, BYTE byPallet)
{
	int i, j;
	DWORD dwLineDst;
	PBYTE pTmp;

	if (m_pBits == NULL) {
		return;
	}

	x = min (x, Width ());
	x = max (x, 0);
	y = min (y, Height ());
	y = max (y, 0);
	if (x + cx > Width ()) {
		cx = Width () - x;
	}
	if (y + cy > Height ()) {
		cy = Height () - y;
	}

	dwLineDst = Width ();
	y = Height () - 1 - y;

	for (j = 0; j < cy; j ++) {
		pTmp = (m_pBits + dwLineDst * (y - j) + x);
		for (i = 0; i < cx; i ++) {
			pTmp[i] = byPallet;
		}
	}
}


/* ========================================================================= */
/* �֐����F	CImg32::BltFrom256												 */
/* ���e�F	��`�]��														 */
/* ���t�F	2008/07/04														 */
/* ========================================================================= */
void CImg32::BltFrom256(
	int dx,				/* [in] �n�_(X) */
	int dy,				/* [in] �n�_(Y) */
	int cx,				/* [in] �� */
	int cy,				/* [in] ���� */
	CImg32 *pSrc,		/* [in] �]���� */
	int sx,				/* [in] �]�����̎n�_(X) */
	int sy,				/* [in] �]�����̎n�_(Y) */
	BOOL bColorKey)		/* [in] �J���[�L�[���g�p */
{
	int i, j;
	BYTE byKey;
	DWORD dwLineDst, dwLineSrc;
	PBYTE pBitsDst, pBitsSrc, dst, src;
	PIMG32RGB pPallet;

	if ((m_pBits == NULL) || (pSrc == NULL)) {
		return;
	}

	/* ���W���N���b�s���O */
	ClipPos (dx, dy, cx, cy, pSrc, sx, sy);

	pBitsDst	= m_pBits;
	pBitsSrc	= pSrc->GetBits ();
	dwLineDst	= BYTES_PER_LINE(Width ());
	dwLineSrc	= pSrc->Width ();
	byKey		= pSrc->GetColorKeyNo ();

	dx	*= BYTES_PER_PIXEL;
	dy	= Height () - 1 - dy;
	sy	= pSrc->Height () - 1 - sy;
	pPallet = (PIMG32RGB)pSrc->GetPallet (0);

	for (j = 0; j < cy; j ++) {
		dst = (pBitsDst + dwLineDst * (dy - j) + dx);
		src = (pBitsSrc + dwLineSrc * (sy - j) + sx);
		for (i = 0; i < cx; i ++) {
			if ((bColorKey == FALSE) || (byKey != src[i])) {
				CopyMemory (dst, &pPallet[src[i]], 3);
			}
			dst += 4;
		}
	}
}


/* ========================================================================= */
/* �֐����F	CImg32::BltAlphaFrom256											 */
/* ���e�F	�������킹��`�]��												 */
/* ���t�F	2008/07/04														 */
/* ========================================================================= */
void CImg32::BltAlphaFrom256(
	int dx,				/* [in] �n�_(X) */
	int dy,				/* [in] �n�_(Y) */
	int cx,				/* [in] �� */
	int cy,				/* [in] ���� */
	CImg32 *pSrc,		/* [in] �]���� */
	int sx,				/* [in] �]�����̎n�_(X) */
	int sy,				/* [in] �]�����̎n�_(Y) */
	BYTE byLevel,		/* [in] ���� */
	BOOL bColorKey)		/* [in] �J���[�L�[���g�p */
{
	int i, j;
	BYTE byKey;
	DWORD dwLineDst, dwLineSrc;
	PBYTE pBitsDst, pBitsSrc, dst, src;
	PIMG32RGB pPallet;

	if ((m_pBits == NULL) || (pSrc == NULL)) {
		return;
	}

	/* ���W���N���b�s���O */
	ClipPos (dx, dy, cx, cy, pSrc, sx, sy);

	pBitsDst	= m_pBits;
	pBitsSrc	= pSrc->GetBits ();
	dwLineDst	= BYTES_PER_LINE(Width ());
	dwLineSrc	= pSrc->Width ();
	byKey		= pSrc->GetColorKeyNo ();
	byLevel		= PercentToHex (byLevel);

	dx	*= BYTES_PER_PIXEL;
	dy	= Height () - 1 - dy;
	sy	= pSrc->Height () - 1 - sy;
	pPallet = (PIMG32RGB)pSrc->GetPallet (0);

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


/* ========================================================================= */
/* �֐����F	CImg32::InitData												 */
/* ���e�F	�f�[�^��������													 */
/* ���t�F	2005/01/24														 */
/* ========================================================================= */
void CImg32::InitData(void)
{
	m_hBmp			= NULL;
	m_hBmpBack		= NULL;
	m_pBits			= NULL;
	m_hDC			= NULL;
	m_dwColorKey	= 0;
	m_pPallet		= NULL;
	ZeroMemory (&m_bmpInfo, sizeof (m_bmpInfo));
}


/* ========================================================================= */
/* �֐����F	CImg32::ColorrefToDword											 */
/* ���e�F	�F�f��DWORD�l�ɕϊ�												 */
/* ���t�F	2005/01/24														 */
/* ========================================================================= */
DWORD CImg32::ColorrefToDword(COLORREF cl)
{
	DWORD dwRet;

	dwRet  = (cl & 0x000000FF) << 16;
	dwRet |= (cl & 0x0000FF00) << 0;
	dwRet |= (cl & 0x00FF0000) >> 16;

	return dwRet;
}


/* ========================================================================= */
/* �֐����F	CImg32::PercentToHex											 */
/* ���e�F	�p�[�Z���g��255MAX��16�i���ɕϊ�								 */
/* ���t�F	2005/01/24														 */
/* ========================================================================= */
BYTE CImg32::PercentToHex(BYTE byPercent)
{
	BYTE byRet;

	byRet = 255 - (byPercent * 255 / 100);

	return byRet;
}


/* ========================================================================= */
/* �֐����F	CImg32::ClipPos													 */
/* ���e�F	���W���N���b�s���O												 */
/* ���t�F	2005/01/28														 */
/* ========================================================================= */
void CImg32::ClipPos(
	int &dx,			/* [in,ou]	�n�_(X) */
	int &dy,			/* [in,ou]	�n�_(Y) */
	int &cx,			/* [in,ou]	�� */
	int &cy,			/* [in,ou]	���� */
	CImg32 *pSrc,		/* [in]		�]���� */
	int &sx,			/* [in,ou]	�]�����̎n�_(X) */
	int &sy)			/* [in,ou]	�]�����̎n�_(Y) */
{
	int dxBack, dyBack, cxBack, cyBack, sxBack, syBack;
	RECT rc1, rc2, rcTmp;

	dxBack = dx;
	dyBack = dy;
	cxBack = cx;
	cyBack = cy;
	sxBack = sx;
	syBack = sy;

	/* �]�������N���b�s���O */
	SetRect (&rc1, sx, sy, sx + cx, sy + cy);
	SetRect (&rc2, 0, 0, pSrc->Width (), pSrc->Height ());
	IntersectRect (&rcTmp, &rc1, &rc2);
	sx = rcTmp.left;
	sy = rcTmp.top;
	cx = rcTmp.right - rcTmp.left;
	cy = rcTmp.bottom - rcTmp.top;

	/* �]������N���b�s���O */
	SetRect (&rc1, dx, dy, dx + cx, dy + cy);
	SetRect (&rc2, 0, 0, Width (), Height ());
	IntersectRect (&rcTmp, &rc1, &rc2);
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

/* Copyright(C)URARA-works 2005 */
