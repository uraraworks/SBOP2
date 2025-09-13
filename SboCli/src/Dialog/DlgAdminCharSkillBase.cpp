/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharSkillBase.cpp									 */
/* 内容			:スキル編集ダイアログクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/07													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "MgrData.h"
#include "InfoSkillBase.h"
#include "LibInfoSkill.h"
#include "DlgAdminCharSkillNONE.h"
#include "DlgAdminCharSkillMOVEATACK.h"
#include "DlgAdminCharSkillHEAL.h"
#include "WndSelectGrp.h"
#include "DlgAdminCharSkillBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminCharSkillBase::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharSkillBase)
	DDX_Control(pDX, IDC_TYPEMAIN, m_ctlTypeMain);
	DDX_Control(pDX, IDC_TYPESUB, m_ctlTypeSub);
	DDX_Control(pDX, IDC_USE, m_ctlUse);
	DDX_Control(pDX, IDC_ICONGRP, m_ctlIconGrp);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_SP, m_dwSP);
}

BEGIN_MESSAGE_MAP(CDlgAdminCharSkillBase, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharSkillBase)
	ON_WM_LBUTTONDOWN()
	ON_CBN_SELCHANGE(IDC_TYPEMAIN, OnSelchangeTypeMain)
	ON_CBN_SELCHANGE(IDC_TYPESUB, OnSelchangeTypeSub)
	ON_CBN_SELCHANGE(IDC_USE, OnSelchangeUse)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
	ON_MESSAGE(WM_WNDCLOSE, OnWndClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillBase::CDlgAdminCharSkillBase					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

CDlgAdminCharSkillBase::CDlgAdminCharSkillBase(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharSkillBase::IDD, pParent)
	, m_strName(_T(""))
	, m_dwSP(0)
{
	//{{AFX_DATA_INIT(CDlgAdminCharSkillBase)
	//}}AFX_DATA_INIT

	m_bInit			= TRUE;
	m_nTypeMain		= -1;
	m_nTypeSub		= -1;
	m_nUse			= -1;
	m_bModeModify	= FALSE;
	m_pDlgType		= NULL;
	m_pInfo			= NULL;
	m_pWndSelectGrp	= NULL;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillBase::~CDlgAdminCharSkillBase				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

CDlgAdminCharSkillBase::~CDlgAdminCharSkillBase()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillBase::Init									 */
/* 内容		:初期化															 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillBase::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillBase::Get									 */
/* 内容		:編集内容を取得													 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillBase::Get(CInfoSkillBase *&pDst)
{
	CLibInfoSkill LibInfo;

	if (m_pInfo == NULL) {
		return;
	}

	SAFE_DELETE (pDst);
	pDst = (PCInfoSkillBase)LibInfo.GetNew (m_pInfo->m_nTypeMain, m_pInfo->m_nTypeSub);
	pDst->Copy (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillBase::SetModify								 */
