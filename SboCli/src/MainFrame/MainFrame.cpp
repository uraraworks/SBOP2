/// @file MainFrame.cpp
/// @brief クライアントメインフレーム 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/09/24
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#if !defined(__EMSCRIPTEN__)
#include <comdef.h>
#else
#include <emscripten/em_js.h>
#endif
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_syswm.h>
#include "resource.h"
#include "SBOVersion.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "ParamUtil.h"
#include "InfoMapBase.h"
#include "LibInfoMapParts.h"
#include "LibInfoMapShadow.h"
#include "LibInfoItem.h"
#include "LibInfoItemWeapon.h"
#include "LibInfoEfcBalloon.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "StateProc.h"
#include "Window/ILoginWindow.h"
#include "WindowLOGIN.h"
#include "LayerMap.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "MgrWindow.h"
#include "MgrKeyInput.h"
#include "MgrSound.h"
#include "Platform/SDLApp.h"
#include "Platform/SDLEventUtil.h"
#include "MainFrame.h"


// 定数定義

#define CLNAME "SboCli" // 登録クラス名
#define TIMER_TOOLCHECK 1000 // ツールチェックタイマー周期

int g_nSboCliRenderFrameRate = DRAWCOUNT;

#if defined(__EMSCRIPTEN__)

EM_JS(void, SBOP2_DebugMarkGameState, (int state, int hasStateProc), {
	Module.sbop2GameState = state;
	Module.sbop2HasStateProc = hasStateProc;
	Module.sbop2ChgStateCount = (Module.sbop2ChgStateCount || 0) + 1;
});
EM_JS(void, SBOP2_DebugMarkInitEnd, (int grpLoaded), {
	Module.sbop2InitEndReached = 1;
	Module.sbop2GrpLoadResult = grpLoaded;
});
#endif

// メッセージコマンド種別
enum {
	MSGCMDTYPE_NONE = 0,
	MSGCMDTYPE_CHGFACE, // 表情変更
	MSGCMDTYPE_CHGHAIR, // 髪変更
	MSGCMDTYPE_CHGCLOTH, // 服装変更
	MSGCMDTYPE_CHGACCE, // アクセサリ変更
	MSGCMDTYPE_BGMVOLUME, // BGM音量設定
	MSGCMDTYPE_CHGCOLOR, // 色変更
	MSGCMDTYPE_SEVOLUME, // 効果音量設定
	MSGCMDTYPE_CHGARMS, // 持ち物変更
	MSGCMDTYPE_CHGSHIELD, // 盾変更
	MSGCMDTYPE_SETITEM, // 配置アイテム変更
	MSGCMDTYPE_MAKEITEM, // アイテム作成
	MSGCMDTYPE_BALLOON, // 噴出し
	MSGCMDTYPE_DICE, // サイコロ
	MSGCMDTYPE_RND, // ランダム
	MSGCMDTYPE_NOW, // 現在時刻
	MSGCMDTYPE_EFFECT, // エフェクト
	MSGCMDTYPE_WHERE, // 最も集まっている場所
	MSGCMDTYPE_MAX
};


