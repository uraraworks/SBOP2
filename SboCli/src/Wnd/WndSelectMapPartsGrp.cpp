/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名	:WndSelectMapPartsGrp.cpp									 */
/* 内容			:マップパーツ画像選択ウィンドウクラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2005/10/07													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "WndSelectMapPartsGrp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* ========================================================================= */
/* クラス設定																 */
/* ========================================================================= */

BEGIN_MESSAGE_MAP(CWndSelectMapPartsGrp, CWnd)
	//{{AFX_MSG_MAP(CWndSelectMapPartsGrp)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CWndSelectMapPartsGrp::CWndSelectMapPartsGrp					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2005/10/07														 */
/* ========================================================================= */

CWndSelectMapPartsGrp::CWndSelectMapPartsGrp()
{
	m_nSelect		= -1;
	m_pWndParent	= NULL;
	m_pMgrData		= NULL;
	m_pMgrGrpData	= NULL;

	m_pImgBack		= new CImg32;
}


/* ========================================================================= */
/* 関数名	:CWndSelectMapPartsGrp::~CWndSelectMapPartsGrp					 */
/* 内容		:デストラクタ													 */
/* 日付		:2005/10/07														 */
/* ========================================================================= */

CWndSelectMapPartsGrp::~CWndSelectMapPartsGrp()
{
	SAFE_DELETE (m_pImgBack);
}


/* ========================================================================= */
/* 関数名	:CWndSelectMapPartsGrp::Create									 */
/* 内容		:作成															 */
/* 日付		:2005/10/07														 */
/* ========================================================================= */

