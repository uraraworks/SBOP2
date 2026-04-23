#include "stdafx.h"
#include "AdminWsHub.h"

#include <sstream>
#include <process.h>

#include "WebSocketProtocol.h"
#include "JsonUtils.h"

// ============================================================
//  CAdminWsHub 実装
// ============================================================

CAdminWsHub::CAdminWsHub()
    : m_bShutdown(false)
{
}

CAdminWsHub &CAdminWsHub::Instance()
{
    static CAdminWsHub s_instance;
    return s_instance;
}

// ------------------------------------------------------------
// 接続登録
// ------------------------------------------------------------

void CAdminWsHub::AddConnection(SOCKET hSocket, const std::string &sessionId)
{
    Connection conn;
    conn.hSocket   = hSocket;
    conn.sessionId = sessionId;
    conn.hThread   = NULL;

    // recv ループスレッドを起動
    AdminWsRecvArgs *pArgs = new AdminWsRecvArgs();
    pArgs->pHub    = this;
    pArgs->hSocket = hSocket;

    unsigned uId = 0;
    HANDLE hThread = reinterpret_cast<HANDLE>(
        _beginthreadex(NULL, 0, RecvThreadProc, pArgs, 0, &uId));

    if (hThread == NULL) {
        // スレッド起動失敗 → ソケットを閉じて終了
        delete pArgs;
        closesocket(hSocket);
        return;
    }

    conn.hThread = hThread;

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_connections.push_back(conn);
    }

    char szLog[128];
    wsprintfA(szLog, "[AdminWsHub] AddConnection: socket=%u session=%s total=%d\n",
              static_cast<unsigned>(hSocket),
              sessionId.c_str(),
              static_cast<int>(m_connections.size()));
    OutputDebugStringA(szLog);
}

// ------------------------------------------------------------
// 接続解除
// ------------------------------------------------------------

void CAdminWsHub::RemoveConnection(SOCKET hSocket)
{
    HANDLE hThread = NULL;

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (std::vector<Connection>::iterator it = m_connections.begin();
             it != m_connections.end(); ++it)
        {
            if (it->hSocket == hSocket) {
                hThread = it->hThread;
                m_connections.erase(it);
                break;
            }
        }
    }

    // スレッドハンドルは Shutdown で join するため、ここではクローズだけ行う。
    // hThread は AddConnection が所有しているので閉じない（Shutdown が行う）。
    (void)hThread;

    char szLog[64];
    wsprintfA(szLog, "[AdminWsHub] RemoveConnection: socket=%u\n",
              static_cast<unsigned>(hSocket));
    OutputDebugStringA(szLog);
}

// ------------------------------------------------------------
// ブロードキャスト
// ------------------------------------------------------------

void CAdminWsHub::BroadcastJson(const std::string &jsonText)
{
    // 送信前にスナップショットを取る（送信中に RemoveConnection されても安全）
    std::vector<SOCKET> sockets;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        sockets.reserve(m_connections.size());
        for (std::vector<Connection>::const_iterator it = m_connections.begin();
             it != m_connections.end(); ++it)
        {
            sockets.push_back(it->hSocket);
        }
    }

    for (size_t i = 0; i < sockets.size(); ++i) {
        // 送信失敗は無視（切断は recv ループが検知して RemoveConnection する）
        WebSocketProtocol::SendTextFrame(sockets[i], jsonText);
    }
}

// ------------------------------------------------------------
// selection_changed ブロードキャスト
// ------------------------------------------------------------

// static
std::string CAdminWsHub::BuildSelectionJson(const Selection &sel)
{
    static const char *const s_typeNames[] = {
        "map_cell",
        "char",
        "placement",
        "event",
    };

    std::ostringstream oss;
    oss << '{';
    oss << "\"type\":\"" << JsonUtils::Escape(s_typeNames[static_cast<int>(sel.type)]) << "\",";
    oss << "\"mapId\":" << sel.nMapId << ",";
    oss << "\"x\":" << sel.nX << ",";
    oss << "\"y\":" << sel.nY << ",";
    if (sel.bHasRefId) {
        oss << "\"refId\":" << sel.nRefId << ",";
    } else {
        oss << "\"refId\":null,";
    }
    oss << "\"updatedAt\":\"" << JsonUtils::Escape(sel.updatedAt) << "\"";
    oss << '}';
    return oss.str();
}

