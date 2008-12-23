/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkEventPAGE.cpp									 */
/* 内容			:会話イベント設定(ページ切り替え)ダイアログクラス 実装ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventPAGE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminTalkEventPAGE::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventPAGE)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventPAGE, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventPAGE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventPAGE::CDlgAdminTalkEventPAGE					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventPAGE::CDlgAdminTalkEventPAGE(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminTalkEventPAGE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventPAGE)
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventPAGE::~CDlgAdminTalkEventPAGE				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventPAGE::~CDlgAdminTalkEventPAGE()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventPAGE::Init									 */
/* 内容		:初期化															 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventPAGE::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* ウィンドウ作成 */
	Create (CDlgAdminTalkEventPAGE::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventPAGE::OnAdminMsg								 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventPAGE::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventPAGE::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventPAGE::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	return TRUE;
}

/* Copyright(C)URARA-works 2008 */
