/// @file ImGuiMsgLog.cpp
/// @brief ImGui版メッセージログ 実装

#include "stdafx.h"
#include <imgui.h>
#include <string>
#include "ImGuiMsgLog.h"
#include "MgrData.h"
#include "MainFrame.h"

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
}

void CImGuiMsgLog::Draw()
{
#if !defined(__EMSCRIPTEN__)
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
#else
    // ブラウザ版: 従来の浮動サブウィンドウ
    if (!m_bVisible) {
        return;
    }
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(u8"メッセージログ", &m_bVisible)) {
        ImGui::End();
        return;
    }
#endif

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

#if !defined(__EMSCRIPTEN__)
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
#else
    // ブラウザ版: 従来の ImGui InputText のまま（動作しているので変更しない）
    {
        // ブラウザ版: 初回表示時とフォーカス要求時のみ SetKeyboardFocusHere
        static bool s_bFocusBrowser = true;
        if (s_bFocusBrowser || ImGui::IsWindowAppearing()) {
            ImGui::SetKeyboardFocusHere(0);
            s_bFocusBrowser = false;
        }

        ImGui::PushStyleColor(ImGuiCol_FrameBg,        ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.30f, 0.30f, 0.30f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  ImVec4(0.35f, 0.35f, 0.35f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text,           ImVec4(1.0f,  1.0f,  1.0f,  1.0f));

        ImGui::SetNextItemWidth(-1.0f);
        ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_EnterReturnsTrue;
        bool bEntered = ImGui::InputText(u8"##chat", m_chatBuf, sizeof(m_chatBuf), inputFlags);
        ImGui::PopStyleColor(4);

        if (bEntered) {
            if (m_chatBuf[0] != '\0' && m_pMgrData != NULL) {
                CMainFrame *pMainFrame = m_pMgrData->GetMainFrame();
                if (pMainFrame != NULL) {
                    std::string sjisMsg = Utf8ToSjis(m_chatBuf);
                    pMainFrame->SendChat(0, sjisMsg.c_str(), NULL);
                }
            }
            m_chatBuf[0] = '\0';
            s_bFocusBrowser = true;
        }
    }
#endif

    ImGui::End();
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
