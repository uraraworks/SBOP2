/// @file DlgAdminTalkEventMENUSet.cpp
/// @brief 会話イベント選択項目の設定ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/28
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventMENUSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminTalkEventMENUSet::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventMENUSet)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Control(pDX, IDC_COMBO, m_Combo);
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventMENUSet, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventMENUSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDlgAdminTalkEventMENUSet::CDlgAdminTalkEventMENUSet(CWnd* pParent)
	: CDlgAdminBase(CDlgAdminTalkEventMENUSet::IDD, pParent)
	, m_strName(_T(""))
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventMENUSet)
	//}}AFX_DATA_INIT

	m_nPage	 = 0;
	m_nPageCount = 0;
}

CDlgAdminTalkEventMENUSet::~CDlgAdminTalkEventMENUSet()
{
}

void CDlgAdminTalkEventMENUSet::Init(CMgrData *pMgrData, int nPage)
{
	CDlgAdminBase::Init(pMgrData);

	m_nPageCount = nPage;
}

BOOL CDlgAdminTalkEventMENUSet::OnInitDialog()
{
	int i;
	CString strTmp;

	CDlgAdminBase::OnInitDialog();

	for (i = 0; i < m_nPageCount; i ++) {
		strTmp.Format(_T("ページ%d"), i + 1);
		m_Combo.InsertString(i, strTmp);
	}
	m_Combo.SetCurSel(m_nPage);

	return TRUE;
}

void CDlgAdminTalkEventMENUSet::PostNcDestroy()
{
	// モードレスダイアログ時はこの関数を消す
	CDialog::PostNcDestroy();
}

void CDlgAdminTalkEventMENUSet::OnOK()
{
	UpdateData();

	m_nPage = m_Combo.GetCurSel();

	CDlgAdminBase::OnOK();
}

