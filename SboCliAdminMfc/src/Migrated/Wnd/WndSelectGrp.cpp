/// @file WndSelectGrp.cpp
/// @brief 画像選択ウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/10/30
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "WndSelectGrp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラス設定

BEGIN_MESSAGE_MAP(CWndSelectGrp, CWnd)
	//{{AFX_MSG_MAP(CWndSelectGrp)
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
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CWndSelectGrp::CWndSelectGrp()
{
	m_nSelect	= -1;
	m_nNotify	= 0;
	m_nGrpSizeX	= 0;
	m_nGrpCountX	= 0;
	m_pWndParent	= NULL;
	m_pMgrData	= NULL;
	m_pMgrGrpData	= NULL;

	m_nType	= 0;
	m_nGrpIDSub	= 0;
	m_pImgBack	= new CImg32;
	m_pImgBase	= new CImg32;
}

CWndSelectGrp::~CWndSelectGrp()
{
	SAFE_DELETE(m_pImgBack);
	SAFE_DELETE(m_pImgBase);
}

BOOL CWndSelectGrp::Create(CWnd *pParent, CMgrData *pMgrData, int nNotify)
{
	BOOL bRet;
	CRect rc, rcParent;

	m_pWndParent	= pParent;
	m_pMgrData	= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData();
	m_nNotify	= nNotify;

	pParent->GetWindowRect(rcParent);
	rc.SetRect(rcParent.left, rcParent.top, rcParent.left + 460, rcParent.top + 300);
	AdjustWindowRect(&rc, WS_EX_TOOLWINDOW, FALSE);

        bRet = CWnd::CreateEx(
                        WS_EX_TOOLWINDOW,
                        AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW),
                        _T("画像の選択"),
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

void CWndSelectGrp::Destroy(void)
{
	DestroyWindow();
}

void CWndSelectGrp::SetType(int nType, int nGrpIDSub)
{
	m_nType	= nType;
	m_nGrpIDSub	= nGrpIDSub;
	RenewGrp(m_nType);
}

int CWndSelectGrp::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int cx, cy, nCount;
	SCROLLINFO stScrollInfo;

	if (CWnd::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	// スクロールバーの設定
	ModifyStyle(0, WS_VSCROLL | WS_HSCROLL);
	nCount = m_pMgrGrpData->GetMapPartsCount();
	cx = 32;
	cy = nCount / 32;
	SetScrollRange(SB_HORZ, 0, cx);
	SetScrollRange(SB_VERT, 0, cy);

	GetScrollInfo(SB_HORZ, &stScrollInfo);
	stScrollInfo.nPage = 1;
	SetScrollInfo(SB_HORZ, &stScrollInfo);
	GetScrollInfo(SB_VERT, &stScrollInfo);
	stScrollInfo.nPage = 1;
	SetScrollInfo(SB_VERT, &stScrollInfo);

	return 0;
}

void CWndSelectGrp::OnClose()
{
	if (m_pWndParent) {
		m_pWndParent->PostMessage(
				WM_WNDCLOSE,
				WINDOWID_SELECTMAPPARTSGRP,
				(LPARAM)MAKELPARAM((WORD)m_nSelect, (WORD)m_nNotify));
	}
	CWnd::OnClose();
}

void CWndSelectGrp::OnPaint()
{
	int x, y, cx, cy;
	HDC hDC;
	CDC *pDCTmp;

	CPaintDC dc(this);

	x  = GetScrollPos(SB_HORZ);
	y  = GetScrollPos(SB_VERT);
	cx = m_pImgBack->Width();
	cy = m_pImgBack->Height();
	m_pImgBack->FillRect(0, 0, cx, cy, 0);
	m_pImgBack->Blt(0, 0, m_pImgBase->Width(), m_pImgBase->Height(), m_pImgBase, x * m_nGrpSizeX, y * m_nGrpSizeX);

	hDC = m_pImgBack->Lock();
	pDCTmp = dc.FromHandle(hDC);
	dc.BitBlt(0, 0, cx, cy, pDCTmp, 0, 0, SRCCOPY);

	m_pImgBack->Unlock();
}

void CWndSelectGrp::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// スクロールバーの最大値を更新
	RenewScrollRange(cx, cy);
}

void CWndSelectGrp::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMin, nMax;
	BOOL bChange;
	UINT nPosNow;

	nPosNow = GetScrollPos(SB_HORZ);
	bChange = TRUE;

	switch (nSBCode) {
	case SB_LEFT:	// 左端へスクロール
		GetScrollRange(SB_HORZ, &nMin, &nMax);
		nPos = nMin;
		break;
	case SB_RIGHT:	// 右端へスクロール
		GetScrollRange(SB_HORZ, &nMin, &nMax);
		nPos = nMax;
		break;
	case SB_LINELEFT:	// 左へスクロール
	case SB_PAGELEFT:	// 1ページ左へスクロール
		nPos = nPosNow - 1;
		break;
	case SB_LINERIGHT:	// 右へスクロール
	case SB_PAGERIGHT:	// 1ページ右へスクロール
		nPos = nPosNow + 1;
		break;
	case SB_THUMBPOSITION:	// 絶対位置へスクロール
	case SB_THUMBTRACK:	// ドラッグされた
		break;
	default:
		bChange = FALSE;
		break;
	}

	if (bChange) {
		SetScrollPos(SB_HORZ, nPos);
		InvalidateRect(NULL);
	}
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CWndSelectGrp::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMin, nMax;
	BOOL bChange;
	UINT nPosNow;

	nPosNow = GetScrollPos(SB_VERT);
	bChange = TRUE;

	switch (nSBCode) {
	case SB_TOP:	// 1番上までスクロール
		GetScrollRange(SB_VERT, &nMin, &nMax);
		nPos = nMin;
		break;
	case SB_BOTTOM:	// 1番下までスクロール
		GetScrollRange(SB_VERT, &nMin, &nMax);
		nPos = nMax;
		break;
	case SB_LINEUP:	// 1行上へスクロール
	case SB_PAGEUP:	// 1ページ上へスクロール
		nPos = nPosNow - 1;
		break;
	case SB_LINEDOWN:	// 1行下へスクロール
	case SB_PAGEDOWN:	// 1ページ下へスクロール
		nPos = nPosNow + 1;
		break;
	case SB_THUMBPOSITION:	// 絶対位置へスクロール
	case SB_THUMBTRACK:	// ドラッグされた
		break;
	default:
		bChange = FALSE;
		break;
	}

	if (bChange) {
		SetScrollPos(SB_VERT, nPos);
		InvalidateRect(NULL);
	}
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CWndSelectGrp::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	BOOL bRet;

	bRet = TRUE;
	if (nHitTest != HTCLIENT) {
		bRet = CWnd::OnSetCursor(pWnd, nHitTest, message);
		goto Exit;
	}

	SetCursor(LoadCursor(NULL, IDC_ARROW));
