/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WndMapPartsGrp.cpp											 */
/* ���e			:�}�b�v�p�[�c�ꗗ�X�^�e�B�b�N�R���g���[���N���X �����t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/15													 */
/* ========================================================================= */

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


/* ========================================================================= */
/* �N���X�ݒ�																 */
/* ========================================================================= */

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


/* ========================================================================= */
/* �֐���	:CWndMapPartsGrp::CWndMapPartsGrp								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/05/15														 */
/* ========================================================================= */

CWndMapPartsGrp::CWndMapPartsGrp()
{
	m_nPos				= 0;
	m_pWndParent		= NULL;
	m_pMgrData			= NULL;
	m_pMgrGrpData		= NULL;

	m_pImgParts		= new CImg32;

	m_ptCursor.x	= m_ptCursor.y	= 0;
//	m_ptMoveSrc.x	= m_ptMoveSrc.y	= 0;
//	m_ptMoveDst.x	= m_ptMoveDst.y	= 0;
}


/* ========================================================================= */
/* �֐���	:CWndMapPartsGrp::~CWndMapPartsGrp								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/05/15														 */
/* ========================================================================= */

CWndMapPartsGrp::~CWndMapPartsGrp()
{
	SAFE_DELETE (m_pImgParts);
}


/* ========================================================================= */
/* �֐���	:CWndMapPartsGrp::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2007/05/15														 */
/* ========================================================================= */

BOOL CWndMapPartsGrp::Create(CWnd *pParent, CMgrData *pMgrData, int nResourceID)
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
/* �֐���	:CWndMapPartsGrp::Destroy										 */
/* ���e		:�j��															 */
/* ���t		:2007/05/17														 */
/* ========================================================================= */

void CWndMapPartsGrp::Destroy(void)
{
	if (m_hWnd) {
		DestroyWindow ();
	}
}


/* ========================================================================= */
/* �֐���	:CWndMapPartsGrp::SetMode										 */
/* ���e		:�摜���[�h�̕ύX												 */
/* ���t		:2007/06/03														 */
/* ========================================================================= */

void CWndMapPartsGrp::SetMode(
	int nMode)		/* 0:�}�b�v�摜 1:�e�摜 */
{
	MakeImage (nMode);
	InvalidateRect (NULL);
}


/* ========================================================================= */
/* �֐���	:CWndMapPartsGrp::OnCreate										 */
/* ���e		:���b�Z�[�W�n���h��(WM_CREATE)									 */
/* ���t		:2007/05/15														 */
/* ========================================================================= */

int CWndMapPartsGrp::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

	MakeImage (0);

	return 0;
}


/* ========================================================================= */
/* �֐���	:CWndMapPartsGrp::OnPaint										 */
/* ���e		:���b�Z�[�W�n���h��(WM_PAINT)									 */
/* ���t		:2007/05/15														 */
/* ========================================================================= */

void CWndMapPartsGrp::OnPaint()
{
	int x, y, cx, cy;
	HDC hDC;
	CDC DCTmp;
	CRect rc, rcTmp;
	CRgn rgn;
	CPaintDC dc(this);

	hDC = m_pImgParts->Lock ();
	DCTmp.Attach (hDC);

	GetClientRect (rc);
	rgn.CreateRectRgn (0, 0, rc.Width (), rc.Height ());
	dc.SelectClipRgn (&rgn, RGN_COPY);

	x = GetScrollPos (SB_HORZ);
	y = GetScrollPos (SB_VERT);

	cx = min (rc.Width (), m_pImgParts->Width ());
	cy = min (rc.Height (), m_pImgParts->Height ());
	dc.FillSolidRect (rc, RGB (128, 128, 128));
	dc.BitBlt (0, 0, cx, cy, &DCTmp, x * 16, y * 16, SRCCOPY);

	x = m_ptCursor.x * 16;
	y = (m_ptCursor.y - m_nPos) * 16;
	rcTmp.SetRect (x, y, x + 15, y + 15);

	dc.DrawEdge (rcTmp, EDGE_BUMP, BF_FLAT | BF_RECT);

	m_pImgParts->Unlock ();
	DCTmp.Detach ();
}


/* ========================================================================= */
/* �֐���	:CWndMapPartsGrp::OnSize										 */
/* ���e		:���b�Z�[�W�n���h��(WM_SIZE)									 */
/* ���t		:2007/05/15														 */
/* ========================================================================= */

