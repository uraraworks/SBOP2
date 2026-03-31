/// @file DlgAdminTalkEventMSG.cpp
/// @brief 会話イベント設定(メッセージ表示)ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/23
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "InfoTalkEventBase.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventMSG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminTalkEventMSG::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminTalkEventNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventMSG)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_MSG, m_strMsg);
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventMSG, CDlgAdminTalkEventNONE)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventMSG)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDlgAdminTalkEventMSG::CDlgAdminTalkEventMSG(CWnd* pParent)
	: CDlgAdminTalkEventNONE(pParent)
	, m_strMsg(_T(""))
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventMSG)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminTalkEventMSG::IDD;
}

CDlgAdminTalkEventMSG::~CDlgAdminTalkEventMSG()
{
}

void CDlgAdminTalkEventMSG::Set(CInfoTalkEventBase *pSrc)
{
	m_strMsg = (LPCTSTR)pSrc->m_strText;

	UpdateData(FALSE);
}

void CDlgAdminTalkEventMSG::Get(CInfoTalkEventBase *pDst)
{
	UpdateData();

	pDst->m_strText = m_strMsg;
}

BOOL CDlgAdminTalkEventMSG::OnInitDialog()
{
	CDlgAdminTalkEventNONE::OnInitDialog();

	RegisterControl(IDC_MSG, LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	return TRUE;
}

