/// @file AdminWindow.cpp
/// @brief 管理者ウィンドウメインフレームクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/01/25
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "afxpriv.h"
#include "resource.h"
#include "Command.h"
#include "PacketADMIN_PARA2.h"
#include "DlgAdminCharModify.h"
#include "DlgAdminCharModifyStatus.h"
#include "DlgAdminCharModifyGrp.h"
#include "DlgAdminCharModifyItem.h"
#include "DlgAdminCharModifySkill.h"
#include "DlgAdminCharList.h"
#include "DlgAdminCharAdmin.h"
#include "DlgAdminCharAddNPC.h"
#include "DlgAdminCharMotionTypeList.h"
#include "DlgAdminCharAccountInfo.h"
#include "DlgAdminCharSkillList.h"
#include "DlgAdminCharDisable.h"
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
#include "DlgAdminAccountAdd.h"
#include "MgrData.h"
#include "WndMap.h"
#include "AdminWindow.h"
#include "AdminApi/AdminUiApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// クラスの設定

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
	ON_COMMAND(IDM_CHAR_MODIFY_SKILL, OnCharModifySkill)
	ON_COMMAND(IDM_CHAR_LIST, OnCharList)
	ON_COMMAND(IDM_CHAR_ADMIN, OnCharAdmin)
	ON_COMMAND(IDM_CHAR_ADDNPC, OnCharAddNPC)
	ON_COMMAND(IDM_CHAR_MOTION, OnCharMotion)
	ON_COMMAND(IDM_CHAR_ACCOUNTINFO, OnCharAccountInfo)
	ON_COMMAND(IDM_CHAR_SKILL, OnCharSkill)
	ON_COMMAND(IDM_CHAR_DISABLE, OnCharDisable)
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
	ON_COMMAND(IDM_ACCOUNT_ADD, OnAccountAdd)
	ON_MESSAGE(WM_ADMINMSG, OnAdminMsg)
	ON_MESSAGE(WM_MAINFRAME, OnMainFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAdminWindow::CAdminWindow()
{
	m_hInitEventWindow	= NULL;
	m_hWnd	= NULL;
	m_hWndParent	= NULL;
	m_nScrID	= SCRIDADMIN_TOP;
	m_pDlgBase	= NULL;
	m_pMgrData	= NULL;
	m_pWndParent	= NULL;
	m_pWndMap	= NULL;
	m_pHost	= NULL;
}

CAdminWindow::~CAdminWindow()
{
}

BOOL CAdminWindow::Create(HWND hWndParent, CMgrData *pMgrData, const SboAdminUiHost* pHost)
{
	BOOL bRet;
	CRect rc;
	CRect rcWork;
	HWND hAnchorWnd;
	HMONITOR hMonitor;
	MONITORINFO mi;
	LPCTSTR pszClass;

	m_pMgrData	= pMgrData;
	m_hWndParent	= hWndParent;
	m_pHost	= pHost;

	// イベントを作成
	m_hInitEventWindow = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (m_hInitEventWindow == NULL) {
		return FALSE;
	}

	ZeroMemory(&rc, sizeof (rc));
	if (m_hWndParent && IsWindow(m_hWndParent)) {
		::GetWindowRect(m_hWndParent, &rc);
	}
	rc.left	= rc.right;
	rc.right	= rc.left + 400;
	rc.bottom	= rc.top + 300;

	hAnchorWnd = (m_hWndParent && IsWindow(m_hWndParent)) ? m_hWndParent : ::GetDesktopWindow();
	hMonitor = MonitorFromWindow(hAnchorWnd, MONITOR_DEFAULTTONEAREST);
	ZeroMemory(&mi, sizeof (mi));
	mi.cbSize = sizeof (mi);
	if (GetMonitorInfo(hMonitor, &mi)) {
		rcWork = mi.rcWork;
		if (rc.left < rcWork.left) {
			rc.OffsetRect(rcWork.left - rc.left, 0);
		}
		if (rc.top < rcWork.top) {
			rc.OffsetRect(0, rcWork.top - rc.top);
		}
		if (rc.right > rcWork.right) {
			rc.OffsetRect(rcWork.right - rc.right, 0);
		}
		if (rc.bottom > rcWork.bottom) {
			rc.OffsetRect(0, rcWork.bottom - rc.bottom);
		}
	}

	pszClass = AfxRegisterWndClass(
		CS_HREDRAW | CS_VREDRAW,
		::LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SBO)));

	bRet = CWnd::CreateEx(
				0,
				pszClass,
				_T("管理者ウィンドウ"),
				WS_OVERLAPPEDWINDOW & ~WS_SYSMENU,
				rc,
				NULL,
				0);
	if (!bRet) {
		DWORD dwErr = GetLastError();
		CString strErr;
		strErr.Format(_T("CAdminWindow::Create failed. GetLastError=0x%08X\r\n"), dwErr);
		OutputDebugString(strErr);
	}

	SetEvent(m_hInitEventWindow);
	return bRet;
}