/* 内容		:編集モードとして設定											 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillBase::SetModify(CInfoSkillBase *pSrc)
{
	CLibInfoSkill LibInfo;

	SAFE_DELETE (m_pInfo);
	m_pInfo = (PCInfoSkillBase)LibInfo.GetNew (pSrc->m_nTypeMain, pSrc->m_nTypeSub);
	m_pInfo->Copy (pSrc);

	m_strName	= m_pInfo->m_strName;
	m_dwSP		= m_pInfo->m_dwSP;
	m_nTypeMain	= m_pInfo->m_nTypeMain;
	m_nTypeSub	= m_pInfo->m_nTypeSub;
	m_nUse		= m_pInfo->m_nUse;

	m_bModeModify = TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillBase::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

BOOL CDlgAdminCharSkillBase::OnInitDialog()
{
	int i, nCount, nNo, nTmp;

	CDlgAdminBase::OnInitDialog();

	RegisterControl (IDC_FRAME, LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl (IDOK,		LH_CTRL_X | LH_CTRL_Y);
	RegisterControl (IDCANCEL,	LH_CTRL_X | LH_CTRL_Y);

	if (m_bModeModify) {
		SetWindowText ("スキルの編集");
	}

	m_ctlIconGrp.Create (this, m_pMgrData);
	m_ctlIconGrp.Init (GRPIDMAIN_ICON32);

	m_ctlTypeMain.InsertString (0, "能力");
	m_ctlTypeMain.SetItemData (0, 0);
	m_ctlTypeMain.InsertString (1, "戦闘");
	m_ctlTypeMain.SetItemData (1, SKILLTYPEMAIN_BATTLE);
	m_ctlTypeMain.InsertString (2, "生活");
	m_ctlTypeMain.SetItemData (2, SKILLTYPEMAIN_LIFE);

	nNo = nTmp = 0;
	if (m_pInfo) {
		for (i = 0; i < SKILLTYPEMAIN_MAX; i ++) {
			if (m_pInfo->m_nTypeMain == m_ctlTypeMain.GetItemData (i)) {
				nNo = i;
				break;
			}
		}
		nTmp = m_pInfo->m_nTypeSub;
		m_ctlIconGrp.Set (m_pInfo->m_dwIconID);
	}
	m_ctlTypeMain.SetCurSel (nNo);
	OnSelchangeTypeMain ();

	nNo = 0;
	if (m_pInfo) {
		m_pInfo->m_nTypeSub = nTmp;
		nCount = m_ctlTypeSub.GetCount ();
		for (i = 0; i < nCount; i ++) {
			if (m_pInfo->m_nTypeSub == m_ctlTypeSub.GetItemData (i)) {
				nNo = i;
				break;
			}
		}
	}
	m_ctlTypeSub.SetCurSel (nNo);

	m_ctlUse.InsertString (0, "制限無し");
	m_ctlUse.SetItemData (0, SKILLUSE_ANY);
	m_ctlUse.InsertString (1, "通常時");
	m_ctlUse.SetItemData (1, SKILLUSE_NORMAL);
	m_ctlUse.InsertString (2, "戦闘モード時");
	m_ctlUse.SetItemData (2, SKILLUSE_BATTLE);

	nNo = 0;
	if (m_pInfo) {
		for (i = 0; i < SKILLUSE_MAX; i ++) {
			if (m_pInfo->m_nUse == m_ctlUse.GetItemData (i)) {
				nNo = i;
				break;
			}
		}
	}
	m_ctlUse.SetCurSel (nNo);

	OnSelchangeTypeSub ();
	OnSelchangeUse ();
	if (m_pDlgType && m_pInfo) {
		m_pDlgType->Set (m_pInfo);
	}
	m_bInit = FALSE;

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnAdminMsg										 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

LRESULT CDlgAdminCharSkillBase::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:		/* 左クリック通知 */
//		m_nPosX = HIWORD (lParam);
//		m_nPosY = LOWORD (lParam);
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
/* 関数名	:CDlgAdminCharSkillBase::PostNcDestroy							 */
/* 内容		:終了処理														 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillBase::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillBase::OnSelchangeTypeMain					 */
/* 内容		:イベントハンドラ(CBN_SELCHANGE)								 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillBase::OnSelchangeTypeMain()
{
	int nTypeMain;

	m_ctlTypeSub.ResetContent ();

	m_ctlTypeSub.InsertString (0, "未設定");
	m_ctlTypeSub.SetItemData (0, 0);

	nTypeMain = m_ctlTypeMain.GetItemData (m_ctlTypeMain.GetCurSel ());
	switch (nTypeMain) {
	case SKILLTYPEMAIN_NONE:			/* 能力 */
		break;
	case SKILLTYPEMAIN_BATTLE:			/* 戦闘 */
		m_ctlTypeSub.InsertString (1, "移動して攻撃");
		m_ctlTypeSub.SetItemData (1, SKILLTYPESUB_BATTLE_MOVEATACK);
		m_ctlTypeSub.InsertString (2, "回復");
		m_ctlTypeSub.SetItemData (2, SKILLTYPESUB_BATTLE_HEAL);
		break;
	case SKILLTYPEMAIN_LIFE:			/* 生活 */
		m_ctlTypeSub.InsertString (1, "釣り");
		m_ctlTypeSub.SetItemData (1, SKILLTYPESUB_LIFE_FISHING);
		break;
	}
	m_ctlTypeSub.SetCurSel (0);
	OnSelchangeTypeSub ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillBase::OnSelchangeTypeSub						 */
