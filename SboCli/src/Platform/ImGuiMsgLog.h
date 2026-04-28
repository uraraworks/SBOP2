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

#if !defined(__EMSCRIPTEN__)
    // SDL_TEXTINPUT 受信時に呼ぶ。UTF-8 文字列を末尾に追加する
    void OnTextInput(const char *pszUtf8);

    // SDL_KEYDOWN 受信時に呼ぶ。Enter で送信、Backspace で1文字削除
    void OnKeyDown(int sdlKeycode);

    // 入力受付状態を設定（ログ窓フォーカス時 true）
    void SetAcceptInput(bool b) { m_bAcceptInput = b; }
#endif

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
#if !defined(__EMSCRIPTEN__)
    int  m_chatLen;         // 現在の入力長（バイト数）
    bool m_bAcceptInput;    // このログ窓が現在フォーカスを持ち入力を受け付けるか
#endif
    static const int MAX_LINES = 3000;
};
