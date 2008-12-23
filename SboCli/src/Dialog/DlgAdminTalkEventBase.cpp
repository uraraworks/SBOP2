/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminTalkEventBase.cpp									 */
/* 内容			:会話イベントの設定ダイアログクラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "InfoTalkEventBase.h"
#include "DlgAdminTalkEventNONE.h"
#include "DlgAdminTalkEventPAGE.h"
#include "DlgAdminTalkEventMSG.h"
#include "DlgAdminTalkEventMENU.h"
#include "DlgAdminTalkEventBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminTalkEventBase::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventBase)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TYPE, m_Type);
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventBase, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventBase)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_TYPE, &CDlgAdminTalkEventBase::OnCbnSelchangeType)
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventBase::CDlgAdminTalkEventBase					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventBase::CDlgAdminTalkEventBase(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminTalkEventBase::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventBase)
	//}}AFX_DATA_INIT

	m_pDlgType = NULL;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventBase::~CDlgAdminTalkEventBase				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventBase::~CDlgAdminTalkEventBase()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventBase::Init									 */
/* 内容		:初期化															 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventBase::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventBase::OnAdminMsg								 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventBase::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventBase::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

BOOL CDlgAdminTalkEventBase::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_Type.InsertString (0, "未設定");
	m_Type.SetItemData (0, TALKEVENTTYPE_NONE);
	m_Type.InsertString (1, "ページ切り替え");
	m_Type.SetItemData (1, TALKEVENTTYPE_PAGE);
	m_Type.InsertString (2, "メッセージ表示");
	m_Type.SetItemData (2, TALKEVENTTYPE_MSG);
	m_Type.InsertString (3, "項目選択");
	m_Type.SetItemData (3, TALKEVENTTYPE_MENU);

	m_Type.SetCurSel (0);

	OnCbnSelchangeType ();

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventBase::OnCbnSelchangeType						 */
/* 内容		:イベントハンドラ(CBN_SELCHANGE)								 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventBase::OnCbnSelchangeType()
{
	int nNo, nType;
	CRect rc;
//	PCInfoMapEventBase pInfoTmp;
//	CLibInfoMapEvent LibInfo;

	nNo = m_Type.GetCurSel ();
	nType = m_Type.GetItemData (nNo);

	if (m_pDlgType) {
		m_pDlgType->DestroyWindow ();
		m_pDlgType = NULL;
	}
	switch (nType) {
	case TALKEVENTTYPE_PAGE:			/* ページ切り替え */
		m_pDlgType = new CDlgAdminTalkEventPAGE(this);
		break;
	case TALKEVENTTYPE_MSG:				/* メッセージ表示 */
		m_pDlgType = new CDlgAdminTalkEventMSG(this);
		break;
	case TALKEVENTTYPE_MENU:			/* 項目選択 */
		m_pDlgType = new CDlgAdminTalkEventMENU(this);
		break;
	default:
		m_pDlgType = new CDlgAdminTalkEventNONE(this);
		break;
	}
	if (m_pDlgType) {
		m_pDlgType->Init (m_pMgrData);
		GetDlgItem (IDC_FRAME)->GetWindowRect (rc);
		ScreenToClient (rc);
		m_pDlgType->MoveWindow (rc.left, rc.top, rc.Width (), rc.Height ());
	}
#if 0
	if (m_nEventType != nEventType) {
		pInfoTmp = (PCInfoMapEventBase)LibInfo.GetNew (nEventType);
		if (m_pInfo) {
			pInfoTmp->m_dwMapEventID	= m_pInfo->m_dwMapEventID;
			pInfoTmp->m_ptPos			= m_pInfo->m_ptPos;
		}
		pInfoTmp->m_nType = nEventType;

		SAFE_DELETE (m_pInfo);
		m_pInfo = pInfoTmp;
	}
#endif
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventBase::PostNcDestroy							 */
/* 内容		:終了処理														 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventBase::PostNcDestroy()
{
	/* モードレスダイアログ時はこの関数を消す */
	CDialog::PostNcDestroy ();
}

/* Copyright(C)URARA-works 2008 */