void CAdminWindow::Destroy(void)
{
	HWND hWndAdmin;

	hWndAdmin = GetSafeHwnd();
	if (m_pWndMap) {
		if (IsWindow(m_pWndMap->GetSafeHwnd())) {
			m_pWndMap->Destroy();
		}
		m_pWndMap = NULL;
	}
	if (m_pDlgBase) {
		if (IsWindow(m_pDlgBase->GetSafeHwnd())) {
			m_pDlgBase->DestroyWindow();
		}
		m_pDlgBase = NULL;
	}
	if (hWndAdmin && IsWindow(hWndAdmin)) {
		CWnd::DestroyWindow();
	}

	m_hInitEventWindow = NULL;

	// 変数を初期化
	m_pWndParent	= NULL;
	m_hWnd	= NULL;
	m_pHost	= NULL;
}

void CAdminWindow::Show(void)
{
	if (m_hWnd == NULL) {
		return;
	}

	ShowWindow(SW_SHOW);
}

void CAdminWindow::Hide(void)
{
	if (m_hWnd == NULL) {
		return;
	}

	ShowWindow(SW_HIDE);
}

void CAdminWindow::ChgScreen(int nScrID)
{
	int nTypeL, nTypeR, nTypeRR;
	CRect rc;

	if (m_pDlgBase) {
		m_pDlgBase->DestroyWindow();
		m_pDlgBase = NULL;
	}
	GetClientRect(rc);

	Initialize(m_hWnd);
	nTypeL  = ADMINNOTIFYTYPE_NONE;
	nTypeR  = ADMINNOTIFYTYPE_NONE;
	nTypeRR = ADMINNOTIFYTYPE_NONE;

	switch (nScrID) {
	case SCRIDADMIN_CHAR_MODIFY:	// 選択キャラ情報編集
		nTypeL  = ADMINNOTIFYTYPE_CHARID;
		nTypeR  = ADMINNOTIFYTYPE_CHARPOS;
		nTypeRR = ADMINNOTIFYTYPE_CHARPOS;
		m_pDlgBase = new CDlgAdminCharModify(this);
		break;
	case SCRIDADMIN_CHAR_MODIFY_STATUS:	// 選択キャラ情報編集[ステータス]
		nTypeL = ADMINNOTIFYTYPE_CHARID;
		m_pDlgBase = new CDlgAdminCharModifyStatus(this);
		break;
	case SCRIDADMIN_CHAR_MODIFY_GRP:	// 選択キャラ情報編集[画像の設定]
		nTypeL = ADMINNOTIFYTYPE_CHARID;
		m_pDlgBase = new CDlgAdminCharModifyGrp(this);
		break;
	case SCRIDADMIN_CHAR_MODIFY_ITEM:	// 選択キャラ情報編集[所持アイテムの設定]
		nTypeL = ADMINNOTIFYTYPE_CHARID;
		m_pDlgBase = new CDlgAdminCharModifyItem(this);
		break;
	case SCRIDADMIN_CHAR_MODIFY_SKILL:	// 選択キャラ情報編集[所持スキルの設定]
		nTypeL = ADMINNOTIFYTYPE_CHARID;
		m_pDlgBase = new CDlgAdminCharModifySkill(this);
		break;
	case SCRIDADMIN_CHAR_LIST:	// 一覧
		m_pDlgBase = new CDlgAdminCharList(this);
		break;
	case SCRIDADMIN_CHAR_ADMIN:	// 管理者権限の設定
		nTypeL = ADMINNOTIFYTYPE_ACCOUNTID;
		m_pDlgBase = new CDlgAdminCharAdmin(this);
		break;
	case SCRIDADMIN_CHAR_ADDNPC:	// NPCの追加
		nTypeL = ADMINNOTIFYTYPE_POS;
		m_pDlgBase = new CDlgAdminCharAddNPC(this);
		break;
	case SCRIDADMIN_CHAR_MOTION:	// モーションの編集
		m_pDlgBase = new CDlgAdminCharMotionTypeList(this);
		break;
	case SCRIDADMIN_CHAR_ACCOUNTINFO:	// アカウント情報の編集
		nTypeL = ADMINNOTIFYTYPE_CHARID;
		m_pDlgBase = new CDlgAdminCharAccountInfo(this);
		break;
	case SCRIDADMIN_CHAR_SKILL:	// スキルの編集
		m_pDlgBase = new CDlgAdminCharSkillList(this);
		break;
	case SCRIDADMIN_CHAR_DISABLE:	// 拒否
		m_pDlgBase = new CDlgAdminCharDisable(this);
		break;
	case SCRIDADMIN_MAP_INFO:	// マップ情報の編集
		m_pDlgBase = new CDlgAdminMapInfo(this);
		break;
	case SCRIDADMIN_MAP_EVENT:	// マップイベント情報の編集
		nTypeL  = ADMINNOTIFYTYPE_POS;
		nTypeR  = ADMINNOTIFYTYPE_POS;
		m_pDlgBase = new CDlgAdminMapEvent(this);
		break;
	case SCRIDADMIN_MAP_PARTS:	// パーツの編集
		nTypeL = ADMINNOTIFYTYPE_MAPEDIT;
		nTypeR = ADMINNOTIFYTYPE_MAPEDIT;
		m_pDlgBase = new CDlgAdminMapParts(this);
		break;
	case SCRIDADMIN_MAP_SHADOW:	// 影の編集
		nTypeL = ADMINNOTIFYTYPE_MAPSHADOWEDIT;
		nTypeR = ADMINNOTIFYTYPE_MAPSHADOWEDIT;
		m_pDlgBase = new CDlgAdminMapShadow(this);
		break;
	case SCRIDADMIN_MAP_OBJECT:	// マップオブジェクトの編集
		m_pDlgBase = new CDlgAdminMapObject(this);
		break;
	case SCRIDADMIN_MAP_OBJECTDATA:	// マップオブジェクトの配置
		nTypeL = ADMINNOTIFYTYPE_POS;
		m_pDlgBase = new CDlgAdminMapObjectData(this);
		break;
	case SCRIDADMIN_ITEMTYPE_LIST:	// アイテム種別一覧
		m_pDlgBase = new CDlgAdminItemTypeList(this);
		break;
	case SCRIDADMIN_ITEM_LIST:	// アイテム一覧
		nTypeL = ADMINNOTIFYTYPE_ITEMID;
		m_pDlgBase = new CDlgAdminItemList(this);
		break;
	case SCRIDADMIN_ITEM_WEAPONLIST:	// 武器情報一覧
		m_pDlgBase = new CDlgAdminItemWeaponList(this);
		break;
	case SCRIDADMIN_EFC_BALLOON_LIST:	// 噴出し一覧
		m_pDlgBase = new CDlgAdminEfcBalloonList(this);
		break;
	case SCRIDADMIN_EFC_EFFECT_LIST:	// エフェクト一覧
		m_pDlgBase = new CDlgAdminEfcEffectList(this);
		break;
	case SCRIDADMIN_SERVER_INFO:	// サーバー情報
		m_pDlgBase = new CDlgAdminServerInfo(this);
		break;
	case SCRIDADMIN_SYSTEM_SET_INITCHARSTATUS:	// キャラステータス初期値の設定
		m_pDlgBase = new CDlgAdminSystemSetInitCharStatus(this);
		break;
	case SCRIDADMIN_ACCOUNT_ADD:	// アカウントの追加
		m_pDlgBase = new CDlgAdminAccountAdd(this);
		break;
	}
	if ((m_pHost) && (m_pHost->SetAdminNotifyTypes)) {
		m_pHost->SetAdminNotifyTypes(m_pHost->userData, nTypeL, nTypeR, nTypeRR);
	} else {
		m_pMgrData->SetAdminNotifyTypeL(nTypeL);
		m_pMgrData->SetAdminNotifyTypeR(nTypeR);
		m_pMgrData->SetAdminNotifyTypeRR(nTypeRR);
	}

	if (m_pDlgBase) {
		m_pDlgBase->SetHost(m_pHost);
		m_pDlgBase->Init(m_pMgrData);
		m_pDlgBase->SetWindowPos(NULL, 0, 0, rc.right, rc.bottom, SWP_NOZORDER | SWP_NOMOVE | SWP_NOREDRAW);
		RegisterControl(m_pDlgBase->m_hWnd, LH_CTRL_WIDTH | LH_CTRL_HEIGHT);
	}
}

