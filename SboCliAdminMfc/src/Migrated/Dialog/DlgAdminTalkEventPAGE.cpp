/// @file DlgAdminTalkEventPAGE.cpp
/// @brief 会話イベント設定(ページ切り替え)ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/23
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "LibInfoItemType.h"
#include "InfoTalkEventPAGE.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventPAGE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminTalkEventPAGE::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminTalkEventNONE::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventPAGE)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO, m_Combo);
	DDX_Control(pDX, IDC_ITEM_CMB, m_cmbItem);
	DDX_Control(pDX, IDC_JUMP_CMB, m_cmbJump);
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventPAGE, CDlgAdminTalkEventNONE)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventPAGE)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO, &CDlgAdminTalkEventPAGE::OnCbnSelchangeCombo)
END_MESSAGE_MAP()

CDlgAdminTalkEventPAGE::CDlgAdminTalkEventPAGE(CWnd* pParent)
	: CDlgAdminTalkEventNONE(pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventPAGE)
	//}}AFX_DATA_INIT

	m_nResourceID = CDlgAdminTalkEventPAGE::IDD;
}

CDlgAdminTalkEventPAGE::~CDlgAdminTalkEventPAGE()
{
}

void CDlgAdminTalkEventPAGE::Set(CInfoTalkEventBase *pSrc)
{
	int i, nCount;
	PCInfoTalkEventPAGE pSrcTmp = (PCInfoTalkEventPAGE)pSrc;

	UpdateData(FALSE);

	m_Combo.SetCurSel(pSrcTmp->m_nPageChgCondition);
	OnCbnSelchangeCombo();
	m_cmbJump.SetCurSel(pSrcTmp->m_nPageJump);

	nCount = m_cmbItem.GetCount();
	for (i = 0; i < nCount; i ++) {
		if (m_cmbItem.GetItemData(i) == pSrcTmp->m_dwData) {
			m_cmbItem.SetCurSel(i);
			break;
		}
	}
}

void CDlgAdminTalkEventPAGE::Get(CInfoTalkEventBase *pDst)
{
	PCInfoTalkEventPAGE pDstTmp = (PCInfoTalkEventPAGE)pDst;

	UpdateData();

	pDstTmp->m_nPageChgCondition	= m_Combo.GetItemData(m_Combo.GetCurSel());
	pDstTmp->m_nPageJump	= m_cmbJump.GetCurSel();
	pDstTmp->m_dwData	= m_cmbItem.GetItemData(m_cmbItem.GetCurSel());
}

BOOL CDlgAdminTalkEventPAGE::OnInitDialog()
{
	int i, nCount;
	PCLibInfoItemType pLibInfoItemType;
	PCInfoItemTypeBase pInfoItem;
	CString strTmp;

	CDlgAdminTalkEventNONE::OnInitDialog();

	m_Combo.InsertString(0, _T("未設定"));
	m_Combo.SetItemData(0, CHGPAGECONDITION_NONE);
	m_Combo.InsertString(1, _T("アイテムあり"));
	m_Combo.SetItemData(1, CHGPAGECONDITION_ITEM);
	m_Combo.InsertString(2, _T("アイテムなし"));
	m_Combo.SetItemData(2, CHGPAGECONDITION_NOITEM);
	m_Combo.SetCurSel(0);
	OnCbnSelchangeCombo();

	for (i = 0; i < m_nPageCount; i ++) {
		strTmp.Format(_T("ページ%d"), i + 1);
		m_cmbJump.InsertString(i, strTmp);
	}
	m_cmbJump.SetCurSel(0);

	pLibInfoItemType = m_pMgrData->GetLibInfoItemType();
	nCount = pLibInfoItemType->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoItem = (PCInfoItemTypeBase)pLibInfoItemType->GetPtr(i);
		m_cmbItem.InsertString(i, Utf8ToTString((LPCSTR)pInfoItem->m_strName));
		m_cmbItem.SetItemData(i, pInfoItem->m_dwTypeID);
	}
	m_cmbItem.SetCurSel(0);

	return TRUE;
}

void CDlgAdminTalkEventPAGE::OnCbnSelchangeCombo()
{
	int nResult, nCmdShow;

	nCmdShow = SW_HIDE;
	nResult  = m_Combo.GetCurSel();
	switch (nResult) {
	case CHGPAGECONDITION_ITEM:	// アイテムあり
	case CHGPAGECONDITION_NOITEM:	// アイテムなし
		nCmdShow = SW_SHOW;
		break;
	}
	GetDlgItem(IDC_ITEM_STA)->ShowWindow(nCmdShow);
	GetDlgItem(IDC_ITEM_CMB)->ShowWindow(nCmdShow);
}