void CWndMapPartsGrp::OnSize(UINT nType, int cx, int cy)
{
	int xx, yy;

	CWnd::OnSize(nType, cx, cy);

	xx = m_pImgParts->Width ();
	if (xx > cx) {
		SetScrollRange (SB_HORZ, 0, (xx - cx) / 16);
	}
	yy = m_pImgParts->Height ();
	if (yy > cy) {
		SetScrollRange (SB_VERT, 0, (yy - cy) / 16);
	}
}


/* ========================================================================= */
/* �֐���	:CWndMapPartsGrp::OnMouseMove									 */
/* ���e		:���b�Z�[�W�n���h��(WM_MOUSEMOVE)								 */
/* ���t		:2007/04/25														 */
/* ========================================================================= */

void CWndMapPartsGrp::OnMouseMove(UINT nFlags, CPoint point)
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
/* �֐���	:CWndMapPartsGrp::OnLButtonDown									 */
/* ���e		:���b�Z�[�W�n���h��(WM_LBUTTONDOWN)								 */
/* ���t		:2007/05/15														 */
/* ========================================================================= */

void CWndMapPartsGrp::OnLButtonDown(UINT nFlags, CPoint point)
{
	int x, y, nNo;

	x = point.x / 16 + GetScrollPos (SB_HORZ);
	y = point.y / 16 + GetScrollPos (SB_VERT);
	nNo = (y * 32) + x;

	m_pWndParent->PostMessage (WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_LBUTTONDOWN, nNo);
}


/* ========================================================================= */
/* �֐���	:CWndMapPartsGrp::OnRButtonDown									 */
/* ���e		:���b�Z�[�W�n���h��(WM_RBUTTONDOWN)								 */
/* ���t		:2007/05/15														 */
/* ========================================================================= */

void CWndMapPartsGrp::OnRButtonDown(UINT nFlags, CPoint point)
{
	int x, y, nNo;

	x = point.x / 16 + GetScrollPos (SB_HORZ);
	y = point.y / 16 + GetScrollPos (SB_VERT);
	nNo = (y * 32) + x;

	m_pWndParent->PostMessage (WM_ADMINMSG, ADMINMSG_NOTIFYTYPE_RBUTTONDOWN, nNo);
}


/* ========================================================================= */
/* �֐���	:CWndMapPartsGrp::OnMouseWheel									 */
/* ���e		:���b�Z�[�W�n���h��(WM_MOUSEWHEEL)								 */
/* ���t		:2007/05/15														 */
/* ========================================================================= */

BOOL CWndMapPartsGrp::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
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
/* �֐���	:CWndMapPartsGrp::OnHScroll										 */
/* ���e		:���b�Z�[�W�n���h��(WM_HSCROLL)									 */
/* ���t		:2007/05/16														 */
/* ========================================================================= */

void CWndMapPartsGrp::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
/* �֐���	:CWndMapPartsGrp::OnVScroll										 */
/* ���e		:���b�Z�[�W�n���h��(WM_VSCROLL)									 */
/* ���t		:2007/05/16														 */
/* ========================================================================= */

void CWndMapPartsGrp::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
/* �֐���	:CWndMapPartsGrp::MakeImage										 */
/* ���e		:�摜���쐬														 */
/* ���t		:2007/04/25														 */
/* ========================================================================= */

void CWndMapPartsGrp::MakeImage(int nMode)
{
	int i, nCount;
	CImg32 *pImg;

	if (m_pMgrData == NULL) {
		return;
	}

	if (nMode == 1) {
		nCount = m_pMgrGrpData->GetMapShadowCount () / 1024;
		pImg = m_pMgrGrpData->GetDibMapShadowTmp (0);
	} else {
		nCount = m_pMgrGrpData->GetMapPartsCount () / 1024;
		pImg = m_pMgrGrpData->GetDibMapPartsTmp (0);
	}

	m_pImgParts->Destroy ();
	m_pImgParts->Create (pImg->Width (), pImg->Height () * nCount);
	m_pImgParts->FillRect (0, 0, m_pImgParts->Width (), m_pImgParts->Height (), RGB (128, 128, 128));

	for (i = 0; i < nCount; i ++) {
		pImg = m_pMgrGrpData->GetDibMapPartsTmp (i);
		m_pImgParts->BltFrom256 (0, i * pImg->Height (), pImg->Width (), pImg->Height (), pImg, 0, 0);
	}
}


/* ========================================================================= */
/* �֐���	:CWndMapPartsGrp::PostNcDestroy									 */
/* ���e		:�I������														 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

void CWndMapPartsGrp::PostNcDestroy()
{
	delete this;
}

/* Copyright(C)URARA-works 2007 */
