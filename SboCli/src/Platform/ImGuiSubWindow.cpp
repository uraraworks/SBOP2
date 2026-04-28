/// @file ImGuiSubWindow.cpp
/// @brief 独立 SDL_Window + 専用 ImGuiContext を持つサブウィンドウラッパー 実装
/// @date 2026/04/27
/// @copyright Copyright(C)URARA-works 2026

#include "stdafx.h"

#if !defined(__EMSCRIPTEN__)

#include <cfloat>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include "ImGuiSubWindow.h"

CImGuiSubWindow::CImGuiSubWindow()
    : m_pWindow(NULL)
    , m_pRenderer(NULL)
    , m_pCtx(NULL)
    , m_bCloseRequested(FALSE)
{
}

CImGuiSubWindow::~CImGuiSubWindow()
{
    Destroy();
}

BOOL CImGuiSubWindow::Create(const char *pszTitle, int nWidth, int nHeight)
{
    if (m_pWindow != NULL) {
        return TRUE;  // すでに生成済み
    }

    // SDL_Window 生成（リサイズ可能）
    m_pWindow = SDL_CreateWindow(
        pszTitle,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        nWidth,
        nHeight,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if (m_pWindow == NULL) {
        return FALSE;
    }

    // SDL_Renderer 生成
    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_pRenderer == NULL) {
        m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_SOFTWARE);
        if (m_pRenderer == NULL) {
            SDL_DestroyWindow(m_pWindow);
            m_pWindow = NULL;
            return FALSE;
        }
    }

    // ImGui Context 生成
    m_pCtx = ImGui::CreateContext();
    if (m_pCtx == NULL) {
        SDL_DestroyRenderer(m_pRenderer);
        m_pRenderer = NULL;
        SDL_DestroyWindow(m_pWindow);
        m_pWindow = NULL;
        return FALSE;
    }

    ImGui::SetCurrentContext(m_pCtx);

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = NULL;  // imgui.ini 保存を無効化

    // 日本語フォントのロード（文字化け対策）
    {
        ImFontConfig fontConfig;
        fontConfig.MergeMode = false;
        ImFont *pFont = io.Fonts->AddFontFromFileTTF(
            "font/NotoSansCJKjp-Regular.otf",
            16.0f,
            &fontConfig,
            io.Fonts->GetGlyphRangesJapanese()
        );
        if (pFont == NULL) {
            io.Fonts->AddFontDefault();
        }
    }

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(m_pWindow, m_pRenderer);
    ImGui_ImplSDLRenderer2_Init(m_pRenderer);

    return TRUE;
}

void CImGuiSubWindow::Destroy()
{
    if (m_pCtx != NULL) {
        ImGui::SetCurrentContext(m_pCtx);
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext(m_pCtx);
        m_pCtx = NULL;
    }
    if (m_pRenderer != NULL) {
        SDL_DestroyRenderer(m_pRenderer);
        m_pRenderer = NULL;
    }
    if (m_pWindow != NULL) {
        SDL_DestroyWindow(m_pWindow);
        m_pWindow = NULL;
    }
    m_bCloseRequested = FALSE;
}

BOOL CImGuiSubWindow::ProcessEvent(const SDL_Event &ev)
{
    if (m_pWindow == NULL || m_pCtx == NULL) {
        return FALSE;
    }

    Uint32 myWindowID = SDL_GetWindowID(m_pWindow);

    // イベントが自分の窓宛てか判定する
    Uint32 evWindowID = 0;
    switch (ev.type)
    {
    case SDL_WINDOWEVENT:
        evWindowID = ev.window.windowID;
        break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        evWindowID = ev.key.windowID;
        break;
    case SDL_TEXTINPUT:
        evWindowID = ev.text.windowID;
        break;
    case SDL_TEXTEDITING:
        evWindowID = ev.edit.windowID;
        break;
    case SDL_MOUSEMOTION:
        evWindowID = ev.motion.windowID;
        break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        evWindowID = ev.button.windowID;
        break;
    case SDL_MOUSEWHEEL:
        evWindowID = ev.wheel.windowID;
        break;
    default:
        return FALSE;
    }

    // windowID 補正
    // マウス: SDL_CaptureMouse 等の影響で windowID がメイン窓のままになることがある→
    //         SDL_GetMouseFocus() がこのサブ窓ならサブ窓宛てとみなす
    // キー/テキスト入力: 同様に windowID が 0 や別窓 ID で来る場合に備え、
    //         SDL_GetKeyboardFocus() がこのサブ窓ならサブ窓宛てとみなす
    //         (これがないとログ窓フォーカス中のキー入力がメイン窓のゲーム側に流れる)
    if (evWindowID != myWindowID) {
        bool bMouseEvent = (ev.type == SDL_MOUSEMOTION ||
                            ev.type == SDL_MOUSEBUTTONDOWN ||
                            ev.type == SDL_MOUSEBUTTONUP ||
                            ev.type == SDL_MOUSEWHEEL);
        bool bKeyEvent = (ev.type == SDL_KEYDOWN ||
                          ev.type == SDL_KEYUP ||
                          ev.type == SDL_TEXTINPUT ||
                          ev.type == SDL_TEXTEDITING);
        if (bMouseEvent && SDL_GetMouseFocus() == m_pWindow) {
            // マウスフォーカスはこの窓にある→サブ窓宛てとみなして処理続行
        } else if (bKeyEvent && SDL_GetKeyboardFocus() == m_pWindow) {
            // キーボードフォーカスはこの窓にある→サブ窓宛てとみなして処理続行
        } else {
            return FALSE;
        }
    }

    // 閉じるボタン検出
    if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_CLOSE) {
        m_bCloseRequested = TRUE;
        Hide();
        return TRUE;
    }

    // ImGui にイベントを渡す
    ImGui::SetCurrentContext(m_pCtx);
    ImGui_ImplSDL2_ProcessEvent(&ev);
    return TRUE;
}

