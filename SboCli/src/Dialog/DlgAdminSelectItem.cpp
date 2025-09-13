/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminSelectItem.cpp										 */
/* 内容			:アイテム選択ダイアログクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/24													 */
/* ========================================================================= */

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

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

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


/* ========================================================================= */
/* 関数名	:CDlgAdminSelectItem::CDlgAdminSelectItem						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/09/24														 */
/* ========================================================================= */

CDlgAdminSelectItem::CDlgAdminSelectItem(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminSelectItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminSelectItem)
	//}}AFX_DATA_INIT

	m_dwSelectItemID = 0;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminSelectItem::~CDlgAdminSelectItem						 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/09/24														 */
/* ========================================================================= */

CDlgAdminSelectItem::~CDlgAdminSelectItem()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminSelectItem::GetSelectItemID							 */
/* 内容		:選択されたアイテムIDを取得										 */
/* 日付		:2007/09/24														 */
/* ========================================================================= */

DWORD CDlgAdminSelectItem::GetSelectItemID()
{
	return m_dwSelectItemID;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminSelectItem::OnInitDialog								 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2007/09/24														 */
/* ========================================================================= */

BOOL CDlgAdminSelectItem::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.Create (this, m_pMgrData);

	RegisterControl (IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl (IDOK,		LH_CTRL_X | LH_CTRL_Y);
	RegisterControl (IDCANCEL,	LH_CTRL_X | LH_CTRL_Y);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminSelectItem::PostNcDestroy								 */
/* 内容		:終了処理														 */
/* 日付		:2007/09/24														 */
/* ========================================================================= */

void CDlgAdminSelectItem::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminSelectItem::OnOK										 */
/* 内容		:ボタンハンドラ(OK)												 */
/* 日付		:2007/09/24														 */
/* ========================================================================= */

void CDlgAdminSelectItem::OnOK()
{
	int nNo;

	nNo = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nNo < 0) {
		goto Exit;
	}

	m_dwSelectItemID = m_List.GetItemData (nNo);

Exit:
	CDlgAdminBase::EndDialog (IDOK);
}

/* Copyright(C)URARA-works 2007 */
