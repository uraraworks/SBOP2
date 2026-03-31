/// @file DlgAdminTmp.cpp
/// @brief ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/23
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTmp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminTmp::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTmp)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminTmp, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTmp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDlgAdminTmp::CDlgAdminTmp(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminTmp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTmp)
	//}}AFX_DATA_INIT
}


CDlgAdminTmp::~CDlgAdminTmp()
{
}


void CDlgAdminTmp::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init(pMgrData);

	// 以下はモードレスダイアログ用
	// ウィンドウ作成
	Create(CDlgAdminTmp::IDD, m_pWndParent);
	ShowWindow(SW_SHOW);
}


void CDlgAdminTmp::OnAdminMsg(int nType, DWORD dwPara)
{
}


BOOL CDlgAdminTmp::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	return TRUE;
}


void CDlgAdminTmp::PostNcDestroy()
{
	// モードレスダイアログ時はこの関数を消す
	CDialog::PostNcDestroy();
}