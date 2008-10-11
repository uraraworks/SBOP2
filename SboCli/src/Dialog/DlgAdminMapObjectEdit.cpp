/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapObjectEdit.cpp									 */
/* 内容			:マップパーツ編集ダイアログクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/09/07													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "Img32.h"
#include "MgrGrpData.h"
#include "MgrData.h"
#include "DlgAdminMapObjectEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラス設定																 */
/* ========================================================================= */

void CDlgAdminMapObjectEdit::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapObjectEdit)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapObjectEdit, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapObjectEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::CDlgAdminMapObjectEdit					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

CDlgAdminMapObjectEdit::CDlgAdminMapObjectEdit(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapObjectEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapObjectEdit)
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::~CDlgAdminMapObjectEdit				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

CDlgAdminMapObjectEdit::~CDlgAdminMapObjectEdit()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::DoModal								 */
/* 内容		:モーダルダイアログ表示											 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

int CDlgAdminMapObjectEdit::DoModal(CMgrData *pMgrData)
{
	m_pMgrData		= pMgrData;

	return CDlgAdminBase::DoModal ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

BOOL CDlgAdminMapObjectEdit::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::OnOK									 */
/* 内容		:ボタンハンドラ(OK)												 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

void CDlgAdminMapObjectEdit::OnOK()
{
	CDlgAdminBase::OnOK();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectEdit::PostNcDestroy							 */
/* 内容		:終了処理														 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

void CDlgAdminMapObjectEdit::PostNcDestroy()
{
}

/* Copyright(C)URARA-works 2008 */
