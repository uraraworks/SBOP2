/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:AdminWindow.cpp											 */
/* 内容			:管理者ウィンドウメインフレームクラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/01/25													 */
/* ========================================================================= */

#include "stdafx.h"
#include "afxpriv.h"
#include "resource.h"
#include "Command.h"
#include "PacketADMIN_PARA2.h"
#include "UraraSockTCPSBO.h"
#include "DlgAdminCharModify.h"
#include "DlgAdminCharModifyStatus.h"
#include "DlgAdminCharModifyGrp.h"
#include "DlgAdminCharModifyItem.h"
#include "DlgAdminCharAdmin.h"
#include "DlgAdminCharAddNPC.h"
#include "DlgAdminCharMotionTypeList.h"
#include "DlgAdminCharAccountInfo.h"
#include "DlgAdminMapInfo.h"
#include "DlgAdminMapEvent.h"
#include "DlgAdminMapParts.h"
#include "DlgAdminMapShadow.h"
#include "DlgAdminMapObject.h"
#include "DlgAdminMapObjectData.h"
#include "DlgAdminItemTypeList.h"
#include "DlgAdminItemList.h"
#include "DlgAdminItemWeaponList.h"
#include "DlgAdminEfcBalloonList.h"
#include "DlgAdminEfcEffectList.h"
#include "DlgAdminServerInfo.h"
#include "DlgAdminSystemSetInitCharStatus.h"
#include "MgrData.h"
#include "WndMap.h"
#include "AdminWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ========================================================================= */
/* クラスの設定																 */
/* ========================================================================= */

