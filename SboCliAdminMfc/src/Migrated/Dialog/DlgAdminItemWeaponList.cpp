/// @file DlgAdminItemWeaponList.cpp
/// @brief アイテム武器情報一覧ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/08/10
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_ITEMWEAPON_ADD.h"
#include "PacketADMIN_ITEMWEAPON_RENEW.h"
#include "PacketITEM_REQ_ITEMWEAPONINFO.h"
#include "LibInfoItemWeapon.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminItemWeaponNew.h"
#include "DlgAdminItemWeaponList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminItemWeaponList::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminItemWeaponList)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminItemWeaponList, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminItemWeaponList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDlgAdminItemWeaponList::CDlgAdminItemWeaponList(CWnd* pParent)
	: CDlgAdminBase(CDlgAdminItemWeaponList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminItemWeaponList)
	//}}AFX_DATA_INIT

	m_pLibInfoItemWeapon = NULL;
}

CDlgAdminItemWeaponList::~CDlgAdminItemWeaponList()
{
}

void CDlgAdminItemWeaponList::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init(pMgrData);

	m_pLibInfoItemWeapon = m_pMgrData->GetLibInfoItemWeapon();

	// ウィンドウ作成
	Create(CDlgAdminItemWeaponList::IDD, m_pWndParent);
	ShowWindow(SW_SHOW);
}

void CDlgAdminItemWeaponList::Renew(void)
{
	int i, nCount, nSelect;
	PCInfoItemWeapon pInfoItemWeapon;
	CString strTmp;

	nSelect = m_List.GetNextItem(-1, LVNI_SELECTED);
	m_List.SetRedraw(FALSE);
	m_List.DeleteAllItems();

	nCount = m_pLibInfoItemWeapon->GetCount();
	m_List.SetItemCount(nCount);

	for (i = 0; i < nCount; i ++) {
		pInfoItemWeapon = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetPtr(i);
		if (pInfoItemWeapon == NULL) {
			continue;
		}
		strTmp.Format(_T("%d"), pInfoItemWeapon->m_dwWeaponInfoID);
		m_List.InsertItem(i, strTmp);
		m_List.SetItemData(i, pInfoItemWeapon->m_dwWeaponInfoID);	// ID
		CString strName = (LPCTSTR)pInfoItemWeapon->m_strName;	// 武器種別名
		m_List.SetItemText(i, 1, strName);
	}

	if (nSelect >= 0) {
		m_List.SetItemState(nSelect, LVNI_SELECTED, LVNI_SELECTED);
		m_List.EnsureVisible(nSelect, FALSE);
	}
	m_List.SetRedraw();
}

void CDlgAdminItemWeaponList::OnAdminMsg(int nType, DWORD dwPara)
{
}

BOOL CDlgAdminItemWeaponList::OnInitDialog()
{
	CPacketITEM_REQ_ITEMWEAPONINFO Packet;

	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn(0, _T("ID"),	LVCFMT_LEFT, 40);
	m_List.InsertColumn(1, _T("武器種別名"),	LVCFMT_LEFT, 120);

	RegisterControl(IDC_LIST, LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	// 全武器情報を要求
	Packet.Make(0);
	SendPacket(&Packet);

	return TRUE;
}

void CDlgAdminItemWeaponList::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWITEMWEAPONINFO:	// 武器情報更新
		Renew();
		break;
	}
}

void CDlgAdminItemWeaponList::OnAdd()
{
	int nResult;
	PCInfoItemWeapon pInfoItemWeapon;
	CPacketADMIN_ITEMWEAPON_ADD Packet;
	CDlgAdminItemWeaponNew Dlg(this);

	pInfoItemWeapon = NULL;

	Dlg.Init(m_pMgrData);
	nResult = Dlg.DoModal();
	if (nResult != IDOK) {
		goto Exit;
	}

	pInfoItemWeapon = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetNew();
	Dlg.Get(pInfoItemWeapon);

	Packet.Make(pInfoItemWeapon);
	SendPacket(&Packet);

Exit:
	SAFE_DELETE(pInfoItemWeapon);
}

void CDlgAdminItemWeaponList::OnModify()
{
	int nResult;
	DWORD dwWeaponInfoID;
	CDlgAdminItemWeaponNew Dlg(this);
	PCInfoItemWeapon pInfoItemWeapon;
	CPacketADMIN_ITEMWEAPON_RENEW Packet;

	nResult = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (nResult < 0) {
		goto Exit;
	}
	dwWeaponInfoID	= m_List.GetItemData(nResult);
	pInfoItemWeapon	= (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetPtr(dwWeaponInfoID);
	if (pInfoItemWeapon == NULL) {
		goto Exit;
	}

	Dlg.Init(m_pMgrData);
	Dlg.Set(pInfoItemWeapon);

	nResult = Dlg.DoModal();
	if (nResult != IDOK) {
		goto Exit;
	}

	Dlg.Get(pInfoItemWeapon);

	Packet.Make(pInfoItemWeapon);
	SendPacket(&Packet);

Exit:
	return;
}

void CDlgAdminItemWeaponList::OnCopy()
{
#if 0
	int nNo, nResult;
	DWORD dwItemID;
	PCInfoItem pInfoItem;
	CPacketADMIN_ITEM_COPY Packet;

	nNo = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}
	dwItemID	= m_List.GetItemData(nNo);
	pInfoItem	= (PCInfoItem)m_pLibInfoItem->GetPtr(dwItemID);
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

void CDlgAdminItemWeaponList::OnDelete()
{
#if 0
	int nResult;
	DWORD dwItemID;
	PCInfoItem pInfoItem;
	CString strTmp;
	CPacketADMIN_ITEM_DELETE Packet;

	nResult = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (nResult < 0) {
		return;
	}
	dwItemID	= m_List.GetItemData(nResult);
	pInfoItem	= (PCInfoItem)m_pLibInfoItem->GetPtr(dwItemID);
	if (pInfoItem == NULL) {
		return;
	}

        strTmp.Format(_T("[%s]を削除しますか？"), (LPCTSTR)pInfoItem->m_strName);
        nResult = MessageBox(strTmp, _T("確認"), MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	Packet.Make(dwItemID);
	SendPacket(&Packet);
#endif
}

