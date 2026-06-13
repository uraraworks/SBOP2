/// @file ImGuiMsgLog.cpp
/// @brief ImGui版メッセージログ 実装

#include "StdAfx.h"
#include <imgui.h>
#include <string>
#include "ImGuiMsgLog.h"
#include "MgrData.h"
#include "MainFrame.h"
#if defined(__EMSCRIPTEN__)
#include <emscripten/em_js.h>
#endif

/// @brief UTF-8 文字列を SJIS (CP932) に変換する
/// @param pszUtf8 UTF-8 文字列（NULL 終端）
/// @return SJIS std::string。変換失敗時は元の文字列をそのまま返す
static std::string Utf8ToSjis(const char *pszUtf8)
{
    if (pszUtf8 == NULL || pszUtf8[0] == '\0') {
        return std::string();
    }
    // UTF-8 → UTF-16
    int nWideLen = MultiByteToWideChar(CP_UTF8, 0, pszUtf8, -1, NULL, 0);
    if (nWideLen <= 0) {
        return std::string(pszUtf8);  // 変換失敗時はそのまま返す
    }
    std::wstring wstr(nWideLen, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, pszUtf8, -1, &wstr[0], nWideLen);

    // UTF-16 → SJIS (CP932)
    int nSjisLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (nSjisLen <= 0) {
        return std::string(pszUtf8);  // 変換失敗時はそのまま返す
    }
    std::string sjis(nSjisLen, '\0');
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &sjis[0], nSjisLen, NULL, NULL);

    // WideCharToMultiByte は NULL 終端を含む長さを返すので末尾の '\0' を除去
    if (!sjis.empty() && sjis.back() == '\0') {
        sjis.pop_back();
    }

    return sjis;
}

#if defined(__EMSCRIPTEN__)
/// @brief SJIS メッセージを DOM チャットログへ転送する EM_JS 関数
/// @param text    UTF-8 テキスト (C文字列ポインタ)
/// @param cssColor "#RRGGBB" 形式の CSS 色文字列 (C文字列ポインタ)
/// window.sbop2ChatLogAdd が未定義でも typeof ガードで落ちないようにする
EM_JS(void, sbop2_chat_log_add_js, (const char *text, const char *cssColor), {
    if (typeof window !== 'undefined' && typeof window.sbop2ChatLogAdd === 'function') {
        window.sbop2ChatLogAdd(UTF8ToString(text), UTF8ToString(cssColor));
    }
});
#endif

CImGuiMsgLog::CImGuiMsgLog()
    : m_pMgrData(NULL)
    , m_bScrollToBottom(false)
    , m_bVisible(true)
#if !defined(__EMSCRIPTEN__)
    , m_chatLen(0)
    , m_bAcceptInput(false)
#endif
{
    m_chatBuf[0] = '\0';
}

CImGuiMsgLog::~CImGuiMsgLog()
{
}

void CImGuiMsgLog::Init(CMgrData *pMgrData)
{
    m_pMgrData = pMgrData;
}

void CImGuiMsgLog::Add(const char *pszLog, unsigned int color)
{
    if (pszLog == NULL || pszLog[0] == '\0') {
        return;
    }

#if defined(__EMSCRIPTEN__)
    // ブラウザ版: DOM側 window.sbop2ChatLogAdd へ転送する
    // COLORREF (0x00BBGGRR) → "#RRGGBB" CSS 色文字列に変換
    {
        unsigned int r = (color >>  0) & 0xFF;
        unsigned int g = (color >>  8) & 0xFF;
        unsigned int b = (color >> 16) & 0xFF;
        char szCssColor[8]; // "#RRGGBB\0"
        snprintf(szCssColor, sizeof(szCssColor), "#%02X%02X%02X", r, g, b);

        // SJIS → UTF-8 変換してから JS へ渡す
        std::string utf8Text = SjisToUtf8(pszLog);
        sbop2_chat_log_add_js(utf8Text.c_str(), szCssColor);
    }
#else
    LogLine line;
    // SJIS (CP932) 文字列を UTF-8 に変換してから保持する
    // ImGui は UTF-8 を要求するため、SJIS のまま渡すと文字化けする
    line.text = SjisToUtf8(pszLog);
    line.color = color;
    m_lines.push_back(line);

    // 上限を超えたら古い行を削除
    while ((int)m_lines.size() > MAX_LINES) {
        m_lines.erase(m_lines.begin());
    }

    m_bScrollToBottom = true;
#endif
}

