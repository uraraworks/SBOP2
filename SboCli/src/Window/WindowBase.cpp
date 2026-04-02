/// @file WindowBase.cpp
/// @brief ウィンドウ基底クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/03
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "Img32.h"
#include "MgrWindow.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "WindowBase.h"
#if defined(__EMSCRIPTEN__)
#include <emscripten/em_js.h>
#endif

#if defined(__EMSCRIPTEN__)
EM_JS(void, SBOP2_QueueCanvasText, (int x, int y, const char *pszText, int r, int g, int b, int size, int outline, int frameR, int frameG, int frameB, int bold), {
	Module.sbop2TextQueue = Module.sbop2TextQueue || [];
	Module.sbop2TextQueue.push({
		x: x,
		y: y,
		text: UTF8ToString(pszText),
		r: r,
		g: g,
		b: b,
		size: size,
		outline: !!outline,
		frameR: frameR,
		frameG: frameG,
		frameB: frameB,
		bold: !!bold
	});
});
EM_JS(void, SBOP2_QueueCanvasRect, (int x, int y, int w, int h, int fillR, int fillG, int fillB, int fillA, int hasFill, int strokeR, int strokeG, int strokeB, int strokeA, int hasStroke, int lineWidth), {
	Module.sbop2RectQueue = Module.sbop2RectQueue || [];
	Module.sbop2RectQueue.push({
		x: x,
		y: y,
		w: w,
		h: h,
		fillR: fillR,
		fillG: fillG,
		fillB: fillB,
		fillA: fillA,
		hasFill: !!hasFill,
		strokeR: strokeR,
		strokeG: strokeG,
		strokeB: strokeB,
		strokeA: strokeA,
		hasStroke: !!hasStroke,
		lineWidth: lineWidth
	});
});
#endif

CWindowBase::CWindowBase()
{
	m_nID	= WINDOWTYPE_NONE;
	m_nPos	= 0;
	m_nPosMax	= 0;
	m_nCursorAnime	= 0;
	m_nCountKeyRepeate	= 0;
	m_dwTimeDrawStart	= 0;
	m_dwLastTimeKey	= 0;
	m_pParent	= NULL;
	m_pDib	= new CImg32;
	m_pMgrData	= NULL;
	m_pMgrDraw	= NULL;
	m_pMgrSound	= NULL;
	m_pMgrGrpData	= NULL;
	m_pDibSystem	= NULL;
	m_hWndMain	= NULL;
	m_dwLastTimerProc	= 0;
	m_dwLastTimeCursor	= 0;
	m_bShow	= TRUE;
	m_bDelete	= FALSE;
	m_bInput	= FALSE;
	m_bActive	= FALSE;

	m_ptViewPos.x	= m_ptViewPos.y	= 0;
	m_sizeWindow.cx	= m_sizeWindow.cy	= 0;

        m_hFont = CreateFont(16, 0, 0, 0, FW_ULTRABOLD,
                        FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
                        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("ＭＳ Ｐゴシック"));
        m_hFont12 = CreateFont(12, 0, 0, 0, FW_NORMAL,
                        FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
                        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("ＭＳ Ｐゴシック"));
        m_hFont12Bold = CreateFont(12, 0, 0, 0, FW_ULTRABOLD,
                        FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
                        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("ＭＳ Ｐゴシック"));
        m_hFont14 = CreateFont(14, 0, 0, 0, FW_ULTRABOLD,
                        FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
                        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("ＭＳ Ｐゴシック"));
        m_hFont16 = CreateFont(16, 0, 0, 0, FW_ULTRABOLD,
                        FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
                        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("ＭＳ ゴシック"));
        m_hFont16Normal = CreateFont(16, 0, 0, 0, FW_NORMAL,
                        FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
                        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("ＭＳ ゴシック"));

	m_pDib->SetColorKey(RGB(255, 0, 255));
}


