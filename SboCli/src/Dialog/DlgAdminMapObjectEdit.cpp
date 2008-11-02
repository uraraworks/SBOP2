/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapObjectEdit.cpp									 */
/* 内容			:マップパーツ編集ダイアログクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/09/07													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoMapObject.h"
#include "Img32.h"
#include "MgrGrpData.h"
#include "MgrData.h"
#include "WndMapPartsGrp.h"
#include "DlgAdminMapObjectEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラス設定																 */
/* ========================================================================= */

void CDlgAdminMapObjectEdit::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapObjectEdit)
	DDX_Control(pDX, IDC_SLIDER, m_Slider);
	DDX_CBIndex(pDX, IDC_GRPNO, m_nGrpNo);
	DDX_Control(pDX, IDC_GRPNO, m_cbGrpNo);
	DDX_Control(pDX, IDC_OBJECT, m_ctlObject);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_CX, m_nCx);
	DDX_Text(pDX, IDC_CY, m_nCy);
	DDX_Text(pDX, IDC_COUNT, m_nCount);
	DDX_Control(pDX, IDC_SPIN_X0, m_ctlSpinX);
	DDX_Control(pDX, IDC_SPIN_Y0, m_ctlSpinY);
	DDX_Text(pDX, IDC_NOW, m_nNow);
}

BEGIN_MESSAGE_MAP(CDlgAdminMapObjectEdit, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapObjectEdit)
	ON_CBN_SELCHANGE(IDC_GRPNO, OnSelchangeGrpNo)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_CX, &CDlgAdminMapObjectEdit::OnEnChangeCx)
	ON_EN_CHANGE(IDC_CY, &CDlgAdminMapObjectEdit::OnEnChangeCy)
	ON_BN_CLICKED(IDC_ADD, &CDlgAdminMapObjectEdit::OnBnClickedAdd)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::CDlgAdminMapObjectEdit					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

