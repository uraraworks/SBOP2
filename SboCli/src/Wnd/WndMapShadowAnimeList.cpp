/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:WndMapShadowAnimeList.cpp									 */
/* 内容			:マップ影アニメーション一覧スタティックコントロールクラス 実装ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/06													 */
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
/* クラス設定																 */
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
/* 関数名	:CWndMapShadowAnimeList::CWndMapShadowAnimeList					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/06/06														 */
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
/* 関数名	:CWndMapShadowAnimeList::~CWndMapShadowAnimeList				 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

CWndMapShadowAnimeList::~CWndMapShadowAnimeList()
{
	SAFE_DELETE (m_pImgShadow);
}


/* ========================================================================= */
/* 関数名	:CWndMapShadowAnimeList::Create									 */
/* 内容		:作成															 */
/* 日付		:2007/06/06														 */
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
                        _T(""),
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
/* 関数名	:CWndMapShadowAnimeList::Destroy								 */
/* 内容		:破棄															 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

void CWndMapShadowAnimeList::Destroy(void)
{
	if (m_hWnd) {
		DestroyWindow ();
	}
}


/* ========================================================================= */
/* 関数名	:CWndMapShadowAnimeList::Renew									 */
/* 内容		:更新															 */
/* 日付		:2007/09/08														 */
/* ========================================================================= */

void CWndMapShadowAnimeList::Renew(void)
{
	MakeImage ();
	InvalidateRect (NULL);
}


/* ========================================================================= */
/* 関数名	:CWndMapShadowAnimeList::OnCreate								 */
/* 内容		:メッセージハンドラ(WM_CREATE)									 */
/* 日付		:2007/06/06														 */
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
/* 関数名	:CWndMapShadowAnimeList::OnPaint								 */
/* 内容		:メッセージハンドラ(WM_PAINT)									 */
/* 日付		:2007/06/06														 */
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
/* 関数名	:CWndMapShadowAnimeList::OnSize									 */
/* 内容		:メッセージハンドラ(WM_SIZE)									 */
/* 日付		:2007/06/06														 */
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
/* 関数名	:CWndMapShadowAnimeList::OnLButtonDown							 */
/* 内容		:メッセージハンドラ(WM_LBUTTONDOWN)								 */
/* 日付		:2007/06/06														 */
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
/* 関数名	:CWndMapShadowAnimeList::OnHScroll								 */
/* 内容		:メッセージハンドラ(WM_HSCROLL)									 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

void CWndMapShadowAnimeList::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMin, nMax, nPosNow;

	GetScrollRange (SB_HORZ, &nMin, &nMax);
	nPosNow = GetScrollPos (SB_HORZ);

	switch (nSBCode) {
	case SB_LEFT:			/* 左端へスクロール */
		nPos = nMin;
		break;
	case SB_RIGHT:			/* 右端へスクロール */
		nPos = nMax;
		break;
	case SB_LINELEFT:		/* 左へスクロール */
	case SB_PAGELEFT:		/* 1ページ左へスクロール */
		if (nPosNow <= 0) {
			return;
		}
		nPos = nPosNow - 1;
		break;
	case SB_LINERIGHT:		/* 右へスクロール */
	case SB_PAGERIGHT:		/* 1ページ右へスクロール */
		if (nPosNow >= nMax) {
			return;
		}
		nPos = nPosNow + 1;
		break;
	case SB_THUMBPOSITION:	/* 絶対位置へスクロール */
	case SB_THUMBTRACK:		/* ドラッグされた */
		break;
	default:
		return;
	}

	SetScrollPos (SB_HORZ, nPos);
	InvalidateRect (NULL);
}


/* ========================================================================= */
/* 関数名	:CWndMapShadowAnimeList::MakeImage								 */
/* 内容		:画像を作成														 */
/* 日付		:2007/06/06														 */
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
//Todo:透過
		m_pImgShadow->BltFrom256 (i * 16, 0, 16, 16, pImg, pInfoAnime->m_wGrpIDBase % 32 * 16, pInfoAnime->m_wGrpIDBase / 32 * 16);
	}
}


/* ========================================================================= */
/* 関数名	:CWndMapShadowAnimeList::PostNcDestroy							 */
/* 内容		:終了処理														 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

void CWndMapShadowAnimeList::PostNcDestroy()
{
	delete this;
}

/* Copyright(C)URARA-works 2007 */
