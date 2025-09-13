/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharDisableNew.cpp									 */
/* 内容			:拒否MACアドレスの追加ダイアログクラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/04/11													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "DlgAdminCharDisableNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

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


/* ========================================================================= */
/* 関数名	:CDlgAdminCharDisableNew::CDlgAdminCharDisableNew				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

CDlgAdminCharDisableNew::CDlgAdminCharDisableNew(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharDisableNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharDisableNew)
	m_strMacAddress = _T("");
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharDisableNew::~CDlgAdminCharDisableNew				 */
/* 内容		:デストラクタ													 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

CDlgAdminCharDisableNew::~CDlgAdminCharDisableNew()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharDisableNew::PostNcDestroy							 */
/* 内容		:終了処理														 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

void CDlgAdminCharDisableNew::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharDisableNew::OnOK									 */
/* 内容		:ボタンハンドラ(OK)												 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

void CDlgAdminCharDisableNew::OnOK()
{
	UpdateData ();

	CDlgAdminBase::EndDialog (IDOK);
}

/* Copyright(C)URARA-works 2009 */
