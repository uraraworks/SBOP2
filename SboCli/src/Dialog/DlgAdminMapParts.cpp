/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapParts.cpp										 */
/* 内容			:マップパーツ編集ダイアログクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/04/25													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "LibInfoMapParts.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_RENEWMAPPARTS.h"
#include "PacketADMIN_MAP_DELETEPARTS.h"
#include "PacketADMIN_MAP_COPYPARTS.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrDraw.h"
#include "MgrGrpData.h"
#include "DlgAdminMapPartsEdit.h"
#include "DlgAdminMapParts.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminMapParts::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapParts)
	DDX_Control(pDX, IDC_MAPPARTS, m_ctlMapParts);
	DDX_Control(pDX, IDC_SCROLL, m_ctlScroll);
	DDX_Control(pDX, IDC_TYPE, m_cbType);
	DDX_CBIndex(pDX, IDC_TYPE, m_nTypeSelect);
	DDX_Text(pDX, IDC_ID, m_strID);
	DDX_Text(pDX, IDC_MSG, m_strMsg);
	DDX_Text(pDX, IDC_ATTR, m_strAttr);
	DDX_Text(pDX, IDC_ATTR2, m_strAttr2);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_PILE, m_bPile);
}

BEGIN_MESSAGE_MAP(CDlgAdminMapParts, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapParts)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_WNDCLOSE, OnWndClose)
	ON_BN_CLICKED(IDC_PILE, &CDlgAdminMapParts::OnBnClickedPile)
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminMapParts::CDlgAdminMapParts							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/04/25														 */
/* ========================================================================= */

