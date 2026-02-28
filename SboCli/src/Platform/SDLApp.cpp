/* Copyright(C)URARA-works 2025 */
/* ========================================================================= */
/* ファイル名	:SDLApp.cpp													 */
/* 内容			:SDLアプリケーション 実装ファイル							 */
/* 作成開始日	:2025/06/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "SDLApp.h"
#include "SboCli_priv.h"	/* DRAWCOUNT, SCRSIZEX, SCRSIZEY */


/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* 1ループでの最大フレームスキップ数（既存 MainLoop と同値） */
#define MAX_FRAME_SKIP	5


/* ========================================================================= */
/* 関数名	:CSDLApp::CSDLApp												 */
/* 内容		:コンストラクタ													 */
/* 日付		:2025/06/01														 */
/* ========================================================================= */

CSDLApp::CSDLApp()
{
	m_bInitialized = FALSE;
}


/* ========================================================================= */
/* 関数名	:CSDLApp::~CSDLApp												 */
/* 内容		:デストラクタ													 */
/* 日付		:2025/06/01														 */
/* ========================================================================= */

CSDLApp::~CSDLApp()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CSDLApp::Init													 */
/* 内容		:SDL2 を初期化する												 */
/* 日付		:2025/06/01														 */
/* ========================================================================= */

BOOL CSDLApp::Init(void)
{
	if (m_bInitialized) {
		return TRUE;
	}

	/* SDL_MAIN_HANDLED 定義済みのため SDL_SetMainReady() が必要 */
	SDL_SetMainReady ();

	/* VIDEO + JOYSTICK を初期化（ゲームコントローラ対応） */
	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
		return FALSE;
	}

	m_bInitialized = TRUE;
	return TRUE;
}


/* ========================================================================= */
/* 関数名	:CSDLApp::Destroy												 */
/* 内容		:SDL2 を終了する												 */
/* 日付		:2025/06/01														 */
/* ========================================================================= */

void CSDLApp::Destroy(void)
{
	m_Window.Destroy ();
	if (m_bInitialized) {
		SDL_Quit ();
		m_bInitialized = FALSE;
	}
}


/* ========================================================================= */
/* 関数名	:CSDLApp::Run													 */
/* 内容		:SDL メインループを実行する										 */
/* 日付		:2025/06/01														 */
/* 補足		:既存 CMainFrame::MainLoop のタイミング制御を移植した			 */
/*			 - SDL_GetTicks() を timeGetTime() の代替に使用				 */
/*			 - SDL_PollEvent + PeekMessage の二重ポーリングで				 */
/*			   SDL イベントと Win32 独自メッセージ（WM_MAINFRAME等）の		 */
/*			   両方を処理する												 */
/* ========================================================================= */

