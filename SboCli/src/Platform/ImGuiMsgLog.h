/// @file ImGuiMsgLog.h
/// @brief ImGui版メッセージログ

#pragma once

#include <vector>
#include <string>

class CMgrData;

class CImGuiMsgLog
{
public:
    CImGuiMsgLog();
    ~CImGuiMsgLog();

    void Init(CMgrData *pMgrData);
    void Add(const char *pszLog, unsigned int color);  // color = COLORREF (0x00BBGGRR)
    void Draw();  // ImGuiウィジェット描画（毎フレーム呼ぶ）

private:
    struct LogLine {
        std::string text;
        unsigned int color;  // COLORREF
    };

    CMgrData *m_pMgrData;
    std::vector<LogLine> m_lines;
    bool m_bScrollToBottom;
    char m_chatBuf[256];
    bool m_bVisible;
    static const int MAX_LINES = 3000;
};