BOOL CAdminWindow::PreCreateWindow(CREATESTRUCT& cs)
{
	WNDCLASS wc;

	if (!CWnd::PreCreateWindow(cs)) {
		return FALSE;
	}

	GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wc);
	wc.style &= ~(CS_HREDRAW | CS_VREDRAW);

	AfxRegisterClass(&wc);
	cs.dwExStyle	&= ~WS_EX_CLIENTEDGE;
	cs.style	= WS_OVERLAPPEDWINDOW & ~WS_SYSMENU;
	cs.lpszName	= _T("管理者ウィンドウ");
	cs.lpszClass	= AfxRegisterWndClass(wc.style, wc.hCursor, wc.hbrBackground, wc.hIcon);

	return TRUE;
}

int CAdminWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	TCHAR szFileName[MAX_PATH];
	POINT pt;

	if (CWnd::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	SetEvent(m_hInitEventWindow);

	m_hWnd = GetSafeHwnd();
	// メニューの設定
	if (!m_Menu.LoadMenu(IDR_MENU1)) {
		DWORD dwErr = GetLastError();
		CString strErr;
		strErr.Format(_T("CAdminWindow::OnCreate LoadMenu(IDR_MENU1) failed. GetLastError=0x%08X\r\n"), dwErr);
		OutputDebugString(strErr);
	} else if (!SetMenu(&m_Menu)) {
		DWORD dwErr = GetLastError();
		CString strErr;
		strErr.Format(_T("CAdminWindow::OnCreate SetMenu failed. GetLastError=0x%08X\r\n"), dwErr);
		OutputDebugString(strErr);
	}

	ChgScreen(SCRIDADMIN_CHAR_MODIFY);

	ZeroMemory(szFileName, sizeof (szFileName));
	GetModuleFileName(NULL, szFileName, _countof(szFileName));
	CString strIniPath(szFileName);
	int nExtPos = strIniPath.ReverseFind(_T('.'));
	if (nExtPos != -1) {
		strIniPath = strIniPath.Left(nExtPos) + _T(".ini");
	} else {
		strIniPath += _T(".ini");
	}

	pt.x = GetPrivateProfileInt(_T("Pos"), _T("AdminX"), -1, strIniPath);
	pt.y = GetPrivateProfileInt(_T("Pos"), _T("AdminY"), -1, strIniPath);
	if (!((pt.x == -1) && (pt.y == -1))) {
		SetWindowPos(NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
	}

	ShowWindow(SW_SHOW);
	if ((m_pHost) && (m_pHost->SetAdminWindow)) {
		m_pHost->SetAdminWindow(m_pHost->userData, m_hWnd);
	} else {
		m_pMgrData->SetAdminWindow(m_hWnd);
	}

	return 0;
}

void CAdminWindow::OnDestroy()
{
	if (m_pWndMap) {
		if (IsWindow(m_pWndMap->GetSafeHwnd())) {
			m_pWndMap->Destroy();
		}
		m_pWndMap = NULL;
	}
	if (m_pDlgBase) {
		if (IsWindow(m_pDlgBase->GetSafeHwnd())) {
			m_pDlgBase->DestroyWindow();
		}
		m_pDlgBase = NULL;
	}
	if ((m_pHost) && (m_pHost->SetAdminWindow)) {
		m_pHost->SetAdminWindow(m_pHost->userData, NULL);
	} else if (m_pMgrData) {
		m_pMgrData->SetAdminWindow(NULL);
	}
	m_hWnd = NULL;
	CWnd::OnDestroy();
}

void CAdminWindow::PostNcDestroy()
{
	delete this;
}

void CAdminWindow::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 400;
	lpMMI->ptMinTrackSize.y = 300;
	
	CWnd::OnGetMinMaxInfo(lpMMI);
}