int CSDLApp::Run(IGameLoopHost *pHost, const char *pszTitle, int nWidth, int nHeight)
{
	SDL_Event	sdlEvent;
	BYTE		byFps;
	DWORD		dwFrameInterval;
	DWORD		dwAccumulated;
	DWORD		dwTimeLast;
	DWORD		dwTimeStart;
	DWORD		dwTimeTmp;
	BOOL		bDraw;
	BOOL		bQuit;

	if (!pHost) {
		return -1;
	}

	/* ウィンドウ生成 */
	if (!m_Window.Create (pszTitle, nWidth, nHeight)) {
		return -1;
	}

	/* SDL_Renderer 生成（Phase 3: GDI BitBlt → SDL_RenderCopy へ移行） */
	if (!m_Window.CreateRenderer ()) {
		return -1;
	}

	/* ゲーム側の初期化（CMainFrame::OnSDLInit） */
	if (!pHost->OnSDLInit (m_Window.GetHWND ())) {
		return -1;
	}

	/* タイミング制御の初期化 */
	/* DrawCount: DRAWCOUNT マクロ（SboCli_priv.h）から取得（30FPS想定） */
	dwFrameInterval = (DRAWCOUNT > 0) ? (DWORD)(1000 / DRAWCOUNT) : 16;
	if (dwFrameInterval == 0) {
		dwFrameInterval = 1;
	}
	dwAccumulated	= 0;
	dwTimeLast		= SDL_GetTicks ();
	dwTimeStart		= dwTimeLast;
	byFps			= 0;
	bQuit			= FALSE;

	/* =============================== */
	/* メインループ                    */
	/* =============================== */
	while (!bQuit)
	{
		/* --- SDL イベント処理 --- */
		while (SDL_PollEvent (&sdlEvent))
		{
			switch (sdlEvent.type)
			{
			case SDL_QUIT:
				/* ウィンドウ閉じるボタン → WM_QUIT に相当 */
				bQuit = TRUE;
				break;

			case SDL_KEYDOWN:
			case SDL_KEYUP:
				/* Phase 3: MgrKeyInput::Renew() が SDL_GetKeyboardState() を使用。 */
				/* SDL_PollEvent() 呼び出し後に SDL のキー状態が更新される。  */
				break;

			default:
				break;
			}
		}

		if (bQuit) {
			break;
		}

		/* --- Win32 メッセージ処理（WM_MAINFRAME / WM_ADMINMSG / ソケット等）--- */
		PollWin32Messages ();

		/* --- ゲームループ終了チェック --- */
		if (pHost->IsQuit ()) {
			break;
		}

		/* --- フレームタイミング制御 --- */
		dwTimeTmp = SDL_GetTicks ();
		{
			DWORD dwElapsed = dwTimeTmp - dwTimeLast;
			dwTimeLast = dwTimeTmp;
			dwAccumulated += dwElapsed;
		}

		{
			DWORD dwFrameSkip = 0;
			while (dwAccumulated >= dwFrameInterval && dwFrameSkip < MAX_FRAME_SKIP)
			{
				dwAccumulated -= dwFrameInterval;
				bDraw = pHost->OnFrame ();

				/* 描画が必要なフレームのみ SDL 描画を実行 */
				if (bDraw)
				{
					SDL_Renderer *pRenderer = m_Window.GetRenderer ();
					if (pRenderer)
					{
						SDL_RenderClear (pRenderer);
						/* MgrDraw::Draw() が SDL_RenderPresent まで行う */
						pHost->OnDraw (pRenderer);
					}
					byFps++;
				}
				dwFrameSkip++;
			}

			/* 残り時間に応じてスリープ（CPU占有を避ける） */
			if (dwAccumulated < dwFrameInterval)
			{
				DWORD dwSleepTime = dwFrameInterval - dwAccumulated;
				SDL_Delay ((dwSleepTime > 1) ? dwSleepTime - 1 : 1);
			}
		}

		/* FPS更新（1秒ごと）*/
		if (dwTimeTmp - dwTimeStart >= 1000)
		{
			byFps		= 0;
			dwTimeStart = dwTimeTmp;
		}
	}

	/* ゲーム側の後片付け */
	pHost->OnSDLDestroy ();

	return 0;
}


/* ========================================================================= */
/* 関数名	:CSDLApp::PollWin32Messages										 */
/* 内容		:Win32 メッセージキューをポーリングする							 */
/* 日付		:2025/06/01														 */
/* 補足		:WM_MAINFRAME, WM_ADMINMSG, WM_URARASOCK_* 等の独自メッセージは	 */
/*			 SDL_PollEvent では処理されないため、別途 PeekMessage でポーリング*/
/* ========================================================================= */

void CSDLApp::PollWin32Messages(void)
{
	MSG msg;

	while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
	{
		/* WM_QUIT が来たら SDL_QUIT イベントとしてブリッジする */
		/* （OnDestroy → PostQuitMessage からの流れを SDL のループ終了に伝達） */
		if (msg.message == WM_QUIT) {
			SDL_Event sdlQuit;
			ZeroMemory (&sdlQuit, sizeof (sdlQuit));
			sdlQuit.type = SDL_QUIT;
			SDL_PushEvent (&sdlQuit);
			break;
		}

		/* Alt/F10 のシステムキーは無視（既存コードと同様） */
		if (msg.message == WM_SYSKEYDOWN) {
			if (msg.wParam == VK_MENU || msg.wParam == VK_F10) {
				continue;
			}
		}

		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
}

/* Copyright(C)URARA-works 2025 */