CMainFrame::CMainFrame()
{
	m_hWnd = NULL;
	m_bWindowActive = TRUE;
	m_bRenewCharInfo = FALSE;
	m_bRequestDraw = FALSE;
	m_nGameState = GAMESTATE_NONE;
	m_nDrawCount = 30;
	m_nFPS = 0;
	m_dwLastTimeCheck = 0;
	m_dwLastToolCheckTick = 0;
	m_dwDrawTime = 0;

	m_pStateProc = NULL;
	m_pMgrData = new CMgrData;
#if defined(__EMSCRIPTEN__)
	m_pSock = NULL;
#else
	m_pSock = new CUraraSockTCPSBO;
#endif
	m_pMgrGrpData = new CMgrGrpData;

	m_pMgrDraw = NULL;
	m_pMgrLayer = NULL;
	m_pMgrWindow = NULL;
	m_pMgrKeyInput = NULL;
	m_pMgrSound = NULL;
	m_pLibInfoChar = NULL;
	m_pLibInfoDisable = NULL;
	m_pLibInfoMap = NULL;
	m_pLibInfoMapObject = NULL;
	m_pLibInfoMapParts = NULL;
	m_pLibInfoMapShadow = NULL;
	m_pLibInfoItemType = NULL;
	m_pLibInfoItem = NULL;
	m_pLibInfoItemWeapon = NULL;
	m_pLibInfoMotion = NULL;
	m_pLibInfoMotionType = NULL;
	m_pLibInfoEfcBalloon = NULL;
	m_pLibInfoEffect = NULL;
	m_pLibInfoSystem = NULL;
	m_pLibInfoSkill = NULL;

	m_hCom = CoInitializeEx(0, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	m_pSDLWindow = NULL;
	ZeroMemory(&m_stSystemTime, sizeof(m_stSystemTime));
	InitializeCriticalSection(&m_csSocketNotify);
	InitializeCriticalSection(&m_csMainFrameNotify);
	InitializeCriticalSection(&m_csMgrDrawNotify);
	InitializeCriticalSection(&m_csWindowNotify);
	InitializeCriticalSection(&m_csAdminNotify);
}


CMainFrame::~CMainFrame()
{
	SAFE_DELETE(m_pStateProc);
	SAFE_DELETE(m_pMgrGrpData);
#if !defined(__EMSCRIPTEN__)
	SAFE_DELETE(m_pSock);
#endif
	SAFE_DELETE(m_pMgrData);
	DeleteCriticalSection(&m_csSocketNotify);
	DeleteCriticalSection(&m_csMainFrameNotify);
	DeleteCriticalSection(&m_csMgrDrawNotify);
	DeleteCriticalSection(&m_csWindowNotify);
	DeleteCriticalSection(&m_csAdminNotify);

	if (SUCCEEDED(m_hCom)) {
		CoUninitialize();
	}
}


int CMainFrame::MainLoop(HINSTANCE hInstance)
{
	// Phase 2: SDL基盤に移行。Win32ウィンドウ生成・メッセージループは SDLApp に委譲する
	char szTitle[256];
#if defined(__EMSCRIPTEN__)
	static CSDLApp sdlApp;
#else
	CSDLApp sdlApp;
#endif

	sprintf_s(szTitle, "%s Ver%s", WNDTITLE, VERTEXT);

	if (!sdlApp.Init()) {
		return -1;
	}

	int nRet = sdlApp.Run(this, szTitle, SCRSIZEX, SCRSIZEY);
#if !defined(__EMSCRIPTEN__)
	sdlApp.Destroy();
#endif

	return nRet;
}


BOOL CMainFrame::OnSDLInit(SDL_Window *pWindow)
{
	if (pWindow == NULL) {
		return FALSE;
	}

	m_pSDLWindow = pWindow;
	m_pMgrData->SetLocalTitleMode(IsLocalTitleModeRequested());

#if defined(__EMSCRIPTEN__)
	m_hWnd = NULL;
	OnInitEnd(NULL);
	return TRUE;
#else
	if (!InitNativeMainWindow(pWindow)) {
		return FALSE;
	}

	// 初期化処理を実行（旧 WM_INITEND 相当）
	OnInitEnd(m_hWnd);

	return TRUE;
#endif
}


BOOL CMainFrame::InitNativeMainWindow(SDL_Window *pWindow)
{
#if defined(__EMSCRIPTEN__)
	UNREFERENCED_PARAMETER(pWindow);
	return FALSE;
#else
	SDL_SysWMinfo wmInfo;
	HWND hWnd;

	SDL_VERSION(&wmInfo.version);
	if (!SDL_GetWindowWMInfo(pWindow, &wmInfo)) {
		return FALSE;
	}

	hWnd = wmInfo.info.win.window;
	m_hWnd = hWnd;

	RestoreMainWindowPosition(pWindow);
	// MainFrame 向けの独自通知は内部キューまたは SDLApp 直送へ寄せたため、
	// ここでは SDL ウィンドウをそのまま使う。
	return TRUE;
#endif
}


void CMainFrame::RestoreMainWindowPosition(SDL_Window *pWindow)
{
	TCHAR szFileName[MAX_PATH];
	POINT pt;

	GetModuleIniPath(szFileName, _countof(szFileName));

	pt.x = GetPrivateProfileInt(_T("Pos"), _T("MainX"), -1, szFileName);
	pt.y = GetPrivateProfileInt(_T("Pos"), _T("MainY"), -1, szFileName);
	if ((pWindow != NULL) && !((pt.x == -1) && (pt.y == -1))) {
		SDL_SetWindowPosition(pWindow, pt.x, pt.y);
	}
}


void CMainFrame::DispatchMgrDrawMessage(int nCode, DWORD dwPara)
{
	OnMgrDraw(nCode, dwPara);
}


void CMainFrame::PostMgrDrawMessage(int nCode, DWORD dwPara)
{
	MgrDrawNotify stNotify;

	stNotify.nCode = nCode;
	stNotify.dwParam = dwPara;

	EnterCriticalSection(&m_csMgrDrawNotify);
	m_aPendingMgrDrawNotify.push_back(stNotify);
	LeaveCriticalSection(&m_csMgrDrawNotify);
}


void CMainFrame::DispatchMainFrameMessage(DWORD dwCommand, DWORD dwParam)
{
	OnMainFrame(dwCommand, dwParam);
}


void CMainFrame::DispatchWindowMessage(int nType, DWORD dwParam)
{
	OnWindowMsg(nType, dwParam);
}


void CMainFrame::DispatchAdminMessage(int nType, DWORD dwParam)
{
	OnAdminMsg(nType, dwParam);
}


void CMainFrame::PostMainFrameMessage(DWORD dwCommand, DWORD dwParam)
{
	MainFrameNotify stNotify;

	stNotify.dwCommand = dwCommand;
	stNotify.dwParam = dwParam;

	EnterCriticalSection(&m_csMainFrameNotify);
	m_aPendingMainFrameNotify.push_back(stNotify);
	LeaveCriticalSection(&m_csMainFrameNotify);
}


void CMainFrame::PostWindowMessage(int nType, DWORD dwParam)
{
	WindowNotify stNotify;

	stNotify.nType = nType;
	stNotify.dwParam = dwParam;

	EnterCriticalSection(&m_csWindowNotify);
	m_aPendingWindowNotify.push_back(stNotify);
	LeaveCriticalSection(&m_csWindowNotify);
}


void CMainFrame::PostAdminMessage(int nType, DWORD dwParam)
{
	AdminNotify stNotify;

	stNotify.nType = nType;
	stNotify.dwParam = dwParam;

	EnterCriticalSection(&m_csAdminNotify);
	m_aPendingAdminNotify.push_back(stNotify);
	LeaveCriticalSection(&m_csAdminNotify);
}


void CMainFrame::OnSDLFocusChanged(BOOL bActive)
{
	m_bWindowActive = bActive;
	if (!bActive) {
		if (m_pMgrKeyInput) {
			m_pMgrKeyInput->Reset();
		}
	}
}


BOOL CMainFrame::OnFrame(void)
{
	BOOL bDraw;

	FlushPendingWindowMessages();
	FlushPendingAdminMessages();
	FlushPendingMgrDrawMessages();
	FlushPendingMainFrameMessages();
	FlushPendingSocketMessages();
	UpdateToolCheck();
	bDraw = TimerProc();
	if (m_bRequestDraw) {
		bDraw = TRUE;
		m_bRequestDraw = FALSE;
	}
	KeyProc();

	return bDraw;
}


void CMainFrame::OnSDLKeyDown(int vk)
{
	CWindowBase *pWindow;

	if (vk == 0) {
		return;
	}

	pWindow = (m_pMgrWindow != NULL) ? m_pMgrWindow->GetActiveWindow() : NULL;
	if ((pWindow != NULL) && pWindow->HandleSDLKeyDown((UINT)vk)) {
		return;
	}

	if (m_pStateProc != NULL) {
		m_pStateProc->OnKeyDown((UINT)vk);
	}
}


void CMainFrame::OnSDLKeyUp(int vk)
{
	if (vk == 0) {
		return;
	}
	OnKeyUp((UINT)vk);
}


void CMainFrame::OnSDLTextInput(LPCSTR pszText)
{
	CWindowBase *pWindow;

	pWindow = (m_pMgrWindow != NULL) ? m_pMgrWindow->GetActiveWindow() : NULL;
	if (pWindow == NULL) {
		return;
	}

	pWindow->HandleSDLTextInput(pszText);
}


void CMainFrame::OnSDLMouseMove(int x, int y)
{
	OnMouseMove(x, y);
}


void CMainFrame::OnSDLMouseLeftButtonDown(int x, int y, BOOL bDoubleClick)
{
	UNREFERENCED_PARAMETER(bDoubleClick);
	OnLButtonDown(x, y);
}


void CMainFrame::OnSDLMouseRightButtonDown(int x, int y, BOOL bDoubleClick)
{
	UNREFERENCED_PARAMETER(bDoubleClick);
	OnRButtonDown(x, y);
}


void CMainFrame::OnSDLMouseRightButtonDoubleClick(int x, int y)
{
	OnRButtonDblClk(x, y);
}


BOOL CMainFrame::OnWin32Message(UINT message, WPARAM wParam, LPARAM lParam)
{
#if defined(__EMSCRIPTEN__)
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	return FALSE;
#else
	if ((message >= URARASOCK_MSGBASE) && (message < URARASOCK_MSGBASE + WM_URARASOCK_MAX)) {
		switch (message - URARASOCK_MSGBASE) {
		case WM_URARASOCK_CONNECT:
			OnConnect();
			return TRUE;
		case WM_URARASOCK_DISCONNECT:
			OnDisConnect();
			return TRUE;
		case WM_URARASOCK_RECV:
			OnRecv((PBYTE)wParam);
			return TRUE;
		}
	}

	return FALSE;
#endif
}


void CMainFrame::OnDraw(SDL_Renderer *pRenderer)
{
	DWORD dwTmp;
#if defined(__EMSCRIPTEN__)
	static BOOL s_bLogged = FALSE;
#endif

	if (m_pMgrDraw == NULL) {
#if defined(__EMSCRIPTEN__)
		if (s_bLogged == FALSE) {
			SDL_Log("OnDraw: m_pMgrDraw=NULL");
			s_bLogged = TRUE;
		}
#endif
		return;
	}
	if (!IsMainWindowInteractive()) {
		return;
	}

#if defined(__EMSCRIPTEN__)
	if (s_bLogged == FALSE) {
		SDL_Log("OnDraw: state=%d renderer=%p", m_nGameState, pRenderer);
		s_bLogged = TRUE;
	}
#endif

	dwTmp = SDL_GetTicks();
	m_pMgrDraw->Draw(pRenderer);
	dwTmp = SDL_GetTicks() - dwTmp;
	m_pMgrData->SetDrawTime(dwTmp);
	m_dwDrawTime += dwTmp;
}


BOOL CMainFrame::IsQuit(void)
{
	// SDL_QUIT がメインの終了トリガーなので常にFALSEを返す
	return FALSE;
}


void CMainFrame::OnSDLDestroy(void)
{
#if !defined(__EMSCRIPTEN__)
	RECT rc;
	TCHAR szFileName[MAX_PATH];
	HWND hWndTmp;
	CmyString strTmp;
#endif

	// タイマー停止
	// ウィンドウ位置等をINIに保存（OnClose相当）
#if !defined(__EMSCRIPTEN__)
	GetModuleIniPath(szFileName, _countof(szFileName));

	if (IsMainWindowInteractive() && (m_pSDLWindow != NULL)) {
		int x, y;

		SDL_GetWindowPosition(m_pSDLWindow, &x, &y);
		strTmp.Format(_T("%d"), x);
		WritePrivateProfileString(_T("Pos"), _T("MainX"), strTmp, szFileName);
		strTmp.Format(_T("%d"), y);
		WritePrivateProfileString(_T("Pos"), _T("MainY"), strTmp, szFileName);

		if (m_nGameState == GAMESTATE_MAP) {
			((CStateProcMAP *)m_pStateProc)->GetMsgLogRect(rc);
			strTmp.Format(_T("%d"), rc.left);
			WritePrivateProfileString(_T("Pos"), _T("LogLeft"), strTmp, szFileName);
			strTmp.Format(_T("%d"), rc.top);
			WritePrivateProfileString(_T("Pos"), _T("LogTop"), strTmp, szFileName);
			strTmp.Format(_T("%d"), rc.right);
			WritePrivateProfileString(_T("Pos"), _T("LogRight"), strTmp, szFileName);
			strTmp.Format(_T("%d"), rc.bottom);
			WritePrivateProfileString(_T("Pos"), _T("LogBottom"), strTmp, szFileName);

			hWndTmp = m_pMgrData->GetAdminWindow();
			if (hWndTmp) {
				GetWindowRect(hWndTmp, &rc);
				strTmp.Format(_T("%d"), rc.left);
				WritePrivateProfileString(_T("Pos"), _T("AdminX"), strTmp, szFileName);
				strTmp.Format(_T("%d"), rc.top);
				WritePrivateProfileString(_T("Pos"), _T("AdminY"), strTmp, szFileName);
			}

			hWndTmp = m_pMgrData->GetDebugWindow();
			if (hWndTmp) {
				GetWindowRect(hWndTmp, &rc);
				strTmp.Format(_T("%d"), rc.left);
				WritePrivateProfileString(_T("Pos"), _T("DebugX"), strTmp, szFileName);
				strTmp.Format(_T("%d"), rc.top);
				WritePrivateProfileString(_T("Pos"), _T("DebugY"), strTmp, szFileName);
			}
		}
	}
#endif

	if (m_pMgrData && (!m_pMgrData->IsLocalTitleMode())) {
		m_pMgrData->SaveIniData();
	}
#if !defined(__EMSCRIPTEN__)
	if (m_pSock) {
		m_pSock->SetNotifySink(NULL, NULL);
		m_pSock->Destroy();
	}
#endif
	m_pSDLWindow = NULL;
	SAFE_DELETE(m_pStateProc);
}


void CMainFrame::DisConnectProc(int nID)
{
	ILoginWindow *pWindow;

#if !defined(__EMSCRIPTEN__)
	m_pSock->Destroy();
#endif

	pWindow = m_pMgrWindow->GetLoginWindow();
	if (pWindow == NULL) {
		return;
	}

	switch (nID) {
	case DISCONNECTID_CONNECT: // 接続失敗
		m_pMgrWindow->MakeWindowMSG("サーバーに接続できませんでした", 3000);
		pWindow->Enable(TRUE);
		break;
	case DISCONNECTID_VERSION: // バージョン不一致
		m_pMgrWindow->MakeWindowMSG("クライアントを最新にしてください", 3000);
		pWindow->Enable(TRUE);
		break;
	case DISCONNECTID_USER: // 満員
	case DISCONNECTID_DISABLE: // 禁止
		pWindow->Enable(TRUE);
		break;
	case DISCONNECTID_PASSWORD: // パスワード不一致
		m_pMgrWindow->MakeWindowMSG("パスワードが間違っています", 3000);
		pWindow->Enable(TRUE);
		break;
	case DISCONNECTID_LOGIN: // ログイン済み
		m_pMgrWindow->MakeWindowMSG("ログイン済みです", 3000);
		pWindow->Enable(TRUE);
		break;
	case DISCONNECTID_MAC: // 作成済み
		m_pMgrWindow->MakeWindowMSG("このPCではアカウント作成済みです", 3000);
		pWindow->Enable(TRUE);
		break;
	}
}


void CMainFrame::ChgMoveState(
	BOOL bChgBGM) // [in] TRUE:BGMを変える
{
	BOOL bChgWait;
	DWORD dwMoveWait;
	PCLayerMap pLayerMap;
	PCInfoMapBase pInfoMap;
	PCInfoCharCli pPlayerChar;

	bChgWait = FALSE;
	pLayerMap = (PCLayerMap)m_pMgrLayer->Get(LAYERTYPE_MAP);
	if (pLayerMap == NULL) {
		return;
	}
	pPlayerChar = m_pMgrData->GetPlayerChar();
	if (pPlayerChar == NULL) {
		return;
	}
	pInfoMap = m_pMgrData->GetMap();

	dwMoveWait = pPlayerChar->GetMoveWait();
	if (pPlayerChar->m_bChargeAtack) {
		bChgWait = TRUE;
		dwMoveWait *= 2;
	}

	switch (pPlayerChar->m_nMoveState) {
	case CHARMOVESTATE_MOVE: // 移動中
	case CHARMOVESTATE_BATTLEMOVE: // 戦闘移動中
	case CHARMOVESTATE_BATTLEATACK: // 戦闘攻撃中
		bChgWait = TRUE;
		break;
	case CHARMOVESTATE_STAND: // 立ち
	case CHARMOVESTATE_SWOON: // 気絶
		if (bChgBGM) {
			m_pMgrSound->PlayBGM(pInfoMap->m_dwBGMID);
		}
		bChgWait = TRUE;
		break;
	case CHARMOVESTATE_BATTLE: // 戦闘中
		if (bChgBGM) {
			m_pMgrSound->PlaySound(SOUNDID_CHARI14_A);
			m_pMgrSound->PlayBGM(BGMID_DAICHI_S);
		}
		bChgWait = TRUE;
		break;
	}
	if (bChgWait) {
		pLayerMap->SetScrollWait(dwMoveWait);
	}
}


void CMainFrame::RenewItemArea(void)
{
	RECT rcTmp;
	PCLayerMap pLayerMap;
	PCInfoCharCli pPlayerChar;

	pPlayerChar = m_pMgrData->GetPlayerChar();
	if (pPlayerChar == NULL) {
		return;
	}
	pLayerMap = (PCLayerMap)m_pMgrLayer->Get(LAYERTYPE_MAP);
	if (pLayerMap == NULL) {
		return;
	}

	rcTmp.left = pLayerMap->m_nViewX - (MAPPARTSSIZE * 2);
	rcTmp.right = pLayerMap->m_nViewX + (DRAW_PARTS_X * MAPPARTSSIZE) + (MAPPARTSSIZE * 2);
	rcTmp.top = pLayerMap->m_nViewY - (MAPPARTSSIZE * 2);
	rcTmp.bottom = pLayerMap->m_nViewY + (DRAW_PARTS_Y * MAPPARTSSIZE) + (MAPPARTSSIZE * 2);
	m_pLibInfoItem->SetArea(pPlayerChar->m_dwMapID, &rcTmp);
}


void CMainFrame::SendChat(int nType, LPCSTR pszMsg, DWORD *pdwDst)
{
	int nTmp, nCmdType, nCount;
	PCInfoCharCli pPlayerChar;
	CParamUtil ParamUtil;

	pPlayerChar = m_pMgrData->GetPlayerChar();

	nCmdType = MSGCMDTYPE_NONE;
	ParamUtil.Set(pszMsg);
	nCount = ParamUtil.GetCount();
	if (nCount > 0) {
		nCmdType = GetMsgCmdType(pszMsg);
	}

	switch (nCmdType) {
	case MSGCMDTYPE_CHGFACE: // 表情変更
		{
			CPacketMSGCMD_CHGFACE Packet;

			Packet.Make(pPlayerChar->m_dwCharID, atoi(ParamUtil.GetParam(1)));
			m_pSock->Send(&Packet);
		}
		break;
	case MSGCMDTYPE_CHGHAIR: // 髪変更
		{
			CPacketMSGCMD_PARA1 Packet;

			Packet.Make(MSGCMDTYPE_CHGHAIR, pPlayerChar->m_dwCharID, atoi(ParamUtil.GetParam(1)));
			m_pSock->Send(&Packet);
		}
		break;
	case MSGCMDTYPE_CHGCLOTH: // 服装変更
		{
			CPacketMSGCMD_CHGCLOTH Packet;

			Packet.Make(pPlayerChar->m_dwCharID, atoi(ParamUtil.GetParam(1)));
			m_pSock->Send(&Packet);
		}
		break;
	case MSGCMDTYPE_CHGACCE: // アクセサリ変更
		{
			CPacketMSGCMD_CHGACCE Packet;

			Packet.Make(pPlayerChar->m_dwCharID, atoi(ParamUtil.GetParam(1)));
			m_pSock->Send(&Packet);
		}
		break;
	case MSGCMDTYPE_BGMVOLUME: // BGM音量設定
		nTmp = atoi(ParamUtil.GetParam(1));
		m_pMgrData->SetBGMVolume(nTmp);
		m_pMgrSound->SetBGMVolume(nTmp);
		break;
	case MSGCMDTYPE_CHGCOLOR: // 色変更
		{
			CPacketMSGCMD_CHGCOLOR Packet;

			Packet.Make(pPlayerChar->m_dwCharID, atoi(ParamUtil.GetParam(1)));
			m_pSock->Send(&Packet);
		}
		break;
	case MSGCMDTYPE_SEVOLUME: // 効果音量設定
		nTmp = atoi(ParamUtil.GetParam(1));
		m_pMgrData->SetSEVolume(nTmp);
		m_pMgrSound->SetSEVolume(nTmp);
		break;
	case MSGCMDTYPE_CHGARMS: // 持ち物変更
		{
			CPacketMSGCMD_CHGARMS Packet;

			Packet.Make(pPlayerChar->m_dwCharID, atoi(ParamUtil.GetParam(1)));
			m_pSock->Send(&Packet);
		}
		break;
	case MSGCMDTYPE_CHGSHIELD: // 盾変更
		{
			int nTmp;
			LPCSTR pszTmp;
			CPacketMSGCMD_PARA1 Packet;

			nTmp = 0;
			pszTmp = ParamUtil.GetParam(1);
			if (pszTmp) {
				nTmp = atoi(pszTmp);
			}
			Packet.Make(SBOCOMMANDID_SUB_MSGCMD_CHGSHIELD, pPlayerChar->m_dwCharID, nTmp);
			m_pSock->Send(&Packet);
		}
		break;
	case MSGCMDTYPE_SETITEM: // 配置アイテム変更
		m_pMgrData->SetSetItemID(atoi(ParamUtil.GetParam(1)));
		break;
	case MSGCMDTYPE_MAKEITEM: // アイテム作成
		{
			CPacketMSGCMD_MAKEITEM Packet;

			Packet.Make(pPlayerChar->m_dwCharID, atoi(ParamUtil.GetParam(1)));
			m_pSock->Send(&Packet);
		}
		break;
	case MSGCMDTYPE_BALLOON: // 噴出し
		{
			LPCSTR pszTmp;
			CPacketMSGCMD_PARA1 Packet;

			pszTmp = ParamUtil.GetParam(0);
			Packet.Make(SBOCOMMANDID_SUB_MSGCMD_BALLOON, pPlayerChar->m_dwCharID, atoi(&pszTmp[1]));
			m_pSock->Send(&Packet);
			if (pdwDst) {
				*pdwDst = atoi(&pszTmp[1]);
			}
		}
		break;
	case MSGCMDTYPE_DICE: // サイコロ
		{
			CPacketMSGCMD_PARA1 Packet;

			Packet.Make(SBOCOMMANDID_SUB_MSGCMD_DICE, pPlayerChar->m_dwCharID, 0);
			m_pSock->Send(&Packet);
		}
		break;
	case MSGCMDTYPE_RND: // ランダム
		{
			int nTmp;
			LPCSTR pszTmp;
			CPacketMSGCMD_PARA1 Packet;

			nTmp = 0;
			pszTmp = ParamUtil.GetParam(1);
			if (pszTmp) {
				nTmp = atoi(pszTmp);
			}
			Packet.Make(SBOCOMMANDID_SUB_MSGCMD_RND, pPlayerChar->m_dwCharID, nTmp);
			m_pSock->Send(&Packet);
		}
		break;
	case MSGCMDTYPE_NOW: // 現在時刻
		{
			SYSTEMTIME sysTime;
			CPacketCHAR_REQ_CHAT Packet;
			CmyString strTmp;

			GetLocalTime(&sysTime);
			strTmp.Format(_T("%02d:%02d:%02d"), sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			Packet.Make(nType, pPlayerChar->m_dwCharID, (LPCSTR)strTmp);
			m_pSock->Send(&Packet);
		}
		break;
	case MSGCMDTYPE_EFFECT: // エフェクト
		{
			LPCSTR pszTmp;
			CPacketMSGCMD_PARA1 Packet;

			pszTmp = ParamUtil.GetParam(0);
			Packet.Make(SBOCOMMANDID_SUB_MSGCMD_EFFECT, pPlayerChar->m_dwCharID, atoi(&pszTmp[1]));
			m_pSock->Send(&Packet);
			if (pdwDst) {
				*pdwDst = atoi(&pszTmp[1]);
			}
		}
		break;
	case MSGCMDTYPE_WHERE: // 最も集まっている場所
		{
			CPacketMSGCMD_PARA1 Packet;

			Packet.Make(SBOCOMMANDID_SUB_MSGCMD_WHERE, pPlayerChar->m_dwCharID, 0);
			m_pSock->Send(&Packet);
		}
		break;
	default:
		{
			CPacketCHAR_REQ_CHAT Packet;

			Packet.Make(nType, pPlayerChar->m_dwCharID, pszMsg);
			m_pSock->Send(&Packet);
		}
		break;
	}
}


void CMainFrame::OnInitEnd(HWND hWnd)
{
	BOOL bRet, bResult;
	TCHAR szName[MAX_PATH], szTmp[MAX_PATH];
	LPTSTR pszTmp;
	GUID stGuid;

	bRet = FALSE;

#if defined(__EMSCRIPTEN__)
	SDL_Log("OnInitEnd: start");
#endif

	// 各マネージャクラスを作成
	m_pMgrData->SetWindowInfo(GetModuleHandle(NULL), hWnd);
	m_pMgrData->Create(this, m_pMgrGrpData);
	m_pMgrData->SetUraraSockTCP(m_pSock);
#if !defined(__EMSCRIPTEN__)
	if (m_pSock) {
		m_pSock->SetNotifySink(&CMainFrame::OnSocketNotifyThunk, this);
	}
#endif

	m_pMgrDraw = m_pMgrData->GetMgrDraw();
	m_pMgrLayer = m_pMgrData->GetMgrLayer();
	m_pMgrWindow = m_pMgrData->GetMgrWindow();
	m_pMgrKeyInput = m_pMgrData->GetMgrKeyInput();
	m_pMgrSound = m_pMgrData->GetMgrSound();
	m_pLibInfoChar = m_pMgrData->GetLibInfoChar();
	m_pLibInfoDisable = m_pMgrData->GetLibInfoDisable();
	m_pLibInfoMap = m_pMgrData->GetLibInfoMap();
	m_pLibInfoMapObject = m_pMgrData->GetLibInfoMapObject();
	m_pLibInfoMapParts = m_pMgrData->GetLibInfoMapParts();
	m_pLibInfoMapShadow = m_pMgrData->GetLibInfoMapShadow();
	m_pLibInfoItemType = m_pMgrData->GetLibInfoItemType();
	m_pLibInfoItem = m_pMgrData->GetLibInfoItem();
	m_pLibInfoItemWeapon = m_pMgrData->GetLibInfoItemWeapon();
	m_pLibInfoMotion = m_pMgrData->GetLibInfoMotion();
	m_pLibInfoMotionType = m_pMgrData->GetLibInfoMotionType();
	m_pLibInfoEfcBalloon = m_pMgrData->GetLibInfoEfcBalloon();
	m_pLibInfoEffect = m_pMgrData->GetLibInfoEffect();
	m_pLibInfoSystem = m_pMgrData->GetLibInfoSystem();
	m_pLibInfoSkill = m_pMgrData->GetLibInfoSkill();

	// グラフィックデータDLLの読み込み
	bResult = m_pMgrGrpData->Load();
	if ((bResult == FALSE) && m_pMgrData->IsLocalTitleMode()) {
		bResult = m_pMgrGrpData->LoadLocalTitleAssets();
	}
#if defined(__EMSCRIPTEN__)
	SBOP2_DebugMarkInitEnd(bResult ? 1 : 0);
#endif
#if defined(__EMSCRIPTEN__)
	SDL_Log("OnInitEnd: grp load result=%d localTitle=%d", bResult, m_pMgrData->IsLocalTitleMode());
#endif
	if (bResult == FALSE) {
#if defined(__EMSCRIPTEN__)
		SDL_Log("OnInitEnd: グラフィックデータDLLの読み込みに失敗しました");
#else
		if (hWnd) {
			MessageBox(hWnd, _T("グラフィックデータDLLの読み込みに失敗しました"), _T("エラー"), MB_OK);
		}
#endif
		goto Exit;
	}

	if (m_pMgrData->IsLocalTitleMode() == FALSE) {
		stGuid = m_pMgrData->GetInputGuid();
		m_pMgrKeyInput->SetDevice(stGuid);
	}

#if !defined(__EMSCRIPTEN__)
	if (m_pMgrData->IsLocalTitleMode() == FALSE) {
		BuildModuleRelativePath(szName, _countof(szName), _T("ss"));
		CreateDirectory(szName, NULL);
		BuildModuleRelativePath(szTmp, _countof(szTmp), _T("Log"));
		CreateDirectory(szTmp, NULL);
	}
#endif

	bResult = FALSE;
	if (m_pMgrData->IsLocalTitleMode() == FALSE) {
		bResult = m_pMgrData->GetDisableLogin();
	}
	if (bResult) {
		goto Exit;
	}
	ChgGameState(GAMESTATE_LOGO);
#if defined(__EMSCRIPTEN__)
	SDL_Log("OnInitEnd: changed to state=%d", m_nGameState);
#endif
#if !defined(__EMSCRIPTEN__)
	if (hWnd) {
		ShowWindow(hWnd, SW_SHOW);
	}
#endif

	PostMainFrameMessage(MAINFRAMEMSG_RENEWVIEWSET, 0);

	bRet = TRUE;
Exit:
	if (bRet == FALSE) {
		PushSDLQuitEvent();
	}
}


void CMainFrame::OnKeyUp(UINT vk)
{
	if (m_pStateProc == NULL) {
		return;
	}
	if (vk == VK_SNAPSHOT) {
		m_pMgrDraw->SaveScreenShot();
	}
}


void CMainFrame::OnLButtonDown(int x, int y)
{
	CWindowBase *pWindow;

	if (m_pStateProc == NULL) {
		return;
	}

	pWindow = (m_pMgrWindow != NULL) ? m_pMgrWindow->GetActiveWindow() : NULL;
	if ((pWindow != NULL) && pWindow->HandleSDLMouseLeftButtonDown(x, y)) {
		return;
	}
	m_pStateProc->OnLButtonDown(x, y);
}


void CMainFrame::OnRButtonDown(int x, int y)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnRButtonDown(x, y);
}


void CMainFrame::OnRButtonDblClk(int x, int y)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnRButtonDblClk(x, y);
}


