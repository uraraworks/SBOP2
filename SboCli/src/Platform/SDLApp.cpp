/// @file SDLApp.cpp
/// @brief SDL application main loop
/// @date 2025/06/01
/// @copyright Copyright(C)URARA-works 2025

#include "stdafx.h"
#if defined(_WIN32)
#include <SDL_syswm.h>
#else
#include <emscripten/emscripten.h>
#include <emscripten/em_js.h>
#endif
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include "SDLApp.h"
#include "SDLEventUtil.h"
#include "SDLInput.h"
#include "SboCli_priv.h"
#if !defined(_WINDLL)
#include "SdlFont.h"
#endif
#include "UraraSockTCP.h"


// Constants

// Limit how many fixed updates run back-to-back when the client stalls.
#define MAX_FRAME_SKIP	5

#if defined(__EMSCRIPTEN__)
static CSDLApp *g_pBrowserSDLApp = NULL;

EM_JS(void, SBOP2_DebugMarkStage, (int stage, int hasRenderer, int r, int g, int b), {
	Module.sbop2Stage = stage;
	Module.sbop2OnDrawHasRenderer = hasRenderer;
});
EM_JS(void, SBOP2_DebugCountOnDraw, (int hasRenderer), {
	Module.sbop2OnDrawCount = (Module.sbop2OnDrawCount || 0) + 1;
	Module.sbop2OnDrawHasRenderer = hasRenderer;
});

extern "C" EMSCRIPTEN_KEEPALIVE void SBOP2_BrowserPumpSingleFrame(void)
{
	if (g_pBrowserSDLApp == NULL) {
		return;
	}
	g_pBrowserSDLApp->RequestBrowserRedraw();
}
#else
static void SBOP2_DebugMarkStage(int stage, int hasRenderer, int r, int g, int b)
{
	UNREFERENCED_PARAMETER(stage);
	UNREFERENCED_PARAMETER(hasRenderer);
	UNREFERENCED_PARAMETER(r);
	UNREFERENCED_PARAMETER(g);
	UNREFERENCED_PARAMETER(b);
}

static void SBOP2_DebugCountOnDraw(int hasRenderer)
{
	UNREFERENCED_PARAMETER(hasRenderer);
}
#endif

#if defined(_WIN32)
static HWND GetMainWindowHandle(SDL_Window *pWindow)
{
	SDL_SysWMinfo wmInfo;

	if (pWindow == NULL) {
		return NULL;
	}

	SDL_VERSION(&wmInfo.version);
	if (!SDL_GetWindowWMInfo(pWindow, &wmInfo)) {
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
	if (!IsMainWindowMessage(hMainWnd, msg)) {
		return FALSE;
	}

	return IsSDLManagedInputWindowMessage(msg.message);
}

static BOOL ShouldBridgeWin32QuitMessage(HWND hMainWnd, const MSG &msg)
{
	if (msg.message == WM_QUIT) {
		return TRUE;
	}

	if (!IsMainWindowMessage(hMainWnd, msg)) {
		return FALSE;
	}

	return (msg.message == WM_CLOSE) ? TRUE : FALSE;
}

#endif

CSDLApp::CSDLApp()
{
	m_bInitialized = FALSE;
	m_pHost = NULL;
	m_byFps = 0;
	m_dwUpdateInterval = 0;
	m_dwRenderInterval = 0;
	m_dwAccumulated = 0;
	m_dwTimeLast = 0;
	m_dwLastRenderTime = 0;
	m_dwTimeStart = 0;
	m_bDrawPending = FALSE;
	m_bQuit = FALSE;
	m_bDestroyCalled = FALSE;
	m_bImGuiInitialized = FALSE;
}

CSDLApp::~CSDLApp()
{
	Destroy();
}

BOOL CSDLApp::Init(void)
{
	if (m_bInitialized) {
		return TRUE;
	}

	SDL_SetMainReady();

	// IME フローティングウィンドウを SDL ウィンドウ内に表示するためのヒント。
	// SDL_CreateWindow() より前に設定する必要がある。
	// これにより ImGui の InputText フォーカス時に IME 変換ウィンドウが
	// カーソル位置（入力欄の真下）に表示される。
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
		return FALSE;
	}

#if !defined(_WINDLL)
	// SDL_ttfフォントシステムの初期化
	{
#if defined(__EMSCRIPTEN__)
		// Emscripten では --preload-file SboCli/font@/font でマウントされる
		SdlFontInit("/font");
#else
		TCHAR szFontDir[MAX_PATH];
		BuildModuleRelativePath(szFontDir, _countof(szFontDir), _T("font\\"));
		std::string ansiFontDir = TStringToAnsiStd(szFontDir);
		SdlFontInit(ansiFontDir.c_str());
#endif
	}
#endif
	m_bInitialized = TRUE;
	return TRUE;
}

