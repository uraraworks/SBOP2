/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:DlgAdminMapObjectData.cpp									 */
/* 内容			:マップオブジェクト配置データ一覧ダイアログクラス 実装ファイル */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/11/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "resource.h"
#include "PacketADMIN_MAP_RENEWOBJECTDATA.h"
#include "PacketADMIN_PARA2.h"
#include "UraraSockTCPSBO.h"
#include "LibInfoMapObject.h"
#include "LibInfoMapObjectData.h"
#include "InfoMapBase.h"
#include "Command.h"
#include "LayoutHelper.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "DlgAdminMapObjectDataEdit.h"
#include "DlgAdminMapObjectData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

void CDlgAdminMapObjectData::DoDataExchange(CDataExchange* pDX)
{
	CDlgAdminBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAdminMapObjectData)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdminMapObjectData, CDlgAdminBase)
	//{{AFX_MSG_MAP(CDlgAdminMapObjectData)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectData::CDlgAdminMapObjectData					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

CDlgAdminMapObjectData::CDlgAdminMapObjectData(CWnd* pParent /*=NULL*/)
	: CDlgAdminBase(CDlgAdminMapObjectData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAdminMapObjectData)
	//}}AFX_DATA_INIT

	m_pWndNotify			= NULL;
	m_pLibInfoMapObject		= NULL;
	m_pLibInfoMapObjectData	= NULL;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectData::~CDlgAdminMapObjectData				 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

