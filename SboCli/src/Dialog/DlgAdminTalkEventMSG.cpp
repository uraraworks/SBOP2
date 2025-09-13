/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkEventMSG.cpp									 */
/* 内容			:会話イベント設定(メッセージ表示)ダイアログクラス 実装ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoTalkEventBase.h"
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
	CDlgAdminTalkEventNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventMSG)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_MSG, m_strMsg);
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventMSG, CDlgAdminTalkEventNONE)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventMSG)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMSG::CDlgAdminTalkEventMSG					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventMSG::CDlgAdminTalkEventMSG(CWnd* pParent /*=NULL*/)
	: CDlgAdminTalkEventNONE(pParent)
	, m_strMsg(_T(""))
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventMSG)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminTalkEventMSG::IDD;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMSG::~CDlgAdminTalkEventMSG					 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventMSG::~CDlgAdminTalkEventMSG()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMSG::Set										 */
/* 内容		:設定から画面に反映												 */
/* 日付		:2008/12/26														 */
/* ========================================================================= */

void CDlgAdminTalkEventMSG::Set(CInfoTalkEventBase *pSrc)
{
	m_strMsg = pSrc->m_strText;

	UpdateData (FALSE);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMSG::Get										 */
/* 内容		:画面から設定に反映												 */
/* 日付		:2008/12/26														 */
/* ========================================================================= */

void CDlgAdminTalkEventMSG::Get(CInfoTalkEventBase *pDst)
{
	UpdateData ();

	pDst->m_strText = m_strMsg;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMSG::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventMSG::OnInitDialog()
{
	CDlgAdminTalkEventNONE::OnInitDialog();

	RegisterControl (IDC_MSG, LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	return TRUE;
}

/* Copyright(C)URARA-works 2008 */
