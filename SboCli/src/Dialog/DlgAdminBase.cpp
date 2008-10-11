/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminBase.h												 */
/* 内容			:管理者ウィンドウ基底ダイアログクラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/03/18													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "MgrData.h"
#include "DlgAdminBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminBase)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminBase, CDialog)
	//{{AFX_MSG_MAP(CDlgAdminBase)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminBase::CDlgAdminBase									 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

CDlgAdminBase::CDlgAdminBase(int nResourceID, CWnd* pParent /*=NULL*/)
	: CDialog(nResourceID, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminBase)
	//}}AFX_DATA_INIT

	m_pMgrData		= NULL;
	m_pWndParent	= pParent;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminBase::~CDlgAdminBase									 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

CDlgAdminBase::~CDlgAdminBase()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminBase::Init											 */
/* 内容		:初期化															 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminBase::Init(CMgrData *pMgrData)
{
	m_pMgrData	= pMgrData;
	m_pSock		= m_pMgrData->GetUraraSockTCP ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminBase::Renew											 */
/* 内容		:更新															 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminBase::Renew(void)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminBase::OnAdminMsg										 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminBase::OnAdminMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case ADMINMSG_ACCOUNTINFO:		/* アカウント情報取得 */
		m_pMgrData->Delete (dwPara);
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminBase::OnMainFrame										 */
/* 内容		:メッセージハンドラ(WM_MAINFRAME)								 */
/* 日付		:2007/08/16														 */
/* ========================================================================= */

void CDlgAdminBase::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminBase::OnInitDialog									 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

BOOL CDlgAdminBase::OnInitDialog()
{
	CDialog::OnInitDialog ();

	Initialize (m_hWnd);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminBase::OnSize											 */
/* 内容		:メッセージハンドラ(WM_SIZE)									 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminBase::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize (nType, cx, cy);

	DoLayout (cx, cy);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminBase::PostNcDestroy									 */
/* 内容		:終了処理														 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminBase::PostNcDestroy()
{
	delete this;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminBase::SelectCmb										 */
/* 内容		:項目を選択														 */
/* 日付		:2007/11/25														 */
/* ========================================================================= */

void CDlgAdminBase::SelectCmb(CComboBox *pCmb, DWORD dwID)
{
	int i, nCount, nNo;
	DWORD dwItemData;

	nNo = 0;

	nCount = pCmb->GetCount ();
	for (i = 0; i < nCount; i ++) {
		dwItemData = pCmb->GetItemData (i);
		if (dwItemData != dwID) {
			continue;
		}
		nNo = i;
		break;
	}
	pCmb->SetCurSel (nNo);
}

/* Copyright(C)URARA-works 2007 */
