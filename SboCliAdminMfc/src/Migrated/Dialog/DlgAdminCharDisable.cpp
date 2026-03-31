/// @file DlgAdminCharDisable.cpp
/// @brief キャラ一覧ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2009/04/06
/// @copyright Copyright(C)URARA-works 2009

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_PARA2.h"
#include "PacketADMIN_DISABLE_RENEWINFO.h"
#include "LibInfoDisable.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminCharDisableNew.h"
#include "DlgAdminCharDisable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminCharDisable::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharDisable)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharDisable, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharDisable)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RENEW, &CDlgAdminCharDisable::OnBnClickedRenew)
	ON_BN_CLICKED(IDC_ADD, &CDlgAdminCharDisable::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_DELETE, &CDlgAdminCharDisable::OnBnClickedDelete)
END_MESSAGE_MAP()

CDlgAdminCharDisable::CDlgAdminCharDisable(CWnd* pParent)
	: CDlgAdminBase(CDlgAdminCharDisable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharDisable)
	//}}AFX_DATA_INIT

	m_pWndNotify	= NULL;
	m_pLibInfoDisable	= new CLibInfoDisable;
}

CDlgAdminCharDisable::~CDlgAdminCharDisable()
{
	SAFE_DELETE(m_pLibInfoDisable);
}

void CDlgAdminCharDisable::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init(pMgrData);

	m_pLibInfoDisable->Create();

	// ウィンドウ作成
	Create(CDlgAdminCharDisable::IDD, m_pWndParent);
	ShowWindow(SW_SHOW);
}

void CDlgAdminCharDisable::Renew(void)
{
	int i, nCount;
	PCInfoDisable pInfo;
	PCLibInfoDisable pLibInfoDisable;

	pLibInfoDisable = m_pMgrData->GetLibInfoDisable();
	m_pLibInfoDisable->CopyAll(pLibInfoDisable);

	m_List.DeleteAllItems();

	nCount = m_pLibInfoDisable->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoDisable)m_pLibInfoDisable->GetPtr(i);
		m_List.InsertItem(i, (LPCTSTR)pInfo->m_strMacAddress);	// 拒否するMACアドレス
		m_List.SetItemData(i, pInfo->m_dwDisableID);
	}
}

void CDlgAdminCharDisable::OnAdminMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case ADMINMSG_RENEWDISABLE:	// 拒否情報更新
		Renew();
		break;
	}
}

BOOL CDlgAdminCharDisable::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn(1, _T("拒否するMACアドレス"),	LVCFMT_LEFT, 200);

	RegisterControl(IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	OnBnClickedRenew();
	return TRUE;
}

void CDlgAdminCharDisable::OnBnClickedRenew()
{
	CPacketADMIN_PARA2 Packet;

	Packet.Make(SBOCOMMANDID_SUB_ADMIN_DISABLE_REQ_INFO);
	SendPacket(&Packet);
}

void CDlgAdminCharDisable::OnBnClickedAdd()
{
	int nResult;
	PCInfoDisable pInfo;
	CDlgAdminCharDisableNew Dlg;
	CPacketADMIN_DISABLE_RENEWINFO Packet;

	nResult = Dlg.DoModal();
	if (nResult != IDOK) {
		return;
	}

	pInfo = (PCInfoDisable)m_pLibInfoDisable->GetNew();
	pInfo->m_strMacAddress = Dlg.m_strMacAddress;
	m_pLibInfoDisable->Add(pInfo);

	Packet.Make(pInfo);
	SendPacket(&Packet);
}

void CDlgAdminCharDisable::OnBnClickedDelete()
{
	int nResult, nNo;
	DWORD dwDisableID;
	CPacketADMIN_PARA2 Packet;

	nNo = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}
	dwDisableID = m_List.GetItemData(nNo);

        nResult = MessageBox(_T("このMACアドレスの拒否を解除しますか？"), _T("確認"), MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	Packet.Make(SBOCOMMANDID_SUB_ADMIN_DISABLE_REQ_DELETE, dwDisableID);
	SendPacket(&Packet);
}

