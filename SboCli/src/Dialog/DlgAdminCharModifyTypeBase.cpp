/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharModifyTypeBase.cpp								 */
/* 内容			:キャラ情報 移動種別の設定ダイアログクラス 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/12													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "DlgAdminCharModifyTypeBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminCharModifyTypeBase::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharModifyTypeBase)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharModifyTypeBase, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharModifyTypeBase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyTypeBase::CDlgAdminCharModifyTypeBase		 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

CDlgAdminCharModifyTypeBase::CDlgAdminCharModifyTypeBase(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharModifyTypeBase::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharModifyTypeBase)
	//}}AFX_DATA_INIT
}
CDlgAdminCharModifyTypeBase::CDlgAdminCharModifyTypeBase(int nResourceID, CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(nResourceID, pParent)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyTypeBase::~CDlgAdminCharModifyTypeBase		 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

CDlgAdminCharModifyTypeBase::~CDlgAdminCharModifyTypeBase()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyTypeBase::Set								 */
/* 内容		:設定															 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CDlgAdminCharModifyTypeBase::Set(CInfoCharCli *pSrc)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyTypeBase::Get								 */
/* 内容		:取得															 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CDlgAdminCharModifyTypeBase::Get(CInfoCharCli *pDst)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyTypeBase::PostNcDestroy						 */
/* 内容		:終了処理														 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CDlgAdminCharModifyTypeBase::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}

/* Copyright(C)URARA-works 2008 */
