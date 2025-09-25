/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkEventSet.cpp									 */
/* 内容			:会話イベント設定ダイアログクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoTalkEvent.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventTab.h"
#include "DlgAdminTalkEventSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminTalkEventSet::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventSet)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TAB, m_Tab);
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventSet, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventSet)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CDlgAdminTalkEventSet::OnTcnSelchangeTab)
	ON_BN_CLICKED(IDC_ADD, &CDlgAdminTalkEventSet::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_DEL, &CDlgAdminTalkEventSet::OnBnClickedDel)
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventSet::CDlgAdminTalkEventSet					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventSet::CDlgAdminTalkEventSet(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminTalkEventSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventSet)
	//}}AFX_DATA_INIT

	m_pInfo		= NULL;
	m_pDlgTab	= NULL;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventSet::~CDlgAdminTalkEventSet					 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventSet::~CDlgAdminTalkEventSet()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventSet::Init									 */
/* 内容		:初期化															 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventSet::Init(CMgrData *pMgrData, CInfoTalkEvent *pInfo/*NULL*/)
{
	CDlgAdminBase::Init (pMgrData);

	m_pInfo = new CInfoTalkEvent;
	if (pInfo) {
		m_pInfo->Copy (pInfo);
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventSet::OnAdminMsg								 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventSet::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventSet::Get										 */
/* 内容		:取得															 */
/* 日付		:2008/12/26														 */
/* ========================================================================= */

void CDlgAdminTalkEventSet::Get(CInfoTalkEvent *&pDst)
{
	SAFE_DELETE (pDst);

	pDst = new CInfoTalkEvent;
	pDst->Copy (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventSet::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventSet::OnInitDialog()
{
	CRect rc;

	CDlgAdminBase::OnInitDialog();

	Renew ();

	m_Tab.GetWindowRect (rc);
	m_Tab.AdjustRect (FALSE, rc);
	ScreenToClient (rc);

	m_pDlgTab = new CDlgAdminTalkEventTab(this);
	m_pDlgTab->Init (m_pMgrData, m_pInfo);
	m_pDlgTab->MoveWindow (rc.left, rc.top, rc.Width (), rc.Height ());
	m_Tab.SetWindowPos (m_pDlgTab, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	RegisterControl (IDC_ADD,	LH_CTRL_X);
	RegisterControl (IDC_DEL,	LH_CTRL_X);
	RegisterControl (IDC_TAB,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl (m_pDlgTab->m_hWnd,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl (IDOK,		LH_CTRL_X | LH_CTRL_Y);
	RegisterControl (IDCANCEL,	LH_CTRL_X | LH_CTRL_Y);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventSet::OnTcnSelchangeTab						 */
/* 内容		:イベントハンドラ(TCN_SELCHANGE)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventSet::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nPage;

	*pResult = 0;

	nPage = m_Tab.GetCurSel ();
	m_pDlgTab->SetPage (nPage);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventSet::PostNcDestroy							 */
/* 内容		:終了処理														 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventSet::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventSet::OnBnClickedAdd							 */
/* 内容		:ボタンハンドラ(ページ追加)										 */
/* 日付		:2008/12/24														 */
/* ========================================================================= */

void CDlgAdminTalkEventSet::OnBnClickedAdd()
{
	AddPage ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventSet::OnBnClickedDel							 */
/* 内容		:ボタンハンドラ(ページ削除)										 */
/* 日付		:2008/12/24														 */
/* ========================================================================= */

void CDlgAdminTalkEventSet::OnBnClickedDel()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventSet::Renew									 */
/* 内容		:更新															 */
/* 日付		:2008/12/24														 */
/* ========================================================================= */

void CDlgAdminTalkEventSet::Renew(void)
{
	int i, nCount;
	CString strTmp;

	m_Tab.DeleteAllItems ();

	nCount = m_pInfo->GetPageCount ();
	for (i = 0; i < nCount; i ++) {
		strTmp.Format(_T("ページ%d"), i + 1);
		m_Tab.InsertItem(nCount, strTmp);
	}
	if (nCount <= 0) {
		AddPage ();
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventSet::AddPage									 */
/* 内容		:ページの追加													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventSet::AddPage()
{
	int nCount;
	CString strTmp;

	nCount = m_pInfo->GetPageCount ();
	strTmp.Format(_T("ページ%d"), nCount + 1);
	m_Tab.InsertItem(nCount, strTmp);
}

/* Copyright(C)URARA-works 2008 */
