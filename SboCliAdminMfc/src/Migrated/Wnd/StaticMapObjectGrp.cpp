/// @file StaticMapObjectGrp.cpp
/// @brief マップオブジェクト表示スタティックコントロールクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/11/02
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "InfoMapObject.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrDraw.h"
#include "MgrGrpData.h"
#include "StaticMapObjectGrp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラス設定

BEGIN_MESSAGE_MAP(CStaticMapObjectGrp, CStatic)
	//{{AFX_MSG_MAP(CStaticMapObjectGrp)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CStaticMapObjectGrp::CStaticMapObjectGrp()
{
	m_nAnimeNo			= 0;
	m_pWndParent		= NULL;
	m_pMgrData			= NULL;
	m_pMgrGrpData		= NULL;
	m_pInfoMapObject	= NULL;

	m_pImgBack			= new CImg32;

	ZeroMemory(&m_ptSelect, sizeof(m_ptSelect));
}

CStaticMapObjectGrp::~CStaticMapObjectGrp()
{
	SAFE_DELETE(m_pImgBack);
}

BOOL CStaticMapObjectGrp::Create(CWnd *pParent, CMgrData *pMgrData)
{
	m_pWndParent	= pParent;
	m_pMgrData		= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData();

	return TRUE;
}

void CStaticMapObjectGrp::Init(CInfoMapObject *pInfoMapObject)
{
	m_pInfoMapObject = pInfoMapObject;
	Set(0);
}

void CStaticMapObjectGrp::Set(int nAnimeNo)
{
	m_nAnimeNo = nAnimeNo;
	RenewGrp();
}

void CStaticMapObjectGrp::IncPosX(void)
{
	m_ptSelect.x ++;
	if (m_ptSelect.x >= m_pInfoMapObject->m_sizeGrp.cx) {
		m_ptSelect.x = 0;
		m_ptSelect.y ++;
		if (m_ptSelect.y >= m_pInfoMapObject->m_sizeGrp.cy) {
			m_ptSelect.y = 0;
		}
	}
	RenewGrp();
}

int CStaticMapObjectGrp::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	m_pImgBack->Create(32, 32);

	return 0;
}

void CStaticMapObjectGrp::OnPaint()
{
	int x, y, cx, cy;
	HDC hDC;
	CDC *pDCTmp;
	CRect rcTmp;
	CPaintDC dc(this);

	cx = m_pImgBack->Width();
	cy = m_pImgBack->Height();

#ifdef _WIN32
	hDC = m_pImgBack->Lock();
		pDCTmp = dc.FromHandle(hDC);
		dc.BitBlt(0, 0, cx, cy, pDCTmp, 0, 0, SRCCOPY);
	m_pImgBack->Unlock();
#endif // _WIN32

	if ((m_pInfoMapObject->m_sizeGrp.cx > 1) || (m_pInfoMapObject->m_sizeGrp.cy > 1)) {
		// 選択位置矩形の描画
		x = m_ptSelect.x * 16;
		y = m_ptSelect.y * 16;
		rcTmp.SetRect(x, y, x + 15, y + 15);
		dc.DrawEdge(rcTmp, EDGE_BUMP, BF_FLAT | BF_RECT);
	}
}

void CStaticMapObjectGrp::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_ptSelect.x = point.x / 16;
	m_ptSelect.y = point.y / 16;

	InvalidateRect(NULL);
}

void CStaticMapObjectGrp::OnRButtonDown(UINT nFlags, CPoint point)
{
	int x, y;
	BYTE byHit;

	x = point.x / 16;
	y = point.y / 16;

	byHit = m_pInfoMapObject->m_pHit[m_pInfoMapObject->m_sizeGrp.cx * y + x];
	byHit = (byHit != 0) ? 0 : 1;
	m_pInfoMapObject->m_pHit[m_pInfoMapObject->m_sizeGrp.cx * y + x] = byHit;

	RenewGrp();
	InvalidateRect(NULL);
}

void CStaticMapObjectGrp::RenewGrp(void)
{
	BYTE byHit;
	int cx, cy, x, y;
	WORD wGrpID;
	PCImg32 pImg;
	PSTMAPOBJECTANIMEINFO pInfo;

	m_pImgBack->Destroy();

	cx = m_pInfoMapObject->m_sizeGrp.cx * 16;
	cy = m_pInfoMapObject->m_sizeGrp.cy * 16;
	m_ptSelect.x = min(m_ptSelect.x, m_pInfoMapObject->m_sizeGrp.cx - 1);
	m_ptSelect.y = min(m_ptSelect.y, m_pInfoMapObject->m_sizeGrp.cy - 1);

	m_pImgBack->Create(cx, cy);
	m_pImgBack->FillRect(0, 0, cx, cy, RGB(255, 0, 255));

	pInfo = m_pInfoMapObject->GetAnimePtr(m_nAnimeNo);
	if (pInfo) {
		for (y = 0; y < cy; y ++) {
			for (x = 0; x < cx; x ++) {
				wGrpID = pInfo->pwGrpID[y * m_pInfoMapObject->m_sizeGrp.cx + x];
				pImg = m_pMgrGrpData->GetDibMapParts(wGrpID / 1024);
				if (pImg) {
					m_pImgBack->BltFrom256(x * 16, y * 16, 16, 16, pImg, ((wGrpID % 1024) % 32) * 16, ((wGrpID % 1024) / 32) * 16);
				}

				byHit = m_pInfoMapObject->m_pHit[m_pInfoMapObject->m_sizeGrp.cx * y + x];
				if (byHit != 0) {
					m_pImgBack->ChgLevel(x * 16, y * 16, 16, 16, 50);
				}
			}
		}
	}

	SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);
	InvalidateRect(NULL);
}