void CImGuiSubWindow::BeginFrame()
{
    if (m_pWindow == NULL || m_pCtx == NULL || m_pRenderer == NULL) {
        return;
    }

    SDL_SetRenderDrawColor(m_pRenderer, 30, 30, 30, 255);
    SDL_RenderClear(m_pRenderer);

    ImGui::SetCurrentContext(m_pCtx);
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    // 対策2: impl_sdl2 のマウス位置更新が multi-context 構成で不安定なため、
    // SDL_GetGlobalMouseState からサブ窓相対座標を手動注入する。
    // キーボードフォーカス条件を外し、マウスがサブ窓矩形内なら正しい座標を、
    // 矩形外なら -FLT_MAX を注入することで hover 判定を常に正確にする。
    {
        int gx = 0, gy = 0;
        int wx = 0, wy = 0;
        int ww = 0, wh = 0;
        SDL_GetGlobalMouseState(&gx, &gy);
        SDL_GetWindowPosition(m_pWindow, &wx, &wy);
        SDL_GetWindowSize(m_pWindow, &ww, &wh);
        int relX = gx - wx;
        int relY = gy - wy;
        if (relX >= 0 && relX < ww && relY >= 0 && relY < wh) {
            ImGui::GetIO().AddMousePosEvent((float)relX, (float)relY);
        } else {
            ImGui::GetIO().AddMousePosEvent(-FLT_MAX, -FLT_MAX);
        }
    }

    ImGui::NewFrame();
}

void CImGuiSubWindow::EndFrame()
{
    if (m_pWindow == NULL || m_pCtx == NULL || m_pRenderer == NULL) {
        return;
    }

    ImGui::SetCurrentContext(m_pCtx);
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_pRenderer);
    SDL_RenderPresent(m_pRenderer);
}

Uint32 CImGuiSubWindow::GetWindowID() const
{
    if (m_pWindow == NULL) {
        return 0;
    }
    return SDL_GetWindowID(m_pWindow);
}

void CImGuiSubWindow::Hide()
{
    if (m_pWindow != NULL) {
        SDL_HideWindow(m_pWindow);
    }
}

void CImGuiSubWindow::Show()
{
    if (m_pWindow != NULL) {
        SDL_ShowWindow(m_pWindow);
    }
}

BOOL CImGuiSubWindow::PumpTextInput(SDL_Window *pFocusWindow)
{
    if (m_pWindow == NULL || m_pCtx == NULL) {
        return FALSE;
    }
    // SDL_GetKeyboardFocus() がこのサブ窓を返す場合にフォーカスありと判定する。
    // pFocusWindow が NULL の場合（フォーカス窓なし）も SDL_GetKeyboardFocus で再確認する。
    SDL_Window *pActualFocus = (pFocusWindow != NULL) ? pFocusWindow : SDL_GetKeyboardFocus();
    if (pActualFocus != m_pWindow) {
        return FALSE;
    }
    // このサブ窓がフォーカスを持っている: ログ窓は自前入力実装なので
    // WantTextInput に依存せず無条件で SDL_StartTextInput を維持する
    if (!SDL_IsTextInputActive()) {
        SDL_StartTextInput();
    }
    return TRUE;
}

#endif // !defined(__EMSCRIPTEN__)
