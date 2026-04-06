/// @file WndMapPartsGrp.cpp
/// @brief マップパーツ一覧スタティックコントロールクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/05/15
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "LibInfoMapParts.h"
#include "InfoMapParts.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrDraw.h"
#include "MgrGrpData.h"
#include "WndMapPartsGrp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラス設定

BEGIN_MESSAGE_MAP(CWndMapPartsGrp, CWnd)
	//{{AFX_MSG_MAP(CWndMapPartsGrp)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CWndMapPartsGrp::CWndMapPartsGrp()
{
	m_nPos			= 0;
	m_pWndParent	= NULL;
	m_pMgrData		= NULL;
	m_pMgrGrpData	= NULL;
	m_nMode			= 0;
	m_nGrpNo		= 0;

	m_pImgParts		= new CImg32;

	m_ptCursor.x	= m_ptCursor.y	= 0;
//	m_ptMoveSrc.x	= m_ptMoveSrc.y	= 0;
//	m_ptMoveDst.x	= m_ptMoveDst.y	= 0;
}

CWndMapPartsGrp::~CWndMapPartsGrp()
{
	SAFE_DELETE(m_pImgParts);
}

BOOL CWndMapPartsGrp::Create(CWnd *pParent, CMgrData *pMgrData, int nResourceID, int nMode/*0*/)
{
	BOOL bRet;
	CWnd *pWnd;
	CRect rc;

	bRet = FALSE;
	m_nMode = nMode;

	pWnd = pParent->GetDlgItem(nResourceID);
	if (pWnd == NULL) {
		goto Exit;
	}
	pWnd->GetWindowRect(rc);
	pParent->ScreenToClient(rc);

	m_pWndParent	= pParent;
	m_pMgrData		= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData();

	bRet = CWnd::Create(
		AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW),
		_T(""),
		WS_CHILD | WS_VISIBLE,
		rc,
		pParent,
		0);
	if (bRet == FALSE) {
		goto Exit;
	}

	pWnd->ShowWindow(SW_HIDE);
	SetScrollPos(SB_HORZ, 0);
	SetScrollPos(SB_VERT, 0);

	bRet = TRUE;
Exit:
	return bRet;
}

void CWndMapPartsGrp::Destroy(void)
{
	if (m_hWnd) {
		DestroyWindow();
	}
}

void CWndMapPartsGrp::SetMode(
	int nMode)		// 0:マップ画像 1:影画像
{
	m_nMode = nMode;
	MakeImage(nMode);
	InvalidateRect(NULL);
}

void CWndMapPartsGrp::SetNo(int nGrpNo)
{
	m_nGrpNo = nGrpNo;
	MakeImage(m_nMode);
	InvalidateRect(NULL);
}

int CWndMapPartsGrp::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SCROLLINFO stScrollInfo;

	if (CWnd::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	GetScrollInfo(SB_HORZ, &stScrollInfo);
	stScrollInfo.nPage = 1;
	SetScrollInfo(SB_HORZ, &stScrollInfo);
	GetScrollInfo(SB_VERT, &stScrollInfo);
	stScrollInfo.nPage = 1;
	SetScrollInfo(SB_VERT, &stScrollInfo);

	MakeImage(m_nMode);

	return 0;
}

void CWndMapPartsGrp::OnPaint()
{
	int x, y, cx, cy;
	HDC hDC;
	CDC DCTmp;
	CRect rc, rcTmp;
	CRgn rgn;
	CPaintDC dc(this);

#ifdef _WIN32
	hDC = m_pImgParts->Lock();
	DCTmp.Attach(hDC);

	GetClientRect(rc);
	rgn.CreateRectRgn(0, 0, rc.Width(), rc.Height());
	dc.SelectClipRgn(&rgn, RGN_COPY);

	x = GetScrollPos(SB_HORZ);
	y = GetScrollPos(SB_VERT);

	cx = min(rc.Width(), m_pImgParts->Width());
	cy = min(rc.Height(), m_pImgParts->Height());
	dc.FillSolidRect(rc, RGB(128, 128, 128));
	dc.BitBlt(0, 0, cx, cy, &DCTmp, x * 16, y * 16, SRCCOPY);

	x = m_ptCursor.x * 16;
	y = (m_ptCursor.y - m_nPos) * 16;
	rcTmp.SetRect(x, y, x + 15, y + 15);

	dc.DrawEdge(rcTmp, EDGE_BUMP, BF_FLAT | BF_RECT);

	m_pImgParts->Unlock();
	DCTmp.Detach();
#endif // _WIN32
}

void CWndMapPartsGrp::OnSize(UINT nType, int cx, int cy)
{
	int xx, yy;

	CWnd::OnSize(nType, cx, cy);

	xx = m_pImgParts->Width();
	if (xx > cx) {
		SetScrollRange(SB_HORZ, 0, (xx - cx) / 16);
	}
	yy = m_pImgParts->Height();
	if (yy > cy) {
		SetScrollRange(SB_VERT, 0, (yy - cy) / 16);
	}
}

