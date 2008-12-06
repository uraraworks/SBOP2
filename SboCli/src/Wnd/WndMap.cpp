/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:WndMap.cpp													 */
/* ���e			:�}�b�v�E�B���h�E�N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/09/12													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoMapBase.h"
#include "PacketADMIN_MAP_SETPARTS.h"
#include "UraraSockTCPSBO.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrDraw.h"
#include "WndMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_VIEW_TOOLBAR1	100
enum {
	TOOLBAR_COPY = 1,		/* �R�s�[ */
	TOOLBAR_PASTE,			/* �\��t�� */
	TOOLBAR_PAINT,			/* �h��Ԃ� */
	TOOLBAR_RANGE,			/* �͈͑I�� */
	TOOLBAR_GRID,			/* �O���b�h�\�� */
};

/* �X�e�[�^�X�o�[�ݒ�p��� */
static UINT indicators[] =
{
	ID_SEPARATOR,           // �X�e�[�^�X ���C�� �C���W�P�[�^
};


/* ========================================================================= */
/* �N���X�ݒ�																 */
/* ========================================================================= */

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


/* ========================================================================= */
/* �֐���	:CWndMap::CWndMap												 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/09/12														 */
/* ========================================================================= */

CWndMap::CWndMap()
{
	m_bRClickFirst	= FALSE;
	m_bViewGrid		= FALSE;
	m_nSelect		= -1;
	m_nNotify		= 0;
	m_pdwParts		= NULL;
	m_pWndParent	= NULL;
	m_pMgrData		= NULL;
	m_pMgrGrpData	= NULL;
	m_pInfoMap		= NULL;
	m_pMgrDraw		= NULL;
	m_pSock			= NULL;

	m_pImgBack		= new CImg32;
	m_pImgBase		= new CImg32;

	m_rcRange.left = -1;
	m_rcCopy. left = -1;
	ZeroMemory (&m_ptBack, sizeof (m_ptBack));
}


/* ========================================================================= */
/* �֐���	:CWndMap::~CWndMap												 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/09/12														 */
/* ========================================================================= */

CWndMap::~CWndMap()
{
	SAFE_DELETE (m_pdwParts);
	SAFE_DELETE (m_pMgrDraw);
	SAFE_DELETE (m_pImgBack);
	SAFE_DELETE (m_pImgBase);
}


/* ========================================================================= */
/* �֐���	:CWndMap::Create												 */
/* ���e		:�쐬															 */
/* ���t		:2008/09/12														 */
/* ========================================================================= */

