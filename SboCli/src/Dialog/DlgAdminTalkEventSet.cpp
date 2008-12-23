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

void CDlgAdminTalkEventSet::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pInfo = new CInfoTalkEvent;
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
/* 関数名	:CDlgAdminTalkEventSet::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventSet::OnInitDialog()
{
	CRect rc;

	CDlgAdminBase::OnInitDialog();

	AddPage ();
	m_Tab.GetWindowRect (rc);
	m_Tab.AdjustRect (FALSE, rc);
	ScreenToClient (rc);

	m_pDlgTab = new CDlgAdminTalkEventTab(this);
	m_pDlgTab->Init (m_pMgrData);
	m_pDlgTab->MoveWindow (rc.left, rc.top, rc.Width (), rc.Height ());

	RegisterControl (IDC_ADD,	LH_CTRL_X);
	RegisterControl (IDC_DEL,	LH_CTRL_X);
	RegisterControl (IDC_TAB,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl (m_pDlgTab->m_hWnd,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl (IDOK,		LH_CTRL_X | LH_CTRL_Y);
	RegisterControl (IDCANCEL,	LH_CTRL_X | LH_CTRL_Y);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventSet::AddPage									 */
/* 内容		:イベントハンドラ(TCN_SELCHANGE)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventSet::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
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
/* 関数名	:CDlgAdminTalkEventSet::AddPage									 */
/* 内容		:ページの追加													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventSet::AddPage()
{
	int nCount;
	PCInfoTalkEventBase pInfo;
	CString strTmp;

	nCount = m_pInfo->GetPageCount ();
	strTmp.Format ("ページ%d", nCount + 1);
	m_Tab.InsertItem(nCount, strTmp);

	pInfo = m_pInfo->GetNew (TALKEVENTTYPE_NONE);
	pInfo->m_nPage = nCount;
	m_pInfo->AddTalkEvent(pInfo);
}

/* Copyright(C)URARA-works 2008 */