void CSDLApp::Destroy(void)
{
	ShutdownImGui();
	m_Window.Destroy();
	if (m_bInitialized) {
#if !defined(_WINDLL)
		SdlFontShutdown();
#endif
		SDL_Quit();
		m_bInitialized = FALSE;
	}
}

void CSDLApp::InitImGui(SDL_Renderer *pRenderer)
{
	if (m_bImGuiInitialized) {
		return;
	}
	if (m_Window.GetSDLWindow() == NULL || pRenderer == NULL) {
		return;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.IniFilename = NULL;  // imgui.ini保存を無効化

	// 日本語フォントのロード（文字化け対策）
	// ※ multi-viewport (ImGuiConfigFlags_ViewportsEnable) は SDL_Renderer バックエンドが
	//   未対応のため有効化しない。ImGui ウィンドウはメインウィンドウ内に表示される。
	{
		ImFontConfig fontConfig;
		fontConfig.MergeMode = false;
		ImFont* pFont = io.Fonts->AddFontFromFileTTF(
			"font/NotoSansCJKjp-Regular.otf",
			16.0f,
			&fontConfig,
			io.Fonts->GetGlyphRangesJapanese()
		);
		if (pFont == NULL) {
			// フォントファイルが見つからない場合はデフォルトフォントにフォールバック
			io.Fonts->AddFontDefault();
		}
	}

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForSDLRenderer(m_Window.GetSDLWindow(), pRenderer);
	ImGui_ImplSDLRenderer2_Init(pRenderer);

	m_bImGuiInitialized = TRUE;
}

void CSDLApp::ShutdownImGui(void)
{
	if (!m_bImGuiInitialized) {
		return;
	}
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	m_bImGuiInitialized = FALSE;
}

int CSDLApp::Run(IGameLoopHost *pHost, const char *pszTitle, int nWidth, int nHeight)
{
	if (!pHost) {
		return -1;
	}

	if (!m_Window.Create(pszTitle, nWidth, nHeight)) {
		return -1;
	}
#if defined(__EMSCRIPTEN__)
	g_pBrowserSDLApp = this;
	{
		std::string strTitle = pszTitle;
		strTitle += " [CW]";
		SDL_SetWindowTitle(m_Window.GetSDLWindow(), strTitle.c_str());
		SBOP2_DebugMarkStage(1, 0, 0, 96, 192);
	}
#endif
	if (!m_Window.CreateRenderer()) {
#if defined(__EMSCRIPTEN__)
		if (m_Window.GetSDLWindow() != NULL) {
			std::string strTitle = SDL_GetWindowTitle(m_Window.GetSDLWindow());
			strTitle += " [CR-NG]";
			SDL_SetWindowTitle(m_Window.GetSDLWindow(), strTitle.c_str());
		}
#else
		return -1;
#endif
	}
#if defined(__EMSCRIPTEN__)
	{
		SDL_Renderer *pRenderer = m_Window.GetRenderer();
		std::string strTitle = SDL_GetWindowTitle(m_Window.GetSDLWindow());
		strTitle += " [CR]";
		SDL_SetWindowTitle(m_Window.GetSDLWindow(), strTitle.c_str());
		SBOP2_DebugMarkStage(2, (pRenderer != NULL) ? 1 : 0, 192, 64, 0);
	}
#endif
	if (!pHost->OnSDLInit(m_Window.GetSDLWindow())) {
#if defined(__EMSCRIPTEN__)
		if (m_Window.GetSDLWindow() != NULL) {
			std::string strTitle = SDL_GetWindowTitle(m_Window.GetSDLWindow());
			strTitle += " [INIT-NG]";
			SDL_SetWindowTitle(m_Window.GetSDLWindow(), strTitle.c_str());
		}
#endif
		return -1;
	}
#if defined(__EMSCRIPTEN__)
	{
		std::string strTitle = pszTitle;
		strTitle += " [INIT]";
		SDL_SetWindowTitle(m_Window.GetSDLWindow(), strTitle.c_str());
		SBOP2_DebugMarkStage(3, (m_Window.GetRenderer() != NULL) ? 1 : 0, 0, 160, 64);
	}
#endif

	InitImGui(m_Window.GetRenderer());

	m_pHost = pHost;
	m_dwUpdateInterval = (GAME_UPDATE_FPS > 0) ? (DWORD)(1000 / GAME_UPDATE_FPS) : 16;
	if (m_dwUpdateInterval == 0) {
		m_dwUpdateInterval = 1;
	}

	m_dwRenderInterval = (g_nSboCliRenderFrameRate > 0)
		? (DWORD)(1000 / g_nSboCliRenderFrameRate)
		: m_dwUpdateInterval;
	if (m_dwRenderInterval == 0) {
		m_dwRenderInterval = 1;
	}

	m_dwAccumulated = 0;
	m_dwTimeLast = SDL_GetTicks();
	m_dwLastRenderTime = (m_dwTimeLast >= m_dwRenderInterval) ? (m_dwTimeLast - m_dwRenderInterval) : 0;
	m_dwTimeStart = m_dwTimeLast;
	m_byFps = 0;
	// 起動直後のロゴを確実に1回描く。
	m_bDrawPending = TRUE;
	m_bQuit = FALSE;
	m_bDestroyCalled = FALSE;

#if defined(__EMSCRIPTEN__)
	{
		SDL_Renderer *pRenderer = m_Window.GetRenderer();
		if (pRenderer != NULL) {
			SDL_RenderClear(pRenderer);
		}
		SBOP2_DebugCountOnDraw((pRenderer != NULL) ? 1 : 0);
		SBOP2_DebugMarkStage(4, (pRenderer != NULL) ? 1 : 0, 208, 192, 0);
		m_pHost->OnDraw(pRenderer);
	}
#endif

#if defined(__EMSCRIPTEN__)
	// requestAnimationFrame 依存だと headless/browser 環境で継続実行されないケースがあるため、
	// ブラウザ版は timeout 駆動で固定更新を回す。
	emscripten_set_main_loop_arg(&CSDLApp::MainLoopThunk, this, GAME_UPDATE_FPS, 1);
	return 0;
#else
	while (!m_bQuit)
	{
		RunFrame();

		if (!m_bQuit && (m_dwAccumulated < m_dwUpdateInterval))
		{
			DWORD dwSleepTime = m_dwUpdateInterval - m_dwAccumulated;
			SDL_Delay((dwSleepTime > 1) ? dwSleepTime - 1 : 1);
		}
	}

	if (!m_bDestroyCalled) {
		m_pHost->OnSDLDestroy();
		m_bDestroyCalled = TRUE;
	}

	return 0;
#endif
}

void CSDLApp::RunFrame(void)
{
	SDL_Event sdlEvent;
	DWORD dwTimeTmp;
#if defined(__EMSCRIPTEN__)
	static BOOL s_bLoggedLoop = FALSE;
	static BOOL s_bLoggedDraw = FALSE;
#endif

	while (SDL_PollEvent(&sdlEvent))
	{
		if (m_bImGuiInitialized) {
			ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
		}
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			m_bQuit = TRUE;
			break;

		case SDL_KEYDOWN:
			// ImGui がキーボードをキャプチャ中の場合はゲーム側に渡さない
			if (!m_bImGuiInitialized || !ImGui::GetIO().WantCaptureKeyboard) {
				m_pHost->OnSDLKeyDown(CSDLInput::ScancodeToVK(sdlEvent.key.keysym.scancode));
			}
			break;

		case SDL_KEYUP:
			// ImGui がキーボードをキャプチャ中の場合はゲーム側に渡さない
			if (!m_bImGuiInitialized || !ImGui::GetIO().WantCaptureKeyboard) {
				m_pHost->OnSDLKeyUp(CSDLInput::ScancodeToVK(sdlEvent.key.keysym.scancode));
			}
			break;

		case SDL_TEXTINPUT:
			// ImGui がキーボードをキャプチャ中の場合はゲーム側に渡さない
			if (!m_bImGuiInitialized || !ImGui::GetIO().WantCaptureKeyboard) {
				m_pHost->OnSDLTextInput(sdlEvent.text.text);
			}
			break;

		case SDL_WINDOWEVENT:
			switch (sdlEvent.window.event)
			{
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_RESTORED:
				m_pHost->OnSDLFocusChanged(TRUE);
				break;

			case SDL_WINDOWEVENT_FOCUS_LOST:
			case SDL_WINDOWEVENT_MINIMIZED:
				m_pHost->OnSDLFocusChanged(FALSE);
				break;

			default:
				break;
			}
			break;

		case SDL_MOUSEMOTION:
			m_pHost->OnSDLMouseMove(sdlEvent.motion.x, sdlEvent.motion.y);
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
				m_pHost->OnSDLMouseLeftButtonDown(
					sdlEvent.button.x,
					sdlEvent.button.y,
					(sdlEvent.button.clicks >= 2) ? TRUE : FALSE);
			} else if (sdlEvent.button.button == SDL_BUTTON_RIGHT) {
				m_pHost->OnSDLMouseRightButtonDown(
					sdlEvent.button.x,
					sdlEvent.button.y,
					(sdlEvent.button.clicks >= 2) ? TRUE : FALSE);
				if (sdlEvent.button.clicks >= 2) {
					m_pHost->OnSDLMouseRightButtonDoubleClick(
						sdlEvent.button.x,
						sdlEvent.button.y);
				}
			}
			break;

		default:
			break;
		}
	}

	if (m_bQuit) {
		goto Exit;
	}

