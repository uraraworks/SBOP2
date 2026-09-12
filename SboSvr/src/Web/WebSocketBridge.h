#pragma once

#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <future>

/// @brief WebSocket→TCPブリッジサーバー
/// WebSocket接続を受け付け、localhostのTCPゲームポートへ透過転送する
class CWebSocketBridge
{
public:
    CWebSocketBridge();
    ~CWebSocketBridge();

    /// @brief ブリッジサーバーを起動する
    /// @param wWsPort  WebSocketリッスンポート
    /// @param wTcpPort 転送先TCPポート（localhost）
    bool Start(unsigned short wWsPort, unsigned short wTcpPort);

    /// @brief ブリッジサーバーを停止する
    void Stop();

private:
    /// @brief メインスレッド処理
    /// @param startedPromise 起動完了(成否)を Start() 側へ伝える promise。
    ///                       スレッド内で必ず1回 set_value() する。
    void Run(std::promise<bool> startedPromise);

    /// @brief リッスンソケットを作成してbind/listen
    bool CreateListener();

    /// @brief リッスンソケットをクローズ
    void CloseListener();

    /// @brief selectループでWebSocket接続を受け付ける
    void ProcessLoop();

    /// @brief 新規接続を受け付けてセッションスレッドを起動
    void HandleAccept();

    /// @brief WebSocketセッション処理（ハンドシェイク→TCP接続→ブリッジ）
    /// @param hWsClient WebSocket側ソケット
    /// @param dwPeerIpNet accept()で得た直接の接続元IPアドレス(ネットワークバイトオーダー)
    void HandleSession(SOCKET hWsClient, DWORD dwPeerIpNet);

    // WebSocketハンドシェイク -------------------------------------------

    /// @brief WebSocketハンドシェイクを行う
    /// @param hClient クライアントソケット
    /// @param outRequest ハンドシェイクで受信した生のHTTPリクエスト文字列(ヘッダ解析用)
    bool PerformHandshake(SOCKET hClient, std::string &outRequest);

    // WebSocketフレーム処理 -------------------------------------------

    /// @brief WebSocketフレームを1フレーム読み込む
    /// @param hSocket   受信ソケット
    /// @param outPayload デマスク済みペイロード
    /// @param outOpcode  フレームのオペコード
    bool ReadWsFrame(SOCKET hSocket, std::vector<unsigned char> &outPayload, int &outOpcode);

    /// @brief WebSocketフレームを送信する（サーバー→クライアントなのでマスクなし）
    bool SendWsFrame(SOCKET hSocket, const unsigned char *pData, size_t nLength, int opcode);

    // TCP中継 -------------------------------------------

    /// @brief WebSocket↔TCP間の双方向転送ループ
    void BridgeLoop(SOCKET hWsClient, SOCKET hTcpSock);

    // ユーティリティ -------------------------------------------

    /// @brief 指定バイト数を確実に送信する
    static bool SendAll(SOCKET hSocket, const char *pData, size_t nLength);

    /// @brief 指定バイト数を確実に受信する
    static bool RecvAll(SOCKET hSocket, unsigned char *pBuf, size_t nLength);

private:
    SOCKET             m_hListen;     ///< リッスンソケット
    std::thread        m_thread;      ///< メインスレッド（ProcessLoopを実行）
    std::future<void>  m_doneFuture;  ///< メインスレッド終了通知（Stopのタイムアウト付き待機に使用）
    std::atomic<bool>  m_bStop;       ///< 停止フラグ
    unsigned short     m_wWsPort;     ///< WebSocketリッスンポート
    unsigned short     m_wTcpPort;    ///< 転送先TCPポート
};
