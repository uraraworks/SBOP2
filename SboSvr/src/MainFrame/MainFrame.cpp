/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MainFrame.cpp												 */
/* 内容			:サーバーメインフレーム 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/04													 */
/* ========================================================================= */

#include "stdafx.h"
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
#include "UpdateServerInfo.h"
#include "MainFrame.h"


/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

#define CLNAME "SboSvr"								/* 登録クラス名 */
#define TIMERID_REDRAW		(100)					/* 再描画タイマー */
#define TIMER_REDRAW		(1000)					/* 再描画周期 */
#define TIMERID_SAVE		(101)					/* 保存タイマー */
#define TIMER_SAVE			(1000 * 60 * 30)		/* 保存周期(30分) */


/* ========================================================================= */
/* 関数名	:CMainFrame::CMainFrame											 */
/* 内容		:コンストラクタ													 */
/* 日付		:2005/04/24														 */
/* ========================================================================= */

CMainFrame::CMainFrame()
{
	m_dwServerStartTime		= 0;
	m_dwLastKeepaliveCheck	= 0;
	m_hWnd					= NULL;
	m_pLibInfoAccount		= NULL;
	m_pLibInfoChar			= NULL;
	m_pLibInfoDisable		= NULL;
	m_pLibInfoMap			= NULL;
	m_pLibInfoMapObject		= NULL;
	m_pLibInfoMapParts		= NULL;
	m_pLibInfoMapShadow		= NULL;
	m_pLibInfoItemType		= NULL;
	m_pLibInfoItem			= NULL;
	m_pLibInfoItemWeapon	= NULL;
	m_pLibInfoEffect		= NULL;
	m_pLibInfoMotion		= NULL;
	m_pLibInfoMotionType	= NULL;
	m_pLibInfoEfcBalloon	= NULL;
	m_pLibInfoSystem		= NULL;
	m_pLibInfoSkill			= NULL;
	m_pLibInfoTalkEvent		= NULL;
	m_pLog					= NULL;

	m_pSock				= new CUraraSockTCPSBO;
	m_pMgrData			= new CMgrData;
	m_pUpdateServerInfo	= new CUpdateServerInfo;

	m_hFont = CreateFont (12, 0, 0, 0, FW_NORMAL,
			FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "ＭＳ ゴシック");
}


/* ========================================================================= */
/* 関数名	:CMainFrame::~CMainFrame										 */
/* 内容		:デストラクタ													 */
/* 日付		:2005/04/24														 */
/* ========================================================================= */

CMainFrame::~CMainFrame()
{
	if (m_hFont) {
		DeleteObject (m_hFont);
		m_hFont = NULL;
	}
	SAFE_DELETE (m_pMgrData);
	SAFE_DELETE (m_pSock);
	SAFE_DELETE (m_pUpdateServerInfo);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::MainLoop											 */
/* 内容		:メインループ													 */
/* 日付		:2006/11/04														 */
/* ========================================================================= */

int CMainFrame::MainLoop(HINSTANCE hInstance)
{
	char szBuf[256];
	MSG msg;
	TIMECAPS tc;
	WNDCLASS wc;

	wc.hInstance		= hInstance;
	wc.lpszClassName	= CLNAME;
	wc.lpfnWndProc		= (WNDPROC)WndProcEntry;
	wc.style			= 0;
	wc.hIcon			= NULL;//LoadIcon (hInstance, (char *)IDI_MAINFRAME);
	wc.hCursor			= LoadCursor ((HINSTANCE)NULL, IDC_ARROW);
	wc.lpszMenuName		= "IDR_MENU";
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hbrBackground	= (HBRUSH)GetStockObject (BLACK_BRUSH);

	/* ウィンドウのクラスを登録 */
	if (!RegisterClass (&wc)) {
		return FALSE;
	}

	/* ウィンドウ作成 */
	wsprintf (szBuf, "%s Ver%s", WNDTITLE, VERTEXT);
	m_hWnd = CreateWindow (
				CLNAME,
				szBuf,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT,
				(int)(strlen (WNDTITLE) * 12), 150,
				NULL,
				NULL,
				hInstance,
				this);
	if (m_hWnd == NULL) {
		return FALSE;
	}

	ShowWindow (m_hWnd, SW_SHOW);

	timeGetDevCaps (&tc, sizeof (TIMECAPS));
	/* マルチメディアタイマーのサービス精度を最大に */
	timeBeginPeriod (tc.wPeriodMin);

	while (1) {
		if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage (&msg);
			DispatchMessage (&msg);

		} else {
			TimerProc ();
		}
	}

	timeEndPeriod (tc.wPeriodMin);
	UnregisterClass (CLNAME, hInstance);

	/* 終了メッセージによりプログラム終了 */
	return (int)msg.wParam;
}


