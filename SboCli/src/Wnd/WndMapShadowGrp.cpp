/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WndMapShadowGrp.cpp										 */
/* ���e			:�}�b�v�e�ꗗ�X�^�e�B�b�N�R���g���[���N���X �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/06													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMapShadow.h"
#include "InfoMapShadow.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrDraw.h"
#include "MgrGrpData.h"
#include "WndMapShadowGrp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* ========================================================================= */
/* �N���X�ݒ�																 */
/* ========================================================================= */

BEGIN_MESSAGE_MAP(CWndMapShadowGrp, CWnd)
	//{{AFX_MSG_MAP(CWndMapShadowGrp)
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


/* ========================================================================= */
/* �֐���	:CWndMapShadowGrp::CWndMapShadowGrp								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

CWndMapShadowGrp::CWndMapShadowGrp()
{
	m_nPos				= 0;
	m_pWndParent		= NULL;
	m_pMgrData			= NULL;
	m_pMgrGrpData		= NULL;

	m_pImgShadow		= new CImg32;

	m_ptCursor.x	= m_ptCursor.y	= 0;
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowGrp::~CWndMapShadowGrp							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

CWndMapShadowGrp::~CWndMapShadowGrp()
{
	SAFE_DELETE (m_pImgShadow);
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowGrp::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

BOOL CWndMapShadowGrp::Create(CWnd *pParent, CMgrData *pMgrData, int nResourceID)
{
	BOOL bRet;
	CWnd *pWnd;
	CRect rc;

	bRet = FALSE;

	pWnd = pParent->GetDlgItem (nResourceID);
	if (pWnd == NULL) {
		goto Exit;
	}
	pWnd->GetWindowRect (rc);
	pParent->ScreenToClient (rc);

	m_pWndParent	= pParent;
	m_pMgrData		= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData ();

	bRet = CWnd::Create (
			AfxRegisterWndClass (CS_HREDRAW | CS_VREDRAW),
			"",
			WS_CHILD | WS_VISIBLE,
			rc,
			pParent,
			0);
	if (bRet == FALSE) {
		goto Exit;
	}

	pWnd->ShowWindow (SW_HIDE);
	SetScrollPos (SB_HORZ, 0);
	SetScrollPos (SB_VERT, 0);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowGrp::Destroy										 */
/* ���e		:�j��															 */
/* ���t		:2007/05/17														 */
/* ========================================================================= */

