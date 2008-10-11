/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:StaticMapParts.cpp											 */
/* ���e			:�}�b�v�p�[�c�ꗗ�X�^�e�B�b�N�R���g���[���N���X �����t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/10													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMapParts.h"
#include "InfoMapParts.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrDraw.h"
#include "MgrGrpData.h"
#include "StaticMapParts.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* ========================================================================= */
/* �N���X�ݒ�																 */
/* ========================================================================= */

BEGIN_MESSAGE_MAP(CStaticMapParts, CStatic)
	//{{AFX_MSG_MAP(CStaticMapParts)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* �֐���	:CStaticMapParts::CStaticMapParts								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/05/10														 */
/* ========================================================================= */

CStaticMapParts::CStaticMapParts()
{
	m_nPos				= 0;
	m_pWndParent		= NULL;
	m_pMgrData			= NULL;
	m_pMgrGrpData		= NULL;
	m_pLibInfoMapParts	= NULL;

	m_pImgParts		= new CImg32;

	m_ptCursor.x	= m_ptCursor.y	= 0;
	m_ptMoveSrc.x	= m_ptMoveSrc.y	= 0;
	m_ptMoveDst.x	= m_ptMoveDst.y	= 0;
}


/* ========================================================================= */
/* �֐���	:CStaticMapParts::~CStaticMapParts								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/05/10														 */
/* ========================================================================= */

CStaticMapParts::~CStaticMapParts()
{
	SAFE_DELETE (m_pImgParts);
}


/* ========================================================================= */
/* �֐���	:CStaticMapParts::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2007/05/10														 */
/* ========================================================================= */

BOOL CStaticMapParts::Create(CWnd *pParent, CMgrData *pMgrData)
{
	m_pWndParent		= pParent;
	m_pMgrData			= pMgrData;
	m_pMgrGrpData		= m_pMgrData->GetMgrGrpData ();
	m_pLibInfoMapParts	= m_pMgrData->GetLibInfoMapParts ();

	MakePartsImage ();

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:CStaticMapParts::Renew											 */
/* ���e		:�p�[�c�ꗗ�摜���쐬���čĕ`��									 */
/* ���t		:2007/05/10														 */
/* ========================================================================= */

void CStaticMapParts::Renew(void)
{
	MakePartsImage ();
	InvalidateRect (NULL);
}


/* ========================================================================= */
/* �֐���	:CStaticMapParts::SetScrollPos									 */
/* ���e		:�c�X�N���[���ʒu��ݒ�											 */
/* ���t		:2007/05/10														 */
/* ========================================================================= */

void CStaticMapParts::SetScrollPos(int nPos)
{
	m_nPos = nPos;
	Renew ();
}


/* ========================================================================= */
/* �֐���	:CStaticMapParts::OnLButtonDown									 */
/* ���e		:���b�Z�[�W�n���h��(WM_LBUTTONDOWN)								 */
/* ���t		:2007/05/10														 */
/* ========================================================================= */

void CStaticMapParts::OnLButtonDown(CPoint point)
{
}


/* ========================================================================= */
/* �֐���	:CStaticMapParts::OnRButtonDown									 */
/* ���e		:���b�Z�[�W�n���h��(WM_RBUTTONDOWN)								 */
/* ���t		:2007/05/10														 */
/* ========================================================================= */

void CStaticMapParts::OnRButtonDown(CPoint point)
{
}


/* ========================================================================= */
/* �֐���	:CStaticMapParts::OnMouseWheel									 */
/* ���e		:���b�Z�[�W�n���h��(WM_MOUSEWHEEL)								 */
/* ���t		:2007/05/10														 */
/* ========================================================================= */

void CStaticMapParts::OnMouseWheel(short zDelta, CPoint pt)
{
}


/* ========================================================================= */
/* �֐���	:CStaticMapParts::OnMouseMove									 */
/* ���e		:���b�Z�[�W�n���h��(WM_MOUSEMOVE)								 */
/* ���t		:2007/04/25														 */
/* ========================================================================= */

void CStaticMapParts::OnMouseMove(CPoint point)
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
/* �֐���	:CStaticMapParts::OnCreate										 */
/* ���e		:���b�Z�[�W�n���h��(WM_CREATE)									 */
/* ���t		:2007/05/10														 */
/* ========================================================================= */

int CStaticMapParts::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	return 0;
}


/* ========================================================================= */
/* �֐���	:CStaticMapParts::OnPaint										 */
/* ���e		:���b�Z�[�W�n���h��(WM_PAINT)									 */
/* ���t		:2007/05/10														 */
/* ========================================================================= */

void CStaticMapParts::OnPaint()
{
	int x, y, cy;
	HDC hDC;
	CDC DCTmp;
	CRect rc, rcTmp;
	CRgn rgn;
	CPaintDC dc(this);

	hDC = m_pImgParts->Lock ();
	DCTmp.Attach (hDC);

	GetWindowRect (rc);
	rgn.CreateRectRgn (0, 0, rc.Width (), rc.Height ());
	dc.SelectClipRgn (&rgn, RGN_COPY);

	ScreenToClient (rc);
	cy = min (rc.Height (), m_pImgParts->Height ());
	dc.BitBlt (0, 0, m_pImgParts->Width (), cy, &DCTmp, 0, 0, SRCCOPY);

	x = m_ptCursor.x * 16;
	y = (m_ptCursor.y - m_nPos) * 16;
	cy = y + 15;
	cy = min (cy, rc.Height ());
	rcTmp.SetRect (x, y, x + 15, cy);

	dc.DrawEdge (rcTmp, EDGE_BUMP, BF_FLAT | BF_RECT);

	m_pImgParts->Unlock ();
	DCTmp.Detach ();
}


/* ========================================================================= */
/* �֐���	:CStaticMapParts::OnSize										 */
/* ���e		:���b�Z�[�W�n���h��(WM_SIZE)									 */
/* ���t		:2007/05/10														 */
/* ========================================================================= */

void CStaticMapParts::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	Renew ();
}


/* ========================================================================= */
/* �֐���	:CStaticMapParts::MakePartsImage								 */
/* ���e		:�p�[�c�ꗗ�摜���쐬											 */
/* ���t		:2007/04/25														 */
/* ========================================================================= */

void CStaticMapParts::MakePartsImage(void)
{
	int i, nCount, ny;
	PCInfoMapParts pInfoMapParts;
	PCMgrDraw pMgrDraw;
	CRect rc;

	if (m_pMgrData == NULL) {
		return;
	}
	pMgrDraw = m_pMgrData->GetMgrDraw ();

	GetWindowRect (rc);
	ny = rc.Height () / 16;

	m_pImgParts->Destroy ();
	m_pImgParts->Create (16 * 23, rc.Height ());
	m_pImgParts->FillRect (0, 0, m_pImgParts->Width (), m_pImgParts->Height (), RGB (128, 128, 128));

	nCount = m_pLibInfoMapParts->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (i);

		if (!((pInfoMapParts->m_ptViewPos.y >= m_nPos) && (pInfoMapParts->m_ptViewPos.y <= m_nPos + ny))) {
			continue;
		}
		pMgrDraw->DrawMapParts (
				m_pImgParts,
				pInfoMapParts->m_ptViewPos.x * 16,
				(pInfoMapParts->m_ptViewPos.y - m_nPos) * 16,
				pInfoMapParts->m_dwPartsID,
				2,
				TRUE,
				FALSE);
	}
}

/* Copyright(C)URARA-works 2007 */
