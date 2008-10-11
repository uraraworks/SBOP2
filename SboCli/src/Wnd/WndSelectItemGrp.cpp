/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WndSelectItemGrp.cpp										 */
/* ���e			:�A�C�e���摜�I���E�B���h�E�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/21													 */
/* ========================================================================= */

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "WndSelectItemGrp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* ========================================================================= */
/* �N���X�ݒ�																 */
/* ========================================================================= */

BEGIN_MESSAGE_MAP(CWndSelectItemGrp, CWnd)
	//{{AFX_MSG_MAP(CWndSelectItemGrp)
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
/* �֐���	:CWndSelectItemGrp::CWndSelectItemGrp							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/08/21														 */
/* ========================================================================= */

CWndSelectItemGrp::CWndSelectItemGrp()
{
	m_nSelect		= -1;
	m_pWndParent	= NULL;
	m_pMgrData		= NULL;
	m_pMgrGrpData	= NULL;

	m_pImgBack		= new CImg32;
}


/* ========================================================================= */
/* �֐���	:CWndSelectItemGrp::~CWndSelectItemGrp							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/08/21														 */
/* ========================================================================= */

CWndSelectItemGrp::~CWndSelectItemGrp()
{
	SAFE_DELETE (m_pImgBack);
}


/* ========================================================================= */
/* �֐���	:CWndSelectItemGrp::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2007/08/21														 */
/* ========================================================================= */

BOOL CWndSelectItemGrp::Create(CWnd *pParent, CMgrData *pMgrData)
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
			"�A�C�e���摜�̑I��",
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
/* �֐���	:CWndSelectItemGrp::Destroy										 */
/* ���e		:�j��															 */
/* ���t		:2007/08/21														 */
/* ========================================================================= */

void CWndSelectItemGrp::Destroy(void)
{
	DestroyWindow ();
}


/* ========================================================================= */
/* �֐���	:CWndSelectItemGrp::OnCreate									 */
/* ���e		:���b�Z�[�W�n���h��(WM_CREATE)									 */
/* ���t		:2007/08/21														 */
/* ========================================================================= */