BOOL CWndSelectMapPartsGrp::Create(CWnd *pParent, CMgrData *pMgrData)
{
	BOOL bRet;
	CRect rc, rcParent;

	m_pWndParent	= pParent;
	m_pMgrData		= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData ();

	pParent->GetWindowRect (rcParent);
	rc.SetRect (rcParent.left, rcParent.top, rcParent.left + 460, rcParent.top + 300);
	AdjustWindowRect (&rc, WS_EX_TOOLWINDOW, FALSE);

        bRet = CWnd::CreateEx (
                        WS_EX_TOOLWINDOW,
                        AfxRegisterWndClass (CS_HREDRAW | CS_VREDRAW),
                        _T("マップパーツ画像の選択"),
			WS_POPUPWINDOW | WS_CAPTION | WS_THICKFRAME | WS_HSCROLL | WS_VSCROLL,
			rc,
			pParent,
			0);
	if (bRet == FALSE) {
		goto Exit;
	}

Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWndSelectMapPartsGrp::Destroy									 */
/* 内容		:破棄															 */
/* 日付		:2005/10/07														 */
/* ========================================================================= */

void CWndSelectMapPartsGrp::Destroy(void)
{
	DestroyWindow ();
}


/* ========================================================================= */
/* 関数名	:CWndSelectMapPartsGrp::OnCreate								 */
/* 内容		:メッセージハンドラ(WM_CREATE)									 */
/* 日付		:2005/10/07														 */
/* ========================================================================= */

int CWndSelectMapPartsGrp::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int cx, cy, nCount;
	SCROLLINFO stScrollInfo;

	if (CWnd::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	/* スクロールバーの設定 */
	ModifyStyle (0, WS_VSCROLL | WS_HSCROLL);
	nCount = m_pMgrGrpData->GetMapPartsCount ();
	cx = 32;
	cy = nCount / 32;
	SetScrollRange (SB_HORZ, 0, cx);
	SetScrollRange (SB_VERT, 0, cy);

	GetScrollInfo (SB_HORZ, &stScrollInfo);
	stScrollInfo.nPage = 1;
	SetScrollInfo (SB_HORZ, &stScrollInfo);
	GetScrollInfo (SB_VERT, &stScrollInfo);
	stScrollInfo.nPage = 1;
	SetScrollInfo (SB_VERT, &stScrollInfo);

	return 0;
}


/* ========================================================================= */
/* 関数名	:CWndSelectMapPartsGrp::OnClose									 */
/* 内容		:メッセージハンドラ(WM_CLOSE)									 */
/* 日付		:2005/08/22														 */
/* ========================================================================= */

void CWndSelectMapPartsGrp::OnClose()
{
	if (m_pWndParent) {
		m_pWndParent->PostMessage (WM_WNDCLOSE, WINDOWID_SELECTMAPPARTSGRP, (LPARAM)m_nSelect);
	}
	CWnd::OnClose ();
}


/* ========================================================================= */
/* 関数名	:CWndSelectMapPartsGrp::OnPaint									 */
/* 内容		:メッセージハンドラ(WM_PAINT)									 */
/* 日付		:2005/10/07														 */
/* ========================================================================= */

void CWndSelectMapPartsGrp::OnPaint()
{
	int x, y, cx, cy, no, nCount;
	HDC hDC;
	CDC *pDCTmp;
	PCImg32 pImg;

	CPaintDC dc(this);

	nCount = m_pMgrGrpData->GetMapPartsCount ();
	cx = 32;
	cy = m_pImgBack->Height () / 16 + 1;
	for (y = 0; y < cy; y ++) {
		for (x = 0; x < cx; x ++) {
			no = x + GetScrollPos (SB_HORZ) + ((y + GetScrollPos (SB_VERT)) * cx);
			pImg = m_pMgrGrpData->GetDibMapPartsTmp (no / 1024);
			if (pImg == NULL) {
				continue;
			}

			m_pImgBack->BltFrom256 (x * 16, y * 16, 16, 16, pImg, ((no % 1024) % cx) * 16, (no % 1024) / 32 * 16);
		}
	}

	hDC = m_pImgBack->Lock ();
	pDCTmp = dc.FromHandle (hDC);

	dc.BitBlt (0, 0, m_pImgBack->Width (), m_pImgBack->Height (), pDCTmp, 0, 0, SRCCOPY);

	m_pImgBack->Unlock ();
}


/* ========================================================================= */
/* 関数名	:CWndSelectMapPartsGrp::OnSize									 */
/* 内容		:メッセージハンドラ(WM_SIZE)									 */
/* 日付		:2005/10/07														 */
/* ========================================================================= */

void CWndSelectMapPartsGrp::OnSize(UINT nType, int cx, int cy)
{
	int cxTmp, cyTmp, nCount;

	CWnd::OnSize(nType, cx, cy);

	m_pImgBack->Destroy ();
	m_pImgBack->Create (cx, cy);

	nCount = m_pMgrGrpData->GetMapPartsCount ();

	/* スクロールバーの最大値を更新 */
	cxTmp = 32 - (cx / 32) - 1;
	cyTmp = (nCount / 32) - (cy / 32) - 1;
	cxTmp = min (32 - 1, cxTmp);
	cyTmp = min (nCount / 32 - 1, cyTmp);
	SetScrollRange (SB_HORZ, 0, cxTmp);
	SetScrollRange (SB_VERT, 0, cyTmp);
}


/* ========================================================================= */
/* 関数名	:CWndSelectMapPartsGrp::OnHScroll								 */
/* 内容		:メッセージハンドラ(WM_HSCROLL)									 */
/* 日付		:2005/10/07														 */
/* ========================================================================= */

void CWndSelectMapPartsGrp::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMin, nMax;
	BOOL bChange;
	UINT nPosNow;

	nPosNow = GetScrollPos (SB_HORZ);
	bChange = TRUE;

	switch (nSBCode) {
	case SB_LEFT:			/* 左端へスクロール */
		GetScrollRange (SB_HORZ, &nMin, &nMax);
		nPos = nMin;
		break;
	case SB_RIGHT:			/* 右端へスクロール */
		GetScrollRange (SB_HORZ, &nMin, &nMax);
		nPos = nMax;
		break;
	case SB_LINELEFT:		/* 左へスクロール */
	case SB_PAGELEFT:		/* 1ページ左へスクロール */
		nPos = nPosNow - 1;
		break;
	case SB_LINERIGHT:		/* 右へスクロール */
	case SB_PAGERIGHT:		/* 1ページ右へスクロール */
		nPos = nPosNow + 1;
		break;
	case SB_THUMBPOSITION:	/* 絶対位置へスクロール */
	case SB_THUMBTRACK:		/* ドラッグされた */
		break;
	default:
		bChange = FALSE;
		break;
	}

	if (bChange) {
		SetScrollPos (SB_HORZ, nPos);
		InvalidateRect (NULL);
	}
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}


/* ========================================================================= */
/* 関数名	:CWndSelectMapPartsGrp::OnVScroll								 */
/* 内容		:メッセージハンドラ(WM_VSCROLL)									 */
/* 日付		:2005/10/07														 */
/* ========================================================================= */