Exit:
	return bRet;
}

void CWndSelectGrp::OnLButtonDown(UINT nFlags, CPoint point)
{
	int no, x, y;

	if ((point.x >= m_pImgBase->Width()) || (point.y >= m_pImgBase->Height())) {
		goto Exit;
	}

	x = point.x / m_nGrpSizeX;
	y = point.y / m_nGrpSizeX;
	no = x + GetScrollPos(SB_HORZ) + ((y + GetScrollPos(SB_VERT)) * m_nGrpCountX);
	if (no < 0) {
		return;
	}

	m_nSelect = no + 1;
	PostMessage(WM_CLOSE, 0, 0);
Exit:
	CWnd::OnLButtonDown(nFlags, point);
}

void CWndSelectGrp::OnRButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_CLOSE, 0, 0);
	CWnd::OnRButtonDown(nFlags, point);
}

BOOL CWndSelectGrp::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int nTmp;

	nTmp = -1;
	if (zDelta < 0) {
		nTmp = 1;
	}
	SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + nTmp);
	InvalidateRect(NULL);

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CWndSelectGrp::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	int nMin, nMax;
	DWORD dwStyle;
	CRect rc;

	if (m_pImgBase->Width()) {
		rc.SetRect(0, 0, m_pImgBase->Width(), m_pImgBase->Height());
		dwStyle = GetStyle();
		AdjustWindowRect(rc, dwStyle, FALSE);

		GetScrollRange(SB_HORZ, &nMin, &nMax);
		if (nMax) {
			rc.bottom += GetSystemMetrics(SM_CYHSCROLL);
		}

		lpMMI->ptMaxTrackSize.x = rc.Width();
		lpMMI->ptMaxTrackSize.y = rc.Height();
	}

	CWnd::OnGetMinMaxInfo(lpMMI);
}

void CWndSelectGrp::PostNcDestroy()
{
	delete this;
}

void CWndSelectGrp::RenewScrollRange(int cx, int cy)
{
	int cxTmp, cyTmp;

	if (m_nGrpSizeX <= 0) {
		return;
	}

	m_pImgBack->Destroy();
	m_pImgBack->Create(cx, cy);

	// スクロールバーの最大値を更新
	cxTmp  = m_pImgBase->Width() / m_nGrpSizeX;
	cxTmp -= (cx / m_nGrpSizeX);
	cxTmp  = max(cxTmp, 0);
	cyTmp  = m_pImgBase->Height() / m_nGrpSizeX;
	cyTmp -= (cx / m_nGrpSizeX);
	cyTmp  = max(cyTmp, 0);
	SetScrollRange(SB_HORZ, 0, cxTmp);
	SetScrollRange(SB_VERT, 0, cyTmp);
}

void CWndSelectGrp::RenewGrp(int nType)
{
	int cx, cy, nCountX;
	DWORD dwParam;
	PCImg32 pImg;
	SIZE sizeWindow;

	nCountX = 0;
	dwParam = 0;
	pImg = NULL;

	m_pImgBase->Destroy();

	m_nGrpSizeX	= m_pMgrGrpData->GetGrpSize(nType);
	m_nGrpCountX	= m_pMgrGrpData->GetGrpCountX(nType);

	switch (nType) {
	case GRPIDMAIN_CHAR:	// キャラ
	case GRPIDMAIN_2X2_CHAR:	// キャラ(32x32)
		dwParam = FAMILYTYPE_HUMAN;
		break;
	default:
		dwParam = m_nGrpIDSub;
		break;
	}

	pImg = m_pMgrGrpData->GetDib(nType, 0, dwParam);
	if (pImg) {
		cx = pImg->Width();
		cy = pImg->Height();
		sizeWindow.cx = cx + GetSystemMetrics(SM_CXVSCROLL);
		sizeWindow.cy = cy + GetSystemMetrics(SM_CYHSCROLL) + GetSystemMetrics(SM_CYSMCAPTION);

		m_pImgBase->Create(cx, cy);
		m_pImgBase->BltFrom256(0, 0, cx, cy, pImg, 0, 0);
		SetWindowPos(NULL, 0, 0, sizeWindow.cx, sizeWindow.cy, SWP_NOZORDER | SWP_NOMOVE);
		RenewScrollRange(cx, cy);
		InvalidateRect(NULL);
	}
}

