/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapPartsEdit.cpp									 */
/* 内容			:マップパーツ編集ダイアログクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/14													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoMapParts.h"
#include "WndMapPartsAnimeList.h"
#include "WndSelectMapPartsGrp.h"
#include "WndMapPartsGrp.h"
#include "Img32.h"
#include "MgrGrpData.h"
#include "MgrData.h"
#include "DlgAdminMapPartsEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラス設定																 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapPartsEdit)
	DDX_Control(pDX, IDC_SLIDER, m_Slider);
	DDX_Text(pDX, IDC_COUNT, m_strCount);
	DDX_Text(pDX, IDC_NOW, m_strNow);
	DDX_Text(pDX, IDC_LEVEL, m_nLevel);
	DDX_Text(pDX, IDC_VIEWTIME, m_nViewTime);
	DDX_Check(pDX, IDC_TYPE_BLOCK, m_bTypeBlock);
	DDX_Check(pDX, IDC_TYPE_PILEBACK, m_bTypePileBack);
	DDX_Check(pDX, IDC_TYPE_PILE, m_bTypePile);
	DDX_Check(pDX, IDC_FISHING, m_bFishing);
	DDX_Check(pDX, IDC_TYPE_DRAWLAST, m_bTypeDrawLast);
	DDX_Check(pDX, IDC_BLOCK_DOWN, m_bBlockDown);
	DDX_Check(pDX, IDC_BLOCK_LEFT, m_bBlockLeft);
	DDX_Check(pDX, IDC_BLOCK_RIGHT, m_bBlockRight);
	DDX_Check(pDX, IDC_BLOCK_UP, m_bBlockUp);
	DDX_CBIndex(pDX, IDC_GRPNO, m_nGrpNo);
	DDX_Control(pDX, IDC_GRPNO, m_cbGrpNo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapPartsEdit, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapPartsEdit)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_LEVEL, OnChangeLevel)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_VIEWTIME, OnChangeViewtime)
	ON_WM_GETMINMAXINFO()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_CBN_SELCHANGE(IDC_GRPNO, OnSelchangeGrpNo)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WNDCLOSE, OnWndClose)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::CDlgAdminMapPartsEdit					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/05/14														 */
/* ========================================================================= */

CDlgAdminMapPartsEdit::CDlgAdminMapPartsEdit(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapPartsEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapPartsEdit)
	m_strCount = _T("");
	m_strNow = _T("");
	m_nLevel = 0;
	m_nViewTime = 0;
	m_bTypeBlock = FALSE;
	m_bTypePileBack = FALSE;
	m_bTypePile = FALSE;
	m_bFishing = FALSE;
	m_bTypeDrawLast = FALSE;
	m_bBlockDown = FALSE;
	m_bBlockLeft = FALSE;
	m_bBlockRight = FALSE;
	m_bBlockUp = FALSE;
	m_nGrpNo = 0;
	//}}AFX_DATA_INIT

	m_nSelectType			= 0;
	m_nAnimeNo				= 0;
	m_wPartsBase			= 0;
	m_wPartsPile			= 0;
	m_dwTimeLastAnime		= 0;
	m_pMgrData				= NULL;
	m_pMgrGrpData			= NULL;
	m_pWndMapPartsAnimeList	= NULL;

	m_sizeWindow.cx = m_sizeWindow.cy = 0;

	m_pInfoMapParts			= new CInfoMapParts;
	m_pWndMapPartsAnimeList	= new CWndMapPartsAnimeList;
	m_pWndMapPartsGrp		= new CWndMapPartsGrp;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::~CDlgAdminMapPartsEdit					 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/05/14														 */
/* ========================================================================= */

