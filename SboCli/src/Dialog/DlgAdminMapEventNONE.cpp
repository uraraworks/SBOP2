/// @file DlgAdminMapEventNONE.cpp
/// @brief マップイベント(未設定)ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/26
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "InfoMapEventBase.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminMapEventNONE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminMapEventNONE::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapEventNONE)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapEventNONE, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapEventNONE)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDlgAdminMapEventNONE::CDlgAdminMapEventNONE(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapEventNONE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapEventNONE)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminMapEventNONE::IDD;
}


CDlgAdminMapEventNONE::~CDlgAdminMapEventNONE()
{
}


void CDlgAdminMapEventNONE::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init(pMgrData);

	// ウィンドウ作成
	Create(m_nResourceID, m_pWndParent);
	ShowWindow(SW_SHOW);
}


void CDlgAdminMapEventNONE::Set(CInfoMapEventBase *pSrc)
{
}


void CDlgAdminMapEventNONE::Get(CInfoMapEventBase *pDst)
{
}


BOOL CDlgAdminMapEventNONE::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	return TRUE;
}


LRESULT CDlgAdminMapEventNONE::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	return -1;
}