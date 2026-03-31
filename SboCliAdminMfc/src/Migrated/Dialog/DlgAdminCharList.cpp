/// @file DlgAdminCharList.cpp
/// @brief キャラ一覧ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/12/01
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_PARA2.h"
#include "InfoCharCli.h"
#include "LibInfoCharCli.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminCharList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminCharList::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharList)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharList, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharList)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RENEW, &CDlgAdminCharList::OnBnClickedRenew)
END_MESSAGE_MAP()

CDlgAdminCharList::CDlgAdminCharList(CWnd* pParent)
	: CDlgAdminBase(CDlgAdminCharList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharList)
	//}}AFX_DATA_INIT

	m_pWndNotify	= NULL;
	m_pLibInfoChar	= NULL;
}

CDlgAdminCharList::~CDlgAdminCharList()
{
}

void CDlgAdminCharList::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init(pMgrData);

	m_pLibInfoChar = m_pMgrData->GetLibInfoCharOnline();

	// ウィンドウ作成
	Create(CDlgAdminCharList::IDD, m_pWndParent);
	ShowWindow(SW_SHOW);
}

void CDlgAdminCharList::Renew(void)
{
	int i, nCount;
	PCInfoCharCli pInfo;
	CString strTmp;

	m_pLibInfoChar = m_pMgrData->GetLibInfoCharOnline();

	m_List.DeleteAllItems();

	nCount = m_pLibInfoChar->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoCharCli)m_pLibInfoChar->GetPtr(i);
		strTmp.Format(_T("%d"), pInfo->m_dwCharID);	// キャラID
		m_List.InsertItem(i, strTmp);
		m_List.SetItemData(i, pInfo->m_dwCharID);
		CString strName = (LPCTSTR)pInfo->m_strCharName;	// キャラ名
		m_List.SetItemText(i, 1, strName);
		strTmp.Format(_T("%d"), pInfo->m_dwMapID);	// マップID
		m_List.SetItemText(i, 2, strTmp);
		strTmp.Format(_T("%d"), pInfo->m_nMapX);	// X座標
		m_List.SetItemText(i, 3, strTmp);
		strTmp.Format(_T("%d"), pInfo->m_nMapY);	// Y座標
		m_List.SetItemText(i, 4, strTmp);
	}
}

void CDlgAdminCharList::OnAdminMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case ADMINMSG_CHAR_ONLINE:	// オンライン中キャラ一覧更新
		Renew();
		break;
	}
}

BOOL CDlgAdminCharList::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn(1, _T("キャラID"),	LVCFMT_LEFT, 60);
	m_List.InsertColumn(2, _T("キャラ名"),	LVCFMT_LEFT, 130);
	m_List.InsertColumn(3, _T("マップID"),	LVCFMT_LEFT, 60);
	m_List.InsertColumn(4, _T("X座標"),	LVCFMT_LEFT, 50);
	m_List.InsertColumn(5, _T("Y座標"),	LVCFMT_LEFT, 50);

	RegisterControl(IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	OnBnClickedRenew();
	return TRUE;
}

void CDlgAdminCharList::OnBnClickedRenew()
{
	CPacketADMIN_PARA2 Packet;

	Packet.Make(SBOCOMMANDID_SUB_ADMIN_CHAR_REQ_ONLINE);
	SendPacket(&Packet);
}

