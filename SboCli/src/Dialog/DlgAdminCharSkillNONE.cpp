/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharSkillNONE.cpp									 */
/* 内容			:スキル(未設定)ダイアログクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/07													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoMapEventBase.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminCharSkillNONE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminCharSkillNONE::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharSkillNONE)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharSkillNONE, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharSkillNONE)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillNONE::CDlgAdminCharSkillNONE					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

CDlgAdminCharSkillNONE::CDlgAdminCharSkillNONE(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharSkillNONE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharSkillNONE)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminCharSkillNONE::IDD;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillNONE::~CDlgAdminCharSkillNONE				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

CDlgAdminCharSkillNONE::~CDlgAdminCharSkillNONE()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillNONE::Init									 */
/* 内容		:初期化															 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillNONE::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* ウィンドウ作成 */
	Create (m_nResourceID, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillNONE::Set									 */
/* 内容		:設定															 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillNONE::Set(CInfoSkillBase *pSrc)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillNONE::Get									 */
/* 内容		:取得															 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

void CDlgAdminCharSkillNONE::Get(CInfoSkillBase *pDst)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillNONE::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

BOOL CDlgAdminCharSkillNONE::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharSkillNONE::OnAdminMsg								 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2008/06/28														 */
/* ========================================================================= */

LRESULT CDlgAdminCharSkillNONE::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	return -1;
}

/* Copyright(C)URARA-works 2008 */
