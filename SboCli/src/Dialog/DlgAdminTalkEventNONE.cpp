/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkEventNONE.cpp									 */
/* 内容			:会話イベント設定(未選択)ダイアログクラス 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventNONE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminTalkEventNONE::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventNONE)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventNONE, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventNONE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventNONE::CDlgAdminTalkEventNONE					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventNONE::CDlgAdminTalkEventNONE(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminTalkEventNONE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventNONE)
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventNONE::~CDlgAdminTalkEventNONE				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventNONE::~CDlgAdminTalkEventNONE()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventNONE::Init									 */
/* 内容		:初期化															 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventNONE::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* ウィンドウ作成 */
	Create (CDlgAdminTalkEventNONE::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventNONE::OnAdminMsg								 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventNONE::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventNONE::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventNONE::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	return TRUE;
}

/* Copyright(C)URARA-works 2008 */