void CWndMapPartsGrp::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint ptBack;
	CRect rc;

	ptBack		 = m_ptCursor;
	m_ptCursor.x = point.x / 16;
	m_ptCursor.y = point.y / 16 + m_nPos;

	if (ptBack != m_ptCursor) {
		InvalidateRect(NULL, FALSE);
	}
}

void CWndMapPartsGrp::OnLButtonDown(UINT nFlags, CPoint point)
{
	int x, y, nNo;

	x = point.x / 16 + GetScrollPos(SB_HORZ);
	y = point.y / 16 + GetScrollPos(SB_VERT);
	nNo = (y * 32) + x;
	nNo += (m_nGrpNo * 1024);

	m_pWndParent->PostMessage(WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_LBUTTONDOWN, nNo);
}

void CWndMapPartsGrp::OnRButtonDown(UINT nFlags, CPoint point)
{
	int x, y, nNo;

	x = point.x / 16 + GetScrollPos(SB_HORZ);
	y = point.y / 16 + GetScrollPos(SB_VERT);
	nNo = (y * 32) + x;
	nNo += (m_nGrpNo * 1024);

	m_pWndParent->PostMessage(WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_RBUTTONDOWN, nNo);
}

BOOL CWndMapPartsGrp::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int nTmp;

	nTmp = -1;
	if (zDelta < 0) {
		nTmp = 1;
	}
	nTmp += GetScrollPos(SB_VERT);
	nTmp = max(nTmp, 0);
	SetScrollPos(SB_VERT, nTmp);
	InvalidateRect(NULL);

	return FALSE;
}

void CWndMapPartsGrp::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMin, nMax, nPosNow;

	GetScrollRange(SB_HORZ, &nMin, &nMax);
	nPosNow = GetScrollPos(SB_HORZ);

	switch (nSBCode) {
	case SB_LEFT:			// 左端へスクロール
		nPos = nMin;
		break;
	case SB_RIGHT:			// 右端へスクロール
		nPos = nMax;
		break;
	case SB_LINELEFT:		// 左へスクロール
	case SB_PAGELEFT:		// 1ページ左へスクロール
		if (nPosNow <= 0) {
			return;
		}
		nPos = nPosNow - 1;
		break;
	case SB_LINERIGHT:		// 右へスクロール
	case SB_PAGERIGHT:		// 1ページ右へスクロール
		if (nPosNow >= nMax) {
			return;
		}
		nPos = nPosNow + 1;
		break;
	case SB_THUMBPOSITION:	// 絶対位置へスクロール
	case SB_THUMBTRACK:		// ドラッグされた
		break;
	default:
		return;
	}

	SetScrollPos(SB_HORZ, nPos);
	InvalidateRect(NULL);
}

void CWndMapPartsGrp::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMin, nMax, nPosNow;

	GetScrollRange(SB_VERT, &nMin, &nMax);
	nPosNow = GetScrollPos(SB_VERT);

	switch (nSBCode) {
	case SB_TOP:			// 1番上までスクロール
		nPos = nMin;
		break;
	case SB_BOTTOM:			// 1番下までスクロール
		nPos = nMax;
		break;
	case SB_LINEUP:			// 1行上へスクロール
	case SB_PAGEUP:			// 1ページ上へスクロール
		if (nPosNow <= 0) {
			return;
		}
		nPos = nPosNow - 1;
		break;
	case SB_LINEDOWN:		// 1行下へスクロール
	case SB_PAGEDOWN:		// 1ページ下へスクロール
		if (nPosNow >= nMax) {
			return;
		}
		nPos = nPosNow + 1;
		break;
	case SB_THUMBPOSITION:	// 絶対位置へスクロール
	case SB_THUMBTRACK:		// ドラッグされた
		break;
	default:
		return;
	}

	SetScrollPos(SB_VERT, nPos);
	InvalidateRect(NULL);
}

void CWndMapPartsGrp::MakeImage(int nMode)
{
	CImg32 *pImg;

	if (m_pMgrData == NULL) {
		return;
	}

	if (nMode == 1) {
		pImg = m_pMgrGrpData->GetDibMapShadowTmp(m_nGrpNo);
	} else {
		pImg = m_pMgrGrpData->GetDibMapPartsTmp(m_nGrpNo);
	}
	if (pImg == NULL) {
		return;
	}

	m_pImgParts->Destroy();
	m_pImgParts->Create(pImg->Width(), pImg->Height());
	m_pImgParts->FillRect(0, 0, m_pImgParts->Width(), m_pImgParts->Height(), RGB(128, 128, 128));

	m_pImgParts->BltFrom256(0, 0, pImg->Width(), pImg->Height(), pImg, 0, 0);
}

void CWndMapPartsGrp::PostNcDestroy()
{
	delete this;
}
