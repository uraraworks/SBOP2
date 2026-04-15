/// @file ImGuiMsgLog.cpp
/// @brief ImGui版メッセージログ 実装

#include "stdafx.h"
#include <imgui.h>
#if defined(_WIN32)
#include <windows.h>
#endif
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
#if defined(_WIN32)
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
#else
    // ブラウザ版：サーバは UTF-8 を直接受け付けるため変換不要
    return std::string(pszUtf8);
#endif
}

/// @brief SJIS (CP932) 文字列を UTF-8 に変換する
/// @param pszSjis SJIS 文字列（NULL 終端）
/// @return UTF-8 std::string。変換失敗時は空文字列を返す
static std::string SjisToUtf8(const char *pszSjis)
{
    if (pszSjis == NULL || pszSjis[0] == '\0') {
        return std::string();
    }
#if defined(_WIN32)
    // SJIS → UTF-16
    int nWideLen = MultiByteToWideChar(CP_ACP, 0, pszSjis, -1, NULL, 0);
    if (nWideLen <= 0) {
        return std::string(pszSjis);  // 変換失敗時はそのまま返す
    }
    std::wstring wstr(nWideLen, L'\0');
    MultiByteToWideChar(CP_ACP, 0, pszSjis, -1, &wstr[0], nWideLen);

    // UTF-16 → UTF-8
    int nUtf8Len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (nUtf8Len <= 0) {
        return std::string(pszSjis);  // 変換失敗時はそのまま返す
    }
    std::string utf8(nUtf8Len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8[0], nUtf8Len, NULL, NULL);

    // WideCharToMultiByte は NULL 終端を含む長さを返すので末尾の '\0' を除去
    if (!utf8.empty() && utf8.back() == '\0') {
        utf8.pop_back();
    }

    return utf8;
#else
    // ブラウザ版：受信データはすでに UTF-8 のため変換不要
    return std::string(pszSjis);
#endif
}

CImGuiMsgLog::CImGuiMsgLog()
    : m_pMgrData(NULL)
    , m_bScrollToBottom(false)
    , m_bVisible(true)
    , m_bFocusInput(false)
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
    if (!m_bVisible) {
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin(u8"メッセージログ", &m_bVisible)) {
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

    // 次フレームでフォーカスを戻す必要がある場合はここで予約する
    if (m_bFocusInput) {
        ImGui::SetKeyboardFocusHere(0);
        m_bFocusInput = false;
    }

    // FrameBg/Hovered/Active と Text を明示的に設定して視認性を確保する
    // デフォルトのダークテーマでは FrameBg がほぼ黒になるため、
    // 入力欄の背景と文字色を明示的に上書きする
    ImGui::PushStyleColor(ImGuiCol_FrameBg,        ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.30f, 0.30f, 0.30f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  ImVec4(0.35f, 0.35f, 0.35f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text,           ImVec4(1.0f,  1.0f,  1.0f,  1.0f));

    // 入力欄を幅いっぱいに広げる
    ImGui::SetNextItemWidth(-1.0f);

    ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
    bool bEntered = ImGui::InputText(u8"##chat", m_chatBuf, sizeof(m_chatBuf), flags);
    ImGui::PopStyleColor(4);

    if (bEntered) {
        if (m_chatBuf[0] != '\0' && m_pMgrData != NULL) {
            CMainFrame *pMainFrame = m_pMgrData->GetMainFrame();
            if (pMainFrame != NULL) {
                // m_chatBuf は ImGui が管理する UTF-8 文字列。
                // SendChat / サーバは SJIS (CP932) を期待するため、
                // ここで UTF-8 → SJIS 変換してから渡す。
                std::string sjisMsg = Utf8ToSjis(m_chatBuf);
                pMainFrame->SendChat(0, sjisMsg.c_str(), NULL);
            }
        }
        m_chatBuf[0] = '\0';
        // 次フレームの InputText 描画前にフォーカスを予約する
        // （InputText 後の SetKeyboardFocusHere(-1) は確実でないため）
        m_bFocusInput = true;
    }

    ImGui::End();
}