CDlgAdminMapObjectEdit::CDlgAdminMapObjectEdit(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapObjectEdit::IDD, pParent)
	, m_strName(_T(""))
	, m_nCx(0)
	, m_nCy(0)
	, m_nCount(0)
	, m_nNow(0)
{
	//{{AFX_DATA_INIT(CDlgAdminMapObjectEdit)
	m_nGrpNo = 0;
	//}}AFX_DATA_INIT

	m_pWndMapPartsGrp = new CWndMapPartsGrp;
	m_pInfoMapObject  = new CInfoMapObject;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::~CDlgAdminMapObjectEdit				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

CDlgAdminMapObjectEdit::~CDlgAdminMapObjectEdit()
{
	SAFE_DELETE (m_pInfoMapObject);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::SetData								 */
/* 内容		:編集用にデータを設定											 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CDlgAdminMapObjectEdit::SetData(CInfoMapObject *pSrc)
{
	m_pInfoMapObject->Copy (pSrc);
	Set ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::GetData								 */
/* 内容		:編集したデータを取得											 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CDlgAdminMapObjectEdit::GetData(CInfoMapObject *pDst)
{
	pDst->Copy (m_pInfoMapObject);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

BOOL CDlgAdminMapObjectEdit::OnInitDialog()
{
	int i, nCount;
	CString strTmp;

	CDlgAdminBase::OnInitDialog();

	m_ctlSpinX.SetRange (1, 15);
	m_ctlSpinY.SetRange (1, 13);

	m_pWndMapPartsGrp->Create (this, m_pMgrData, IDC_PARTS);
	m_ctlObject.Create (this, m_pMgrData);
	m_ctlObject.Init (m_pInfoMapObject);

	RegisterControl (IDC_SLIDER, LH_CTRL_WIDTH);
	RegisterControl (IDOK,		 LH_CTRL_X | LH_CTRL_Y);
	RegisterControl (IDCANCEL,	 LH_CTRL_X | LH_CTRL_Y);
	RegisterControl (m_pWndMapPartsGrp->GetSafeHwnd (), LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	nCount = m_pMgrData->GetMgrGrpData ()->GetMapPartsCount () / 1024;
	for (i = 0; i < nCount; i ++) {
		strTmp.Format ("画像 %d 枚目", i + 1);
		m_cbGrpNo.AddString (strTmp);
	}
	m_nGrpNo = m_pMgrData->GetLastSelectGrpNo ();
	m_nGrpNo = min (m_nGrpNo, nCount - 1);
	m_cbGrpNo.SetCurSel (m_nGrpNo);
	OnSelchangeGrpNo ();
	OnSelectAnime ();

	Set ();

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::OnSelchangeGrpNo						 */
/* 内容		:イベントハンドラ(CBN_SELCHANGE)								 */
/* 日付		:2008/10/30														 */
/* ========================================================================= */

void CDlgAdminMapObjectEdit::OnSelchangeGrpNo()
{
	UpdateData ();

	m_pWndMapPartsGrp->SetNo (m_nGrpNo);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::OnHScroll								 */
/* 内容		:メッセージハンドラ(WM_HSCROLL)									 */
/* 日付		:2008/11/02														 */
/* ========================================================================= */

void CDlgAdminMapObjectEdit::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	OnSelectAnime ();

	CDlgAdminBase::OnHScroll (nSBCode, nPos, pScrollBar);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::OnOK									 */
/* 内容		:ボタンハンドラ(OK)												 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

void CDlgAdminMapObjectEdit::OnOK()
{
	Get ();

	CDlgAdminBase::OnOK();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::OnAdminMsg								 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2008/11/02														 */
/* ========================================================================= */

LRESULT CDlgAdminMapObjectEdit::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	int nNo;

	switch (wParam) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:		/* 左クリック通知 */
		nNo = m_Slider.GetPos ();
		m_pInfoMapObject->SetGrpID (nNo, m_ctlObject.m_ptSelect.x, m_ctlObject.m_ptSelect.y, (WORD)lParam);
		m_ctlObject.IncPosX ();
		break;
	}

	return 0;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::PostNcDestroy							 */
/* 内容		:終了処理														 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

void CDlgAdminMapObjectEdit::PostNcDestroy()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::OnEnChangeCx							 */
/* 内容		:イベントハンドラ(EN_CHANGE)									 */
/* 日付		:2008/11/02														 */
/* ========================================================================= */

void CDlgAdminMapObjectEdit::OnEnChangeCx()
{
	if (IsWindowVisible () == FALSE) {
		return;
	}

	Get ();
	m_ctlObject.Set (0);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::OnEnChangeCx							 */
/* 内容		:イベントハンドラ(EN_CHANGE)									 */
/* 日付		:2008/11/02														 */
/* ========================================================================= */

void CDlgAdminMapObjectEdit::OnEnChangeCy()
{
	if (IsWindowVisible () == FALSE) {
		return;
	}

	Get ();
	m_ctlObject.Set (0);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::OnBnClickedAdd							 */
/* 内容		:ボタンハンドラ(追加)											 */
/* 日付		:2008/11/02														 */
/* ========================================================================= */

void CDlgAdminMapObjectEdit::OnBnClickedAdd()
{
	Get ();
	m_pInfoMapObject->AddAnime ();
	Set ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::Set									 */
/* 内容		:画面に反映														 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CDlgAdminMapObjectEdit::Set(void)
{
	m_strName	= m_pInfoMapObject->m_strName;
	m_nCx		= m_pInfoMapObject->m_sizeGrp.cx;
	m_nCy		= m_pInfoMapObject->m_sizeGrp.cy;
	m_nCount	= m_pInfoMapObject->GetAnimeCount ();

	if (m_hWnd) {
		m_Slider.SetRange (0, m_nCount - 1, TRUE);
		UpdateData (FALSE);
		m_ctlObject.Set (0);
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::Get									 */
/* 内容		:画面から取得													 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CDlgAdminMapObjectEdit::Get(void)
{
	UpdateData ();

	m_pInfoMapObject->RenewGrpSize (m_nCx, m_nCy);
	m_pInfoMapObject->m_strName		= m_strName;
	m_pInfoMapObject->m_sizeGrp.cx	= m_nCx;
	m_pInfoMapObject->m_sizeGrp.cy	= m_nCy;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::OnSelectAnime							 */
/* 内容		:コマ選択処理													 */
/* 日付		:2008/11/02														 */
/* ========================================================================= */

void CDlgAdminMapObjectEdit::OnSelectAnime(void)
{
	int nNo;

	nNo = m_Slider.GetPos ();
	m_ctlObject.Set (nNo);
	m_nNow = nNo + 1;

	UpdateData (FALSE);
}

/* Copyright(C)URARA-works 2008 */
