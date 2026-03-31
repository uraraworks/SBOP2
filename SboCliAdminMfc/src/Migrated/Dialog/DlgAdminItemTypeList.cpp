/// @file DlgAdminItemTypeList.cpp
/// @brief アイテム種別一覧ダイアログクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/09/28
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "resource.h"
#include "PacketADMIN_ITEMTYPE_ADD.h"
#include "PacketADMIN_ITEMTYPE_COPY.h"
#include "PacketADMIN_ITEMTYPE_DELETE.h"
#include "PacketITEM_REQ_ITEMTYPEINFO.h"
#include "PacketITEM_RENEWITEMTYPEINFO.h"
#include "UraraSockTCPSBO.h"
#include "LibInfoItemType.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "DlgAdminItemTypeNew.h"
#include "DlgAdminItemTypeList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

void CDlgAdminItemTypeList::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminItemTypeList)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminItemTypeList, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminItemTypeList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_RENEW, OnRenew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDlgAdminItemTypeList::CDlgAdminItemTypeList(CWnd* pParent)
	: CDlgAdminBase(CDlgAdminItemTypeList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminItemTypeList)
	//}}AFX_DATA_INIT

	m_pWndNotify	= NULL;
	m_pLibInfoItemType	= NULL;
}

CDlgAdminItemTypeList::~CDlgAdminItemTypeList()
{
}

void CDlgAdminItemTypeList::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init(pMgrData);

	m_pLibInfoItemType = m_pMgrData->GetLibInfoItemType();

	// ウィンドウ作成
	Create(CDlgAdminItemTypeList::IDD, m_pWndParent);
	ShowWindow(SW_SHOW);
}

void CDlgAdminItemTypeList::Renew(void)
{
	int i, nCount;
	PCInfoItemTypeBase pInfoItemType;
	CString strTmp;

	m_List.DeleteAllItems();

	nCount = m_pLibInfoItemType->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr(i);
		strTmp.Format(_T("%d"), pInfoItemType->m_dwTypeID);
		m_List.InsertItem(i, strTmp);
		m_List.SetItemData(i, pInfoItemType->m_dwTypeID);	// ID
		CString strName = (LPCTSTR)pInfoItemType->m_strName;	// アイテム名
		m_List.SetItemText(i, 1, strName);	// アイテム名

		strTmp = m_pLibInfoItemType->GetTypeName(pInfoItemType->m_dwItemTypeID);
		m_List.SetItemText(i, 2, strTmp);	// 種別
	}
}

void CDlgAdminItemTypeList::OnAdminMsg(int nType, DWORD dwPara)
{
}

BOOL CDlgAdminItemTypeList::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn(0, _T("ID"),	LVCFMT_LEFT, 40);
	m_List.InsertColumn(1, _T("アイテム名"),	LVCFMT_LEFT, 120);
	m_List.InsertColumn(2, _T("種別"),	LVCFMT_LEFT, 120);

	RegisterControl(IDC_RENEW,	LH_CTRL_X);
	RegisterControl(IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	// 全アイテム情報を要求
	OnRenew();

	return TRUE;
}

void CDlgAdminItemTypeList::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWITEMTYPEINFO:	// アイテム種別情報更新
		Renew();
		break;
	}
}

void CDlgAdminItemTypeList::OnAdd()
{
	int nResult;
	PCInfoItemTypeBase pInfoItem;
	CPacketADMIN_ITEMTYPE_ADD Packet;
	CDlgAdminItemTypeNew Dlg(this);

	pInfoItem = NULL;

	Dlg.Init(m_pMgrData, &m_pWndNotify);
	nResult = Dlg.DoModal();
	if (nResult != IDOK) {
		goto Exit;
	}

	Dlg.Get(pInfoItem);

	Packet.Make(pInfoItem);
	SendPacket(&Packet);

Exit:
	SAFE_DELETE(pInfoItem);
}

void CDlgAdminItemTypeList::OnModify()
{
	int nResult;
	DWORD dwItemID;
	CDlgAdminItemTypeNew Dlg(this);
	PCInfoItemTypeBase pInfoItem, pInfoItemTmp;
	CPacketITEM_RENEWITEMTYPEINFO Packet;

	nResult = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (nResult < 0) {
		goto Exit;
	}
	dwItemID	= m_List.GetItemData(nResult);
	pInfoItem	= (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr(dwItemID);
	if (pInfoItem == NULL) {
		goto Exit;
	}

	Dlg.Init(m_pMgrData, &m_pWndNotify);
	Dlg.SetModify(pInfoItem);

	nResult = Dlg.DoModal();
	if (nResult != IDOK) {
		goto Exit;
	}

	pInfoItemTmp = NULL;
	Dlg.Get(pInfoItemTmp);
	pInfoItem->Copy(pInfoItemTmp);
	SAFE_DELETE(pInfoItemTmp);

	Packet.Make(pInfoItem);
	SendPacket(&Packet);

Exit:
	return;
}

void CDlgAdminItemTypeList::OnCopy()
{
	int nNo, nResult;
	DWORD dwItemID;
	PCInfoItemTypeBase pInfoItem;
	CPacketADMIN_ITEMTYPE_COPY Packet;

	nNo = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}
	dwItemID	= m_List.GetItemData(nNo);
	pInfoItem	= (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr(dwItemID);
	if (pInfoItem == NULL) {
		return;
	}

        nResult = MessageBox(_T("選択されているアイテムをコピーしますか？"), _T("確認"), MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	Packet.Make(dwItemID);
	SendPacket(&Packet);
}

void CDlgAdminItemTypeList::OnDelete()
{
	int nResult;
	DWORD dwItemID;
	PCInfoItemTypeBase pInfoItem;
	CString strTmp;
	CPacketADMIN_ITEMTYPE_DELETE Packet;

	nResult = m_List.GetNextItem(-1, LVNI_SELECTED);
	if (nResult < 0) {
		return;
	}
	dwItemID	= m_List.GetItemData(nResult);
	pInfoItem	= (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr(dwItemID);
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
}

void CDlgAdminItemTypeList::OnRenew()
{
	CPacketITEM_REQ_ITEMTYPEINFO Packet;

	// 全アイテム種別情報を要求
	Packet.Make(0);
	SendPacket(&Packet);
}

int CDlgAdminItemTypeList::GetIndex(DWORD dwItemID)
{
	int nRet, i, nCount;
	DWORD dwItemData;

	nRet = -1;

	nCount = m_List.GetItemCount();
	for (i = 0; i < nCount; i ++) {
		dwItemData = m_List.GetItemData(i);
		if (dwItemData == dwItemID) {
			nRet = i;
			break;
		}
	}

	return nRet;
}