/* 内容		:イベントハンドラ(CBN_SELCHANGE)								 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillBase::OnSelchangeTypeSub()
{
	int nTypeMain, nTypeSub;
	CRect rc;
	PCInfoSkillBase pInfoTmp;
	CLibInfoSkill LibInfo;

	nTypeMain = m_ctlTypeMain.GetItemData (m_ctlTypeMain.GetCurSel ());
	nTypeSub  = m_ctlTypeSub. GetItemData (m_ctlTypeSub. GetCurSel ());

	if (m_pDlgType) {
		UnregisterControl (m_pDlgType->m_hWnd);
		m_pDlgType->DestroyWindow ();
		m_pDlgType = NULL;
	}

	switch (nTypeMain) {
	case SKILLTYPEMAIN_NONE:				/* 能力 */
		break;
	case SKILLTYPEMAIN_BATTLE:				/* 戦闘 */
		switch (nTypeSub) {
		case SKILLTYPESUB_BATTLE_MOVEATACK:		/* 移動して攻撃 */
			m_pDlgType = new CDlgAdminCharSkillMOVEATACK(this);
			break;
		case SKILLTYPESUB_BATTLE_HEAL:			/* 回復 */
			m_pDlgType = new CDlgAdminCharSkillHEAL(this);
			break;
		}
		break;
	case SKILLTYPEMAIN_LIFE:				/* 生活 */
		break;
	}
	if (m_pDlgType == NULL) {
		m_pDlgType = new CDlgAdminCharSkillNONE(this);
	}

	if (m_pDlgType) {
		m_pDlgType->Init (m_pMgrData);
		GetDlgItem (IDC_FRAME)->GetWindowRect (rc);
		ScreenToClient (rc);
		m_pDlgType->MoveWindow (rc.left, rc.top, rc.Width (), rc.Height ());
		RegisterControl (m_pDlgType->m_hWnd, LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	}

	if (m_bInit == FALSE) {
		if ((m_nTypeMain != nTypeMain) || (m_nTypeSub != nTypeSub)) {
			pInfoTmp = (PCInfoSkillBase)LibInfo.GetNew (nTypeMain, nTypeSub);
			if (m_pInfo) {
				pInfoTmp->m_dwSkillID = m_pInfo->m_dwSkillID;
			}

			SAFE_DELETE (m_pInfo);
			m_pInfo = pInfoTmp;
		}
	}
	m_nTypeMain = nTypeMain;
	m_nTypeSub  = nTypeSub;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillBase::OnSelchangeUse							 */
/* 内容		:イベントハンドラ(CBN_SELCHANGE)								 */
/* 日付		:2008/12/09														 */
/* ========================================================================= */

void CDlgAdminCharSkillBase::OnSelchangeUse()
{
	int nNo;

	nNo = m_ctlUse.GetCurSel ();
	if (nNo < 0) {
		return;
	}
	m_nUse = m_ctlUse.GetItemData (nNo);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillBase::OnOK									 */
/* 内容		:ボタンハンドラ(OK)												 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillBase::OnOK()
{
	UpdateData ();

	if (m_pInfo) {
		m_pInfo->m_strName	= m_strName;
		m_pInfo->m_dwSP		= m_dwSP;
		m_pInfo->m_nUse		= m_nUse;
		if (m_pDlgType) {
			m_pDlgType->Get (m_pInfo);
		}
		m_pInfo->m_dwIconID = m_ctlIconGrp.GetIDSub ();
	}

	CDlgAdminBase::EndDialog (IDOK);
}

/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillBase::OnLButtonDown							 */
/* 内容		:メッセージハンドラ(WM_LBUTTONDOWN)								 */
/* 日付		:2009/05/04														 */
/* ========================================================================= */

void CDlgAdminCharSkillBase::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bResult;
	CRect rc;
	CPoint ptTmp;

	if (m_pWndSelectGrp) {
		goto Exit;
	}

	ptTmp = point;
	ClientToScreen (&ptTmp);

	GetDlgItem (IDC_ICONGRP)->GetWindowRect (rc);
	bResult = rc.PtInRect (ptTmp);
	if (bResult == FALSE) {
		goto Exit;
	}
	m_pWndSelectGrp = new CWndSelectGrp;
	m_pWndSelectGrp->Create (this, m_pMgrData, 0);
	m_pWndSelectGrp->SetType (GRPIDMAIN_ICON32, m_ctlIconGrp.GetIDSub ());
	m_pWndSelectGrp->ShowWindow (SW_SHOW);

Exit:
	CDlgAdminBase::OnLButtonDown (nFlags, point);
}

/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillBase::OnWndClose								 */
/* 内容		:メッセージハンドラ(WM_WNDCLOSE)								 */
/* 日付		:2009/05/04														 */
/* ========================================================================= */

LRESULT CDlgAdminCharSkillBase::OnWndClose(WPARAM wParam, LPARAM lParam)
{
	int nHi  = HIWORD (lParam);
	int nLow = LOWORD (lParam);

	switch (wParam) {
	case WINDOWID_SELECTMAPPARTSGRP:	/* 画像選択 */
		m_pWndSelectGrp = NULL;
		if (nLow == 0xFFFF) {
			break;
		}
		m_ctlIconGrp.Set (nLow - 1);
		break;
	}

	return -1;
}

/* Copyright(C)URARA-works 2008 */
