/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminSelectEffect.cpp									 */
/* 内容			:エフェクト選択ダイアログクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/11													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "MgrData.h"
#include "DlgAdminSelectEffect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

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


/* ========================================================================= */
/* 関数名	:CDlgAdminSelectEffect::CDlgAdminSelectEffect					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

CDlgAdminSelectEffect::CDlgAdminSelectEffect(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminSelectEffect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminSelectEffect)
	//}}AFX_DATA_INIT

	m_dwSelectID = 0;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminSelectEffect::~CDlgAdminSelectEffect					 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

CDlgAdminSelectEffect::~CDlgAdminSelectEffect()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminSelectEffect::GetSelectID								 */
/* 内容		:選択されたエフェクトIDを取得									 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

DWORD CDlgAdminSelectEffect::GetSelectID()
{
	return m_dwSelectID;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminSelectEffect::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

BOOL CDlgAdminSelectEffect::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.Create (this, m_pMgrData);

	RegisterControl (IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl (IDOK,		LH_CTRL_X | LH_CTRL_Y);
	RegisterControl (IDCANCEL,	LH_CTRL_X | LH_CTRL_Y);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminSelectEffect::PostNcDestroy							 */
/* 内容		:終了処理														 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

void CDlgAdminSelectEffect::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminSelectEffect::OnOK									 */
/* 内容		:ボタンハンドラ(OK)												 */
/* 日付		:2008/08/11														 */
/* ========================================================================= */

void CDlgAdminSelectEffect::OnOK()
{
	int nNo;

	nNo = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nNo < 0) {
		goto Exit;
	}

	m_dwSelectID = m_List.GetItemData (nNo);

Exit:
	CDlgAdminBase::EndDialog (IDOK);
}

/* Copyright(C)URARA-works 2008 */
