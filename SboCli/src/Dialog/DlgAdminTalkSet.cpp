/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkSet.cpp										 */
/* 内容			:会話データ設定ダイアログクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/22													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "MgrData.h"
#include "DlgAdminTalkSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminTalkSet::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkSet)
	DDX_Text(pDX, IDC_TALK, m_strTalk);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkSet, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTalkSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkSet::CDlgAdminTalkSet								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

CDlgAdminTalkSet::CDlgAdminTalkSet(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminTalkSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkSet)
	m_strTalk = _T("");
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkSet::~CDlgAdminTalkSet							 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

CDlgAdminTalkSet::~CDlgAdminTalkSet()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkSet::PostNcDestroy								 */
/* 内容		:終了処理														 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

void CDlgAdminTalkSet::PostNcDestroy()
{
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkSet::OnOK											 */
/* 内容		:ボタンハンドラ(OK)												 */
/* 日付		:2008/11/22														 */
/* ========================================================================= */

void CDlgAdminTalkSet::OnOK()
{
	UpdateData ();

	CDialog::OnOK ();
}

/* Copyright(C)URARA-works 2008 */
