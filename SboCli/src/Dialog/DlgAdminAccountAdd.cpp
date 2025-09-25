/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:DlgAdminAccountAdd.cpp										 */
/* 内容			:アカウント追加ダイアログクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/01/17													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "Packet.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminAccountAdd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminAccountAdd::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminAccountAdd)
	DDX_Text(pDX, IDC_ACCOUNT, m_strAccount);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminAccountAdd, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminAccountAdd)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminAccountAdd::CDlgAdminAccountAdd						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2009/01/17														 */
/* ========================================================================= */

CDlgAdminAccountAdd::CDlgAdminAccountAdd(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminAccountAdd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminAccountAdd)
	m_strAccount = _T("");
	m_strPassword = _T("");
	//}}AFX_DATA_INIT
}


/* ========================================================================= */
/* 関数名	:CDlgAdminAccountAdd::~CDlgAdminAccountAdd						 */
/* 内容		:デストラクタ													 */
/* 日付		:2009/01/17														 */
/* ========================================================================= */

CDlgAdminAccountAdd::~CDlgAdminAccountAdd()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminAccountAdd::Init										 */
/* 内容		:初期化															 */
/* 日付		:2009/01/17														 */
/* ========================================================================= */

void CDlgAdminAccountAdd::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	/* ウィンドウ作成 */
	Create (CDlgAdminAccountAdd::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminAccountAdd::OnSend									 */
/* 内容		:ボタンハンドラ(送信)											 */
/* 日付		:2009/01/17														 */
/* ========================================================================= */

void CDlgAdminAccountAdd::OnSend()
{
	int nResult;
	CPacketADMIN_ACCOUNT_REQ_ADD Packet;

        nResult = MessageBox (_T("アカウント情報を送信しますか？"), _T("確認"), MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	UpdateData ();

	Packet.Make ((LPCSTR)m_strAccount, (LPCSTR)m_strPassword);
	m_pSock->Send (&Packet);
}

/* Copyright(C)URARA-works 2009 */
