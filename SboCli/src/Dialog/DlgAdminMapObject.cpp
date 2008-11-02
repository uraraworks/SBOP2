/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapObject.cpp										 */
/* 内容			:マップイベント一覧ダイアログクラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/09/07													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "PacketADMIN_MAP_RENEWMAPOBJECT.h"
#include "UraraSockTCPSBO.h"
#include "LibInfoMapObject.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "DlgAdminMapObjectEdit.h"
#include "DlgAdminMapObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminMapObject::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapObject)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapObject, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapObject)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObject::CDlgAdminMapObject							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

CDlgAdminMapObject::CDlgAdminMapObject(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapObject::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapObject)
	//}}AFX_DATA_INIT

	m_pWndNotify		= NULL;
	m_pLibInfoMapObject	= NULL;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObject::~CDlgAdminMapObject						 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

CDlgAdminMapObject::~CDlgAdminMapObject()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObject::Init										 */
/* 内容		:初期化															 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

void CDlgAdminMapObject::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pLibInfoMapObject = m_pMgrData->GetLibInfoMapObject ();
	m_pMgrData->GetMgrGrpData ()->ReadMapPartsTmp ();

	/* ウィンドウ作成 */
	Create (CDlgAdminMapObject::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObject::OnAdminMsg									 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CDlgAdminMapObject::OnAdminMsg(int nType, DWORD dwPara)
{
	switch (nType) {
	case ADMINMSG_RENEWMAPOBJECT:	/* マップオブジェクト情報更新 */
		Renew ();
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObject::Renew										 */
/* 内容		:一覧を更新														 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

void CDlgAdminMapObject::Renew(void)
{
	int i, nCount;
	PCInfoMapObject pInfo;
	CString strTmp;

	m_List.DeleteAllItems ();

	nCount = m_pLibInfoMapObject->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapObject)m_pLibInfoMapObject->GetPtr (i);
		strTmp.Format ("%d", pInfo->m_dwObjectID);				/* ID */
		m_List.InsertItem (i, strTmp);
		m_List.SetItemData (i, pInfo->m_dwObjectID);
		m_List.SetItemText (i, 1, (LPCSTR)pInfo->m_strName);	/* オブジェクト名 */
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObject::OnInitDialog								 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

BOOL CDlgAdminMapObject::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (1, "ID",				LVCFMT_LEFT, 50);
	m_List.InsertColumn (2, "オブジェクト名",	LVCFMT_LEFT, 200);

	RegisterControl (IDC_RENEW,	LH_CTRL_X);
	RegisterControl (IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	Renew ();
	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObject::OnAdd										 */
/* 内容		:ボタンハンドラ(新規追加)										 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

void CDlgAdminMapObject::OnAdd()
{
	int nResult;
	CPacketADMIN_MAP_RENEWMAPOBJECT Packet;
	CDlgAdminMapObjectEdit Dlg(this);

	Dlg.Init (m_pMgrData);
	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}
	Packet.Make (Dlg.m_pInfoMapObject);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObject::OnModify									 */
/* 内容		:ボタンハンドラ(編集)											 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

void CDlgAdminMapObject::OnModify()
{
	int nResult;
	DWORD dwObjectID;
	PCInfoMapObject pInfo;
	CPacketADMIN_MAP_RENEWMAPOBJECT Packet;
	CDlgAdminMapObjectEdit Dlg(this);

	nResult = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nResult < 0) {
		return;
	}
	dwObjectID = m_List.GetItemData (nResult);
	pInfo = (PCInfoMapObject)m_pLibInfoMapObject->GetPtr (dwObjectID);

	Dlg.Init (m_pMgrData);
	Dlg.SetData (pInfo);

	nResult = Dlg.DoModal ();
	if (nResult != IDOK) {
		return;
	}
	Packet.Make (Dlg.m_pInfoMapObject);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObject::OnCopy										 */
/* 内容		:ボタンハンドラ(コピー)											 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

void CDlgAdminMapObject::OnCopy()
{
#if 0
	int nNo, nResult;
	DWORD dwItemID;
	PCInfoMapEventBase pInfoItem;
	CPacketADMIN_ITEMTYPE_COPY Packet;

	nNo = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nNo < 0) {
		return;
	}
	dwItemID	= m_List.GetItemData (nNo);
	pInfoItem	= (PCInfoMapEventBase)m_pLibInfoMapEvent->GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		return;
	}

	nResult = MessageBox ("選択されているアイテムをコピーしますか？", "確認", MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	Packet.Make (dwItemID);
	m_pSock->Send (&Packet);
#endif
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObject::OnDelete									 */
/* 内容		:ボタンハンドラ(削除)											 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

void CDlgAdminMapObject::OnDelete()
{
#if 0
	int nResult;
	DWORD dwMapEventID;
	PCInfoMapEventBase pInfo;
	CPacketADMIN_PARA2 Packet;

	nResult = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nResult < 0) {
		return;
	}
	dwMapEventID	= m_List.GetItemData (nResult);
	pInfo			= (PCInfoMapEventBase)m_pLibInfoMapEvent->GetPtr (dwMapEventID);
	if (pInfo == NULL) {
		return;
	}

	nResult = MessageBox ("選択されているイベントを削除しますか？", "確認", MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	Packet.Make (SBOCOMMANDID_SUB_ADMIN_MAP_DELETEEVENT, m_pInfoMap->m_dwMapID, dwMapEventID);
	m_pSock->Send (&Packet);
#endif
}

/* Copyright(C)URARA-works 2008 */