void CAdminWindow::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	DoLayout(cx, cy);
}

LRESULT CAdminWindow::OnAdminMsg(WPARAM wParam, LPARAM lParam)
{
	if (m_pDlgBase) {
		m_pDlgBase->OnAdminMsg(wParam, lParam);
	}
	return -1;
}

LRESULT CAdminWindow::OnMainFrame(WPARAM wParam, LPARAM lParam)
{
	if (m_pDlgBase) {
		m_pDlgBase->OnMainFrame(wParam, lParam);
	}
	return -1;
}

void CAdminWindow::OnMapInfo()
{
	ChgScreen(SCRIDADMIN_MAP_INFO);
}

void CAdminWindow::OnMapEvent()
{
	ChgScreen(SCRIDADMIN_MAP_EVENT);
}

void CAdminWindow::OnMapParts()
{
	ChgScreen(SCRIDADMIN_MAP_PARTS);
}

void CAdminWindow::OnMapShadow()
{
	ChgScreen(SCRIDADMIN_MAP_SHADOW);
}

void CAdminWindow::OnMapAdd()
{
	int nResult;
	CPacketADMIN_PARA2 Packet;

	CString strMessage = Utf8ToTString("マップを追加しますか？");
	CString strCaption = Utf8ToTString("確認");
	nResult = MessageBox(strMessage, strCaption, MB_YESNO | MB_ICONQUESTION);
	if (nResult != IDYES) {
		return;
	}
	Packet.Make(SBOCOMMANDID_SUB_ADMIN_MAP_ADD, 0, 0);
	if ((m_pHost) && (m_pHost->SendAdminPacket)) {
		m_pHost->SendAdminPacket(m_pHost->userData, &Packet);
	}
}

