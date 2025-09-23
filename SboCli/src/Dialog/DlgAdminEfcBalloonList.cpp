/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminEfcBalloonList.cpp									 */
/* 内容			:噴出し一覧ダイアログクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/12/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "LibInfoEfcBalloon.h"
#include "PacketADMIN_EFC_RENEWBALLOON.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminEfcBalloon.h"
#include "DlgAdminEfcBalloonList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminEfcBalloonList::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminEfcBalloonList)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminEfcBalloonList, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminEfcBalloonList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_RENEW, OnRenew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloonList::CDlgAdminEfcBalloonList				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

CDlgAdminEfcBalloonList::CDlgAdminEfcBalloonList(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminEfcBalloonList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminEfcBalloonList)
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloonList::~CDlgAdminEfcBalloonList				 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

CDlgAdminEfcBalloonList::~CDlgAdminEfcBalloonList()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloonList::Init									 */
/* 内容		:初期化															 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CDlgAdminEfcBalloonList::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfo = m_pMgrData->GetLibInfoEfcBalloon ();

	/* ウィンドウ作成 */
	Create (CDlgAdminEfcBalloonList::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloonList::Renew									 */
/* 内容		:一覧を更新														 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CDlgAdminEfcBalloonList::Renew(void)
{
	int i, nCount;
	DWORD dwListID;
	ARRAYDWORD aIDList;
	CmyString strTmp;

	m_List.DeleteAllItems ();

	m_pLibInfo->GetListID (aIDList);
	nCount = aIDList.size();
	for (i = 0; i < nCount; i ++) {
		dwListID = aIDList[i];
		strTmp.Format ("%d", dwListID);
		m_List.InsertItem (i, strTmp);
		m_List.SetItemData (i, dwListID);
		m_pLibInfo->GetName (dwListID, strTmp);
		m_List.SetItemText (i, 1, (LPCTSTR)strTmp);
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloonList::OnAdminMsg							 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CDlgAdminEfcBalloonList::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloonList::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

BOOL CDlgAdminEfcBalloonList::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (0, "ID",			LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, "噴出し名",		LVCFMT_LEFT, 120);

	RegisterControl (IDC_RENEW,	LH_CTRL_X);
	RegisterControl (IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	Renew ();

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloonList::OnMainFrame							 */
/* 内容		:メッセージハンドラ(WM_MAINFRAME)								 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CDlgAdminEfcBalloonList::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWBALLOON:		/* 噴出し情報更新 */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloonList::OnAdd									 */
/* 内容		:ボタンハンドラ(新規追加)										 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CDlgAdminEfcBalloonList::OnAdd()
{
	int nResult;
	CDlgAdminEfcBalloon Dlg(this);
	CPacketADMIN_EFC_RENEWBALLOON Packet;

	Dlg.Init (m_pMgrData, 0);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}

	Dlg.GetList (m_pLibInfo);

	Packet.Make (Dlg.GetListID (), m_pLibInfo);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloonList::OnModify								 */
/* 内容		:ボタンハンドラ(編集)											 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CDlgAdminEfcBalloonList::OnModify()
{
	int nNo, nResult;
	CDlgAdminEfcBalloon Dlg(this);
	CPacketADMIN_EFC_RENEWBALLOON Packet;

	nNo = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}

	Dlg.Init (m_pMgrData, m_List.GetItemData (nNo), TRUE);
	Dlg.SetList (m_pLibInfo);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}

	Dlg.GetList (m_pLibInfo);

	Packet.Make (Dlg.GetListID (), m_pLibInfo);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloonList::OnDelete								 */
/* 内容		:ボタンハンドラ(削除)											 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CDlgAdminEfcBalloonList::OnDelete()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminEfcBalloonList::OnRenew								 */
/* 内容		:ボタンハンドラ(更新)											 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CDlgAdminEfcBalloonList::OnRenew()
{
}

/* Copyright(C)URARA-works 2007 */
