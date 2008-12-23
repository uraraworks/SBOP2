/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkEventMSG.cpp									 */
/* 内容			:会話イベント設定(メッセージ表示)ダイアログクラス 実装ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventMSG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminTalkEventMSG::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventMSG)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventMSG, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventMSG)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMSG::CDlgAdminTalkEventMSG					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventMSG::CDlgAdminTalkEventMSG(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminTalkEventMSG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventMSG)
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMSG::~CDlgAdminTalkEventMSG				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventMSG::~CDlgAdminTalkEventMSG()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMSG::Init									 */
/* 内容		:初期化															 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventMSG::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* ウィンドウ作成 */
	Create (CDlgAdminTalkEventMSG::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMSG::OnAdminMsg								 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventMSG::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMSG::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventMSG::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	RegisterControl (IDC_MSG, LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	return TRUE;
}

/* Copyright(C)URARA-works 2008 */
