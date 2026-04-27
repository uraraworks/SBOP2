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
#include <emscripten/em_asm.h>
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

#if defined(__EMSCRIPTEN__)
// DOM 上の FPS 表示を更新する（秒間呼出し回数・1秒内最大時間・レイヤー・Swizzle・Present の各区間 ms を含む）
EM_JS(void, updateFpsDisplay,
      (int fps, int mlps, int ofps, int maxRf, int maxDraw, int layerMs, int swizzleMs, int presentMs), {
    var el = document.getElementById('fpsDisplay');
    if (el) {
        el.textContent = 'FPS:' + fps +
            ' ML/s:' + mlps +
            ' OF/s:' + ofps +
            ' MaxRF:' + maxRf + 'ms' +
            ' MaxD:' + maxDraw + 'ms' +
            '(L:' + layerMs + ' S:' + swizzleMs + ' P:' + presentMs + ')';
    }
});
#endif


// Constants

// Limit how many fixed updates run back-to-back when the client stalls.
#define MAX_FRAME_SKIP	5

#if defined(__EMSCRIPTEN__)
static CSDLApp *g_pBrowserSDLApp = NULL;

extern "C" EMSCRIPTEN_KEEPALIVE void SBOP2_BrowserPumpSingleFrame(void)
{
	if (g_pBrowserSDLApp == NULL) {
		return;
	}
	g_pBrowserSDLApp->RequestBrowserRedraw();
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

// グローバル FPS カウンタ（ImGuiDbg から参照）
BYTE g_byFpsLast = 0;
DWORD g_dwDrawTimeLast = 0;
DWORD g_dwDrawLayerMs = 0;    // レイヤー合成時間（ms）
DWORD g_dwDrawSwizzleMs = 0;  // BGRA→RGBA 変換時間（ms）
DWORD g_dwDrawPresentMs = 0;  // Present 時間（ms）
DWORD g_dwMainLoopCallsPerSec = 0;  // MainLoopThunk の秒間呼出し回数
DWORD g_dwOnFrameCallsPerSec = 0;   // OnFrame (ゲーム更新) の秒間呼出し回数
DWORD g_dwMaxRunFrameMs = 0;        // 1秒内の最大 RunFrame 時間
DWORD g_dwMaxDrawMs = 0;            // 1秒内の最大 OnDraw 時間

CSDLApp::CSDLApp()
{
	m_bInitialized = FALSE;
	m_pHost = NULL;
	m_byFps = 0;
	m_byFpsLast = 0;
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
#if !defined(__EMSCRIPTEN__)
	m_pSubDbg = NULL;
	m_pSubLog = NULL;
#endif
	m_dwMainLoopCallCount = 0;
	m_dwOnFrameCallCount = 0;
	m_dwMaxRunFrameThisSec = 0;
	m_dwMaxDrawThisSec = 0;
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

#ifdef __EMSCRIPTEN__
	// ブラウザ版で SDL2 が window レベルでキーイベントを preventDefault するのを抑止する。
	// canvas にフォーカスがあるときだけキャプチャするように制限し、
	// DOM の <input> 要素 (ログイン入力欄等) にキー入力が届くようにする。
	SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT, "#canvas");
#endif

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
#if !defined(__EMSCRIPTEN__)
	if (m_pSubDbg != NULL) {
		m_pSubDbg->Destroy();
		delete m_pSubDbg;
		m_pSubDbg = NULL;
	}
	if (m_pSubLog != NULL) {
		m_pSubLog->Destroy();
		delete m_pSubLog;
		m_pSubLog = NULL;
	}
#endif
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
#endif
	if (!m_Window.CreateRenderer()) {
#if !defined(__EMSCRIPTEN__)
		return -1;
#endif
	}
	if (!pHost->OnSDLInit(m_Window.GetSDLWindow())) {
		return -1;
	}

	InitImGui(m_Window.GetRenderer());

#if !defined(__EMSCRIPTEN__)
	// サブウィンドウ生成（デバッグ / メッセージログ）
	m_pSubDbg = new CImGuiSubWindow();
	if (!m_pSubDbg->Create(u8"デバッグ", 320, 200)) {
		delete m_pSubDbg;
		m_pSubDbg = NULL;
	}
	m_pSubLog = new CImGuiSubWindow();
	if (!m_pSubLog->Create(u8"メッセージログ", 480, 360)) {
		delete m_pSubLog;
		m_pSubLog = NULL;
	}
	// メイン ImGui コンテキストに戻す
	if (m_bImGuiInitialized) {
		ImGui::SetCurrentContext(ImGui::GetCurrentContext());
	}
#endif

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
		// SDL_RenderClear は呼ばない。
		// canvas 2D は MgrDraw::Draw が putImageData で直接描画するため、
		// SDL_RenderClear で黒クリアすると canvas が上書きされてしまう。
		m_pHost->OnDraw(pRenderer);
	}