CWindowBase::~CWindowBase()
{
	int i, nCount;
	PCWindowBase pChild;

	nCount = m_apChild.size();
	for (i = 0; i < nCount; i ++) {
		pChild = m_apChild[i];
		pChild->m_bDelete = TRUE;
	}
	if (m_pParent) {
		m_pParent->DeleteChild(this);
	}

	if (m_hFont) {
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}
	if (m_hFont12) {
		DeleteObject(m_hFont12);
		m_hFont12 = NULL;
	}
	if (m_hFont12Bold) {
		DeleteObject(m_hFont12Bold);
		m_hFont12Bold = NULL;
	}
	if (m_hFont14) {
		DeleteObject(m_hFont14);
		m_hFont14 = NULL;
	}
	if (m_hFont16) {
		DeleteObject(m_hFont16);
		m_hFont16 = NULL;
	}
	if (m_hFont16Normal) {
		DeleteObject(m_hFont16Normal);
		m_hFont16Normal = NULL;
	}
	SAFE_DELETE(m_pDib);
}


void CWindowBase::Create(CMgrData *pMgrData)
{
	m_pMgrData	= pMgrData;
	m_pMgrDraw	= m_pMgrData->	GetMgrDraw();
	m_pMgrSound	= m_pMgrData->	GetMgrSound();
	m_pMgrGrpData	= m_pMgrData->	GetMgrGrpData();
	m_hWndMain	= m_pMgrData->	GetMainWindow();
	m_pDibSystem	= m_pMgrGrpData->	GetDibSystem();
}


void CWindowBase::OnWindowMsg(int nType, DWORD dwPara)
{
}


void CWindowBase::PostWindowMessage(DWORD dwPara)
{
	if (m_pMgrData == NULL) {
		return;
	}

	m_pMgrData->PostWindowMessage(m_nID, dwPara);
}


void CWindowBase::Update(void)
{
	m_dwTimeDrawStart = 0;
}


void CWindowBase::Draw(PCImg32 pDst)
{
	if (m_dwTimeDrawStart) {
		goto Exit;
	}

	m_dwTimeDrawStart = timeGetTime();

Exit:
	return;
}


BOOL CWindowBase::TimerProc(void)
{
	BOOL bRet;

	bRet = FALSE;

	if (timeGetTime() - m_dwLastTimeCursor > 300) {
		m_dwLastTimeCursor = timeGetTime();
		m_nCursorAnime ++;
		if (m_nCursorAnime >= 2) {
			m_nCursorAnime = 0;
		}
	}

	if (m_dwTimeDrawStart == 0) {
		bRet = TRUE;
	}

	return bRet;
}


void CWindowBase::KeyProc(
	BYTE byCode,	// [in] イベント
	BOOL bDown)
{
	BOOL bResult;
	DWORD dwTime, dwTimeTmp;

	if (m_bActive == FALSE) {
		return;
	}

	dwTime = 250;
	if (m_nCountKeyRepeate > 1) {
		dwTime = 125;
	}
	dwTimeTmp = timeGetTime() - m_dwLastTimeKey;

	bResult = FALSE;
	if (byCode == 'B') {
		bResult = OnB(bDown);

	} else if (byCode == 'F') {
		bResult = OnF(bDown);

	} else if (byCode == 'I') {
		bResult = OnI(bDown);

	} else if (byCode == 'J') {
		bResult = OnJ(bDown);

	} else if (byCode == 'K') {
		bResult = OnK(bDown);

	} else if (byCode == 'L') {
		bResult = OnL(bDown);

	} else if (byCode == 'S') {
		bResult = OnS(bDown);

	} else if (byCode == 'X') {
		bResult = OnX(bDown);

	} else if (byCode == 'Z') {
		bResult = OnZ(bDown);

	} else if (byCode == VK_ESCAPE) {
		bResult = OnEscape(bDown);

	} else if (byCode == VK_SPACE) {
		bResult = OnSpace(bDown);

	} else if ((bDown) && (dwTimeTmp > dwTime)) {
		bResult = TRUE;
		if (byCode == VK_UP) {
			bResult = OnUp();

		} else if (byCode == VK_DOWN) {
			bResult = OnDown();

		} else if (byCode == VK_RIGHT) {
			bResult = OnRight();

		} else if (byCode == VK_LEFT) {
			bResult = OnLeft();

		} else {
			bResult = FALSE;
		}

		if (bResult) {
			m_nCursorAnime = 0;
			m_nCountKeyRepeate ++;
		}

	} else if (bDown == FALSE) {
		m_dwLastTimeKey	= 0;
		m_nCountKeyRepeate	= 0;
	}

	if (bResult) {
		m_dwTimeDrawStart	= 0;
		m_dwLastTimeKey	= timeGetTime();
	}
}


