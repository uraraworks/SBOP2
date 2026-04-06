/// @file StaticGrp.cpp
/// @brief 画像表示スタティックコントロールクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/11/10
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrDraw.h"
#include "MgrGrpData.h"
#include "StaticGrp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラス設定

BEGIN_MESSAGE_MAP(CStaticGrp, CStatic)
	//{{AFX_MSG_MAP(CStaticGrp)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CStaticGrp::CStaticGrp()
{
	m_pWndParent	= NULL;
	m_pMgrData		= NULL;
	m_pMgrGrpData	= NULL;
	m_dwGrpIDMain	= 0;
	m_dwGrpIDSub	= 0;
	m_dwGrpIDParam	= 0;

	m_pImgBack		= new CImg32;
}

CStaticGrp::~CStaticGrp()
{
	SAFE_DELETE(m_pImgBack);
}

BOOL CStaticGrp::Create(CWnd *pParent, CMgrData *pMgrData)
{
	m_pWndParent	= pParent;
	m_pMgrData		= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData();

	return TRUE;
}

void CStaticGrp::Init(CImg32 *pSrc)
{
	int cx, cy;

	m_pImgBack->Destroy();
	cx = pSrc->Width();
	cy = pSrc->Height();
	m_pImgBack->Create(cx, cy);
	m_pImgBack->Blt(0, 0, cx, cy, pSrc, 0, 0);

	SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);
	InvalidateRect(NULL);
}

void CStaticGrp::Init(DWORD dwGrpIDMain)
{
	m_dwGrpIDMain = dwGrpIDMain;
	Set(0);
}

void CStaticGrp::Set(DWORD dwGrpIdSub)
{
	m_dwGrpIDSub = dwGrpIdSub;
	RenewGrp(m_dwGrpIDMain, m_dwGrpIDSub);
}

void CStaticGrp::SetParam(DWORD dwGrpIDParam)
{
	m_dwGrpIDParam = dwGrpIDParam;
	RenewGrp(m_dwGrpIDMain, m_dwGrpIDSub);
}

int CStaticGrp::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	m_pImgBack->Create(32, 32);

	return 0;
}

void CStaticGrp::OnPaint()
{
	int cx, cy;
	HDC hDC;
	CDC *pDCTmp;
	CPaintDC dc(this);

	cx = m_pImgBack->Width();
	cy = m_pImgBack->Height();

#ifdef _WIN32
	hDC = m_pImgBack->Lock();
		pDCTmp = dc.FromHandle(hDC);
		dc.BitBlt(0, 0, cx, cy, pDCTmp, 0, 0, SRCCOPY);
	m_pImgBack->Unlock();
#endif // _WIN32
}

void CStaticGrp::RenewGrp(DWORD dwGrpIDMain, DWORD dwGrpIDSub)
{
	PCImg32 pImgTmp;
	DWORD dwParam;
	int x, y, nSize, nCountX;

	m_pImgBack->Destroy();

	x = y = 0;
	pImgTmp = NULL;
	dwParam = 0;

	nSize	= m_pMgrGrpData->GetGrpSize(dwGrpIDMain);
	nCountX	= m_pMgrGrpData->GetGrpCountX(dwGrpIDMain);
	if (dwGrpIDSub == 0) {
		goto Exit;
	}

	switch (dwGrpIDMain) {
	case GRPIDMAIN_CHAR:		// キャラ画像
	case GRPIDMAIN_2X2_CHAR:	// キャラ(32x32)画像
		dwGrpIDSub --;
		dwParam = FAMILYTYPE_HUMAN;
		x = (dwGrpIDSub % nCountX) * nSize;
		y = (dwGrpIDSub / nCountX) * nSize;
		break;
	case GRPIDMAIN_ICON32:		// アイコン(２倍表示)
		dwParam = m_dwGrpIDParam;
		if (dwGrpIDSub == 0) {
			break;
		}
		x = (dwGrpIDSub % nCountX) * nSize;
		y = (dwGrpIDSub / nCountX) * nSize;
		nSize = 32;
		break;
	default:
		dwGrpIDSub --;
		dwParam = m_dwGrpIDParam;
		if (dwGrpIDSub == 0) {
			break;
		}
		x = (dwGrpIDSub % nCountX) * nSize;
		y = (dwGrpIDSub / nCountX) * nSize;
		break;
	}

	pImgTmp = m_pMgrGrpData->GetDib(dwGrpIDMain, dwGrpIDSub, dwParam);

Exit:
	m_pImgBack->Create(nSize, nSize);
	if (pImgTmp) {
		if (dwGrpIDMain == GRPIDMAIN_ICON32) {
			m_pMgrData->GetMgrDraw()->DrawIcon(m_pImgBack, 0, 0, dwGrpIDSub);
		} else {
			m_pImgBack->BltFrom256(0, 0, nSize, nSize, pImgTmp, x, y);
		}
	}
	SetWindowPos(NULL, 0, 0, nSize, nSize, SWP_NOZORDER | SWP_NOMOVE);
	InvalidateRect(NULL);
}
