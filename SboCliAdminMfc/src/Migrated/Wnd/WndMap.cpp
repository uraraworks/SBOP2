/// @file WndMap.cpp
/// @brief マップウィンドウクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/09/12
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "InfoMapBase.h"
#include "PacketBase.h"
#include "PacketADMIN_MAP_SETPARTS.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrDraw.h"
#include "AdminApi/AdminUiApi.h"
#include "WndMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_VIEW_TOOLBAR1	100
enum {
	TOOLBAR_COPY = 1,	// コピー
	TOOLBAR_PASTE,	// 貼り付け
	TOOLBAR_PAINT,	// 塗りつぶし
	TOOLBAR_RANGE,	// 範囲選択
	TOOLBAR_GRID,	// グリッド表示
};

// ステータスバー設定用情報
static UINT indicators[] =
{
	ID_SEPARATOR,           // ステータス ライン インジケータ
};

// クラス設定

BEGIN_MESSAGE_MAP(CWndMap, CWnd)
	//{{AFX_MSG_MAP(CWndMap)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(TOOLBAR_COPY, OnUpdateCommandUI)
	ON_UPDATE_COMMAND_UI(TOOLBAR_PASTE, OnUpdateCommandUI)
	ON_UPDATE_COMMAND_UI(TOOLBAR_PAINT, OnUpdateCommandUI)
	ON_UPDATE_COMMAND_UI(TOOLBAR_RANGE, OnUpdateCommandUI)
	ON_UPDATE_COMMAND_UI(TOOLBAR_GRID, OnUpdateCommandUI)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(TOOLBAR_COPY, OnCopy)
	ON_COMMAND(TOOLBAR_PASTE, OnPaste)
	ON_COMMAND(TOOLBAR_PAINT, OnToolPaint)
	ON_COMMAND(TOOLBAR_RANGE, OnRange)
	ON_COMMAND(TOOLBAR_GRID, OnGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CWndMap::CWndMap()
{
	m_bRClickFirst	= FALSE;
	m_bViewGrid	= FALSE;
	m_nSelect	= -1;
	m_nNotify	= 0;
	m_pdwParts	= NULL;
	m_pWndParent	= NULL;
	m_pMgrData	= NULL;
	m_pMgrGrpData	= NULL;
	m_pInfoMap	= NULL;
	m_pMgrDraw	= NULL;
	m_pHost	= NULL;

	m_pImgBack	= new CImg32;
	m_pImgBase	= new CImg32;

	m_rcRange.left = -1;
	m_rcCopy. left = -1;
	ZeroMemory(&m_ptBack, sizeof (m_ptBack));
}

CWndMap::~CWndMap()
{
	SAFE_DELETE(m_pdwParts);
	SAFE_DELETE(m_pMgrDraw);
	SAFE_DELETE(m_pImgBack);
	SAFE_DELETE(m_pImgBase);
}

BOOL CWndMap::Create(CWnd *pParent, CMgrData *pMgrData, int nNotify, const SboAdminUiHost* pHost)
{
	BOOL bRet;
	CRect rc, rcParent;

	m_pWndParent	= pParent;
	m_pMgrData	= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData();
	m_nNotify	= nNotify;
	m_pHost	= pHost;
	if ((m_pHost) && (m_pHost->GetMapData)) {
		m_pInfoMap = (CInfoMapBase*)m_pHost->GetMapData(m_pHost->userData);
	} else {
		m_pInfoMap = m_pMgrData->GetMap();
	}

	m_pMgrDraw = new CMgrDraw;
	m_pMgrDraw->Create(m_pMgrData);

	pParent->GetWindowRect(rcParent);
	rc.SetRect(rcParent.left, rcParent.top, rcParent.left + 460, rcParent.top + 300);
	AdjustWindowRect(&rc, WS_EX_TOOLWINDOW, FALSE);

        bRet = CWnd::CreateEx(
                        WS_EX_TOOLWINDOW,
                        AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS),
                        _T("マップウィンドウ"),
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

void CWndMap::Destroy(void)
{
	DestroyWindow();
}

int CWndMap::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int cx, cy, nCount;
	SIZE sizeTmp;
	SCROLLINFO stScrollInfo;

	if (CWnd::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_LIST | TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP, CRect(0,0,0,0), ID_VIEW_TOOLBAR1)) {
		return -1;
	}
	m_ImgListToolBar.Create(IDB_TOOLBAR, 16, 5, RGB(192, 192, 192));
	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_ImgListToolBar);

	RenewToolbar();

	// スクロールバーの設定
	ModifyStyle(0, WS_VSCROLL | WS_HSCROLL);
	nCount = m_pMgrGrpData->GetMapPartsCount();
	cx = m_pInfoMap->m_sizeMap.cx;
	cy = m_pInfoMap->m_sizeMap.cy;
	SetScrollRange(SB_HORZ, 0, cx);
	SetScrollRange(SB_VERT, 0, cy);

	GetScrollInfo(SB_HORZ, &stScrollInfo);
	stScrollInfo.nPage = 1;
	SetScrollInfo(SB_HORZ, &stScrollInfo);
	GetScrollInfo(SB_VERT, &stScrollInfo);
	stScrollInfo.nPage = 1;
	SetScrollInfo(SB_VERT, &stScrollInfo);

	if ((m_pHost) && (m_pHost->GetWndMapSize)) {
		int nCx, nCy;
		m_pHost->GetWndMapSize(m_pHost->userData, &nCx, &nCy);
		sizeTmp.cx = nCx;
		sizeTmp.cy = nCy;
	} else {
		sizeTmp = m_pMgrData->GetWndMap();
	}
	SetWindowPos(NULL, 0, 0, sizeTmp.cx, sizeTmp.cy, SWP_NOZORDER | SWP_NOMOVE);

	// ステータスバーの設定
	if (!m_StatusBar.Create(this) ||
		!m_StatusBar.SetIndicators(indicators, sizeof (indicators) / sizeof (UINT))) {
		return -1;
	}
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	return 0;
}