ILoginWindow *CWindowBase::GetLoginWindowInterface(void)
{
	return NULL;
}


BOOL CWindowBase::HandleSDLKeyDown(UINT vk)
{
	UNREFERENCED_PARAMETER(vk);
	return FALSE;
}


void CWindowBase::HandleSDLTextInput(LPCSTR pszText)
{
	UNREFERENCED_PARAMETER(pszText);
}


BOOL CWindowBase::HandleSDLMouseLeftButtonDown(int x, int y)
{
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	return FALSE;
}


void CWindowBase::SetShow(BOOL bShow)
{
	m_bShow	= bShow;
	m_dwLastTimerProc	= 0;
}


void CWindowBase::Redraw(void)
{
	m_dwTimeDrawStart = 0;
}


void CWindowBase::SetPos(int nPos)
{
	m_nPos = nPos;
}


void CWindowBase::AddChild(CWindowBase *pChild)
{
	m_apChild.push_back(pChild);
}


void CWindowBase::DeleteChild(CWindowBase *pChild)
{
	int i, nCount;
	PCWindowBase pChildTmp;

	nCount = m_apChild.size();
	for (i = 0; i < nCount; i ++) {
		pChildTmp = m_apChild[i];
		m_apChild.erase(m_apChild.begin() + i);
		break;
	}
}


BOOL CWindowBase::OnUp(void)
{
	return FALSE;
}


BOOL CWindowBase::OnDown(void)
{
	return FALSE;
}


BOOL CWindowBase::OnLeft(void)
{
	return FALSE;
}


BOOL CWindowBase::OnRight(void)
{
	return FALSE;
}


BOOL CWindowBase::OnB(BOOL bDown)
{
	return FALSE;
}


BOOL CWindowBase::OnF(BOOL bDown)
{
	return FALSE;
}


BOOL CWindowBase::OnI(BOOL bDown)
{
	return FALSE;
}


BOOL CWindowBase::OnJ(BOOL bDown)
{
	return FALSE;
}


BOOL CWindowBase::OnK(BOOL bDown)
{
	return FALSE;
}


BOOL CWindowBase::OnL(BOOL bDown)
{
	return FALSE;
}


BOOL CWindowBase::OnS(BOOL bDown)
{
	return FALSE;
}


BOOL CWindowBase::OnX(BOOL bDown)
{
	return FALSE;
}


BOOL CWindowBase::OnZ(BOOL bDown)
{
	return FALSE;
}


BOOL CWindowBase::OnEscape(BOOL bDown)
{
	return FALSE;
}


BOOL CWindowBase::OnSpace(BOOL bDown)
{
	return FALSE;
}


void CWindowBase::TextOut2(HDC hDC, int x, int y, LPCTSTR pStr, COLORREF Color, BOOL bDraw, COLORREF ColorFrame)
{
	if (pStr == NULL) {
		return;
	}
#ifdef UNICODE
	int nLen = wcslen((LPCWSTR)pStr);
#else
	int nLen = strlen((LPCSTR)pStr);
#endif
	if (nLen <= 0) {
		return;
	}
	// 縁取りする？
	if (bDraw) {
		SetTextColor(hDC, ColorFrame);
#ifdef UNICODE
		::TextOutW(hDC, x - 1, y, (LPCWSTR)pStr, nLen);
		::TextOutW(hDC, x + 1, y, (LPCWSTR)pStr, nLen);
		::TextOutW(hDC, x, y - 1, (LPCWSTR)pStr, nLen);
		::TextOutW(hDC, x, y + 1, (LPCWSTR)pStr, nLen);
#else
		::TextOutA(hDC, x - 1, y, (LPCSTR)pStr, nLen);
		::TextOutA(hDC, x + 1, y, (LPCSTR)pStr, nLen);
		::TextOutA(hDC, x, y - 1, (LPCSTR)pStr, nLen);
		::TextOutA(hDC, x, y + 1, (LPCSTR)pStr, nLen);
#endif
	}
	SetTextColor(hDC, Color);
#ifdef UNICODE
	::TextOutW(hDC, x, y, (LPCWSTR)pStr, nLen);
#else
	::TextOutA(hDC, x, y, (LPCSTR)pStr, nLen);
#endif
}