#if defined(__EMSCRIPTEN__)
	if (s_bLoggedLoop == FALSE) {
		SDL_Log("RunFrame: entered main loop");
		s_bLoggedLoop = TRUE;
		SBOP2_DebugMarkStage(5, (m_Window.GetRenderer() != NULL) ? 1 : 0, 160, 0, 160);
	}
#endif

	PollWin32Messages();

	// ImGui の InputText にフォーカスがある場合に SDL_TEXTINPUT イベントを発生させる
	// imgui_impl_sdl2 の 2023-04-06 更新で SDL_StartTextInput() 自動呼び出しが廃止されたため、
	// アプリ側で WantTextInput フラグを見て手動で制御する。
	// StopTextInput は SBOP2 ネイティブ入力欄（WindowCHAT 等）が自分で管理するため、
	// ImGui 側からは Stop しない（干渉防止）。
	if (m_bImGuiInitialized && ImGui::GetIO().WantTextInput) {
		if (!SDL_IsTextInputActive()) {
			SDL_StartTextInput();
		}
	}

	if (m_pHost->IsQuit()) {
		m_bQuit = TRUE;
		goto Exit;
	}

	dwTimeTmp = SDL_GetTicks();
	{
		DWORD dwElapsed = dwTimeTmp - m_dwTimeLast;
		m_dwTimeLast = dwTimeTmp;
#if defined(__EMSCRIPTEN__)
		// ブラウザでは headless/RAF 環境で経過時間が極端に小さく見えることがある。
		// 少なくとも 1 update 分は進めて fade と再描画を止めない。
		if (dwElapsed < m_dwUpdateInterval) {
			dwElapsed = m_dwUpdateInterval;
		}
#endif
		m_dwAccumulated += dwElapsed;
	}

	{
		DWORD dwFrameSkip = 0;
		while ((m_dwAccumulated >= m_dwUpdateInterval) && (dwFrameSkip < MAX_FRAME_SKIP))
		{
			m_dwAccumulated -= m_dwUpdateInterval;
			if (m_pHost->OnFrame()) {
				m_bDrawPending = TRUE;
			}
			dwFrameSkip++;
		}
	}

	m_dwRenderInterval = (g_nSboCliRenderFrameRate > 0)
		? (DWORD)(1000 / g_nSboCliRenderFrameRate)
		: m_dwUpdateInterval;
	if (m_dwRenderInterval == 0) {
		m_dwRenderInterval = 1;
	}

	if (m_bDrawPending && ((dwTimeTmp - m_dwLastRenderTime) >= m_dwRenderInterval))
	{
		SDL_Renderer *pRenderer = m_Window.GetRenderer();
		if ((pRenderer != NULL)
#if defined(__EMSCRIPTEN__)
			|| TRUE
#endif
			)
		{
#if defined(__EMSCRIPTEN__)
			if (s_bLoggedDraw == FALSE) {
				SDL_Log("RunFrame: drawing frame pending=%d", m_bDrawPending);
				s_bLoggedDraw = TRUE;
				if (m_Window.GetSDLWindow() != NULL) {
					std::string strTitle = SDL_GetWindowTitle(m_Window.GetSDLWindow());
					if (strTitle.find("[DRAW]") == std::string::npos) {
						strTitle += " [DRAW]";
						SDL_SetWindowTitle(m_Window.GetSDLWindow(), strTitle.c_str());
					}
				}
			}
#endif
			if (pRenderer != NULL) {
				SDL_RenderClear(pRenderer);
			}
			SBOP2_DebugCountOnDraw((pRenderer != NULL) ? 1 : 0);
			SBOP2_DebugMarkStage(6, (pRenderer != NULL) ? 1 : 0, 224, 32, 32);
			m_pHost->OnDraw(pRenderer);
			// ImGui描画
			if (m_bImGuiInitialized && pRenderer != NULL) {
				ImGui_ImplSDLRenderer2_NewFrame();
				ImGui_ImplSDL2_NewFrame();
				ImGui::NewFrame();
				m_pHost->OnDrawImGui();
				ImGui::Render();
				ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), pRenderer);
			}
			// RenderPresent（MgrDraw::Drawから移動）
			if (pRenderer != NULL) {
				SDL_RenderPresent(pRenderer);
			}
			m_byFps++;
		}
		m_dwLastRenderTime = dwTimeTmp;
		m_bDrawPending = FALSE;
	}

	if (dwTimeTmp - m_dwTimeStart >= 1000)
	{
		m_byFps = 0;
		m_dwTimeStart = dwTimeTmp;
	}

