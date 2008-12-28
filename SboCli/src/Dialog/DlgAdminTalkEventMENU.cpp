/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkEventMENU.cpp									 */
/* 内容			:会話イベント設定(項目選択)ダイアログクラス 実装ファイル	 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "InfoTalkEventMENU.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventMENUSet.h"
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
	DDX_Control(pDX, IDC_LIST, m_List);
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventMENU, CDlgAdminTalkEventNONE)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventMENU)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADD, &CDlgAdminTalkEventMENU::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_MODIFY, &CDlgAdminTalkEventMENU::OnBnClickedModify)
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
/* 関数名	:CDlgAdminTalkEventMENU::Set									 */
/* 内容		:設定から画面に反映												 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

void CDlgAdminTalkEventMENU::Set(CInfoTalkEventBase *pSrc)
{
	int i, nCount;
	PCInfoTalkEventMENU pInfo;
	PSTTALKEVENTMENUINFO pMenuInfo;
	CString strTmp;

	m_List.DeleteAllItems ();

	pInfo = (PCInfoTalkEventMENU)pSrc;
	nCount = pInfo->GetMenuInfoCount ();
	for (i = 0; i < nCount; i ++) {
		pMenuInfo = pInfo->GetPtr (i);
		m_List.InsertItem (i, pMenuInfo->strName);
		strTmp.Format ("%d", pMenuInfo->nPage + 1);
		m_List.SetItemText (i, 1, strTmp);
	}

	UpdateData (FALSE);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMENU::Get									 */
/* 内容		:画面から設定に反映												 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

void CDlgAdminTalkEventMENU::Get(CInfoTalkEventBase *pDst)
{
	int i, nCount;
	PCInfoTalkEventMENU pInfo;

	pInfo = (PCInfoTalkEventMENU)pDst;
	pInfo->DeleteAllMenuInfo ();

	UpdateData ();

	nCount = m_List.GetItemCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo->AddMenuInfo (
				atoi (m_List.GetItemText (i, 1)) - 1,
				m_List.GetItemText (i, 0));
	}
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

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (0, "項目名", 0, 150);
	m_List.InsertColumn (1, "ジャンプ先", 0, 150);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMENU::OnBnClickedAdd							 */
/* 内容		:ボタンハンドラ(追加)											 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

void CDlgAdminTalkEventMENU::OnBnClickedAdd()
{
	int nResult, nCount;
	CString strTmp;
	CDlgAdminTalkEventMENUSet Dlg(this);

	Dlg.Init (m_pMgrData, m_nPageCount);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}

	nCount = m_List.GetItemCount ();
	m_List.InsertItem (nCount, Dlg.m_strName);
	strTmp.Format ("%d", Dlg.m_nPage + 1);
	m_List.SetItemText (nCount, 1, strTmp);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventMENU::OnBnClickedModify						 */
/* 内容		:ボタンハンドラ(編集)											 */
/* 日付		:2008/12/28														 */
/* ========================================================================= */

void CDlgAdminTalkEventMENU::OnBnClickedModify()
{
	int nResult, nSelect;
	CString strTmp;
	CDlgAdminTalkEventMENUSet Dlg(this);

	nSelect = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nSelect < 0) {
		return;
	}

	Dlg.Init (m_pMgrData, m_nPageCount);
	Dlg.m_strName	= m_List.GetItemText (nSelect, 0);
	Dlg.m_nPage		= atoi (m_List.GetItemText (nSelect, 1));
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}

	m_List.SetItemText (nSelect, 0, Dlg.m_strName);
	strTmp.Format ("%d", Dlg.m_nPage + 1);
	m_List.SetItemText (nSelect, 1, strTmp);
}

/* Copyright(C)URARA-works 2008 */