CDlgAdminMapPartsEdit::~CDlgAdminMapPartsEdit()
{
	SAFE_DELETE (m_pInfoMapParts);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::DoModal									 */
/* 内容		:モーダルダイアログ表示											 */
/* 日付		:2007/05/14														 */
/* ========================================================================= */

int CDlgAdminMapPartsEdit::DoModal(CMgrData *pMgrData)
{
	m_pMgrData		= pMgrData;
	m_pMgrGrpData	= m_pMgrData->GetMgrGrpData ();

	return CDlgAdminBase::DoModal ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2007/05/14														 */
/* ========================================================================= */

BOOL CDlgAdminMapPartsEdit::OnInitDialog()
{
	int i, nCount;
	CString strTmp;
	CSize sizeTmp;
	CRect rc;

	CDlgAdminBase::OnInitDialog();

	m_pWndMapPartsGrp->Create (this, m_pMgrData, IDC_PARTS);
	m_pWndMapPartsAnimeList->Create (this, m_pMgrData, IDC_LIST, m_pInfoMapParts);

	GetWindowRect (rc);
	m_sizeWindow.cx = rc.Width ();
	m_sizeWindow.cy = rc.Height ();

	GetDlgItem (IDC_BASE)->		SetWindowPos (NULL, 0, 0, 16, 16, SWP_NOMOVE);
	GetDlgItem (IDC_PILE)->		SetWindowPos (NULL, 0, 0, 16, 16, SWP_NOMOVE);
	GetDlgItem (IDC_PREVIEW)->	SetWindowPos (NULL, 0, 0, 16, 16, SWP_NOMOVE);

	RegisterControl (IDC_LIST,	LH_CTRL_WIDTH);
	RegisterControl (IDOK,		LH_CTRL_X | LH_CTRL_Y);
	RegisterControl (IDCANCEL,	LH_CTRL_X | LH_CTRL_Y);
	RegisterControl (m_pWndMapPartsGrp->GetSafeHwnd (), LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	GetDlgItem (IDC_STOP)->EnableWindow (FALSE);
	CheckRadioButton (IDC_MOVE_UP, IDC_MOVE_NONE, IDC_MOVE_NONE);
	CheckRadioButton (IDC_GRP_PARTS, IDC_GRP_SHASOW, IDC_GRP_PARTS);

	/* Todo:未実装なので選択できないようにしておく */
	GetDlgItem (IDC_MOVE_UP)->		EnableWindow (FALSE);
	GetDlgItem (IDC_MOVE_DOWN)->	EnableWindow (FALSE);
	GetDlgItem (IDC_MOVE_LEFT)->	EnableWindow (FALSE);
	GetDlgItem (IDC_MOVE_RIGHT)->	EnableWindow (FALSE);

	nCount = m_pMgrData->GetMgrGrpData ()->GetMapPartsCount () / 1024;
	for (i = 0; i < nCount; i ++) {
		strTmp.Format ("画像 %d 枚目", i + 1);
		m_cbGrpNo.AddString (strTmp);
	}
	m_nGrpNo = m_pMgrData->GetLastSelectGrpNo ();
	m_nGrpNo = min (m_nGrpNo, nCount - 1);
	m_cbGrpNo.SetCurSel (m_nGrpNo);
	OnSelchangeGrpNo ();

	SetData ();

	sizeTmp = m_pMgrData->GetDlgMapPartsEdit ();
	SetWindowPos (NULL, 0, 0, sizeTmp.cx, sizeTmp.cy, SWP_NOZORDER | SWP_NOMOVE);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::OnPaint									 */
/* 内容		:メッセージハンドラ(WM_PAINT)									 */
/* 日付		:2007/05/14														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::OnPaint()
{
	int no;
	BYTE byLevel;
	HDC hDC;
	CDC *pDC, *pDCTmp;
	CImg32 ImgTmp, ImgPreview, *pImg;

	CPaintDC dc(this);

	ImgTmp.Create (32, 32);
	ImgTmp.SetColorKey (RGB (255, 0, 255));
	ImgPreview.Create (32, 32);

	/* 下地 */
	no = m_wPartsBase;
	pImg = m_pMgrGrpData->GetDibMapParts (no / 1024);
	if (pImg == NULL) {
		return;
	}
	ImgTmp.		BltFrom256 (0, 0, 16, 16, pImg, (no % 1024) % 32 * 16, (no % 1024) / 32 * 16);
	ImgPreview.	BltFrom256 (0, 0, 16, 16, pImg, (no % 1024) % 32 * 16, (no % 1024) / 32 * 16);
	hDC		= ImgTmp.Lock ();
	pDCTmp	= dc.FromHandle (hDC);
	pDC		= GetDlgItem (IDC_BASE)->GetDC ();
	pDC->StretchBlt (0, 0, 32, 32, pDCTmp, 0, 0, 16, 16, SRCCOPY);
	ImgTmp.Unlock ();

	/* 重ね合わせ */
	no = m_wPartsPile;
	pImg = m_pMgrGrpData->GetDibMapParts (no / 1024);
	if (pImg == NULL) {
		return;
	}
	ImgTmp.BltFrom256 (0, 0, 16, 16, pImg, (no % 1024) % 32 * 16, (no % 1024) / 32 * 16);
	if (m_wPartsPile > 0) {
		byLevel = (BYTE)m_nLevel;
		ImgPreview.BltAlphaFrom256 (0, 0, 16, 16, pImg, (no % 1024) % 32 * 16, (no % 1024) / 32 * 16, byLevel, TRUE);
	}
	hDC		= ImgTmp.Lock ();
	pDCTmp	= dc.FromHandle (hDC);
	pDC		= GetDlgItem (IDC_PILE)->GetDC ();
	pDC->StretchBlt (0, 0, 32, 32, pDCTmp, 0, 0, 16, 16, SRCCOPY);
	ImgTmp.Unlock ();

	/* プレビュー */
	hDC		= ImgPreview.Lock ();
	pDCTmp	= dc.FromHandle (hDC);
	pDC		= GetDlgItem (IDC_PREVIEW)->GetDC ();
	pDC->StretchBlt (0, 0, 32, 32, pDCTmp, 0, 0, 16, 16, SRCCOPY);
	ImgPreview.Unlock ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::OnWndClose								 */
/* 内容		:メッセージハンドラ(WM_WNDCLOSE)								 */
/* 日付		:2007/05/14														 */
/* ========================================================================= */

LRESULT CDlgAdminMapPartsEdit::OnWndClose(WPARAM wParam, LPARAM lParam)
{
	int nNo;
	PCInfoAnime pInfo;

	switch (wParam) {
	case WINDOWID_SELECTMAPPARTSGRP:	/* マップパーツ画像選択 */
		if (lParam >= 0) {
			if (m_nSelectType == 1) {
				m_wPartsBase = (WORD)lParam;

			} else {
				m_wPartsPile = (WORD)lParam;
			}
			if (m_pInfoMapParts->GetAnimeCount () > 0) {
				nNo = m_Slider.GetPos ();
				pInfo = m_pInfoMapParts->GetAnimePtr (nNo);
				pInfo->m_wGrpIDBase = m_wPartsBase;
				pInfo->m_wGrpIDPile = m_wPartsPile;

			} else {
				m_pInfoMapParts->m_wGrpIDBase = m_wPartsBase;
				m_pInfoMapParts->m_wGrpIDPile = m_wPartsPile;
			}
			InvalidateRect (NULL);
			m_pWndMapPartsAnimeList->Renew ();
		}
		break;
	}
	m_nSelectType = 0;

	return 0;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::OnAdminMsg								 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2007/05/18														 */
/* ========================================================================= */

LRESULT CDlgAdminMapPartsEdit::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:		/* 左クリック通知 */
		m_nSelectType = 1;
		PostMessage (WM_WNDCLOSE, WINDOWID_SELECTMAPPARTSGRP, lParam);
		break;
	case ADMINMSG_NOTIFYTYPE_RBUTTONDOWN:		/* 右クリック通知 */
		m_nSelectType = 0;
		PostMessage (WM_WNDCLOSE, WINDOWID_SELECTMAPPARTSGRP, lParam);
		break;
	case ADMINMSG_SELECT_ANIME:					/* アニメーションコマ選択 */
		m_nAnimeNo = lParam;
		m_Slider.SetPos (m_nAnimeNo);
		OnSelectAnime ();
		break;
	}

	return 0;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::OnHScroll								 */
/* 内容		:メッセージハンドラ(WM_HSCROLL)									 */
/* 日付		:2007/05/14														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	OnSelectAnime ();

	CDlgAdminBase::OnHScroll (nSBCode, nPos, pScrollBar);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::OnGetMinMaxInfo							 */
/* 内容		:メッセージハンドラ(WM_GETMINMAXINFO)							 */
/* 日付		:2007/05/15														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	if (m_sizeWindow.cx > 0) {
		lpMMI->ptMinTrackSize.x = m_sizeWindow.cx;
		lpMMI->ptMinTrackSize.y = m_sizeWindow.cy;
	}
	
	CDlgAdminBase::OnGetMinMaxInfo (lpMMI);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::OnMouseMove								 */
/* 内容		:メッセージハンドラ(WM_MOUSEMOVE)								 */
/* 日付		:2007/05/16														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	CDlgAdminBase::OnMouseMove(nFlags, point);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::OnLButtonDown							 */
/* 内容		:メッセージハンドラ(WM_LBUTTONUP)								 */
/* 日付		:2007/05/14														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bResult;
	CRect rc;

	ClientToScreen (&point);

	GetDlgItem (IDC_BASE)->GetWindowRect (rc);
	bResult = rc.PtInRect (point);
	if (bResult) {
		m_nSelectType = 1;
	}
	GetDlgItem (IDC_PILE)->GetWindowRect (rc);
	bResult = rc.PtInRect (point);
	if (bResult) {
		m_nSelectType = 2;
	}

	CDlgAdminBase::OnLButtonDown(nFlags, point);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::OnMouseWheel							 */
/* 内容		:メッセージハンドラ(WM_MOUSEWHEEL)								 */
/* 日付		:2007/05/16														 */
/* ========================================================================= */

BOOL CDlgAdminMapPartsEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return CDlgAdminBase::OnMouseWheel (nFlags, zDelta, pt);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::OnTimer									 */
/* 内容		:メッセージハンドラ(WM_TIMER)									 */
/* 日付		:2007/05/28														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::OnTimer(UINT nIDEvent)
{
	int nMin, nMax;
	DWORD dwTime;
	PCInfoAnime pInfo;

	m_Slider.GetRange (nMin, nMax);

	pInfo = m_pInfoMapParts->GetAnimePtr (m_nAnimeNo);
	if (pInfo == NULL) {
		return;
	}
	dwTime = timeGetTime () - m_dwTimeLastAnime;
	if (dwTime < (DWORD)(pInfo->m_byWait * 10)) {
		return;
	}
	m_dwTimeLastAnime = timeGetTime ();

	m_nAnimeNo ++;
	if (m_nAnimeNo > nMax) {
		m_nAnimeNo = 0;
	}

	m_Slider.SetPos (m_nAnimeNo);
	OnSelectAnime ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::OnChangeLevel							 */
/* 内容		:イベントハンドラ(ENM_CHANGE)									 */
/* 日付		:2007/05/14														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::OnChangeLevel()
{
	int nNo;
	PCInfoAnime pInfo;

	UpdateData ();
	InvalidateRect (NULL);

	if (m_pInfoMapParts->GetAnimeCount () > 0) {
		nNo					= m_Slider.GetPos ();
		pInfo				= m_pInfoMapParts->GetAnimePtr (nNo);
		pInfo->m_byLevel	= m_nLevel;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::OnChangeViewtime						 */
/* 内容		:イベントハンドラ(ENM_CHANGE)									 */
/* 日付		:2007/05/14														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::OnChangeViewtime()
{
	int nNo;
	PCInfoAnime pInfo;

	UpdateData ();
	InvalidateRect (NULL);

	if (m_pInfoMapParts->GetAnimeCount () > 0) {
		nNo				= m_Slider.GetPos ();
		pInfo			= m_pInfoMapParts->GetAnimePtr (nNo);
		pInfo->m_byWait	= m_nViewTime;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::OnAdd									 */
/* 内容		:ボタンハンドラ(追加)											 */
/* 日付		:2007/05/14														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::OnAdd()
{
	int nCount;

	m_pInfoMapParts->AddAnime ();
	nCount = m_pInfoMapParts->GetAnimeCount ();
	m_strCount.Format ("%d", nCount);
	m_Slider.SetRange (0, nCount - 1, TRUE);
	m_Slider.SetPos (nCount - 1);

	OnSelectAnime ();
	RenewButtonState ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::OnDel									 */
/* 内容		:ボタンハンドラ(削除)											 */
/* 日付		:2007/05/14														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::OnDel()
{
	int nTmp;

	if (m_pInfoMapParts->GetAnimeCount () > 0) {
		m_pInfoMapParts->DeleteAnime (m_Slider.GetPos ());
		m_strCount.Format ("%d", m_pInfoMapParts->GetAnimeCount ());
		m_Slider.SetRange (0, m_pInfoMapParts->GetAnimeCount () - 1, TRUE);
	}

	nTmp = m_Slider.GetPos () + 1;
	if (m_pInfoMapParts->GetAnimeCount () == 0) {
		nTmp = 0;
	}

	OnSelectAnime ();
	RenewButtonState ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::OnPlay									 */
/* 内容		:ボタンハンドラ(再生)											 */
/* 日付		:2007/05/28														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::OnPlay()
{
	GetDlgItem (IDC_PLAY)->EnableWindow (FALSE);
	GetDlgItem (IDC_STOP)->EnableWindow (TRUE);

	SetTimer (0, 10, NULL);

	m_nAnimeNo			= 0;
	m_dwTimeLastAnime	= timeGetTime ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::OnStop									 */
/* 内容		:ボタンハンドラ(停止)											 */
/* 日付		:2007/05/28														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::OnStop()
{
	GetDlgItem (IDC_PLAY)->EnableWindow (TRUE);
	GetDlgItem (IDC_STOP)->EnableWindow (FALSE);

	KillTimer (0);

	m_nAnimeNo			= 0;
	m_dwTimeLastAnime	= 0;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::OnSelchangeGrpNo						 */
/* 内容		:イベントハンドラ(CBN_SELCHANGE)								 */
/* 日付		:2008/10/11														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::OnSelchangeGrpNo()
{
	UpdateData ();

	m_pWndMapPartsGrp->SetNo (m_nGrpNo);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::OnOK									 */
/* 内容		:ボタンハンドラ(OK)												 */
/* 日付		:2007/05/14														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::OnOK()
{
	CRect rcTmp;
	SIZE sizeTmp;

	UpdateData ();

	GetWindowRect (rcTmp);
	sizeTmp.cx = rcTmp.Width ();
	sizeTmp.cy = rcTmp.Height ();
	m_pMgrData->SetDlgMapPartsEdit (sizeTmp);
	m_pMgrData->SetLastSelectGrpNo (m_nGrpNo);

	GetData ();
	CDlgAdminBase::OnOK();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::SetData									 */
/* 内容		:データを画面に反映												 */
/* 日付		:2007/05/26														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::SetData(void)
{
	PCInfoAnime pAnime;

	m_strCount.Format ("%d", m_pInfoMapParts->m_byAnimeCount);
	m_Slider.SetRange (0, m_pInfoMapParts->m_byAnimeCount - 1, TRUE);
	if (m_pInfoMapParts->m_byAnimeCount > 0) {
		pAnime = m_pInfoMapParts->GetAnimePtr (0);
		m_nLevel		= pAnime->m_byLevel;
		m_nViewTime		= pAnime->m_byWait;
		m_wPartsBase	= pAnime->m_wGrpIDBase;
		m_wPartsPile	= pAnime->m_wGrpIDPile;
		m_strNow.Format ("%d", 1);

	} else {
		m_nLevel		= m_pInfoMapParts->m_byLevel;
		m_nViewTime		= 0;
		m_wPartsBase	= m_pInfoMapParts->m_wGrpIDBase;
		m_wPartsPile	= m_pInfoMapParts->m_wGrpIDPile;
		m_strNow.Format ("%d", 0);
	}

	m_bBlockUp		= (m_pInfoMapParts->m_byBlockDirection & BIT_PARTSBLOCK_UP)		? TRUE : FALSE;
	m_bBlockDown	= (m_pInfoMapParts->m_byBlockDirection & BIT_PARTSBLOCK_DOWN)	? TRUE : FALSE;
	m_bBlockLeft	= (m_pInfoMapParts->m_byBlockDirection & BIT_PARTSBLOCK_LEFT)	? TRUE : FALSE;
	m_bBlockRight	= (m_pInfoMapParts->m_byBlockDirection & BIT_PARTSBLOCK_RIGHT)	? TRUE : FALSE;

	UpdateData (FALSE);
	SetPartsType (m_pInfoMapParts->m_dwPartsType);
	RenewButtonState ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::GetData									 */
/* 内容		:画面からデータを取得											 */
/* 日付		:2007/05/26														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::GetData(void)
{
	OnChangeViewtime ();

	m_pInfoMapParts->m_dwPartsType		= GetPartsType ();
	m_pInfoMapParts->m_byLevel			= m_nLevel;
	m_pInfoMapParts->m_byBlockDirection	= 0;
	m_pInfoMapParts->m_byBlockDirection |= ((m_bBlockUp)	? BIT_PARTSBLOCK_UP		: 0);
	m_pInfoMapParts->m_byBlockDirection |= ((m_bBlockDown)	? BIT_PARTSBLOCK_DOWN	: 0);
	m_pInfoMapParts->m_byBlockDirection |= ((m_bBlockLeft)	? BIT_PARTSBLOCK_LEFT	: 0);
	m_pInfoMapParts->m_byBlockDirection |= ((m_bBlockRight)	? BIT_PARTSBLOCK_RIGHT	: 0);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::SetPartsType							 */
/* 内容		:パーツ種別に応じてチェックボックスの状態を設定					 */
/* 日付		:2007/05/15														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::SetPartsType(DWORD dwType)
{
	m_bTypeBlock	= FALSE;
	m_bTypePile		= FALSE;
	m_bTypePileBack	= FALSE;
	m_bFishing		= FALSE;
	m_bTypeDrawLast	= FALSE;

	if (dwType & BIT_PARTSHIT_BLOCK) {			/* ぶつかる */
		m_bTypeBlock = TRUE;
	}
	if (dwType & BIT_PARTSHIT_PILE) {			/* 重なる */
		m_bTypePile = TRUE;
	}
	if (dwType & BIT_PARTSHIT_PILEBACK) {		/* 背景ごと重なる */
		m_bTypePileBack = TRUE;
	}
	if (dwType & BIT_PARTSHIT_FISHING) {		/* 釣り場 */
		m_bFishing = TRUE;
	}
	if (dwType & BIT_PARTSHIT_DRAWLAST) {		/* キャラの後に描画 */
		m_bTypeDrawLast = TRUE;
	}

	UpdateData (FALSE);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::GetPartsType							 */
/* 内容		:チェックボックスの状態に応じてパーツ種別を取得					 */
/* 日付		:2007/05/15														 */
/* ========================================================================= */

DWORD CDlgAdminMapPartsEdit::GetPartsType(void)
{
	DWORD dwRet;

	UpdateData ();

	dwRet = 0;

	if (m_bTypeBlock) {			/* ぶつかる */
		dwRet |= BIT_PARTSHIT_BLOCK;
	}
	if (m_bTypePile) {			/* 重なる */
		dwRet |= BIT_PARTSHIT_PILE;
	}
	if (m_bTypePileBack) {		/* 背景ごと重なる */
		dwRet |= BIT_PARTSHIT_PILEBACK;
	}
	if (m_bFishing) {			/* 釣り場 */
		dwRet |= BIT_PARTSHIT_FISHING;
	}
	if (m_bTypeDrawLast) {		/* キャラの後に描画 */
		dwRet |= BIT_PARTSHIT_DRAWLAST;
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::OnSelectAnime							 */
/* 内容		:コマ選択処理													 */
/* 日付		:2007/05/14														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::OnSelectAnime(void)
{
	int nNo;
	PCInfoAnime pInfo;

	nNo		= m_Slider.GetPos ();
	pInfo	= m_pInfoMapParts->GetAnimePtr (nNo);

	if (pInfo) {
		m_wPartsBase	= pInfo->m_wGrpIDBase;
		m_wPartsPile	= pInfo->m_wGrpIDPile;
		m_nLevel		= pInfo->m_byLevel;
		m_nViewTime		= pInfo->m_byWait;
	} else {
		nNo = -1;
	}

	m_strNow.Format ("%d", nNo + 1);
	UpdateData (FALSE);
	InvalidateRect (NULL);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::RenewButtonState						 */
/* 内容		:ボタン状態の設定												 */
/* 日付		:2007/05/14														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::RenewButtonState(void)
{
	BOOL bEnable;
	int nMin, nMax;

	bEnable = FALSE;
	nMin	= nMax = 0;

	m_Slider.GetRange (nMin, nMax);
	if (nMax >= 0) {
		bEnable = TRUE;
	}

	GetDlgItem (IDC_DEL)->		EnableWindow (bEnable);
	GetDlgItem (IDC_VIEWTIME)->	EnableWindow (bEnable);
	GetDlgItem (IDC_SLIDER)->	EnableWindow (bEnable);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapPartsEdit::PostNcDestroy							 */
/* 内容		:終了処理														 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminMapPartsEdit::PostNcDestroy()
{
}

/* Copyright(C)URARA-works 2007 */