void CMainFrame::OnMouseMove(int x, int y)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnMouseMove(x, y);
}


void CMainFrame::OnMgrDraw(int nCode, DWORD dwPara)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnMgrDraw(nCode, dwPara);
}


void CMainFrame::OnWindowMsg(int nCode, DWORD dwPara)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnWindowMsg(nCode, dwPara);
}


void CMainFrame::OnAdminMsg(int nCode, DWORD dwPara)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnAdminMsg(nCode, dwPara);
}


void CMainFrame::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	if (m_pStateProc == NULL) {
		return;
	}
	switch (dwCommand) {
	case MAINFRAMEMSG_CHGSTATE: // 状態変更
		ChgGameState(dwParam);
		break;
	case MAINFRAMEMSG_CONNECT: // 接続
		Connect();
		break;
	case MAINFRAMEMSG_RENEWVIEWSET: // 表示設定更新
		m_nDrawCount = 30;
		if (m_pMgrData->GetOption60Frame() == TRUE) {
			m_nDrawCount = 60;
		}
		g_nSboCliRenderFrameRate = m_nDrawCount;
		m_nFPS = 1000 / m_nDrawCount + 1;
		break;
	default:
		m_pStateProc->OnMainFrame(dwCommand, dwParam);
		break;
	}
}


