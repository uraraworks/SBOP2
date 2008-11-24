/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapEventBase.cpp									 */
/* 内容			:マップイベント編集ダイアログクラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoMapEventBase.h"
#include "LibInfoMapEvent.h"
#include "MgrData.h"
#include "DlgAdminMapEventMOVE.h"
#include "DlgAdminMapEventMAPMOVE.h"
#include "DlgAdminMapEventINITSTATUS.h"
#include "DlgAdminMapEventGRPIDTMP.h"
#include "DlgAdminMapEventBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminMapEventBase::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapEventBase)
	DDX_Control(pDX, IDC_TYPE, m_ctlType);
	DDX_Control(pDX, IDC_HITTYPE, m_ctlHitType);
	DDX_Control(pDX, IDC_DIRECTION, m_ctlDirection);
	DDX_Text(pDX, IDC_POSX, m_nPosX);
	DDX_Text(pDX, IDC_POSY, m_nPosY);
	DDX_Text(pDX, IDC_POSX2, m_nPosX2);
	DDX_Text(pDX, IDC_POSY2, m_nPosY2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapEventBase, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapEventBase)
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
	ON_CBN_SELCHANGE(IDC_HITTYPE, OnSelchangeHitType)
	ON_CBN_SELCHANGE(IDC_DIRECTION, OnSelchangeDirection)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventBase::CDlgAdminMapEventBase					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/06/24														 */
/* ========================================================================= */

CDlgAdminMapEventBase::CDlgAdminMapEventBase(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapEventBase::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapEventBase)
	m_nPosX = 0;
	m_nPosY = 0;
	m_nPosX2 = 0;
	m_nPosY2 = 0;
	//}}AFX_DATA_INIT

	m_nEventType	= -1;
	m_nHitType		= -1;
	m_nHitDirection	= -1;
	m_bModeModify	= FALSE;
	m_ppWndNotify	= NULL;
	m_pDlgType		= NULL;
	m_pInfo			= NULL;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventBase::~CDlgAdminMapEventBase					 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/06/24														 */
/* ========================================================================= */

CDlgAdminMapEventBase::~CDlgAdminMapEventBase()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventBase::Init									 */
/* 内容		:初期化															 */
/* 日付		:2008/06/24														 */
/* ========================================================================= */

