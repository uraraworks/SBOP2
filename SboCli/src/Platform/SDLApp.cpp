/* Copyright(C)URARA-works 2025 */
/* ========================================================================= */
/* File        : SDLApp.cpp                                                    */
/* Contents    : SDL application main loop                                     */
/* Created     : 2025/06/01                                                    */
/* ========================================================================= */

#include "stdafx.h"
#include <SDL_syswm.h>
#include "SDLApp.h"
#include "SDLEventUtil.h"
#include "SDLInput.h"
#include "SboCli_priv.h"


/* ========================================================================= */
/* Constants                                                                  */
/* ========================================================================= */

/* Limit how many fixed updates run back-to-back when the client stalls. */
#define MAX_FRAME_SKIP	5

static HWND GetMainWindowHandle(SDL_Window *pWindow)
{
	SDL_SysWMinfo wmInfo;

	if (pWindow == NULL) {
		return NULL;
	}

	SDL_VERSION (&wmInfo.version);
	if (!SDL_GetWindowWMInfo (pWindow, &wmInfo)) {
		return NULL;
	}

	return wmInfo.info.win.window;
}

static BOOL IsMainWindowMessage(HWND hMainWnd, const MSG &msg)
{
	if ((hMainWnd == NULL) || (msg.hwnd != hMainWnd)) {
		return FALSE;
	}

	return TRUE;
}

static BOOL ShouldSkipMainWindowInputMessage(HWND hMainWnd, const MSG &msg)
{
	if (!IsMainWindowMessage (hMainWnd, msg)) {
		return FALSE;
	}

	return IsSDLManagedInputWindowMessage (msg.message);
}

static BOOL ShouldBridgeWin32QuitMessage(HWND hMainWnd, const MSG &msg)
{
	if (msg.message == WM_QUIT) {
		return TRUE;
	}

	if (!IsMainWindowMessage (hMainWnd, msg)) {
		return FALSE;
	}

	return (msg.message == WM_CLOSE) ? TRUE : FALSE;
}

/* ========================================================================= */
/* Function    : CSDLApp::CSDLApp                                             */
/* ========================================================================= */

CSDLApp::CSDLApp()
{
	m_bInitialized = FALSE;
}


/* ========================================================================= */
/* Function    : CSDLApp::~CSDLApp                                            */
/* ========================================================================= */

CSDLApp::~CSDLApp()
{
	Destroy ();
}


/* ========================================================================= */
/* Function    : CSDLApp::Init                                                */
/* ========================================================================= */

BOOL CSDLApp::Init(void)
{
	if (m_bInitialized) {
		return TRUE;
	}

	SDL_SetMainReady ();

	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
		return FALSE;
	}

	m_bInitialized = TRUE;
	return TRUE;
}


/* ========================================================================= */
/* Function    : CSDLApp::Destroy                                             */
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
/* Function    : CSDLApp::Run                                                 */
/* Contents    : Run fixed-step update loop with decoupled render rate.       */
/* ========================================================================= */