void CAdminWindow::OnMapObject()
{
	ChgScreen(SCRIDADMIN_MAP_OBJECT);
}

void CAdminWindow::OnMapObjectData()
{
	ChgScreen(SCRIDADMIN_MAP_OBJECTDATA);
}

void CAdminWindow::OnMapWindow()
{
	if (m_pWndMap && IsWindow(m_pWndMap->GetSafeHwnd())) {
		return;
	}

	m_pWndMap = new CWndMap;
	m_pWndMap->Create(this, m_pMgrData, 0, m_pHost);
	m_pWndMap->ShowWindow(SW_SHOW);
}

void CAdminWindow::OnCharModify()
{
	ChgScreen(SCRIDADMIN_CHAR_MODIFY);
}

void CAdminWindow::OnCharModifyStatus()
{
	ChgScreen(SCRIDADMIN_CHAR_MODIFY_STATUS);
}

void CAdminWindow::OnCharModifyGrp()
{
	ChgScreen(SCRIDADMIN_CHAR_MODIFY_GRP);
}

void CAdminWindow::OnCharModifyItem()
{
	ChgScreen(SCRIDADMIN_CHAR_MODIFY_ITEM);
}

void CAdminWindow::OnCharModifySkill()
{
	ChgScreen(SCRIDADMIN_CHAR_MODIFY_SKILL);
}

void CAdminWindow::OnCharList()
{
	ChgScreen(SCRIDADMIN_CHAR_LIST);
}

void CAdminWindow::OnCharAdmin()
{
	ChgScreen(SCRIDADMIN_CHAR_ADMIN);
}

void CAdminWindow::OnCharAddNPC()
{
	ChgScreen(SCRIDADMIN_CHAR_ADDNPC);
}

void CAdminWindow::OnCharMotion()
{
	ChgScreen(SCRIDADMIN_CHAR_MOTION);
}

void CAdminWindow::OnCharAccountInfo()
{
	ChgScreen(SCRIDADMIN_CHAR_ACCOUNTINFO);
}

void CAdminWindow::OnCharSkill()
{
	ChgScreen(SCRIDADMIN_CHAR_SKILL);
}

void CAdminWindow::OnCharDisable()
{
	ChgScreen(SCRIDADMIN_CHAR_DISABLE);
}

void CAdminWindow::OnItemTypeList()
{
	ChgScreen(SCRIDADMIN_ITEMTYPE_LIST);
}

void CAdminWindow::OnItemList()
{
	ChgScreen(SCRIDADMIN_ITEM_LIST);
}

void CAdminWindow::OnItemWeaponList()
{
	ChgScreen(SCRIDADMIN_ITEM_WEAPONLIST);
}

void CAdminWindow::OnEfcBalloonList()
{
	ChgScreen(SCRIDADMIN_EFC_BALLOON_LIST);
}

void CAdminWindow::OnEfcEffectList()
{
	ChgScreen(SCRIDADMIN_EFC_EFFECT_LIST);
}

void CAdminWindow::OnServerInfo()
{
	ChgScreen(SCRIDADMIN_SERVER_INFO);
}

