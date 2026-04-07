/// @file DlgAdminItemTypeNewHP.cpp
/// @brief アイテム種別(HP増減)設定ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/08/15
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "InfoItemTypeBase.h"
#include "InfoMotion.h"
#include "MgrData.h"
#include "DlgAdminItemTypeNewHP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminItemTypeNewHP::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminItemTypeNewHP)
	DDX_Control(pDX, IDC_TARGET, m_ctlTarget);
	DDX_Control(pDX, IDC_AREA, m_ctlArea);
	DDX_Text(pDX, IDC_VALUE1, m_nValue1);
	DDX_Text(pDX, IDC_VALUE2, m_nValue2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminItemTypeNewHP, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminItemTypeNewHP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDlgAdminItemTypeNewHP::CDlgAdminItemTypeNewHP(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminItemTypeNewHP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminItemTypeNewHP)
	m_nValue1 = 0;
	m_nValue2 = 0;
	//}}AFX_DATA_INIT

	m_byTarget = 0; // 使用対象
	m_byArea = 0; // 使用範囲
}


CDlgAdminItemTypeNewHP::~CDlgAdminItemTypeNewHP()
{
}


void CDlgAdminItemTypeNewHP::Set(CInfoItemTypeBase *pSrc)
{
	m_byTarget	= pSrc->m_byTarget; // 使用対象
	m_byArea	= pSrc->m_byArea; // 使用範囲
	m_nValue1	= (int)pSrc->m_dwValue; // 最小
	m_nValue2	= (int)pSrc->m_dwValue2; // 最大
}


void CDlgAdminItemTypeNewHP::Get(CInfoItemTypeBase *&pDst)
{
	pDst->m_byTarget	= m_byTarget; // 使用対象
	pDst->m_byArea	= m_byArea; // 使用範囲
	pDst->m_dwValue	= m_nValue1; // 最小
	pDst->m_dwValue2	= m_nValue2; // 最大
}


BOOL CDlgAdminItemTypeNewHP::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_ctlTarget.InsertString(0, _T("味方"));
	m_ctlTarget.InsertString(1, _T("敵"));

	m_ctlArea.InsertString(0, _T("自分"));
	m_ctlArea.InsertString(1, _T("1歩前"));
	m_ctlArea.InsertString(2, _T("周囲"));
	m_ctlArea.InsertString(3, _T("画面内"));

	SetTarget(m_byTarget);
	SetArea(m_byArea);

	return TRUE;
}


void CDlgAdminItemTypeNewHP::PostNcDestroy()
{
	CDialog::PostNcDestroy();
}


void CDlgAdminItemTypeNewHP::OnOK()
{
	UpdateData();

	m_byTarget	= GetTarget();
	m_byArea	= GetArea();

	CDlgAdminBase::EndDialog(IDOK);
}


void CDlgAdminItemTypeNewHP::SetTarget(BYTE byTarget)
{
	m_ctlTarget.SetCurSel(byTarget);
}


BYTE CDlgAdminItemTypeNewHP::GetTarget(void)
{
	int nSelect;
	BYTE byRet;

	byRet	= 0;
	nSelect	= m_ctlTarget.GetCurSel();
	if (nSelect >= 0) {
		byRet = (BYTE)nSelect;
	}

	return byRet;
}


void CDlgAdminItemTypeNewHP::SetArea(BYTE byTarget)
{
	m_ctlArea.SetCurSel(byTarget);
}


BYTE CDlgAdminItemTypeNewHP::GetArea(void)
{
	int nSelect;
	BYTE byRet;

	byRet	= 0;
	nSelect	= m_ctlArea.GetCurSel();
	if (nSelect >= 0) {
		byRet = (BYTE)nSelect;
	}

	return byRet;
}