void CMainFrame::OnConnect(void)
{
#if defined(__EMSCRIPTEN__)
	return;
#else
	CPacketVERSION_REQ_VERSIONCHECK Packet;

	Packet.Make(VERSIONVAL);
	m_pSock->Send(&Packet);
#endif
}


void CMainFrame::OnDisConnect(void)
{
#if defined(__EMSCRIPTEN__)
	return;
#else
	switch (m_nGameState) {
	case GAMESTATE_LOGIN: // ログイン画面
		DisConnectProc(DISCONNECTID_CONNECT);
		break;
	case GAMESTATE_MAP: // マップ画面
		m_pMgrData->AddSystemMsg(TRUE, "サーバーとの接続が切れました", RGB(0, 200, 255));
		m_pMgrSound->PlaySound(SOUNDID_W_CHAT);

		DispatchMainFrameMessage(MAINFRAMEMSG_RENEWSYSTEMMSG, 0);
		FlashMainWindow();
		ChgGameState(GAMESTATE_DISCONNECT);
		break;
	default: // その他
		ChgGameState(GAMESTATE_DISCONNECT);
		break;
	}
#endif
}


void CMainFrame::OnRecv(PBYTE pData)
{
#if defined(__EMSCRIPTEN__)
	UNREFERENCED_PARAMETER(pData);
	return;
#else
	CPacketBase Packet;

	Packet.Set(pData);

#if defined(_DEBUG) && !defined(__EMSCRIPTEN__)
	static TCHAR s_szLogPath[MAX_PATH] = {0};
	if (s_szLogPath[0] == 0) {
		BuildModuleRelativePath(s_szLogPath, _countof(s_szLogPath), _T("SboCli_PacketLog.txt"));
	}
	if (s_szLogPath[0] != 0) {
		SYSTEMTIME st;
		GetLocalTime(&st);
		FILE *fp = NULL;
		if (_tfopen_s(&fp, s_szLogPath, _T("a, ccs=UNICODE")) == 0 && fp) {
			_ftprintf(fp, _T("%04d-%02d-%02d %02d:%02d:%02d.%03d\tmain=%u\tsub=%u\n"),
				st.wYear, st.wMonth, st.wDay,
				st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				Packet.m_byCmdMain, Packet.m_byCmdSub);
			fclose(fp);
		}
	}
#endif

	switch (Packet.m_byCmdMain) {
	case SBOCOMMANDID_MAIN_VERSION: RecvProcVERSION(Packet.m_byCmdSub, pData); break; // バージョン系
	case SBOCOMMANDID_MAIN_CONNECT: RecvProcCONNECT(Packet.m_byCmdSub, pData); break; // 接続系
	case SBOCOMMANDID_MAIN_ACCOUNT: RecvProcACCOUNT(Packet.m_byCmdSub, pData); break; // アカウント系
	case SBOCOMMANDID_MAIN_CHAR: RecvProcCHAR(Packet.m_byCmdSub, pData); break; // キャラ系
	case SBOCOMMANDID_MAIN_MAP: RecvProcMAP(Packet.m_byCmdSub, pData); break; // マップ系
	case SBOCOMMANDID_MAIN_ITEM: RecvProcITEM(Packet.m_byCmdSub, pData); break; // アイテム系
	case SBOCOMMANDID_MAIN_ADMIN: RecvProcADMIN(Packet.m_byCmdSub, pData); break; // 管理者系
	case SBOCOMMANDID_MAIN_EFFECT: RecvProcEFFECT(Packet.m_byCmdSub, pData); break; // エフェクト系
	case SBOCOMMANDID_MAIN_SYSTEM: RecvProcSYSTEM(Packet.m_byCmdSub, pData); break; // システム系
	case SBOCOMMANDID_MAIN_SKILL: RecvProcSKILL(Packet.m_byCmdSub, pData); break; // スキル系
	}

	m_pSock->DeleteRecvData(pData);
#endif
}


