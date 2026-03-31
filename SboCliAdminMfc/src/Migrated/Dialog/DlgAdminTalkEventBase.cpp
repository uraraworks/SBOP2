/// @file DlgAdminTalkEventBase.cpp
/// @brief 会話イベントの設定ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/23
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
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

// クラスの設定

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

CDlgAdminTalkEventBase::CDlgAdminTalkEventBase(CWnd* pParent)
	: CDlgAdminBase(CDlgAdminTalkEventBase::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventBase)
	//}}AFX_DATA_INIT

	m_pDlgType	= NULL;
	m_pInfo	= NULL;
}

CDlgAdminTalkEventBase::~CDlgAdminTalkEventBase()
{
	SAFE_DELETE(m_pInfo);
}

void CDlgAdminTalkEventBase::Init(CMgrData *pMgrData, int nPage, CInfoTalkEventBase *pInfo/*=NULL*/)
{
	int nType;
	CInfoTalkEvent InfoTalkEvent;

	CDlgAdminBase::Init(pMgrData);

	m_nPageCount = nPage;
	nType	= (pInfo) ? pInfo->m_nEventType : TALKEVENTTYPE_NONE;
	m_pInfo	= InfoTalkEvent.GetNew(nType);
	if (pInfo) {
		m_pInfo->Copy(pInfo);
	}
}

void CDlgAdminTalkEventBase::Get(CInfoTalkEventBase *&pDst)
{
	CInfoTalkEvent InfoTalkEvent;

	SAFE_DELETE(pDst);

	pDst = InfoTalkEvent.GetNew(m_pInfo->m_nEventType);
	pDst->Copy(m_pInfo);
}

void CDlgAdminTalkEventBase::OnAdminMsg(int nType, DWORD dwPara)
{
}

BOOL CDlgAdminTalkEventBase::OnInitDialog()
{
	int i, nCount, nNo;

	CDlgAdminBase::OnInitDialog();

	m_Type.InsertString(0, _T("未設定"));
	m_Type.SetItemData(0, TALKEVENTTYPE_NONE);
	m_Type.InsertString(1, _T("ページ切り替え"));
	m_Type.SetItemData(1, TALKEVENTTYPE_PAGE);
	m_Type.InsertString(2, _T("メッセージ表示"));
	m_Type.SetItemData(2, TALKEVENTTYPE_MSG);
	m_Type.InsertString(3, _T("項目選択"));
	m_Type.SetItemData(3, TALKEVENTTYPE_MENU);
	m_Type.InsertString(4, _T("スキル追加"));
	m_Type.SetItemData(4, TALKEVENTTYPE_ADDSKILL);

	nNo = 0;
	nCount = m_Type.GetCount();
	for (i = 0; i < nCount; i ++) {
		if (m_pInfo->m_nEventType == m_Type.GetItemData(i)) {
			nNo = i;
			break;
		}
	}
	m_Type.SetCurSel(nNo);

	OnCbnSelchangeType();

	return TRUE;
}

void CDlgAdminTalkEventBase::OnCbnSelchangeType()
{
	int nNo, nType;
	CRect rc;
	PCInfoTalkEventBase pInfoTmp;
	CInfoTalkEvent InfoTalkEvent;

	nNo	= m_Type.GetCurSel();
	nType	= m_Type.GetItemData(nNo);

	if (m_pDlgType) {
		m_pDlgType->DestroyWindow();
		m_pDlgType = NULL;
	}
	switch (nType) {
	case TALKEVENTTYPE_PAGE:	// ページ切り替え
		m_pDlgType = new CDlgAdminTalkEventPAGE(this);
		break;
	case TALKEVENTTYPE_MSG:	// メッセージ表示
		m_pDlgType = new CDlgAdminTalkEventMSG(this);
		break;
	case TALKEVENTTYPE_MENU:	// 項目選択
		m_pDlgType = new CDlgAdminTalkEventMENU(this);
		break;
	case TALKEVENTTYPE_ADDSKILL:	// スキル追加
		m_pDlgType = new CDlgAdminTalkEventADDSKILL(this);
		break;
	default:
		m_pDlgType = new CDlgAdminTalkEventNONE(this);
		break;
	}
	// 種別が変わった？
	if (m_pInfo->m_nEventType != nType) {
		pInfoTmp = InfoTalkEvent.GetNew(nType);
		pInfoTmp->m_nEventType = nType;
		SAFE_DELETE(m_pInfo);
		m_pInfo = pInfoTmp;
	}

	GetDlgItem(IDC_FRAME)->GetWindowRect(rc);
	ScreenToClient(rc);

	m_pDlgType->Init(m_pMgrData, m_nPageCount);
	m_pDlgType->MoveWindow(rc.left, rc.top, rc.Width(), rc.Height());
	m_pDlgType->Set(m_pInfo);
}

void CDlgAdminTalkEventBase::PostNcDestroy()
{
	// モードレスダイアログ時はこの関数を消す
	CDialog::PostNcDestroy();
}

void CDlgAdminTalkEventBase::OnOK()
{
	UpdateData();

	if (m_pDlgType) {
		m_pDlgType->Get(m_pInfo);
	}

	CDlgAdminBase::OnOK();
}

