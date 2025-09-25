/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:MainFrame.cpp												 */
/* 内容			:クライアントメインフレーム 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/09/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include <comdef.h>
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
#include "WindowLOGIN.h"
#include "LayerMap.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "MgrWindow.h"
#include "MgrKeyInput.h"
#include "MgrSound.h"
#include "MainFrame.h"


/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

#define CLNAME "SboCli"								/* 登録クラス名 */
#define TIMERID_TOOLCHECK	100						/* ツールチェックタイマー */
#define TIMER_TOOLCHECK		1000					/* ツールチェックタイマー周期 */
#define TIMERID_ACTIVECHECK	101						/* アクティブウィンドウチェックタイマー */
#define TIMER_ACTIVECHECK	1000					/* アクティブウィンドウチェックタイマー周期 */

/* メッセージコマンド種別 */
enum {
	MSGCMDTYPE_NONE = 0,
	MSGCMDTYPE_CHGFACE,								/* 表情変更 */
	MSGCMDTYPE_CHGHAIR,								/* 髪変更 */
	MSGCMDTYPE_CHGCLOTH,							/* 服装変更 */
	MSGCMDTYPE_CHGACCE,								/* アクセサリ変更 */
	MSGCMDTYPE_BGMVOLUME,							/* BGM音量設定 */
	MSGCMDTYPE_CHGCOLOR,							/* 色変更 */
	MSGCMDTYPE_SEVOLUME,							/* 効果音量設定 */
	MSGCMDTYPE_CHGARMS,								/* 持ち物変更 */
	MSGCMDTYPE_CHGSHIELD,							/* 盾変更 */
	MSGCMDTYPE_SETITEM,								/* 配置アイテム変更 */
	MSGCMDTYPE_MAKEITEM,							/* アイテム作成 */
	MSGCMDTYPE_BALLOON,								/* 噴出し */
	MSGCMDTYPE_DICE,								/* サイコロ */
	MSGCMDTYPE_RND,									/* ランダム */
	MSGCMDTYPE_NOW,									/* 現在時刻 */
	MSGCMDTYPE_EFFECT,								/* エフェクト */
	MSGCMDTYPE_WHERE,								/* 最も集まっている場所 */
	MSGCMDTYPE_MAX
};


/* ========================================================================= */
/* 関数名	:CMainFrame::CMainFrame											 */
/* 内容		:コンストラクタ													 */
/* 日付		:2005/05/04														 */
/* ========================================================================= */

CMainFrame::CMainFrame()
{
	m_hWnd				= NULL;
	m_bWindowActive		= TRUE;
	m_bRenewCharInfo	= FALSE;
	m_nGameState		= GAMESTATE_NONE;
	m_nDrawCount		= 30;
	m_nFPS				= 0;
	m_dwLastTimeCheck	= 0;
	m_dwDrawTime		= 0;

	m_pStateProc	= NULL;
	m_pMgrData		= new CMgrData;
	m_pSock			= new CUraraSockTCPSBO;
	m_pMgrGrpData	= new CMgrGrpData;

	m_pMgrDraw				= NULL;
	m_pMgrLayer				= NULL;
	m_pMgrWindow			= NULL;
	m_pMgrKeyInput			= NULL;
	m_pMgrSound				= NULL;
	m_pLibInfoChar			= NULL;
	m_pLibInfoDisable		= NULL;
	m_pLibInfoMap			= NULL;
	m_pLibInfoMapObject		= NULL;
	m_pLibInfoMapParts		= NULL;
	m_pLibInfoMapShadow		= NULL;
	m_pLibInfoItemType		= NULL;
	m_pLibInfoItem			= NULL;
	m_pLibInfoItemWeapon	= NULL;
	m_pLibInfoMotion		= NULL;
	m_pLibInfoMotionType	= NULL;
	m_pLibInfoEfcBalloon	= NULL;
	m_pLibInfoEffect		= NULL;
	m_pLibInfoSystem		= NULL;
	m_pLibInfoSkill			= NULL;

	m_hCom = CoInitializeEx (0, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	ZeroMemory (&m_stSystemTime, sizeof (m_stSystemTime));
}


/* ========================================================================= */
/* 関数名	:CMainFrame::~CMainFrame										 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

CMainFrame::~CMainFrame()
{
	SAFE_DELETE (m_pStateProc);
	SAFE_DELETE (m_pMgrGrpData);
	SAFE_DELETE (m_pSock);
	SAFE_DELETE (m_pMgrData);

	if (SUCCEEDED (m_hCom)) {
		CoUninitialize ();
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::MainLoop											 */
/* 内容		:メインループ													 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

int CMainFrame::MainLoop(HINSTANCE hInstance)
{
	char szBuf[256];
	BYTE byFps, byNowFrame, byFrameBack, byFrameTmp;
	DWORD dwStyle, dwTimeTmp, dwTimeFps, dwTimeStart;
	BOOL bResult, bDraw;
	RECT rcTmp;
	MSG msg;
	TIMECAPS tc;
	WNDCLASS wc;

	wc.hInstance		= hInstance;
	wc.lpszClassName	= CLNAME;
	wc.lpfnWndProc		= (WNDPROC)WndProcEntry;
	wc.style			= CS_DBLCLKS;
	wc.hIcon			= LoadIcon (hInstance, MAKEINTRESOURCE (IDI_SBO));
	wc.hCursor			= LoadCursor ((HINSTANCE)NULL, IDC_ARROW);
	wc.lpszMenuName		= NULL;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hbrBackground	= (HBRUSH)GetStockObject (BLACK_BRUSH);

	/* ウィンドウのクラスを登録 */
	if (!RegisterClass (&wc)) {
		return FALSE;
	}

	/* ウィンドウサイズを計算 */
	dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN | WS_MINIMIZEBOX;
	SetRect (&rcTmp, 0, 0, SCRSIZEX, SCRSIZEY);
	AdjustWindowRect (&rcTmp, dwStyle, FALSE);

	/* ウィンドウ作成 */
	wsprintf (szBuf, _T("%s Ver%s"), WNDTITLE, VERTEXT);
	m_hWnd = CreateWindow (
				CLNAME,
				szBuf,
				dwStyle,
				CW_USEDEFAULT, CW_USEDEFAULT,
				rcTmp.right - rcTmp.left, rcTmp.bottom - rcTmp.top,
				NULL,
				NULL,
				hInstance,
				this);
	if (m_hWnd == NULL) {
		return FALSE;
	}

	timeGetDevCaps (&tc, sizeof (TIMECAPS));
	/* マルチメディアタイマーのサービス精度を最大に */
	timeBeginPeriod (tc.wPeriodMin);

	byFps		= 0;
	m_nFPS		= 1000 / m_nDrawCount + 1;
	byFrameBack	= 0;
	dwTimeStart	= timeGetTime ();

	while (1) {
		if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			if (msg.message == WM_SYSKEYDOWN) {
				if ((msg.wParam == VK_MENU) || (msg.wParam == VK_F10)) {
					continue;
				}
			}
			TranslateMessage (&msg);
			DispatchMessage (&msg);

		} else {
			dwTimeTmp	= timeGetTime ();
			bResult		= TimerProc ();
			bDraw		|= bResult;
			/* 時間的に描画するだろうフレーム番号を求める */
			byNowFrame = (BYTE)((dwTimeTmp - dwTimeStart) / m_nFPS);
			if ((byFps <= m_nDrawCount) && (byFrameBack != byNowFrame)) {
				if (byFps && (byNowFrame - byFrameBack != 1)) {
					bDraw = FALSE;
				}
				byFrameTmp = (BYTE)(m_dwDrawTime / m_nFPS);
				if (byFrameTmp >= byNowFrame) {
					bDraw = FALSE;
				}
				KeyProc ();
				if (bDraw) {
					InvalidateRect (m_hWnd, NULL, FALSE);
					OnPaint (m_hWnd);
					byFps ++;
				}
				bDraw = FALSE;
				byFrameBack = byNowFrame;
			}

			if (dwTimeTmp > dwTimeStart + 1000) {
				/* FPSの更新 */
				dwTimeStart = dwTimeFps = timeGetTime ();
				m_dwDrawTime = 0;
				byFps = 0;
				bDraw = FALSE;
			}
		}
	}

	timeEndPeriod (tc.wPeriodMin);
	UnregisterClass (CLNAME, hInstance);

	/* 終了メッセージによりプログラム終了 */
	return (int)msg.wParam;
}


