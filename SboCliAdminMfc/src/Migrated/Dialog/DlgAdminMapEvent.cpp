/// @file DlgAdminMapEvent.cpp
/// @brief マップイベント一覧ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/06/24
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "LibInfoMapEvent.h"
#include "InfoMapBase.h"
#include "PacketADMIN_MAP_EVENT.h"
#include "PacketADMIN_PARA2.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminMapEventBase.h"
#include "DlgAdminMapEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminMapEvent::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapEvent)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapEvent, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapEvent)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDlgAdminMapEvent::CDlgAdminMapEvent(CWnd* pParent)
	: CDlgAdminBase(CDlgAdminMapEvent::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapEvent)
	//}}AFX_DATA_INIT

	m_pWndNotify	= NULL;
	m_pInfoMap	= NULL;
	m_pLibInfoMapEvent	= NULL;
}

CDlgAdminMapEvent::~CDlgAdminMapEvent()
{
}

void CDlgAdminMapEvent::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init(pMgrData);

	m_pInfoMap = m_pMgrData->GetMap();
	m_pLibInfoMapEvent = m_pInfoMap->m_pLibInfoMapEvent;

	// ウィンドウ作成
	Create(CDlgAdminMapEvent::IDD, m_pWndParent);
	ShowWindow(SW_SHOW);
}

void CDlgAdminMapEvent::Renew(void)
{
	int i, nCount;
	PCInfoMapEventBase pInfo;
	CString strTmp;

	m_pInfoMap = m_pMgrData->GetMap();
	m_pLibInfoMapEvent = m_pInfoMap->m_pLibInfoMapEvent;

	m_List.DeleteAllItems();

	nCount = m_pLibInfoMapEvent->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapEventBase)m_pLibInfoMapEvent->GetPtr(i);
		strTmp.Format(_T("%d"), pInfo->m_dwMapEventID);	// ID
		m_List.InsertItem(i, strTmp);
		m_List.SetItemData(i, pInfo->m_dwMapEventID);
		strTmp.Format(_T("%d"), pInfo->m_ptPos.x);	// X座標
		m_List.SetItemText(i, 1, strTmp);
		strTmp.Format(_T("%d"), pInfo->m_ptPos.y);	// Y座標
		m_List.SetItemText(i, 2, strTmp);

		strTmp = m_pMgrData->GetMapEventName(pInfo->m_nType);
		m_List.SetItemText(i, 3, strTmp);	// 種別
	}
}

void CDlgAdminMapEvent::OnAdminMsg(int nType, DWORD dwPara)
{
	PCDlgAdminMapEventBase pDlg;

	switch (nType) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:	// 左クリック通知
	case ADMINMSG_NOTIFYTYPE_RBUTTONDOWN:	// 右クリック通知
		pDlg = (PCDlgAdminMapEventBase)m_pWndNotify;
		if (pDlg == NULL) {
			break;
		}
		pDlg->PostMessage(WM_ADMINMSG, (WPARAM)nType, (LPARAM)dwPara);
		break;
	case ADMINMSG_RENEWMAPINFO:	// マップ情報更新
		Renew();
		break;
	}
}

BOOL CDlgAdminMapEvent::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	if (m_pMgrData) {
		m_pMgrData->SetMapEventEditMode(TRUE);
	}

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn(1, _T("ID"),	LVCFMT_LEFT, 50);
	m_List.InsertColumn(2, _T("X座標"),	LVCFMT_LEFT, 50);
	m_List.InsertColumn(3, _T("Y座標"),	LVCFMT_LEFT, 50);
	m_List.InsertColumn(4, _T("イベント種別"),	LVCFMT_LEFT, 200);

	RegisterControl(IDC_RENEW,	LH_CTRL_X);
	RegisterControl(IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	Renew();
	return TRUE;
}

void CDlgAdminMapEvent::OnAdd()
{
	int nResult;
	PCInfoMapEventBase pInfo;
	CPacketADMIN_MAP_EVENT Packet;
	CDlgAdminMapEventBase Dlg(this);

	pInfo = NULL;

	Dlg.Init(m_pMgrData, &m_pWndNotify);
	nResult = Dlg.DoModal();
	if (nResult != IDOK) {
		goto Exit;
	}

	Dlg.Get(pInfo);

	Packet.Make(m_pInfoMap->m_dwMapID, pInfo);
	SendPacket(&Packet);

Exit:
	SAFE_DELETE(pInfo);
	m_pWndNotify = NULL;
}

void CDlgAdminMapEvent::OnModify()
{
	int nResult;
	DWORD dwMapEventID;
	CDlgAdminMapEventBase Dlg(this);
	PCInfoMapEventBase pInfo, pInfoTmp;
	CPacketADMIN_MAP_EVENT Packet;

	nResult = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (nResult < 0) {
		goto Exit;
	}
	dwMapEventID	= m_List.GetItemData(nResult);
	pInfo	= (PCInfoMapEventBase)m_pLibInfoMapEvent->GetPtr(dwMapEventID);
	if (pInfo == NULL) {
		goto Exit;
	}

	Dlg.Init(m_pMgrData, &m_pWndNotify);
	Dlg.SetModify(pInfo);

	nResult = Dlg.DoModal();
	if (nResult != IDOK) {
		goto Exit;
	}

	pInfoTmp = NULL;
	Dlg.Get(pInfoTmp);
	pInfo = m_pLibInfoMapEvent->Renew(dwMapEventID, pInfoTmp);
	SAFE_DELETE(pInfoTmp);

	Packet.Make(m_pInfoMap->m_dwMapID, pInfo);
	SendPacket(&Packet);

Exit:
	m_pWndNotify = NULL;
}

void CDlgAdminMapEvent::OnCopy()
{
#if 0
	int nNo, nResult;
	DWORD dwItemID;
	PCInfoMapEventBase pInfoItem;
	CPacketADMIN_ITEMTYPE_COPY Packet;

	nNo = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}
	dwItemID	= m_List.GetItemData(nNo);
	pInfoItem	= (PCInfoMapEventBase)m_pLibInfoMapEvent->GetPtr(dwItemID);
	if (pInfoItem == NULL) {
		return;
	}

        nResult = MessageBox(_T("選択されているアイテムをコピーしますか？"), _T("確認"), MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	Packet.Make(dwItemID);
	SendPacket(&Packet);
#endif
}

void CDlgAdminMapEvent::OnDelete()
{
	int nResult;
	DWORD dwMapEventID;
	PCInfoMapEventBase pInfo;
	CPacketADMIN_PARA2 Packet;

	nResult = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (nResult < 0) {
		return;
	}
	dwMapEventID	= m_List.GetItemData(nResult);
	pInfo	= (PCInfoMapEventBase)m_pLibInfoMapEvent->GetPtr(dwMapEventID);
	if (pInfo == NULL) {
		return;
	}

        nResult = MessageBox(_T("選択されているイベントを削除しますか？"), _T("確認"), MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	Packet.Make(SBOCOMMANDID_SUB_ADMIN_MAP_DELETEEVENT, m_pInfoMap->m_dwMapID, dwMapEventID);
	SendPacket(&Packet);
}

void CDlgAdminMapEvent::PostNcDestroy()
{
	if (m_pMgrData) {
		m_pMgrData->SetMapEventEditMode(FALSE);
	}
	CDialog::PostNcDestroy();
}

