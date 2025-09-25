/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminCharModifyItem.cpp									 */
/* 内容			:選択キャラ情報編集[所持アイテムの設定]ダイアログクラス 実装ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/23													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "UraraSockTCPSBO.h"
#include "PacketADMIN_ITEM_ADD.h"
#include "PacketADMIN_CHAR_MODIFYITEM.h"
#include "PacketITEM_RENEWITEMINFO.h"
#include "LibInfoItem.h"
#include "Packet.h"
#include "LayoutHelper.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "MgrData.h"
#include "DlgAdminItemNew.h"
#include "DlgAdminSelectItem.h"
#include "DlgAdminCharModifyItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminCharModifyItem::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminCharModifyItem)
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Text(pDX, IDC_STATIC_CHARNAME, m_strCharName);
	DDX_Text(pDX, IDC_STATIC_CHARID, m_strCharID);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminCharModifyItem, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminCharModifyItem)
	ON_BN_CLICKED(IDC_ADDNEW, OnAddnew)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyItem::CDlgAdminCharModifyItem				 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/09/23														 */
/* ========================================================================= */

CDlgAdminCharModifyItem::CDlgAdminCharModifyItem(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminCharModifyItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminCharModifyItem)
	m_strCharName = _T("");
	m_strCharID = _T("");
	//}}AFX_DATA_INIT

	m_pInfoChar		= NULL;
	m_pLibInfoItem	= NULL;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyItem::~CDlgAdminCharModifyItem				 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/09/23														 */
/* ========================================================================= */

CDlgAdminCharModifyItem::~CDlgAdminCharModifyItem()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyItem::Init									 */
/* 内容		:初期化															 */
/* 日付		:2007/09/23														 */
/* ========================================================================= */