/* ========================================================================= */
/* 関数名	:CMainFrame::DisConnectProc										 */
/* 内容		:切断処理														 */
/* 日付		:2005/06/12														 */
/* ========================================================================= */

void CMainFrame::DisConnectProc(int nID)
{
	PCWindowLOGIN pWindow;

	m_pSock->Destroy ();

	pWindow = (PCWindowLOGIN)m_pMgrWindow->GetWindow (WINDOWTYPE_LOGIN);
	if (pWindow == NULL) {
		return;
	}

	switch (nID) {
	case DISCONNECTID_CONNECT:		/* 接続失敗 */
		m_pMgrWindow->MakeWindowMSG ("サーバーに接続できませんでした", 3000);
		pWindow->Enable (TRUE);
		break;
	case DISCONNECTID_VERSION:		/* バージョン不一致 */
		m_pMgrWindow->MakeWindowMSG ("クライアントを最新にしてください", 3000);
		pWindow->Enable (TRUE);
		break;
	case DISCONNECTID_USER:			/* 満員 */
	case DISCONNECTID_DISABLE:		/* 禁止 */
		pWindow->Enable (TRUE);
		break;
	case DISCONNECTID_PASSWORD:		/* パスワード不一致 */
		m_pMgrWindow->MakeWindowMSG ("パスワードが間違っています", 3000);
		pWindow->Enable (TRUE);
		break;
	case DISCONNECTID_LOGIN:		/* ログイン済み */
		m_pMgrWindow->MakeWindowMSG ("ログイン済みです", 3000);
		pWindow->Enable (TRUE);
		break;
	case DISCONNECTID_MAC:			/* 作成済み */
		m_pMgrWindow->MakeWindowMSG ("このPCではアカウント作成済みです", 3000);
		pWindow->Enable (TRUE);
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::ChgMoveState										 */
/* 内容		:プレイヤー移動状態変更処理										 */
/* 日付		:2008/06/29														 */
/* ========================================================================= */

void CMainFrame::ChgMoveState(
	BOOL bChgBGM)		/* [in] TRUE:BGMを変える */
{
	BOOL bChgWait;
	DWORD dwMoveWait;
	PCLayerMap pLayerMap;
	PCInfoMapBase pInfoMap;
	PCInfoCharCli pPlayerChar;

	bChgWait = FALSE;
	pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);
	if (pLayerMap == NULL) {
		return;
	}
	pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (pPlayerChar == NULL) {
		return;
	}
	pInfoMap = m_pMgrData->GetMap ();

	dwMoveWait = pPlayerChar->GetMoveWait ();
	if (pPlayerChar->m_bChargeAtack) {
		bChgWait = TRUE;
		dwMoveWait *= 2;
	}

	switch (pPlayerChar->m_nMoveState) {
	case CHARMOVESTATE_MOVE:			/* 移動中 */
	case CHARMOVESTATE_BATTLEMOVE:		/* 戦闘移動中 */
	case CHARMOVESTATE_BATTLEATACK:		/* 戦闘攻撃中 */
		bChgWait = TRUE;
		break;
	case CHARMOVESTATE_STAND:			/* 立ち */
	case CHARMOVESTATE_SWOON:			/* 気絶 */
		if (bChgBGM) {
			m_pMgrSound->PlayBGM (pInfoMap->m_dwBGMID);
		}
		bChgWait = TRUE;
		break;
	case CHARMOVESTATE_BATTLE:			/* 戦闘中 */
		if (bChgBGM) {
			m_pMgrSound->PlaySound (SOUNDID_CHARI14_A);
			m_pMgrSound->PlayBGM (BGMID_DAICHI_S);
		}
		bChgWait = TRUE;
		break;
	}
	if (bChgWait) {
		pLayerMap->SetScrollWait (dwMoveWait);
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::RenewItemArea										 */
/* 内容		:画面内のアイテム情報を更新										 */
/* 日付		:2008/06/29														 */
/* ========================================================================= */

void CMainFrame::RenewItemArea(void)
{
	RECT rcTmp;
	PCLayerMap pLayerMap;
	PCInfoCharCli pPlayerChar;

	pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (pPlayerChar == NULL) {
		return;
	}
	pLayerMap = (PCLayerMap)m_pMgrLayer->Get (LAYERTYPE_MAP);
	if (pLayerMap == NULL) {
		return;
	}

	rcTmp.left	 = pLayerMap->m_nViewX - 2;
	rcTmp.right	 = pLayerMap->m_nViewX + (DRAW_PARTS_X * 2) + 2;
	rcTmp.top	 = pLayerMap->m_nViewY - 2;
	rcTmp.bottom = pLayerMap->m_nViewY + (DRAW_PARTS_Y * 2) + 2;
	m_pLibInfoItem->SetArea (pPlayerChar->m_dwMapID, &rcTmp);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::WndProcEntry										 */
/* 内容		:メインウィンドウプロシージャ(エントリポイント)					 */
/* 日付		:2006/09/24														 */
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
/* 関数名	:CMainFrame::SendChat											 */
/* 内容		:チャット発言													 */
/* 日付		:2008/07/25														 */
/* ========================================================================= */

void CMainFrame::SendChat(int nType, LPCSTR pszMsg, DWORD *pdwDst)
{
	int nTmp, nCmdType, nCount;
	PCInfoCharCli pPlayerChar;
	CParamUtil ParamUtil;

	pPlayerChar = m_pMgrData->GetPlayerChar ();

	nCmdType = MSGCMDTYPE_NONE;
	ParamUtil.Set (pszMsg);
	nCount = ParamUtil.GetCount ();
	if (nCount > 0) {
		nCmdType = GetMsgCmdType (pszMsg);
	}

	switch (nCmdType) {
	case MSGCMDTYPE_CHGFACE:			/* 表情変更 */
		{
			CPacketMSGCMD_CHGFACE Packet;

			Packet.Make (pPlayerChar->m_dwCharID, atoi (ParamUtil.GetParam (1)));
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_CHGHAIR:			/* 髪変更 */
		{
			CPacketMSGCMD_PARA1 Packet;

			Packet.Make (MSGCMDTYPE_CHGHAIR, pPlayerChar->m_dwCharID, atoi (ParamUtil.GetParam (1)));
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_CHGCLOTH:			/* 服装変更 */
		{
			CPacketMSGCMD_CHGCLOTH Packet;

			Packet.Make (pPlayerChar->m_dwCharID, atoi (ParamUtil.GetParam (1)));
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_CHGACCE:			/* アクセサリ変更 */
		{
			CPacketMSGCMD_CHGACCE Packet;

			Packet.Make (pPlayerChar->m_dwCharID, atoi (ParamUtil.GetParam (1)));
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_BGMVOLUME:			/* BGM音量設定 */
		nTmp = atoi (ParamUtil.GetParam (1));
		m_pMgrData->SetBGMVolume (nTmp);
		m_pMgrSound->SetBGMVolume (nTmp);
		break;
	case MSGCMDTYPE_CHGCOLOR:			/* 色変更 */
		{
			CPacketMSGCMD_CHGCOLOR Packet;

			Packet.Make (pPlayerChar->m_dwCharID, atoi (ParamUtil.GetParam (1)));
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_SEVOLUME:			/* 効果音量設定 */
		nTmp = atoi (ParamUtil.GetParam (1));
		m_pMgrData->SetSEVolume (nTmp);
		m_pMgrSound->SetSEVolume (nTmp);
		break;
	case MSGCMDTYPE_CHGARMS:			/* 持ち物変更 */
		{
			CPacketMSGCMD_CHGARMS Packet;

			Packet.Make (pPlayerChar->m_dwCharID, atoi (ParamUtil.GetParam (1)));
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_CHGSHIELD:			/* 盾変更 */
		{
			int nTmp;
			LPCSTR pszTmp;
			CPacketMSGCMD_PARA1 Packet;

			nTmp = 0;
			pszTmp = ParamUtil.GetParam (1);
			if (pszTmp) {
				nTmp = atoi (pszTmp);
			}
			Packet.Make (SBOCOMMANDID_SUB_MSGCMD_CHGSHIELD, pPlayerChar->m_dwCharID, nTmp);
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_SETITEM:			/* 配置アイテム変更 */
		m_pMgrData->SetSetItemID (atoi (ParamUtil.GetParam (1)));
		break;
	case MSGCMDTYPE_MAKEITEM:			/* アイテム作成 */
		{
			CPacketMSGCMD_MAKEITEM Packet;

			Packet.Make (pPlayerChar->m_dwCharID, atoi (ParamUtil.GetParam (1)));
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_BALLOON:			/* 噴出し */
		{
			LPCSTR pszTmp;
			CPacketMSGCMD_PARA1 Packet;

			pszTmp = ParamUtil.GetParam (0);
			Packet.Make (SBOCOMMANDID_SUB_MSGCMD_BALLOON, pPlayerChar->m_dwCharID, atoi (&pszTmp[1]));
			m_pSock->Send (&Packet);
			if (pdwDst) {
				*pdwDst = atoi (&pszTmp[1]);
			}
		}
		break;
	case MSGCMDTYPE_DICE:			/* サイコロ */
		{
			CPacketMSGCMD_PARA1 Packet;

			Packet.Make (SBOCOMMANDID_SUB_MSGCMD_DICE, pPlayerChar->m_dwCharID, 0);
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_RND:			/* ランダム */
		{
			int nTmp;
			LPCSTR pszTmp;
			CPacketMSGCMD_PARA1 Packet;

			nTmp = 0;
			pszTmp = ParamUtil.GetParam (1);
			if (pszTmp) {
				nTmp = atoi (pszTmp);
			}
			Packet.Make (SBOCOMMANDID_SUB_MSGCMD_RND, pPlayerChar->m_dwCharID, nTmp);
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_NOW:			/* 現在時刻 */
		{
			SYSTEMTIME sysTime;
			CPacketCHAR_REQ_CHAT Packet;
			CmyString strTmp;

			GetLocalTime (&sysTime);
			strTmp.Format(_T("%02d:%02d:%02d"), sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			Packet.Make (nType, pPlayerChar->m_dwCharID, (LPCSTR)strTmp);
			m_pSock->Send (&Packet);
		}
		break;
	case MSGCMDTYPE_EFFECT:			/* エフェクト */
		{
			LPCSTR pszTmp;
			CPacketMSGCMD_PARA1 Packet;

			pszTmp = ParamUtil.GetParam (0);
			Packet.Make (SBOCOMMANDID_SUB_MSGCMD_EFFECT, pPlayerChar->m_dwCharID, atoi (&pszTmp[1]));
			m_pSock->Send (&Packet);
			if (pdwDst) {
				*pdwDst = atoi (&pszTmp[1]);
			}
		}
		break;
	case MSGCMDTYPE_WHERE:			/* 最も集まっている場所 */
		{
			CPacketMSGCMD_PARA1 Packet;

			Packet.Make (SBOCOMMANDID_SUB_MSGCMD_WHERE, pPlayerChar->m_dwCharID, 0);
			m_pSock->Send (&Packet);
		}
		break;
	default:
		{
			CPacketCHAR_REQ_CHAT Packet;

			Packet.Make (nType, pPlayerChar->m_dwCharID, pszMsg);
			m_pSock->Send (&Packet);
		}
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::WndProc											 */
/* 内容		:メインウィンドウプロシージャ									 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

LRESULT CMainFrame::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	HANDLE_MSG (hWnd, WM_CREATE,		OnCreate);
	HANDLE_MSG (hWnd, WM_CLOSE,			OnClose);
	HANDLE_MSG (hWnd, WM_DESTROY,		OnDestroy);
	HANDLE_MSG (hWnd, WM_PAINT,			OnPaint);
	HANDLE_MSG (hWnd, WM_TIMER,			OnTimer);
	HANDLE_MSG (hWnd, WM_COMMAND,		OnCommand);
	HANDLE_MSG (hWnd, WM_ACTIVATE,		OnActivate);
	HANDLE_MSG (hWnd, WM_KEYUP,			OnKeyUp);
	HANDLE_MSG (hWnd, WM_LBUTTONDOWN,	OnLButtonDown);
	HANDLE_MSG (hWnd, WM_RBUTTONDOWN,	OnRButtonDown);
	HANDLE_MSG (hWnd, WM_RBUTTONDBLCLK,	OnRButtonDblClk);
	HANDLE_MSG (hWnd, WM_MOUSEMOVE,		OnMouseMove);

	case WM_INITEND:				/* 初期化完了 */
		OnInitEnd (hWnd);
		break;
	case WM_CTLCOLORSTATIC:
		{
			int nID = GetWindowLong ((HWND)lParam, GWL_ID);
			HDC hDC = (HDC)wParam;

			if (nID != IDC_SAVEPASSWORD) {
				return DefWindowProc (hWnd, msg, wParam, lParam);
			}
			SetBkMode (hDC, TRANSPARENT);
			SetTextColor (hDC, RGB(0, 0, 0));
		}
		return (LRESULT)GetStockObject(NULL_BRUSH);
	case WM_MAINFRAME:				/* メインフレームへの通知 */
		OnMainFrame (wParam, lParam);
		break;
	case WM_WINDOWMSG:				/* ウィンドウからの通知 */
		OnWindowMsg (wParam, lParam);
		break;
	case WM_ADMINMSG:				/* 管理者ウィンドウ関連の通知 */
		OnAdminMsg (wParam, lParam);
		break;
	case WM_MGRDRAW:				/* 描画管理からの通知 */
		OnMgrDraw (wParam, lParam);
		break;
	case WM_ERASEBKGND:
		break;

	default:
		if ((msg >= URARASOCK_MSGBASE) && (msg < URARASOCK_MSGBASE + WM_URARASOCK_MAX)) {
			switch (msg - URARASOCK_MSGBASE) {
			case WM_URARASOCK_CONNECT:		/* 接続 */
				OnConnect ();
				break;
			case WM_URARASOCK_DISCONNECT:	/* 切断 */
				OnDisConnect ();
				break;
			case WM_URARASOCK_RECV:			/* 受信 */
				OnRecv ((PBYTE)wParam);
				break;
			}
		} else {
			return DefWindowProc (hWnd, msg, wParam, lParam);
		}
	}
	return 0;
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnCreate											 */
/* 内容		:メッセージハンドラ(WM_CREATE)									 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

BOOL CMainFrame::OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
    TCHAR szFileName[MAX_PATH];
    POINT pt;

    ZeroMemory (szFileName, sizeof (szFileName));
    GetModuleFileName (NULL, szFileName, _countof (szFileName));
    size_t nNameLen = _tcslen (szFileName);
    if (nNameLen >= 3) {
            _tcscpy_s (szFileName + nNameLen - 3, _countof (szFileName) - (nNameLen - 3), _T("ini"));
    } else {
            _tcscat_s (szFileName, _T(".ini"));
    }

        pt.x = GetPrivateProfileInt (_T("Pos"), _T("MainX"), -1, szFileName);
        pt.y = GetPrivateProfileInt (_T("Pos"), _T("MainY"), -1, szFileName);
	if (!((pt.x == -1) && (pt.y == -1))) {
		SetWindowPos (hWnd, NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE);
	}

	SetTimer (hWnd, TIMERID_TOOLCHECK, TIMER_TOOLCHECK, NULL);
	SetTimer (hWnd, TIMERID_ACTIVECHECK, TIMER_ACTIVECHECK, NULL);
	PostMessage (hWnd, WM_INITEND, 0, 0);

	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnInitEnd											 */
/* 内容		:メッセージハンドラ(WM_INITEND)									 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CMainFrame::OnInitEnd(HWND hWnd)
{
	BOOL bRet, bResult;
	TCHAR szName[MAX_PATH], szTmp[MAX_PATH];
	LPTSTR pszTmp;
	GUID stGuid;

	bRet = FALSE;

	/* 各マネージャクラスを作成 */
	m_pMgrData->	SetWindowInfo (GetModuleHandle (NULL), hWnd);
	m_pMgrData->	Create (this, m_pMgrGrpData);
	m_pMgrData->	SetUraraSockTCP (m_pSock);

	m_pMgrDraw				= m_pMgrData->GetMgrDraw ();
	m_pMgrLayer				= m_pMgrData->GetMgrLayer ();
	m_pMgrWindow			= m_pMgrData->GetMgrWindow ();
	m_pMgrKeyInput			= m_pMgrData->GetMgrKeyInput ();
	m_pMgrSound				= m_pMgrData->GetMgrSound ();
	m_pLibInfoChar			= m_pMgrData->GetLibInfoChar ();
	m_pLibInfoDisable		= m_pMgrData->GetLibInfoDisable ();
	m_pLibInfoMap			= m_pMgrData->GetLibInfoMap ();
	m_pLibInfoMapObject		= m_pMgrData->GetLibInfoMapObject ();
	m_pLibInfoMapParts		= m_pMgrData->GetLibInfoMapParts ();
	m_pLibInfoMapShadow		= m_pMgrData->GetLibInfoMapShadow ();
	m_pLibInfoItemType		= m_pMgrData->GetLibInfoItemType ();
	m_pLibInfoItem			= m_pMgrData->GetLibInfoItem ();
	m_pLibInfoItemWeapon	= m_pMgrData->GetLibInfoItemWeapon ();
	m_pLibInfoMotion		= m_pMgrData->GetLibInfoMotion ();
	m_pLibInfoMotionType	= m_pMgrData->GetLibInfoMotionType ();
	m_pLibInfoEfcBalloon	= m_pMgrData->GetLibInfoEfcBalloon ();
	m_pLibInfoEffect		= m_pMgrData->GetLibInfoEffect ();
	m_pLibInfoSystem		= m_pMgrData->GetLibInfoSystem ();
	m_pLibInfoSkill			= m_pMgrData->GetLibInfoSkill ();

	/* グラフィックデータDLLの読み込み */
	bResult = m_pMgrGrpData->Load ();
        if (bResult == FALSE) {
                MessageBox (hWnd, _T("グラフィックデータDLLの読み込みに失敗しました"), _T("エラー"), MB_OK);
		goto Exit;
	}

	stGuid = m_pMgrData->GetInputGuid ();
	m_pMgrKeyInput->SetDevice (stGuid, hWnd);

	ZeroMemory (szName, sizeof (szName));
	GetModuleFileName (NULL, szName, _countof (szName));
	pszTmp = _tcsrchr (szName, _T('\\'));
	if (pszTmp) {
		pszTmp[1] = _T('\0');
	}
	_stprintf_s (szTmp, _T("%sss"), szName);
	CreateDirectory (szTmp, NULL);
	_stprintf_s (szTmp, _T("%sLog"), szName);
	CreateDirectory (szTmp, NULL);

	bResult = m_pMgrData->GetDisableLogin();
	if (bResult) {
		goto Exit;
	}
	ChgGameState (GAMESTATE_LOGO);
	ShowWindow (hWnd, SW_SHOW);

	PostMessage (hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWVIEWSET, 0);

	bRet = TRUE;
Exit:
	if (bRet == FALSE) {
		PostMessage (hWnd, WM_CLOSE, 0, 0);
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnClose											 */
/* 内容		:メッセージハンドラ(WM_CLOSE)									 */
/* 日付		:2008/06/09														 */
/* ========================================================================= */

void CMainFrame::OnClose(HWND hWnd)
{
        RECT rc;
        TCHAR szFileName[MAX_PATH];
        HWND hWndTmp;
        CmyString strTmp;

        ZeroMemory (szFileName, sizeof (szFileName));

        GetModuleFileName (NULL, szFileName, _countof (szFileName));
        size_t nNameLen = _tcslen (szFileName);
        if (nNameLen >= 3) {
                _tcscpy_s (szFileName + nNameLen - 3, _countof (szFileName) - (nNameLen - 3), _T("ini"));
        } else {
                _tcscat_s (szFileName, _T(".ini"));
        }

	if ((IsIconic (hWnd) == FALSE) && (IsWindowVisible (hWnd))) {
		GetWindowRect (hWnd, &rc);

		/* メインウィンドウ */
		strTmp.Format(_T("%d"), rc.left);
                WritePrivateProfileString (_T("Pos"), _T("MainX"), strTmp, szFileName);
                strTmp.Format(_T("%d"), rc.top);
                WritePrivateProfileString (_T("Pos"), _T("MainY"), strTmp, szFileName);

		if (m_nGameState == GAMESTATE_MAP) {
			/* ログウィンドウ */
			((CStateProcMAP *)m_pStateProc)->GetMsgLogRect (rc);
			strTmp.Format(_T("%d"), rc.left);
                        WritePrivateProfileString (_T("Pos"), _T("LogLeft"), strTmp, szFileName);
                        strTmp.Format(_T("%d"), rc.top);
                        WritePrivateProfileString (_T("Pos"), _T("LogTop"), strTmp, szFileName);
                        strTmp.Format(_T("%d"), rc.right);
                        WritePrivateProfileString (_T("Pos"), _T("LogRight"), strTmp, szFileName);
                        strTmp.Format(_T("%d"), rc.bottom);
                        WritePrivateProfileString (_T("Pos"), _T("LogBottom"), strTmp, szFileName);

			hWndTmp = m_pMgrData->GetAdminWindow ();
			if (hWndTmp) {
				/* 管理者ウィンドウ */
				GetWindowRect (hWndTmp, &rc);
				strTmp.Format(_T("%d"), rc.left);
                                WritePrivateProfileString (_T("Pos"), _T("AdminX"), strTmp, szFileName);
                                strTmp.Format(_T("%d"), rc.top);
                                WritePrivateProfileString (_T("Pos"), _T("AdminY"), strTmp, szFileName);
			}

			hWndTmp = m_pMgrData->GetDebugWindow ();
			if (hWndTmp) {
				/* デバッグウィンドウ */
				GetWindowRect (hWndTmp, &rc);
				strTmp.Format(_T("%d"), rc.left);
                                WritePrivateProfileString (_T("Pos"), _T("DebugX"), strTmp, szFileName);
                                strTmp.Format(_T("%d"), rc.top);
                                WritePrivateProfileString (_T("Pos"), _T("DebugY"), strTmp, szFileName);
			}
		}
	}
	m_pMgrData->SaveIniData ();
	m_pSock->Destroy ();

	DestroyWindow (hWnd);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnDestroy											 */
/* 内容		:メッセージハンドラ(WM_DESTROY)									 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CMainFrame::OnDestroy(HWND hWnd)
{
	PostQuitMessage (0);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnPaint											 */
/* 内容		:メッセージハンドラ(WM_PAINT)									 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CMainFrame::OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	DWORD dwTmp;
	HDC hDC;

	dwTmp	= timeGetTime ();
	hDC		= BeginPaint (hWnd, &ps);

	if (IsWindowVisible (m_hWnd)) {
		m_pMgrDraw->Draw (hDC);
	}

	dwTmp = timeGetTime () - dwTmp;
	m_pMgrData->SetDrawTime (dwTmp);
	m_dwDrawTime += dwTmp;

	EndPaint (hWnd, &ps);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnTimer											 */
/* 内容		:メッセージハンドラ(WM_TIMER)									 */
/* 日付		:2008/08/03														 */
/* ========================================================================= */

void CMainFrame::OnTimer(HWND hWnd, UINT id)
{
	switch (id) {
	case TIMERID_TOOLCHECK:		/* ツールチェックタイマー */
		{
			DWORD dwTmp, dwScond, dwTimeTmp;
			SYSTEMTIME sysTime;

			if (m_dwLastTimeCheck == 0) {
				GetLocalTime (&m_stSystemTime);
				m_dwLastTimeCheck = timeGetTime ();
				break;
			}
			GetLocalTime (&sysTime);
			dwTimeTmp = timeGetTime ();
			if (sysTime.wMinute != m_stSystemTime.wMinute) {
				m_dwLastTimeCheck = 0;
				break;
			}
			if (sysTime.wSecond == m_stSystemTime.wSecond) {
				break;
			}
			dwTmp	= (dwTimeTmp - m_dwLastTimeCheck) / 1000;
			dwScond	= abs (sysTime.wSecond - m_stSystemTime.wSecond);
			if (dwScond == dwTmp) {
				m_dwLastTimeCheck = 0;
				break;
			}
			if (dwTmp < dwScond * 3) {
				m_dwLastTimeCheck = 0;
				break;
			}
			PostMessage (hWnd, WM_CLOSE, 0, 0);
		}
		break;

	case TIMERID_ACTIVECHECK:	/* アクティブウィンドウチェックタイマー */
		{
			HWND hWnd;

			m_bWindowActive = FALSE;
			hWnd = GetFocus ();
			if (m_hWnd == hWnd) {
				m_bWindowActive = TRUE;
			}
		}
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnCommand											 */
/* 内容		:メッセージハンドラ(WM_LBUTTONDOWN)								 */
/* 日付		:2005/06/11														 */
/* ========================================================================= */

void CMainFrame::OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify)
{
	/* ボタンが押された？ */
	if (codeNotify == BN_CLICKED) {
		/* 各ウィンドウで処理させる */
		PostMessage (hWndCtl, WM_COMMAND, MAKELONG (codeNotify, id), (LPARAM)hWndCtl);
		return;
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnActivate											 */
/* 内容		:メッセージハンドラ(WM_ACTIVATE)								 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

void CMainFrame::OnActivate(HWND hWnd, UINT state, HWND hwndActDeact, BOOL fMinimized)
{
	/* 非アクティブになる？ */
	if (state == WA_INACTIVE) {
		m_bWindowActive = FALSE;
		m_pMgrKeyInput->Reset ();

	} else {
		m_bWindowActive = TRUE;
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnKeyUp											 */
/* 内容		:メッセージハンドラ(WM_KEYUP)									 */
/* 日付		:2007/02/13														 */
/* ========================================================================= */

void CMainFrame::OnKeyUp(HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	if (m_pStateProc == NULL) {
		return;
	}
	if (vk == VK_SNAPSHOT) {
		m_pMgrDraw->SaveScreenShot ();
	}
}


/* ========================================================================= */
/* 関数名	:OnLButtonDown													 */
/* 内容		:メッセージハンドラ(WM_LBUTTONDOWN)								 */
/* 日付		:2007/03/17														 */
/* ========================================================================= */

void CMainFrame::OnLButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnLButtonDown (x, y);
}


/* ========================================================================= */
/* 関数名	:OnRButtonDown													 */
/* 内容		:メッセージハンドラ(WM_RBUTTONDOWN)								 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

void CMainFrame::OnRButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnRButtonDown (x, y);
}


/* ========================================================================= */
/* 関数名	:OnRButtonDblClk												 */
/* 内容		:メッセージハンドラ(WM_RBUTTONDBLCLK)							 */
/* 日付		:2007/09/16														 */
/* ========================================================================= */

void CMainFrame::OnRButtonDblClk(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnRButtonDblClk (x, y);
}


/* ========================================================================= */
/* 関数名	:OnMouseMove													 */
/* 内容		:メッセージハンドラ(WM_MOUSEMOVE)								 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

void CMainFrame::OnMouseMove(HWND hWnd, int x, int y, UINT keyFlags)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnMouseMove (x, y);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnMgrDraw											 */
/* 内容		:メッセージハンドラ(WM_MGTDRAW)									 */
/* 日付		:2007/02/27														 */
/* ========================================================================= */

void CMainFrame::OnMgrDraw(int nCode, DWORD dwPara)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnMgrDraw (nCode, dwPara);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnWindowMsg										 */
/* 内容		:メッセージハンドラ(WM_WINDOWMSG)								 */
/* 日付		:2005/06/22														 */
/* ========================================================================= */

void CMainFrame::OnWindowMsg(int nCode, DWORD dwPara)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnWindowMsg (nCode, dwPara);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnAdminMsg											 */
/* 内容		:メッセージハンドラ(WM_ADMINMSG)								 */
/* 日付		:2007/03/18														 */
/* ========================================================================= */

void CMainFrame::OnAdminMsg(int nCode, DWORD dwPara)
{
	if (m_pStateProc == NULL) {
		return;
	}
	m_pStateProc->OnAdminMsg (nCode, dwPara);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnMainFrame										 */
/* 内容		:メッセージハンドラ(WM_MAINFRAME)								 */
/* 日付		:2006/11/03														 */
/* ========================================================================= */

void CMainFrame::OnMainFrame(DWORD dwCommand, DWORD dwParam)
{
	if (m_pStateProc == NULL) {
		return;
	}
	switch (dwCommand) {
	case MAINFRAMEMSG_CHGSTATE:			/* 状態変更 */
		ChgGameState (dwParam);
		break;
	case MAINFRAMEMSG_CONNECT:			/* 接続 */
		Connect ();
		break;
	case MAINFRAMEMSG_RENEWVIEWSET:		/* 表示設定更新 */
		m_nDrawCount = 30;
		if (m_pMgrData->GetOption60Frame () == TRUE) {
			m_nDrawCount = 60;
		}
		m_nFPS = 1000 / m_nDrawCount + 1;
		break;
	default:
		m_pStateProc->OnMainFrame (dwCommand, dwParam);
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnConnect											 */
/* 内容		:メッセージハンドラ(WM_URARASOCK_CONNECT)						 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CMainFrame::OnConnect(void)
{
	CPacketVERSION_REQ_VERSIONCHECK Packet;

	Packet.Make (VERSIONVAL);
	m_pSock->Send (&Packet);
}


/* ========================================================================= */
/* 関数名：	CMainFrame::OnDisConnect										 */
/* 内容：	メッセージハンドラ(WM_URARASOCK_DISCONNECT)						 */
/* 日付：	2005/06/12														 */
/* ========================================================================= */

void CMainFrame::OnDisConnect(void)
{
	switch (m_nGameState) {
	case GAMESTATE_LOGIN:		/* ログイン画面 */
		DisConnectProc (DISCONNECTID_CONNECT);
		break;
	case GAMESTATE_MAP:			/* マップ画面 */
		m_pMgrData->AddSystemMsg (TRUE, "サーバーとの接続が切れました", RGB (0, 200, 255));
		m_pMgrSound->PlaySound (SOUNDID_W_CHAT);

		SendMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWSYSTEMMSG, 0);
		FlashMainWindow ();
		ChgGameState (GAMESTATE_DISCONNECT);
		break;
	default:					/* その他 */
		ChgGameState (GAMESTATE_DISCONNECT);
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::OnRecv												 */
/* 内容		:メッセージハンドラ(WM_URARASOCK_RECV)							 */
/* 日付		:2006/11/05														 */
/* ========================================================================= */

void CMainFrame::OnRecv(PBYTE pData)
{
	CPacketBase Packet;

	Packet.Set (pData);

	switch (Packet.m_byCmdMain) {
	case SBOCOMMANDID_MAIN_VERSION:	RecvProcVERSION	(Packet.m_byCmdSub, pData);		break;		/* バージョン系 */
	case SBOCOMMANDID_MAIN_CONNECT:	RecvProcCONNECT	(Packet.m_byCmdSub, pData);		break;		/* 接続系 */
	case SBOCOMMANDID_MAIN_ACCOUNT:	RecvProcACCOUNT	(Packet.m_byCmdSub, pData);		break;		/* アカウント系 */
	case SBOCOMMANDID_MAIN_CHAR:	RecvProcCHAR	(Packet.m_byCmdSub, pData);		break;		/* キャラ系 */
	case SBOCOMMANDID_MAIN_MAP:		RecvProcMAP		(Packet.m_byCmdSub, pData);		break;		/* マップ系 */
	case SBOCOMMANDID_MAIN_ITEM:	RecvProcITEM	(Packet.m_byCmdSub, pData);		break;		/* アイテム系 */
	case SBOCOMMANDID_MAIN_ADMIN:	RecvProcADMIN	(Packet.m_byCmdSub, pData);		break;		/* 管理者系 */
	case SBOCOMMANDID_MAIN_EFFECT:	RecvProcEFFECT	(Packet.m_byCmdSub, pData);		break;		/* エフェクト系 */
	case SBOCOMMANDID_MAIN_SYSTEM:	RecvProcSYSTEM	(Packet.m_byCmdSub, pData);		break;		/* システム系 */
	case SBOCOMMANDID_MAIN_SKILL:	RecvProcSKILL	(Packet.m_byCmdSub, pData);		break;		/* スキル系 */
	}

	m_pSock->DeleteRecvData (pData);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::TimerProc											 */
/* 内容		:時間処理														 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

BOOL CMainFrame::TimerProc(void)
{
	BOOL bRet, bDraw, bResult;
	PCInfoCharCli pPlayerChar;

	bRet = FALSE;
	bDraw = FALSE;
	bDraw |= m_pMgrDraw->			TimerProc ();
	bDraw |= m_pMgrLayer->			TimerProc ();
	bDraw |= m_pMgrWindow->			TimerProc ();
	bDraw |= m_pLibInfoChar->		Proc ();
	bDraw |= m_pLibInfoMapParts->	Proc ();
	bDraw |= m_pLibInfoMapShadow->	Proc ();

	if (m_pStateProc) {
		bDraw |= m_pStateProc->TimerProc ();
	}

	if (m_bRenewCharInfo) {
		m_bRenewCharInfo = FALSE;
		pPlayerChar = m_pMgrData->GetPlayerChar ();
		if (pPlayerChar) {
			/* 重なり調整 */
			m_pLibInfoChar->SortY ();

			bResult = m_pLibInfoChar->DeleteOutScreen (pPlayerChar);
			if (bResult) {
				PostMessage (m_hWnd, WM_MAINFRAME, MAINFRAMEMSG_RENEWCHARCOUNT, m_pLibInfoChar->GetCount ());
			}
		}
	}

	MsgWaitForMultipleObjects (0, NULL, FALSE, 1, QS_ALLINPUT);

	bRet = bDraw;
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CMainFrame::KeyProc											 */
/* 内容		:キー入力処理													 */
/* 日付		:2007/03/19														 */
/* ========================================================================= */

void CMainFrame::KeyProc(void)
{
	int i;
	BOOL bResult, bDown;
	BYTE byCode, byCodeTmp, abyCode[] = {VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, 0};

	if (m_bWindowActive == FALSE) {
		return;
	}
	if (GetForegroundWindow () != m_hWnd) {
		m_bWindowActive = FALSE;
		return;
	}

	/* ウィンドウで必要であればウィンドウ、そうでなければ状態処理のキー処理を行う */
	m_pMgrKeyInput->Renew (byCode, bDown);
	if (byCode == 0) {
		for (i = 0; ; i ++) {
			byCodeTmp = abyCode[i];
			if (byCodeTmp == 0) {
				break;
			}
			bResult = m_pMgrKeyInput->IsInput (byCodeTmp);
			if (bResult) {
				byCode	= byCodeTmp;
				bDown	= TRUE;
				break;
			}
		}
	}
	if ((byCode == 0) && (bDown == FALSE)) {
		return;
	}
	bResult = m_pMgrWindow->IsKeyInput ();
	if (bResult) {
		m_pMgrWindow->KeyProc (byCode, bDown);

	} else {
		if (m_pStateProc) {
			m_pStateProc->KeyProc (byCode, bDown);
		}
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::ChgGameState										 */
/* 内容		:ゲーム状態を切り替える											 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

void CMainFrame::ChgGameState(int nGameState)
{
	m_pMgrLayer->	DeleteAll ();
	m_pMgrWindow->	DeleteAll ();
	SAFE_DELETE (m_pStateProc);

	m_nGameState = nGameState;

	switch (nGameState) {
	case GAMESTATE_LOGO:		m_pStateProc = new CStateProcLOGO;			break;	/* URARA-worksロゴ */
	case GAMESTATE_LOGIN:		m_pStateProc = new CStateProcLOGIN;			break;	/* ログイン画面 */
	case GAMESTATE_DISCONNECT:	m_pStateProc = new CStateProcDISCONNECT;	break;	/* 切断 */
	case GAMESTATE_INFO:		m_pStateProc = new CStateProcINFO;			break;	/* お知らせ画面 */
	case GAMESTATE_LOGINMENU:	m_pStateProc = new CStateProcLOGINMENU;		break;	/* メニュー画面 */
	case GAMESTATE_MAP:			m_pStateProc = new CStateProcMAP;			break;	/* マップ画面 */
	}

	if (m_pStateProc) {
		m_pStateProc->Create (m_pMgrData, m_pSock);
		m_pStateProc->Init ();
	}
}


/* ========================================================================= */
/* 関数名	:CMainFrame::Connect											 */
/* 内容		:サーバーへ接続													 */
/* 日付		:2005/06/09														 */
/* ========================================================================= */

void CMainFrame::Connect(void)
{
	WORD wPort;
	CmyString strTmp;

	strTmp	= m_pMgrData->GetServerAddr ();
	wPort	= m_pMgrData->GetServerPort ();

	m_pMgrWindow->MakeWindowMSG ("サーバーに接続しています");
	m_pSock->Connect (m_hWnd, URARASOCK_MSGBASE, URARASOCK_PRECHECK, wPort, strTmp);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::FlashMainWindow									 */
/* 内容		:ウィンドウを点滅させる											 */
/* 日付		:2008/06/21														 */
/* ========================================================================= */

void CMainFrame::FlashMainWindow(void)
{
	UINT uState;
	FLASHWINFO stFwi;
	APPBARDATA stAbd;

	if (m_bWindowActive) {
		return;
	}
	ZeroMemory (&stAbd, sizeof (stAbd));
	stAbd.cbSize = sizeof (stAbd);
	uState = SHAppBarMessage (ABM_GETSTATE, &stAbd);
	if (uState & ABS_AUTOHIDE) {
		return;
	}
	if (m_pMgrData->GetOptionTaskbar () == FALSE) {
		return;
	}

	ZeroMemory (&stFwi, sizeof (stFwi));
	stFwi.cbSize	= sizeof (stFwi);
	stFwi.hwnd		= m_hWnd;
	stFwi.dwFlags	= FLASHW_TRAY;
	stFwi.uCount	= 4;
	stFwi.dwTimeout	= 300;
	FlashWindowEx (&stFwi);
}


/* ========================================================================= */
/* 関数名	:CMainFrame::GetMsgCmdType										 */
/* 内容		:チャット入力処理												 */
/* 日付		:2007/05/02														 */
/* ========================================================================= */

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

	ParamUtil.Set (pszText);
	nCount	= ParamUtil.GetCount ();
	pszTmp	= ParamUtil.GetParam (0);

	/* 表情変更 */
	if (strcmp (&pszTmp[1], "face") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGFACE;
		}

	/* 髪変更 */
	} else if (strcmp (&pszTmp[1], "hair") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGHAIR;
		}

	/* 服装変更 */
	} else if (strcmp (&pszTmp[1], "cloth") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGCLOTH;
		}

	/* アクセサリ変更 */
	} else if (strcmp (&pszTmp[1], "acce") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGACCE;
		}

	/* BGM音量設定 */
	} else if (strcmp (&pszTmp[1], "bgm") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_BGMVOLUME;
		}

	/* 色変更 */
	} else if (strcmp (&pszTmp[1], "color") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGCOLOR;
		}

	/* 効果音量設定 */
	} else if (strcmp (&pszTmp[1], "se") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_SEVOLUME;
		}

	/* 持ち物変更 */
	} else if (strcmp (&pszTmp[1], "arms") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGARMS;
		}

	/* 盾変更 */
	} else if (strcmp (&pszTmp[1], "shield") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_CHGSHIELD;
		}

	/* 配置アイテム変更 */
	} else if (strcmp (&pszTmp[1], "item") == 0) {
		if (nCount == 2) {
			nRet = MSGCMDTYPE_SETITEM;
		}

	/* アイテム作成 */
	} else if (strcmp (&pszTmp[1], "makeitem") == 0) {
		if (m_pMgrData->GetAdminLevel () > ADMINLEVEL_NONE) {
			if (nCount == 2) {
				nRet = MSGCMDTYPE_MAKEITEM;
			}
		}

	/* 噴出し */
//	} else if (strcmp (&pszTmp[1], "balloon") == 0) {
//		if (nCount == 2) {
//			nRet = MSGCMDTYPE_BALLOON;
//		}

	/* サイコロ */
	} else if (strcmp (&pszTmp[1], "dice") == 0) {
		if (nCount == 1) {
			nRet = MSGCMDTYPE_DICE;
		}

	/* ランダム */
	} else if (strcmp (&pszTmp[1], "rnd") == 0) {
		if (nCount <= 2) {
			nRet = MSGCMDTYPE_RND;
		}

	/* 現在時刻 */
	} else if (strcmp (&pszTmp[1], "now") == 0) {
		if (nCount == 1) {
			nRet = MSGCMDTYPE_NOW;
		}

	/* 最も集まっている場所 */
	} else if (strcmp (&pszTmp[1], "where") == 0) {
		if (nCount == 1) {
			nRet = MSGCMDTYPE_WHERE;
		}
	}

Exit:
	return nRet;
}

/* Copyright(C)URARA-works 2006 */
