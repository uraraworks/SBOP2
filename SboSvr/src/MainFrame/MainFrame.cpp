/// @file MainFrame.cpp
/// @brief サーバーメインフレーム 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/11/04
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#include <math.h>
#include <time.h>
#include "resource.h"
#include "SBOVersion.h"
#include "UraraSockTCPSBO.h"
#include "Command.h"
#include "Packet.h"
#include "LibInfoAccount.h"
#include "LibInfoMapBase.h"
#include "LibInfoCharSvr.h"
#include "LibInfoDisable.h"
#include "TextOutput.h"
#include "InfoCharSvr.h"
#include "MgrData.h"
#include "MainFrame.h"
#include "Web/HttpServer.h"
#include "Web/WebSocketBridge.h"
#include "../Platform/SvrPlatform.h"

// 定数定義

#define CLNAME "SboSvr"	// 登録クラス名
#define TIMER_REDRAW	(1000)	// 再描画周期
#define TIMER_SAVE	(1000 * 60 * 30)	// 保存周期(30分)

CMainFrame::CMainFrame()
{
	m_dwServerStartTime	= 0;
	m_dwLastKeepaliveCheck	= 0;
	m_dwLastClockTime	= 0;
	m_dwLastSaveTime	= 0;
	m_hWnd	= NULL;
	m_bHeadless	= FALSE;
	m_bQuit	= FALSE;
	m_pLibInfoAccount	= NULL;
	m_pLibInfoChar	= NULL;
	m_pLibInfoDisable	= NULL;
	m_pLibInfoMap	= NULL;
	m_pLibInfoMapObject	= NULL;
	m_pLibInfoMapParts	= NULL;
	m_pLibInfoMapShadow	= NULL;
	m_pLibInfoItemType	= NULL;
	m_pLibInfoItem	= NULL;
	m_pLibInfoItemWeapon	= NULL;
	m_pLibInfoEffect	= NULL;
	m_pLibInfoMotion	= NULL;
	m_pLibInfoMotionType	= NULL;
	m_pLibInfoEfcBalloon	= NULL;
	m_pLibInfoSystem	= NULL;
	m_pLibInfoSkill	= NULL;
	m_pLibInfoTalkEvent	= NULL;
	m_pLog	= NULL;
	m_pHttpServer	= NULL;
	m_pWebSocketBridge	= NULL;

	m_pSock	= new CUraraSockTCPSBO;
	m_pMgrData	= new CMgrData;
	m_pHttpServer	= new CHttpServer;
	m_pWebSocketBridge	= new CWebSocketBridge;

        m_hFont = CreateFont(12, 0, 0, 0, FW_NORMAL,
                        FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
                        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("ＭＳ ゴシック"));
}

CMainFrame::~CMainFrame()
{
	if (m_hFont) {
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}
	if (m_pHttpServer) {
		m_pHttpServer->Stop();
	}
	if (m_pWebSocketBridge) {
		m_pWebSocketBridge->Stop();
	}
	// m_pSock が生きているうちに未処理の受信データを解放する
	ClearSockNotify();

	SAFE_DELETE(m_pMgrData);
	SAFE_DELETE(m_pSock);
	SAFE_DELETE(m_pHttpServer);
	SAFE_DELETE(m_pWebSocketBridge);
}

CMainFrame *CMainFrame::s_pInstance = NULL;

// SboPlatform::InstallStopSignalHandler() へ渡す: 終了要求を立てる
//
// ヘッドレス動作時に Ctrl+C(またはそれに相当するシグナル)で安全に
// 停止させるために使う。別スレッド/シグナルコンテキストから呼ばれるため、
// 終了要求を立てるだけにする。停止処理(DB の書き戻しを含む)はメイン
// スレッドが行う。

void CMainFrame::RequestQuitStatic(void)
{
	if (s_pInstance) {
		s_pInstance->RequestQuit();
	}
}

// SboPlatform::InstallStopSignalHandler() へ渡す: 終了処理が完了したか
//
// Windows 実装はこれが true を返すまでハンドラ内で待つ
// (ここで返すと OS にプロセスを落とされ、DB の書き戻しが行われないため)。

bool CMainFrame::IsQuittingDoneStatic(void)
{
	return !(s_pInstance && (s_pInstance->m_bQuit != FALSE));
}

int CMainFrame::MainLoop(HINSTANCE hInstance, BOOL bHeadless)
{
	int nRet;

	m_bHeadless	= bHeadless;
	m_bQuit	= FALSE;
	s_pInstance	= this;

	if (bHeadless) {
		nRet = MainLoopHeadless();
	} else {
		nRet = MainLoopWindow(hInstance);
	}

	s_pInstance	= NULL;
	return nRet;
}

// メインループ(ヘッドレス)
//
// ウィンドウもメッセージポンプも作らない。通信の通知は
// SetNotifySink() 経由でキューに入るため、ウィンドウは不要。
//
// 停止はコンソールシグナル(Ctrl+C など)で行う。親プロセスの
// コンソールに接続できた場合のみ受け取れる点に注意。

