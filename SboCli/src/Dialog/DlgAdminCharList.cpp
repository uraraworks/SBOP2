/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharList.cpp										 */
/* 内容			:キャラ一覧ダイアログクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_PARA2.h"
#include "InfoCharCli.h"
#include "LibInfoCharCli.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminCharList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminCharList::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharList)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharList, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharList)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RENEW, &CDlgAdminCharList::OnBnClickedRenew)
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminCharList::CDlgAdminCharList							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/01														 */
/* ========================================================================= */

CDlgAdminCharList::CDlgAdminCharList(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharList)
	//}}AFX_DATA_INIT

	m_pWndNotify	= NULL;
	m_pLibInfoChar	= NULL;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharList::~CDlgAdminCharList							 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/01														 */
/* ========================================================================= */

CDlgAdminCharList::~CDlgAdminCharList()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharList::Init										 */
/* 内容		:初期化															 */
/* 日付		:2008/12/01														 */
/* ========================================================================= */

void CDlgAdminCharList::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfoChar = m_pMgrData->GetLibInfoCharOnline ();

	/* ウィンドウ作成 */
	Create (CDlgAdminCharList::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharList::Renew										 */
/* 内容		:一覧を更新														 */
/* 日付		:2008/12/01														 */
/* ========================================================================= */

void CDlgAdminCharList::Renew(void)
{
	int i, nCount;
	PCInfoCharCli pInfo;
	CString strTmp;

	m_pLibInfoChar = m_pMgrData->GetLibInfoCharOnline ();

	m_List.DeleteAllItems ();

	nCount = m_pLibInfoChar->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoCharCli)m_pLibInfoChar->GetPtr (i);
		strTmp.Format ("%d", pInfo->m_dwCharID);					/* キャラID */
		m_List.InsertItem (i, strTmp);
		m_List.SetItemData (i, pInfo->m_dwCharID);
		m_List.SetItemText (i, 1, (LPCSTR)pInfo->m_strCharName);	/* キャラ名 */
		strTmp.Format ("%d", pInfo->m_dwMapID);						/* マップID */
		m_List.SetItemText (i, 2, strTmp);
		strTmp.Format ("%d", pInfo->m_nMapX);						/* X座標 */
		m_List.SetItemText (i, 3, strTmp);
		strTmp.Format ("%d", pInfo->m_nMapY);						/* Y座標 */
		m_List.SetItemText (i, 4, strTmp);
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharList::OnAdminMsg									 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2008/12/01														 */
/* ========================================================================= */

void CDlgAdminCharList::OnAdminMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case ADMINMSG_CHAR_ONLINE:	/* オンライン中キャラ一覧更新 */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharList::OnInitDialog								 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/12/01														 */
/* ========================================================================= */

BOOL CDlgAdminCharList::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (1, "キャラID",		LVCFMT_LEFT, 60);
	m_List.InsertColumn (2, "キャラ名",		LVCFMT_LEFT, 130);
	m_List.InsertColumn (3, "マップID",		LVCFMT_LEFT, 60);
	m_List.InsertColumn (4, "X座標",		LVCFMT_LEFT, 50);
	m_List.InsertColumn (5, "Y座標",		LVCFMT_LEFT, 50);

	RegisterControl (IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	OnBnClickedRenew ();
	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharList::OnBnClickedRenew							 */
/* 内容		:ボタンハンドラ(更新)											 */
/* 日付		:2008/12/01														 */
/* ========================================================================= */

void CDlgAdminCharList::OnBnClickedRenew()
{
	CPacketADMIN_PARA2 Packet;

	Packet.Make (SBOCOMMANDID_SUB_ADMIN_CHAR_REQ_ONLINE);
	m_pSock->Send (&Packet);
}

/* Copyright(C)URARA-works 2008 */
