/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminItemList.cpp										 */
/* 内容			:アイテム一覧ダイアログクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/08/14													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "PacketADMIN_ITEM_ADD.h"
#include "PacketADMIN_ITEM_COPY.h"
#include "PacketADMIN_ITEM_DELETE.h"
#include "PacketITEM_REQ_ITEMINFO.h"
#include "PacketITEM_RENEWITEMINFO.h"
#include "UraraSockTCPSBO.h"
#include "LibInfoItemType.h"
#include "LibInfoItem.h"
#include "LayoutHelper.h"
#include "InfoCharCli.h"
#include "LibInfoCharCli.h"
#include "MgrData.h"
#include "DlgAdminItemNew.h"
#include "DlgAdminItemList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminItemList::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminItemList)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminItemList, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminItemList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_RENEW, OnRenew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminItemList::CDlgAdminItemList							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/08/14														 */
/* ========================================================================= */

CDlgAdminItemList::CDlgAdminItemList(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminItemList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminItemList)
	//}}AFX_DATA_INIT

	m_pWndNotify		= NULL;
	m_pLibInfoItemType	= NULL;
	m_pLibInfoItem		= NULL;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemList::~CDlgAdminItemList							 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/08/14														 */
/* ========================================================================= */

CDlgAdminItemList::~CDlgAdminItemList()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemList::Init										 */
/* 内容		:初期化															 */
/* 日付		:2007/08/14														 */
/* ========================================================================= */

