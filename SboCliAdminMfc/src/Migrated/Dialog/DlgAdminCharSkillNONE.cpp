/// @file DlgAdminCharSkillNONE.cpp
/// @brief スキル(未設定)ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/07
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "InfoMapEventBase.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminCharSkillNONE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminCharSkillNONE::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharSkillNONE)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharSkillNONE, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharSkillNONE)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDlgAdminCharSkillNONE::CDlgAdminCharSkillNONE(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharSkillNONE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharSkillNONE)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminCharSkillNONE::IDD;
}


CDlgAdminCharSkillNONE::~CDlgAdminCharSkillNONE()
{
}


void CDlgAdminCharSkillNONE::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init(pMgrData);

	// ウィンドウ作成
	Create(m_nResourceID, m_pWndParent);
	ShowWindow(SW_SHOW);
}


void CDlgAdminCharSkillNONE::Set(CInfoSkillBase *pSrc)
{
}


void CDlgAdminCharSkillNONE::Get(CInfoSkillBase *pDst)
{
}


BOOL CDlgAdminCharSkillNONE::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	return TRUE;
}


LRESULT CDlgAdminCharSkillNONE::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	return -1;
}