BOOL CMainFrame::TimerProc(void)
{
	BOOL bRet, bDraw, bResult;
	PCInfoCharCli pPlayerChar;

	bRet = FALSE;
	bDraw = FALSE;
	bDraw |= m_pMgrDraw->TimerProc();
	bDraw |= m_pMgrLayer->TimerProc();
	bDraw |= m_pMgrWindow->TimerProc();
#if defined(__EMSCRIPTEN__)
	if (m_pStateProc) {
		bDraw |= m_pStateProc->TimerProc();
	}
#else
	bDraw |= m_pLibInfoChar->Proc();
	bDraw |= m_pLibInfoMapParts->Proc();
	bDraw |= m_pLibInfoMapShadow->Proc();

	if (m_pStateProc) {
		bDraw |= m_pStateProc->TimerProc();
	}

	if (m_bRenewCharInfo) {
		m_bRenewCharInfo = FALSE;
		pPlayerChar = m_pMgrData->GetPlayerChar();
		if (pPlayerChar) {
			// 重なり調整
			m_pLibInfoChar->SortY();

			bResult = m_pLibInfoChar->DeleteOutScreen(pPlayerChar);
			if (bResult) {
				PostMainFrameMessage(MAINFRAMEMSG_RENEWCHARCOUNT, m_pLibInfoChar->GetCount());
			}
		}
	}
#endif

#if !defined(__EMSCRIPTEN__)
	MsgWaitForMultipleObjects(0, NULL, FALSE, 1, QS_ALLINPUT);
#endif

	bRet = bDraw;
	return bRet;
}


