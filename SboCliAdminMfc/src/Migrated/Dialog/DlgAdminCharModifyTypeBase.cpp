/// @file DlgAdminCharModifyTypeBase.cpp
/// @brief キャラ情報 移動種別の設定ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/07/12
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "DlgAdminCharModifyTypeBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminCharModifyTypeBase::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharModifyTypeBase)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharModifyTypeBase, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharModifyTypeBase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDlgAdminCharModifyTypeBase::CDlgAdminCharModifyTypeBase(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharModifyTypeBase::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharModifyTypeBase)
	//}}AFX_DATA_INIT
}
CDlgAdminCharModifyTypeBase::CDlgAdminCharModifyTypeBase(int nResourceID, CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(nResourceID, pParent)
{
}


CDlgAdminCharModifyTypeBase::~CDlgAdminCharModifyTypeBase()
{
}


void CDlgAdminCharModifyTypeBase::Set(CInfoCharCli *pSrc)
{
}


void CDlgAdminCharModifyTypeBase::Get(CInfoCharCli *pDst)
{
}


void CDlgAdminCharModifyTypeBase::PostNcDestroy()
{
	CDialog::PostNcDestroy();
}