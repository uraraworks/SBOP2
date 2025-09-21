/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapEventNONE.cpp									 */
/* 内容			:マップイベント(未設定)ダイアログクラス 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/06/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoMapEventBase.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminMapEventNONE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminMapEventNONE::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapEventNONE)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapEventNONE, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapEventNONE)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventNONE::CDlgAdminMapEventNONE					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/06/26														 */
/* ========================================================================= */

CDlgAdminMapEventNONE::CDlgAdminMapEventNONE(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapEventNONE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapEventNONE)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminMapEventNONE::IDD;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventNONE::~CDlgAdminMapEventNONE					 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/06/26														 */
/* ========================================================================= */

CDlgAdminMapEventNONE::~CDlgAdminMapEventNONE()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventNONE::Init									 */
/* 内容		:初期化															 */
/* 日付		:2008/06/26														 */
/* ========================================================================= */

void CDlgAdminMapEventNONE::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* ウィンドウ作成 */
	Create (m_nResourceID, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventNONE::Set										 */
/* 内容		:設定															 */
/* 日付		:2008/06/26														 */
/* ========================================================================= */

void CDlgAdminMapEventNONE::Set(CInfoMapEventBase *pSrc)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventNONE::Get										 */
/* 内容		:取得															 */
/* 日付		:2008/06/26														 */
/* ========================================================================= */

void CDlgAdminMapEventNONE::Get(CInfoMapEventBase *pDst)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventNONE::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/06/26														 */
/* ========================================================================= */

BOOL CDlgAdminMapEventNONE::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapEventNONE::OnAdminMsg								 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2008/06/28														 */
/* ========================================================================= */

LRESULT CDlgAdminMapEventNONE::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	return -1;
}

/* Copyright(C)URARA-works 2008 */
