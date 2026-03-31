/// @file DlgAdminTalkEventSet.cpp
/// @brief 会話イベント設定ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/23
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "InfoTalkEvent.h"
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

// クラスの設定

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

CDlgAdminTalkEventSet::CDlgAdminTalkEventSet(CWnd* pParent)
	: CDlgAdminBase(CDlgAdminTalkEventSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventSet)
	//}}AFX_DATA_INIT

	m_pInfo	= NULL;
	m_pDlgTab	= NULL;
}

CDlgAdminTalkEventSet::~CDlgAdminTalkEventSet()
{
	SAFE_DELETE(m_pInfo);
}

void CDlgAdminTalkEventSet::Init(CMgrData *pMgrData, CInfoTalkEvent *pInfo/*=NULL*/)
{
	CDlgAdminBase::Init(pMgrData);

	m_pInfo = new CInfoTalkEvent;
	if (pInfo) {
		m_pInfo->Copy(pInfo);
	}
}

void CDlgAdminTalkEventSet::OnAdminMsg(int nType, DWORD dwPara)
{
}

void CDlgAdminTalkEventSet::Get(CInfoTalkEvent *&pDst)
{
	SAFE_DELETE(pDst);

	pDst = new CInfoTalkEvent;
	pDst->Copy(m_pInfo);
}

BOOL CDlgAdminTalkEventSet::OnInitDialog()
{
	CRect rc;

	CDlgAdminBase::OnInitDialog();

	Renew();

	m_Tab.GetWindowRect(rc);
	m_Tab.AdjustRect(FALSE, rc);
	ScreenToClient(rc);

	m_pDlgTab = new CDlgAdminTalkEventTab(this);
	m_pDlgTab->Init(m_pMgrData, m_pInfo);
	m_pDlgTab->MoveWindow(rc.left, rc.top, rc.Width(), rc.Height());
	m_Tab.SetWindowPos(m_pDlgTab, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	RegisterControl(IDC_ADD,	LH_CTRL_X);
	RegisterControl(IDC_DEL,	LH_CTRL_X);
	RegisterControl(IDC_TAB,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl(m_pDlgTab->m_hWnd,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl(IDOK,	LH_CTRL_X | LH_CTRL_Y);
	RegisterControl(IDCANCEL,	LH_CTRL_X | LH_CTRL_Y);

	return TRUE;
}

void CDlgAdminTalkEventSet::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nPage;

	*pResult = 0;

	nPage = m_Tab.GetCurSel();
	m_pDlgTab->SetPage(nPage);
}

void CDlgAdminTalkEventSet::PostNcDestroy()
{
	CDialog::PostNcDestroy();
}

void CDlgAdminTalkEventSet::OnBnClickedAdd()
{
	AddPage();
}

void CDlgAdminTalkEventSet::OnBnClickedDel()
{
}

void CDlgAdminTalkEventSet::Renew(void)
{
	int i, nCount;
	CString strTmp;

	m_Tab.DeleteAllItems();

	nCount = m_pInfo->GetPageCount();
	for (i = 0; i < nCount; i ++) {
		strTmp.Format(_T("ページ%d"), i + 1);
		m_Tab.InsertItem(nCount, strTmp);
	}
	if (nCount <= 0) {
		AddPage();
	}
}

void CDlgAdminTalkEventSet::AddPage()
{
	int nCount;
	CString strTmp;

	nCount = m_pInfo->GetPageCount();
	strTmp.Format(_T("ページ%d"), nCount + 1);
	m_Tab.InsertItem(nCount, strTmp);
}

