/// @file DlgAdminItemTypeNewLIGHT.cpp
/// @brief アイテム種別(灯り)設定ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/10/19
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "InfoItemTypeBase.h"
#include "MgrData.h"
#include "DlgAdminItemTypeNewLIGHT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminItemTypeNewLIGHT::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminItemTypeNewLIGHT)
	DDX_Text(pDX, IDC_VALUE1, m_nValue1);
	DDX_Text(pDX, IDC_VALUE2, m_nValue2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminItemTypeNewLIGHT, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminItemTypeNewLIGHT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDlgAdminItemTypeNewLIGHT::CDlgAdminItemTypeNewLIGHT(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminItemTypeNewLIGHT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminItemTypeNewLIGHT)
	m_nValue1 = 0;
	m_nValue2 = 0;
	//}}AFX_DATA_INIT
}


CDlgAdminItemTypeNewLIGHT::~CDlgAdminItemTypeNewLIGHT()
{
}


void CDlgAdminItemTypeNewLIGHT::Set(CInfoItemTypeBase *pSrc)
{
	m_nValue1	= (int)pSrc->m_dwValue; // 灯りレベル
	m_nValue2	= (int)pSrc->m_dwValue2; // 持続時間
}


void CDlgAdminItemTypeNewLIGHT::Get(CInfoItemTypeBase *&pDst)
{
	pDst->m_dwValue	= m_nValue1; // 灯りレベル
	pDst->m_dwValue2	= m_nValue2; // 持続時間
}


void CDlgAdminItemTypeNewLIGHT::PostNcDestroy()
{
	CDialog::PostNcDestroy();
}