void CWindowBase::TextOut3(HDC hDC, int x, int y, int cx, int cy, LPCTSTR pStr, COLORREF Color)
{
	if (pStr == NULL) {
		return;
	}

	int nLen = lstrlen(pStr);
	if (nLen <= 0) {
		return;
	}

	RECT rc;

	SetRect(&rc, x, y, cx, cy);
	SetTextColor(hDC, RGB(10, 10, 10));

	SetRect(&rc, x - 1, y, x + cx, y + cy);	DrawText(hDC, pStr, nLen, &rc, DT_RIGHT | DT_NOCLIP);
	SetRect(&rc, x + 1, y, x + cx, y + cy);	DrawText(hDC, pStr, nLen, &rc, DT_RIGHT | DT_NOCLIP);
	SetRect(&rc, x, y - 1, x + cx, y + cy);	DrawText(hDC, pStr, nLen, &rc, DT_RIGHT | DT_NOCLIP);
	SetRect(&rc, x, y + 1, x + cx, y + cy);	DrawText(hDC, pStr, nLen, &rc, DT_RIGHT | DT_NOCLIP);

	SetTextColor(hDC, Color);
	SetRect(&rc, x, y, x + cx, y + cy);	DrawText(hDC, pStr, nLen, &rc, DT_RIGHT | DT_NOCLIP);
}



void CWindowBase::TextOut4(HDC hDC, int x, int y, LPCTSTR pStr, COLORREF ColorFrame, COLORREF Color)
{
	if (pStr == NULL) {
		return;
	}

	int nLen = lstrlen(pStr);
	if (nLen <= 0) {
		return;
	}
	SetTextColor(hDC, ColorFrame);

	::TextOut(hDC, x - 2, y, pStr, nLen);
	::TextOut(hDC, x - 1, y, pStr, nLen);
	::TextOut(hDC, x - 1, y - 2, pStr, nLen);
	::TextOut(hDC, x - 2, y - 1, pStr, nLen);
	::TextOut(hDC, x - 1, y - 1, pStr, nLen);
	::TextOut(hDC, x - 2, y + 1, pStr, nLen);
	::TextOut(hDC, x - 1, y + 1, pStr, nLen);
	::TextOut(hDC, x - 1, y + 2, pStr, nLen);

	::TextOut(hDC, x + 2, y, pStr, nLen);
	::TextOut(hDC, x + 1, y, pStr, nLen);
	::TextOut(hDC, x + 1, y - 2, pStr, nLen);
	::TextOut(hDC, x + 2, y - 1, pStr, nLen);
	::TextOut(hDC, x + 1, y - 1, pStr, nLen);
	::TextOut(hDC, x + 2, y + 1, pStr, nLen);
	::TextOut(hDC, x + 1, y + 1, pStr, nLen);
	::TextOut(hDC, x + 1, y + 2, pStr, nLen);

	::TextOut(hDC, x, y - 2, pStr, nLen);
	::TextOut(hDC, x, y - 1, pStr, nLen);
	::TextOut(hDC, x, y + 2, pStr, nLen);
	::TextOut(hDC, x, y + 1, pStr, nLen);

	SetTextColor(hDC, Color);
	::TextOut(hDC, x, y, pStr, nLen);
}