void CMainFrame::UpdateToolCheck(void)
{
#if defined(__EMSCRIPTEN__)
	return;
#else
	DWORD dwNow;

	if ((m_pMgrData == NULL) || m_pMgrData->IsLocalTitleMode()) {
		return;
	}

	dwNow = SDL_GetTicks();
	if (m_dwLastToolCheckTick == 0) {
		m_dwLastToolCheckTick = dwNow;
		return;
	}
	if ((dwNow - m_dwLastToolCheckTick) < TIMER_TOOLCHECK) {
		return;
	}

	m_dwLastToolCheckTick = dwNow;
	CheckToolResponsiveness();
#endif
}


void CMainFrame::CheckToolResponsiveness(void)
{
#if !defined(__EMSCRIPTEN__)
	DWORD dwTmp, dwScond, dwTimeTmp;
	SYSTEMTIME sysTime;

	if (m_dwLastTimeCheck == 0) {
		GetLocalTime(&m_stSystemTime);
		m_dwLastTimeCheck = timeGetTime();
		return;
	}
	GetLocalTime(&sysTime);
	dwTimeTmp = timeGetTime();
	if (sysTime.wMinute != m_stSystemTime.wMinute) {
		m_dwLastTimeCheck = 0;
		return;
	}
	if (sysTime.wSecond == m_stSystemTime.wSecond) {
		return;
	}
	dwTmp = (dwTimeTmp - m_dwLastTimeCheck) / 1000;
	dwScond = abs(sysTime.wSecond - m_stSystemTime.wSecond);
	if (dwScond == dwTmp) {
		m_dwLastTimeCheck = 0;
		return;
	}
	if (dwTmp < dwScond * 3) {
		m_dwLastTimeCheck = 0;
		return;
	}
	PushSDLQuitEvent();
#endif
}