void CDlgAdminItemList::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfoItemType	= m_pMgrData->GetLibInfoItemType ();
	m_pLibInfoItem		= m_pMgrData->GetLibInfoItem ();

	/* ウィンドウ作成 */
	Create (CDlgAdminItemList::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemList::Renew										 */
/* 内容		:一覧を更新														 */
/* 日付		:2007/08/14														 */
/* ========================================================================= */

void CDlgAdminItemList::Renew(void)
{
	int i, nCount, nSelect;
	PCInfoItem pInfoItem;
	PCInfoCharCli pInfoChar;
	PCLibInfoCharCli pLibInfoChar;
	CString strTmp;

	pLibInfoChar = m_pMgrData->GetLibInfoChar ();

	nSelect = m_List.GetNextItem (-1, LVNI_SELECTED);
	m_List.SetRedraw (FALSE);
	m_List.DeleteAllItems ();

	nCount = m_pLibInfoItem->GetCount ();
	m_List.SetItemCount (nCount);

	for (i = 0; i < nCount; i ++) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtr (i);
		if (pInfoItem == NULL) {
			continue;
		}
		strTmp.Format ("%d", pInfoItem->m_dwItemID);
		m_List.InsertItem (i, strTmp);
		m_List.SetItemData (i, pInfoItem->m_dwItemID);		/* ID */
		m_List.SetItemText (i, 1, pInfoItem->m_strName);	/* アイテム名 */

		strTmp = m_pLibInfoItemType->GetTypeNameTypeID (pInfoItem->m_dwItemTypeID);
		m_List.SetItemText (i, 2, strTmp);					/* 種別 */

		strTmp.Empty ();
		pInfoChar = (PCInfoCharCli)pLibInfoChar->GetPtr (pInfoItem->m_dwCharID);
		if (pInfoChar) {
			strTmp.Format ("[%u]%s", pInfoChar->m_dwCharID, (LPCSTR)pInfoChar->m_strCharName);
		} else {
			if (pInfoItem->m_dwCharID != 0) {
				strTmp.Format ("[%u]", pInfoItem->m_dwCharID);
			}
		}
		m_List.SetItemText (i, 3, strTmp);					/* 所持キャラ */

		strTmp.Empty ();
		if (pInfoItem->m_dwMapID) {
			strTmp.Format ("MAPID:%u (%d, %d)", pInfoItem->m_dwMapID, pInfoItem->m_ptPos.x, pInfoItem->m_ptPos.y);
		}
		m_List.SetItemText (i, 4, strTmp);					/* 配置座標 */
	}

	if (nSelect >= 0) {
		m_List.SetItemState (nSelect, LVNI_SELECTED, LVNI_SELECTED);
		m_List.EnsureVisible (nSelect, FALSE);
	}
	m_List.SetRedraw ();
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemList::OnAdminMsg									 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2007/08/23														 */
/* ========================================================================= */

void CDlgAdminItemList::OnAdminMsg(int nType, DWORD dwPara)
{
	int nResult;

	switch (nType) {
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:	/* 左クリック通知 */
		switch (m_pMgrData->GetAdminNotifyTypeL ()) {
		case ADMINNOTIFYTYPE_ITEMID:			/* アイテムID */
			nResult = GetIndex (dwPara);
			if (nResult < 0) {
				break;
			}
			/* クリックされたアイテムを選択 */
			m_List.SetItemState (nResult, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			break;

		case ADMINNOTIFYTYPE_CHARPOS:			/* キャラ座標 */
			{
				PCInfoCharCli pInfoChar;
				PCDlgAdminItemNew pDlg;

				if (m_pWndNotify == NULL) {
					break;
				}
				pDlg = (PCDlgAdminItemNew)m_pWndNotify;
				pInfoChar = m_pMgrData->GetPlayerChar ();

				pDlg->m_nMapID	= pInfoChar->m_dwMapID;
				pDlg->m_nPosX	= HIWORD (dwPara);
				pDlg->m_nPosY	= LOWORD (dwPara);
				pDlg->UpdateData (FALSE);
			}
			break;
		}
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemList::OnInitDialog								 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2007/08/14														 */
/* ========================================================================= */

BOOL CDlgAdminItemList::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (0, "ID",			LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, "アイテム名",	LVCFMT_LEFT, 120);
	m_List.InsertColumn (2, "種別",			LVCFMT_LEFT, 120);
	m_List.InsertColumn (3, "所持キャラ",	LVCFMT_LEFT, 120);
	m_List.InsertColumn (4, "配置座標",		LVCFMT_LEFT, 120);

	RegisterControl (IDC_RENEW,	LH_CTRL_X);
	RegisterControl (IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	/* 全アイテム情報を要求 */
	OnRenew ();

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemList::OnMainFrame									 */
/* 内容		:メッセージハンドラ(WM_MAINFRAME)								 */
/* 日付		:2007/08/16														 */
/* ========================================================================= */

void CDlgAdminItemList::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWITEMINFO:		/* アイテム情報更新 */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemList::OnAdd										 */
/* 内容		:ボタンハンドラ(新規追加)										 */
/* 日付		:2007/08/15														 */
/* ========================================================================= */

void CDlgAdminItemList::OnAdd()
{
	int nResult;
	PCInfoItem pInfoItem;
	CPacketADMIN_ITEM_ADD Packet;
	CDlgAdminItemNew Dlg(this);

	nResult = m_pLibInfoItemType->GetCount ();
	if (nResult <= 0) {
		MessageBox ("アイテム種別が無いので追加できません", "エラー", MB_OK | MB_ICONINFORMATION);
		return;
	}

	pInfoItem = NULL;
	m_pMgrData->SetAdminNotifyTypeL (ADMINNOTIFYTYPE_CHARPOS);

	Dlg.Init (m_pMgrData, &m_pWndNotify);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		goto Exit;
	}

	pInfoItem = (PCInfoItem)m_pLibInfoItem->GetNew ();
	Dlg.Get (pInfoItem);

	Packet.Make (pInfoItem);
	m_pSock->Send (&Packet);

Exit:
	m_pMgrData->SetAdminNotifyTypeL (ADMINNOTIFYTYPE_ITEMID);
	m_pWndNotify = NULL;
	SAFE_DELETE (pInfoItem);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemList::OnModify									 */
/* 内容		:ボタンハンドラ(編集)											 */
/* 日付		:2007/08/16														 */
/* ========================================================================= */

void CDlgAdminItemList::OnModify()
{
	int nResult;
	DWORD dwItemID;
	CDlgAdminItemNew Dlg(this);
	PCInfoItem pInfoItem;
	CPacketITEM_RENEWITEMINFO Packet;

	m_pMgrData->SetAdminNotifyTypeL (ADMINNOTIFYTYPE_CHARPOS);

	nResult = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nResult < 0) {
		goto Exit;
	}
	dwItemID	= m_List.GetItemData (nResult);
	pInfoItem	= (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		goto Exit;
	}

	Dlg.Init (m_pMgrData, &m_pWndNotify);
	Dlg.SetModify (pInfoItem);

	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		goto Exit;
	}

	Dlg.Get (pInfoItem);

	Packet.Make (pInfoItem);
	m_pSock->Send (&Packet);

Exit:
	m_pMgrData->SetAdminNotifyTypeL (ADMINNOTIFYTYPE_ITEMID);
	m_pWndNotify = NULL;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemList::OnCopy										 */
/* 内容		:ボタンハンドラ(コピー)											 */
/* 日付		:2007/09/22														 */
/* ========================================================================= */

void CDlgAdminItemList::OnCopy()
{
	int nNo, nResult;
	DWORD dwItemID;
	PCInfoItem pInfoItem;
	CPacketADMIN_ITEM_COPY Packet;

	nNo = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}
	dwItemID	= m_List.GetItemData (nNo);
	pInfoItem	= (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		return;
	}

	nResult = MessageBox ("選択されているアイテムをコピーしますか？", "確認", MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	Packet.Make (dwItemID);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemList::OnDelete									 */
/* 内容		:ボタンハンドラ(削除)											 */
/* 日付		:2007/08/19														 */
/* ========================================================================= */

void CDlgAdminItemList::OnDelete()
{
	int nResult;
	DWORD dwItemID;
	PCInfoItem pInfoItem;
	CString strTmp;
	CPacketADMIN_ITEM_DELETE Packet;

	nResult = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nResult < 0) {
		return;
	}
	dwItemID	= m_List.GetItemData (nResult);
	pInfoItem	= (PCInfoItem)m_pLibInfoItem->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		return;
	}

	strTmp.Format ("[%s]を削除しますか？", (LPCSTR)pInfoItem->m_strName);
	nResult = MessageBox (strTmp, "確認", MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	Packet.Make (dwItemID);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemList::OnRenew										 */
/* 内容		:ボタンハンドラ(更新)											 */
/* 日付		:2007/09/08														 */
/* ========================================================================= */

void CDlgAdminItemList::OnRenew()
{
	CPacketITEM_REQ_ITEMINFO Packet;

	/* 全アイテム情報を要求 */
	Packet.Make (0);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemList::GetIndex									 */
/* 内容		:アイテムIDからインデックスを取得								 */
/* 日付		:2007/08/23														 */
/* ========================================================================= */

int CDlgAdminItemList::GetIndex(DWORD dwItemID)
{
	int nRet, i, nCount;
	DWORD dwItemData;

	nRet = -1;

	nCount = m_List.GetItemCount ();
	for (i = 0; i < nCount; i ++) {
		dwItemData = m_List.GetItemData (i);
		if (dwItemData == dwItemID) {
			nRet = i;
			break;
		}
	}

	return nRet;
}

/* Copyright(C)URARA-works 2007 */
