/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTmp.cpp											 */
/* 内容			:ダイアログクラス 実装ファイル								 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTmp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminTmp::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTmp)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminTmp, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTmp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminTmp::CDlgAdminTmp										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTmp::CDlgAdminTmp(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminTmp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTmp)
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTmp::~CDlgAdminTmp									 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTmp::~CDlgAdminTmp()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTmp::Init												 */
/* 内容		:初期化															 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTmp::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* 以下はモードレスダイアログ用 */
	/* ウィンドウ作成 */
	Create (CDlgAdminTmp::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTmp::OnAdminMsg										 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTmp::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTmp::OnInitDialog										 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTmp::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTmp::PostNcDestroy									 */
/* 内容		:終了処理														 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTmp::PostNcDestroy()
{
	/* モードレスダイアログ時はこの関数を消す */
	CDialog::PostNcDestroy ();
}

/* Copyright(C)URARA-works 2008 */