int CMainFrame::MainLoopHeadless(void)
{
	WORD wPort;

	// 親のコンソールに繋がれば Ctrl+C とメッセージ出力ができる
	SboPlatform::AttachParentConsole();

	// 多重起動防止ロックと停止要求の受け口は、初期化より前に確保する。
	// 後に回すと、二重起動したときに既存インスタンスと同じポートを
	// 一時的に奪ってから競合に気づくことになる。
	//
	// 停止要求の受け口が無いと強制終了しか手段が無くなり、
	// DB の書き戻しが飛ぶため必須。
	wPort = (WORD)SboPlatform::GetIniInt(SboPlatform::GetIniFilePath().c_str(), "Setting", "Port", 2006);

	if (SboPlatform::AcquireServerInstanceLock(wPort) == false) {
		WriteConsoleMessage(_T("同じポート(%u)のサーバーが既に起動しています"), (unsigned int)wPort);
		return SBOSVR_EXIT_ALREADY_RUNNING;
	}
	if (SboPlatform::OpenStopRequestChannel(wPort) == false) {
		// 多重起動ではない(ロックは取れている)。受け口だけが作れなかった
		// 状態で起動すると --stop も Ctrl+C も効かなくなるため、ここで諦める。
		WriteConsoleMessage(_T("停止要求の受け口を用意できませんでした (Port:%u)"), (unsigned int)wPort);
		SboPlatform::ReleaseServerInstanceLock();
		return SBOSVR_EXIT_ERROR;
	}

	SboPlatform::InstallStopSignalHandler(&CMainFrame::RequestQuitStatic, &CMainFrame::IsQuittingDoneStatic);

	if (InitServer() == FALSE) {
		WriteConsoleMessage(_T("サーバーの初期化に失敗しました"));
		SboPlatform::UninstallStopSignalHandler();
		SboPlatform::CloseStopRequestChannel();
		SboPlatform::ReleaseServerInstanceLock();
		return SBOSVR_EXIT_ERROR;
	}

	if (m_pLog) {
		m_pLog->Write("ヘッドレスで起動しました。停止は --stop または Ctrl+C");
	}
	WriteConsoleMessage(_T("SboSvr をヘッドレスで起動しました (Port:%u)。停止は --stop または Ctrl+C"), (unsigned int)wPort);

	SboPlatform::BeginHighResolutionTimer();

	while (m_bQuit == FALSE) {
		if (SboPlatform::IsStopRequested()) {
			break;
		}
		TimerProc();
	}

	SboPlatform::EndHighResolutionTimer();

	if (m_pLog) {
		m_pLog->Write("停止要求を受け付けました");
	}

	TermServer();
	m_pMgrData->Save();
	WriteConsoleMessage(_T("SboSvr を停止しました"));
	SboPlatform::CloseStopRequestChannel();
	SboPlatform::ReleaseServerInstanceLock();

	// ここで初めてシグナルハンドラの待ちを解く
	m_bQuit	= FALSE;
	SboPlatform::UninstallStopSignalHandler();

	return SBOSVR_EXIT_OK;
}

int CMainFrame::MainLoopWindow(HINSTANCE hInstance)
{
	TCHAR szBuf[256];
	MSG msg;
	TIMECAPS tc;
	WNDCLASS wc;

	wc.hInstance	= hInstance;
	wc.lpszClassName	= _T(CLNAME);
	wc.lpfnWndProc	= (WNDPROC)WndProcEntry;
	wc.style	= 0;
	wc.hIcon	= NULL;//LoadIcon (hInstance, (char *)IDI_MAINFRAME);
	wc.hCursor	= LoadCursor((HINSTANCE)NULL, IDC_ARROW);
	wc.lpszMenuName	= _T("IDR_MENU");
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);

	// ウィンドウのクラスを登録
	if (!RegisterClass(&wc)) {
		return FALSE;
	}

	// ウィンドウ作成
	wsprintf(szBuf, _T("%s Ver%s"), _T(WNDTITLE), _T(VERTEXT));
	m_hWnd = CreateWindow(
				_T(CLNAME),
				szBuf,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT,
				(int)(strlen(WNDTITLE) * 12), 150,
				NULL,
				NULL,
				hInstance,
				this);
	if (m_hWnd == NULL) {
		return FALSE;
	}

	ShowWindow(m_hWnd, SW_SHOW);

	timeGetDevCaps(&tc, sizeof (TIMECAPS));
	// マルチメディアタイマーのサービス精度を最大に
	timeBeginPeriod(tc.wPeriodMin);

	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		} else {
			TimerProc();
		}
	}

	timeEndPeriod(tc.wPeriodMin);
	UnregisterClass(_T(CLNAME), hInstance);

	// 終了メッセージによりプログラム終了
	return (int)msg.wParam;
}

