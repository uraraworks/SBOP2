/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:DlgAdminItemTypeList.cpp									 */
/* 内容			:アイテム種別一覧ダイアログクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/28													 */
/* ========================================================================= */

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

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

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


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeList::CDlgAdminItemTypeList					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/09/28														 */
/* ========================================================================= */

CDlgAdminItemTypeList::CDlgAdminItemTypeList(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminItemTypeList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminItemTypeList)
	//}}AFX_DATA_INIT

	m_pWndNotify	= NULL;
	m_pLibInfoItemType	= NULL;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeList::~CDlgAdminItemTypeList					 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/09/28														 */
/* ========================================================================= */

CDlgAdminItemTypeList::~CDlgAdminItemTypeList()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeList::Init									 */
/* 内容		:初期化															 */
/* 日付		:2007/09/28														 */
/* ========================================================================= */

void CDlgAdminItemTypeList::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfoItemType = m_pMgrData->GetLibInfoItemType ();

	/* ウィンドウ作成 */
	Create (CDlgAdminItemTypeList::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeList::Renew									 */
/* 内容		:一覧を更新														 */
/* 日付		:2007/09/28														 */
/* ========================================================================= */

void CDlgAdminItemTypeList::Renew(void)
{
	int i, nCount;
	PCInfoItemTypeBase pInfoItemType;
	CString strTmp;

	m_List.DeleteAllItems ();

	nCount = m_pLibInfoItemType->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (i);
		strTmp.Format ("%d", pInfoItemType->m_dwTypeID);
		m_List.InsertItem (i, strTmp);
		m_List.SetItemData (i, pInfoItemType->m_dwTypeID);		/* ID */
		m_List.SetItemText (i, 1, pInfoItemType->m_strName);	/* アイテム名 */

		strTmp = m_pLibInfoItemType->GetTypeName (pInfoItemType->m_dwItemTypeID);
		m_List.SetItemText (i, 2, strTmp);					/* 種別 */
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeList::OnAdminMsg								 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2007/09/28														 */
/* ========================================================================= */

void CDlgAdminItemTypeList::OnAdminMsg(int nType, DWORD dwPara)
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeList::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2007/09/28														 */
/* ========================================================================= */

BOOL CDlgAdminItemTypeList::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (0, "ID",			LVCFMT_LEFT, 40);
	m_List.InsertColumn (1, "アイテム名",	LVCFMT_LEFT, 120);
	m_List.InsertColumn (2, "種別",			LVCFMT_LEFT, 120);

	RegisterControl (IDC_RENEW,	LH_CTRL_X);
	RegisterControl (IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	/* 全アイテム情報を要求 */
	OnRenew ();

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeList::OnMainFrame								 */
/* 内容		:メッセージハンドラ(WM_MAINFRAME)								 */
/* 日付		:2007/09/28														 */
/* ========================================================================= */

void CDlgAdminItemTypeList::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	switch (dwCommand) {
	case MAINFRAMEMSG_RENEWITEMTYPEINFO:		/* アイテム種別情報更新 */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeList::OnAdd									 */
/* 内容		:ボタンハンドラ(新規追加)										 */
/* 日付		:2007/09/28														 */
/* ========================================================================= */

void CDlgAdminItemTypeList::OnAdd()
{
	int nResult;
	PCInfoItemTypeBase pInfoItem;
	CPacketADMIN_ITEMTYPE_ADD Packet;
	CDlgAdminItemTypeNew Dlg(this);

	pInfoItem = NULL;

	Dlg.Init (m_pMgrData, &m_pWndNotify);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		goto Exit;
	}

	Dlg.Get (pInfoItem);

	Packet.Make (pInfoItem);
	m_pSock->Send (&Packet);

Exit:
	SAFE_DELETE (pInfoItem);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeList::OnModify								 */
/* 内容		:ボタンハンドラ(編集)											 */
/* 日付		:2007/09/28														 */
/* ========================================================================= */

void CDlgAdminItemTypeList::OnModify()
{
	int nResult;
	DWORD dwItemID;
	CDlgAdminItemTypeNew Dlg(this);
	PCInfoItemTypeBase pInfoItem, pInfoItemTmp;
	CPacketITEM_RENEWITEMTYPEINFO Packet;

	nResult = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nResult < 0) {
		goto Exit;
	}
	dwItemID	= m_List.GetItemData (nResult);
	pInfoItem	= (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		goto Exit;
	}

	Dlg.Init (m_pMgrData, &m_pWndNotify);
	Dlg.SetModify (pInfoItem);

	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		goto Exit;
	}

	pInfoItemTmp = NULL;
	Dlg.Get (pInfoItemTmp);
	pInfoItem->Copy (pInfoItemTmp);
	SAFE_DELETE (pInfoItemTmp);

	Packet.Make (pInfoItem);
	m_pSock->Send (&Packet);

Exit:
	return;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeList::OnCopy									 */
/* 内容		:ボタンハンドラ(コピー)											 */
/* 日付		:2007/09/28														 */
/* ========================================================================= */

void CDlgAdminItemTypeList::OnCopy()
{
	int nNo, nResult;
	DWORD dwItemID;
	PCInfoItemTypeBase pInfoItem;
	CPacketADMIN_ITEMTYPE_COPY Packet;

	nNo = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}
	dwItemID	= m_List.GetItemData (nNo);
	pInfoItem	= (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (dwItemID);
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
/* 関数名	:CDlgAdminItemTypeList::OnDelete								 */
/* 内容		:ボタンハンドラ(削除)											 */
/* 日付		:2007/09/28														 */
/* ========================================================================= */

void CDlgAdminItemTypeList::OnDelete()
{
	int nResult;
	DWORD dwItemID;
	PCInfoItemTypeBase pInfoItem;
	CString strTmp;
	CPacketADMIN_ITEMTYPE_DELETE Packet;

	nResult = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nResult < 0) {
		return;
	}
	dwItemID	= m_List.GetItemData (nResult);
	pInfoItem	= (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (dwItemID);
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
/* 関数名	:CDlgAdminItemTypeList::OnRenew									 */
/* 内容		:ボタンハンドラ(更新)											 */
/* 日付		:2007/09/28														 */
/* ========================================================================= */

void CDlgAdminItemTypeList::OnRenew()
{
	CPacketITEM_REQ_ITEMTYPEINFO Packet;

	/* 全アイテム種別情報を要求 */
	Packet.Make (0);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminItemTypeList::GetIndex								 */
/* 内容		:アイテムIDからインデックスを取得								 */
/* 日付		:2007/09/28														 */
/* ========================================================================= */

int CDlgAdminItemTypeList::GetIndex(DWORD dwItemID)
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