Exit:
	if (m_bQuit && !m_bDestroyCalled) {
		if (m_pHost) {
			m_pHost->OnSDLDestroy();
		}
		m_bDestroyCalled = TRUE;
#if defined(__EMSCRIPTEN__)
		emscripten_cancel_main_loop();
#endif
	}
}

#if defined(__EMSCRIPTEN__)
void CSDLApp::RequestBrowserRedraw(void)
{
	m_bDrawPending = TRUE;
	RunFrame();
}
#endif

#if defined(__EMSCRIPTEN__)
void CSDLApp::MainLoopThunk(void *pArg)
{
	CSDLApp *pApp;

	pApp = (CSDLApp *)pArg;
	if (pApp == NULL) {
		return;
	}
	pApp->RunFrame();
}
#endif

void CSDLApp::PollWin32Messages(void)
{
#if defined(__EMSCRIPTEN__)
	return;
#else
	MSG msg;
	HWND hMainWnd;

	hMainWnd = GetMainWindowHandle(m_Window.GetSDLWindow());

	// SDL 管理済みのメインウィンドウ入力はここで捨てる。
	// それ以外の独自メッセージや子ウィンドウ通知は Win32 として流す。
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (ShouldBridgeWin32QuitMessage(hMainWnd, msg)) {
			PushSDLQuitEvent();
			if (msg.message == WM_QUIT) {
				break;
			}
			continue;
		}

		if (ShouldSkipMainWindowInputMessage(hMainWnd, msg)) {
			continue;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#endif
}