void CMainFrame::SendToScreenChar(
	CInfoCharBase *pInfoChar,	// [in] 送信基準のキャラ情報
	CPacketBase *pPacket,	// [in] 送信パケット
	BOOL bExcludeSelf)	// [in] TRUE の場合、送信元自身をスキップする
{
	int i, nCount;
	PCInfoCharSvr pInfoCharTmp;

	nCount = m_pLibInfoChar->GetCountLogIn();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(i);
		if (bExcludeSelf && (pInfoCharTmp == pInfoChar)) {
			continue;
		}
		if (pInfoCharTmp->IsLogin() == FALSE) {
			continue;
		}
		if (pInfoCharTmp->m_dwMapID != pInfoChar->m_dwMapID) {
			continue;
		}
		// 画面外？
		if (!(
			(abs(pInfoCharTmp->m_nMapX - pInfoChar->m_nMapX) < DRAW_PARTS_X * MAPPARTSSIZE + MAPPARTSSIZE) &&
			(abs(pInfoCharTmp->m_nMapY - pInfoChar->m_nMapY) < DRAW_PARTS_Y * MAPPARTSSIZE + MAPPARTSSIZE))) {
#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
			switch (pPacket->m_byCmdSub) {
			case SBOCOMMANDID_SUB_CHAR_MOVE_START:
			case SBOCOMMANDID_SUB_CHAR_MOVE_DIR_CHANGE:
			case SBOCOMMANDID_SUB_CHAR_MOVE_STOP:
				m_pLog->Write(
					"[MOVE_SCREEN_SKIP] [PACKET:%d][SRC_CHARID:%u][DST_CHARID:%u][SRC_POS:%d,%d][DST_POS:%d,%d]",
					pPacket->m_byCmdSub,
					pInfoChar->m_dwCharID,
					pInfoCharTmp->m_dwCharID,
					pInfoChar->m_nMapX,
					pInfoChar->m_nMapY,
					pInfoCharTmp->m_nMapX,
					pInfoCharTmp->m_nMapY);
				break;
			}
#endif
			continue;
		}
		if (pInfoCharTmp->m_dwSessionID) {
#if SBO_ENABLE_POS_SYNC_DEBUG_LOG
			switch (pPacket->m_byCmdSub) {
			case SBOCOMMANDID_SUB_CHAR_MOVE_START:
			case SBOCOMMANDID_SUB_CHAR_MOVE_DIR_CHANGE:
			case SBOCOMMANDID_SUB_CHAR_MOVE_STOP:
				m_pLog->Write(
					"[MOVE_SCREEN_SEND] [PACKET:%d][SRC_CHARID:%u][DST_CHARID:%u][DST_SESSION:%u][SRC_POS:%d,%d][DST_POS:%d,%d]",
					pPacket->m_byCmdSub,
					pInfoChar->m_dwCharID,
					pInfoCharTmp->m_dwCharID,
					pInfoCharTmp->m_dwSessionID,
					pInfoChar->m_nMapX,
					pInfoChar->m_nMapY,
					pInfoCharTmp->m_nMapX,
					pInfoCharTmp->m_nMapY);
				break;
			}
#endif
			m_pSock->SendTo(pInfoCharTmp->m_dwSessionID, pPacket);
		}
	}
}

void CMainFrame::SendToMapChar(
	DWORD dwMapID,	// [in] マップID(0で全てのマップ)
	CPacketBase *pPacket)	// [in] 送信パケット
{
	int i, nCount;
	PCInfoCharSvr pInfoCharTmp;

	nCount = m_pLibInfoChar->GetCountLogIn();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(i);
		if (pInfoCharTmp->IsLogin() == FALSE) {
			continue;
		}
		if (dwMapID != 0) {
			if (pInfoCharTmp->m_dwMapID != dwMapID) {
				continue;
			}
		}
		if (pInfoCharTmp->m_dwSessionID) {
			m_pSock->SendTo(pInfoCharTmp->m_dwSessionID, pPacket);
		}
	}
}

void CMainFrame::SendToAdminChar(
	CPacketBase *pPacket)	// [in] 送信パケット
{
	int i, nCount;
	PCInfoCharSvr pInfoCharTmp;
	PCInfoAccount pInfoAccount;

	nCount = m_pLibInfoChar->GetCountLogIn();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(i);
		if (pInfoCharTmp->IsLogin() == FALSE) {
			continue;
		}
		pInfoAccount = m_pLibInfoAccount->GetPtrSessionID(pInfoCharTmp->m_dwSessionID);
		if (pInfoAccount == NULL) {
			continue;
		}
		if (pInfoAccount->m_nAdminLevel <= ADMINLEVEL_NONE) {
			continue;
		}

		m_pSock->SendTo(pInfoAccount->m_dwSessionID, pPacket);
	}
}

void CMainFrame::SendToClient(
	DWORD dwSessionID,	// [in] セッションID
	CPacketBase *pPacket)	// [in] 送信パケット
{
	if (dwSessionID == 0) {
		return;
	}
	m_pSock->SendTo(dwSessionID, pPacket);
}