/* ========================================================================= */
/* 関数名	:CMainFrame::SendToScreenChar									 */
/* 内容		:指定キャラと同じ画面のキャラへ送信								 */
/* 日付		:2007/01/21														 */
/* ========================================================================= */

void CMainFrame::SendToScreenChar(
	CInfoCharBase *pInfoChar,		/* [in] 送信基準のキャラ情報 */
	CPacketBase *pPacket)			/* [in] 送信パケット */
{
	int i, nCount;
	PCInfoCharSvr pInfoCharTmp;

	nCount = m_pLibInfoChar->GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (i);
		if (pInfoCharTmp->IsLogin () == FALSE) {
			continue;
		}
		if (pInfoCharTmp->m_dwMapID != pInfoChar->m_dwMapID) {
			continue;
		}
		/* 画面外？ */
		if (!(
			(abs (pInfoCharTmp->m_nMapX - pInfoChar->m_nMapX) < DRAW_PARTS_X * 2 + 2) &&
			(abs (pInfoCharTmp->m_nMapY - pInfoChar->m_nMapY) < DRAW_PARTS_Y * 2 + 2))) {
			continue;
		}
		if (pInfoCharTmp->m_dwSessionID) {
			m_pSock->SendTo (pInfoCharTmp->m_dwSessionID, pPacket);
		}
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::SendToMapChar										 */
/* 内容		:指定マップにいるキャラへ送信									 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

void CMainFrame::SendToMapChar(
	DWORD dwMapID,				/* [in] マップID(0で全てのマップ) */
	CPacketBase *pPacket)		/* [in] 送信パケット */
{
	int i, nCount;
	PCInfoCharSvr pInfoCharTmp;

	nCount = m_pLibInfoChar->GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (i);
		if (pInfoCharTmp->IsLogin () == FALSE) {
			continue;
		}
		if (dwMapID != 0) {
			if (pInfoCharTmp->m_dwMapID != dwMapID) {
				continue;
			}
		}
		if (pInfoCharTmp->m_dwSessionID) {
			m_pSock->SendTo (pInfoCharTmp->m_dwSessionID, pPacket);
		}
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::SendToAdminChar									 */
/* 内容		:管理者権限を持っているキャラへ送信								 */
/* 日付		:2007/08/19														 */
/* ========================================================================= */

void CMainFrame::SendToAdminChar(
	CPacketBase *pPacket)		/* [in] 送信パケット */
{
	int i, nCount;
	PCInfoCharSvr pInfoCharTmp;
	PCInfoAccount pInfoAccount;

	nCount = m_pLibInfoChar->GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (i);
		if (pInfoCharTmp->IsLogin () == FALSE) {
			continue;
		}
		pInfoAccount = m_pLibInfoAccount->GetPtrSessionID (pInfoCharTmp->m_dwSessionID);
		if (pInfoAccount == NULL) {
			continue;
		}
		if (pInfoAccount->m_nAdminLevel <= ADMINLEVEL_NONE) {
			continue;
		}

		m_pSock->SendTo (pInfoAccount->m_dwSessionID, pPacket);
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::SendToClient										 */
/* 内容		:セッションIDが有効な場合のみ送信								 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

void CMainFrame::SendToClient(
	DWORD dwSessionID,			/* [in] セッションID */
	CPacketBase *pPacket)		/* [in] 送信パケット */
{
	if (dwSessionID == 0) {
		return;
	}
	m_pSock->SendTo (dwSessionID, pPacket);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::WndProcEntry										 */
/* 内容		:メインウィンドウプロシージャ(エントリポイント)					 */
/* 日付		:2005/04/24														 */
/* ========================================================================= */

LRESULT CALLBACK CMainFrame::WndProcEntry(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CMainFrame* pThis;

	if (msg == WM_CREATE) {
		SetWindowLong (hWnd, GWL_USERDATA, (LONG)(((LPCREATESTRUCT)lParam)->lpCreateParams));
	}

	/* ユーザデータから this ポインタを取得し、処理を行う */
	pThis = (CMainFrame *)GetWindowLong (hWnd, GWL_USERDATA);
	if (pThis) {
		return pThis->WndProc (hWnd, msg, wParam, lParam);
	}
	return DefWindowProc (hWnd, msg, wParam, lParam);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::WndProc											 */
/* 内容		:メインウィンドウプロシージャ									 */
/* 日付		:2005/04/24														 */
/* ========================================================================= */

LRESULT CMainFrame::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	HANDLE_MSG (hWnd, WM_CREATE,	OnCreate);
	HANDLE_MSG (hWnd, WM_CLOSE,		OnClose);
	HANDLE_MSG (hWnd, WM_DESTROY,	OnDestroy);
	HANDLE_MSG (hWnd, WM_PAINT,		OnPaint);
	HANDLE_MSG (hWnd, WM_TIMER,		OnTimer);
	HANDLE_MSG (hWnd, WM_COMMAND,	OnCommand);

	case WM_DISCONNECT:					/* 切断 */
		OnDisconnect ((DWORD)lParam);
		break;

	default:
		if ((msg >= URARASOCK_MSGBASE) && (msg < URARASOCK_MSGBASE + WM_URARASOCK_MAX)) {
			switch (msg - URARASOCK_MSGBASE) {
			case WM_URARASOCK_HOST:				/* 待ち受け開始 */
				break;

			case WM_URARASOCK_ADDCLIENT:		/* クライアントが接続した */
				OnAddClient ((DWORD)lParam);
				break;

			case WM_URARASOCK_DECCLIENT:		/* クライアントが切断した */
				OnDecClient ((DWORD)lParam);
				break;

			case WM_URARASOCK_RECV:				/* 受信 */
				OnRecv ((PBYTE)wParam, (DWORD)lParam);
				break;
			}
		} else {
			/* 修理しないメッセージはOSに返す */
			return DefWindowProc (hWnd, msg, wParam, lParam);
		}
	}
	return 0;
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnCreate											 */
/* 内容		:メッセージハンドラ(WM_CREATE)									 */
/* 日付		:2005/04/24														 */
/* ========================================================================= */

BOOL CMainFrame::OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	WORD wPort;
	char szName[MAX_PATH], szTmp[MAX_PATH];
	LPSTR pszTmp;
	RECT rc;

	sgenrand (GetTickCount ());

	ZeroMemory (szName, sizeof (szName));
	GetModuleFileName (NULL, szName, MAX_PATH);
	strcpy (szName + strlen (szName) - 3, "ini");

	rc.left		= GetPrivateProfileInt ("Pos", "MainLeft",		-1, szName);
	rc.top		= GetPrivateProfileInt ("Pos", "MainTop",		-1, szName);
	rc.right	= GetPrivateProfileInt ("Pos", "MainRight",		-1, szName);
	rc.bottom	= GetPrivateProfileInt ("Pos", "MainBottom",	-1, szName);
	if (!((rc.left == -1) && (rc.top == -1))) {
		SetWindowPos (hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER);
	}

	/* 作業用フォルダを作成 */
	GetModuleFileName (NULL, szName, MAX_PATH);
	pszTmp		= strrchr (szName, '\\');
	pszTmp[1]	= 0;
	wsprintf (szTmp, "%sSBODATA", szName);
	CreateDirectory (szTmp, NULL);

	m_dwServerStartTime = timeGetTime ();

	m_pMgrData->Create (this, m_pSock);
	m_pMgrData->Load ();

	wPort = m_pMgrData->GetPort ();
	m_pSock->Host (hWnd, URARASOCK_MSGBASE, URARASOCK_PRECHECK, wPort, 100);

	m_pLog					= m_pMgrData->GetLog ();
	m_pLibInfoAccount		= m_pMgrData->GetLibInfoAccount ();
	m_pLibInfoMap			= m_pMgrData->GetLibInfoMap ();
	m_pLibInfoMapObject		= m_pMgrData->GetLibInfoMapObject ();
	m_pLibInfoMapParts		= m_pMgrData->GetLibInfoMapParts ();
	m_pLibInfoMapShadow		= m_pMgrData->GetLibInfoMapShadow ();
	m_pLibInfoChar			= m_pMgrData->GetLibInfoChar ();
	m_pLibInfoDisable		= m_pMgrData->GetLibInfoDisable ();
	m_pLibInfoItemType		= m_pMgrData->GetLibInfoItemType ();
	m_pLibInfoItem			= m_pMgrData->GetLibInfoItem ();
	m_pLibInfoItemWeapon	= m_pMgrData->GetLibInfoItemWeapon ();
	m_pLibInfoEffect		= m_pMgrData->GetLibInfoEffect ();
	m_pLibInfoMotion		= m_pMgrData->GetLibInfoMotion ();
	m_pLibInfoMotionType	= m_pMgrData->GetLibInfoMotionType ();
	m_pLibInfoEfcBalloon	= m_pMgrData->GetLibInfoEfcBalloon ();
	m_pLibInfoSystem		= m_pMgrData->GetLibInfoSystem ();
	m_pLibInfoSkill			= m_pMgrData->GetLibInfoSkill ();
	m_pLibInfoTalkEvent		= m_pMgrData->GetLibInfoTalkEvent ();

	UpdateServerInfo (FALSE);
	m_pUpdateServerInfo->Create (
			m_pMgrData->GetFtpAccount (),
			m_pMgrData->GetFtpPassword (),
			m_pMgrData->GetFtpServerAddr (),
			m_pMgrData->GetFtpUploadPath ());

	SetTimer (hWnd, TIMERID_REDRAW,	TIMER_REDRAW,	NULL);
	SetTimer (hWnd, TIMERID_SAVE,	TIMER_SAVE,		NULL);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnClose											 */
/* 内容		:メッセージハンドラ(WM_CLOSE)									 */
/* 日付		:2008/08/01														 */
/* ========================================================================= */

void CMainFrame::OnClose(HWND hWnd)
{
	RECT rc;
	char szFileName[MAX_PATH];
	CmyString strTmp;

	ZeroMemory (szFileName, sizeof (szFileName));

	GetModuleFileName (NULL, szFileName, MAX_PATH);
	strcpy (szFileName + strlen (szFileName) - 3, "ini");

	if ((IsIconic (hWnd) == FALSE) && (IsWindowVisible (hWnd))) {
		GetWindowRect (hWnd, &rc);

		/* メインウィンドウ */
		strTmp.Format ("%d", rc.left);
		WritePrivateProfileString ("Pos", "MainLeft", strTmp, szFileName);
		strTmp.Format ("%d", rc.top);
		WritePrivateProfileString ("Pos", "MainTop", strTmp, szFileName);
		strTmp.Format ("%d", rc.right);
		WritePrivateProfileString ("Pos", "MainRight", strTmp, szFileName);
		strTmp.Format ("%d", rc.bottom);
		WritePrivateProfileString ("Pos", "MainBottom", strTmp, szFileName);
	}
	m_pSock->Destroy ();

	DestroyWindow (hWnd);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnDestroy											 */
/* 内容		:メッセージハンドラ(WM_DESTROY)									 */
/* 日付		:2005/04/24														 */
/* ========================================================================= */

void CMainFrame::OnDestroy(HWND hWnd)
{
	m_pMgrData->Save ();

	PostQuitMessage (0);
}


/* ========================================================================= */
/* 関数名：	CMainFrame::OnPaint												 */
/* 内容：	メッセージハンドラ(WM_PAINT)									 */
/* 日付：	2007/02/15														 */
/* ========================================================================= */

void CMainFrame::OnPaint(HWND hWnd)
{
	char szTmp[100];
	DWORD dwTime;
	HFONT hFontOld;
	HDC hDC;
	PAINTSTRUCT ps;

	hDC		= BeginPaint (hWnd, &ps);
	dwTime	= timeGetTime () - m_dwServerStartTime;

	SetBkMode (hDC, TRANSPARENT);
	hFontOld = (HFONT)SelectObject (hDC, m_hFont);

	SetTextColor (hDC, RGB(0, 255, 0));
	MyTextOut (hDC, 0, 0, 		"サーバー稼動時間");
	MyTextOut (hDC, 0, 12 * 1, 	"接続数");
	MyTextOut (hDC, 0, 12 * 2, 	"処理キャラ数");
	MyTextOut (hDC, 0, 12 * 3, 	"処理マップ数");

	SetTextColor (hDC, RGB(255, 255, 255));

	wsprintf (szTmp, "%04d:%02d:%02d",
			 dwTime / 3600000,
			 (dwTime % 3600000 - ((dwTime % 60000) / 1000)) / 60000,
			 (dwTime % 60000) / 1000);
	MyTextOut (hDC, 120, 0, szTmp);

	wsprintf (szTmp, "%d", m_pLibInfoChar->GetCountOnline ());
	MyTextOut (hDC, 120, 12 * 1, szTmp);

	wsprintf (szTmp, "%d", m_pLibInfoChar->GetCount ());
	MyTextOut (hDC, 120, 12 * 2, szTmp);

	wsprintf (szTmp, "%d", m_pLibInfoMap->GetCount ());
	MyTextOut (hDC, 120, 12 * 3, szTmp);

	SelectObject (hDC, hFontOld);

	EndPaint (hWnd, &ps);
}


/* ========================================================================= */
/* 関数名：	CMainFrame::OnTimer												 */
/* 内容：	メッセージハンドラ(WM_TIMER)									 */
/* 日付：	2007/02/15														 */
/* ========================================================================= */

void CMainFrame::OnTimer(HWND hWnd, UINT id)
{
	switch (id) {
	case TIMERID_REDRAW:			/* 再描画タイマー */
		{
			SYSTEMTIME sysTime;
			GetLocalTime (&sysTime);

			if (sysTime.wMinute == 0) {
				if (sysTime.wHour != m_pMgrData->GetLastSendClock ()) {
					CmyString strTmp;
					CPacketMAP_SYSTEMMSG Packet;

					m_pMgrData->SetLastSendClock ((BYTE)sysTime.wHour);

					strTmp.Format ("SYSTEM:サーバーが%d時頃をお知らせします", (BYTE)sysTime.wHour);
					Packet.Make (strTmp);
					m_pSock->SendTo (0, &Packet);
				}
			}
			InvalidateRect (hWnd, NULL, TRUE);
		}
		break;

	case TIMERID_SAVE:				/* 保存タイマー */
		{
			CPacketMAP_SYSTEMMSG Packet;
			CmyString strTmp;

			m_pMgrData->Save ();
			strTmp.Format ("SYSTEM:サーバー情報を保存しました");
			Packet.Make (strTmp);
			m_pSock->SendTo (0, &Packet);
			UpdateServerInfo (FALSE, TRUE);
		}
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnCommand											 */
/* 内容		:メッセージハンドラ(WM_COMMAND)									 */
/* 日付		:2007/10/20														 */
/* ========================================================================= */

void CMainFrame::OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify)
{
	switch (id) {
	case IDM_ACCOUNT_DELETEALL:		/* 全アカウントを削除 */
		OnCommandACCOUNT_DELETEALL ();
		break;
	case IDM_UPDATE_RENEW:			/* アップデートファイル更新 */
		OnCommandUPDATE_RENEW ();
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnAddClient										 */
/* 内容		:メッセージハンドラ(WM_URARASOCK_ADDCLIENT)						 */
/* 日付		:2005/06/08														 */
/* ========================================================================= */

void CMainFrame::OnAddClient(DWORD dwSessionID)
{
	/* バージョンチェック */
//	m_pSendCommand->SendCONNECT_VERCHECK (dwID, VERSIONVAL);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnDecClient										 */
/* 内容		:メッセージハンドラ(WM_URARASOCK_DECCLIENT)						 */
/* 日付		:2005/08/14														 */
/* ========================================================================= */

void CMainFrame::OnDecClient(DWORD dwSessionID)
{
	time_t timeTmp;
	DWORD dwLoginTime;
	PCInfoCharSvr pChar;
	PCInfoAccount pInfoAccount;
	CPacketCHAR_STATE Packet;

	dwLoginTime = 0;
	time (&timeTmp);

	pInfoAccount = m_pLibInfoAccount->GetPtrSessionID (dwSessionID);
	if (pInfoAccount) {
		pInfoAccount->m_dwSessionID		= 0;
		pInfoAccount->m_dwLastKeepalive	= 0;
	}

	pChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrSessionID (dwSessionID);
	if (pChar == NULL) {
		if (pInfoAccount) {
			m_pLog->Write ("ログアウト dwSessionID:%u [%s]", dwSessionID, (LPCSTR)pInfoAccount->m_strAccount);
		}
		return;
	}
	if (pInfoAccount && pChar) {
		dwLoginTime = (DWORD)timeTmp - pInfoAccount->m_dwTimeLastLogin;
		m_pLog->Write ("ログアウト dwSessionID:%u [ACC:%s][CHAR:%s][TIME:%02d:%02d:%02d]",
				dwSessionID,
				(LPCSTR)pInfoAccount->m_strAccount,
				(LPCSTR)pChar->m_strCharName,
				dwLoginTime / 600,
				(dwLoginTime % 600) / 60,
				(dwLoginTime % 60));

		m_pLibInfoChar->LogOut (pChar->m_dwCharID);
	}
	UpdateServerInfo ();
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnRecv												 */
/* 内容		:メッセージハンドラ(WM_URARASOCK_RECV)							 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CMainFrame::OnRecv(PBYTE pData, DWORD dwSessionID)
{
	CPacketBase Packet;

	Packet.Set (pData);

	switch (Packet.m_byCmdMain) {
	case SBOCOMMANDID_MAIN_VERSION:	RecvProcVERSION	(Packet.m_byCmdSub, pData, dwSessionID);	break;		/* バージョン系 */
	case SBOCOMMANDID_MAIN_CONNECT:	RecvProcCONNECT	(Packet.m_byCmdSub, pData, dwSessionID);	break;		/* 接続系 */
	case SBOCOMMANDID_MAIN_ACCOUNT:	RecvProcACCOUNT	(Packet.m_byCmdSub, pData, dwSessionID);	break;		/* アカウント系 */
	case SBOCOMMANDID_MAIN_CHAR:	RecvProcCHAR	(Packet.m_byCmdSub, pData, dwSessionID);	break;		/* キャラ系 */
	case SBOCOMMANDID_MAIN_MAP:		RecvProcMAP		(Packet.m_byCmdSub, pData, dwSessionID);	break;		/* マップ系 */
	case SBOCOMMANDID_MAIN_ITEM:	RecvProcITEM	(Packet.m_byCmdSub, pData, dwSessionID);	break;		/* アイテム系 */
	case SBOCOMMANDID_MAIN_ADMIN:	RecvProcADMIN	(Packet.m_byCmdSub, pData, dwSessionID);	break;		/* 管理者系 */
	case SBOCOMMANDID_MAIN_MSGCMD:	RecvProcMSGCMD	(Packet.m_byCmdSub, pData, dwSessionID);	break;		/* メッセージコマンド系 */
	case SBOCOMMANDID_MAIN_EFFECT:	RecvProcEFFECT	(Packet.m_byCmdSub, pData, dwSessionID);	break;		/* エフェクト系 */
	case SBOCOMMANDID_MAIN_BATTLE:	RecvProcBATTLE	(Packet.m_byCmdSub, pData, dwSessionID);	break;		/* 戦闘系 */
	}

	SAFE_DELETE_ARRAY (pData);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnDisconnect										 */
/* 内容		:メッセージハンドラ(WM_DISCONNECT)								 */
/* 日付		:2008/01/05														 */
/* ========================================================================= */

void CMainFrame::OnDisconnect(DWORD dwSessionID)
{
	m_pSock->DeleteClient (dwSessionID);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::TimerProc											 */
/* 内容		:時間処理														 */
/* 日付		:2005/04/24														 */
/* ========================================================================= */

void CMainFrame::TimerProc(void)
{
	m_pLibInfoChar->	Proc ();
	m_pLibInfoDisable->	Proc ();
	TimerProcKeepalive ();

	MsgWaitForMultipleObjects (0, NULL, FALSE, 1, QS_ALLINPUT);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::TimerProcKeepalive									 */
/* 内容		:時間処理(生存確認チェック)										 */
/* 日付		:2008/06/21														 */
/* ========================================================================= */

void CMainFrame::TimerProcKeepalive(void)
{
	int i, nCount;
	DWORD dwTmp, dwTimeTmp;
	time_t timeTmp;
	PCInfoCharSvr pInfoChar;
	PCInfoAccount pInfoAccount;

	time (&timeTmp);
	dwTimeTmp = (DWORD)timeTmp;

	if (dwTimeTmp - m_dwLastKeepaliveCheck < 10) {
		return;
	}
	m_dwLastKeepaliveCheck = dwTimeTmp;

	nCount = m_pLibInfoChar->GetCountLogIn ();
	for (i = 0; i < nCount; i ++) {
		pInfoChar = (PCInfoCharSvr)m_pLibInfoChar->GetPtrLogIn (i);
		pInfoAccount = (PCInfoAccount)m_pLibInfoAccount->GetPtr (pInfoChar->m_dwAccountID);
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
		/* 1分以上生存確認通知を受けていないので切断する */
		PostMessage (m_hWnd, WM_DISCONNECT, 0, pInfoChar->m_dwSessionID);
		dwTmp = dwTimeTmp - pInfoAccount->m_dwTimeLastLogin;
		m_pLog->Write ("生存確認タイムアウト dwSessionID:%u [ACC:%s][CHAR:%s][時間:%ds]",
				pInfoChar->m_dwSessionID,
				(LPCSTR)pInfoAccount->m_strAccount,
				(LPCSTR)pInfoChar->m_strCharName,
				dwTmp);
		pInfoAccount->m_dwLastKeepalive = dwTimeTmp;
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::MyTextOut											 */
/* 内容		:文字列表示														 */
/* 日付		:2003/05/17														 */
/* ========================================================================= */

void CMainFrame::MyTextOut(HDC hDC, int x, int y, LPCSTR pStr)
{
	TextOut (hDC, x, y, pStr, strlen (pStr));
}


/* ========================================================================= */
/* 関数名	:CMainFrame::UpdateServerInfo									 */
/* 内容		:サーバー情報を更新												 */
/* 日付		:2008/05/23														 */
/* ========================================================================= */

void CMainFrame::UpdateServerInfo(
	BOOL bSend/*=TRUE*/,		/* [in] TRUE:オンライン数を通知する */
	BOOL bUpload/*=FALSE*/)		/* [in] TRUE:すぐにアップロードする */
{
	int nCount;
	CmyString strTmp;
	CPacketMAP_ONLINE Packet;

	nCount = m_pLibInfoChar->GetCountOnline ();
	m_pLibInfoChar->GetPlaceName (strTmp);
	m_pUpdateServerInfo->Update (nCount, strTmp, bUpload);

	if (bSend) {
		/* オンライン数を通知 */
		Packet.Make (nCount);
		m_pSock->SendTo (0, &Packet);
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnCommandACCOUNT_DELETEALL							 */
/* 内容		:メニューハンドラ(全アカウントを削除)							 */
/* 日付		:2007/10/20														 */
/* ========================================================================= */

void CMainFrame::OnCommandACCOUNT_DELETEALL(void)
{
	int i, j, nCount, nCount2, nResult;
	PCInfoAccount pInfoAccount;

	nResult = MessageBox (m_hWnd, "全てのアカウントとキャラを削除しますか？", "確認", MB_YESNO | MB_ICONWARNING);
	if (nResult != IDYES) {
		return;
	}

	nCount = m_pLibInfoAccount->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoAccount = (PCInfoAccount)m_pLibInfoAccount->GetPtr (i);
		nCount2 = pInfoAccount->m_adwCharID.GetSize ();
		for (j = 0; j < nCount2; j ++) {
			m_pLibInfoChar->Delete (pInfoAccount->m_adwCharID[j]);
		}
	}
	m_pLibInfoAccount->DeleteAll ();
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnCommandUPDATE_RENEW								 */
/* 内容		:メニューハンドラ(アップデートファイル更新)						 */
/* 日付		:2008/06/07														 */
/* ========================================================================= */

void CMainFrame::OnCommandUPDATE_RENEW(void)
{
	m_pMgrData->ReadHashList ();
}

/* Copyright(C)URARA-works 2006 */
