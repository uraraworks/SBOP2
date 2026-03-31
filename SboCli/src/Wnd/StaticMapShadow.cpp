/// @file StaticMapShadow.cpp
/// @brief マップ影一覧スタティックコントロールクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/06/06
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "LibInfoMapShadow.h"
#include "InfoMapShadow.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrDraw.h"
#include "MgrGrpData.h"
#include "StaticMapShadow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラス設定

BEGIN_MESSAGE_MAP(CStaticMapShadow, CStatic)
	//{{AFX_MSG_MAP(CStaticMapShadow)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CStaticMapShadow::CStaticMapShadow()
{
	m_nPos				= 0;
	m_pWndParent		= NULL;
	m_pMgrData			= NULL;
	m_pMgrGrpData		= NULL;
	m_pLibInfoMapShadow	= NULL;

	m_pImgShadow		= new CImg32;

	m_ptCursor.x	= m_ptCursor.y	= 0;
	m_ptMoveSrc.x	= m_ptMoveSrc.y	= 0;
	m_ptMoveDst.x	= m_ptMoveDst.y	= 0;
}

CStaticMapShadow::~CStaticMapShadow()
{
	SAFE_DELETE(m_pImgShadow);
}

BOOL CStaticMapShadow::Create(CWnd *pParent, CMgrData *pMgrData)
{
	m_pWndParent		= pParent;
	m_pMgrData			= pMgrData;
	m_pMgrGrpData		= m_pMgrData->GetMgrGrpData();
	m_pLibInfoMapShadow	= m_pMgrData->GetLibInfoMapShadow();

	MakeShadowImage();

	return TRUE;
}

void CStaticMapShadow::Renew(void)
{
	MakeShadowImage();
	InvalidateRect(NULL);
}

void CStaticMapShadow::SetScrollPos(int nPos)
{
	m_nPos = nPos;
	Renew();
}

void CStaticMapShadow::OnLButtonDown(CPoint point)
{
}

void CStaticMapShadow::OnRButtonDown(CPoint point)
{
}

void CStaticMapShadow::OnMouseWheel(short zDelta, CPoint pt)
{
}

void CStaticMapShadow::OnMouseMove(CPoint point)
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

int CStaticMapShadow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	return 0;
}

void CStaticMapShadow::OnPaint()
{
	int x, y, cy;
	HDC hDC;
	CDC DCTmp;
	CRect rc, rcTmp;
	CRgn rgn;
	CPaintDC dc(this);

	hDC = m_pImgShadow->Lock();
	DCTmp.Attach(hDC);

	GetWindowRect(rc);
	rgn.CreateRectRgn(0, 0, rc.Width(), rc.Height());
	dc.SelectClipRgn(&rgn, RGN_COPY);

	ScreenToClient(rc);
	cy = min(rc.Height(), m_pImgShadow->Height());
	dc.BitBlt(0, 0, m_pImgShadow->Width(), cy, &DCTmp, 0, 0, SRCCOPY);

	x = m_ptCursor.x * 16;
	y = (m_ptCursor.y - m_nPos) * 16;
	cy = y + 15;
	cy = min(cy, rc.Height());
	rcTmp.SetRect(x, y, x + 15, cy);

	dc.DrawEdge(rcTmp, EDGE_BUMP, BF_FLAT | BF_RECT);

	m_pImgShadow->Unlock();
	DCTmp.Detach();
}

void CStaticMapShadow::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	Renew();
}

void CStaticMapShadow::MakeShadowImage(void)
{
	int i, nCount, ny;
	PCInfoMapShadow pInfoMapShadow;
	PCMgrDraw pMgrDraw;
	CRect rc;

	if (m_pMgrData == NULL) {
		return;
	}
	pMgrDraw = m_pMgrData->GetMgrDraw();

	GetWindowRect(rc);
	ny = rc.Height() / 16;

	m_pImgShadow->Destroy();
	m_pImgShadow->Create(16 * 20, rc.Height());
	m_pImgShadow->FillRect(0, 0, m_pImgShadow->Width(), m_pImgShadow->Height(), RGB(128, 128, 128));

	nCount = m_pLibInfoMapShadow->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoMapShadow = (PCInfoMapShadow)m_pLibInfoMapShadow->GetPtr(i);

		if (!((pInfoMapShadow->m_ptViewPos.y >= m_nPos) && (pInfoMapShadow->m_ptViewPos.y <= m_nPos + ny))) {
			continue;
		}
		pMgrDraw->DrawMapShadow(
				m_pImgShadow,
				pInfoMapShadow->m_ptViewPos.x * 16,
				(pInfoMapShadow->m_ptViewPos.y - m_nPos) * 16,
				pInfoMapShadow->m_dwShadowID,
				TRUE,
				FALSE);
	}
}
