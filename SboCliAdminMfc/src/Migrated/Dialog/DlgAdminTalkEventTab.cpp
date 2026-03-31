/// @file DlgAdminTalkEventTab.cpp
/// @brief 会話イベントタブダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/23
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "InfoTalkEvent.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminTalkEventBase.h"
#include "DlgAdminTalkEventTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminTalkEventTab::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminTalkEventTab)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST, m_List);
}

BEGIN_MESSAGE_MAP(CDlgAdminTalkEventTab, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminTalkEventTab)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADD, &CDlgAdminTalkEventTab::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_MODIFY, &CDlgAdminTalkEventTab::OnBnClickedModify)
	ON_BN_CLICKED(IDC_DEL, &CDlgAdminTalkEventTab::OnBnClickedDel)
	ON_BN_CLICKED(IDC_UP, &CDlgAdminTalkEventTab::OnBnClickedUp)
	ON_BN_CLICKED(IDC_DOWN, &CDlgAdminTalkEventTab::OnBnClickedDown)
END_MESSAGE_MAP()

CDlgAdminTalkEventTab::CDlgAdminTalkEventTab(CWnd* pParent)
	: CDlgAdminBase(CDlgAdminTalkEventTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminTalkEventTab)
	//}}AFX_DATA_INIT

	m_nPage	= 0;
	m_pInfo	= NULL;
}

CDlgAdminTalkEventTab::~CDlgAdminTalkEventTab()
{
}

void CDlgAdminTalkEventTab::Init(CMgrData *pMgrData, CInfoTalkEvent *pInfo)
{
	CDlgAdminBase::Init(pMgrData);

	m_pInfo = pInfo;

	// ウィンドウ作成
	Create(CDlgAdminTalkEventTab::IDD, m_pWndParent);
	ShowWindow(SW_SHOW);
}

void CDlgAdminTalkEventTab::OnAdminMsg(int nType, DWORD dwPara)
{
}

void CDlgAdminTalkEventTab::SetPage(int nPage)
{
	m_nPage = nPage;
	Renew();
}

BOOL CDlgAdminTalkEventTab::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	RegisterControl(IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	RegisterControl(IDC_ADD,	LH_CTRL_Y);
	RegisterControl(IDC_MODIFY,	LH_CTRL_Y);
	RegisterControl(IDC_DEL,	LH_CTRL_Y);
	RegisterControl(IDC_UP,	LH_CTRL_X | LH_CTRL_Y);
	RegisterControl(IDC_DOWN,	LH_CTRL_X | LH_CTRL_Y);

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn(0, _T("イベント種別"), 0, 100);

	Renew();

	return TRUE;
}

void CDlgAdminTalkEventTab::OnBnClickedAdd()
{
	int nResult;
	PCInfoTalkEventBase pInfo;
	CDlgAdminTalkEventBase Dlg(this);

	Dlg.Init(m_pMgrData, m_pInfo->GetPageCount());
	nResult = Dlg.DoModal();
	if (nResult != IDOK) {
		return;
	}
	pInfo = NULL;
	Dlg.Get(pInfo);
	pInfo->m_nPage = m_nPage;
	m_pInfo->AddTalkEvent(pInfo);

	Renew();
}

void CDlgAdminTalkEventTab::OnBnClickedModify()
{
	int nSelect, nResult;
	PCInfoTalkEventBase pInfo;
	CDlgAdminTalkEventBase Dlg(this);

	nSelect = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (nSelect < 0) {
		return;
	}
	pInfo = (PCInfoTalkEventBase)m_List.GetItemData(nSelect);

	Dlg.Init(m_pMgrData, m_pInfo->GetPageCount(), pInfo);
	nResult = Dlg.DoModal();
	if (nResult != IDOK) {
		return;
	}
	pInfo = NULL;
	Dlg.Get(pInfo);
	m_pInfo->SetPtr(m_nPage, nSelect, pInfo);
	m_List.SetItemData(nSelect, (DWORD)pInfo);

	Renew();
}

void CDlgAdminTalkEventTab::OnBnClickedDel()
{
}

void CDlgAdminTalkEventTab::OnBnClickedUp()
{
	int nSelect;

	nSelect = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (nSelect < 1) {
		return;
	}
	m_pInfo->UpTalkEvent(m_nPage, nSelect);
	Renew();
	m_List.SetItemState(nSelect - 1, LVNI_SELECTED, LVNI_SELECTED);
}

void CDlgAdminTalkEventTab::OnBnClickedDown()
{
	int nSelect, nCount;

	nCount  = m_List.GetItemCount();
	nSelect = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (nSelect >= nCount - 1) {
		return;
	}
	m_pInfo->DownTalkEvent(m_nPage, nSelect);
	Renew();
	m_List.SetItemState(nSelect + 1, LVNI_SELECTED, LVNI_SELECTED);
}

void CDlgAdminTalkEventTab::Renew(void)
{
	int i, nCount;
	PCInfoTalkEventBase pInfo;
	ARRAYTALKEVENTBASEINFO apInfo;
	CString strTmp;

	m_List.DeleteAllItems();
	m_pInfo->GetEventArray(m_nPage, apInfo);

	nCount = apInfo.size();
	for (i = 0; i < nCount; i ++) {
		pInfo = apInfo[i];
		strTmp = "未選択";
		switch (pInfo->m_nEventType) {
		case TALKEVENTTYPE_PAGE:	// ページ切り替え
			strTmp = "ページ切り替え";
			break;
		case TALKEVENTTYPE_MSG:	// メッセージ表示
			strTmp = "メッセージ表示";
			break;
		case TALKEVENTTYPE_MENU:	// 項目選択
			strTmp = "項目選択";
			break;
		case TALKEVENTTYPE_ADDSKILL:	// スキル追加
			strTmp = "スキル追加";
			break;
		}
		m_List.InsertItem(i, strTmp);
		m_List.SetItemData(i, (DWORD)pInfo);
	}
}

