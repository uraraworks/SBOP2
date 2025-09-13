/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ƒtƒ@ƒCƒ‹–¼	:DlgAdminMapParts.cpp										 */
/* “à—e			:ƒ}ƒbƒvƒp[ƒc•ÒWƒ_ƒCƒAƒƒOƒNƒ‰ƒX ŽÀ‘•ƒtƒ@ƒCƒ‹				 */
/* ì¬			:”N‚ª‚ç”N’†t‚¤‚ç‚ç(URARA-works)							 */
/* ì¬ŠJŽn“ú	:2007/04/25													 */
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
/* ƒNƒ‰ƒX‚ÌÝ’è																 */
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
/* ŠÖ”–¼	:CDlgAdminMapParts::CDlgAdminMapParts							 */
/* “à—e		:ƒRƒ“ƒXƒgƒ‰ƒNƒ^													 */
/* “ú•t		:2007/04/25														 */
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
/* ŠÖ”–¼	:CDlgAdminMapParts::~CDlgAdminMapParts							 */
/* “à—e		:ƒfƒXƒgƒ‰ƒNƒ^													 */
/* “ú•t		:2007/04/25														 */
/* ========================================================================= */

CDlgAdminMapParts::~CDlgAdminMapParts()
{
	SAFE_DELETE (m_pImgParts);
	if (m_pMgrGrpData) {
		m_pMgrGrpData->DeleteMapPartsTmp ();
	}
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminMapParts::Init										 */
/* “à—e		:‰Šú‰»															 */
/* “ú•t		:2007/04/25														 */
/* ========================================================================= */

void CDlgAdminMapParts::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pMgrGrpData		= m_pMgrData->GetMgrGrpData ();
	m_pLibInfoMapParts	= m_pMgrData->GetLibInfoMapParts ();
	m_pSock				= m_pMgrData->GetUraraSockTCP ();

	m_pMgrGrpData->ReadMapPartsTmp ();

	/* ƒEƒBƒ“ƒhƒEì¬ */
	Create (CDlgAdminMapParts::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminMapParts::OnAdminMsg									 */
/* “à—e		:ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_ADMINMSG)								 */
/* “ú•t		:2007/05/04														 */
/* ========================================================================= */

void CDlgAdminMapParts::OnAdminMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case ADMINMSG_RENEWMAPPARTS:	/* ƒ}ƒbƒvƒp[ƒcî•ñXV */
		MakePartsImage ();
		break;
	}
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminMapParts::OnInitDialog								 */
/* “à—e		:ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_INITDIALOG)								 */
/* “ú•t		:2007/04/25														 */
/* ========================================================================= */

BOOL CDlgAdminMapParts::OnInitDialog()
{
	CWnd *pWnd;
	CRect rc, rcTmp;
	SCROLLINFO stScrollInfo;

	CDlgAdminBase::OnInitDialog();

	m_ctlMapParts.Create (this, m_pMgrData);

	m_cbType.AddString ("ƒ}ƒbƒv•ÒW—p‚É‘I‘ð");
	m_cbType.AddString ("ƒp[ƒc‚Ì’Ç‰Á");
	m_cbType.AddString ("ƒp[ƒc‚Ì•ÒW");
	m_cbType.AddString ("ƒp[ƒc‚ÌˆÚ“®");
	m_cbType.AddString ("ƒp[ƒc‚Ìíœ");
	m_cbType.AddString ("ƒp[ƒc‚ÌƒRƒs[");
	m_cbType.SetCurSel (0);

	pWnd = GetDlgItem (IDC_MAPPARTS);
	pWnd->GetWindowRect (rc);
	rc.right = rc.left + 16 * 23;
	rc.bottom = rc.top + 32 * 7;
	pWnd->SetWindowPos (NULL, 0, 0, rc.Width (), rc.Height (), SWP_NOZORDER | SWP_NOMOVE);

	/* ƒXƒNƒ[ƒ‹ƒo[‚ÌÝ’è */
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
/* ŠÖ”–¼	:CDlgAdminMapParts::OnSize										 */
/* “à—e		:ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_SIZE)									 */
/* “ú•t		:2007/04/28														 */
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
/* ŠÖ”–¼	:CDlgAdminMapParts::OnMouseMove									 */
/* “à—e		:ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_MOUSEMOVE)								 */
/* “ú•t		:2007/04/25														 */
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
/* ŠÖ”–¼	:CDlgAdminMapParts::OnLButtonDown								 */
/* “à—e		:ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_LBUTTONDOWN)								 */
/* “ú•t		:2007/04/28														 */
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
	case 0:		/* ƒ}ƒbƒv•ÒW—p‚É‘I‘ð */
	case 2:		/* •ÒW */
		m_dwSelectPartsID = GetSelectMapPartsID ();
		m_pMgrData->SetSelectMapPartsID (m_dwSelectPartsID);
		m_strID.Format ("ID:%u", m_dwSelectPartsID);
		RenewPartsInfo ();
		break;
	case 1:		/* ’Ç‰Á */
		{
			CPacketADMIN_RENEWMAPPARTS Packet;

			m_dwSelectPartsID = GetSelectMapPartsID ();
			if (m_dwSelectPartsID) {
				RenewMessage ("‚»‚±‚É‚Í’Ç‰Á‚Å‚«‚Ü‚¹‚ñ");
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
	case 3:		/* ˆÚ“® */
		{
			CPacketADMIN_RENEWMAPPARTS Packet;

			switch (m_nState) {
			case 0:			/* ˆÚ“®Œ³‚Ì‘I‘ð */
				m_dwSelectPartsID = GetSelectMapPartsID ();
				if (m_dwSelectPartsID == 0) {
					break;
				}
				m_ptMoveSrc	= m_ptCursor;
				m_nState	= 1;
				RenewMessage ("ˆÚ“®æ‚ðƒNƒŠƒbƒN‚µ‚Ä‚­‚¾‚³‚¢");
				break;
			case 1:			/* ˆÚ“®æ‚Ì‘I‘ð */
				m_ptMoveDst = m_ptCursor;

				dwTmp = GetSelectMapPartsID ();
				pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (&m_ptMoveDst);
				if (pInfoMapParts) {
					/* ˆÚ“®æ‚Éƒp[ƒc‚ª‚ ‚Á‚½ê‡‚Í“ü‚êŠ·‚¦‚é */
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
	case 4:		/* íœ */
		{
			int nResult;
			CPacketADMIN_MAP_DELETEPARTS Packet;

			m_dwSelectPartsID = GetSelectMapPartsID ();
			if (m_dwSelectPartsID == 0) {
				break;
			}

			nResult = MessageBox ("‘I‘ð‚µ‚Ä‚¢‚éƒp[ƒc‚ðíœ‚µ‚Ü‚·‚©H", "Šm”F", MB_ICONQUESTION | MB_YESNO);
			if (nResult != IDYES) {
				break;
			}
			Packet.Make (m_dwSelectPartsID);
			m_pSock->Send (&Packet);
		}
		break;
	case 5:		/* ƒRƒs[ */
		{
			CPacketADMIN_MAP_COPYPARTS Packet;

			switch (m_nState) {
			case 0:			/* ƒRƒs[Œ³‚Ì‘I‘ð */
				m_dwSelectPartsID = GetSelectMapPartsID ();
				if (m_dwSelectPartsID == 0) {
					break;
				}
				m_ptMoveSrc	= m_ptCursor;
				m_nState	= 1;
				RenewMessage ("ƒRƒs[æ‚ðƒNƒŠƒbƒN‚µ‚Ä‚­‚¾‚³‚¢");
				break;
			case 1:			/* ˆÚ“®æ‚Ì‘I‘ð */
				dwTmp = GetSelectMapPartsID ();
				pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (&m_ptMoveDst);
				if (pInfoMapParts) {
					/* ƒRƒs[æ‚Éƒp[ƒc‚ª‚ ‚Á‚½ê‡‚Íˆ—‚µ‚È‚¢ */
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
/* ŠÖ”–¼	:CDlgAdminMapParts::OnRButtonDown								 */
/* “à—e		:ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_RBUTTONDOWN)								 */
/* “ú•t		:2007/09/08														 */
/* ========================================================================= */

void CDlgAdminMapParts::OnRButtonDown(UINT nFlags, CPoint point)
{
	PCInfoMapParts pInfoMapParts;

	switch (m_nSelectType) {
	case 2:		/* •ÒW */
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
/* ŠÖ”–¼	:CDlgAdminMapParts::OnWndClose									 */
/* “à—e		:ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_WNDCLOSE)								 */
/* “ú•t		:2007/04/28														 */
/* ========================================================================= */

LRESULT CDlgAdminMapParts::OnWndClose(WPARAM wParam, LPARAM lParam)
{
	PCInfoMapParts pInfoMapParts;

	switch (wParam) {
	case WINDOWID_SELECTMAPPARTSGRP:	/* ƒ}ƒbƒvƒp[ƒc‰æ‘œ‘I‘ð */
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
/* ŠÖ”–¼	:CDlgAdminMapParts::OnVScroll									 */
/* “à—e		:ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_VSCROLL)									 */
/* “ú•t		:2007/05/04														 */
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
	case SB_TOP:			/* 1”Ôã‚Ü‚ÅƒXƒNƒ[ƒ‹ */
		pScrollBar->GetScrollRange (&nMin, &nMax);
		nPos = nMin;
		break;
	case SB_BOTTOM:			/* 1”Ô‰º‚Ü‚ÅƒXƒNƒ[ƒ‹ */
		pScrollBar->GetScrollRange (&nMin, &nMax);
		nPos = nMax;
		break;
	case SB_LINEUP:			/* 1sã‚ÖƒXƒNƒ[ƒ‹ */
		nPos = nPosNow;
		if (nPosNow == 0) {
			break;
		}
		nPos --;
		break;
	case SB_PAGEUP:			/* 1ƒy[ƒWã‚ÖƒXƒNƒ[ƒ‹ */
		nPos = nPosNow - stScrollInfo.nPage;
		break;
	case SB_LINEDOWN:		/* 1s‰º‚ÖƒXƒNƒ[ƒ‹ */
		nPos = nPosNow + 1;
		break;
	case SB_PAGEDOWN:		/* 1ƒy[ƒW‰º‚ÖƒXƒNƒ[ƒ‹ */
		nPos = nPosNow + stScrollInfo.nPage;
		break;
	case SB_THUMBPOSITION:	/* â‘ÎˆÊ’u‚ÖƒXƒNƒ[ƒ‹ */
	case SB_THUMBTRACK:		/* ƒhƒ‰ƒbƒO‚³‚ê‚½ */
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
/* ŠÖ”–¼	:CDlgAdminMapParts::OnMouseWheel								 */
/* “à—e		:ƒƒbƒZ[ƒWƒnƒ“ƒhƒ‰(WM_MOUSEWHEEL)								 */
/* “ú•t		:2007/05/04														 */
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
/* ŠÖ”–¼	:CDlgAdminMapParts::OnSelchangeType								 */
/* “à—e		:ƒCƒxƒ“ƒgƒnƒ“ƒhƒ‰(CBN_SELCHANGE)								 */
/* “ú•t		:2007/04/28														 */
/* ========================================================================= */

void CDlgAdminMapParts::OnSelchangeType()
{
	UpdateData ();

	m_nState		= 0;
	m_nSelectType	= m_cbType.GetCurSel ();

	switch (m_nSelectType) {
	case 1:		/* ’Ç‰Á */
		RenewMessage ("’Ç‰Á‚·‚éêŠ‚ðƒNƒŠƒbƒN‚µ‚Ä‚­‚¾‚³‚¢");
		break;
	case 2:		/* •ÒW */
		RenewMessage ("•ÒW‚·‚éƒp[ƒc‚ð‰EƒNƒŠƒbƒN‚µ‚Ä‚­‚¾‚³‚¢");
		break;
	case 3:		/* ˆÚ“® */
		RenewMessage ("ˆÚ“®‚·‚éƒp[ƒc‚ðƒNƒŠƒbƒN‚µ‚Ä‚­‚¾‚³‚¢");
		break;
	case 4:		/* íœ */
		RenewMessage ("íœ‚·‚éƒp[ƒc‚ðƒNƒŠƒbƒN‚µ‚Ä‚­‚¾‚³‚¢");
		break;
	case 5:		/* ƒRƒs[ */
		RenewMessage ("ƒRƒs[Œ³‚Ìƒp[ƒc‚ðƒNƒŠƒbƒN‚µ‚Ä‚­‚¾‚³‚¢");
		break;
	default:
		RenewMessage ("");
		break;
	}
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminMapParts::OnBnClickedPile								 */
/* “à—e		:ƒ{ƒ^ƒ“ƒnƒ“ƒhƒ‰(d‚Ë—pƒ}ƒbƒv)									 */
/* “ú•t		:2008/12/06														 */
/* ========================================================================= */

void CDlgAdminMapParts::OnBnClickedPile()
{
	UpdateData();
	m_pMgrData->SetEditMapPile (m_bPile);
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminMapParts::RenewMessage								 */
/* “à—e		:ƒƒbƒZ[ƒW—“‚ðXV												 */
/* “ú•t		:2007/04/28														 */
/* ========================================================================= */

void CDlgAdminMapParts::RenewMessage(LPCSTR pszMsg)
{
	m_strMsg = pszMsg;
	UpdateData (FALSE);
}


/* ========================================================================= */
/* ŠÖ”–¼	:CDlgAdminMapParts::RenewPartsInfo								 */
/* “à—e		:ƒp[ƒcî•ñ‚ðXV												 */
/* “ú•t		:2007/06/11														 */
/* ========================================================================= */

void CDlgAdminMapParts::RenewPartsInfo(void)
{
	PCInfoMapParts pInfoMapParts;

	m_strAttr  = "‘®«:";
	m_strAttr2 = "i‚ß‚È‚¢•ûŒü:";

	pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (m_dwSelectPartsID);
	if (pInfoMapParts == NULL) {
		return;
	}

	if (pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_BLOCK) {
		m_strAttr += "‚Ô‚Â‚©‚é ";
	}
	if (pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_PILE) {
		m_strAttr += "d‚È‚é ";
	}
	if (pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_PILEBACK) {
		m_strAttr += "‰º’n‚àd‚Ë‚é ";
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
/* ŠÖ”–¼	:CDlgAdminMapParts::MakePartsImage								 */
/* “à—e		:ƒp[ƒcˆê——‰æ‘œ‚ðì¬											 */
/* “ú•t		:2007/04/25														 */
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
/* ŠÖ”–¼	:CDlgAdminMapParts::GetSelectMapPartsID							 */
/* “à—e		:‘I‘ð’†‚Ìƒ}ƒbƒvƒp[ƒcID‚ðŽæ“¾									 */
/* “ú•t		:2007/04/28														 */
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
