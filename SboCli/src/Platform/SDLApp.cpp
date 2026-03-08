/* Copyright(C)URARA-works 2025 */
/* ========================================================================= */
/* File        : SDLApp.cpp                                                    */
/* Contents    : SDL application main loop                                     */
/* Created     : 2025/06/01                                                    */
/* ========================================================================= */

#include "stdafx.h"
#include "SDLApp.h"
#include "SboCli_priv.h"


/* ========================================================================= */
/* Constants                                                                  */
/* ========================================================================= */

/* Limit how many fixed updates run back-to-back when the client stalls. */
#define MAX_FRAME_SKIP	5


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
	if (!pHost->OnSDLInit (m_Window.GetHWND ())) {
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
			case SDL_KEYUP:
				/* Keyboard state is sampled separately by MgrKeyInput::Renew(). */
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

	while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT) {
			SDL_Event sdlQuit;
			ZeroMemory (&sdlQuit, sizeof (sdlQuit));
			sdlQuit.type = SDL_QUIT;
			SDL_PushEvent (&sdlQuit);
			break;
		}

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