void CWindowBase::DrawBrowserText(int x, int y, LPCTSTR pStr, COLORREF Color, int nFontSize, BOOL bDraw, COLORREF ColorFrame, BOOL bBold)
{
#if defined(__EMSCRIPTEN__)
	if (pStr == NULL) {
		return;
	}

	const int nLen = lstrlen(pStr);
	if (nLen <= 0) {
		return;
	}

	int nUtf8Len = WideCharToMultiByte(CP_UTF8, 0, pStr, nLen, NULL, 0, NULL, NULL);
	if (nUtf8Len <= 0) {
		return;
	}

	std::vector<char> aUtf8((size_t)nUtf8Len + 1, 0);
	WideCharToMultiByte(CP_UTF8, 0, pStr, nLen, &aUtf8[0], nUtf8Len, NULL, NULL);
	aUtf8[(size_t)nUtf8Len] = '\0';

	const int nDrawX = m_ptViewPos.x + 32 + x;
	const int nDrawY = m_ptViewPos.y + 32 + y - 2;
	const int nColorR = (int)(Color & 0xFF);
	const int nColorG = (int)((Color >> 8) & 0xFF);
	const int nColorB = (int)((Color >> 16) & 0xFF);
	const int nFrameR = (int)(ColorFrame & 0xFF);
	const int nFrameG = (int)((ColorFrame >> 8) & 0xFF);
	const int nFrameB = (int)((ColorFrame >> 16) & 0xFF);

	SBOP2_QueueCanvasText(
		nDrawX,
		nDrawY,
		&aUtf8[0],
		nColorR,
		nColorG,
		nColorB,
		nFontSize,
		(int)bDraw,
		nFrameR,
		nFrameG,
		nFrameB,
		(int)bBold);
#else
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	UNREFERENCED_PARAMETER(pStr);
	UNREFERENCED_PARAMETER(Color);
	UNREFERENCED_PARAMETER(nFontSize);
	UNREFERENCED_PARAMETER(bDraw);
	UNREFERENCED_PARAMETER(ColorFrame);
	UNREFERENCED_PARAMETER(bBold);
#endif
}


void CWindowBase::DrawBrowserRect(int x, int y, int cx, int cy, COLORREF ColorFill, BOOL bFill, COLORREF ColorStroke, BOOL bStroke, int nLineWidth, BYTE byFillAlpha, BYTE byStrokeAlpha)
{
#if defined(__EMSCRIPTEN__)
	const int nDrawX = m_ptViewPos.x + 32 + x;
	const int nDrawY = m_ptViewPos.y + 32 + y;
	const int nFillR = (int)(ColorFill & 0xFF);
	const int nFillG = (int)((ColorFill >> 8) & 0xFF);
	const int nFillB = (int)((ColorFill >> 16) & 0xFF);
	const int nStrokeR = (int)(ColorStroke & 0xFF);
	const int nStrokeG = (int)((ColorStroke >> 8) & 0xFF);
	const int nStrokeB = (int)((ColorStroke >> 16) & 0xFF);

	SBOP2_QueueCanvasRect(
		nDrawX,
		nDrawY,
		cx,
		cy,
		nFillR,
		nFillG,
		nFillB,
		(int)byFillAlpha,
		(int)bFill,
		nStrokeR,
		nStrokeG,
		nStrokeB,
		(int)byStrokeAlpha,
		(int)bStroke,
		nLineWidth);
#else
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	UNREFERENCED_PARAMETER(cx);
	UNREFERENCED_PARAMETER(cy);
	UNREFERENCED_PARAMETER(ColorFill);
	UNREFERENCED_PARAMETER(bFill);
	UNREFERENCED_PARAMETER(ColorStroke);
	UNREFERENCED_PARAMETER(bStroke);
	UNREFERENCED_PARAMETER(nLineWidth);
	UNREFERENCED_PARAMETER(byFillAlpha);
	UNREFERENCED_PARAMETER(byStrokeAlpha);
#endif
}



void CWindowBase::DrawFrame(int nType)
{
	DrawFrame(0, 0, m_sizeWindow.cx, m_sizeWindow.cy, nType);
}