void CWndMapShadowGrp::Destroy(void)
{
	if (m_hWnd) {
		DestroyWindow ();
	}
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowGrp::OnCreate										 */
/* ���e		:���b�Z�[�W�n���h��(WM_CREATE)									 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

int CWndMapShadowGrp::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SCROLLINFO stScrollInfo;

	if (CWnd::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	GetScrollInfo (SB_HORZ, &stScrollInfo);
	stScrollInfo.nPage = 1;
	SetScrollInfo (SB_HORZ, &stScrollInfo);
	GetScrollInfo (SB_VERT, &stScrollInfo);
	stScrollInfo.nPage = 1;
	SetScrollInfo (SB_VERT, &stScrollInfo);

	MakeImage ();

	return 0;
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowGrp::OnPaint										 */
/* ���e		:���b�Z�[�W�n���h��(WM_PAINT)									 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CWndMapShadowGrp::OnPaint()
{
	int x, y, cx, cy;
	HDC hDC;
	CDC DCTmp;
	CRect rc, rcTmp;
	CRgn rgn;
	CPaintDC dc(this);

	hDC = m_pImgShadow->Lock ();
	DCTmp.Attach (hDC);

	GetClientRect (rc);
	rgn.CreateRectRgn (0, 0, rc.Width (), rc.Height ());
	dc.SelectClipRgn (&rgn, RGN_COPY);

	x = GetScrollPos (SB_HORZ);
	y = GetScrollPos (SB_VERT);

	cx = min (rc.Width (), m_pImgShadow->Width ());
	cy = min (rc.Height (), m_pImgShadow->Height ());
	dc.FillSolidRect (rc, RGB (128, 128, 128));
	dc.BitBlt (0, 0, cx, cy, &DCTmp, x * 16, y * 16, SRCCOPY);

	x = m_ptCursor.x * 16;
	y = (m_ptCursor.y - m_nPos) * 16;
	rcTmp.SetRect (x, y, x + 15, y + 15);

	dc.DrawEdge (rcTmp, EDGE_BUMP, BF_FLAT | BF_RECT);

	m_pImgShadow->Unlock ();
	DCTmp.Detach ();
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowGrp::OnSize										 */
/* ���e		:���b�Z�[�W�n���h��(WM_SIZE)									 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CWndMapShadowGrp::OnSize(UINT nType, int cx, int cy)
{
	int xx, yy;

	CWnd::OnSize(nType, cx, cy);

	xx = m_pImgShadow->Width ();
	if (xx > cx) {
		SetScrollRange (SB_HORZ, 0, (xx - cx) / 16);
	}
	yy = m_pImgShadow->Height ();
	if (yy > cy) {
		SetScrollRange (SB_VERT, 0, (yy - cy) / 16);
	}
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowGrp::OnMouseMove									 */
/* ���e		:���b�Z�[�W�n���h��(WM_MOUSEMOVE)								 */
/* ���t		:2007/04/25														 */
/* ========================================================================= */

void CWndMapShadowGrp::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint ptBack;
	CRect rc;

	ptBack		 = m_ptCursor;
	m_ptCursor.x = point.x / 16;
	m_ptCursor.y = point.y / 16 + m_nPos;

	if (ptBack != m_ptCursor) {
		InvalidateRect (NULL, FALSE);
	}
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowGrp::OnLButtonDown								 */
/* ���e		:���b�Z�[�W�n���h��(WM_LBUTTONDOWN)								 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CWndMapShadowGrp::OnLButtonDown(UINT nFlags, CPoint point)
{
	int x, y, nNo;

	x = point.x / 16 + GetScrollPos (SB_HORZ);
	y = point.y / 16 + GetScrollPos (SB_VERT);
	nNo = (y * 32) + x;

	m_pWndParent->PostMessage (WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_LBUTTONDOWN, nNo);
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowGrp::OnRButtonDown								 */
/* ���e		:���b�Z�[�W�n���h��(WM_RBUTTONDOWN)								 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CWndMapShadowGrp::OnRButtonDown(UINT nFlags, CPoint point)
{
	int x, y, nNo;

	x = point.x / 16 + GetScrollPos (SB_HORZ);
	y = point.y / 16 + GetScrollPos (SB_VERT);
	nNo = (y * 32) + x;

	m_pWndParent->PostMessage (WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_RBUTTONDOWN, nNo);
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowGrp::OnMouseWheel									 */
/* ���e		:���b�Z�[�W�n���h��(WM_MOUSEWHEEL)								 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

BOOL CWndMapShadowGrp::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int nTmp;

	nTmp = -1;
	if (zDelta < 0) {
		nTmp = 1;
	}
	nTmp += GetScrollPos (SB_VERT);
	nTmp = max (nTmp, 0);
	SetScrollPos (SB_VERT, nTmp);
	InvalidateRect (NULL);

	return FALSE;
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowGrp::OnHScroll									 */
/* ���e		:���b�Z�[�W�n���h��(WM_HSCROLL)									 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CWndMapShadowGrp::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMin, nMax, nPosNow;

	GetScrollRange (SB_HORZ, &nMin, &nMax);
	nPosNow = GetScrollPos (SB_HORZ);

	switch (nSBCode) {
	case SB_LEFT:			/* ���[�փX�N���[�� */
		nPos = nMin;
		break;
	case SB_RIGHT:			/* �E�[�փX�N���[�� */
		nPos = nMax;
		break;
	case SB_LINELEFT:		/* ���փX�N���[�� */
	case SB_PAGELEFT:		/* 1�y�[�W���փX�N���[�� */
		if (nPosNow <= 0) {
			return;
		}
		nPos = nPosNow - 1;
		break;
	case SB_LINERIGHT:		/* �E�փX�N���[�� */
	case SB_PAGERIGHT:		/* 1�y�[�W�E�փX�N���[�� */
		if (nPosNow >= nMax) {
			return;
		}
		nPos = nPosNow + 1;
		break;
	case SB_THUMBPOSITION:	/* ��Έʒu�փX�N���[�� */
	case SB_THUMBTRACK:		/* �h���b�O���ꂽ */
		break;
	default:
		return;
	}

	SetScrollPos (SB_HORZ, nPos);
	InvalidateRect (NULL);
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowGrp::OnVScroll									 */
/* ���e		:���b�Z�[�W�n���h��(WM_VSCROLL)									 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CWndMapShadowGrp::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMin, nMax, nPosNow;

	GetScrollRange (SB_VERT, &nMin, &nMax);
	nPosNow = GetScrollPos (SB_VERT);

	switch (nSBCode) {
	case SB_TOP:			/* 1�ԏ�܂ŃX�N���[�� */
		nPos = nMin;
		break;
	case SB_BOTTOM:			/* 1�ԉ��܂ŃX�N���[�� */
		nPos = nMax;
		break;
	case SB_LINEUP:			/* 1�s��փX�N���[�� */
	case SB_PAGEUP:			/* 1�y�[�W��փX�N���[�� */
		if (nPosNow <= 0) {
			return;
		}
		nPos = nPosNow - 1;
		break;
	case SB_LINEDOWN:		/* 1�s���փX�N���[�� */
	case SB_PAGEDOWN:		/* 1�y�[�W���փX�N���[�� */
		if (nPosNow >= nMax) {
			return;
		}
		nPos = nPosNow + 1;
		break;
	case SB_THUMBPOSITION:	/* ��Έʒu�փX�N���[�� */
	case SB_THUMBTRACK:		/* �h���b�O���ꂽ */
		break;
	default:
		return;
	}

	SetScrollPos (SB_VERT, nPos);
	InvalidateRect (NULL);
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowGrp::MakeImage									 */
/* ���e		:�摜���쐬														 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CWndMapShadowGrp::MakeImage(void)
{
	int i, nCount;
	CImg32 *pImg;

	if (m_pMgrData == NULL) {
		return;
	}

	nCount = m_pMgrGrpData->GetMapShadowCount () / 1024;
	pImg = m_pMgrGrpData->GetDibMapShadowTmp (0);

	m_pImgShadow->Destroy ();
	m_pImgShadow->Create (pImg->Width (), pImg->Height () * nCount);
	m_pImgShadow->FillRect (0, 0, m_pImgShadow->Width (), m_pImgShadow->Height (), RGB (128, 128, 128));

	for (i = 0; i < nCount; i ++) {
		pImg = m_pMgrGrpData->GetDibMapShadowTmp (i);
		m_pImgShadow->BltFrom256 (0, i * pImg->Height (), pImg->Width (), pImg->Height (), pImg, 0, 0);
	}
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowGrp::PostNcDestroy								 */
/* ���e		:�I������														 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CWndMapShadowGrp::PostNcDestroy()
{
	delete this;
}

/* Copyright(C)URARA-works 2007 */