#endif

#if defined(__EMSCRIPTEN__)
	// fps=0 を渡すと emscripten は requestAnimationFrame 駆動になり、
	// ブラウザの vsync に同期するためフレーム間隔のジッターが減って体感が軽くなる。
	// 論理更新は m_dwAccumulated 方式で GAME_UPDATE_FPS に固定されるため、
	// モニタのリフレッシュレートに関係なくゲーム速度は一定。
	emscripten_set_main_loop_arg(&CSDLApp::MainLoopThunk, this, 0, 1);
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

	while (SDL_PollEvent(&sdlEvent))
	{
#if !defined(__EMSCRIPTEN__)
		// サブウィンドウ宛てイベントはサブ窓側で処理し、メイン窓・ゲーム側には渡さない
		{
			BOOL bHandledBySub = FALSE;
			if (m_pSubDbg != NULL && m_pSubDbg->IsCreated()) {
				if (m_pSubDbg->ProcessEvent(sdlEvent)) {
					bHandledBySub = TRUE;
				}
			}
			if (!bHandledBySub && m_pSubLog != NULL && m_pSubLog->IsCreated()) {
				if (m_pSubLog->ProcessEvent(sdlEvent)) {
					bHandledBySub = TRUE;
				}
			}
			if (bHandledBySub) {
				// SDL_QUIT はメイン窓にも通知する
				if (sdlEvent.type == SDL_QUIT) {
					m_bQuit = TRUE;
				}
				continue;
			}
		}
#endif // !defined(__EMSCRIPTEN__)

		if (m_bImGuiInitialized) {
			ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
		}
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			m_bQuit = TRUE;
			break;

		case SDL_KEYDOWN:
			// サブ窓宛てキーイベントはゲーム側に渡さない（上で continue 済み）
			// ImGui がキーボードをキャプチャ中の場合もゲーム側に渡さない
			if (!m_bImGuiInitialized || !ImGui::GetIO().WantCaptureKeyboard) {
				m_pHost->OnSDLKeyDown(CSDLInput::ScancodeToVK(sdlEvent.key.keysym.scancode));
			}
			m_bDrawPending = TRUE;
			break;

		case SDL_KEYUP:
			// ImGui がキーボードをキャプチャ中の場合はゲーム側に渡さない
			if (!m_bImGuiInitialized || !ImGui::GetIO().WantCaptureKeyboard) {
				m_pHost->OnSDLKeyUp(CSDLInput::ScancodeToVK(sdlEvent.key.keysym.scancode));
			}
			m_bDrawPending = TRUE;
			break;

		case SDL_TEXTINPUT:
			// ImGui がキーボードをキャプチャ中の場合はゲーム側に渡さない
			if (!m_bImGuiInitialized || !ImGui::GetIO().WantCaptureKeyboard) {
				m_pHost->OnSDLTextInput(sdlEvent.text.text);
			}
			m_bDrawPending = TRUE;
			break;

		case SDL_WINDOWEVENT:
			switch (sdlEvent.window.event)
			{
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_RESTORED:
				m_pHost->OnSDLFocusChanged(TRUE);
				m_bDrawPending = TRUE;
				break;

			case SDL_WINDOWEVENT_FOCUS_LOST:
			case SDL_WINDOWEVENT_MINIMIZED:
				m_pHost->OnSDLFocusChanged(FALSE);
				m_bDrawPending = TRUE;
				break;

			default:
				break;
			}
			break;

		case SDL_MOUSEMOTION:
			m_pHost->OnSDLMouseMove(sdlEvent.motion.x, sdlEvent.motion.y);
			m_bDrawPending = TRUE;
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
			m_bDrawPending = TRUE;
			break;

		default:
			break;
		}
	}

	if (m_bQuit) {
		goto Exit;
	}

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
		m_dwAccumulated += dwElapsed;
	}

	{
		DWORD dwFrameSkip = 0;
		while ((m_dwAccumulated >= m_dwUpdateInterval) && (dwFrameSkip < MAX_FRAME_SKIP))
		{
			m_dwAccumulated -= m_dwUpdateInterval;
			m_dwOnFrameCallCount++;  // 秒間 OnFrame 呼出し回数カウント
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
			// Emscripten版: canvas 2D は MgrDraw::Draw が putImageData で直接描画するため、
			// SDL_RenderClear / ImGui描画 / SDL_RenderPresent はすべてスキップする。
			{
				DWORD dwDrawStart = SDL_GetTicks();
				m_pHost->OnDraw(pRenderer);
				g_dwDrawTimeLast = SDL_GetTicks() - dwDrawStart;
				if (g_dwDrawTimeLast > m_dwMaxDrawThisSec) {
					m_dwMaxDrawThisSec = g_dwDrawTimeLast;
				}
			}
#else
			if (pRenderer != NULL) {
				SDL_RenderClear(pRenderer);
			}
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
			// サブウィンドウ描画
			if (m_pSubDbg != NULL && m_pSubDbg->IsCreated()) {
				m_pSubDbg->BeginFrame();
				m_pHost->OnDrawImGuiSub(IGameLoopHost::IMGUI_SUBWINDOW_DEBUG);
				m_pSubDbg->EndFrame();
			}
			if (m_pSubLog != NULL && m_pSubLog->IsCreated()) {
				m_pSubLog->BeginFrame();
				m_pHost->OnDrawImGuiSub(IGameLoopHost::IMGUI_SUBWINDOW_LOG);
				m_pSubLog->EndFrame();
			}
			// サブ窓描画後にメイン ImGui コンテキストを復元
			if (m_bImGuiInitialized) {
				// 次フレームの ImGui NewFrame のためにコンテキストを保持（何もしなくても OK）
			}
#endif
			m_byFps++;
		}
		m_dwLastRenderTime = dwTimeTmp;
		m_bDrawPending = FALSE;
	}

	if (dwTimeTmp - m_dwTimeStart >= 1000)
	{
		m_byFpsLast = m_byFps;
		g_byFpsLast = m_byFpsLast;
		g_dwMainLoopCallsPerSec = m_dwMainLoopCallCount;
		g_dwOnFrameCallsPerSec = m_dwOnFrameCallCount;
		g_dwMaxRunFrameMs = m_dwMaxRunFrameThisSec;
		g_dwMaxDrawMs = m_dwMaxDrawThisSec;
		m_dwMainLoopCallCount = 0;
		m_dwOnFrameCallCount = 0;
		m_dwMaxRunFrameThisSec = 0;
		m_dwMaxDrawThisSec = 0;
#if defined(__EMSCRIPTEN__)
		updateFpsDisplay((int)m_byFpsLast,
		                 (int)g_dwMainLoopCallsPerSec,
		                 (int)g_dwOnFrameCallsPerSec,
		                 (int)g_dwMaxRunFrameMs,
		                 (int)g_dwMaxDrawMs,
		                 (int)g_dwDrawLayerMs,
		                 (int)g_dwDrawSwizzleMs,
		                 (int)g_dwDrawPresentMs);
#endif
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
	CSDLApp *pApp = (CSDLApp *)pArg;
	if (pApp == NULL) {
		return;
	}
	// Click to Start 前はフレーム進行を止める。
	// 一度 true になったら以後は JS へ問い合わせない。
	static bool s_bUserActivated = false;
	if (!s_bUserActivated) {
		int nActivated = EM_ASM_INT({
			return (Module && Module.sbop2UserActivated) ? 1 : 0;
		});
		if (nActivated == 0) {
			return;
		}
		s_bUserActivated = true;
	}
	pApp->m_dwMainLoopCallCount++;
	DWORD dwStart = SDL_GetTicks();
	pApp->RunFrame();
	DWORD dwElapsed = SDL_GetTicks() - dwStart;
	if (dwElapsed > pApp->m_dwMaxRunFrameThisSec) {
		pApp->m_dwMaxRunFrameThisSec = dwElapsed;
	}
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
