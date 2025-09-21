/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkEventMENUSet.cpp								 */
/* 内容			:会話イベント選択項目の設定ダイアログクラス 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/28													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventMENUSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminTalkEventMENUSet::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventMENUSet)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Control(pDX, IDC_COMBO, m_Combo);
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventMENUSet, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventMENUSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMENUSet::CDlgAdminTalkEventMENUSet			 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

CDlgAdminTalkEventMENUSet::CDlgAdminTalkEventMENUSet(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminTalkEventMENUSet::IDD, pParent)
	, m_strName(_T(""))
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventMENUSet)
	//}}AFX_DATA_INIT

	m_nPage		 = 0;
	m_nPageCount = 0;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMENUSet::~CDlgAdminTalkEventMENUSet			 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

CDlgAdminTalkEventMENUSet::~CDlgAdminTalkEventMENUSet()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMENUSet::Init								 */
/* 内容		:初期化															 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

void CDlgAdminTalkEventMENUSet::Init(CMgrData *pMgrData, int nPage)
{
	CDlgAdminBase::Init (pMgrData);

	m_nPageCount = nPage;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMENUSet::OnInitDialog						 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventMENUSet::OnInitDialog()
{
	int i;
	CString strTmp;

	CDlgAdminBase::OnInitDialog();

	for (i = 0; i < m_nPageCount; i ++) {
		strTmp.Format ("ページ%d", i + 1);
		m_Combo.InsertString (i, strTmp);
	}
	m_Combo.SetCurSel (m_nPage);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMENUSet::PostNcDestroy						 */
/* 内容		:終了処理														 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

void CDlgAdminTalkEventMENUSet::PostNcDestroy()
{
	/* モードレスダイアログ時はこの関数を消す */
	CDialog::PostNcDestroy ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMENUSet::OnOK								 */
/* 内容		:ボタンハンドラ(OK)												 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

void CDlgAdminTalkEventMENUSet::OnOK()
{
	UpdateData ();

	m_nPage = m_Combo.GetCurSel ();

	CDlgAdminBase::OnOK ();
}

/* Copyright(C)URARA-works 2008 */