void CWindowBase::DrawFrame(int x, int y, int cx, int cy, int nType, BOOL bRightErace)
{
	typedef struct _FRAMEINFO {
		POINT	pt;	// 左上の開始位置
		COLORREF	cl;	// 背景色
		int	nSize;	// 枠のサイズ
		int	nPos;	// 表示位置補正
	} FRAMEINFO, *PFRAMEINFO;
	int i, nTmp, xx, yy, nSize, nPos;
	COLORREF clTmp;
	FRAMEINFO astFrameinfo[] = {
		 0,	  0, RGB(255, 235, 200),	16, 32,
		 0,	 48, RGB(255, 235, 200),	16, 32,
		48,	 48, RGB(255, 235, 200),	16, 32,
		96,	 48, RGB(255, 235, 200),	16, 32,
		 0,	 96, RGB(255, 211, 76),	16, 32,
		 0,	816, RGB(255, 235, 200),	 8, 40,
		48,	816, RGB(255, 235, 200),	 8, 40,
		96,	816, RGB(196, 140, 81),	 8, 40
	};

	xx	= astFrameinfo[nType].pt.x;
	yy	= astFrameinfo[nType].pt.y;
	clTmp	= astFrameinfo[nType].cl;
	nTmp	= (nType == 4) ? 3 : 0;
	nSize	= astFrameinfo[nType].nSize;
	nPos	= astFrameinfo[nType].nPos;

	// 背景塗りつぶし
	m_pDib->FillRect(x + 0, y + 8, cx -  0, cy - 16, clTmp);
	m_pDib->FillRect(x + 8, y + 0, cx - 16, cy -  0, clTmp);

	// 横線
	for (i = 0; i < cx / 8 - 2; i ++) {
		m_pDib->BltFrom256(x + (i + 1) * 8, y,	8, 16, m_pDibSystem, xx + 16, yy, TRUE);
		m_pDib->BltFrom256(x + (i + 1) * 8, y + cy - 8,	8, 8,  m_pDibSystem, xx + 16, yy + 40, TRUE);
	}

	// 縦線
	for (i = 0; i < cy / 8 - 2; i ++) {
		if ((i == 1) && (nTmp != 0)) {
			m_pDib->BltFrom256(x,	y + (i + 1) * 8 + nTmp, 8, 8 - nTmp, m_pDibSystem, xx,	yy + 16 + nTmp, TRUE);
			m_pDib->BltFrom256(x + cx - 8,	y + (i + 1) * 8 + nTmp, 8, 8 - nTmp, m_pDibSystem, xx + 40,	yy + 16 + nTmp, TRUE);
		} else {
			m_pDib->BltFrom256(x,	y + (i + 1) * 8, 8, 8, m_pDibSystem, xx,	yy + 16 + nTmp, TRUE);
			m_pDib->BltFrom256(x + cx - 8,	y + (i + 1) * 8, 8, 8, m_pDibSystem, xx + 40,	yy + 16 + nTmp, TRUE);
		}
	}

	// 四隅
	m_pDib->BltFrom256(x, y,	16, 16 + nTmp,	m_pDibSystem, xx, yy, TRUE);
	m_pDib->BltFrom256(x, y + cy - 16,	16, 16,	m_pDibSystem, xx, yy + 32, TRUE);
	if (bRightErace == FALSE) {
		m_pDib->BltFrom256(x + cx - nSize, y, nSize, nSize + nTmp, m_pDibSystem, xx + nPos, yy, TRUE);
		m_pDib->BltFrom256(x + cx - nSize, y + cy - nSize, nSize, nSize, m_pDibSystem, xx + nPos, yy + nPos, TRUE);
	}
}