IMPLEMENT_DYNAMIC(CAdminWindow, CWnd)
BEGIN_MESSAGE_MAP(CAdminWindow, CWnd)
	//{{AFX_MSG_MAP(CAdminWindow)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(IDM_MAP_INFO, OnMapInfo)
	ON_COMMAND(IDM_MAP_EVENT, OnMapEvent)
	ON_COMMAND(IDM_MAP_PARTS, OnMapParts)
	ON_COMMAND(IDM_MAP_SHADOW, OnMapShadow)
	ON_COMMAND(IDM_MAP_ADD, OnMapAdd)
	ON_COMMAND(IDM_MAP_OBJECT, OnMapObject)
	ON_COMMAND(IDM_MAP_OBJECTDATA, OnMapObjectData)
	ON_COMMAND(IDM_MAP_WINDOW, OnMapWindow)
	ON_COMMAND(IDM_CHAR_MODIFY, OnCharModify)
	ON_COMMAND(IDM_CHAR_MODIFY_STATUS, OnCharModifyStatus)
	ON_COMMAND(IDM_CHAR_MODIFY_GRP, OnCharModifyGrp)
	ON_COMMAND(IDM_CHAR_MODIFY_ITEM, OnCharModifyItem)
	ON_COMMAND(IDM_CHAR_ADMIN, OnCharAdmin)
	ON_COMMAND(IDM_CHAR_ADDNPC, OnCharAddNPC)
	ON_COMMAND(IDM_CHAR_MOTION, OnCharMotion)
	ON_COMMAND(IDM_CHAR_ACCOUNTINFO, OnCharAccountInfo)
	ON_COMMAND(IDM_ITEMTYPE_LIST, OnItemTypeList)
	ON_COMMAND(IDM_ITEM_LIST, OnItemList)
	ON_COMMAND(IDM_ITEM_WEAPONLIST, OnItemWeaponList)
	ON_COMMAND(IDM_EFC_BALLOON_LIST, OnEfcBalloonList)
	ON_COMMAND(IDM_EFC_EFFECT_LIST, OnEfcEffectList)
	ON_COMMAND(IDM_SERVER_INFO, OnServerInfo)
	ON_COMMAND(IDM_DEBUG_MOVENOBLOCK, OnDebugMoveNoBlock)
	ON_COMMAND(IDM_DEBUG_GRID_OFF, OnDebugGridOff)
	ON_COMMAND(IDM_DEBUG_GRID, OnDebugGrid)
	ON_COMMAND(IDM_DEBUG_GRID_HALF, OnDebugGridHalf)
	ON_COMMAND(IDM_SYSTEM_SET_INITCHARSTATUS, OnSystemSetInitCharStatus)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
	ON_MESSAGE(WM_MAINFRAME, OnMainFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/* ========================================================================= */
/* 関数名	:CAdminWindow::CAdminWindow										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/01/25														 */
/* ========================================================================= */

CAdminWindow::CAdminWindow()
{
	m_hWnd			= NULL;
	m_hWndParent	= NULL;
	m_nScrID		= SCRIDADMIN_TOP;
	m_pDlgBase		= NULL;
	m_pMgrData		= NULL;
	m_pWndParent	= NULL;
	m_pWndMap		= NULL;
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::~CAdminWindow									 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/01/25														 */
/* ========================================================================= */

CAdminWindow::~CAdminWindow()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::Create											 */
/* 内容		:作成															 */
/* 日付		:2006/01/25														 */
/* ========================================================================= */

BOOL CAdminWindow::Create(HWND hWndParent, CMgrData *pMgrData)
{
	BOOL bRet;

	m_pMgrData		= pMgrData;
	m_hWndParent	= hWndParent;

	bRet = CmyThread::Create ();
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::Destroy											 */
/* 内容		:破棄															 */
/* 日付		:2006/01/25														 */
/* ========================================================================= */

void CAdminWindow::Destroy(void)
{
	if (m_hThread) {
		if (m_hWnd) {
			PostMessage (WM_CLOSE);
		}
		WaitForSingleObject (m_hThread, INFINITE);
	}
	CmyThread::Destroy ();

	/* 変数を初期化 */
	m_pWndParent	= NULL;
	m_hWnd			= NULL;
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::Show												 */
/* 内容		:表示															 */
/* 日付		:2006/03/12														 */
/* ========================================================================= */

void CAdminWindow::Show(void)
{
	if (m_hWnd == NULL) {
		return;
	}

	ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::Hide												 */
/* 内容		:非表示															 */
/* 日付		:2006/03/12														 */
/* ========================================================================= */

void CAdminWindow::Hide(void)
{
	if (m_hWnd == NULL) {
		return;
	}

	ShowWindow (SW_HIDE);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::ChgScreen										 */
/* 内容		:画面切り替え													 */
/* 日付		:2006/01/26														 */
/* ========================================================================= */

void CAdminWindow::ChgScreen(int nScrID)
{
	int nTypeL, nTypeR, nTypeRR;
	CRect rc;

	if (m_pDlgBase) {
		m_pDlgBase->DestroyWindow ();
		m_pDlgBase = NULL;
	}
	GetClientRect (rc);

	Initialize (m_hWnd);
	nTypeL  = ADMINNOTIFYTYPE_NONE;
	nTypeR  = ADMINNOTIFYTYPE_NONE;
	nTypeRR = ADMINNOTIFYTYPE_NONE;

	switch (nScrID) {
	case SCRIDADMIN_CHAR_MODIFY:				/* 選択キャラ情報編集 */
		nTypeL  = ADMINNOTIFYTYPE_CHARID;
		nTypeR  = ADMINNOTIFYTYPE_CHARPOS;
		nTypeRR = ADMINNOTIFYTYPE_CHARPOS;
		m_pDlgBase = new CDlgAdminCharModify(this);
		break;
	case SCRIDADMIN_CHAR_MODIFY_STATUS:			/* 選択キャラ情報編集[ステータス] */
		nTypeL = ADMINNOTIFYTYPE_CHARID;
		m_pDlgBase = new CDlgAdminCharModifyStatus(this);
		break;
	case SCRIDADMIN_CHAR_MODIFY_GRP:			/* 選択キャラ情報編集[画像の設定] */
		nTypeL = ADMINNOTIFYTYPE_CHARID;
		m_pDlgBase = new CDlgAdminCharModifyGrp(this);
		break;
	case SCRIDADMIN_CHAR_MODIFY_ITEM:			/* 選択キャラ情報編集[所持アイテムの設定] */
		nTypeL = ADMINNOTIFYTYPE_CHARID;
		m_pDlgBase = new CDlgAdminCharModifyItem(this);
		break;
	case SCRIDADMIN_CHAR_ADMIN:					/* 管理者権限の設定 */
		nTypeL = ADMINNOTIFYTYPE_ACCOUNTID;
		m_pDlgBase = new CDlgAdminCharAdmin(this);
		break;
	case SCRIDADMIN_CHAR_ADDNPC:				/* NPCの追加 */
		nTypeL = ADMINNOTIFYTYPE_POS;
		m_pDlgBase = new CDlgAdminCharAddNPC(this);
		break;
	case SCRIDADMIN_CHAR_MOTION:				/* モーションの編集 */
		m_pDlgBase = new CDlgAdminCharMotionTypeList(this);
		break;
	case SCRIDADMIN_CHAR_ACCOUNTINFO:			/* アカウント情報の編集 */
		nTypeL = ADMINNOTIFYTYPE_CHARID;
		m_pDlgBase = new CDlgAdminCharAccountInfo(this);
		break;
	case SCRIDADMIN_MAP_INFO:					/* マップ情報の編集 */
		m_pDlgBase = new CDlgAdminMapInfo(this);
		break;
	case SCRIDADMIN_MAP_EVENT:					/* マップイベント情報の編集 */
		nTypeL  = ADMINNOTIFYTYPE_POS;
		nTypeR  = ADMINNOTIFYTYPE_POS;
		m_pDlgBase = new CDlgAdminMapEvent(this);
		break;
	case SCRIDADMIN_MAP_PARTS:					/* パーツの編集 */
		nTypeL = ADMINNOTIFYTYPE_MAPEDIT;
		nTypeR = ADMINNOTIFYTYPE_MAPEDIT;
		m_pDlgBase = new CDlgAdminMapParts(this);
		break;
	case SCRIDADMIN_MAP_SHADOW:					/* 影の編集 */
		nTypeL = ADMINNOTIFYTYPE_MAPSHADOWEDIT;
		nTypeR = ADMINNOTIFYTYPE_MAPSHADOWEDIT;
		m_pDlgBase = new CDlgAdminMapShadow(this);
		break;
	case SCRIDADMIN_MAP_OBJECT:					/* マップオブジェクトの編集 */
		m_pDlgBase = new CDlgAdminMapObject(this);
		break;
	case SCRIDADMIN_MAP_OBJECTDATA:				/* マップオブジェクトの配置 */
		nTypeL = ADMINNOTIFYTYPE_POS;
		m_pDlgBase = new CDlgAdminMapObjectData(this);
		break;
	case SCRIDADMIN_ITEMTYPE_LIST:				/* アイテム種別一覧 */
		m_pDlgBase = new CDlgAdminItemTypeList(this);
		break;
	case SCRIDADMIN_ITEM_LIST:					/* アイテム一覧 */
		nTypeL = ADMINNOTIFYTYPE_ITEMID;
		m_pDlgBase = new CDlgAdminItemList(this);
		break;
	case SCRIDADMIN_ITEM_WEAPONLIST:			/* 武器情報一覧 */
		m_pDlgBase = new CDlgAdminItemWeaponList(this);
		break;
	case SCRIDADMIN_EFC_BALLOON_LIST:			/* 噴出し一覧 */
		m_pDlgBase = new CDlgAdminEfcBalloonList(this);
		break;
	case SCRIDADMIN_EFC_EFFECT_LIST:			/* エフェクト一覧 */
		m_pDlgBase = new CDlgAdminEfcEffectList(this);
		break;
	case SCRIDADMIN_SERVER_INFO:				/* サーバー情報 */
		m_pDlgBase = new CDlgAdminServerInfo(this);
		break;
	case SCRIDADMIN_SYSTEM_SET_INITCHARSTATUS:	/* キャラステータス初期値の設定 */
		m_pDlgBase = new CDlgAdminSystemSetInitCharStatus(this);
		break;
	}
	m_pMgrData->SetAdminNotifyTypeL (nTypeL);
	m_pMgrData->SetAdminNotifyTypeR (nTypeR);
	m_pMgrData->SetAdminNotifyTypeRR (nTypeR);

	if (m_pDlgBase) {
		m_pDlgBase->Init (m_pMgrData);
		m_pDlgBase->SetWindowPos (NULL, 0, 0, rc.right, rc.bottom, SWP_NOZORDER | SWP_NOMOVE | SWP_NOREDRAW);
		RegisterControl (m_pDlgBase->m_hWnd, LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	}
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::PreCreateWindow									 */
/* 内容		:ウィンドウ作成前処理											 */
/* 日付		:2006/01/25														 */
/* ========================================================================= */

BOOL CAdminWindow::PreCreateWindow(CREATESTRUCT& cs)
{
	WNDCLASS wc;

	if (!CWnd::PreCreateWindow (cs)) {
		return FALSE;
	}

	GetClassInfo (AfxGetInstanceHandle (), cs.lpszClass, &wc);
	wc.style &= ~(CS_HREDRAW | CS_VREDRAW);

	AfxRegisterClass (&wc);
	cs.dwExStyle	&= ~WS_EX_CLIENTEDGE;
	cs.style		= WS_OVERLAPPEDWINDOW & ~WS_SYSMENU;
	cs.lpszName		= "管理者ウィンドウ";
	cs.lpszClass	= AfxRegisterWndClass (wc.style, wc.hCursor, wc.hbrBackground, wc.hIcon);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnCreate											 */
/* 内容		:メッセージハンドラ(WM_CREATE)									 */
/* 日付		:2006/01/25														 */
/* ========================================================================= */

int CAdminWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	char szFileName[MAX_PATH];
	POINT pt;

	if (CWnd::OnCreate (lpCreateStruct) == -1) {
		return -1;
	}

	m_hWnd = GetSafeHwnd ();
	/* メニューの設定 */
	m_Menu.LoadMenu ("IDR_MENU1");
	SetMenu (&m_Menu);

	ChgScreen (SCRIDADMIN_CHAR_MODIFY);

	ZeroMemory (szFileName, sizeof (szFileName));
	GetModuleFileName (NULL, szFileName, MAX_PATH);
	strcpy (szFileName + strlen (szFileName) - 3, "ini");

	pt.x = GetPrivateProfileInt ("Pos", "AdminX", -1, szFileName);
	pt.y = GetPrivateProfileInt ("Pos", "AdminY", -1, szFileName);
	if (!((pt.x == -1) && (pt.y == -1))) {
		SetWindowPos (NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
	}

	ShowWindow (SW_SHOW);
	m_pMgrData->SetAdminWindow (m_hWnd);

	return 0;
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnDestroy										 */
/* 内容		:メッセージハンドラ(WM_DESTROY)									 */
/* 日付		:2006/02/11														 */
/* ========================================================================= */

void CAdminWindow::OnDestroy()
{
	CWnd::OnDestroy ();
	PostQuitMessage (0);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::PostNcDestroy									 */
/* 内容		:終了処理														 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

void CAdminWindow::PostNcDestroy()
{
//	delete this;
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnGetMinMaxInfo									 */
/* 内容		:メッセージハンドラ(WM_GETMINMAXINFO)							 */
/* 日付		:2006/01/25														 */
/* ========================================================================= */

void CAdminWindow::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 400;
	lpMMI->ptMinTrackSize.y = 300;
	
	CWnd::OnGetMinMaxInfo (lpMMI);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnSize											 */
/* 内容		:メッセージハンドラ(WM_SIZE)									 */
/* 日付		:2006/01/25														 */
/* ========================================================================= */

void CAdminWindow::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize (nType, cx, cy);

	DoLayout (cx, cy);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnAdminMsg										 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

LRESULT CAdminWindow::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	if (m_pDlgBase) {
		m_pDlgBase->OnAdminMsg (wParam, lParam);
	}
	return -1;
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnMainFrame										 */
/* 内容		:メッセージハンドラ(WM_MAINFRAME)								 */
/* 日付		:2007/08/16														 */
/* ========================================================================= */

LRESULT CAdminWindow::OnMainFrame(WPARAM wParam, LPARAM lParam)
{
	if (m_pDlgBase) {
		m_pDlgBase->OnMainFrame (wParam, lParam);
	}
	return -1;
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnMapInfo										 */
/* 内容		:メニューハンドラ(マップ情報の編集)								 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

void CAdminWindow::OnMapInfo()
{
	ChgScreen (SCRIDADMIN_MAP_INFO);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnMapEvent										 */
/* 内容		:メニューハンドラ(マップイベント情報の編集)						 */
/* 日付		:2008/06/24														 */
/* ========================================================================= */

void CAdminWindow::OnMapEvent()
{
	ChgScreen (SCRIDADMIN_MAP_EVENT);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnMapParts										 */
/* 内容		:メニューハンドラ(パーツの編集)									 */
/* 日付		:2007/04/25														 */
/* ========================================================================= */

void CAdminWindow::OnMapParts()
{
	ChgScreen (SCRIDADMIN_MAP_PARTS);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnMapShadow										 */
/* 内容		:メニューハンドラ(影の編集)										 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

void CAdminWindow::OnMapShadow()
{
	ChgScreen (SCRIDADMIN_MAP_SHADOW);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnMapAdd											 */
/* 内容		:メニューハンドラ(マップの追加)									 */
/* 日付		:2008/07/27														 */
/* ========================================================================= */

void CAdminWindow::OnMapAdd()
{
	int nResult;
	CPacketADMIN_PARA2 Packet;

	nResult = MessageBox ("マップを追加しますか？", "確認", MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}
	Packet.Make (SBOCOMMANDID_SUB_ADMIN_MAP_ADD, 0, 0);
	m_pMgrData->GetUraraSockTCP ()->Send (&Packet);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnMapObject										 */
/* 内容		:メニューハンドラ(マップオブジェクトの編集)						 */
/* 日付		:2008/09/07														 */
/* ========================================================================= */

void CAdminWindow::OnMapObject()
{
	ChgScreen (SCRIDADMIN_MAP_OBJECT);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnMapObjectData									 */
/* 内容		:メニューハンドラ(マップオブジェクトの配置)						 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CAdminWindow::OnMapObjectData()
{
	ChgScreen (SCRIDADMIN_MAP_OBJECTDATA);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnMapWindow										 */
/* 内容		:メニューハンドラ(マップウィンドウ)								 */
/* 日付		:2008/09/12														 */
/* ========================================================================= */

void CAdminWindow::OnMapWindow()
{
	if (m_pWndMap && IsWindow (m_pWndMap->GetSafeHwnd ())) {
		return;
	}

	m_pWndMap = new CWndMap;
	m_pWndMap->Create (this, m_pMgrData, 0);
	m_pWndMap->ShowWindow (SW_SHOW);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnCharModify										 */
/* 内容		:メニューハンドラ(選択キャラの編集)								 */
/* 日付		:2007/03/17														 */
/* ========================================================================= */

void CAdminWindow::OnCharModify()
{
	ChgScreen (SCRIDADMIN_CHAR_MODIFY);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnCharModifyStatus								 */
/* 内容		:メニューハンドラ(選択キャラの編集[ステータス])					 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CAdminWindow::OnCharModifyStatus()
{
	ChgScreen (SCRIDADMIN_CHAR_MODIFY_STATUS);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnCharModifyGrp									 */
/* 内容		:メニューハンドラ(選択キャラの編集[画像の設定])					 */
/* 日付		:2007/09/02														 */
/* ========================================================================= */

void CAdminWindow::OnCharModifyGrp()
{
	ChgScreen (SCRIDADMIN_CHAR_MODIFY_GRP);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnCharModifyItem									 */
/* 内容		:メニューハンドラ(選択キャラの編集[所持アイテムの設定])			 */
/* 日付		:2007/09/23														 */
/* ========================================================================= */

void CAdminWindow::OnCharModifyItem()
{
	ChgScreen (SCRIDADMIN_CHAR_MODIFY_ITEM);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnCharAdmin										 */
/* 内容		:メニューハンドラ(管理者権限の設定)								 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

void CAdminWindow::OnCharAdmin()
{
	ChgScreen (SCRIDADMIN_CHAR_ADMIN);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnCharAddNPC										 */
/* 内容		:メニューハンドラ(NPCの追加)									 */
/* 日付		:2007/09/01														 */
/* ========================================================================= */

void CAdminWindow::OnCharAddNPC()
{
	ChgScreen (SCRIDADMIN_CHAR_ADDNPC);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnCharMotion										 */
/* 内容		:メニューハンドラ(モーションの編集)								 */
/* 日付		:2007/10/28														 */
/* ========================================================================= */

void CAdminWindow::OnCharMotion()
{
	ChgScreen (SCRIDADMIN_CHAR_MOTION);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnCharAccountInfo								 */
/* 内容		:メニューハンドラ(アカウント情報の編集)							 */
/* 日付		:2008/06/06														 */
/* ========================================================================= */

void CAdminWindow::OnCharAccountInfo()
{
	ChgScreen (SCRIDADMIN_CHAR_ACCOUNTINFO);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnItemTypeList									 */
/* 内容		:メニューハンドラ(アイテム種別一覧)								 */
/* 日付		:2007/09/30														 */
/* ========================================================================= */

void CAdminWindow::OnItemTypeList()
{
	ChgScreen (SCRIDADMIN_ITEMTYPE_LIST);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnItemList										 */
/* 内容		:メニューハンドラ(アイテム一覧)									 */
/* 日付		:2007/08/14														 */
/* ========================================================================= */

void CAdminWindow::OnItemList()
{
	ChgScreen (SCRIDADMIN_ITEM_LIST);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnItemWeaponList									 */
/* 内容		:メニューハンドラ(武器情報一覧)									 */
/* 日付		:2007/08/14														 */
/* ========================================================================= */

void CAdminWindow::OnItemWeaponList()
{
	ChgScreen (SCRIDADMIN_ITEM_WEAPONLIST);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnEfcBalloonList									 */
/* 内容		:メニューハンドラ(噴出し一覧)									 */
/* 日付		:2007/12/24														 */
/* ========================================================================= */

void CAdminWindow::OnEfcBalloonList()
{
	ChgScreen (SCRIDADMIN_EFC_BALLOON_LIST);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnEfcEffectList									 */
/* 内容		:メニューハンドラ(エフェクト一覧)								 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

void CAdminWindow::OnEfcEffectList()
{
	ChgScreen (SCRIDADMIN_EFC_EFFECT_LIST);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnServerInfo										 */
/* 内容		:メニューハンドラ(サーバー情報)									 */
/* 日付		:2007/07/08														 */
/* ========================================================================= */

void CAdminWindow::OnServerInfo()
{
	ChgScreen (SCRIDADMIN_SERVER_INFO);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnDebugMoveNoBlock								 */
/* 内容		:メニューハンドラ(移動当たり判定無効)							 */
/* 日付		:2008/07/20														 */
/* ========================================================================= */

void CAdminWindow::OnDebugMoveNoBlock()
{
	BOOL bResult;

	bResult = m_pMgrData->GetMoveNoBlock ();
	bResult = (bResult) ? FALSE : TRUE;

	if (bResult) {
		GetMenu ()->CheckMenuItem (IDM_DEBUG_MOVENOBLOCK, MF_BYCOMMAND | MF_CHECKED);
	} else {
		GetMenu ()->CheckMenuItem (IDM_DEBUG_MOVENOBLOCK, MF_BYCOMMAND | MF_UNCHECKED);
	}
	m_pMgrData->SetMoveNoBlock (bResult);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnDebugGridOff									 */
/* 内容		:メニューハンドラ(グリッド表示-表示しない)						 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

void CAdminWindow::OnDebugGridOff()
{
	GetMenu ()->CheckMenuItem (IDM_DEBUG_GRID_OFF,	MF_BYCOMMAND | MF_CHECKED);
	GetMenu ()->CheckMenuItem (IDM_DEBUG_GRID,		MF_BYCOMMAND | MF_UNCHECKED);
	GetMenu ()->CheckMenuItem (IDM_DEBUG_GRID_HALF,	MF_BYCOMMAND | MF_UNCHECKED);
	m_pMgrData->SetViewGrid (0);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnDebugGrid										 */
/* 内容		:メニューハンドラ(グリッド表示-1キャラサイズ)					 */
/* 日付		:2008/05/02														 */
/* ========================================================================= */

void CAdminWindow::OnDebugGrid()
{
	GetMenu ()->CheckMenuItem (IDM_DEBUG_GRID_OFF,	MF_BYCOMMAND | MF_UNCHECKED);
	GetMenu ()->CheckMenuItem (IDM_DEBUG_GRID,		MF_BYCOMMAND | MF_CHECKED);
	GetMenu ()->CheckMenuItem (IDM_DEBUG_GRID_HALF,	MF_BYCOMMAND | MF_UNCHECKED);
	m_pMgrData->SetViewGrid (1);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnDebugGridHalf									 */
/* 内容		:メニューハンドラ(グリッド表示-半キャラサイズ)					 */
/* 日付		:2008/05/02														 */
/* ========================================================================= */

void CAdminWindow::OnDebugGridHalf()
{
	GetMenu ()->CheckMenuItem (IDM_DEBUG_GRID_OFF,	MF_BYCOMMAND | MF_UNCHECKED);
	GetMenu ()->CheckMenuItem (IDM_DEBUG_GRID,		MF_BYCOMMAND | MF_UNCHECKED);
	GetMenu ()->CheckMenuItem (IDM_DEBUG_GRID_HALF,	MF_BYCOMMAND | MF_CHECKED);
	m_pMgrData->SetViewGrid (2);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::OnSystemSetInitCharStatus						 */
/* 内容		:メニューハンドラ(キャラステータス初期値の設定)					 */
/* 日付		:2008/09/22														 */
/* ========================================================================= */

void CAdminWindow::OnSystemSetInitCharStatus()
{
	ChgScreen (SCRIDADMIN_SYSTEM_SET_INITCHARSTATUS);
}


/* ========================================================================= */
/* 関数名	:CAdminWindow::ThreadMain										 */
/* 内容		:メイン処理														 */
/* 日付		:2007/04/28														 */
/* ========================================================================= */

void CAdminWindow::ThreadMain(void)
{
	MSG msg;
	BOOL bResult;
	CRect rc;

	m_pWndParent = CWnd::FromHandle (m_hWndParent);
	m_pWndParent->GetWindowRect (rc);
	rc.left		= rc.right;
	rc.right	= rc.left + 400;
	rc.bottom	= rc.top + 300;

	/* ウィンドウを作成 */
	bResult = CWnd::Create (
					NULL,
					NULL,
					AFX_WS_DEFAULT_VIEW,
					rc,
					m_pWndParent,
					0);
	if (bResult == FALSE) {
		goto Exit;
	}

	m_pWndParent->SetFocus ();
	ShowWindow (SW_SHOW);

	while (1) {
		if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		} else {
			if (m_pWndMap && IsWindow (m_pWndMap->GetSafeHwnd ())) {
				/* ツールバーが有効になるようにアイドルメッセージを通知 */
				m_pWndMap->SendMessageToDescendants (WM_IDLEUPDATECMDUI);
			}
			MsgWaitForMultipleObjects (0, NULL, FALSE, 1, QS_ALLINPUT);
		}
	}

Exit:
	return;
}

/* Copyright(C)URARA-works 2006 */
