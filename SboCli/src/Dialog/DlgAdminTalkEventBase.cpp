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
#include "InfoTalkEvent.h"
#include "DlgAdminTalkEventNONE.h"
#include "DlgAdminTalkEventPAGE.h"
#include "DlgAdminTalkEventMSG.h"
#include "DlgAdminTalkEventMENU.h"
#include "DlgAdminTalkEventADDSKILL.h"
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

	m_pDlgType	= NULL;
	m_pInfo		= NULL;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventBase::~CDlgAdminTalkEventBase				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

CDlgAdminTalkEventBase::~CDlgAdminTalkEventBase()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventBase::Init									 */
/* 内容		:初期化															 */
/* 日付		:2008/12/23														 */
/* ========================================================================= */

void CDlgAdminTalkEventBase::Init(CMgrData *pMgrData, int nPage, CInfoTalkEventBase *pInfo/*NULL*/)
{
	int nType;
	CInfoTalkEvent InfoTalkEvent;

	CDlgAdminBase::Init (pMgrData);

	m_nPageCount = nPage;
	nType	= (pInfo) ? pInfo->m_nEventType : TALKEVENTTYPE_NONE;
	m_pInfo	= InfoTalkEvent.GetNew (nType);
	if (pInfo) {
		m_pInfo->Copy (pInfo);
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventBase::Get									 */
/* 内容		:取得															 */
/* 日付		:2008/12/26														 */
/* ========================================================================= */

void CDlgAdminTalkEventBase::Get(CInfoTalkEventBase *&pDst)
{
	CInfoTalkEvent InfoTalkEvent;

	SAFE_DELETE (pDst);

	pDst = InfoTalkEvent.GetNew (m_pInfo->m_nEventType);
	pDst->Copy (m_pInfo);
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
	int i, nCount, nNo;

	CDlgAdminBase::OnInitDialog();

	m_Type.InsertString (0, "未設定");
	m_Type.SetItemData (0, TALKEVENTTYPE_NONE);
	m_Type.InsertString (1, "ページ切り替え");
	m_Type.SetItemData (1, TALKEVENTTYPE_PAGE);
	m_Type.InsertString (2, "メッセージ表示");
	m_Type.SetItemData (2, TALKEVENTTYPE_MSG);
	m_Type.InsertString (3, "項目選択");
	m_Type.SetItemData (3, TALKEVENTTYPE_MENU);
	m_Type.InsertString (4, "スキル追加");
	m_Type.SetItemData (4, TALKEVENTTYPE_ADDSKILL);

	nNo = 0;
	nCount = m_Type.GetCount ();
	for (i = 0; i < nCount; i ++) {
		if (m_pInfo->m_nEventType == m_Type.GetItemData (i)) {
			nNo = i;
			break;
		}
	}
	m_Type.SetCurSel (nNo);

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
	PCInfoTalkEventBase pInfoTmp;
	CInfoTalkEvent InfoTalkEvent;

	nNo		= m_Type.GetCurSel ();
	nType	= m_Type.GetItemData (nNo);

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
	case TALKEVENTTYPE_ADDSKILL:		/* スキル追加 */
		m_pDlgType = new CDlgAdminTalkEventADDSKILL(this);
		break;
	default:
		m_pDlgType = new CDlgAdminTalkEventNONE(this);
		break;
	}
	/* 種別が変わった？ */
	if (m_pInfo->m_nEventType != nType) {
		pInfoTmp = InfoTalkEvent.GetNew (nType);
		pInfoTmp->m_nEventType = nType;
		SAFE_DELETE (m_pInfo);
		m_pInfo = pInfoTmp;
	}

	GetDlgItem (IDC_FRAME)->GetWindowRect (rc);
	ScreenToClient (rc);

	m_pDlgType->Init (m_pMgrData, m_nPageCount);
	m_pDlgType->MoveWindow (rc.left, rc.top, rc.Width (), rc.Height ());
	m_pDlgType->Set (m_pInfo);
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


/* ========================================================================= */
/* 関数名	:CDlgAdminTalkEventBase::OnOK									 */
/* 内容		:ボタンハンドラ(OK)												 */
/* 日付		:2008/12/25														 */
/* ========================================================================= */

void CDlgAdminTalkEventBase::OnOK()
{
	UpdateData ();

	if (m_pDlgType) {
		m_pDlgType->Get (m_pInfo);
	}

	CDlgAdminBase::OnOK ();
}

/* Copyright(C)URARA-works 2008 */