void CMainFrame::OnSocketNotifyThunk(void *pUserData, UINT uMsgOffset, WPARAM wParam, LPARAM lParam)
{
	CMainFrame *pThis;

	pThis = (CMainFrame *)pUserData;
	if (pThis == NULL) {
		return;
	}

	pThis->PostSocketMessage(URARASOCK_MSGBASE + uMsgOffset, wParam, lParam);
}


void CMainFrame::PostSocketMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	SocketNotify stNotify;

	stNotify.message = message;
	stNotify.wParam = wParam;
	stNotify.lParam = lParam;

	EnterCriticalSection(&m_csSocketNotify);
	m_aPendingSocketNotify.push_back(stNotify);
	LeaveCriticalSection(&m_csSocketNotify);
}


void CMainFrame::FlushPendingSocketMessages(void)
{
	std::deque<SocketNotify> aNotify;

	EnterCriticalSection(&m_csSocketNotify);
	aNotify.swap(m_aPendingSocketNotify);
	LeaveCriticalSection(&m_csSocketNotify);

	while (!aNotify.empty()) {
		const SocketNotify stNotify = aNotify.front();
		aNotify.pop_front();
		OnWin32Message(stNotify.message, stNotify.wParam, stNotify.lParam);
	}
}


void CMainFrame::KeyProc(void)
{
	int i;
	BOOL bResult, bDown;
	BYTE byCode, byCodeTmp, abyCode[] = {VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, 0};

	if (m_pMgrKeyInput == NULL) {
		return;
	}
	if (!IsMainWindowInteractive()) {
		return;
	}

	// ウィンドウで必要であればウィンドウ、そうでなければ状態処理のキー処理を行う
	m_pMgrKeyInput->Renew(byCode, bDown);
	if (byCode == 0) {
		for (i = 0; ; i++) {
			byCodeTmp = abyCode[i];
			if (byCodeTmp == 0) {
				break;
			}
			bResult = m_pMgrKeyInput->IsInput(byCodeTmp);
			if (bResult) {
				byCode = byCodeTmp;
				bDown = TRUE;
				break;
			}
		}
	}
	if ((byCode == 0) && (bDown == FALSE)) {
		return;
	}
	bResult = m_pMgrWindow->IsKeyInput();
	if (bResult) {
		m_pMgrWindow->KeyProc(byCode, bDown);

	} else {
		if (m_pStateProc) {
			m_pStateProc->KeyProc(byCode, bDown);
		}
	}
}


void CMainFrame::ChgGameState(int nGameState)
{
#if defined(__EMSCRIPTEN__)
	SDL_Log("ChgGameState: %d -> %d", m_nGameState, nGameState);
#endif
	m_pMgrLayer->DeleteAll();
	m_pMgrWindow->DeleteAll();
	SAFE_DELETE(m_pStateProc);

	m_nGameState = nGameState;

	switch (nGameState) {
	case GAMESTATE_LOGO: m_pStateProc = new CStateProcLOGO; break; // URARA-worksロゴ
	case GAMESTATE_LOGIN: m_pStateProc = new CStateProcLOGIN; break; // ログイン画面
#if !defined(__EMSCRIPTEN__)
	case GAMESTATE_DISCONNECT: m_pStateProc = new CStateProcDISCONNECT; break; // 切断
	case GAMESTATE_INFO: m_pStateProc = new CStateProcINFO; break; // お知らせ画面
	case GAMESTATE_LOGINMENU: m_pStateProc = new CStateProcLOGINMENU; break; // メニュー画面
	case GAMESTATE_MAP: m_pStateProc = new CStateProcMAP; break; // マップ画面
#endif
	}

	if (m_pStateProc) {
		m_pStateProc->Create(m_pMgrData, m_pSock);
		m_pStateProc->Init();
	}
	m_bRequestDraw = TRUE;
#if defined(__EMSCRIPTEN__)
	SBOP2_DebugMarkGameState(m_nGameState, (m_pStateProc != NULL) ? 1 : 0);
#endif
}


void CMainFrame::Connect(void)
{
#if defined(__EMSCRIPTEN__)
	return;
#else
	WORD wPort;
	CmyString strTmp;

	if ((m_pMgrData == NULL) || m_pMgrData->IsLocalTitleMode()) {
		return;
	}

	strTmp = m_pMgrData->GetServerAddr();
	wPort = m_pMgrData->GetServerPort();

	m_pMgrWindow->MakeWindowMSG("サーバーに接続しています");
	m_pSock->Connect(NULL, URARASOCK_MSGBASE, URARASOCK_PRECHECK, wPort, strTmp);
#endif
}