void CDlgAdminMapEventBase::Init(CMgrData *pMgrData, CWnd **pWndNotify)
{
	CDlgAdminBase::Init (pMgrData);

	m_ppWndNotify = pWndNotify;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventBase::Get										 */
/* 内容		:編集内容を取得													 */
/* 日付		:2008/06/25														 */
/* ========================================================================= */

void CDlgAdminMapEventBase::Get(CInfoMapEventBase *&pDst)
{
	CLibInfoMapEvent LibInfo;

	if (m_pInfo == NULL) {
		return;
	}

	SAFE_DELETE (pDst);
	pDst = (PCInfoMapEventBase)LibInfo.GetNew (m_pInfo->m_nType);
	pDst->Copy (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventBase::SetModify								 */
/* 内容		:編集モードとして設定											 */
/* 日付		:2007/08/16														 */
/* ========================================================================= */

void CDlgAdminMapEventBase::SetModify(CInfoMapEventBase *pSrc)
{
	CLibInfoMapEvent LibInfo;

	SAFE_DELETE (m_pInfo);
	m_pInfo = (PCInfoMapEventBase)LibInfo.GetNew (pSrc->m_nType);
	m_pInfo->Copy (pSrc);

	m_nPosX  = m_pInfo->m_ptPos.x;
	m_nPosY  = m_pInfo->m_ptPos.y;
	m_nPosX2 = m_pInfo->m_ptPos2.x;
	m_nPosY2 = m_pInfo->m_ptPos2.y;

	m_bModeModify = TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventBase::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/06/24														 */
/* ========================================================================= */

BOOL CDlgAdminMapEventBase::OnInitDialog()
{
	int i, nNo;
//	LPCSTR pszTmp;

	CDlgAdminBase::OnInitDialog();

	if (m_ppWndNotify) {
		*m_ppWndNotify = this;
	}

	if (m_bModeModify) {
		SetWindowText ("マップイベントの編集");
	}

	m_ctlType.InsertString (0, "未設定");
	m_ctlType.SetItemData (0, MAPEVENTTYPE_NONE);
	m_ctlType.InsertString (1, "マップ内移動");
	m_ctlType.SetItemData (1, MAPEVENTTYPE_MOVE);
	m_ctlType.InsertString (2, "マップ間移動");
	m_ctlType.SetItemData (2, MAPEVENTTYPE_MAPMOVE);
	m_ctlType.InsertString (3, "ゴミ箱");
	m_ctlType.SetItemData (3, MAPEVENTTYPE_TRASHBOX);
	m_ctlType.InsertString (4, "ステータス初期化");
	m_ctlType.SetItemData (4, MAPEVENTTYPE_INITSTATUS);
	m_ctlType.InsertString (5, "一時画像設定");
	m_ctlType.SetItemData (5, MAPEVENTTYPE_GRPIDTMP);

	m_ctlHitType.InsertString (0, "マップ座標で判定");
	m_ctlHitType.SetItemData (0, MAPEVENTHITTYPE_MAPPOS);
	m_ctlHitType.InsertString (1, "キャラ座標で判定");
	m_ctlHitType.SetItemData (1, MAPEVENTHITTYPE_CHARPOS);
	m_ctlHitType.InsertString (2, "範囲で判定");
	m_ctlHitType.SetItemData (2, MAPEVENTHITTYPE_AREA);

	m_ctlDirection.InsertString (0, "上");
	m_ctlDirection.InsertString (1, "下");
	m_ctlDirection.InsertString (2, "左");
	m_ctlDirection.InsertString (3, "右");
	m_ctlDirection.InsertString (4, "指定無し");

	nNo = 0;
	if (m_pInfo) {
		for (i = 0; i < MAPEVENTTYPE_MAX; i ++) {
			if (m_pInfo->m_nType == m_ctlType.GetItemData (i)) {
				nNo = i;
				m_nEventType = m_pInfo->m_nType;
				break;
			}
		}
	}
	m_ctlType.SetCurSel (nNo);

	nNo = 0;
	if (m_pInfo) {
		for (i = 0; i < MAPEVENTHITTYPE_MAX; i ++) {
			if (m_pInfo->m_nHitType == m_ctlHitType.GetItemData (i)) {
				nNo = i;
				m_nHitType = m_pInfo->m_nHitType;
				break;
			}
		}
	}
	m_ctlHitType.SetCurSel (nNo);

	nNo = 4;
	if (m_pInfo) {
		nNo = m_pInfo->m_nHitDirection;
		if (nNo < 0) {
			nNo = 4;
		}
	}
	m_ctlDirection.SetCurSel (nNo);

	OnSelchangeType ();
	OnSelchangeHitType ();
	OnSelchangeDirection ();
	if (m_pDlgType && m_pInfo) {
		m_pDlgType->Set (m_pInfo);
	}

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnAdminMsg										 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

LRESULT CDlgAdminMapEventBase::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:		/* 左クリック通知 */
		m_nPosX = HIWORD (lParam);
		m_nPosY = LOWORD (lParam);
		UpdateData (FALSE);
		break;
	case ADMINMSG_NOTIFYTYPE_RBUTTONDOWN:		/* 右クリック通知 */
		if (m_pDlgType == NULL) {
			break;
		}
		m_pDlgType->PostMessage (WM_ADMINMSG, wParam, lParam);
		break;
	}

	return -1;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventBase::PostNcDestroy							 */
/* 内容		:終了処理														 */
/* 日付		:2008/06/24														 */
/* ========================================================================= */

void CDlgAdminMapEventBase::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventBase::OnSelchangeType							 */
/* 内容		:イベントハンドラ(CBN_SELCHANGE)								 */
/* 日付		:2008/06/24														 */
/* ========================================================================= */

void CDlgAdminMapEventBase::OnSelchangeType()
{
	int nNo, nEventType;
	CRect rc;
	PCInfoMapEventBase pInfoTmp;
	CLibInfoMapEvent LibInfo;

	nNo = m_ctlType.GetCurSel ();
	nEventType = m_ctlType.GetItemData (nNo);

	if (m_pDlgType) {
		m_pDlgType->DestroyWindow ();
		m_pDlgType = NULL;
	}
	switch (nEventType) {
	case MAPEVENTTYPE_MOVE:			/* マップ内移動 */
		m_pDlgType = new CDlgAdminMapEventMOVE(this);
		break;
	case MAPEVENTTYPE_MAPMOVE:		/* マップ間移動 */
		m_pDlgType = new CDlgAdminMapEventMAPMOVE(this);
		break;
	case MAPEVENTTYPE_INITSTATUS:	/* ステータス初期化 */
		m_pDlgType = new CDlgAdminMapEventINITSTATUS(this);
		break;
	case MAPEVENTTYPE_GRPIDTMP:		/* 一時画像設定 */
		m_pDlgType = new CDlgAdminMapEventGRPIDTMP(this);
		break;
	default:
		m_pDlgType = new CDlgAdminMapEventNONE(this);
		break;
	}
	if (m_pDlgType) {
		m_pDlgType->Init (m_pMgrData);
		GetDlgItem (IDC_EVENT)->GetWindowRect (rc);
		ScreenToClient (rc);
		m_pDlgType->MoveWindow (rc.left, rc.top, rc.Width (), rc.Height ());
	}

	if (m_nEventType != nEventType) {
		pInfoTmp = (PCInfoMapEventBase)LibInfo.GetNew (nEventType);
		if (m_pInfo) {
			pInfoTmp->m_dwMapEventID	= m_pInfo->m_dwMapEventID;
			pInfoTmp->m_ptPos			= m_pInfo->m_ptPos;
		}
		pInfoTmp->m_nType = nEventType;

		SAFE_DELETE (m_pInfo);
		m_pInfo = pInfoTmp;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventBase::OnSelchangeHitType						 */
/* 内容		:イベントハンドラ(CBN_SELCHANGE)								 */
/* 日付		:2008/10/26														 */
/* ========================================================================= */

void CDlgAdminMapEventBase::OnSelchangeHitType()
{
	int nNo;

	if (m_pInfo == NULL) {
		return;
	}

	nNo = m_ctlHitType.GetCurSel ();
	m_nHitType = m_ctlHitType.GetItemData (nNo);
	m_pInfo->m_nHitType = m_nHitType;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventBase::OnSelchangeDirection					 */
/* 内容		:イベントハンドラ(CBN_SELCHANGE)								 */
/* 日付		:2008/11/24														 */
/* ========================================================================= */

void CDlgAdminMapEventBase::OnSelchangeDirection()
{
	int nNo;

	if (m_pInfo == NULL) {
		return;
	}

	nNo = m_ctlDirection.GetCurSel ();
	m_nHitDirection = m_ctlDirection.GetCurSel ();
	m_pInfo->m_nHitDirection = m_nHitDirection;
	if (m_nHitDirection > 3) {
		m_pInfo->m_nHitDirection = -1;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventBase::OnOK									 */
/* 内容		:ボタンハンドラ(OK)												 */
/* 日付		:2007/08/16														 */
/* ========================================================================= */

void CDlgAdminMapEventBase::OnOK()
{
	UpdateData ();

	if (m_pInfo) {
		m_pInfo->m_ptPos.x  = m_nPosX;
		m_pInfo->m_ptPos.y  = m_nPosY;
		m_pInfo->m_ptPos2.x = m_nPosX2;
		m_pInfo->m_ptPos2.y = m_nPosY2;
		if (m_pDlgType) {
			m_pDlgType->Get (m_pInfo);
		}
	}

	CDlgAdminBase::EndDialog (IDOK);
}

/* Copyright(C)URARA-works 2008 */