void CWndMap::OnUpdateCommandUI(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CWndMap::OnClose()
{
	CRect rcTmp;
	SIZE sizeTmp;

	GetWindowRect(rcTmp);
	sizeTmp.cx = rcTmp.Width();
	sizeTmp.cy = rcTmp.Height();
	if ((m_pHost) && (m_pHost->SetWndMapSize)) {
		m_pHost->SetWndMapSize(m_pHost->userData, sizeTmp.cx, sizeTmp.cy);
	} else {
		m_pMgrData->SetWndMap(sizeTmp);
	}

	if (m_pWndParent) {
		m_pWndParent->PostMessage(
				WM_WNDCLOSE,
				WINDOWID_SELECTMAPPARTSGRP,
				(LPARAM)MAKELPARAM((WORD)m_nSelect, (WORD)m_nNotify));
	}
	CWnd::OnClose();
}

void CWndMap::OnPaint()
{
	BOOL bResult;
	int x, y, xx, yy, cx, cy, xxx, yyy;
	const int nTileSize = MAPPARTSSIZE;
	HDC hDC;
	DWORD dwPartsID;
	POINT ptTmp;
	CRect rcTmp, rcRangeTmp;
	CSize size;
	CDC *pDCTmp;

	CPaintDC dc(this);
	size = m_wndToolBar.CalcFixedLayout(FALSE, TRUE);

	x  = GetScrollPos(SB_HORZ);
	y  = GetScrollPos(SB_VERT);
	cx = m_pImgBack->Width();
	cy = m_pImgBack->Height();
	m_pImgBack->FillRect(0, 0, cx, cy, 0);

	GetCursorPos(&ptTmp);
	ptTmp.y -= size.cy;
	ScreenToClient(&ptTmp);
	xxx = ptTmp.x / nTileSize;
	yyy = ptTmp.y / nTileSize;

	for (yy = 0; yy < cy / nTileSize + 1; yy ++) {
		for (xx = 0; xx < cx / nTileSize + 1; xx ++) {
			dwPartsID = m_pInfoMap->GetParts(xx + x, yy + y);
			m_pMgrDraw->DrawMapParts(m_pImgBack, xx * nTileSize, yy * nTileSize, dwPartsID, 1, TRUE);
			if (m_bViewGrid) {
				m_pImgBack->XorRect(xx * nTileSize + (nTileSize - 1), yy * nTileSize, 1, nTileSize);
				m_pImgBack->XorRect(xx * nTileSize, yy * nTileSize + (nTileSize - 1), nTileSize, 1);
			}
		}
	}

	rcRangeTmp = m_rcCopy;
	if (rcRangeTmp.left > rcRangeTmp.right) {
		rcRangeTmp.left  = m_rcCopy.right;
		rcRangeTmp.right = m_rcCopy.left;
	}
	if (rcRangeTmp.top > rcRangeTmp.bottom) {
		rcRangeTmp.top	= m_rcCopy.bottom;
		rcRangeTmp.bottom	= m_rcCopy.top;
	}

	// 張り付け中？
	if (GetCheck(TOOLBAR_PASTE)) {
		// 貼り付けプレビューを表示する
		for (yy = 0; yy < rcRangeTmp.Height(); yy ++) {
			for (xx = 0; xx < rcRangeTmp.Width(); xx ++) {
				dwPartsID = m_pdwParts[xx + yy * rcRangeTmp.Width()];
				m_pMgrDraw->DrawMapParts(m_pImgBack, (xx + xxx) * nTileSize, (yy + yyy) * nTileSize, dwPartsID, 1, TRUE, FALSE, 40);
			}
		}
	}

	rcRangeTmp = m_rcRange;
	if (rcRangeTmp.left > rcRangeTmp.right) {
		rcRangeTmp.left  = m_rcRange.right;
		rcRangeTmp.right = m_rcRange.left;
	}
	if (rcRangeTmp.top > rcRangeTmp.bottom) {
		rcRangeTmp.top	= m_rcRange.bottom;
		rcRangeTmp.bottom	= m_rcRange.top;
	}

	xx = ptTmp.x / nTileSize * nTileSize;
	yy = ptTmp.y / nTileSize * nTileSize;

	hDC = m_pImgBack->Lock();
	pDCTmp = dc.FromHandle(hDC);

	bResult = GetCheck(TOOLBAR_RANGE);
	if (bResult && (rcRangeTmp.left != -1)) {
		rcTmp.SetRect(rcRangeTmp.left - x, rcRangeTmp.top - y, rcRangeTmp.right - x, rcRangeTmp.bottom - y);
		rcTmp.left	*= nTileSize;
		rcTmp.top	*= nTileSize;
		rcTmp.right	*= nTileSize;
		rcTmp.bottom	*= nTileSize;
		rcTmp.right	+= nTileSize - 1;
		rcTmp.bottom	+= nTileSize - 1;
	} else {
		rcTmp.SetRect(xx, yy, xx + nTileSize - 1, yy + nTileSize - 1);
	}
	pDCTmp->DrawEdge(rcTmp, EDGE_BUMP, BF_MONO | BF_RECT);

	dc.BitBlt(0, size.cy, cx, cy, pDCTmp, 0, 0, SRCCOPY);
//	m_pImgBack->Unlock ();

	RenewStatusText();
}

void CWndMap::OnSize(UINT nType, int cx, int cy)
{
	CSize size;

	CWnd::OnSize(nType, cx, cy);

	size = m_wndToolBar.CalcFixedLayout(FALSE, TRUE);
	m_wndToolBar.MoveWindow(0, 0, cx, size.cy);

	// スクロールバーの最大値を更新
	RenewScrollRange(cx, cy - size.cy);

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
}

void CWndMap::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMin, nMax;
	BOOL bChange;
	UINT nPosNow;
	SCROLLINFO stScrollInfo;

	nPosNow = GetScrollPos(SB_HORZ);
	bChange = TRUE;

	GetScrollInfo(SB_HORZ, &stScrollInfo);

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
		nPos = nPosNow - 1;
		break;
	case SB_PAGELEFT:	// 1ページ左へスクロール
		nPos = nPosNow - stScrollInfo.nPage;
		break;
	case SB_LINERIGHT:	// 右へスクロール
		nPos = nPosNow + 1;
		break;
	case SB_PAGERIGHT:	// 1ページ右へスクロール
		nPos = nPosNow + stScrollInfo.nPage;
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

void CWndMap::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMin, nMax;
	BOOL bChange;
	UINT nPosNow;
	SCROLLINFO stScrollInfo;

	nPosNow = GetScrollPos(SB_VERT);
	bChange = TRUE;

	GetScrollInfo(SB_VERT, &stScrollInfo);

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
		nPos = nPosNow - 1;
		break;
	case SB_PAGEUP:	// 1ページ上へスクロール
		nPos = nPosNow - stScrollInfo.nPage;
		break;
	case SB_LINEDOWN:	// 1行下へスクロール
		nPos = nPosNow + 1;
		break;
	case SB_PAGEDOWN:	// 1ページ下へスクロール
		nPos = nPosNow + stScrollInfo.nPage;
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

BOOL CWndMap::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
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

void CWndMap::OnLButtonDown(UINT nFlags, CPoint point)
{
	DWORD dwPartsID;
	int x, y, xx, yy;
	const int nTileSize = MAPPARTSSIZE;
	CSize size;
	CPacketADMIN_MAP_SETPARTS Packet;

	if (m_pInfoMap == NULL) {
		return;
	}
	size = m_wndToolBar.CalcFixedLayout(FALSE, TRUE);
	point.y -= size.cy;

	x  = GetScrollPos(SB_HORZ);
	y  = GetScrollPos(SB_VERT);
	x += (point.x / nTileSize);
	y += (point.y / nTileSize);

	if ((x >= m_pInfoMap->m_sizeMap.cx) || (y >= m_pInfoMap->m_sizeMap.cy)) {
		return;
	}
	m_ptBack.x = x;
	m_ptBack.y = y;

	// 範囲選択中？
	if (GetCheck(TOOLBAR_RANGE)) {
		if (nFlags != -1) {
			m_rcRange.SetRect(x, y, x, y);
		} else {
			m_rcRange.right  = x;
			m_rcRange.bottom = y;
		}

	// 貼り付け中？
	} else if (GetCheck(TOOLBAR_PASTE)) {
		if (m_rcCopy.left != -1) {
			for (yy = 0; yy < m_rcCopy.Height(); yy ++) {
				for (xx = 0; xx < m_rcCopy.Width(); xx ++) {
					dwPartsID = m_pdwParts[xx + yy * m_rcCopy.Width()];
					m_pInfoMap->SetParts(x + xx, y + yy, dwPartsID);
					Packet.Make(m_pInfoMap->m_dwMapID, x + xx, y + yy, dwPartsID, FALSE);
					SendPacket(&Packet);
				}
			}
		}

	} else {
		DWORD dwSelPartsID = ((m_pHost) && (m_pHost->GetSelectMapPartsID)) ? m_pHost->GetSelectMapPartsID(m_pHost->userData) : m_pMgrData->GetSelectMapPartsID();
		m_pInfoMap->SetParts(x, y, dwSelPartsID);
		Packet.Make(m_pInfoMap->m_dwMapID, x, y, dwSelPartsID, FALSE);
		SendPacket(&Packet);
	}

	InvalidateRect(NULL);
}

void CWndMap::OnLButtonUp(UINT nFlags, CPoint point)
{
	int x, y;
	const int nTileSize = MAPPARTSSIZE;
	CSize size;

	if (m_pInfoMap == NULL) {
		return;
	}
	size = m_wndToolBar.CalcFixedLayout(FALSE, TRUE);
	point.y -= size.cy;

	x  = GetScrollPos(SB_HORZ);
	y  = GetScrollPos(SB_VERT);
	x += (point.x / nTileSize);
	y += (point.y / nTileSize);

	if ((x >= m_pInfoMap->m_sizeMap.cx) || (y >= m_pInfoMap->m_sizeMap.cy)) {
		return;
	}

	// 範囲選択中？
	if (GetCheck(TOOLBAR_RANGE)) {
		m_wndToolBar.GetToolBarCtrl().EnableButton(TOOLBAR_COPY,  TRUE);
		m_wndToolBar.GetToolBarCtrl().EnableButton(TOOLBAR_PASTE, TRUE);
	}
}

void CWndMap::OnRButtonDown(UINT nFlags, CPoint point)
{
	int x, y;
	DWORD dwPartsID;
	const int nTileSize = MAPPARTSSIZE;
	CSize size;

	if (m_pInfoMap == NULL) {
		return;
	}
	size = m_wndToolBar.CalcFixedLayout(FALSE, TRUE);
	point.y -= size.cy;

	x  = GetScrollPos(SB_HORZ);
	y  = GetScrollPos(SB_VERT);
	x += (point.x / nTileSize);
	y += (point.y / nTileSize);

	if ((x >= m_pInfoMap->m_sizeMap.cx) || (y >= m_pInfoMap->m_sizeMap.cy)) {
		return;
	}
	m_ptBack.x = x;
	m_ptBack.y = y;

	// 範囲選択中？
	if (GetCheck(TOOLBAR_RANGE)) {
		if (m_bRClickFirst == FALSE) {
			m_bRClickFirst = TRUE;
			// パーツを拾う
			dwPartsID = m_pInfoMap->GetParts(x, y);
			if ((m_pHost) && (m_pHost->SetSelectMapPartsID)) {
				m_pHost->SetSelectMapPartsID(m_pHost->userData, dwPartsID);
			} else {
				m_pMgrData->SetSelectMapPartsID(dwPartsID);
			}
		} else {
			m_rcRange.SetRect(x, y, x, y);
		}

	// 貼り付け中？
	} else if (GetCheck(TOOLBAR_PASTE)) {
		m_rcRange.left = -1;
		m_rcCopy.left = -1;
		SetCheck(TOOLBAR_PASTE, FALSE);

	} else {
		// パーツを拾う
		dwPartsID = m_pInfoMap->GetParts(x, y);
		if ((m_pHost) && (m_pHost->SetSelectMapPartsID)) {
			m_pHost->SetSelectMapPartsID(m_pHost->userData, dwPartsID);
		} else {
			m_pMgrData->SetSelectMapPartsID(dwPartsID);
		}
	}
}

void CWndMap::OnRButtonUp(UINT nFlags, CPoint point)
{
	int x, y;
	const int nTileSize = MAPPARTSSIZE;
	CSize size;
	CRect rcTmp;

	if (m_pInfoMap == NULL) {
		return;
	}
	size = m_wndToolBar.CalcFixedLayout(FALSE, TRUE);
	point.y -= size.cy;

	x  = GetScrollPos(SB_HORZ);
	y  = GetScrollPos(SB_VERT);
	x += (point.x / nTileSize);
	y += (point.y / nTileSize);

	if ((x >= m_pInfoMap->m_sizeMap.cx) || (y >= m_pInfoMap->m_sizeMap.cy)) {
		return;
	}

	// 範囲選択中？
	if (GetCheck(TOOLBAR_RANGE)) {
		if (m_bRClickFirst == FALSE) {
			rcTmp = m_rcRange;
			if (rcTmp.left > rcTmp.right) {
				rcTmp.left  = m_rcRange.right;
				rcTmp.right = m_rcRange.left;
			}
			if (rcTmp.top > rcTmp.bottom) {
				rcTmp.top	= m_rcRange.bottom;
				rcTmp.bottom	= m_rcRange.top;
			}
			rcTmp.right ++;
			rcTmp.bottom ++;
			if (rcTmp.IsRectEmpty() == FALSE) {
				OnCopy();
				SetCheck(TOOLBAR_PASTE, TRUE);
				OnPaste();
			}
		}
		m_bRClickFirst = FALSE;
	}

	InvalidateRect(NULL);
}

void CWndMap::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_pInfoMap == NULL) {
		return;
	}

	// 範囲選択中？
	if (GetCheck(TOOLBAR_RANGE)) {
		m_rcRange.left = -1;
		m_rcCopy.left = -1;
		SetCheck(TOOLBAR_RANGE, FALSE);
	}

	InvalidateRect(NULL);
}