void CAdminWsHub::BroadcastSelectionChanged(const std::string &sessionId, const Selection *pSel)
{
    std::ostringstream oss;
    oss << "{\"kind\":\"selection_changed\",";
    oss << "\"sessionId\":\"" << JsonUtils::Escape(sessionId) << "\",";
    oss << "\"payload\":";
    if (pSel != NULL) {
        oss << BuildSelectionJson(*pSel);
    } else {
        oss << "null";
    }
    oss << "}";

    BroadcastJson(oss.str());
}

// ------------------------------------------------------------
// シャットダウン
// ------------------------------------------------------------

void CAdminWsHub::Shutdown()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_bShutdown = true;
    }

    // 全接続に Close フレームを送信してソケットを閉じる
    // スナップショットを取ってから操作する
    std::vector<Connection> snapshot;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        snapshot = m_connections;
    }

    for (size_t i = 0; i < snapshot.size(); ++i) {
        WebSocketProtocol::SendCloseFrame(snapshot[i].hSocket);
        shutdown(snapshot[i].hSocket, SD_BOTH);
        closesocket(snapshot[i].hSocket);
    }

    // 全スレッドが終了するまで待つ（最大 5 秒）
    for (size_t i = 0; i < snapshot.size(); ++i) {
        if (snapshot[i].hThread != NULL) {
            WaitForSingleObject(snapshot[i].hThread, 5000);
            CloseHandle(snapshot[i].hThread);
        }
    }

    // 接続リストをクリア
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_connections.clear();
    }

    OutputDebugStringA("[AdminWsHub] Shutdown complete\n");
}

// ------------------------------------------------------------
// recv ループスレッド
// ------------------------------------------------------------

// static
unsigned __stdcall CAdminWsHub::RecvThreadProc(void *lpParam)
{
    AdminWsRecvArgs *pArgs = reinterpret_cast<AdminWsRecvArgs *>(lpParam);
    if (pArgs != NULL) {
        if (pArgs->pHub != NULL) {
            pArgs->pHub->RunRecvLoop(pArgs->hSocket);
        }
        delete pArgs;
    }
    return 0;
}

void CAdminWsHub::RunRecvLoop(SOCKET hSocket)
{
    // タイムアウトを長めに設定（管理画面は長時間接続を維持する）
    DWORD dwTimeout = 60000; // 60 秒
    setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO,
               reinterpret_cast<const char *>(&dwTimeout), sizeof(dwTimeout));

    while (true) {
        // シャットダウン中なら抜ける
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_bShutdown) {
                break;
            }
        }

        std::vector<unsigned char> payload;
        int nOpcode = 0;

        if (!WebSocketProtocol::ReadFrame(hSocket, payload, nOpcode)) {
            // 切断またはエラー
            break;
        }

        if (nOpcode == WebSocketProtocol::kOpcodeClose) {
            // Close フレームを受信したらエコーして終了
            WebSocketProtocol::SendCloseFrame(hSocket);
            break;
        } else if (nOpcode == WebSocketProtocol::kOpcodePing) {
            // Ping には Pong で応答
            if (!payload.empty()) {
                WebSocketProtocol::SendFrame(hSocket, &payload[0], payload.size(),
                                             WebSocketProtocol::kOpcodePong);
            } else {
                WebSocketProtocol::SendFrame(hSocket, NULL, 0,
                                             WebSocketProtocol::kOpcodePong);
            }
        } else if ((nOpcode == WebSocketProtocol::kOpcodeText) ||
                   (nOpcode == WebSocketProtocol::kOpcodeBinary))
        {
            // クライアントからのメッセージは現時点では無視（警告ログのみ）
            OutputDebugStringA("[AdminWsHub] RunRecvLoop: unexpected client message (ignored)\n");
        }
        // その他のオペコードも無視
    }

    // 切断処理
    RemoveConnection(hSocket);
    shutdown(hSocket, SD_BOTH);
    closesocket(hSocket);

    OutputDebugStringA("[AdminWsHub] RunRecvLoop: exited\n");
}
