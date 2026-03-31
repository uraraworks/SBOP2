/// @file DlgAdminSelectEffect.cpp
/// @brief エフェクト選択ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/08/11
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "MgrData.h"
#include "DlgAdminSelectEffect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminSelectEffect::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminSelectEffect)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminSelectEffect, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminSelectEffect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDlgAdminSelectEffect::CDlgAdminSelectEffect(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminSelectEffect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminSelectEffect)
	//}}AFX_DATA_INIT

	m_dwSelectID = 0;
}


CDlgAdminSelectEffect::~CDlgAdminSelectEffect()
{
}


DWORD CDlgAdminSelectEffect::GetSelectID()
{
	return m_dwSelectID;
}


BOOL CDlgAdminSelectEffect::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.Create(this, m_pMgrData);

	RegisterControl(IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl(IDOK,	LH_CTRL_X | LH_CTRL_Y);
	RegisterControl(IDCANCEL,	LH_CTRL_X | LH_CTRL_Y);

	return TRUE;
}


void CDlgAdminSelectEffect::PostNcDestroy()
{
	CDialog::PostNcDestroy();
}


void CDlgAdminSelectEffect::OnOK()
{
	int nNo;

	nNo = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (nNo < 0) {
		goto Exit;
	}

	m_dwSelectID = m_List.GetItemData(nNo);

Exit:
	CDlgAdminBase::EndDialog(IDOK);
}