CDlgAdminMapParts::CDlgAdminMapParts(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapParts::IDD, pParent)
	, m_bPile(FALSE)
{
	//{{AFX_DATA_INIT(CDlgAdminMapParts)
	m_nTypeSelect = 0;
	m_strID = _T("");
	m_strMsg = _T("");
	m_strAttr = _T("");
	m_strAttr2 = _T("");
	//}}AFX_DATA_INIT

	m_nSelectType		= 0;
	m_nState			= 0;
	m_dwSelectPartsID	= 0;

	m_pMgrGrpData		= NULL;
	m_pImgParts			= new CImg32;
	m_pLibInfoMapParts	= NULL;
	m_pSock				= NULL;

	m_ptMoveSrc.x = m_ptMoveSrc.y = 0;
	m_ptMoveDst.x = m_ptMoveDst.y = 0;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapParts::~CDlgAdminMapParts							 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/04/25														 */
/* ========================================================================= */

CDlgAdminMapParts::~CDlgAdminMapParts()
{
	SAFE_DELETE (m_pImgParts);
	if (m_pMgrGrpData) {
		m_pMgrGrpData->DeleteMapPartsTmp ();
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapParts::Init										 */
/* 内容		:初期化															 */
/* 日付		:2007/04/25														 */
/* ========================================================================= */

void CDlgAdminMapParts::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pMgrGrpData		= m_pMgrData->GetMgrGrpData ();
	m_pLibInfoMapParts	= m_pMgrData->GetLibInfoMapParts ();
	m_pSock				= m_pMgrData->GetUraraSockTCP ();

	m_pMgrGrpData->ReadMapPartsTmp ();

	/* ウィンドウ作成 */
	Create (CDlgAdminMapParts::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapParts::OnAdminMsg									 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2007/05/04														 */
/* ========================================================================= */

void CDlgAdminMapParts::OnAdminMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case ADMINMSG_RENEWMAPPARTS:	/* マップパーツ情報更新 */
		MakePartsImage ();
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapParts::OnInitDialog								 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2007/04/25														 */
/* ========================================================================= */

BOOL CDlgAdminMapParts::OnInitDialog()
{
	CWnd *pWnd;
	CRect rc, rcTmp;
	SCROLLINFO stScrollInfo;

	CDlgAdminBase::OnInitDialog();

	m_ctlMapParts.Create (this, m_pMgrData);

	m_cbType.AddString ("マップ編集用に選択");
	m_cbType.AddString ("パーツの追加");
	m_cbType.AddString ("パーツの編集");
	m_cbType.AddString ("パーツの移動");
	m_cbType.AddString ("パーツの削除");
	m_cbType.AddString ("パーツのコピー");
	m_cbType.SetCurSel (0);

	pWnd = GetDlgItem (IDC_MAPPARTS);
	pWnd->GetWindowRect (rc);
	rc.right = rc.left + 16 * 23;
	rc.bottom = rc.top + 32 * 7;
	pWnd->SetWindowPos (NULL, 0, 0, rc.Width (), rc.Height (), SWP_NOZORDER | SWP_NOMOVE);

	/* スクロールバーの設定 */
	ScreenToClient (rc);
	m_ctlScroll.GetWindowRect (rcTmp);
	m_ctlScroll.SetWindowPos (NULL, rc.right, rc.top, rcTmp.Width (), rc.Height (), SWP_NOZORDER);
	m_ctlScroll.GetScrollInfo (&stScrollInfo);
	stScrollInfo.nPage	= 1;
	stScrollInfo.nMax	= 100;
	m_ctlScroll.SetScrollInfo (&stScrollInfo);

	RegisterControl (IDC_MAPPARTS,	LH_CTRL_HEIGHT);
	RegisterControl (IDC_SCROLL,	LH_CTRL_HEIGHT);

	MakePartsImage ();
	pWnd->SetFocus ();

	m_pMgrData->SetEditMapPile (m_bPile);

	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapParts::OnSize										 */
/* 内容		:メッセージハンドラ(WM_SIZE)									 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

void CDlgAdminMapParts::OnSize(UINT nType, int cx, int cy)
{
	HWND hWnd;

	CDlgAdminBase::OnSize (nType, cx, cy);

	hWnd = m_ctlScroll.GetSafeHwnd ();
	if (hWnd == NULL) {
		return;
	}

	MakePartsImage ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapParts::OnMouseMove									 */
/* 内容		:メッセージハンドラ(WM_MOUSEMOVE)								 */
/* 日付		:2007/04/25														 */
/* ========================================================================= */

void CDlgAdminMapParts::OnMouseMove(UINT nFlags, CPoint point)
{
	int nPos;
	BOOL bResult;
	CPoint ptBack;
	CRect rc;

	CDlgAdminBase::OnMouseMove(nFlags, point);

	GetDlgItem (IDC_MAPPARTS)->GetWindowRect (rc);
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

	m_ctlMapParts.OnMouseMove (point);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapParts::OnLButtonDown								 */
/* 内容		:メッセージハンドラ(WM_LBUTTONDOWN)								 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

void CDlgAdminMapParts::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bResult;
	DWORD dwTmp;
	CRect rc;
	PCInfoMapParts pInfoMapParts;

	GetDlgItem (IDC_MAPPARTS)->GetWindowRect (rc);
	ScreenToClient (rc);

	bResult = rc.PtInRect (point);
	if (bResult == FALSE) {
		return;
	}

	GetDlgItem (IDC_MAPPARTS)->SetFocus ();

	switch (m_nSelectType) {
	case 0:		/* マップ編集用に選択 */
	case 2:		/* 編集 */
		m_dwSelectPartsID = GetSelectMapPartsID ();
		m_pMgrData->SetSelectMapPartsID (m_dwSelectPartsID);
		m_strID.Format(_T("ID:%u"), m_dwSelectPartsID);
		RenewPartsInfo ();
		break;
	case 1:		/* 追加 */
		{
			CPacketADMIN_RENEWMAPPARTS Packet;

			m_dwSelectPartsID = GetSelectMapPartsID ();
			if (m_dwSelectPartsID) {
				RenewMessage ("そこには追加できません");
				break;
			}
			pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetNew ();
			pInfoMapParts->m_ptViewPos = m_ptCursor;
			m_pLibInfoMapParts->Add (pInfoMapParts);
			MakePartsImage ();

			Packet.Make (pInfoMapParts);
			m_pSock->Send (&Packet);
		}
		break;
	case 3:		/* 移動 */
		{
			CPacketADMIN_RENEWMAPPARTS Packet;

			switch (m_nState) {
			case 0:			/* 移動元の選択 */
				m_dwSelectPartsID = GetSelectMapPartsID ();
				if (m_dwSelectPartsID == 0) {
					break;
				}
				m_ptMoveSrc	= m_ptCursor;
				m_nState	= 1;
				RenewMessage ("移動先をクリックしてください");
				break;
			case 1:			/* 移動先の選択 */
				m_ptMoveDst = m_ptCursor;

				dwTmp = GetSelectMapPartsID ();
				pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (&m_ptMoveDst);
				if (pInfoMapParts) {
					/* 移動先にパーツがあった場合は入れ換える */
					pInfoMapParts->m_ptViewPos = m_ptMoveSrc;
					Packet.Make (pInfoMapParts);
					m_pSock->Send (&Packet);
				}
				pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (m_dwSelectPartsID);
				if (pInfoMapParts) {
					pInfoMapParts->m_ptViewPos = m_ptMoveDst;
					Packet.Make (pInfoMapParts);
					m_pSock->Send (&Packet);
				}
				MakePartsImage ();
				OnSelchangeType ();
				break;
			}
		}
		break;
	case 4:		/* 削除 */
		{
			int nResult;
			CPacketADMIN_MAP_DELETEPARTS Packet;

			m_dwSelectPartsID = GetSelectMapPartsID ();
			if (m_dwSelectPartsID == 0) {
				break;
			}

                        nResult = MessageBox (_T("選択しているパーツを削除しますか？"), _T("確認"), MB_ICONQUESTION | MB_YESNO);
			if (nResult != IDYES) {
				break;
			}
			Packet.Make (m_dwSelectPartsID);
			m_pSock->Send (&Packet);
		}
		break;
	case 5:		/* コピー */
		{
			CPacketADMIN_MAP_COPYPARTS Packet;

			switch (m_nState) {
			case 0:			/* コピー元の選択 */
				m_dwSelectPartsID = GetSelectMapPartsID ();
				if (m_dwSelectPartsID == 0) {
					break;
				}
				m_ptMoveSrc	= m_ptCursor;
				m_nState	= 1;
				RenewMessage ("コピー先をクリックしてください");
				break;
			case 1:			/* 移動先の選択 */
				dwTmp = GetSelectMapPartsID ();
				pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (&m_ptMoveDst);
				if (pInfoMapParts) {
					/* コピー先にパーツがあった場合は処理しない */
					break;
				}
				Packet.Make (m_dwSelectPartsID, m_ptCursor);
				m_pSock->Send (&Packet);
				OnSelchangeType ();
				break;
			}
		}
		break;
	}

	CDlgAdminBase::OnLButtonDown(nFlags, point);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapParts::OnRButtonDown								 */
/* 内容		:メッセージハンドラ(WM_RBUTTONDOWN)								 */
/* 日付		:2007/09/08														 */
/* ========================================================================= */

void CDlgAdminMapParts::OnRButtonDown(UINT nFlags, CPoint point)
{
	PCInfoMapParts pInfoMapParts;

	switch (m_nSelectType) {
	case 2:		/* 編集 */
		{
			int nResult;
			CDlgAdminMapPartsEdit Dlg(this);
			CPacketADMIN_RENEWMAPPARTS Packet;

			m_dwSelectPartsID = GetSelectMapPartsID ();
			pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (m_dwSelectPartsID);
			if (pInfoMapParts == NULL) {
				break;
			}
			Dlg.m_pInfoMapParts->Copy (pInfoMapParts);
			nResult = Dlg.DoModal (m_pMgrData);
			if (nResult != IDOK) {
				break;
			}
			pInfoMapParts->Copy (Dlg.m_pInfoMapParts);
			MakePartsImage ();

			Packet.Make (pInfoMapParts);
			m_pSock->Send (&Packet);
		}
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapParts::OnWndClose									 */
/* 内容		:メッセージハンドラ(WM_WNDCLOSE)								 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

LRESULT CDlgAdminMapParts::OnWndClose(WPARAM wParam, LPARAM lParam)
{
	PCInfoMapParts pInfoMapParts;

	switch (wParam) {
	case WINDOWID_SELECTMAPPARTSGRP:	/* マップパーツ画像選択 */
		if (lParam < 0) {
			break;
		}
		pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (m_dwSelectPartsID);
		if (pInfoMapParts == NULL) {
			break;
		}
		pInfoMapParts->m_wGrpIDBase = (WORD)lParam;
		MakePartsImage ();
		break;
	}

	return 0;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapParts::OnVScroll									 */
/* 内容		:メッセージハンドラ(WM_VSCROLL)									 */
/* 日付		:2007/05/04														 */
/* ========================================================================= */

void CDlgAdminMapParts::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nMin, nMax;
	BOOL bChange;
	UINT nPosNow;
	SCROLLINFO stScrollInfo;

	nPosNow = pScrollBar->GetScrollPos ();
	bChange = TRUE;

	m_ctlScroll.GetScrollInfo (&stScrollInfo);

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
		nPos = nPosNow;
		if (nPosNow == 0) {
			break;
		}
		nPos --;
		break;
	case SB_PAGEUP:			/* 1ページ上へスクロール */
		nPos = nPosNow - stScrollInfo.nPage;
		break;
	case SB_LINEDOWN:		/* 1行下へスクロール */
		nPos = nPosNow + 1;
		break;
	case SB_PAGEDOWN:		/* 1ページ下へスクロール */
		nPos = nPosNow + stScrollInfo.nPage;
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
		m_ctlMapParts.SetScrollPos (nPos);
	}

	CDlgAdminBase::OnVScroll (nSBCode, nPos, pScrollBar);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapParts::OnMouseWheel								 */
/* 内容		:メッセージハンドラ(WM_MOUSEWHEEL)								 */
/* 日付		:2007/05/04														 */
/* ========================================================================= */

BOOL CDlgAdminMapParts::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int nTmp;

	nTmp = -1;
	if (zDelta < 0) {
		nTmp = 1;
	}
	nTmp += m_ctlScroll.GetScrollPos ();
	nTmp = max (nTmp, 0);
	m_ctlScroll.SetScrollPos (nTmp);
	m_ctlMapParts.SetScrollPos (nTmp);

	return CDlgAdminBase::OnMouseWheel (nFlags, zDelta, pt);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapParts::OnSelchangeType								 */
/* 内容		:イベントハンドラ(CBN_SELCHANGE)								 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

void CDlgAdminMapParts::OnSelchangeType()
{
	UpdateData ();

	m_nState		= 0;
	m_nSelectType	= m_cbType.GetCurSel ();

	switch (m_nSelectType) {
	case 1:		/* 追加 */
		RenewMessage ("追加する場所をクリックしてください");
		break;
	case 2:		/* 編集 */
		RenewMessage ("編集するパーツを右クリックしてください");
		break;
	case 3:		/* 移動 */
		RenewMessage ("移動するパーツをクリックしてください");
		break;
	case 4:		/* 削除 */
		RenewMessage ("削除するパーツをクリックしてください");
		break;
	case 5:		/* コピー */
		RenewMessage ("コピー元のパーツをクリックしてください");
		break;
	default:
		RenewMessage ("");
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapParts::OnBnClickedPile								 */
/* 内容		:ボタンハンドラ(重ね用マップ)									 */
/* 日付		:2008/12/06														 */
/* ========================================================================= */

void CDlgAdminMapParts::OnBnClickedPile()
{
	UpdateData();
	m_pMgrData->SetEditMapPile (m_bPile);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapParts::RenewMessage								 */
/* 内容		:メッセージ欄を更新												 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

void CDlgAdminMapParts::RenewMessage(LPCSTR pszMsg)
{
	m_strMsg = pszMsg;
	UpdateData (FALSE);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapParts::RenewPartsInfo								 */
/* 内容		:パーツ情報を更新												 */
/* 日付		:2007/06/11														 */
/* ========================================================================= */

void CDlgAdminMapParts::RenewPartsInfo(void)
{
	PCInfoMapParts pInfoMapParts;

	m_strAttr  = "属性:";
	m_strAttr2 = "進めない方向:";

	pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (m_dwSelectPartsID);
	if (pInfoMapParts == NULL) {
		return;
	}

	if (pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_BLOCK) {
		m_strAttr += "ぶつかる ";
	}
	if (pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_PILE) {
		m_strAttr += "重なる ";
	}
	if (pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_PILEBACK) {
		m_strAttr += "下地も重ねる ";
	}

	if (pInfoMapParts->m_byBlockDirection & BIT_PARTSBLOCK_UP) {
		m_strAttr2 += "ª ";
	}
	if (pInfoMapParts->m_byBlockDirection & BIT_PARTSBLOCK_DOWN) {
		m_strAttr2 += "« ";
	}
	if (pInfoMapParts->m_byBlockDirection & BIT_PARTSBLOCK_LEFT) {
		m_strAttr2 += "© ";
	}
	if (pInfoMapParts->m_byBlockDirection & BIT_PARTSBLOCK_RIGHT) {
		m_strAttr2 += "¨ ";
	}

	UpdateData (FALSE);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapParts::MakePartsImage								 */
/* 内容		:パーツ一覧画像を作成											 */
/* 日付		:2007/04/25														 */
/* ========================================================================= */

void CDlgAdminMapParts::MakePartsImage(void)
{
	SCROLLINFO stScrollInfo;
	CRect rc;

	m_ctlMapParts.Renew ();
	m_ctlMapParts.GetWindowRect (rc);

	m_ctlScroll.GetScrollInfo (&stScrollInfo);
	stScrollInfo.nPage	= (rc.Height () / 16) - 1;
	stScrollInfo.nMax	= 1000;
	m_ctlScroll.SetScrollInfo (&stScrollInfo);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapParts::GetSelectMapPartsID							 */
/* 内容		:選択中のマップパーツIDを取得									 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

DWORD CDlgAdminMapParts::GetSelectMapPartsID(void)
{
	DWORD dwRet;
	PCInfoMapParts pInfoMapParts;

	dwRet = 0;

	pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (&m_ptCursor);
	if (pInfoMapParts == NULL) {
		goto Exit;
	}

	dwRet = pInfoMapParts->m_dwPartsID;
Exit:
	return dwRet;
}

/* Copyright(C)URARA-works 2007 */