void CMainFrame::FlashMainWindow(void)
{
#if !defined(__EMSCRIPTEN__)
	UINT uState;
	FLASHWINFO stFwi;
	APPBARDATA stAbd;

	if ((m_pMgrData == NULL) || m_pMgrData->IsLocalTitleMode()) {
		return;
	}
	if (m_bWindowActive) {
		return;
	}
	ZeroMemory(&stAbd, sizeof(stAbd));
	stAbd.cbSize = sizeof(stAbd);
	uState = SHAppBarMessage(ABM_GETSTATE, &stAbd);
	if (uState & ABS_AUTOHIDE) {
		return;
	}
	if (m_pMgrData->GetOptionTaskbar() == FALSE) {
		return;
	}

	ZeroMemory(&stFwi, sizeof(stFwi));
	stFwi.cbSize = sizeof(stFwi);
	stFwi.hwnd = m_hWnd;
	stFwi.dwFlags = FLASHW_TRAY;
	stFwi.uCount = 4;
	stFwi.dwTimeout = 300;
	FlashWindowEx(&stFwi);
#endif
}


BOOL CMainFrame::IsMainWindowInteractive(void) const
{
	Uint32 dwWindowFlags;

	if (m_pSDLWindow == NULL) {
		return TRUE;
	}

	dwWindowFlags = SDL_GetWindowFlags(m_pSDLWindow);
	if (dwWindowFlags & SDL_WINDOW_HIDDEN) {
		return FALSE;
	}
	if (dwWindowFlags & SDL_WINDOW_MINIMIZED) {
		return FALSE;
	}

	return TRUE;
}


void CMainFrame::FlushPendingMainFrameMessages(void)
{
	std::deque<MainFrameNotify> aNotify;

	EnterCriticalSection(&m_csMainFrameNotify);
	aNotify.swap(m_aPendingMainFrameNotify);
	LeaveCriticalSection(&m_csMainFrameNotify);

	while (!aNotify.empty()) {
		const MainFrameNotify stNotify = aNotify.front();
		aNotify.pop_front();
		DispatchMainFrameMessage(stNotify.dwCommand, stNotify.dwParam);
	}
}


void CMainFrame::FlushPendingWindowMessages(void)
{
	std::deque<WindowNotify> aNotify;

	EnterCriticalSection(&m_csWindowNotify);
	aNotify.swap(m_aPendingWindowNotify);
	LeaveCriticalSection(&m_csWindowNotify);

	while (!aNotify.empty()) {
		const WindowNotify stNotify = aNotify.front();
		aNotify.pop_front();
		DispatchWindowMessage(stNotify.nType, stNotify.dwParam);
	}
}


void CMainFrame::FlushPendingAdminMessages(void)
{
	std::deque<AdminNotify> aNotify;

	EnterCriticalSection(&m_csAdminNotify);
	aNotify.swap(m_aPendingAdminNotify);
	LeaveCriticalSection(&m_csAdminNotify);

	while (!aNotify.empty()) {
		const AdminNotify stNotify = aNotify.front();
		aNotify.pop_front();
		DispatchAdminMessage(stNotify.nType, stNotify.dwParam);
	}
}


void CMainFrame::FlushPendingMgrDrawMessages(void)
{
	std::deque<MgrDrawNotify> aNotify;

	EnterCriticalSection(&m_csMgrDrawNotify);
	aNotify.swap(m_aPendingMgrDrawNotify);
	LeaveCriticalSection(&m_csMgrDrawNotify);

	while (!aNotify.empty()) {
		const MgrDrawNotify stNotify = aNotify.front();
		aNotify.pop_front();
		DispatchMgrDrawMessage(stNotify.nCode, stNotify.dwParam);
	}
}


int CMainFrame::GetMsgCmdType(LPCSTR pszText)
{
	int nRet, nCount;
	LPCSTR pszTmp;
	CParamUtil ParamUtil;

	nRet = MSGCMDTYPE_NONE;

	if (pszText[0] == '#') {
		return MSGCMDTYPE_BALLOON;
	}

	if (pszText[0] == '$') {
		return MSGCMDTYPE_EFFECT;
	}

	if (pszText[0] != '/') {
		goto Exit;
	}

	ParamUtil.Set(pszText);
	nCount = ParamUtil.GetCount();
	pszTmp = ParamUtil.GetParam(0);

	// 表情変更
	if (strcmp(&pszTmp[1], "face") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGFACE;
		}

	// 髪変更
	} else if (strcmp(&pszTmp[1], "hair") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGHAIR;
		}

	// 服装変更
	} else if (strcmp(&pszTmp[1], "cloth") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGCLOTH;
		}

	// アクセサリ変更
	} else if (strcmp(&pszTmp[1], "acce") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGACCE;
		}

	// BGM音量設定
	} else if (strcmp(&pszTmp[1], "bgm") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_BGMVOLUME;
		}

	// 色変更
	} else if (strcmp(&pszTmp[1], "color") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGCOLOR;
		}

	// 効果音量設定
	} else if (strcmp(&pszTmp[1], "se") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_SEVOLUME;
		}

	// 持ち物変更
	} else if (strcmp(&pszTmp[1], "arms") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGARMS;
		}

	// 盾変更
	} else if (strcmp(&pszTmp[1], "shield") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGSHIELD;
		}

	// 配置アイテム変更
	} else if (strcmp(&pszTmp[1], "item") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_SETITEM;
		}

	// アイテム作成
	} else if (strcmp(&pszTmp[1], "makeitem") == 0) {
		if (m_pMgrData->GetAdminLevel() > ADMINLEVEL_NONE) {
			if (nCount == 2) {
				nRet = MSGCMDTYPE_MAKEITEM;
			}
		}

	// 噴出し
//	} else if (strcmp(&pszTmp[1], "balloon") == 0) {
//		if (nCount == 2) {
//			nRet = MSGCMDTYPE_BALLOON;
//		}

	// サイコロ
	} else if (strcmp(&pszTmp[1], "dice") == 0) {
		if (nCount == 1) {
			nRet = MSGCMDTYPE_DICE;
		}

	// ランダム
	} else if (strcmp(&pszTmp[1], "rnd") == 0) {
		if (nCount <= 2) {
			nRet = MSGCMDTYPE_RND;
		}

	// 現在時刻
	} else if (strcmp(&pszTmp[1], "now") == 0) {
		if (nCount == 1) {
			nRet = MSGCMDTYPE_NOW;
		}

	// 最も集まっている場所
	} else if (strcmp(&pszTmp[1], "where") == 0) {
		if (nCount == 1) {
			nRet = MSGCMDTYPE_WHERE;
		}
	}

Exit:
	return nRet;
}