void CWindowBase::DrawFrame2(int x, int y, int cx, int cy, int nType, CImg32 *pDst, int nBottom)
{
	int i, xx, yy, aTypeX[] = {0, 24, 0}, aTypeY[] = {144, 144, 168};
	COLORREF clTmp, clType[] = {RGB(255, 255, 255), RGB(255, 255, 255), RGB(255, 235, 200)};

	xx	= aTypeX[nType];
	yy	= aTypeY[nType];
	clTmp	= clType[nType];

	if (pDst == NULL) {
		pDst = m_pDib;
	}
	if (cx % 8) {
		cx += (8 - (cx % 8));
	}
	if (cy % 8) {
		cy += (8 - (cy % 8));
	}

	// 背景塗りつぶし
	pDst->FillRect(x, y, cx, cy - nBottom, clTmp);

	// 四隅
	pDst->BltFrom256(x - 8,	y - 8,	8, 8, m_pDibSystem, xx,	 yy, TRUE);
	pDst->BltFrom256(x + cx,	y - 8,	8, 8, m_pDibSystem, xx + 16, yy, TRUE);
	pDst->BltFrom256(x - 8,	y + cy - nBottom, 8, 8, m_pDibSystem, xx,	yy + 16, TRUE);
	pDst->BltFrom256(x + cx,	y + cy - nBottom, 8, 8, m_pDibSystem, xx + 16,  yy + 16, TRUE);

	// 横線
	for (i = 0; i < cx / 8; i ++) {
		pDst->BltFrom256(x + i * 8, y - 8, 8, 8, m_pDibSystem, xx + 8, yy, TRUE);
		pDst->BltFrom256(x + i * 8, y + cy - nBottom, 8, 8, m_pDibSystem, xx + 8, yy + 16,	TRUE);
	}

	// 縦線
	for (i = 0; i < cy / 8; i ++) {
		pDst->BltFrom256(x - 8, y + i * 8, 8, 8, m_pDibSystem, xx, yy + 8, TRUE);
		pDst->BltFrom256(x + cx, y + i * 8 - nBottom, 8, 8, m_pDibSystem, xx + 16,	yy + 8, TRUE);
	}
}


void CWindowBase::DrawFrame3(int x, int y, int cx, int cy, int nType)
{
	int i, xx, yy, aTypeX[] = {96}, aTypeY[] = {48};
	COLORREF clTmp, clType[] = {RGB(139, 213, 255)};

	xx	= aTypeX[nType];
	yy	= aTypeY[nType];
	clTmp	= clType[nType];

	// 背景塗りつぶし
	m_pDib->FillRect(x + 16, y + 32, cx - 32, cy - 32, clTmp);

	// 四隅
	m_pDib->BltFrom256(x,	y,	16, 32, m_pDibSystem, xx,	yy,	TRUE);
	m_pDib->BltFrom256(x + cx - 16,	y,	16, 32, m_pDibSystem, xx + 32,	yy,	TRUE);
	m_pDib->BltFrom256(x,	y + cy - 16,	16, 16, m_pDibSystem, xx,	yy + 32,	TRUE);
	m_pDib->BltFrom256(x + cx - 16,	y + cy - 16,	16, 16, m_pDibSystem, xx + 32,	yy + 32,	TRUE);

	// 横線
	for (i = 0; i < cx / 16 - 2; i ++) {
		m_pDib->BltFrom256(x + 16 + i * 16, y,	16, 32, m_pDibSystem, xx + 16, yy,	TRUE);
		m_pDib->BltFrom256(x + 16 + i * 16, y + cy - 16,	16, 16, m_pDibSystem, xx + 16, yy + 32,	TRUE);
	}

	// 縦線
	for (i = 0; i < cy / 16 - 2; i ++) {
		m_pDib->BltFrom256(x,	y + 32 + i * 16, 16, 16, m_pDibSystem, xx,	yy + 24, TRUE);
		m_pDib->BltFrom256(x + cx - 16,	y + 32 + i * 16, 16, 16, m_pDibSystem, xx + 32,	yy + 24, TRUE);
	}
}


void CWindowBase::DrawCursor(int x, int y)
{
//	if (m_bActive && (m_nCursorAnime == 0)) {
//	return;
//	}

	m_pDib->BltFrom256(x, y, 24, 24, m_pDibSystem, 48, 0, TRUE);
}


void CWindowBase::DrawInputFrame1(int x, int y, int cx, int cy, int nType)
{
	int i, yy, nCount, nyyType[] = {128, 152};

	yy	= nyyType[nType];
	nCount	= cx;

	m_pDib->BltFrom256(x - 6, y - 6, 16, 24, m_pDibSystem, 48, yy, TRUE);
	for (i = 0; i < nCount; i ++) {
		m_pDib->BltFrom256(x + i, y - 6, 1, 24, m_pDibSystem, 63, yy, TRUE);
	}
	m_pDib->BltFrom256(x + i, y - 6, 5, 24, m_pDibSystem, 107, yy, TRUE);
}


void CWindowBase::DrawIconFrame(int x, int y)
{
	m_pDib->BltFrom256(x, y, 34, 45, m_pDibSystem, 0, 883, TRUE);
}
