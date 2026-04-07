/// @file ImGuiMsgLog.cpp
/// @brief ImGui版メッセージログ 実装

#include "stdafx.h"
#include <imgui.h>
#include "ImGuiMsgLog.h"
#include "MgrData.h"
#include "MainFrame.h"

CImGuiMsgLog::CImGuiMsgLog()
    : m_pMgrData(NULL)
    , m_bScrollToBottom(false)
    , m_bVisible(true)
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
    line.text = pszLog;
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
    ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
    if (ImGui::InputText(u8"##chat", m_chatBuf, sizeof(m_chatBuf), flags)) {
        if (m_chatBuf[0] != '\0' && m_pMgrData != NULL) {
            CMainFrame *pMainFrame = m_pMgrData->GetMainFrame();
            if (pMainFrame != NULL) {
                pMainFrame->SendChat(0, m_chatBuf, NULL);
            }
        }
        m_chatBuf[0] = '\0';
        ImGui::SetKeyboardFocusHere(-1);  // 入力欄にフォーカスを戻す
    }

    ImGui::End();
}
