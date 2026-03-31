/// @file DlgAdminTalkSet.cpp
/// @brief 会話データ設定ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/11/22
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "MgrData.h"
#include "LayoutHelper.h"
#include "InfoTalkEvent.h"
#include "DlgAdminTalkEventSet.h"
#include "DlgAdminTalkSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminTalkSet::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkSet)
	DDX_Text(pDX, IDC_TALK, m_strTalk);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkSet, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTalkSet)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_TALKEVENT, &CDlgAdminTalkSet::OnBnClickedTalkevent)
END_MESSAGE_MAP()

CDlgAdminTalkSet::CDlgAdminTalkSet(CWnd* pParent)
	: CDlgAdminBase(CDlgAdminTalkSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkSet)
	m_strTalk = _T("");
	//}}AFX_DATA_INIT

	m_pInfo = NULL;
	m_pInfo = new CInfoTalkEvent;
}

CDlgAdminTalkSet::~CDlgAdminTalkSet()
{
	SAFE_DELETE(m_pInfo);
}

BOOL CDlgAdminTalkSet::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	GetDlgItem(IDCANCEL)->SetFocus();

	RegisterControl(IDC_TALK,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl(IDOK,	LH_CTRL_X | LH_CTRL_Y);
	RegisterControl(IDCANCEL,	LH_CTRL_X | LH_CTRL_Y);

	return FALSE;
}

void CDlgAdminTalkSet::PostNcDestroy()
{
	CDialog::PostNcDestroy();
}

void CDlgAdminTalkSet::OnOK()
{
	UpdateData();

	CDialog::OnOK();
}

void CDlgAdminTalkSet::OnBnClickedTalkevent()
{
	int nResult;
	CDlgAdminTalkEventSet Dlg(this);

	Dlg.Init(m_pMgrData, m_pInfo);
	nResult = Dlg.DoModal();
	if (nResult != IDOK) {
		return;
	}
	Dlg.Get(m_pInfo);
}