BOOL CWndMap::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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

void CWndMap::OnMouseMove(UINT nFlags, CPoint point)
{
	int x, y;
	const int nTileSize = MAPPARTSSIZE;
	SIZE size;
	CPoint ptTmp;

	if (m_pInfoMap == NULL) {
		return;
	}
	{
		CInfoMapBase* pCurrentMap;
		if ((m_pHost) && (m_pHost->GetMapData)) {
			pCurrentMap = (CInfoMapBase*)m_pHost->GetMapData(m_pHost->userData);
		} else {
			pCurrentMap = m_pMgrData->GetMap();
		}
		if (m_pInfoMap != pCurrentMap) {
			PostMessage(WM_CLOSE);
			return;
		}
	}

	ptTmp = point;
	size = m_wndToolBar.CalcFixedLayout(FALSE, TRUE);
	ptTmp.y -= size.cy;

	x  = GetScrollPos(SB_HORZ);
	y  = GetScrollPos(SB_VERT);
	x += (ptTmp.x / nTileSize);
	y += (ptTmp.y / nTileSize);
	if ((x == m_ptBack.x) && (y == m_ptBack.y)) {
		return;
	}

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
		OnLButtonDown(-1, point);

	} else if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
		if (m_bRClickFirst) {
			m_rcRange.SetRect(m_ptBack.x, m_ptBack.y, m_ptBack.x, m_ptBack.y);
			m_bRClickFirst = FALSE;
		}
		if (GetCheck(TOOLBAR_RANGE)) {
			OnLButtonDown(-1, point);

		} else {
			if (GetCheck(TOOLBAR_PASTE) == FALSE) {
				if (m_rcCopy.left == -1) {
					SetCheck(TOOLBAR_RANGE, TRUE);
					m_rcRange.SetRect(m_ptBack.x, m_ptBack.y, m_ptBack.x, m_ptBack.y);
					m_bRClickFirst = FALSE;
				}
			}
		}
	}

	m_ptBack.x = x;
	m_ptBack.y = y;

	InvalidateRect(NULL);
}

