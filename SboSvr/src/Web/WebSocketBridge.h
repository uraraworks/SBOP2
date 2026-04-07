#pragma once

#include <vector>
#include <string>

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
    /// @brief スレッドエントリポイント（_beginthreadex用）
    static unsigned __stdcall ThreadProc(void *lpParam);

    /// @brief メインスレッド処理
    void Run();

    /// @brief リッスンソケットを作成してbind/listen
    bool CreateListener();

    /// @brief リッスンソケットをクローズ
    void CloseListener();

    /// @brief selectループでWebSocket接続を受け付ける
    void ProcessLoop();

    /// @brief 新規接続を受け付けてセッションスレッドを起動
    void HandleAccept();

    /// @brief 各セッション用スレッドエントリポイント
    static unsigned __stdcall SessionThreadProc(void *lpParam);

    /// @brief WebSocketセッション処理（ハンドシェイク→TCP接続→ブリッジ）
    void HandleSession(SOCKET hWsClient);

    // WebSocketハンドシェイク -------------------------------------------

    /// @brief WebSocketハンドシェイクを行う
    bool PerformHandshake(SOCKET hClient);

    /// @brief Sec-WebSocket-AcceptキーをSHA-1+Base64で計算する
    static std::string ComputeAcceptKey(const std::string &clientKey);

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

    // SHA-1・Base64 自前実装 -------------------------------------------

    /// @brief SHA-1ハッシュを計算する（RFC 3174準拠）
    static void Sha1(const unsigned char *pData, size_t nLength, unsigned char hash[20]);

    /// @brief バイナリデータをBase64エンコードする
    static std::string Base64Encode(const unsigned char *pData, size_t nLength);

private:
    SOCKET          m_hListen;       ///< リッスンソケット
    HANDLE          m_hThread;       ///< メインスレッドハンドル
    HANDLE          m_hStopEvent;    ///< 停止イベント
    HANDLE          m_hStartedEvent; ///< 起動完了イベント
    unsigned short  m_wWsPort;       ///< WebSocketリッスンポート
    unsigned short  m_wTcpPort;      ///< 転送先TCPポート
    bool            m_bInitSucceeded;///< 初期化成功フラグ
};

/// @brief HandleSession用スレッド引数
struct WebSocketSessionArgs
{
    CWebSocketBridge *pBridge;   ///< ブリッジオブジェクト
    SOCKET            hWsClient; ///< WebSocketクライアントソケット
};