LRESULT CALLBACK CMainFrame::WndProcEntry(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CMainFrame* pThis;

	if (msg == WM_CREATE) {
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)(((LPCREATESTRUCT)lParam)->lpCreateParams));
	}

	// ユーザデータから this ポインタを取得し、処理を行う
	pThis = (CMainFrame *)GetWindowLong(hWnd, GWL_USERDATA);
	if (pThis) {
		return pThis->WndProc(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CMainFrame::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	HANDLE_MSG(hWnd, WM_CREATE,	OnCreate);
	HANDLE_MSG(hWnd, WM_CLOSE,	OnClose);
	HANDLE_MSG(hWnd, WM_DESTROY,	OnDestroy);
	HANDLE_MSG(hWnd, WM_PAINT,	OnPaint);
	HANDLE_MSG(hWnd, WM_COMMAND,	OnCommand);

	default:
		// 通信の通知は SetNotifySink() 経由でキューへ入るため、ここには来ない。
		// 修理しないメッセージはOSに返す
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

// 接続したコンソールへ出力する
//
// コンソールが無い場合は何も起きない。

void CMainFrame::WriteConsoleMessage(LPCTSTR pszFormat, ...)
{
	va_list args;
	TCHAR szBuf[1024];

	va_start(args, pszFormat);
	_vsntprintf_s(szBuf, _countof(szBuf), _TRUNCATE, pszFormat, args);
	va_end(args);

	_ftprintf(stdout, _T("%s\n"), szBuf);
	fflush(stdout);
}

// 稼働中のヘッドレスサーバーへ停止を要求する
//
// 同じ ini を使うインスタンス(= 同じポート)を対象にする。
// 停止通知を出したあと、相手が終了するまで待つ。
//
// 戻り値は稼働中のサーバーを止められたか。

BOOL CMainFrame::RequestStopRunningServer(void)
{
	WORD wPort;

	wPort = (WORD)SboPlatform::GetIniInt(SboPlatform::GetIniFilePath().c_str(), "Setting", "Port", 2006);

	SboPlatform::AttachParentConsole();

	if (SboPlatform::SendStopRequest(wPort) == false) {
		// 稼働中のヘッドレスサーバーが見つからない
		WriteConsoleMessage(_T("稼働中のヘッドレスサーバー(Port:%u)が見つかりません"), (unsigned int)wPort);
		return FALSE;
	}
	WriteConsoleMessage(_T("停止を要求しました (Port:%u)。終了を待っています"), (unsigned int)wPort);

	if (SboPlatform::WaitForServerExit(wPort, 30000) == true) {
		WriteConsoleMessage(_T("停止しました"));
	} else {
		WriteConsoleMessage(_T("30秒待っても終了しませんでした。停止要求は送られています"));
	}

	return TRUE;
}

// ウィンドウ位置を復元
//
// ヘッドレス時は呼ばない。位置を持たないため読み込む意味が無い。

void CMainFrame::LoadWindowPos(HWND hWnd)
{
	RECT rc;
	std::string strIni = SboPlatform::GetIniFilePath();
	const char *pszIni = strIni.c_str();

	rc.left	= SboPlatform::GetIniInt(pszIni, "Pos", "MainLeft",	-1);
	rc.top	= SboPlatform::GetIniInt(pszIni, "Pos", "MainTop",	-1);
	rc.right	= SboPlatform::GetIniInt(pszIni, "Pos", "MainRight",	-1);
	rc.bottom	= SboPlatform::GetIniInt(pszIni, "Pos", "MainBottom",	-1);
	if (!((rc.left == -1) && (rc.top == -1))) {
		SetWindowPos(hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER);
	}
}

// ウィンドウ位置を保存
//
// ヘッドレス時は呼ばない。位置が無いのに書き込むと、次に
// ウィンドウ付きで起動したときの位置を壊してしまう。

void CMainFrame::SaveWindowPos(HWND hWnd)
{
	RECT rc;
	char szValue[32];

	if ((IsIconic(hWnd) != FALSE) || (IsWindowVisible(hWnd) == FALSE)) {
		return;
	}

	std::string strIni = SboPlatform::GetIniFilePath();
	const char *pszIni = strIni.c_str();
	GetWindowRect(hWnd, &rc);

	_snprintf_s(szValue, sizeof (szValue), _TRUNCATE, "%d", (int)rc.left);
	SboPlatform::SetIniString(pszIni, "Pos", "MainLeft", szValue);
	_snprintf_s(szValue, sizeof (szValue), _TRUNCATE, "%d", (int)rc.top);
	SboPlatform::SetIniString(pszIni, "Pos", "MainTop", szValue);
	_snprintf_s(szValue, sizeof (szValue), _TRUNCATE, "%d", (int)rc.right);
	SboPlatform::SetIniString(pszIni, "Pos", "MainRight", szValue);
	_snprintf_s(szValue, sizeof (szValue), _TRUNCATE, "%d", (int)rc.bottom);
	SboPlatform::SetIniString(pszIni, "Pos", "MainBottom", szValue);
}

// サーバー初期化
//
// ウィンドウに依存しない。ヘッドレス時は m_hWnd が NULL のまま呼ばれる。

BOOL CMainFrame::InitServer(void)
{
        WORD wPort;
        WORD wHttpPort;
        TCHAR szName[MAX_PATH];
        TCHAR szTmp[MAX_PATH];
        LPTSTR pszTmp;

	sgenrand(SboPlatform::GetTickMs());

	ZeroMemory(szName, sizeof (szName));
	ZeroMemory(szTmp, sizeof (szTmp));

	// 作業用フォルダを作成
	SboPlatform::MakeDirectory(SboPlatform::MakeExeRelativePath("SBODATA").c_str());

	m_dwServerStartTime = SboPlatform::GetTickMs();

        m_pMgrData->Create(this, m_pSock);
        m_pMgrData->Load();

        if (m_pHttpServer) {
                m_pHttpServer->SetMgrData(m_pMgrData);
        }

	wPort = m_pMgrData->GetPort();
        wHttpPort = m_pMgrData->GetHttpPort();

	// ソケット通知をウィンドウメッセージではなくコールバックで受け取る。
	// Host() より前に登録しておく必要がある。
	// シンクを登録済みなら hWnd は NULL でよい(ヘッドレス時)。
	m_pSock->SetNotifySink(&CMainFrame::OnSocketNotifyThunk, this);
	m_pSock->Host(m_hWnd, URARASOCK_MSGBASE, URARASOCK_PRECHECK, wPort, 100);

	m_pLog	= m_pMgrData->GetLog();
	if (m_pHttpServer) {
		if (m_pHttpServer->Start(wHttpPort)) {
			if (m_pLog) {
				m_pLog->Write("HTTPサーバーを起動しました [Port:%u]", wHttpPort);
			}
		} else {
			if (m_pLog) {
				m_pLog->Write("HTTPサーバーの起動に失敗しました [Port:%u]", wHttpPort);
			}
		}
	}
	// WebSocketブリッジサーバーの起動
	// WebSocketポート = HTTPポート+1（例: 18080 → 18081）
	if (m_pWebSocketBridge) {
		unsigned short wWsPort = static_cast<unsigned short>(wHttpPort + 1);
		if (m_pWebSocketBridge->Start(wWsPort, wPort)) {
			if (m_pLog) {
				m_pLog->Write("WebSocketブリッジを起動しました [WsPort:%u → TcpPort:%u]", wWsPort, wPort);
			}
		} else {
			if (m_pLog) {
				m_pLog->Write("WebSocketブリッジの起動に失敗しました [WsPort:%u]", wWsPort);
			}
		}
	}
	m_pLibInfoAccount	= m_pMgrData->GetLibInfoAccount();
	m_pLibInfoMap	= m_pMgrData->GetLibInfoMap();
	m_pLibInfoMapObject	= m_pMgrData->GetLibInfoMapObject();
	m_pLibInfoMapParts	= m_pMgrData->GetLibInfoMapParts();
	m_pLibInfoMapShadow	= m_pMgrData->GetLibInfoMapShadow();
	m_pLibInfoChar	= m_pMgrData->GetLibInfoChar();
	m_pLibInfoDisable	= m_pMgrData->GetLibInfoDisable();
	m_pLibInfoItemType	= m_pMgrData->GetLibInfoItemType();
	m_pLibInfoItem	= m_pMgrData->GetLibInfoItem();
	m_pLibInfoItemWeapon	= m_pMgrData->GetLibInfoItemWeapon();
	m_pLibInfoEffect	= m_pMgrData->GetLibInfoEffect();
	m_pLibInfoMotion	= m_pMgrData->GetLibInfoMotion();
	m_pLibInfoMotionType	= m_pMgrData->GetLibInfoMotionType();
	m_pLibInfoEfcBalloon	= m_pMgrData->GetLibInfoEfcBalloon();
	m_pLibInfoSystem	= m_pMgrData->GetLibInfoSystem();
	m_pLibInfoSkill	= m_pMgrData->GetLibInfoSkill();
	m_pLibInfoTalkEvent	= m_pMgrData->GetLibInfoTalkEvent();

	m_dwLastClockTime	= SboPlatform::GetTickMs();
	m_dwLastSaveTime	= m_dwLastClockTime;

	return TRUE;
}

// サーバー終了処理
//
// ウィンドウに依存しない。DB の保存は呼び出し側で行う。

void CMainFrame::TermServer(void)
{
        if (m_pHttpServer) {
                m_pHttpServer->Stop();
                if (m_pLog) {
                        m_pLog->Write("HTTPサーバーを停止しました");
                }
        }
        if (m_pWebSocketBridge) {
                m_pWebSocketBridge->Stop();
                if (m_pLog) {
                        m_pLog->Write("WebSocketブリッジを停止しました");
                }
        }
	// 通知を止めてから破棄する。停止後に積まれた分は捨てる。
	m_pSock->SetNotifySink(NULL, NULL);
	m_pSock->Destroy();
	ClearSockNotify();
}

BOOL CMainFrame::OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	m_hWnd	= hWnd;

	LoadWindowPos(hWnd);

	return InitServer();
}

void CMainFrame::OnClose(HWND hWnd)
{
	SaveWindowPos(hWnd);
	TermServer();

	DestroyWindow(hWnd);
}

void CMainFrame::OnDestroy(HWND hWnd)
{
	m_pMgrData->Save();

	PostQuitMessage(0);
}

// サーバー状態の表示項目を取得
//
// GDI にもウィンドウにも依存しない。ヘッドレス化した際は
// この結果をコンソールやログへ出せばよい。
//
// 戻り値は格納した項目数。

int CMainFrame::GetServerStateItem(
	SERVERSTATEITEM *paItem,	// [out] 項目の格納先
	int nMax)	// [in] 格納先の要素数
{
	int nCount;
	DWORD dwTime;

	if ((paItem == NULL) || (nMax <= 0)) {
		return 0;
	}

	nCount	= 0;
	dwTime	= SboPlatform::GetTickMs() - m_dwServerStartTime;

	if (nCount < nMax) {
		paItem[nCount].strLabel	= _T("サーバー稼動時間");
		paItem[nCount].strValue.Format(_T("%04d:%02d:%02d"),
				 dwTime / 3600000,
				 (dwTime % 3600000 - ((dwTime % 60000) / 1000)) / 60000,
				 (dwTime % 60000) / 1000);
		nCount ++;
	}
	if (nCount < nMax) {
		paItem[nCount].strLabel	= _T("接続数");
		paItem[nCount].strValue.Format(_T("%d"), m_pLibInfoChar->GetCountOnline());
		nCount ++;
	}
	if (nCount < nMax) {
		paItem[nCount].strLabel	= _T("処理キャラ数");
		paItem[nCount].strValue.Format(_T("%d"), m_pLibInfoChar->GetCount());
		nCount ++;
	}
	if (nCount < nMax) {
		paItem[nCount].strLabel	= _T("処理マップ数");
		paItem[nCount].strValue.Format(_T("%d"), m_pLibInfoMap->GetCount());
		nCount ++;
	}

	return nCount;
}

void CMainFrame::OnPaint(HWND hWnd)
{
	int i, nCount;
	HFONT hFontOld;
	HDC hDC;
	PAINTSTRUCT ps;
	SERVERSTATEITEM aItem[SERVERSTATEITEM_MAX];

	nCount	= GetServerStateItem(aItem, _countof(aItem));

	hDC	= BeginPaint(hWnd, &ps);

	SetBkMode(hDC, TRANSPARENT);
	hFontOld = (HFONT)SelectObject(hDC, m_hFont);

	for (i = 0; i < nCount; i ++) {
		SetTextColor(hDC, RGB(0, 255, 0));
		MyTextOut(hDC, 0, 12 * i, aItem[i].strLabel);

		SetTextColor(hDC, RGB(255, 255, 255));
		MyTextOut(hDC, 120, 12 * i, aItem[i].strValue);
	}

	SelectObject(hDC, hFontOld);

	EndPaint(hWnd, &ps);
}

// 時間処理(時報と状態表示の更新)
//
// 従来は再描画タイマーの WM_TIMER で駆動していたが、
// ウィンドウに依存しないよう経過時間の判定に置き換えた。

void CMainFrame::TimerProcClock(void)
{
	DWORD dwNow;
	SboPlatform::LOCALTIME LocalTime;

	dwNow = SboPlatform::GetTickMs();
	if (dwNow - m_dwLastClockTime < TIMER_REDRAW) {
		return;
	}
	m_dwLastClockTime = dwNow;

	SboPlatform::GetLocalTime(&LocalTime);

	if (LocalTime.nMinute == 0) {
		if (LocalTime.nHour != m_pMgrData->GetLastSendClock()) {
			CmyString strTmp;
			CPacketMAP_SYSTEMMSG Packet;

			m_pMgrData->SetLastSendClock((BYTE)LocalTime.nHour);

			strTmp.Format(_T("SYSTEM:サーバーが%d時頃をお知らせします"), (BYTE)LocalTime.nHour);
			Packet.Make(strTmp);
			m_pSock->SendTo(0, &Packet);
		}
	}

	// 状態表示はウィンドウがある場合のみ更新する
	if (m_hWnd) {
		InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

// 時間処理(定期保存)
//
// 従来は保存タイマーの WM_TIMER で駆動していた。

void CMainFrame::TimerProcSave(void)
{
	DWORD dwNow;
	CPacketMAP_SYSTEMMSG Packet;
	CmyString strTmp;

	dwNow = SboPlatform::GetTickMs();
	if (dwNow - m_dwLastSaveTime < TIMER_SAVE) {
		return;
	}
	m_dwLastSaveTime = dwNow;

	m_pMgrData->Save();
	strTmp.Format(_T("SYSTEM:サーバー情報を保存しました"));
	Packet.Make(strTmp);
	m_pSock->SendTo(0, &Packet);
}

void CMainFrame::OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify)
{
	switch (id) {
	case IDM_UPDATE_RENEW:	// アップデートファイル更新
		OnCommandUPDATE_RENEW();
		break;
	}
}

void CMainFrame::OnAddClient(DWORD dwSessionID)
{
	// バージョンチェック
//	m_pSendCommand->SendCONNECT_VERCHECK (dwID, VERSIONVAL);
}

void CMainFrame::OnDecClient(DWORD dwSessionID)
{
	time_t timeTmp;
	DWORD dwLoginTime;
	PCInfoCharSvr pChar;
	PCInfoAccount pInfoAccount;
	CPacketCHAR_STATE Packet;

	dwLoginTime = 0;
	time(&timeTmp);

	pInfoAccount = m_pLibInfoAccount->GetPtrSessionID(dwSessionID);
	if (pInfoAccount) {
		pInfoAccount->m_dwSessionID	= 0;
		pInfoAccount->m_dwLastKeepalive	= 0;
	}

	pChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrSessionID(dwSessionID);
	if (pChar == NULL) {
		if (pInfoAccount) {
			m_pLog->Write("ログアウト dwSessionID:%u [%s]", dwSessionID, pInfoAccount->m_strAccount.GetUtf8Pointer());
		}
		return;
	}
	if (pInfoAccount && pChar) {
		dwLoginTime = (DWORD)timeTmp - pInfoAccount->m_dwTimeLastLogin;
		m_pLog->Write("ログアウト dwSessionID:%u [ACC:%s][CHAR:%s][TIME:%02d:%02d:%02d]",
				dwSessionID,
				pInfoAccount->m_strAccount.GetUtf8Pointer(),
				pChar->m_strCharName.GetUtf8Pointer(),
				dwLoginTime / 600,
				(dwLoginTime % 600) / 60,
				(dwLoginTime % 60));

		m_pLibInfoChar->LogOut(pChar->m_dwCharID);
	}
	NotifyOnlineCount();
}

void CMainFrame::OnRecv(PBYTE pData, DWORD dwSessionID)
{
	CPacketBase Packet;

	Packet.Set(pData);

	switch (Packet.m_byCmdMain) {
	case SBOCOMMANDID_MAIN_VERSION:	RecvProcVERSION(Packet.m_byCmdSub, pData, dwSessionID);	break;	// バージョン系
	case SBOCOMMANDID_MAIN_CONNECT:	RecvProcCONNECT(Packet.m_byCmdSub, pData, dwSessionID);	break;	// 接続系
	case SBOCOMMANDID_MAIN_ACCOUNT:	RecvProcACCOUNT(Packet.m_byCmdSub, pData, dwSessionID);	break;	// アカウント系
	case SBOCOMMANDID_MAIN_CHAR:	RecvProcCHAR(Packet.m_byCmdSub, pData, dwSessionID);	break;	// キャラ系
	case SBOCOMMANDID_MAIN_MAP:	RecvProcMAP(Packet.m_byCmdSub, pData, dwSessionID);	break;	// マップ系
	case SBOCOMMANDID_MAIN_ITEM:	RecvProcITEM(Packet.m_byCmdSub, pData, dwSessionID);	break;	// アイテム系
	case SBOCOMMANDID_MAIN_ADMIN:	RecvProcADMIN(Packet.m_byCmdSub, pData, dwSessionID);	break;	// 管理者系
	case SBOCOMMANDID_MAIN_MSGCMD:	RecvProcMSGCMD(Packet.m_byCmdSub, pData, dwSessionID);	break;	// メッセージコマンド系
	case SBOCOMMANDID_MAIN_EFFECT:	RecvProcEFFECT(Packet.m_byCmdSub, pData, dwSessionID);	break;	// エフェクト系
	case SBOCOMMANDID_MAIN_BATTLE:	RecvProcBATTLE(Packet.m_byCmdSub, pData, dwSessionID);	break;	// 戦闘系
	}

	if (m_pSock) {
		m_pSock->DeleteRecvData(pData);
	} else {
		SAFE_DELETE_ARRAY(pData);
	}
}

void CMainFrame::OnDisconnect(DWORD dwSessionID)
{
	m_pSock->DeleteClient(dwSessionID);
}

// ソケット通知コールバック
//
// 通信ライブラリの専用スレッドから呼ばれる。ここでゲームロジックを
// 呼んではならない。キューへ積むだけにして、実処理は ProcSockNotify()
// がメインスレッドで行う。

void CMainFrame::OnSocketNotifyThunk(void *pUserData, UINT uMsgOffset, WPARAM wParam, LPARAM lParam)
{
	CMainFrame *pThis;
	SOCKNOTIFYINFO Info;

	pThis = (CMainFrame *)pUserData;
	if (pThis == NULL) {
		return;
	}

	Info.pData	= NULL;
	Info.dwSessionID	= (DWORD)lParam;

	switch (uMsgOffset) {
	case WM_URARASOCK_ADDCLIENT:	// クライアントが接続した
		Info.Type	= SOCKNOTIFY_ADDCLIENT;
		break;

	case WM_URARASOCK_DECCLIENT:	// クライアントが切断した
		Info.Type	= SOCKNOTIFY_DECCLIENT;
		break;

	case WM_URARASOCK_RECV:	// 受信
		Info.Type	= SOCKNOTIFY_RECV;
		Info.pData	= (PBYTE)wParam;
		break;

	default:	// 待ち受け開始など、サーバーでは使わない通知
		return;
	}

	pThis->PushSockNotify(Info);
}

// ソケット通知をキューへ積む

void CMainFrame::PushSockNotify(const SOCKNOTIFYINFO &Info)
{
	m_SectSockNotify.Enter();
	m_deqSockNotify.push_back(Info);
	m_SectSockNotify.Leave();
}

// 溜まったソケット通知を捌く
//
// 捌いている最中に積まれた分は次回に回す。これは従来の PostMessage が
// 「次のメッセージポンプで処理される」挙動だったのと合わせるため。
// キューを直接舐めながら処理すると、切断要求などが同一周回で連鎖して
// 順序と再入の前提が変わってしまう。

void CMainFrame::ProcSockNotify(void)
{
	std::deque<SOCKNOTIFYINFO> deqProc;

	m_SectSockNotify.Enter();
	m_deqSockNotify.swap(deqProc);
	m_SectSockNotify.Leave();

	while (!deqProc.empty()) {
		SOCKNOTIFYINFO Info = deqProc.front();
		deqProc.pop_front();

		switch (Info.Type) {
		case SOCKNOTIFY_ADDCLIENT:
			OnAddClient(Info.dwSessionID);
			break;

		case SOCKNOTIFY_DECCLIENT:
			OnDecClient(Info.dwSessionID);
			break;

		case SOCKNOTIFY_RECV:
			// pData の解放は OnRecv() が行う
			OnRecv(Info.pData, Info.dwSessionID);
			break;

		case SOCKNOTIFY_DISCONNECT:
			OnDisconnect(Info.dwSessionID);
			break;
		}
	}
}

// ソケット通知キューを破棄
//
// 未処理の受信データが残っている場合があるので、m_pSock が生きている
// うちに呼ぶこと。

void CMainFrame::ClearSockNotify(void)
{
	std::deque<SOCKNOTIFYINFO> deqProc;

	m_SectSockNotify.Enter();
	m_deqSockNotify.swap(deqProc);
	m_SectSockNotify.Leave();

	while (!deqProc.empty()) {
		SOCKNOTIFYINFO Info = deqProc.front();
		deqProc.pop_front();

		if (Info.pData == NULL) {
			continue;
		}
		if (m_pSock) {
			m_pSock->DeleteRecvData(Info.pData);
		} else {
			SAFE_DELETE_ARRAY(Info.pData);
		}
	}
}

// 切断を予約する
//
// 即時に切断するとパケット処理の途中でセッションが消えるため、
// 従来は PostMessage(WM_DISCONNECT) で次のポンプに回していた。
// その挙動をキューで再現する。

void CMainFrame::RequestDisconnect(DWORD dwSessionID)
{
	SOCKNOTIFYINFO Info;

	Info.Type	= SOCKNOTIFY_DISCONNECT;
	Info.dwSessionID	= dwSessionID;
	Info.pData	= NULL;

	PushSockNotify(Info);
}

void CMainFrame::TimerProc(void)
{
	ProcSockNotify();

	m_pLibInfoChar->	Proc();
	m_pLibInfoDisable->	Proc();
	TimerProcKeepalive();
	TimerProcClock();
	TimerProcSave();

	MsgWaitForMultipleObjects(0, NULL, FALSE, 1, QS_ALLINPUT);
}

void CMainFrame::TimerProcKeepalive(void)
{
	int i, nCount;
	DWORD dwTmp, dwTimeTmp;
	time_t timeTmp;
	PCInfoCharSvr pInfoChar;
	PCInfoAccount pInfoAccount;

	time(&timeTmp);
	dwTimeTmp = (DWORD)timeTmp;

	if (dwTimeTmp - m_dwLastKeepaliveCheck < 10) {
		return;
	}
	m_dwLastKeepaliveCheck = dwTimeTmp;

	nCount = m_pLibInfoChar->GetCountLogIn();
	for (i = 0; i < nCount; i ++) {
		pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn(i);
		pInfoAccount = (PCInfoAccount)m_pLibInfoAccount->GetPtr(pInfoChar->m_dwAccountID);
		if (pInfoAccount == NULL) {
			continue;
		}
		dwTmp = dwTimeTmp - pInfoAccount->m_dwLastKeepalive;
		if (dwTmp < 60) {
			continue;
		}
		if (pInfoChar->m_dwSessionID == 0) {
			continue;
		}
		// 1分以上生存確認通知を受けていないので切断する
		RequestDisconnect(pInfoChar->m_dwSessionID);
		dwTmp = dwTimeTmp - pInfoAccount->m_dwTimeLastLogin;
		m_pLog->Write("生存確認タイムアウト dwSessionID:%u [ACC:%s][CHAR:%s][時間:%ds]",
				pInfoChar->m_dwSessionID,
				pInfoAccount->m_strAccount.GetUtf8Pointer(),
				pInfoChar->m_strCharName.GetUtf8Pointer(),
				dwTmp);
		pInfoAccount->m_dwLastKeepalive = dwTimeTmp;
	}
}

void CMainFrame::MyTextOut(HDC hDC, int x, int y, LPCTSTR pStr)
{
	if (pStr == NULL) {
		return;
	}

	int nLen = static_cast<int>(_tcslen(pStr));
	::TextOut(hDC, x, y, pStr, nLen);
}

// オンライン数を全クライアントへ通知する
//
// 以前は ServerInfo.csv の書き出しと FTP アップロードも兼ねていたが、
// 管理用 HTTP の /api/server が同じ情報をライブデータで返すようになったため
// そちらは廃止した。

void CMainFrame::NotifyOnlineCount(void)
{
	int nCount;
	CPacketMAP_ONLINE Packet;

	if (m_pLibInfoChar == NULL) {
		return;
	}

	nCount = m_pLibInfoChar->GetCountOnline();

	Packet.Make(nCount);
	m_pSock->SendTo(0, &Packet);
}

void CMainFrame::OnCommandUPDATE_RENEW(void)
{
	m_pMgrData->ReadHashList();
}