void CWndMap::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
#if 0
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
#endif
	CWnd::OnGetMinMaxInfo(lpMMI);
}

void CWndMap::PostNcDestroy()
{
	delete this;
}

void CWndMap::RenewScrollRange(int cx, int cy)
{
	CRect rc;
	SCROLLINFO stScrollInfo;
	const int nTileSize = MAPPARTSSIZE;

	m_StatusBar.GetWindowRect(rc);

	m_pImgBack->Destroy();
	m_pImgBack->Create(cx, cy - rc.Height());

	if (m_pInfoMap == NULL) {
		return;
	}

	// スクロールバーの最大値を更新
	SetScrollRange(SB_HORZ, 0, m_pInfoMap->m_sizeMap.cx);
	SetScrollRange(SB_VERT, 0, m_pInfoMap->m_sizeMap.cy);

	GetScrollInfo(SB_HORZ, &stScrollInfo);
	stScrollInfo.nPage = max(1, (m_pImgBack->Width() / nTileSize) - 1);
	SetScrollInfo(SB_HORZ, &stScrollInfo);
	GetScrollInfo(SB_VERT, &stScrollInfo);
	stScrollInfo.nPage = max(1, (m_pImgBack->Height() / nTileSize) - 1);
	SetScrollInfo(SB_VERT, &stScrollInfo);
}

