/// @file DlgAdminSelectItem.cpp
/// @brief アイテム選択ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/24
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "resource.h"
#include "InfoItem.h"
#include "MgrGrpData.h"
#include "MgrData.h"
#include "DlgAdminSelectItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminSelectItem::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminSelectItem)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminSelectItem, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminSelectItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDlgAdminSelectItem::CDlgAdminSelectItem(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminSelectItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminSelectItem)
	//}}AFX_DATA_INIT

	m_dwSelectItemID = 0;
}


CDlgAdminSelectItem::~CDlgAdminSelectItem()
{
}


DWORD CDlgAdminSelectItem::GetSelectItemID()
{
	return m_dwSelectItemID;
}


BOOL CDlgAdminSelectItem::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.Create(this, m_pMgrData);

	RegisterControl(IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl(IDOK,	LH_CTRL_X | LH_CTRL_Y);
	RegisterControl(IDCANCEL,	LH_CTRL_X | LH_CTRL_Y);

	return TRUE;
}


void CDlgAdminSelectItem::PostNcDestroy()
{
	CDialog::PostNcDestroy();
}


void CDlgAdminSelectItem::OnOK()
{
	int nNo;

	nNo = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (nNo < 0) {
		goto Exit;
	}

	m_dwSelectItemID = m_List.GetItemData(nNo);

Exit:
	CDlgAdminBase::EndDialog(IDOK);
}