CDlgAdminMapObjectData::~CDlgAdminMapObjectData()
{
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectData::Init									 */
/* 内容		:初期化															 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectData::Init(CMgrData *pMgrData)
{
	CDlgAdminBase::Init (pMgrData);

	m_pInfoMap = m_pMgrData->GetMap ();
	m_pLibInfoMapObject		= m_pMgrData->GetLibInfoMapObject ();
	m_pLibInfoMapObjectData = m_pInfoMap->m_pLibInfoMapObjectData;

	/* ウィンドウ作成 */
	Create (CDlgAdminMapObjectData::IDD, m_pWndParent);
	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectData::OnAdminMsg								 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2008/11/01														 */
/* ========================================================================= */

void CDlgAdminMapObjectData::OnAdminMsg(int nType, DWORD dwPara)
{
	PCDlgAdminMapObjectDataEdit pDlg;

	switch (nType) {
	case ADMINMSG_RENEWMAPINFO:					/* マップ情報更新 */
		Renew ();
		break;
	case ADMINMSG_NOTIFYTYPE_LBUTTONDOWN:		/* 左クリック通知 */
		pDlg = (PCDlgAdminMapObjectDataEdit)m_pWndNotify;
		if (pDlg == NULL) {
			break;
		}
		pDlg->PostMessage (WM_ADMINMSG, (WPARAM)nType, (LPARAM)dwPara);
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectData::Renew									 */
/* 内容		:一覧を更新														 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectData::Renew(void)
{
	int i, nCount, nCountTmp;
	PCInfoMapObjectData pInfo;
	PCInfoMapObject pInfoMapObject;
	CString strTmp;

	m_pInfoMap = m_pMgrData->GetMap ();
	m_pLibInfoMapObject		= m_pMgrData->GetLibInfoMapObject ();
	m_pLibInfoMapObjectData = m_pInfoMap->m_pLibInfoMapObjectData;

	m_List.DeleteAllItems ();

	nCountTmp = 0;
	nCount = m_pLibInfoMapObjectData->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapObjectData)m_pLibInfoMapObjectData->GetPtr (i);
		pInfoMapObject = (PCInfoMapObject)m_pLibInfoMapObject->GetPtr (pInfo->m_dwObjectID);
		if (pInfoMapObject == NULL) {
			continue;
		}
		strTmp.Format ("%d", pInfo->m_dwDataID);				/* ID */
		m_List.InsertItem (nCountTmp, strTmp);
		m_List.SetItemData (nCountTmp, pInfo->m_dwDataID);
		strTmp.Format ("%d", pInfo->m_ptPos.x);					/* X座標 */
		m_List.SetItemText (nCountTmp, 1, strTmp);
		strTmp.Format ("%d", pInfo->m_ptPos.y);					/* Y座標 */
		m_List.SetItemText (nCountTmp, 2, strTmp);
		m_List.SetItemText (nCountTmp, 3, (LPCSTR)pInfoMapObject->m_strName);	/* オブジェクト名 */
		nCountTmp ++;
	}
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectData::OnInitDialog							 */
/* 内容		:メッセージハンドラ(WM_INITDIALOG)								 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

BOOL CDlgAdminMapObjectData::OnInitDialog()
{
	CDlgAdminBase::OnInitDialog();

	m_List.SetExtendedStyle (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_List.InsertColumn (1, "ID",				LVCFMT_LEFT, 50);
	m_List.InsertColumn (2, "X座標",			LVCFMT_LEFT, 50);
	m_List.InsertColumn (3, "Y座標",			LVCFMT_LEFT, 50);
	m_List.InsertColumn (4, "オブジェクト名",	LVCFMT_LEFT, 200);

	RegisterControl (IDC_RENEW,	LH_CTRL_X);
	RegisterControl (IDC_LIST,	LH_CTRL_WIDTH | LH_CTRL_HEIGHT);

	Renew ();
	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectData::OnAdd									 */
/* 内容		:ボタンハンドラ(新規追加)										 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectData::OnAdd()
{
	int nResult;
	PCInfoMapObjectData pInfo;
	CPacketADMIN_MAP_RENEWOBJECTDATA Packet;
	CDlgAdminMapObjectDataEdit Dlg(this);

	pInfo = NULL;

	Dlg.Init (m_pMgrData, &m_pWndNotify);
	nResult = Dlg.DoModal ();
	m_pWndNotify = NULL;
	if (nResult != IDOK) {
		goto Exit;
	}
	pInfo = (PCInfoMapObjectData)m_pLibInfoMapObjectData->GetNew (); 
	Dlg.Get (pInfo);

	Packet.Make (m_pInfoMap->m_dwMapID, pInfo);
	m_pSock->Send (&Packet);

Exit:
	SAFE_DELETE (pInfo);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectData::OnModify								 */
/* 内容		:ボタンハンドラ(編集)											 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectData::OnModify()
{
	int nResult;
	DWORD dwDataID;
	PCInfoMapObjectData pInfo;
	CPacketADMIN_MAP_RENEWOBJECTDATA Packet;
	CDlgAdminMapObjectDataEdit Dlg(this);

	nResult = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nResult < 0) {
		return;
	}
	dwDataID	= m_List.GetItemData (nResult);
	pInfo		= (PCInfoMapObjectData)m_pLibInfoMapObjectData->GetPtr (dwDataID);

	Dlg.Init (m_pMgrData, &m_pWndNotify);
	Dlg.SetModify (pInfo);

	nResult = Dlg.DoModal ();
	m_pWndNotify = NULL;
	if (nResult != IDOK) {
		return;
	}
	Dlg.Get (pInfo);

	Packet.Make (m_pInfoMap->m_dwMapID, pInfo);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectData::OnCopy									 */
/* 内容		:ボタンハンドラ(コピー)											 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectData::OnCopy()
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

        nResult = MessageBox (_T("選択されているアイテムをコピーしますか？"), _T("確認"), MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	Packet.Make (dwItemID);
	m_pSock->Send (&Packet);
#endif
}


/* ========================================================================= */
/* 関数名	:CDlgAdminMapObjectData::OnDelete								 */
/* 内容		:ボタンハンドラ(削除)											 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CDlgAdminMapObjectData::OnDelete()
{
	int nResult;
	DWORD dwDataID;
	PCInfoMapObjectData pInfo;
	CPacketADMIN_PARA2 Packet;

	nResult = m_List.GetNextItem (-1, LVNI_SELECTED);
	if (nResult < 0) {
		return;
	}
	dwDataID	= m_List.GetItemData (nResult);
	pInfo		= (PCInfoMapObjectData)m_pLibInfoMapObjectData->GetPtr (dwDataID);
	if (pInfo == NULL) {
		return;
	}

        nResult = MessageBox (_T("選択されているデータを削除しますか？"), _T("確認"), MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}

	Packet.Make (SBOCOMMANDID_SUB_ADMIN_MAP_DELETEOBJECTDATA, m_pInfoMap->m_dwMapID, dwDataID);
	m_pSock->Send (&Packet);
}

/* Copyright(C)URARA-works 2008 */