void CImGuiMsgLog::Draw()
{
#if defined(__EMSCRIPTEN__)
    // ブラウザ版: チャットログ・入力は DOM 側(shell.html)が担うため何もしない
    return;
#else
    // ネイティブ版: 独立ウィンドウ全体をパネルで埋める
    ImGuiViewport *vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->WorkPos);
    ImGui::SetNextWindowSize(vp->WorkSize);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
                           | ImGuiWindowFlags_NoMove    | ImGuiWindowFlags_NoCollapse
                           | ImGuiWindowFlags_NoBringToFrontOnFocus;
    if (!ImGui::Begin(u8"メッセージログ##sub", NULL, flags)) {
        ImGui::End();
        return;
    }

    // ログ表示エリア（下部にチャット入力を残す）
    float footerHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    if (ImGui::BeginChild("LogScroll", ImVec2(0, -footerHeight), ImGuiChildFlags_Borders)) {
        for (size_t i = 0; i < m_lines.size(); i++) {
            // COLORREF (0x00BBGGRR) → ImVec4 (R,G,B,A) に変換
            unsigned int cr = m_lines[i].color;
            float r = ((cr >>  0) & 0xFF) / 255.0f;
            float g = ((cr >>  8) & 0xFF) / 255.0f;
            float b = ((cr >> 16) & 0xFF) / 255.0f;
            ImGui::TextColored(ImVec4(r, g, b, 1.0f), "%s", m_lines[i].text.c_str());
        }
        if (m_bScrollToBottom) {
            ImGui::SetScrollHereY(1.0f);
            m_bScrollToBottom = false;
        }
    }
    ImGui::EndChild();

    // チャット入力
    ImGui::Separator();

    // ネイティブ版: SDL_TEXTINPUT / SDL_KEYDOWN で自前バッファを更新する自前実装
    {
        ImVec2 inputSize(ImGui::GetContentRegionAvail().x, ImGui::GetFrameHeight());
        ImVec2 cursorPos = ImGui::GetCursorScreenPos();
        // 入力欄背景矩形
        ImGui::GetWindowDrawList()->AddRectFilled(
            cursorPos,
            ImVec2(cursorPos.x + inputSize.x, cursorPos.y + inputSize.y),
            IM_COL32(64, 64, 64, 255)
        );
        // テキスト描画（内側に少しパディング）
        ImGui::SetCursorScreenPos(ImVec2(cursorPos.x + 4.0f, cursorPos.y + 2.0f));
        if (m_bAcceptInput) {
            // 入力受付中: バッファ内容 + カーソル
            ImGui::TextUnformatted(m_chatBuf);
            ImGui::SameLine(0, 0);
            ImGui::Text("_");
        } else {
            ImGui::TextUnformatted(m_chatBuf);
        }
        // 描画カーソルをフレーム高分進める
        ImGui::SetCursorScreenPos(ImVec2(cursorPos.x, cursorPos.y + inputSize.y + ImGui::GetStyle().ItemSpacing.y));
    }

    ImGui::End();
#endif // __EMSCRIPTEN__
}

#if !defined(__EMSCRIPTEN__)
void CImGuiMsgLog::OnTextInput(const char *pszUtf8)
{
    if (pszUtf8 == NULL || pszUtf8[0] == '\0') {
        return;
    }
    size_t addLen = strlen(pszUtf8);
    if (m_chatLen + (int)addLen < (int)sizeof(m_chatBuf) - 1) {
        memcpy(m_chatBuf + m_chatLen, pszUtf8, addLen);
        m_chatLen += (int)addLen;
        m_chatBuf[m_chatLen] = '\0';
    }
}

void CImGuiMsgLog::OnKeyDown(int keycode)
{
    if (keycode == SDLK_BACKSPACE) {
        // UTF-8 末尾1文字を削除: 継続バイト (0x80-0xBF) を逆方向にスキップしてリードバイトまで削る
        if (m_chatLen > 0) {
            int i = m_chatLen - 1;
            // 継続バイト (10xxxxxx) をスキップ
            while (i > 0 && ((unsigned char)m_chatBuf[i] & 0xC0) == 0x80) {
                i--;
            }
            m_chatLen = i;
            m_chatBuf[m_chatLen] = '\0';
        }
    } else if (keycode == SDLK_RETURN || keycode == SDLK_KP_ENTER) {
        if (m_chatBuf[0] != '\0' && m_pMgrData != NULL) {
            CMainFrame *pMainFrame = m_pMgrData->GetMainFrame();
            if (pMainFrame != NULL) {
                std::string sjisMsg = Utf8ToSjis(m_chatBuf);
                pMainFrame->SendChat(0, sjisMsg.c_str(), NULL);
            }
        }
        m_chatLen = 0;
        m_chatBuf[0] = '\0';
    }
}
#endif
