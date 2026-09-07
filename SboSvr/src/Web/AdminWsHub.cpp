#include "StdAfx.h"
#include "AdminWsHub.h"

#include <sstream>
#include <chrono>
#include <system_error>
#include <utility>

#include "WebSocketProtocol.h"
#include "JsonUtils.h"
#include "../Platform/SvrPlatform.h"

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

    // recv ループスレッドを起動する。
    // スレッド終了を Shutdown 側からタイムアウト付きで検知できるよう、
    // スレッド側で set_value() する promise を渡し、対応する future を
    // Connection に保持しておく。
    std::promise<void> donePromise;
    conn.doneFuture = donePromise.get_future();

    try {
        conn.thread = std::thread(
            [this, hSocket, promise = std::move(donePromise)]() mutable {
                RunRecvLoop(hSocket);
                promise.set_value();
            });
    } catch (const std::system_error &) {
        // スレッド起動失敗 → ソケットを閉じて終了
        closesocket(hSocket);
        return;
    }

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_connections.push_back(std::move(conn));
    }

    char szLog[128];
    snprintf(szLog, sizeof(szLog), "[AdminWsHub] AddConnection: socket=%u session=%s total=%d\n",
              static_cast<unsigned>(hSocket),
              sessionId.c_str(),
              static_cast<int>(m_connections.size()));
    SboPlatform::WriteDebugLine(szLog);
}

// ------------------------------------------------------------
// 接続解除
// ------------------------------------------------------------

void CAdminWsHub::RemoveConnection(SOCKET hSocket)
{
    Connection removed;
    bool bFound = false;

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (std::vector<Connection>::iterator it = m_connections.begin();
             it != m_connections.end(); ++it)
        {
            if (it->hSocket == hSocket) {
                removed = std::move(*it);
                m_connections.erase(it);
                bFound = true;
                break;
            }
        }
    }

    // RemoveConnection は recv ループスレッド自身の終了処理から呼ばれる。
    // 自スレッドを join すると即デッドロックするため、ここでは join せず
    // detach する（呼び出し元に戻った直後にスレッドは自然終了するのでリークしない）。
    // Shutdown 実行時にまだ残っている接続は Shutdown 側で待機・join/detach する。
    if (bFound && removed.thread.joinable()) {
        removed.thread.detach();
    }

    char szLog[64];
    snprintf(szLog, sizeof(szLog), "[AdminWsHub] RemoveConnection: socket=%u\n",
              static_cast<unsigned>(hSocket));
    SboPlatform::WriteDebugLine(szLog);
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
    // スナップショットを取ってから操作する。
    // Connection は std::thread を持つためコピーできないので、ムーブで退避する
    // （退避後 m_connections は空になる）。
    std::vector<Connection> snapshot;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        snapshot = std::move(m_connections);
    }

    for (size_t i = 0; i < snapshot.size(); ++i) {
        WebSocketProtocol::SendCloseFrame(snapshot[i].hSocket);
        shutdown(snapshot[i].hSocket, SD_BOTH);
        closesocket(snapshot[i].hSocket);
    }

    // 全スレッドが終了するまで待つ（最大 5 秒）。
    // std::thread にはタイムアウト付き join が存在しないため、スレッド終了時に
    // set_value() される doneFuture を wait_for でタイムアウト付き待機する代用とする。
    // タイムアウト以内に終了を確認できれば join、間に合わなければ
    // 従来の WaitForSingleObject タイムアウト後の挙動
    // （＝スレッドを強制終了させず、待つのをやめてハンドルだけ閉じる＝もう追跡しない）
    // に合わせて detach し、スレッドの終了は待たない。
    for (size_t i = 0; i < snapshot.size(); ++i) {
        if (!snapshot[i].thread.joinable()) {
            continue;
        }

        bool bFinished = snapshot[i].doneFuture.valid() &&
            (snapshot[i].doneFuture.wait_for(std::chrono::milliseconds(5000)) ==
             std::future_status::ready);

        if (bFinished) {
            snapshot[i].thread.join();
        } else {
            snapshot[i].thread.detach();
        }
    }

    // m_connections はスナップショット退避（ムーブ）時点で既に空になっている。

    SboPlatform::WriteDebugLine("[AdminWsHub] Shutdown complete\n");
}

// ------------------------------------------------------------
// recv ループスレッド
// ------------------------------------------------------------

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
            SboPlatform::WriteDebugLine("[AdminWsHub] RunRecvLoop: unexpected client message (ignored)\n");
        }
        // その他のオペコードも無視
    }

    // 切断処理
    RemoveConnection(hSocket);
    shutdown(hSocket, SD_BOTH);
    closesocket(hSocket);

    SboPlatform::WriteDebugLine("[AdminWsHub] RunRecvLoop: exited\n");
}
