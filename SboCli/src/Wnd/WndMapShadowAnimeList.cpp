/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:WndMapShadowAnimeList.cpp									 */
/* ���e			:�}�b�v�e�A�j���[�V�����ꗗ�X�^�e�B�b�N�R���g���[���N���X �����t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/06													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMapShadow.h"
#include "InfoMapShadow.h"
#include "InfoAnime.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrDraw.h"
#include "MgrGrpData.h"
#include "WndMapShadowAnimeList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* ========================================================================= */
/* �N���X�ݒ�																 */
/* ========================================================================= */

BEGIN_MESSAGE_MAP(CWndMapShadowAnimeList, CWnd)
	//{{AFX_MSG_MAP(CWndMapShadowAnimeList)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CWndMapShadowAnimeList::CWndMapShadowAnimeList					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

CWndMapShadowAnimeList::CWndMapShadowAnimeList()
{
	m_pWndParent	= NULL;
	m_pMgrData		= NULL;
	m_pMgrGrpData	= NULL;

	m_pImgShadow		= new CImg32;
	m_pInfoMapShadow	= NULL;
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowAnimeList::~CWndMapShadowAnimeList				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

CWndMapShadowAnimeList::~CWndMapShadowAnimeList()
{
	SAFE_DELETE (m_pImgShadow);
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowAnimeList::Create									 */
/* ���e		:�쐬															 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

BOOL CWndMapShadowAnimeList::Create(CWnd *pParent, CMgrData *pMgrData, int nResourceID, CInfoMapShadow *pInfoMapShadow)
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
	m_pInfoMapShadow	= pInfoMapShadow;

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

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowAnimeList::Destroy								 */
/* ���e		:�j��															 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CWndMapShadowAnimeList::Destroy(void)
{
	if (m_hWnd) {
		DestroyWindow ();
	}
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowAnimeList::Renew									 */
/* ���e		:�X�V															 */
/* ���t		:2007/09/08														 */
/* ========================================================================= */

void CWndMapShadowAnimeList::Renew(void)
{
	MakeImage ();
	InvalidateRect (NULL);
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowAnimeList::OnCreate								 */
/* ���e		:���b�Z�[�W�n���h��(WM_CREATE)									 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

int CWndMapShadowAnimeList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
//	SCROLLINFO stScrollInfo;
	CRect rc;

	if (CWnd::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}
#if 0
	GetScrollInfo (SB_HORZ, &stScrollInfo);
	stScrollInfo.nPage = 1;
	SetScrollInfo (SB_HORZ, &stScrollInfo);
#endif

	GetWindowRect (rc);
	rc.bottom = rc.top + 15;
	SetWindowPos (NULL, 0, 0, rc.Width (), rc.Height (), SWP_NOZORDER | SWP_NOMOVE);

	MakeImage ();

	return 0;
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowAnimeList::OnPaint								 */
/* ���e		:���b�Z�[�W�n���h��(WM_PAINT)									 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CWndMapShadowAnimeList::OnPaint()
{
	int x, cx, cy;
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

	cx = min (rc.Width (), m_pImgShadow->Width ());
	cy = min (rc.Height (), m_pImgShadow->Height ());
	dc.FillSolidRect (rc, RGB (128, 128, 128));
	dc.BitBlt (0, 0, cx, cy, &DCTmp, x * 16, 0, SRCCOPY);

	m_pImgShadow->Unlock ();
	DCTmp.Detach ();
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowAnimeList::OnSize									 */
/* ���e		:���b�Z�[�W�n���h��(WM_SIZE)									 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CWndMapShadowAnimeList::OnSize(UINT nType, int cx, int cy)
{
	int xx;

	CWnd::OnSize(nType, cx, cy);

	xx = m_pImgShadow->Width ();
	if (xx > cx) {
		SetScrollRange (SB_HORZ, 0, (xx - cx) / 16);
	}
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowAnimeList::OnLButtonDown							 */
/* ���e		:���b�Z�[�W�n���h��(WM_LBUTTONDOWN)								 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CWndMapShadowAnimeList::OnLButtonDown(UINT nFlags, CPoint point)
{
	int x, y, nNo;

	x = point.x / 16 + GetScrollPos (SB_HORZ);
	y = point.y / 16 + GetScrollPos (SB_VERT);
	nNo = (y * 32) + x;

	m_pWndParent->PostMessage (WM_ADMINMSG, ADMINMSG_SELECT_ANIME, nNo);
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowAnimeList::OnHScroll								 */
/* ���e		:���b�Z�[�W�n���h��(WM_HSCROLL)									 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CWndMapShadowAnimeList::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
/* �֐���	:CWndMapShadowAnimeList::MakeImage								 */
/* ���e		:�摜���쐬														 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CWndMapShadowAnimeList::MakeImage(void)
{
	int i, nCount;
	CImg32 *pImg;
	PCInfoAnime pInfoAnime;

	if (m_pMgrData == NULL) {
		return;
	}

	nCount = m_pInfoMapShadow->GetAnimeCount ();

	m_pImgShadow->Destroy ();
	m_pImgShadow->Create (nCount * 16, 16);
	m_pImgShadow->FillRect (0, 0, m_pImgShadow->Width (), m_pImgShadow->Height (), RGB (128, 128, 128));

	for (i = 0; i < nCount; i ++) {
		pInfoAnime	= m_pInfoMapShadow->GetAnimePtr (i);
		pImg		= m_pMgrGrpData->GetDibMapShadowTmp (pInfoAnime->m_wGrpIDBase / 1024);
//Todo:����
		m_pImgShadow->BltFrom256 (i * 16, 0, 16, 16, pImg, pInfoAnime->m_wGrpIDBase % 32 * 16, pInfoAnime->m_wGrpIDBase / 32 * 16);
	}
}


/* ========================================================================= */
/* �֐���	:CWndMapShadowAnimeList::PostNcDestroy							 */
/* ���e		:�I������														 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CWndMapShadowAnimeList::PostNcDestroy()
{
	delete this;
}

/* Copyright(C)URARA-works 2007 */
