/// @file ImGuiSubWindow.h
/// @brief 独立 SDL_Window + 専用 ImGuiContext を持つサブウィンドウラッパー
/// @date 2026/04/27
/// @copyright Copyright(C)URARA-works 2026
/// @details ネイティブ版専用 (!defined(__EMSCRIPTEN__))。
///          ImGui サブウィンドウを独立した OS ウィンドウとして表示するために使用する。

#pragma once

#if !defined(__EMSCRIPTEN__)

#define SDL_MAIN_HANDLED
#include <SDL.h>

struct ImGuiContext;

class CImGuiSubWindow
{
public:
            CImGuiSubWindow();
    virtual ~CImGuiSubWindow();

    // SDL ウィンドウ + Renderer + ImGuiContext を生成する
    // pszTitle: UTF-8 タイトル文字列
    BOOL    Create(const char *pszTitle, int nWidth, int nHeight);

    // 破棄する
    void    Destroy();

    // SDL イベントを処理する。自分の窓宛てなら TRUE を返す
    BOOL    ProcessEvent(const SDL_Event &ev);

    // フレーム描画開始（RenderClear + NewFrame 一式）
    void    BeginFrame();

    // フレーム描画終了（Render + RenderDrawData + Present）
    void    EndFrame();

    // ウィンドウ ID を返す
    Uint32  GetWindowID() const;

    // 閉じるボタンが押されたか
    BOOL    IsCloseRequested() const { return m_bCloseRequested; }

    // 表示・非表示
    void    Hide();
    void    Show();

    // テキスト入力ポンプ: このウィンドウにフォーカスがあり WantTextInput なら
    // SDL_StartTextInput を呼ぶ。フォーカスがある場合 TRUE を返す。
    BOOL    PumpTextInput(SDL_Window *pFocusWindow);

    BOOL    IsCreated() const { return (m_pWindow != NULL); }

private:
    SDL_Window      *m_pWindow;
    SDL_Renderer    *m_pRenderer;
    ImGuiContext    *m_pCtx;
    BOOL             m_bCloseRequested;
};

#endif // !defined(__EMSCRIPTEN__)
