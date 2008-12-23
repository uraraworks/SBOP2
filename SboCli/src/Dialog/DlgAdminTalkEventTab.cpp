/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkEventTab.cpp									 */
/* 内容			:会話イベントタブダイアログクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventBase.h"
#include "DlgAdminTalkEventTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminTalkEventTab::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventTab)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST, m_List);
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventTab, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventTab)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADD, &CDlgAdminTalkEventTab::OnBnClickedAdd)
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventTab::CDlgAdminTalkEventTab					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventTab::CDlgAdminTalkEventTab(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminTalkEventTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventTab)
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventTab::~CDlgAdminTalkEventTab					 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventTab::~CDlgAdminTalkEventTab()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventTab::Init									 */
/* 内容		:初期化															 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventTab::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* ウィンドウ作成 */
	Create (CDlgAdminTalkEventTab::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventTab::OnAdminMsg								 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventTab::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventTab::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventTab::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	RegisterControl (IDC_LIST,		LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl (IDC_ADD,		LH_CTRL_Y);
	RegisterControl (IDC_MODIFY,	LH_CTRL_Y);
	RegisterControl (IDC_DEL,		LH_CTRL_Y);
	RegisterControl (IDC_UP,		LH_CTRL_X | LH_CTRL_Y);
	RegisterControl (IDC_DOWN,		LH_CTRL_X | LH_CTRL_Y);

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (0, "イベント種別", 0, 100);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventTab::OnBnClickedAdd							 */
/* 内容		:ボタンハンドラ(追加)											 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventTab::OnBnClickedAdd()
{
	int nResult;

	CDlgAdminTalkEventBase Dlg(this);

	Dlg.Init (m_pMgrData);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}
}

/* Copyright(C)URARA-works 2008 */