int CSDLApp::Run(IGameLoopHost *pHost, const char *pszTitle, int nWidth, int nHeight)
{
	SDL_Event	sdlEvent;
	BYTE		byFps;
	DWORD		dwUpdateInterval;
	DWORD		dwRenderInterval;
	DWORD		dwAccumulated;
	DWORD		dwTimeLast;
	DWORD		dwLastRenderTime;
	DWORD		dwTimeStart;
	DWORD		dwTimeTmp;
	BOOL		bDraw;
	BOOL		bDrawPending;
	BOOL		bQuit;

	if (!pHost) {
		return -1;
	}

	if (!m_Window.Create (pszTitle, nWidth, nHeight)) {
		return -1;
	}
	if (!m_Window.CreateRenderer ()) {
		return -1;
	}
	if (!pHost->OnSDLInit (m_Window.GetSDLWindow ())) {
		return -1;
	}

	dwUpdateInterval = (GAME_UPDATE_FPS > 0) ? (DWORD)(1000 / GAME_UPDATE_FPS) : 16;
	if (dwUpdateInterval == 0) {
		dwUpdateInterval = 1;
	}

	dwRenderInterval = (g_nSboCliRenderFrameRate > 0)
		? (DWORD)(1000 / g_nSboCliRenderFrameRate)
		: dwUpdateInterval;
	if (dwRenderInterval == 0) {
		dwRenderInterval = 1;
	}

	dwAccumulated		= 0;
	dwTimeLast			= SDL_GetTicks ();
	dwLastRenderTime	= (dwTimeLast >= dwRenderInterval) ? (dwTimeLast - dwRenderInterval) : 0;
	dwTimeStart			= dwTimeLast;
	byFps				= 0;
	bDrawPending		= FALSE;
	bQuit				= FALSE;

	while (!bQuit)
	{
		while (SDL_PollEvent (&sdlEvent))
		{
			switch (sdlEvent.type)
			{
			case SDL_QUIT:
				bQuit = TRUE;
				break;

			case SDL_KEYDOWN:
				/* Keyboard state is sampled separately by MgrKeyInput::Renew(). */
				break;

			case SDL_KEYUP:
				pHost->OnSDLKeyUp (CSDLInput::ScancodeToVK (sdlEvent.key.keysym.scancode));
				break;

			case SDL_WINDOWEVENT:
				switch (sdlEvent.window.event)
				{
				case SDL_WINDOWEVENT_FOCUS_GAINED:
				case SDL_WINDOWEVENT_RESTORED:
					pHost->OnSDLFocusChanged (TRUE);
					break;

				case SDL_WINDOWEVENT_FOCUS_LOST:
				case SDL_WINDOWEVENT_MINIMIZED:
					pHost->OnSDLFocusChanged (FALSE);
					break;

				default:
					break;
				}
				break;

			case SDL_MOUSEMOTION:
				pHost->OnSDLMouseMove (sdlEvent.motion.x, sdlEvent.motion.y);
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
					pHost->OnSDLMouseLeftButtonDown (
						sdlEvent.button.x,
						sdlEvent.button.y,
						(sdlEvent.button.clicks >= 2) ? TRUE : FALSE);
				} else if (sdlEvent.button.button == SDL_BUTTON_RIGHT) {
					pHost->OnSDLMouseRightButtonDown (
						sdlEvent.button.x,
						sdlEvent.button.y,
						(sdlEvent.button.clicks >= 2) ? TRUE : FALSE);
					if (sdlEvent.button.clicks >= 2) {
						pHost->OnSDLMouseRightButtonDoubleClick (
							sdlEvent.button.x,
							sdlEvent.button.y);
					}
				}
				break;

			default:
				break;
			}
		}

		if (bQuit) {
			break;
		}

		PollWin32Messages ();

		if (pHost->IsQuit ()) {
			break;
		}

		dwTimeTmp = SDL_GetTicks ();
		{
			DWORD dwElapsed = dwTimeTmp - dwTimeLast;
			dwTimeLast = dwTimeTmp;
			dwAccumulated += dwElapsed;
		}

		{
			DWORD dwFrameSkip = 0;
			while ((dwAccumulated >= dwUpdateInterval) && (dwFrameSkip < MAX_FRAME_SKIP))
			{
				dwAccumulated -= dwUpdateInterval;
				bDraw = pHost->OnFrame ();
				if (bDraw) {
					bDrawPending = TRUE;
				}
				dwFrameSkip++;
			}
		}

		dwRenderInterval = (g_nSboCliRenderFrameRate > 0)
			? (DWORD)(1000 / g_nSboCliRenderFrameRate)
			: dwUpdateInterval;
		if (dwRenderInterval == 0) {
			dwRenderInterval = 1;
		}

		if (bDrawPending && ((dwTimeTmp - dwLastRenderTime) >= dwRenderInterval))
		{
			SDL_Renderer *pRenderer = m_Window.GetRenderer ();
			if (pRenderer)
			{
				SDL_RenderClear (pRenderer);
				pHost->OnDraw (pRenderer);
				byFps++;
			}
			dwLastRenderTime = dwTimeTmp;
			bDrawPending = FALSE;
		}

		if (dwAccumulated < dwUpdateInterval)
		{
			DWORD dwSleepTime = dwUpdateInterval - dwAccumulated;
			SDL_Delay ((dwSleepTime > 1) ? dwSleepTime - 1 : 1);
		}

		if (dwTimeTmp - dwTimeStart >= 1000)
		{
			byFps = 0;
			dwTimeStart = dwTimeTmp;
		}
	}

	pHost->OnSDLDestroy ();

	return 0;
}


/* ========================================================================= */
/* Function    : CSDLApp::PollWin32Messages                                   */
/* ========================================================================= */

void CSDLApp::PollWin32Messages(void)
{
	MSG msg;
	HWND hMainWnd;

	hMainWnd = GetMainWindowHandle (m_Window.GetSDLWindow ());

	/* SDL 管理済みのメインウィンドウ入力はここで捨てる。 */
	/* それ以外の独自メッセージや子ウィンドウ通知は Win32 として流す。 */
	while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (ShouldBridgeWin32QuitMessage (hMainWnd, msg)) {
			PushSDLQuitEvent ();
			if (msg.message == WM_QUIT) {
				break;
			}
			continue;
		}

		if (ShouldSkipMainWindowInputMessage (hMainWnd, msg)) {
			continue;
		}

		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
}

/* Copyright(C)URARA-works 2025 */