void CDlgAdminCharModifyItem::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfoItem = m_pMgrData->GetLibInfoItem ();

	/* ウィンドウ作成 */
	Create (CDlgAdminCharModifyItem::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyItem::Renew									 */
/* 内容		:更新															 */
/* 日付		:2007/09/23														 */
/* ========================================================================= */

void CDlgAdminCharModifyItem::Renew(void)
{
	int i, nCount;
	PCInfoItem pInfoItem;
	CString strTmp;

	m_strCharID.	Empty ();
	m_strCharName.	Empty ();
	m_List.DeleteAllItems ();

	if (m_pInfoChar == NULL) {
		goto Exit;
	}

	m_strCharID.Format(_T("%u"), m_pInfoChar->m_dwCharID);
	m_strCharName = m_pInfoChar->m_strCharName;

	nCount = m_pInfoChar->m_adwItemID.size();
	for (i = 0; i < nCount; i ++) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (m_pInfoChar->m_adwItemID[i]);
		if (pInfoItem == NULL) {
			continue;
		}
		strTmp.Format(_T("%u"), pInfoItem->m_dwItemID);
		m_List.InsertItem (i, strTmp, 0);			/* ID */
		m_List.SetItemData (i, pInfoItem->m_dwItemID);
		strTmp.Format(_T("%s"), (LPCTSTR)pInfoItem->m_strName);
		m_List.SetItemText (i, 1, strTmp);			/* アイテム名 */
		strTmp = m_pLibInfoItem->GetTypeName (pInfoItem->m_dwItemTypeID);
		m_List.SetItemText (i, 2, strTmp);			/* 種別 */
	}

Exit:
	UpdateData (FALSE);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyItem::OnAdminMsg							 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

void CDlgAdminCharModifyItem::OnAdminMsg(int nType, DWORD dwPara)
{
	PCLibInfoCharCli pLibInfoChar;
	CPacketCHAR_REQ_CHARINFO PacketCHAR_REQ_CHARINFO;

	pLibInfoChar	= m_pMgrData->GetLibInfoChar ();
	m_pInfoChar		= (PCInfoCharCli)pLibInfoChar->GetPtr (dwPara);

	PacketCHAR_REQ_CHARINFO.Make (dwPara);
	m_pSock->Send (&PacketCHAR_REQ_CHARINFO);

	Renew ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyItem::OnMainFrame							 */
/* 内容		:メッセージハンドラ(WM_MAINFRAME)								 */
/* 日付		:2007/09/23														 */
/* ========================================================================= */

void CDlgAdminCharModifyItem::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWCHARINFO:		/* キャラ情報更新 */
		if (m_pInfoChar == NULL) {
			break;
		}
		if (m_pInfoChar->m_dwCharID != dwParam) {
			break;
		}
		Renew ();
		break;
	case MAINFRAMEMSG_RENEWITEMINFO:		/* アイテム情報更新 */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyItem::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2007/09/23														 */
/* ========================================================================= */

BOOL CDlgAdminCharModifyItem::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog ();

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (0, _T("ID"), LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, _T("アイテム名"), LVCFMT_LEFT, 120);
	m_List.InsertColumn (2, _T("種別"), LVCFMT_LEFT, 120);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyItem::OnAddnew								 */
/* 内容		:ボタンハンドラ(新規追加)										 */
/* 日付		:2007/09/23														 */
/* ========================================================================= */

void CDlgAdminCharModifyItem::OnAddnew()
{
	BOOL bResult;
	int nResult;
	PCInfoItem pInfoItem;
	CDlgAdminItemNew Dlg(this);
	CPacketADMIN_ITEM_ADD Packet;

	if (m_pInfoChar == NULL) {
		return;
	}
	bResult = m_pInfoChar->IsItemAdd ();
	if (bResult == FALSE) {
		return;
	}

	Dlg.Init (m_pMgrData, NULL, FALSE);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetNew ();
	Dlg.Get (pInfoItem);
	pInfoItem->m_dwCharID = m_pInfoChar->m_dwCharID;

	Packet.Make (pInfoItem);
	m_pSock->Send (&Packet);

	SAFE_DELETE (pInfoItem);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyItem::OnAdd									 */
/* 内容		:ボタンハンドラ(追加)											 */
/* 日付		:2007/09/24														 */
/* ========================================================================= */

void CDlgAdminCharModifyItem::OnAdd()
{
	BOOL bResult;
	int nResult;
	DWORD dwItemID;
	CDlgAdminSelectItem Dlg(this);
	CPacketADMIN_CHAR_MODIFYITEM Packet;

	if (m_pInfoChar == NULL) {
		return;
	}
	bResult = m_pInfoChar->IsItemAdd ();
	if (bResult == FALSE) {
		return;
	}

	Dlg.Init (m_pMgrData);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}
	dwItemID = Dlg.GetSelectItemID ();
	if (dwItemID == 0) {
		return;
	}

	Packet.Make (m_pInfoChar->m_dwCharID, dwItemID, CHARMODIFYITEMTYPE_ADD);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyItem::OnModify								 */
/* 内容		:ボタンハンドラ(編集)											 */
/* 日付		:2007/09/24														 */
/* ========================================================================= */

void CDlgAdminCharModifyItem::OnModify()
{
	int nNo, nResult;
	DWORD dwItemID;
	PCInfoItem pInfoItem;
	CDlgAdminItemNew Dlg(this);
	CPacketITEM_RENEWITEMINFO Packet;

	if (m_pInfoChar == NULL) {
		return;
	}

	nNo = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}
	dwItemID	= m_List.GetItemData (nNo);
	pInfoItem	= (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		return;
	}
	Dlg.Init (m_pMgrData, NULL, FALSE);
	Dlg.SetModify (pInfoItem);

	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}

	Dlg.Get (pInfoItem);

	Packet.Make (pInfoItem);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminCharModifyItem::OnDelete								 */
/* 内容		:ボタンハンドラ(削除)											 */
/* 日付		:2007/09/24														 */
/* ========================================================================= */

void CDlgAdminCharModifyItem::OnDelete()
{
	int nNo;
	DWORD dwItemID;
	CPacketADMIN_CHAR_MODIFYITEM Packet;

	if (m_pInfoChar == NULL) {
		return;
	}

	nNo = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}
	dwItemID = m_List.GetItemData (nNo);

	Packet.Make (m_pInfoChar->m_dwCharID, dwItemID, CHARMODIFYITEMTYPE_DELETE);
	m_pSock->Send (&Packet);
}

/* Copyright(C)URARA-works 2007 */
