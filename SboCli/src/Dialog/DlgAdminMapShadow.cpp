/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapShadow.cpp										 */
/* 内容			:マップ影編集ダイアログクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/06/06													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "LibInfoMapShadow.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_RENEWMAPSHADOW.h"
#include "PacketADMIN_MAP_DELETEPARTS.h"
#include "PacketADMIN_MAP_DELETEMAPSHADOW.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrDraw.h"
#include "MgrGrpData.h"
#include "DlgAdminMapShadowEdit.h"
#include "DlgAdminMapShadow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminMapShadow::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapShadow)
	DDX_Control(pDX, IDC_MAPSHADOW, m_ctlMapShadow);
	DDX_Control(pDX, IDC_SCROLL, m_ctlScroll);
	DDX_Control(pDX, IDC_TYPE, m_cbType);
	DDX_CBIndex(pDX, IDC_TYPE, m_nTypeSelect);
	DDX_Text(pDX, IDC_ID, m_strID);
	DDX_Text(pDX, IDC_MSG, m_strMsg);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapShadow, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapShadow)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WNDCLOSE, OnWndClose)
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminMapShadow::CDlgAdminMapShadow							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

CDlgAdminMapShadow::CDlgAdminMapShadow(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapShadow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapShadow)
	m_nTypeSelect = 0;
	m_strID = _T("");
	m_strMsg = _T("");
	//}}AFX_DATA_INIT

	m_nSelectType		= 0;
	m_nState			= 0;
	m_dwSelectShadowID	= 0;

	m_pMgrGrpData		= NULL;
	m_pImgShadow		= new CImg32;
	m_pLibInfoMapShadow	= NULL;
	m_pSock				= NULL;

	m_ptMoveSrc.x = m_ptMoveSrc.y = 0;
	m_ptMoveDst.x = m_ptMoveDst.y = 0;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapShadow::~CDlgAdminMapShadow						 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

CDlgAdminMapShadow::~CDlgAdminMapShadow()
{
	SAFE_DELETE (m_pImgShadow);
	if (m_pMgrGrpData) {
		m_pMgrGrpData->DeleteMapShadowTmp ();
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapShadow::Init										 */
/* 内容		:初期化															 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadow::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pMgrGrpData		= m_pMgrData->GetMgrGrpData ();
	m_pLibInfoMapShadow	= m_pMgrData->GetLibInfoMapShadow ();
	m_pSock				= m_pMgrData->GetUraraSockTCP ();

	m_pMgrGrpData->ReadMapShadowTmp ();

	/* ウィンドウ作成 */
	Create (CDlgAdminMapShadow::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapShadow::OnAdminMsg									 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadow::OnAdminMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case ADMINMSG_RENEWMAPSHADOW:	/* マップ影情報更新 */
		MakeShadowImage ();
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapShadow::OnInitDialog								 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

BOOL CDlgAdminMapShadow::OnInitDialog()
{
	CWnd *pWnd;
	CRect rc, rcTmp;
	SCROLLINFO stScrollInfo;

	CDlgAdminBase::OnInitDialog();

	m_ctlMapShadow.Create (this, m_pMgrData);

	m_cbType.AddString ("マップ編集用に選択");
	m_cbType.AddString ("影の追加");
	m_cbType.AddString ("影の編集");
	m_cbType.AddString ("影の移動");
	m_cbType.AddString ("影の削除");
	m_cbType.SetCurSel (0);

	pWnd = GetDlgItem (IDC_MAPSHADOW);
	pWnd->GetWindowRect (rc);
	rc.right = rc.left + 32 * 10;
	rc.bottom = rc.top + 32 * 8;
	pWnd->SetWindowPos (NULL, 0, 0, rc.Width (), rc.Height (), SWP_NOZORDER | SWP_NOMOVE);

	/* スクロールバーの設定 */
	ScreenToClient (rc);
	m_ctlScroll.GetWindowRect (rcTmp);
	m_ctlScroll.SetWindowPos (NULL, rc.right, rc.top, rcTmp.Width (), rc.Height (), SWP_NOZORDER);
	m_ctlScroll.GetScrollInfo (&stScrollInfo);
	stScrollInfo.nPage	= 1;
	stScrollInfo.nMax	= 100;
	m_ctlScroll.SetScrollInfo (&stScrollInfo);

	RegisterControl (IDC_MAPSHADOW,	LH_CTRL_HEIGHT);
	RegisterControl (IDC_SCROLL,	LH_CTRL_HEIGHT);

	MakeShadowImage ();
	pWnd->SetFocus ();

	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapShadow::OnSize										 */
/* 内容		:メッセージハンドラ(WM_SIZE)									 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadow::OnSize(UINT nType, int cx, int cy)
{
	HWND hWnd;

	CDlgAdminBase::OnSize (nType, cx, cy);

	hWnd = m_ctlScroll.GetSafeHwnd ();
	if (hWnd == NULL) {
		return;
	}

	MakeShadowImage ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapShadow::OnPaint									 */
/* 内容		:メッセージハンドラ(WM_PAINT)									 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadow::OnPaint()
{
	int x, y, cy, nPos;
	HDC hDC;
	CWnd *pWnd;
	CDC DCTmp, *pDC;
	CRect rc, rcTmp;
	CRgn rgn;
	CPaintDC dc(this);

	pWnd = GetDlgItem (IDC_MAPSHADOW);
	nPos = m_ctlScroll.GetScrollPos ();

	hDC = m_pImgShadow->Lock ();
	DCTmp.Attach (hDC);
	pDC = pWnd->GetDC ();

	pWnd->GetWindowRect (rc);
	rgn.CreateRectRgn (0, 0, rc.Width (), rc.Height ());
	pDC->SelectClipRgn (&rgn, RGN_COPY);

	ScreenToClient (rc);
	cy = min (rc.Height (), m_pImgShadow->Height ());
	pDC->BitBlt (0, 0, m_pImgShadow->Width (), cy, &DCTmp, 0, 0, SRCCOPY);

	x = m_ptCursor.x * 16;
	y = (m_ptCursor.y - nPos) * 16;
	cy = y + 31;
	cy = min (cy, rc.Height ());
	rcTmp.SetRect (x, y, x + 15, cy);

	pDC->DrawEdge (rcTmp, EDGE_BUMP, BF_FLAT | BF_RECT);

	m_pImgShadow->Unlock ();
	DCTmp.Detach ();
	pWnd->ReleaseDC (pDC);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapShadow::OnMouseMove								 */
/* 内容		:メッセージハンドラ(WM_MOUSEMOVE)								 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadow::OnMouseMove(UINT nFlags, CPoint point)
{
	int nPos;
	BOOL bResult;
	CPoint ptBack;
	CRect rc;

	CDlgAdminBase::OnMouseMove(nFlags, point);

	GetDlgItem (IDC_MAPSHADOW)->GetWindowRect (rc);
	ScreenToClient (rc);

	bResult = rc.PtInRect (point);
	if (bResult == FALSE) {
		return;
	}
	nPos		 = m_ctlScroll.GetScrollPos ();
	point.x		-= rc.left;
	point.y		-= rc.top;
	ptBack		 = m_ptCursor;
	m_ptCursor.x = point.x / 16;
	m_ptCursor.y = point.y / 16 + nPos;

	m_ctlMapShadow.OnMouseMove (point);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapShadow::OnLButtonDown								 */
/* 内容		:メッセージハンドラ(WM_LBUTTONDOWN)								 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadow::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bResult;
	DWORD dwTmp;
	CRect rc;
	PCInfoMapShadow pInfoMapShadow;

	GetDlgItem (IDC_MAPSHADOW)->GetWindowRect (rc);
	ScreenToClient (rc);

	bResult = rc.PtInRect (point);
	if (bResult == FALSE) {
		return;
	}

	GetDlgItem (IDC_MAPSHADOW)->SetFocus ();

	switch (m_nSelectType) {
	case 0:		/* マップ編集用に選択 */
	case 2:		/* 編集 */
		m_dwSelectShadowID = GetSelectMapShadowID ();
		m_pMgrData->SetSelectMapShadowID (m_dwSelectShadowID);
		m_strID.Format ("ID:%u", m_dwSelectShadowID);
		UpdateData (FALSE);
		break;
	case 1:		/* 追加 */
		m_dwSelectShadowID = GetSelectMapShadowID ();
		if (m_dwSelectShadowID) {
			RenewMessage ("そこには追加できません");
			break;
		}
		pInfoMapShadow = (PCInfoMapShadow)m_pLibInfoMapShadow->GetNew ();
		pInfoMapShadow->m_ptViewPos = m_ptCursor;
		m_pLibInfoMapShadow->Add (pInfoMapShadow);
		MakeShadowImage ();
		break;
	case 3:		/* 移動 */
		{
			CPacketADMIN_RENEWMAPSHADOW Packet;

			switch (m_nState) {
			case 0:			/* 移動元の選択 */
				m_dwSelectShadowID = GetSelectMapShadowID ();
				if (m_dwSelectShadowID == 0) {
					break;
				}
				m_ptMoveSrc	= m_ptCursor;
				m_nState	= 1;
				RenewMessage ("移動先をクリックしてください");
				break;
			case 1:			/* 移動先の選択 */
				m_ptMoveDst = m_ptCursor;

				dwTmp = GetSelectMapShadowID ();
				pInfoMapShadow = (PCInfoMapShadow)m_pLibInfoMapShadow->GetPtr (&m_ptMoveDst);
				if (pInfoMapShadow) {
					/* 移動先に影があった場合は入れ換える */
					pInfoMapShadow->m_ptViewPos = m_ptMoveSrc;
					Packet.Make (pInfoMapShadow);
					m_pSock->Send (&Packet);
				}
				pInfoMapShadow = (PCInfoMapShadow)m_pLibInfoMapShadow->GetPtr (m_dwSelectShadowID);
				if (pInfoMapShadow) {
					pInfoMapShadow->m_ptViewPos = m_ptMoveDst;
					Packet.Make (pInfoMapShadow);
					m_pSock->Send (&Packet);
				}
				MakeShadowImage ();
				OnSelchangeType ();
				break;
			}
		}
		break;
	case 4:		/* 削除 */
		{
			int nResult;
			CPacketADMIN_MAP_DELETEMAPSHADOW Packet;

			m_dwSelectShadowID = GetSelectMapShadowID ();
			if (m_dwSelectShadowID == 0) {
				break;
			}

			nResult = MessageBox ("選択している影を削除しますか？", "確認", MB_ICONQUESTION | MB_YESNO);
			if (nResult != IDYES) {
				break;
			}
			Packet.Make (m_dwSelectShadowID);
			m_pSock->Send (&Packet);
		}
		break;
	}

	CDlgAdminBase::OnLButtonDown(nFlags, point);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapShadow::OnRButtonDown								 */
/* 内容		:メッセージハンドラ(WM_RBUTTONDOWN)								 */
/* 日付		:2007/09/08														 */
/* ========================================================================= */

void CDlgAdminMapShadow::OnRButtonDown(UINT nFlags, CPoint point)
{
	PCInfoMapShadow pInfoMapShadow;

	switch (m_nSelectType) {
	case 2:		/* 編集 */
		{
			int nResult;
			CDlgAdminMapShadowEdit Dlg(this);
			CPacketADMIN_RENEWMAPSHADOW Packet;

			m_dwSelectShadowID = GetSelectMapShadowID ();
			pInfoMapShadow = (PCInfoMapShadow)m_pLibInfoMapShadow->GetPtr (m_dwSelectShadowID);
			if (pInfoMapShadow == NULL) {
				break;
			}
			Dlg.m_pInfoMapShadow->Copy (pInfoMapShadow);
			nResult = Dlg.DoModal (m_pMgrData);
			if (nResult != IDOK) {
				break;
			}
			pInfoMapShadow->Copy (Dlg.m_pInfoMapShadow);
			MakeShadowImage ();

			Packet.Make (pInfoMapShadow);
			m_pSock->Send (&Packet);
		}
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapShadow::OnWndClose									 */
/* 内容		:メッセージハンドラ(WM_WNDCLOSE)								 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

LRESULT  CDlgAdminMapShadow::OnWndClose(WPARAM wParam, LPARAM lParam)
{
	PCInfoMapShadow pInfoMapShadow;

	switch (wParam) {
	case WINDOWID_SELECTMAPSHADOWGRP:	/* マップ影画像選択 */
		if (lParam < 0) {
			break;
		}
		pInfoMapShadow = (PCInfoMapShadow)m_pLibInfoMapShadow->GetPtr (m_dwSelectShadowID);
		if (pInfoMapShadow == NULL) {
			break;
		}
		pInfoMapShadow->m_wGrpID = (WORD)lParam;
		MakeShadowImage ();
		break;
	}

	return 0;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapShadow::OnVScroll									 */
/* 内容		:メッセージハンドラ(WM_VSCROLL)									 */
/* 日付		:2007/05/04														 */
/* ========================================================================= */

void CDlgAdminMapShadow::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMin, nMax;
	BOOL bChange;
	UINT nPosNow;

	nPosNow = pScrollBar->GetScrollPos ();
	bChange = TRUE;

	switch (nSBCode) {
	case SB_TOP:			/* 1番上までスクロール */
		pScrollBar->GetScrollRange (&nMin, &nMax);
		nPos = nMin;
		break;
	case SB_BOTTOM:			/* 1番下までスクロール */
		pScrollBar->GetScrollRange (&nMin, &nMax);
		nPos = nMax;
		break;
	case SB_LINEUP:			/* 1行上へスクロール */
	case SB_PAGEUP:			/* 1ページ上へスクロール */
		nPos = nPosNow;
		if (nPosNow == 0) {
			break;
		}
		nPos --;
		break;
	case SB_LINEDOWN:		/* 1行下へスクロール */
	case SB_PAGEDOWN:		/* 1ページ下へスクロール */
		nPos = nPosNow + 1;
		break;
	case SB_THUMBPOSITION:	/* 絶対位置へスクロール */
	case SB_THUMBTRACK:		/* ドラッグされた */
		break;
	default:
		bChange = FALSE;
		break;
	}

	if (bChange) {
		pScrollBar->SetScrollPos (nPos);
		m_ctlMapShadow.SetScrollPos (nPos);
	}

	CDlgAdminBase::OnVScroll (nSBCode, nPos, pScrollBar);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapShadow::OnMouseWheel								 */
/* 内容		:メッセージハンドラ(WM_MOUSEWHEEL)								 */
/* 日付		:2007/05/04														 */
/* ========================================================================= */

BOOL CDlgAdminMapShadow::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int nTmp;

	nTmp = -1;
	if (zDelta < 0) {
		nTmp = 1;
	}
	nTmp += m_ctlScroll.GetScrollPos ();
	nTmp = max (nTmp, 0);
	m_ctlScroll.SetScrollPos (nTmp);
	m_ctlMapShadow.SetScrollPos (nTmp);

	return CDlgAdminBase::OnMouseWheel (nFlags, zDelta, pt);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapShadow::OnSelchangeType							 */
/* 内容		:イベントハンドラ(CBN_SELCHANGE)								 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadow::OnSelchangeType()
{
	UpdateData ();

	m_nState		= 0;
	m_nSelectType	= m_cbType.GetCurSel ();

	switch (m_nSelectType) {
	case 1:		/* 追加 */
		RenewMessage ("追加する場所をクリックしてください");
		break;
	case 2:		/* 編集 */
		RenewMessage ("編集する影を右クリックしてください");
		break;
	case 3:		/* 移動 */
		RenewMessage ("移動する影をクリックしてください");
		break;
	case 4:		/* 削除 */
		RenewMessage ("削除する影をクリックしてください");
		break;
	default:
		RenewMessage ("");
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapShadow::RenewMessage								 */
/* 内容		:メッセージ欄を更新												 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadow::RenewMessage(LPCSTR pszMsg)
{
	m_strMsg = pszMsg;
	UpdateData (FALSE);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapShadow::MakeShadowImage							 */
/* 内容		:影一覧画像を作成												 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

void CDlgAdminMapShadow::MakeShadowImage(void)
{
	m_ctlMapShadow.Renew ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapShadow::GetSelectMapShadowID						 */
/* 内容		:選択中のマップ影IDを取得										 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

DWORD CDlgAdminMapShadow::GetSelectMapShadowID(void)
{
	DWORD dwRet;
	PCInfoMapShadow pInfoMapShadow;

	dwRet = 0;

	pInfoMapShadow = (PCInfoMapShadow)m_pLibInfoMapShadow->GetPtr (&m_ptCursor);
	if (pInfoMapShadow == NULL) {
		goto Exit;
	}

	dwRet = pInfoMapShadow->m_dwShadowID;
Exit:
	return dwRet;
}

/* Copyright(C)URARA-works 2007 */