void CWndSelectMapPartsGrp::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMin, nMax;
	BOOL bChange;
	UINT nPosNow;

	nPosNow = GetScrollPos (SB_VERT);
	bChange = TRUE;

	switch (nSBCode) {
	case SB_TOP:			/* 1番上までスクロール */
		GetScrollRange (SB_VERT, &nMin, &nMax);
		nPos = nMin;
		break;
	case SB_BOTTOM:			/* 1番下までスクロール */
		GetScrollRange (SB_VERT, &nMin, &nMax);
		nPos = nMax;
		break;
	case SB_LINEUP:			/* 1行上へスクロール */
	case SB_PAGEUP:			/* 1ページ上へスクロール */
		nPos = nPosNow - 1;
		break;
	case SB_LINEDOWN:		/* 1行下へスクロール */
	case SB_PAGEDOWN:		/* 1ページ下へスクロール */
		nPos = nPosNow + 1;
		break;
	case SB_THUMBPOSITION:	/* 絶対位置へスクロール */
	case SB_THUMBTRACK:		/* ドラッグされた */
		break;
	default:
		bChange = FALSE;
		break;
	}

	if (bChange) {
		SetScrollPos (SB_VERT, nPos);
		InvalidateRect (NULL);
	}
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}


/* ========================================================================= */
/* 関数名	:CWndSelectMapPartsGrp::OnSetCursor								 */
/* 内容		:メッセージハンドラ(WM_SETCURSOR)								 */
/* 日付		:2005/12/07														 */
/* ========================================================================= */

BOOL CWndSelectMapPartsGrp::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	BOOL bRet;

	bRet = TRUE;
	if (nHitTest != HTCLIENT) {
		bRet = CWnd::OnSetCursor (pWnd, nHitTest, message);
		goto Exit;
	}

	SetCursor (LoadCursor (NULL, IDC_ARROW));
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CWndSelectMapPartsGrp::OnLButtonDown							 */
/* 内容		:メッセージハンドラ(WM_LBUTTONDOWN)								 */
/* 日付		:2005/10/07														 */
/* ========================================================================= */

void CWndSelectMapPartsGrp::OnLButtonDown(UINT nFlags, CPoint point)
{
	int no, x, y, cx;
	PCImg32 pImg;

	pImg = m_pMgrGrpData->GetDibMapParts (0);

	if ((point.x >= pImg->Width ()) || (point.y >= pImg->Height ())) {
		goto Exit;
	}

	cx = pImg->Width () / 32;
	x = point.x / 32;
	y = point.y / 32;
	no = x + GetScrollPos (SB_HORZ) + ((y + GetScrollPos (SB_VERT)) * cx);
	if (no < 0) {
		return;
	}

	m_nSelect = no;
	PostMessage (WM_CLOSE, 0, 0);
Exit:
	CWnd::OnLButtonDown (nFlags, point);
}


/* ========================================================================= */
/* 関数名	:CWndSelectMapPartsGrp::OnRButtonDown							 */
/* 内容		:メッセージハンドラ(WM_RBUTTONDOWN)								 */
/* 日付		:2005/11/07														 */
/* ========================================================================= */

void CWndSelectMapPartsGrp::OnRButtonDown(UINT nFlags, CPoint point)
{
	PostMessage (WM_CLOSE, 0, 0);
	CWnd::OnRButtonDown (nFlags, point);
}


/* ========================================================================= */
/* 関数名	:CWndSelectMapPartsGrp::OnMouseWheel							 */
/* 内容		:メッセージハンドラ(WM_MOUSEWHEEL)								 */
/* 日付		:2005/11/30														 */
/* ========================================================================= */

BOOL CWndSelectMapPartsGrp::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int nTmp;

	nTmp = -1;
	if (zDelta < 0) {
		nTmp = 1;
	}
	SetScrollPos (SB_VERT, GetScrollPos (SB_VERT) + nTmp);
	InvalidateRect (NULL);

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}


/* ========================================================================= */
/* 関数名	:CWndSelectMapPartsGrp::PostNcDestroy							 */
/* 内容		:終了処理														 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

void CWndSelectMapPartsGrp::PostNcDestroy()
{
	delete this;
}

/* Copyright(C)URARA-works 2005 */
