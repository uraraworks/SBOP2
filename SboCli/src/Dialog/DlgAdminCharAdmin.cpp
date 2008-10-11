/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharAdmin.cpp										 */
/* 内容			:管理者権限の設定ダイアログクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_REQ_ADMINLEVEL.h"
#include "PacketADMIN_RENEWADMINLEVEL.h"
#include "LayoutHelper.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "DlgAdminCharAdmin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminCharAdmin::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharAdmin)
	DDX_Control(pDX, IDC_ADMINLEVEL, m_ctAdminLevel);
	DDX_Text(pDX, IDC_STATIC_CHARNAME, m_strCharName);
	DDX_Text(pDX, IDC_STATIC_ACCOUNTID, m_strAccountID);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharAdmin, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharAdmin)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminCharAdmin::CDlgAdminCharAdmin							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

CDlgAdminCharAdmin::CDlgAdminCharAdmin(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharAdmin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharAdmin)
	m_strAccountID = _T("");
	//}}AFX_DATA_INIT

	m_nAdminLevel = 0;
	m_dwAccountID = 0;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharAdmin::~CDlgAdminCharAdmin						 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

CDlgAdminCharAdmin::~CDlgAdminCharAdmin()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharAdmin::Init										 */
/* 内容		:初期化															 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

void CDlgAdminCharAdmin::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* ウィンドウ作成 */
	Create (CDlgAdminCharAdmin::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharAdmin::Renew										 */
/* 内容		:更新															 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

void CDlgAdminCharAdmin::Renew(void)
{
	PCLibInfoCharCli pLibInfoChar;
	PCInfoCharCli pInfoChar;

	m_strAccountID.	Empty ();
	m_strCharName.	Empty ();
	m_ctAdminLevel.	SetCurSel (0);

	pLibInfoChar	= m_pMgrData->GetLibInfoChar ();
	pInfoChar		= (PCInfoCharCli)pLibInfoChar->GetPtrAccountID (m_dwAccountID);
	if (pInfoChar == NULL) {
		goto Exit;
	}

	m_strAccountID.Format ("%u", m_dwAccountID);
	m_strCharName = pInfoChar->m_strCharName;
	m_ctAdminLevel.SetCurSel (m_nAdminLevel);

Exit:
	UpdateData (FALSE);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharAdmin::OnAdminMsg									 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminCharAdmin::OnAdminMsg(int nType, DWORD dwPara)
{
	CPacketADMIN_REQ_ADMINLEVEL Packet;

	switch (nType) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:		/* 左クリック通知 */
		m_dwAccountID = dwPara;
		if (m_dwAccountID == 0) {
			break;
		}

		/* 選択されたアカウントの管理者権限レベルを要求 */
		Packet.Make (m_dwAccountID);
		m_pSock->Send (&Packet);
		break;
	case ADMINMSG_ADMINLEVEL:					/* 管理者レベル通知 */
		m_nAdminLevel = dwPara;
		break;
	default:
		return;
	}

	Renew ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharAdmin::OnInitDialog								 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

BOOL CDlgAdminCharAdmin::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog ();

//	m_pLayoutHelper->RegisterControl (GetDlgItem (IDC_LIST)->m_hWnd, LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	m_ctAdminLevel.SetCurSel (0);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharAdmin::OnSend										 */
/* 内容		:ボタンハンドラ(送信)											 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminCharAdmin::OnSend()
{
	CPacketADMIN_RENEWADMINLEVEL Packet;

	if (m_dwAccountID == 0) {
		return;
	}

	Packet.Make (m_dwAccountID, m_ctAdminLevel.GetCurSel ());
	m_pSock->Send (&Packet);
}

/* Copyright(C)URARA-works 2007 */
