/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkEventMENU.cpp									 */
/* 内容			:会話イベント設定(項目選択)ダイアログクラス 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventMENU.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminTalkEventMENU::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminTalkEventNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventMENU)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventMENU, CDlgAdminTalkEventNONE)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventMENU)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMENU::CDlgAdminTalkEventMENU					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventMENU::CDlgAdminTalkEventMENU(CWnd* pParent /*=NULL*/)
	: CDlgAdminTalkEventNONE(pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventMENU)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminTalkEventMENU::IDD;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMENU::~CDlgAdminTalkEventMENU				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventMENU::~CDlgAdminTalkEventMENU()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMENU::Init									 */
/* 内容		:初期化															 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventMENU::Init(CMgrData *pMgrData)
{
	CDlgAdminTalkEventNONE::Init (pMgrData);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMENU::OnAdminMsg								 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventMENU::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMENU::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventMENU::OnInitDialog()
{
	CDlgAdminTalkEventNONE::OnInitDialog();

	RegisterControl (IDC_LIST,		LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl (IDC_ADD,		LH_CTRL_Y);
	RegisterControl (IDC_MODIFY,	LH_CTRL_Y);
	RegisterControl (IDC_DEL,		LH_CTRL_Y);

	return TRUE;
}

/* Copyright(C)URARA-works 2008 */
