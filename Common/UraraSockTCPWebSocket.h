/// @file UraraSockTCPWebSocket.h
/// @brief WebSocket版TCP通信クラス（Emscripten用）
#pragma once
// このヘッダが直接インクルードされたことをUraraSockTCP.hに知らせるマクロ
#define URARASOCKTCP_WEBSOCKET_INCLUDED

#include "UraraSockTCP.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/websocket.h>
#include <vector>
#include <deque>
#include <mutex>

/// @brief Emscripten WebSocket による CUraraSockTCP 実装
class CUraraSockTCPWebSocket : public CUraraSockTCP
{
public:
    CUraraSockTCPWebSocket();
    virtual ~CUraraSockTCPWebSocket();

    // CUraraSockTCP インターフェース実装
    virtual void    DeleteRecvData(PBYTE pData) override;
    virtual void    Destroy(void) override;
    virtual BOOL    Host(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, DWORD dwCount) override;
    virtual BOOL    Connect(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, LPCSTR pszAddr) override;
    virtual void    DeleteClient(DWORD dwID) override;
    virtual void    SendCancel(DWORD dwID = 0) override;
    virtual void    Send(PBYTE pData, DWORD dwSize, BYTE byPriority = URARASOCK_SENDPRIORITY_MIDDLE) override;
    virtual void    SendTo(DWORD dwID, PBYTE pData, DWORD dwSize, BYTE byPriority = URARASOCK_SENDPRIORITY_MIDDLE) override;
    virtual DWORD   GetThrowghPutSend(DWORD dwID) override;
    virtual DWORD   GetThrowghPutRecv(DWORD dwID) override;
    virtual DWORD   GetQueCount(DWORD dwID) override;
    virtual DWORD   GetIPAddress(DWORD dwID) override;

    // コールバック設定（CUraraSockTCPSBOから呼ばれる）
    void SetNotifySink(PFURARASOCKNOTIFY pfNotify, void* pUserData);

private:
    // WebSocketイベントハンドラ（static）
    static EM_BOOL OnOpen(int eventType, const EmscriptenWebSocketOpenEvent* event, void* userData);
    static EM_BOOL OnMessage(int eventType, const EmscriptenWebSocketMessageEvent* event, void* userData);
    static EM_BOOL OnClose(int eventType, const EmscriptenWebSocketCloseEvent* event, void* userData);
    static EM_BOOL OnError(int eventType, const EmscriptenWebSocketErrorEvent* event, void* userData);

    // 親への通知
    void NotifyParent(UINT uMsgOffset, WPARAM wParam, LPARAM lParam);

    EMSCRIPTEN_WEBSOCKET_T  m_socket;           // WebSocketハンドル
    BOOL                    m_bConnected;       // 接続済みフラグ
    PFURARASOCKNOTIFY       m_pfNotify;         // 通知コールバック
    void*                   m_pNotifyUserData;  // コールバックのユーザーデータ
    DWORD                   m_dwPreCheckKey;    // プリチェックキー
};

// GetUraraSockTCP() をここで定義（CUraraSockTCPWebSocket の完全定義後）
inline CUraraSockTCP *GetUraraSockTCP(void) { return new CUraraSockTCPWebSocket(); }

#endif // __EMSCRIPTEN__
