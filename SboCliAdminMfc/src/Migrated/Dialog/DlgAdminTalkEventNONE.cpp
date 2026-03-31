/// @file DlgAdminTalkEventNONE.cpp
/// @brief 会話イベント設定(未選択)ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/23
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventNONE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminTalkEventNONE::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventNONE)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventNONE, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventNONE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDlgAdminTalkEventNONE::CDlgAdminTalkEventNONE(CWnd* pParent)
	: CDlgAdminBase(CDlgAdminTalkEventNONE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventNONE)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminTalkEventNONE::IDD;
	m_nPageCount  = 0;
}

CDlgAdminTalkEventNONE::~CDlgAdminTalkEventNONE()
{
}

void CDlgAdminTalkEventNONE::Init(CMgrData *pMgrData, int nPage)
{
	CDlgAdminBase::Init(pMgrData);

	m_nPageCount = nPage;

	// ウィンドウ作成
	Create(m_nResourceID, m_pWndParent);
	ShowWindow(SW_SHOW);
}

void CDlgAdminTalkEventNONE::OnAdminMsg(int nType, DWORD dwPara)
{
}

void CDlgAdminTalkEventNONE::Set(CInfoTalkEventBase *pSrc)
{
}

void CDlgAdminTalkEventNONE::Get(CInfoTalkEventBase *pDst)
{
}

BOOL CDlgAdminTalkEventNONE::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	return TRUE;
}