void CWndMap::RenewToolbar(void)
{
	CToolBarCtrl *pToolBarCtrl;
	TBBUTTON Button;

	pToolBarCtrl = &m_wndToolBar.GetToolBarCtrl();

	ZeroMemory(&Button, sizeof (Button));
	Button.iBitmap	= 0;
	Button.idCommand	= TOOLBAR_COPY;
	pToolBarCtrl->AddButtons(1, &Button);

	Button.iBitmap	= 1;
	Button.idCommand	= TOOLBAR_PASTE;
	Button.fsStyle	= TBSTYLE_CHECK;
	pToolBarCtrl->AddButtons(1, &Button);

	Button.iBitmap	= 2;
	Button.idCommand	= TOOLBAR_PAINT;
	Button.fsStyle	= TBSTYLE_BUTTON;
	pToolBarCtrl->AddButtons(1, &Button);

	Button.iBitmap	= 3;
	Button.idCommand	= TOOLBAR_RANGE;
	Button.fsStyle	= TBSTYLE_CHECK;
	pToolBarCtrl->AddButtons(1, &Button);

	Button.iBitmap	= 4;
	Button.idCommand	= TOOLBAR_GRID;
	Button.fsStyle	= TBSTYLE_CHECK;
	pToolBarCtrl->AddButtons(1, &Button);
}

