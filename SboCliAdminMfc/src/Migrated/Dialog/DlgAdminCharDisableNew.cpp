/// @file DlgAdminCharDisableNew.cpp
/// @brief 拒否MACアドレスの追加ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/04/11
/// @copyright Copyright(C)URARA-works 2009

#include "stdafx.h"
#include "resource.h"
#include "DlgAdminCharDisableNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminCharDisableNew::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharDisableNew)
	DDX_Text(pDX, IDC_MACADDRESS, m_strMacAddress);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharDisableNew, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharDisableNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDlgAdminCharDisableNew::CDlgAdminCharDisableNew(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharDisableNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharDisableNew)
	m_strMacAddress = _T("");
	//}}AFX_DATA_INIT
}


CDlgAdminCharDisableNew::~CDlgAdminCharDisableNew()
{
}


void CDlgAdminCharDisableNew::PostNcDestroy()
{
	CDialog::PostNcDestroy();
}


void CDlgAdminCharDisableNew::OnOK()
{
	UpdateData();

	CDlgAdminBase::EndDialog(IDOK);
}