int CWndSelectItemGrp::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int cx, cy, nCount;
	SCROLLINFO stScrollInfo;

	if (CWnd::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	/* �X�N���[���o�[�̐ݒ� */
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
/* �֐���	:CWndSelectItemGrp::OnClose										 */
/* ���e		:���b�Z�[�W�n���h��(WM_CLOSE)									 */
/* ���t		:2007/08/21														 */
/* ========================================================================= */

void CWndSelectItemGrp::OnClose()
{
	if (m_pWndParent) {
		m_pWndParent->PostMessage (WM_WNDCLOSE, WINDOWID_SELECTMAPPARTSGRP, (LPARAM)m_nSelect);
	}
	CWnd::OnClose ();
}


/* ========================================================================= */
/* �֐���	:CWndSelectItemGrp::OnPaint										 */
/* ���e		:���b�Z�[�W�n���h��(WM_PAINT)									 */
/* ���t		:2007/08/21														 */
/* ========================================================================= */

void CWndSelectItemGrp::OnPaint()
{
	int x, y, cx, cy, no, nCount;
	HDC hDC;
	CDC *pDCTmp;
	PCImg32 pImg;

	CPaintDC dc(this);

	nCount = m_pMgrGrpData->GetItemCount ();
	cx = 32;
	cy = m_pImgBack->Height () / 16 + 1;
	for (y = 0; y < cy; y ++) {
		for (x = 0; x < cx; x ++) {
			no = x + GetScrollPos (SB_HORZ) + ((y + GetScrollPos (SB_VERT)) * cx);
			pImg = m_pMgrGrpData->GetDibItemTmp (no / 1024);
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
/* �֐���	:CWndSelectItemGrp::OnSize										 */
/* ���e		:���b�Z�[�W�n���h��(WM_SIZE)									 */
/* ���t		:2007/08/21														 */
/* ========================================================================= */

void CWndSelectItemGrp::OnSize(UINT nType, int cx, int cy)
{
	int cxTmp, cyTmp, nCount;

	CWnd::OnSize(nType, cx, cy);

	m_pImgBack->Destroy ();
	m_pImgBack->Create (cx, cy);

	nCount = m_pMgrGrpData->GetItemCount ();

	/* �X�N���[���o�[�̍ő�l���X�V */
	cxTmp = 32 - (cx / 32) - 1;
	cyTmp = (nCount / 32) - (cy / 32) - 1;
	cxTmp = min (32 - 1, cxTmp);
	cyTmp = min (nCount / 32 - 1, cyTmp);
	SetScrollRange (SB_HORZ, 0, cxTmp);
	SetScrollRange (SB_VERT, 0, cyTmp);
}


/* ========================================================================= */
/* �֐���	:CWndSelectItemGrp::OnHScroll									 */
/* ���e		:���b�Z�[�W�n���h��(WM_HSCROLL)									 */
/* ���t		:2007/08/21														 */
/* ========================================================================= */

void CWndSelectItemGrp::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMin, nMax;
	BOOL bChange;
	UINT nPosNow;

	nPosNow = GetScrollPos (SB_HORZ);
	bChange = TRUE;

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
	case SB_PAGELEFT:		/* 1�y�[�W���փX�N���[�� */
		nPos = nPosNow - 1;
		break;
	case SB_LINERIGHT:		/* �E�փX�N���[�� */
	case SB_PAGERIGHT:		/* 1�y�[�W�E�փX�N���[�� */
		nPos = nPosNow + 1;
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
/* �֐���	:CWndSelectItemGrp::OnVScroll									 */
/* ���e		:���b�Z�[�W�n���h��(WM_VSCROLL)									 */
/* ���t		:2007/08/21														 */
/* ========================================================================= */

void CWndSelectItemGrp::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMin, nMax;
	BOOL bChange;
	UINT nPosNow;

	nPosNow = GetScrollPos (SB_VERT);
	bChange = TRUE;

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
	case SB_PAGEUP:			/* 1�y�[�W��փX�N���[�� */
		nPos = nPosNow - 1;
		break;
	case SB_LINEDOWN:		/* 1�s���փX�N���[�� */
	case SB_PAGEDOWN:		/* 1�y�[�W���փX�N���[�� */
		nPos = nPosNow + 1;
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
/* �֐���	:CWndSelectItemGrp::OnSetCursor									 */
/* ���e		:���b�Z�[�W�n���h��(WM_SETCURSOR)								 */
/* ���t		:2007/08/21														 */
/* ========================================================================= */

BOOL CWndSelectItemGrp::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
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
/* �֐���	:CWndSelectItemGrp::OnLButtonDown								 */
/* ���e		:���b�Z�[�W�n���h��(WM_LBUTTONDOWN)								 */
/* ���t		:2007/08/21														 */
/* ========================================================================= */

void CWndSelectItemGrp::OnLButtonDown(UINT nFlags, CPoint point)
{
	int no, x, y, cx;
	PCImg32 pImg;

	pImg = m_pMgrGrpData->GetDibItemTmp (0);

	if ((point.x >= pImg->Width ()) || (point.y >= pImg->Height ())) {
		goto Exit;
	}

	cx = pImg->Width () / 16;
	x = point.x / 16;
	y = point.y / 16;
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
/* �֐���	:CWndSelectItemGrp::OnRButtonDown								 */
/* ���e		:���b�Z�[�W�n���h��(WM_RBUTTONDOWN)								 */
/* ���t		:2007/08/21														 */
/* ========================================================================= */

void CWndSelectItemGrp::OnRButtonDown(UINT nFlags, CPoint point)
{
	PostMessage (WM_CLOSE, 0, 0);
	CWnd::OnRButtonDown (nFlags, point);
}


/* ========================================================================= */
/* �֐���	:CWndSelectItemGrp::OnMouseWheel								 */
/* ���e		:���b�Z�[�W�n���h��(WM_MOUSEWHEEL)								 */
/* ���t		:2007/08/21														 */
/* ========================================================================= */

BOOL CWndSelectItemGrp::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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
/* �֐���	:CWndSelectItemGrp::PostNcDestroy								 */
/* ���e		:�I������														 */
/* ���t		:2007/08/21														 */
/* ========================================================================= */

void CWndSelectItemGrp::PostNcDestroy()
{
	delete this;
}

/* Copyright(C)URARA-works 2007 */