void CWndMap::RenewStatusText(void)
{
	int x, y;
	BOOL bResult;
	const int nTileSize = MAPPARTSSIZE;
	CPoint point;
	CRect rcRangeTmp;
	CSize size;
	CString strTmp, strPane;

	GetCursorPos(&point);
	ScreenToClient(&point);
	size = m_wndToolBar.CalcFixedLayout(FALSE, TRUE);
	point.y -= size.cy;

	x  = GetScrollPos(SB_HORZ);
	y  = GetScrollPos(SB_VERT);
	x += (point.x / nTileSize);
	y += (point.y / nTileSize);

	strPane.Format(_T("座標(%3d,%3d)"), x, y);

	// 範囲選択中？
	bResult = GetCheck(TOOLBAR_RANGE);
	if (bResult) {
		rcRangeTmp = m_rcRange;
		if (rcRangeTmp.left > rcRangeTmp.right) {
			rcRangeTmp.left  = m_rcRange.right;
			rcRangeTmp.right = m_rcRange.left;
		}
		if (rcRangeTmp.top > rcRangeTmp.bottom) {
			rcRangeTmp.top	= m_rcRange.bottom;
			rcRangeTmp.bottom	= m_rcRange.top;
		}
		if (rcRangeTmp.left != -1) {
			strTmp.Format(_T(" 選択(%3d,%3d) 範囲(%3d,%3d)"),
					rcRangeTmp.left, rcRangeTmp.top, rcRangeTmp.Width() + 1, rcRangeTmp.Height() + 1);
			strPane += strTmp;
		}
	}
	// 張り付け中？
	bResult = GetCheck(TOOLBAR_PASTE);
	if (bResult) {
		rcRangeTmp = m_rcCopy;
		if (rcRangeTmp.left > rcRangeTmp.right) {
			rcRangeTmp.left  = m_rcCopy.right;
			rcRangeTmp.right = m_rcCopy.left;
		}
		if (rcRangeTmp.top > rcRangeTmp.bottom) {
			rcRangeTmp.top	= m_rcCopy.bottom;
			rcRangeTmp.bottom	= m_rcCopy.top;
		}
		if (rcRangeTmp.left != -1) {
			strTmp.Format(_T(" 選択(%3d,%3d) 範囲(%3d,%3d)"),
					rcRangeTmp.left, rcRangeTmp.top, rcRangeTmp.Width(), rcRangeTmp.Height());
			strPane += strTmp;
		}
	}

	m_StatusBar.SetPaneText(0, strPane);
}