BOOL CWndMap::Create(CWnd *pParent, CMgrData *pMgrData, int nNotify)
{
	BOOL bRet;
	CRect rc, rcParent;

	m_pWndParent	= pParent;
	m_pMgrData		= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData ();
	m_nNotify		= nNotify;
	m_pSock			= m_pMgrData->GetUraraSockTCP ();
	m_pInfoMap		= m_pMgrData->GetMap ();

	m_pMgrDraw = new CMgrDraw;
	m_pMgrDraw->Create (m_pMgrData);

	pParent->GetWindowRect (rcParent);
	rc.SetRect (rcParent.left, rcParent.top, rcParent.left + 460, rcParent.top + 300);
	AdjustWindowRect (&rc, WS_EX_TOOLWINDOW, FALSE);

	bRet = CWnd::CreateEx (
			WS_EX_TOOLWINDOW,
			AfxRegisterWndClass (CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS),
			"�}�b�v�E�B���h�E",
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
/* �֐���	:CWndMap::Destroy												 */
/* ���e		:�j��															 */
/* ���t		:2008/09/12														 */
/* ========================================================================= */

void CWndMap::Destroy(void)
{
	DestroyWindow ();
}


/* ========================================================================= */
/* �֐���	:CWndMap::OnCreate												 */
/* ���e		:���b�Z�[�W�n���h��(WM_CREATE)									 */
/* ���t		:2008/09/12														 */
/* ========================================================================= */

int CWndMap::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int cx, cy, nCount;
	SIZE sizeTmp;
	SCROLLINFO stScrollInfo;

	if (CWnd::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	if (!m_wndToolBar.CreateEx (this, TBSTYLE_LIST | TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP, CRect(0,0,0,0), ID_VIEW_TOOLBAR1)) {
		return -1;
	}
	m_ImgListToolBar.Create (IDB_TOOLBAR, 16, 5, RGB (192, 192, 192));
	m_wndToolBar.GetToolBarCtrl ().SetImageList (&m_ImgListToolBar);

	RenewToolbar ();

	/* �X�N���[���o�[�̐ݒ� */
	ModifyStyle (0, WS_VSCROLL | WS_HSCROLL);
	nCount = m_pMgrGrpData->GetMapPartsCount ();
	cx = m_pInfoMap->m_sizeMap.cx;
	cy = m_pInfoMap->m_sizeMap.cy;
	SetScrollRange (SB_HORZ, 0, cx);
	SetScrollRange (SB_VERT, 0, cy);

	GetScrollInfo (SB_HORZ, &stScrollInfo);
	stScrollInfo.nPage = 1;
	SetScrollInfo (SB_HORZ, &stScrollInfo);
	GetScrollInfo (SB_VERT, &stScrollInfo);
	stScrollInfo.nPage = 1;
	SetScrollInfo (SB_VERT, &stScrollInfo);

	sizeTmp = m_pMgrData->GetWndMap ();
	SetWindowPos (NULL, 0, 0, sizeTmp.cx, sizeTmp.cy, SWP_NOZORDER | SWP_NOMOVE);

	/* �X�e�[�^�X�o�[�̐ݒ� */
	if (!m_StatusBar.Create (this) ||
		!m_StatusBar.SetIndicators (indicators, sizeof (indicators) / sizeof (UINT))) {
		return -1;
	}
	RepositionBars (AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	return 0;
}


/* ========================================================================= */
/* �֐���	:CWndMap::OnUpdateCommandUI										 */
/* ���e		:�c�[���o�[�{�^��������ԃ`�F�b�N�n���h��						 */
/* ���t		:2008/09/13														 */
/* ========================================================================= */

void CWndMap::OnUpdateCommandUI(CCmdUI* pCmdUI)
{
	pCmdUI->Enable ();
}


/* ========================================================================= */
/* �֐���	:CWndMap::OnClose												 */
/* ���e		:���b�Z�[�W�n���h��(WM_CLOSE)									 */
/* ���t		:2008/09/12														 */
/* ========================================================================= */

void CWndMap::OnClose()
{
	CRect rcTmp;
	SIZE sizeTmp;

	GetWindowRect (rcTmp);
	sizeTmp.cx = rcTmp.Width ();
	sizeTmp.cy = rcTmp.Height ();
	m_pMgrData->SetWndMap (sizeTmp);

	if (m_pWndParent) {
		m_pWndParent->PostMessage (
				WM_WNDCLOSE,
				WINDOWID_SELECTMAPPARTSGRP,
				(LPARAM)MAKELPARAM ((WORD)m_nSelect, (WORD)m_nNotify));
	}
	CWnd::OnClose ();
}


/* ========================================================================= */
/* �֐���	:CWndMap::OnPaint												 */
/* ���e		:���b�Z�[�W�n���h��(WM_PAINT)									 */
/* ���t		:2008/09/12														 */
/* ========================================================================= */

void CWndMap::OnPaint()
{
	BOOL bResult;
	int x, y, xx, yy, cx, cy, xxx, yyy;
	HDC hDC;
	DWORD dwPartsID;
	POINT ptTmp;
	CRect rcTmp, rcRangeTmp;
	CSize size;
	CDC *pDCTmp;

	CPaintDC dc(this);
	size = m_wndToolBar.CalcFixedLayout (FALSE, TRUE);

	x  = GetScrollPos (SB_HORZ);
	y  = GetScrollPos (SB_VERT);
	cx = m_pImgBack->Width ();
	cy = m_pImgBack->Height ();
	m_pImgBack->FillRect (0, 0, cx, cy, 0);

	GetCursorPos (&ptTmp);
	ptTmp.y -= size.cy;
	ScreenToClient (&ptTmp);
	xxx = ptTmp.x / 16;
	yyy = ptTmp.y / 16;

	for (yy = 0; yy < cy / 16 + 1; yy ++) {
		for (xx = 0; xx < cx / 16 + 1; xx ++) {
			dwPartsID = m_pInfoMap->GetParts (xx + x, yy + y);
			m_pMgrDraw->DrawMapParts (m_pImgBack, xx * 16, yy * 16, dwPartsID, 2, TRUE);
			if (m_bViewGrid) {
				m_pImgBack->XorRect (xx * 16 + 15, yy * 16, 1, 16);
				m_pImgBack->XorRect (xx * 16, yy * 16 + 15, 16, 1);
			}
		}
	}

	rcRangeTmp = m_rcCopy;
	if (rcRangeTmp.left > rcRangeTmp.right) {
		rcRangeTmp.left  = m_rcCopy.right;
		rcRangeTmp.right = m_rcCopy.left;
	}
	if (rcRangeTmp.top > rcRangeTmp.bottom) {
		rcRangeTmp.top		= m_rcCopy.bottom;
		rcRangeTmp.bottom	= m_rcCopy.top;
	}

	/* ����t�����H */
	if (GetCheck (TOOLBAR_PASTE)) {
		/* �\��t���v���r���[��\������ */
		for (yy = 0; yy < rcRangeTmp.Height (); yy ++) {
			for (xx = 0; xx < rcRangeTmp.Width (); xx ++) {
				dwPartsID = m_pdwParts[xx + yy * rcRangeTmp.Width ()];
				m_pMgrDraw->DrawMapParts (m_pImgBack, (xx + xxx) * 16, (yy + yyy) * 16, dwPartsID, 2, TRUE, FALSE, 40);
			}
		}
	}

	rcRangeTmp = m_rcRange;
	if (rcRangeTmp.left > rcRangeTmp.right) {
		rcRangeTmp.left  = m_rcRange.right;
		rcRangeTmp.right = m_rcRange.left;
	}
	if (rcRangeTmp.top > rcRangeTmp.bottom) {
		rcRangeTmp.top		= m_rcRange.bottom;
		rcRangeTmp.bottom	= m_rcRange.top;
	}

	xx = ptTmp.x / 16 * 16;
	yy = ptTmp.y / 16 * 16;

	hDC = m_pImgBack->Lock ();
	pDCTmp = dc.FromHandle (hDC);

	bResult = GetCheck (TOOLBAR_RANGE);
	if (bResult && (rcRangeTmp.left != -1)) {
		rcTmp.SetRect (rcRangeTmp.left - x, rcRangeTmp.top - y, rcRangeTmp.right - x, rcRangeTmp.bottom - y);
		rcTmp.left		*= 16;
		rcTmp.top		*= 16;
		rcTmp.right		*= 16;
		rcTmp.bottom	*= 16;
		rcTmp.right		+= 15;
		rcTmp.bottom	+= 15;
	} else {
		rcTmp.SetRect (xx, yy, xx + 15, yy + 15);
	}
	pDCTmp->DrawEdge (rcTmp, EDGE_BUMP, BF_MONO | BF_RECT);

	dc.BitBlt (0, size.cy, cx, cy, pDCTmp, 0, 0, SRCCOPY);
//	m_pImgBack->Unlock ();

	RenewStatusText ();
}


/* ========================================================================= */
/* �֐���	:CWndMap::OnSize												 */
/* ���e		:���b�Z�[�W�n���h��(WM_SIZE)									 */
/* ���t		:2008/09/12														 */
/* ========================================================================= */

void CWndMap::OnSize(UINT nType, int cx, int cy)
{
	CSize size;

	CWnd::OnSize(nType, cx, cy);

	size = m_wndToolBar.CalcFixedLayout (FALSE, TRUE);
	m_wndToolBar.MoveWindow(0, 0, cx, size.cy);

	/* �X�N���[���o�[�̍ő�l���X�V */
	RenewScrollRange (cx, cy - size.cy);

	RepositionBars (AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
}


/* ========================================================================= */
/* �֐���	:CWndMap::OnHScroll												 */
/* ���e		:���b�Z�[�W�n���h��(WM_HSCROLL)									 */
/* ���t		:2008/09/12														 */
/* ========================================================================= */

void CWndMap::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMin, nMax;
	BOOL bChange;
	UINT nPosNow;
	SCROLLINFO stScrollInfo;

	nPosNow = GetScrollPos (SB_HORZ);
	bChange = TRUE;

	GetScrollInfo (SB_HORZ, &stScrollInfo);

	switch (nSBCode) {
	case SB_LEFT:			/* ���[�փX�N���[�� */
		GetScrollRange (SB_HORZ, &nMin, &nMax);
		nPos = nMin;
		break;
	case SB_RIGHT:			/* �E�[�փX�N���[�� */
		GetScrollRange (SB_HORZ, &nMin, &nMax);
		nPos = nMax;
		break;
	case SB_LINELEFT:		/* ���փX�N���[�� */
		nPos = nPosNow - 1;
		break;
	case SB_PAGELEFT:		/* 1�y�[�W���փX�N���[�� */
		nPos = nPosNow - stScrollInfo.nPage;
		break;
	case SB_LINERIGHT:		/* �E�փX�N���[�� */
		nPos = nPosNow + 1;
		break;
	case SB_PAGERIGHT:		/* 1�y�[�W�E�փX�N���[�� */
		nPos = nPosNow + stScrollInfo.nPage;
		break;
	case SB_THUMBPOSITION:	/* ��Έʒu�փX�N���[�� */
	case SB_THUMBTRACK:		/* �h���b�O���ꂽ */
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
/* �֐���	:CWndMap::OnVScroll												 */
/* ���e		:���b�Z�[�W�n���h��(WM_VSCROLL)									 */
/* ���t		:2008/09/12														 */
/* ========================================================================= */

void CWndMap::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMin, nMax;
	BOOL bChange;
	UINT nPosNow;
	SCROLLINFO stScrollInfo;

	nPosNow = GetScrollPos (SB_VERT);
	bChange = TRUE;

	GetScrollInfo (SB_VERT, &stScrollInfo);

	switch (nSBCode) {
	case SB_TOP:			/* 1�ԏ�܂ŃX�N���[�� */
		GetScrollRange (SB_VERT, &nMin, &nMax);
		nPos = nMin;
		break;
	case SB_BOTTOM:			/* 1�ԉ��܂ŃX�N���[�� */
		GetScrollRange (SB_VERT, &nMin, &nMax);
		nPos = nMax;
		break;
	case SB_LINEUP:			/* 1�s��փX�N���[�� */
		nPos = nPosNow - 1;
		break;
	case SB_PAGEUP:			/* 1�y�[�W��փX�N���[�� */
		nPos = nPosNow - stScrollInfo.nPage;
		break;
	case SB_LINEDOWN:		/* 1�s���փX�N���[�� */
		nPos = nPosNow + 1;
		break;
	case SB_PAGEDOWN:		/* 1�y�[�W���փX�N���[�� */
		nPos = nPosNow + stScrollInfo.nPage;
		break;
	case SB_THUMBPOSITION:	/* ��Έʒu�փX�N���[�� */
	case SB_THUMBTRACK:		/* �h���b�O���ꂽ */
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
/* �֐���	:CWndMap::OnSetCursor											 */
/* ���e		:���b�Z�[�W�n���h��(WM_SETCURSOR)								 */
/* ���t		:2008/09/12														 */
/* ========================================================================= */

BOOL CWndMap::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
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
/* �֐���	:CWndMap::OnLButtonDown											 */
/* ���e		:���b�Z�[�W�n���h��(WM_LBUTTONDOWN)								 */
/* ���t		:2008/09/12														 */
/* ========================================================================= */

void CWndMap::OnLButtonDown(UINT nFlags, CPoint point)
{
	DWORD dwPartsID;
	int x, y, xx, yy;
	CSize size;
	CPacketADMIN_MAP_SETPARTS Packet;

	if (m_pInfoMap == NULL) {
		return;
	}
	size = m_wndToolBar.CalcFixedLayout (FALSE, TRUE);
	point.y -= size.cy;

	x  = GetScrollPos (SB_HORZ);
	y  = GetScrollPos (SB_VERT);
	x += (point.x / 16);
	y += (point.y / 16);

	if ((x >= m_pInfoMap->m_sizeMap.cx) || (y >= m_pInfoMap->m_sizeMap.cy)) {
		return;
	}
	m_ptBack.x = x;
	m_ptBack.y = y;

	/* �͈͑I�𒆁H */
	if (GetCheck (TOOLBAR_RANGE)) {
		if (nFlags != -1) {
			m_rcRange.SetRect (x, y, x, y);
		} else {
			m_rcRange.right  = x;
			m_rcRange.bottom = y;
		}

	/* �\��t�����H */
	} else if (GetCheck (TOOLBAR_PASTE)) {
		if (m_rcCopy.left != -1) {
			for (yy = 0; yy < m_rcCopy.Height (); yy ++) {
				for (xx = 0; xx < m_rcCopy.Width (); xx ++) {
					dwPartsID = m_pdwParts[xx + yy * m_rcCopy.Width ()];
					m_pInfoMap->SetParts (x + xx, y + yy, dwPartsID);
					Packet.Make (m_pInfoMap->m_dwMapID, x + xx, y + yy, dwPartsID, FALSE);
					m_pSock->Send (&Packet);
				}
			}
		}

	} else {
		m_pInfoMap->SetParts (x, y, m_pMgrData->GetSelectMapPartsID ());
		Packet.Make (m_pInfoMap->m_dwMapID, x, y, m_pMgrData->GetSelectMapPartsID (), FALSE);
		m_pSock->Send (&Packet);
	}

	InvalidateRect (NULL);
}


/* ========================================================================= */
/* �֐���	:CWndMap::OnLButtonUp											 */
/* ���e		:���b�Z�[�W�n���h��(WM_LBUTTONUP)								 */
/* ���t		:2008/09/13														 */
/* ========================================================================= */

void CWndMap::OnLButtonUp(UINT nFlags, CPoint point)
{
	int x, y;
	CSize size;

	if (m_pInfoMap == NULL) {
		return;
	}
	size = m_wndToolBar.CalcFixedLayout (FALSE, TRUE);
	point.y -= size.cy;

	x  = GetScrollPos (SB_HORZ);
	y  = GetScrollPos (SB_VERT);
	x += (point.x / 16);
	y += (point.y / 16);

	if ((x >= m_pInfoMap->m_sizeMap.cx) || (y >= m_pInfoMap->m_sizeMap.cy)) {
		return;
	}

	/* �͈͑I�𒆁H */
	if (GetCheck (TOOLBAR_RANGE)) {
		m_wndToolBar.GetToolBarCtrl ().EnableButton (TOOLBAR_COPY,  TRUE);
		m_wndToolBar.GetToolBarCtrl ().EnableButton (TOOLBAR_PASTE, TRUE);
	}
}


/* ========================================================================= */
/* �֐���	:CWndMap::OnRButtonDown											 */
/* ���e		:���b�Z�[�W�n���h��(WM_RBUTTONDOWN)								 */
/* ���t		:2008/09/12														 */
/* ========================================================================= */

void CWndMap::OnRButtonDown(UINT nFlags, CPoint point)
{
	int x, y;
	DWORD dwPartsID;
	CSize size;

	if (m_pInfoMap == NULL) {
		return;
	}
	size = m_wndToolBar.CalcFixedLayout (FALSE, TRUE);
	point.y -= size.cy;

	x  = GetScrollPos (SB_HORZ);
	y  = GetScrollPos (SB_VERT);
	x += (point.x / 16);
	y += (point.y / 16);

	if ((x >= m_pInfoMap->m_sizeMap.cx) || (y >= m_pInfoMap->m_sizeMap.cy)) {
		return;
	}
	m_ptBack.x = x;
	m_ptBack.y = y;

	/* �͈͑I�𒆁H */
	if (GetCheck (TOOLBAR_RANGE)) {
		if (m_bRClickFirst == FALSE) {
			m_bRClickFirst = TRUE;
			/* �p�[�c���E�� */
			dwPartsID = m_pInfoMap->GetParts (x, y);
			m_pMgrData->SetSelectMapPartsID (dwPartsID);
		} else {
			m_rcRange.SetRect (x, y, x, y);
		}

	/* �\��t�����H */
	} else if (GetCheck (TOOLBAR_PASTE)) {
		m_rcRange.left = -1;
		m_rcCopy.left = -1;
		SetCheck (TOOLBAR_PASTE, FALSE);

	} else {
		/* �p�[�c���E�� */
		dwPartsID = m_pInfoMap->GetParts (x, y);
		m_pMgrData->SetSelectMapPartsID (dwPartsID);
	}
}


/* ========================================================================= */
/* �֐���	:CWndMap::OnRButtonUp											 */
/* ���e		:���b�Z�[�W�n���h��(WM_RBUTTONUP)								 */
/* ���t		:2008/09/14														 */
/* ========================================================================= */

void CWndMap::OnRButtonUp(UINT nFlags, CPoint point)
{
	int x, y;
	CSize size;
	CRect rcTmp;

	if (m_pInfoMap == NULL) {
		return;
	}
	size = m_wndToolBar.CalcFixedLayout (FALSE, TRUE);
	point.y -= size.cy;

	x  = GetScrollPos (SB_HORZ);
	y  = GetScrollPos (SB_VERT);
	x += (point.x / 16);
	y += (point.y / 16);

	if ((x >= m_pInfoMap->m_sizeMap.cx) || (y >= m_pInfoMap->m_sizeMap.cy)) {
		return;
	}

	/* �͈͑I�𒆁H */
	if (GetCheck (TOOLBAR_RANGE)) {
		if (m_bRClickFirst == FALSE) {
			rcTmp = m_rcRange;
			if (rcTmp.left > rcTmp.right) {
				rcTmp.left  = m_rcRange.right;
				rcTmp.right = m_rcRange.left;
			}
			if (rcTmp.top > rcTmp.bottom) {
				rcTmp.top		= m_rcRange.bottom;
				rcTmp.bottom	= m_rcRange.top;
			}
			rcTmp.right ++;
			rcTmp.bottom ++;
			if (rcTmp.IsRectEmpty () == FALSE) {
				OnCopy ();
				SetCheck (TOOLBAR_PASTE, TRUE);
				OnPaste ();
			}
		}
		m_bRClickFirst = FALSE;
	}

	InvalidateRect (NULL);
}


/* ========================================================================= */
/* �֐���	:CWndMap::OnRButtonDblClk										 */
/* ���e		:���b�Z�[�W�n���h��(WM_RBUTTONDBLCLK)							 */
/* ���t		:2008/09/14														 */
/* ========================================================================= */

void CWndMap::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_pInfoMap == NULL) {
		return;
	}

	/* �͈͑I�𒆁H */
	if (GetCheck (TOOLBAR_RANGE)) {
		m_rcRange.left = -1;
		m_rcCopy.left = -1;
		SetCheck (TOOLBAR_RANGE, FALSE);
	}

	InvalidateRect (NULL);
}


/* ========================================================================= */
/* �֐���	:CWndMap::OnMouseWheel											 */
/* ���e		:���b�Z�[�W�n���h��(WM_MOUSEWHEEL)								 */
/* ���t		:2008/09/12														 */
/* ========================================================================= */

BOOL CWndMap::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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
/* �֐���	:CWndMap::OnMouseMove											 */
/* ���e		:���b�Z�[�W�n���h��(WM_MOUSEMOVE)								 */
/* ���t		:2007/04/25														 */
/* ========================================================================= */

void CWndMap::OnMouseMove(UINT nFlags, CPoint point)
{
	int x, y;
	SIZE size;
	CPoint ptTmp;

	if (m_pInfoMap == NULL) {
		return;
	}
	if (m_pInfoMap != m_pMgrData->GetMap ()) {
		PostMessage (WM_CLOSE);
		return;
	}

	ptTmp = point;
	size = m_wndToolBar.CalcFixedLayout (FALSE, TRUE);
	ptTmp.y -= size.cy;

	x  = GetScrollPos (SB_HORZ);
	y  = GetScrollPos (SB_VERT);
	x += (ptTmp.x / 16);
	y += (ptTmp.y / 16);
	if ((x == m_ptBack.x) && (y == m_ptBack.y)) {
		return;
	}

	if (GetAsyncKeyState (VK_LBUTTON) & 0x8000) {
		OnLButtonDown (-1, point);

	} else if (GetAsyncKeyState (VK_RBUTTON) & 0x8000) {
		if (m_bRClickFirst) {
			m_rcRange.SetRect (m_ptBack.x, m_ptBack.y, m_ptBack.x, m_ptBack.y);
			m_bRClickFirst = FALSE;
		}
		if (GetCheck (TOOLBAR_RANGE)) {
			OnLButtonDown (-1, point);

		} else {
			if (GetCheck (TOOLBAR_PASTE) == FALSE) {
				if (m_rcCopy.left == -1) {
					SetCheck (TOOLBAR_RANGE, TRUE);
					m_rcRange.SetRect (m_ptBack.x, m_ptBack.y, m_ptBack.x, m_ptBack.y);
					m_bRClickFirst = FALSE;
				}
			}
		}
	}

	m_ptBack.x = x;
	m_ptBack.y = y;

	InvalidateRect (NULL);
}


/* ========================================================================= */
/* �֐���	:CWndMap::OnGetMinMaxInfo										 */
/* ���e		:���b�Z�[�W�n���h��(WM_MINMAXINFO)								 */
/* ���t		:2007/11/04														 */
/* ========================================================================= */

void CWndMap::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
#if 0
	int nMin, nMax;
	DWORD dwStyle;
	CRect rc;

	if (m_pImgBase->Width ()) {
		rc.SetRect (0, 0, m_pImgBase->Width (), m_pImgBase->Height ());
		dwStyle = GetStyle ();
		AdjustWindowRect (rc, dwStyle, FALSE);

		GetScrollRange (SB_HORZ, &nMin, &nMax);
		if (nMax) {
			rc.bottom += GetSystemMetrics (SM_CYHSCROLL);
		}

		lpMMI->ptMaxTrackSize.x = rc.Width ();
		lpMMI->ptMaxTrackSize.y = rc.Height ();
	}
#endif
	CWnd::OnGetMinMaxInfo (lpMMI);
}


/* ========================================================================= */
/* �֐���	:CWndMap::PostNcDestroy											 */
/* ���e		:�I������														 */
/* ���t		:2008/09/12														 */
/* ========================================================================= */

void CWndMap::PostNcDestroy()
{
	delete this;
}


/* ========================================================================= */
/* �֐���	:CWndMap::RenewScrollRange										 */
/* ���e		:�X�N���[���o�[�͈̔͂��X�V										 */
/* ���t		:2008/09/12														 */
/* ========================================================================= */

void CWndMap::RenewScrollRange(int cx, int cy)
{
	CRect rc;
	SCROLLINFO stScrollInfo;

	m_StatusBar.GetWindowRect (rc);

	m_pImgBack->Destroy ();
	m_pImgBack->Create (cx, cy - rc.Height ());

	if (m_pInfoMap == NULL) {
		return;
	}

	/* �X�N���[���o�[�̍ő�l���X�V */
	SetScrollRange (SB_HORZ, 0, m_pInfoMap->m_sizeMap.cx);
	SetScrollRange (SB_VERT, 0, m_pInfoMap->m_sizeMap.cy);

	GetScrollInfo (SB_HORZ, &stScrollInfo);
	stScrollInfo.nPage = (m_pImgBack->Width () / 16) - 1;
	SetScrollInfo (SB_HORZ, &stScrollInfo);
	GetScrollInfo (SB_VERT, &stScrollInfo);
	stScrollInfo.nPage = (m_pImgBack->Height () / 16) - 1;
	SetScrollInfo (SB_VERT, &stScrollInfo);
}


/* ========================================================================= */
/* �֐���	:CWndMap::RenewToolbar											 */
/* ���e		:�c�[���o�[���X�V												 */
/* ���t		:2008/09/13														 */
/* ========================================================================= */

void CWndMap::RenewToolbar(void)
{
	CToolBarCtrl *pToolBarCtrl;
	TBBUTTON Button;

	pToolBarCtrl = &m_wndToolBar.GetToolBarCtrl ();

	ZeroMemory (&Button, sizeof (Button));
	Button.iBitmap		= 0;
	Button.idCommand	= TOOLBAR_COPY;
	pToolBarCtrl->AddButtons (1, &Button);

	Button.iBitmap		= 1;
	Button.idCommand	= TOOLBAR_PASTE;
	Button.fsStyle		= TBSTYLE_CHECK;
	pToolBarCtrl->AddButtons (1, &Button);

	Button.iBitmap		= 2;
	Button.idCommand	= TOOLBAR_PAINT;
	Button.fsStyle		= TBSTYLE_BUTTON;
	pToolBarCtrl->AddButtons (1, &Button);

	Button.iBitmap		= 3;
	Button.idCommand	= TOOLBAR_RANGE;
	Button.fsStyle		= TBSTYLE_CHECK;
	pToolBarCtrl->AddButtons (1, &Button);

	Button.iBitmap		= 4;
	Button.idCommand	= TOOLBAR_GRID;
	Button.fsStyle		= TBSTYLE_CHECK;
	pToolBarCtrl->AddButtons (1, &Button);
}


/* ========================================================================= */
/* �֐���	:CWndMap::RenewStatusText										 */
/* ���e		:�X�e�[�^�X�o�[�̃e�L�X�g���X�V									 */
/* ���t		:2008/11/08														 */
/* ========================================================================= */

void CWndMap::RenewStatusText(void)
{
	int x, y;
	BOOL bResult;
	CPoint point;
	CRect rcRangeTmp;
	CSize size;
	CString strTmp, strPane;

	GetCursorPos (&point);
	ScreenToClient (&point);
	size = m_wndToolBar.CalcFixedLayout (FALSE, TRUE);
	point.y -= size.cy;

	x  = GetScrollPos (SB_HORZ);
	y  = GetScrollPos (SB_VERT);
	x += (point.x / 16);
	y += (point.y / 16);

	strPane.Format ("���W(%3d,%3d)", x, y);

	/* �͈͑I�𒆁H */
	bResult = GetCheck (TOOLBAR_RANGE);
	if (bResult) {
		rcRangeTmp = m_rcRange;
		if (rcRangeTmp.left > rcRangeTmp.right) {
			rcRangeTmp.left  = m_rcRange.right;
			rcRangeTmp.right = m_rcRange.left;
		}
		if (rcRangeTmp.top > rcRangeTmp.bottom) {
			rcRangeTmp.top		= m_rcRange.bottom;
			rcRangeTmp.bottom	= m_rcRange.top;
		}
		if (rcRangeTmp.left != -1) {
			strTmp.Format (" �I��(%3d,%3d) �͈�(%3d,%3d)",
					rcRangeTmp.left, rcRangeTmp.top, rcRangeTmp.Width () + 1, rcRangeTmp.Height () + 1);
			strPane += strTmp;
		}
	}
	/* ����t�����H */
	bResult = GetCheck (TOOLBAR_PASTE);
	if (bResult) {
		rcRangeTmp = m_rcCopy;
		if (rcRangeTmp.left > rcRangeTmp.right) {
			rcRangeTmp.left  = m_rcCopy.right;
			rcRangeTmp.right = m_rcCopy.left;
		}
		if (rcRangeTmp.top > rcRangeTmp.bottom) {
			rcRangeTmp.top		= m_rcCopy.bottom;
			rcRangeTmp.bottom	= m_rcCopy.top;
		}
		if (rcRangeTmp.left != -1) {
			strTmp.Format (" �I��(%3d,%3d) �͈�(%3d,%3d)",
					rcRangeTmp.left, rcRangeTmp.top, rcRangeTmp.Width (), rcRangeTmp.Height ());
			strPane += strTmp;
		}
	}

	m_StatusBar.SetPaneText (0, strPane);
}


/* ========================================================================= */
/* �֐���	:CWndMap::GetCheck												 */
/* ���e		:�c�[���o�[�̃{�^����Ԃ��擾									 */
/* ���t		:2008/09/13														 */
/* ========================================================================= */

BOOL CWndMap::GetCheck(int nNo)
{
	BOOL bRet;
	int nState;

	bRet = FALSE;
	nState = m_wndToolBar.GetToolBarCtrl ().GetState (nNo);
	if (nState & TBSTATE_CHECKED) {
		bRet = TRUE;
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWndMap::SetCheck												 */
/* ���e		:�c�[���o�[�̃{�^����Ԃ�ݒ�									 */
/* ���t		:2008/09/13														 */
/* ========================================================================= */

void CWndMap::SetCheck(int nNo, BOOL bCheck)
{
	int nState;

	nState = (bCheck) ? TBSTATE_CHECKED : 0;
	m_wndToolBar.GetToolBarCtrl ().SetState (nNo, nState);
}


/* ========================================================================= */
/* �֐���	:CWndMap::OnCopy												 */
/* ���e		:�{�^���n���h��(�R�s�[)											 */
/* ���t		:2008/09/13														 */
/* ========================================================================= */

void CWndMap::OnCopy()
{
	int xx, yy;
	DWORD dwPartsID;

	SetCheck (TOOLBAR_RANGE, FALSE);
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

	SAFE_DELETE (m_pdwParts);
	m_pdwParts = new DWORD[m_rcCopy.Width () * m_rcCopy.Height ()];

	/* �R�s�[���e���쐬 */
	for (yy = 0; yy < m_rcCopy.Height (); yy ++) {
		for (xx = 0; xx < m_rcCopy.Width (); xx ++) {
			dwPartsID = m_pInfoMap->GetParts (m_rcCopy.left + xx, m_rcCopy.top + yy);
			m_pdwParts[xx + yy * m_rcCopy.Width ()] = dwPartsID;
		}
	}
}


/* ========================================================================= */
/* �֐���	:CWndMap::OnPaste												 */
/* ���e		:�{�^���n���h��(�\��t��)										 */
/* ���t		:2008/09/13														 */
/* ========================================================================= */

void CWndMap::OnPaste()
{
	if (m_rcCopy.left == -1) {
		return;
	}
	SetCheck (TOOLBAR_RANGE, FALSE);
}


/* ========================================================================= */
/* �֐���	:CWndMap::OnToolPaint											 */
/* ���e		:�{�^���n���h��(�h��Ԃ�)										 */
/* ���t		:2008/09/13														 */
/* ========================================================================= */

void CWndMap::OnToolPaint()
{
	DWORD dwPartsID;
	int x, y, xx, yy;
	CPacketADMIN_MAP_SETPARTS Packet;

	if (m_rcRange.left == -1) {
		return;
	}
	if (GetCheck (TOOLBAR_RANGE) == FALSE) {
		return;
	}

	x  = m_rcRange.left;
	y  = m_rcRange.top;

	dwPartsID = m_pMgrData->GetSelectMapPartsID ();
	if (m_rcRange.left != -1) {
		for (yy = 0; yy <= m_rcRange.Height (); yy ++) {
			for (xx = 0; xx <= m_rcRange.Width (); xx ++) {
				m_pInfoMap->SetParts (x + xx, y + yy, dwPartsID);
				Packet.Make (m_pInfoMap->m_dwMapID, x + xx, y + yy, dwPartsID, FALSE);
				m_pSock->Send (&Packet);
			}
		}
	}
	InvalidateRect (NULL);
}


/* ========================================================================= */
/* �֐���	:CWndMap::OnRange												 */
/* ���e		:�{�^���n���h��(�͈͎w��)										 */
/* ���t		:2008/09/13														 */
/* ========================================================================= */

void CWndMap::OnRange()
{
	SetCheck (TOOLBAR_PASTE, FALSE);
}


/* ========================================================================= */
/* �֐���	:CWndMap::OnGrid												 */
/* ���e		:�{�^���n���h��(�O���b�h�\��)									 */
/* ���t		:2008/11/25														 */
/* ========================================================================= */

void CWndMap::OnGrid()
{
	m_bViewGrid = (m_bViewGrid) ? FALSE : TRUE;
	SetCheck (TOOLBAR_GRID, m_bViewGrid);

	InvalidateRect (NULL);
}

/* Copyright(C)URARA-works 2008 */