void CAdminWindow::OnDebugMoveNoBlock()
{
	BOOL bResult;

	if ((m_pHost) && (m_pHost->GetMoveNoBlock)) {
		bResult = m_pHost->GetMoveNoBlock(m_pHost->userData);
	} else {
		bResult = m_pMgrData->GetMoveNoBlock();
	}
	bResult = (bResult) ? FALSE : TRUE;

	if (bResult) {
		GetMenu()->CheckMenuItem(IDM_DEBUG_MOVENOBLOCK, MF_BYCOMMAND | MF_CHECKED);
	} else {
		GetMenu()->CheckMenuItem(IDM_DEBUG_MOVENOBLOCK, MF_BYCOMMAND | MF_UNCHECKED);
	}
	if ((m_pHost) && (m_pHost->SetMoveNoBlock)) {
		m_pHost->SetMoveNoBlock(m_pHost->userData, bResult);
	} else {
		m_pMgrData->SetMoveNoBlock(bResult);
	}
}

void CAdminWindow::OnDebugGridOff()
{
	GetMenu()->CheckMenuItem(IDM_DEBUG_GRID_OFF,	MF_BYCOMMAND | MF_CHECKED);
	GetMenu()->CheckMenuItem(IDM_DEBUG_GRID,	MF_BYCOMMAND | MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_DEBUG_GRID_HALF,	MF_BYCOMMAND | MF_UNCHECKED);
	if ((m_pHost) && (m_pHost->SetViewGrid)) {
		m_pHost->SetViewGrid(m_pHost->userData, 0);
	} else {
		m_pMgrData->SetViewGrid(0);
	}
}

void CAdminWindow::OnDebugGrid()
{
	GetMenu()->CheckMenuItem(IDM_DEBUG_GRID_OFF,	MF_BYCOMMAND | MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_DEBUG_GRID,	MF_BYCOMMAND | MF_CHECKED);
	GetMenu()->CheckMenuItem(IDM_DEBUG_GRID_HALF,	MF_BYCOMMAND | MF_UNCHECKED);
	if ((m_pHost) && (m_pHost->SetViewGrid)) {
		m_pHost->SetViewGrid(m_pHost->userData, 1);
	} else {
		m_pMgrData->SetViewGrid(1);
	}
}

void CAdminWindow::OnDebugGridHalf()
{
	GetMenu()->CheckMenuItem(IDM_DEBUG_GRID_OFF,	MF_BYCOMMAND | MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_DEBUG_GRID,	MF_BYCOMMAND | MF_UNCHECKED);
	GetMenu()->CheckMenuItem(IDM_DEBUG_GRID_HALF,	MF_BYCOMMAND | MF_CHECKED);
	if ((m_pHost) && (m_pHost->SetViewGrid)) {
		m_pHost->SetViewGrid(m_pHost->userData, 2);
	} else {
		m_pMgrData->SetViewGrid(2);
	}
}

void CAdminWindow::OnSystemSetInitCharStatus()
{
	ChgScreen(SCRIDADMIN_SYSTEM_SET_INITCHARSTATUS);
}

void CAdminWindow::OnAccountAdd()
{
	ChgScreen(SCRIDADMIN_ACCOUNT_ADD);
}

void CAdminWindow::ThreadMain(void)
{
	MSG msg;
	BOOL bResult;
	CRect rc;

	ZeroMemory(&rc, sizeof (rc));
	if (m_hWndParent && IsWindow(m_hWndParent)) {
		::GetWindowRect(m_hWndParent, &rc);
	}
	rc.left	= rc.right;
	rc.right	= rc.left + 400;
	rc.bottom	= rc.top + 300;

	// ウィンドウを作成
	bResult = CWnd::Create(
					NULL,
					NULL,
					AFX_WS_DEFAULT_VIEW,
					rc,
					NULL,
					0);
	if (bResult == FALSE) {
		DWORD dwErr = GetLastError();
		CString strErr;
		strErr.Format(_T("CAdminWindow::ThreadMain Create failed. GetLastError=0x%08X\r\n"), dwErr);
		OutputDebugString(strErr);
		if (m_hInitEventWindow) {
			SetEvent(m_hInitEventWindow);
		}
		goto Exit;
	}

	if (m_hWndParent && IsWindow(m_hWndParent)) {
		::SetFocus(m_hWndParent);
	}
	ShowWindow(SW_SHOW);

	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			if (m_pWndMap && IsWindow(m_pWndMap->GetSafeHwnd())) {
				// ツールバーが有効になるようにアイドルメッセージを通知
				m_pWndMap->SendMessageToDescendants(WM_IDLEUPDATECMDUI);
			}
			MsgWaitForMultipleObjects(0, NULL, FALSE, 1, QS_ALLINPUT);
		}
	}

Exit:
	return;
}