BOOL CWndMap::GetCheck(int nNo)
{
	BOOL bRet;
	int nState;

	bRet = FALSE;
	nState = m_wndToolBar.GetToolBarCtrl().GetState(nNo);
	if (nState & TBSTATE_CHECKED) {
		bRet = TRUE;
	}

	return bRet;
}

void CWndMap::SetCheck(int nNo, BOOL bCheck)
{
	int nState;

	nState = (bCheck) ? TBSTATE_CHECKED : 0;
	m_wndToolBar.GetToolBarCtrl().SetState(nNo, nState);
}

void CWndMap::OnCopy()
{
	int xx, yy;
	DWORD dwPartsID;

	SetCheck(TOOLBAR_RANGE, FALSE);
	m_rcCopy = m_rcRange;
	if (m_rcCopy.left > m_rcCopy.right) {
		m_rcCopy.left  = m_rcRange.right;
		m_rcCopy.right = m_rcRange.left;
	}
	if (m_rcCopy.top > m_rcCopy.bottom) {
		m_rcCopy.top	= m_rcRange.bottom;
		m_rcCopy.bottom	= m_rcRange.top;
	}

	m_rcCopy.right ++;
	m_rcCopy.bottom ++;

	SAFE_DELETE(m_pdwParts);
	m_pdwParts = new DWORD[m_rcCopy.Width() * m_rcCopy.Height()];

	// コピー内容を作成
	for (yy = 0; yy < m_rcCopy.Height(); yy ++) {
		for (xx = 0; xx < m_rcCopy.Width(); xx ++) {
			dwPartsID = m_pInfoMap->GetParts(m_rcCopy.left + xx, m_rcCopy.top + yy);
			m_pdwParts[xx + yy * m_rcCopy.Width()] = dwPartsID;
		}
	}
}

void CWndMap::OnPaste()
{
	if (m_rcCopy.left == -1) {
		return;
	}
	SetCheck(TOOLBAR_RANGE, FALSE);
}

void CWndMap::OnToolPaint()
{
	DWORD dwPartsID;
	int x, y, xx, yy;
	CPacketADMIN_MAP_SETPARTS Packet;

	if (m_rcRange.left == -1) {
		return;
	}
	if (GetCheck(TOOLBAR_RANGE) == FALSE) {
		return;
	}

	x  = m_rcRange.left;
	y  = m_rcRange.top;

	dwPartsID = ((m_pHost) && (m_pHost->GetSelectMapPartsID)) ? m_pHost->GetSelectMapPartsID(m_pHost->userData) : m_pMgrData->GetSelectMapPartsID();
	if (m_rcRange.left != -1) {
		for (yy = 0; yy <= m_rcRange.Height(); yy ++) {
			for (xx = 0; xx <= m_rcRange.Width(); xx ++) {
				m_pInfoMap->SetParts(x + xx, y + yy, dwPartsID);
				Packet.Make(m_pInfoMap->m_dwMapID, x + xx, y + yy, dwPartsID, FALSE);
				SendPacket(&Packet);
			}
		}
	}
	InvalidateRect(NULL);
}

void CWndMap::OnRange()
{
	SetCheck(TOOLBAR_PASTE, FALSE);
}

void CWndMap::OnGrid()
{
	m_bViewGrid = (m_bViewGrid) ? FALSE : TRUE;
	SetCheck(TOOLBAR_GRID, m_bViewGrid);

	InvalidateRect(NULL);
}

void CWndMap::SendPacket(CPacketBase* pPacket)
{
	if (pPacket == NULL) {
		return;
	}
	if ((m_pHost) && (m_pHost->SendAdminPacket)) {
		m_pHost->SendAdminPacket(m_pHost->userData, pPacket);
	}
}

