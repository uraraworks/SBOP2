#include "StdAfx.h"
#include "WebSocketBridge.h"

#include <string>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <system_error>

#include "crc.h" // CCRC: pre-check ハンドシェイクのCRC計算用
#include "../Platform/SvrPlatform.h"
#include "WebSocketProtocol.h"

// ============================================================
//  WebSocket オペコード定数
// ============================================================
namespace
{
const int WS_OPCODE_CONTINUATION = 0x0; ///< 継続フレーム
const int WS_OPCODE_TEXT         = 0x1; ///< テキストフレーム
const int WS_OPCODE_BINARY       = 0x2; ///< バイナリフレーム
const int WS_OPCODE_CLOSE        = 0x8; ///< 接続クローズ
const int WS_OPCODE_PING         = 0x9; ///< Ping
const int WS_OPCODE_PONG         = 0xA; ///< Pong

/// PACKETINFO ヘッダサイズ（dwSize 4byte + dwCRC 4byte）
const DWORD kPacketInfoSize = 8;

/// セッションスレッドの受信タイムアウト（ミリ秒）
const DWORD kSessionTimeoutMs = 30000;

} // anonymous namespace

// ============================================================
//  CWebSocketBridge 実装
// ============================================================

CWebSocketBridge::CWebSocketBridge()
    : m_hListen(INVALID_SOCKET)
    , m_bStop(false)
    , m_wWsPort(0)
    , m_wTcpPort(0)
{
}

CWebSocketBridge::~CWebSocketBridge()
{
    Stop();
}

bool CWebSocketBridge::Start(unsigned short wWsPort, unsigned short wTcpPort)
{
    if (m_thread.joinable()) {
        return false;
    }

    m_wWsPort  = wWsPort;
    m_wTcpPort = wTcpPort;
    m_bStop.store(false);

    // 起動完了(成否)は promise/future で Start() 側へ伝える。
    // スレッド終了通知(m_doneFuture)は Stop() のタイムアウト付き待機に使う。
    std::promise<bool> startedPromise;
    std::future<bool> startedFuture = startedPromise.get_future();

    std::promise<void> donePromise;
    m_doneFuture = donePromise.get_future();

    try {
        m_thread = std::thread(
            [this, promise = std::move(startedPromise), doneProm = std::move(donePromise)]() mutable {
                Run(std::move(promise));
                doneProm.set_value();
            });
    } catch (const std::system_error &) {
        return false;
    }

    // 起動完了を最大5秒待つ
    std::future_status status = startedFuture.wait_for(std::chrono::milliseconds(5000));
    if ((status != std::future_status::ready) || !startedFuture.get()) {
        Stop();
        return false;
    }

    return true;
}

void CWebSocketBridge::Stop()
{
    m_bStop.store(true);

    if (m_thread.joinable()) {
        // std::thread にはタイムアウト付き join が無いため、スレッド終了時に
        // set_value() される m_doneFuture を wait_for でタイムアウト付き待機する
        // 代用とする。間に合えば join、間に合わなければ従来の
        // WaitForSingleObject タイムアウト後の挙動（スレッドを強制終了させず
        // 追跡をやめるだけ）に合わせて detach する。
        bool bFinished = m_doneFuture.valid() &&
            (m_doneFuture.wait_for(std::chrono::milliseconds(5000)) ==
             std::future_status::ready);

        if (bFinished) {
            m_thread.join();
        } else {
            m_thread.detach();
        }
    }
}

// ------------------------------------------------------------
// メインスレッド本体
// ------------------------------------------------------------

void CWebSocketBridge::Run(std::promise<bool> startedPromise)
{
    bool bSocketStarted = false;

    // ソケットライブラリの初期化(Windowsでは参照カウント方式のWSAStartup相当。
    // SboPlatform::SocketStartup/SocketCleanup 参照)。
    if (SboPlatform::SocketStartup()) {
        bSocketStarted = true;

        if (CreateListener()) {
            startedPromise.set_value(true);
            ProcessLoop();
        } else {
            startedPromise.set_value(false);
        }
    } else {
        startedPromise.set_value(false);
    }

    CloseListener();

    if (bSocketStarted) {
        SboPlatform::SocketCleanup();
    }
}

// ------------------------------------------------------------
// リッスンソケット管理
// ------------------------------------------------------------

bool CWebSocketBridge::CreateListener()
{
    m_hListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_hListen == INVALID_SOCKET) {
        return false;
    }

    // SO_REUSEADDR でアドレス再利用を許可
    DWORD dwReuse = 1;
    setsockopt(m_hListen, SOL_SOCKET, SO_REUSEADDR,
               reinterpret_cast<const char *>(&dwReuse), sizeof(dwReuse));

    sockaddr_in service;
    ZeroMemory(&service, sizeof(service));
    service.sin_family      = AF_INET;
    service.sin_addr.s_addr = htonl(INADDR_ANY);
    service.sin_port        = htons(m_wWsPort);

    if (::bind(m_hListen, reinterpret_cast<const sockaddr *>(&service),
             sizeof(service)) == SOCKET_ERROR) {
        closesocket(m_hListen);
        m_hListen = INVALID_SOCKET;
        return false;
    }

    if (listen(m_hListen, SOMAXCONN) == SOCKET_ERROR) {
        closesocket(m_hListen);
        m_hListen = INVALID_SOCKET;
        return false;
    }

    return true;
}

void CWebSocketBridge::CloseListener()
{
    if (m_hListen != INVALID_SOCKET) {
        closesocket(m_hListen);
        m_hListen = INVALID_SOCKET;
    }
}

// ------------------------------------------------------------
// メインループ（selectでWebSocket接続を待ち受け）
// ------------------------------------------------------------

void CWebSocketBridge::ProcessLoop()
{
    while (!m_bStop.load()) {
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(m_hListen, &readSet);

        timeval tv;
        tv.tv_sec  = 0;
        tv.tv_usec = 500000; // 500ms タイムアウト

        int nReady = select(0, &readSet, NULL, NULL, &tv);
        if (nReady == SOCKET_ERROR) {
            break;
        }

        if (nReady == 0) {
            continue;
        }

        if (FD_ISSET(m_hListen, &readSet)) {
            HandleAccept();
        }
    }
}

void CWebSocketBridge::HandleAccept()
{
    SOCKET hWsClient = accept(m_hListen, NULL, NULL);
    if (hWsClient == INVALID_SOCKET) {
        SboPlatform::WriteDebugLine("[WebSocketBridge] HandleAccept: accept FAILED\n");
        return;
    }
    SboPlatform::WriteDebugLine("[WebSocketBridge] HandleAccept: accepted\n");

    // セッションスレッドは自律実行で、このクラス側では追跡しない
    // （従来の _beginthreadex + 直後の CloseHandle と同じ扱い）ので detach する。
    try {
        std::thread(&CWebSocketBridge::HandleSession, this, hWsClient).detach();
    } catch (const std::system_error &) {
        // スレッド作成失敗時はここで後始末
        closesocket(hWsClient);
    }
}

// ------------------------------------------------------------
// セッションスレッド
// ------------------------------------------------------------

void CWebSocketBridge::HandleSession(SOCKET hWsClient)
{
    SboPlatform::WriteDebugLine("[WebSocketBridge] HandleSession: start\n");

    // ソケットのタイムアウトを設定
    DWORD dwTimeout = kSessionTimeoutMs;
    setsockopt(hWsClient, SOL_SOCKET, SO_RCVTIMEO,
               reinterpret_cast<const char *>(&dwTimeout), sizeof(dwTimeout));
    setsockopt(hWsClient, SOL_SOCKET, SO_SNDTIMEO,
               reinterpret_cast<const char *>(&dwTimeout), sizeof(dwTimeout));

    // 1. WebSocketハンドシェイク
    if (!PerformHandshake(hWsClient)) {
        SboPlatform::WriteDebugLine("[WebSocketBridge] HandleSession: handshake FAILED\n");
        closesocket(hWsClient);
        return;
    }
    SboPlatform::WriteDebugLine("[WebSocketBridge] HandleSession: handshake OK\n");

    // 2. localhost の TCPゲームポートへ接続
    SOCKET hTcpSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (hTcpSock == INVALID_SOCKET) {
        SboPlatform::WriteDebugLine("[WebSocketBridge] HandleSession: TCP socket create FAILED\n");
        closesocket(hWsClient);
        return;
    }

    sockaddr_in tcpAddr;
    ZeroMemory(&tcpAddr, sizeof(tcpAddr));
    tcpAddr.sin_family      = AF_INET;
    tcpAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
    tcpAddr.sin_port        = htons(m_wTcpPort);

    {
        char szLog[128];
        wsprintfA(szLog, "[WebSocketBridge] TCP connect to 127.0.0.1:%d\n", (int)m_wTcpPort);
        SboPlatform::WriteDebugLine(szLog);
    }

    if (connect(hTcpSock, reinterpret_cast<const sockaddr *>(&tcpAddr),
                sizeof(tcpAddr)) == SOCKET_ERROR) {
        char szLog[128];
        wsprintfA(szLog, "[WebSocketBridge] TCP connect FAILED: WSA=%d\n", WSAGetLastError());
        SboPlatform::WriteDebugLine(szLog);
        closesocket(hTcpSock);
        closesocket(hWsClient);
        return;
    }

    SboPlatform::WriteDebugLine("[WebSocketBridge] HandleSession: TCP connected, starting bridge\n");

    // 4. TCP pre-check ハンドシェイク
    //    SboSockLib はクライアント接続直後に認証チャレンジを送ってくる。
    //    WebSocketBridge が TCP クライアントとしてこれに応答する。
    {
        // チャレンジ受信: [PACKETINFO(8)][dwChallenge(4)]
        unsigned char abyHeader[8];
        if (!RecvAll(hTcpSock, abyHeader, 8)) {
            SboPlatform::WriteDebugLine("[WebSocketBridge] pre-check: header recv FAILED\n");
            closesocket(hTcpSock);
            closesocket(hWsClient);
            return;
        }
        DWORD dwPayloadSize = 0;
        memcpy(&dwPayloadSize, &abyHeader[0], sizeof(DWORD));
        if (dwPayloadSize != sizeof(DWORD)) {
            SboPlatform::WriteDebugLine("[WebSocketBridge] pre-check: unexpected payload size\n");
            closesocket(hTcpSock);
            closesocket(hWsClient);
            return;
        }
        DWORD dwChallenge = 0;
        if (!RecvAll(hTcpSock, reinterpret_cast<unsigned char *>(&dwChallenge), sizeof(DWORD))) {
            SboPlatform::WriteDebugLine("[WebSocketBridge] pre-check: challenge recv FAILED\n");
            closesocket(hTcpSock);
            closesocket(hWsClient);
            return;
        }

        // レスポンス計算
        const DWORD kPreCheckKey = 0x56BB3E5E;
        DWORD dwResponse = (dwChallenge & kPreCheckKey) * kPreCheckKey;

        // レスポンス送信: [PACKETINFO(8)][dwResponse(4)]
        unsigned char abySend[12]; // 8 + 4
        DWORD dwRespSize = sizeof(DWORD);
        memcpy(&abySend[0], &dwRespSize, sizeof(DWORD));           // dwSize = 4
        // CRC は SboSockLib のものと互換にする
        CCRC crc;
        DWORD dwCRC = crc.GetCRC(reinterpret_cast<PBYTE>(&dwResponse), sizeof(DWORD));
        memcpy(&abySend[4], &dwCRC, sizeof(DWORD));                // dwCRC
        memcpy(&abySend[8], &dwResponse, sizeof(DWORD));           // payload
        if (!SendAll(hTcpSock, reinterpret_cast<const char *>(abySend), sizeof(abySend))) {
            SboPlatform::WriteDebugLine("[WebSocketBridge] pre-check: response send FAILED\n");
            closesocket(hTcpSock);
            closesocket(hWsClient);
            return;
        }

        SboPlatform::WriteDebugLine("[WebSocketBridge] pre-check: OK\n");
    }

    // 3. 双方向ブリッジループ
    BridgeLoop(hWsClient, hTcpSock);

    shutdown(hTcpSock,  SD_BOTH);
    shutdown(hWsClient, SD_BOTH);
    closesocket(hTcpSock);
    closesocket(hWsClient);
}

// ============================================================
//  WebSocketハンドシェイク
// ============================================================

bool CWebSocketBridge::PerformHandshake(SOCKET hClient)
{
    // HTTPリクエストヘッダを "\r\n\r\n" まで受信
    std::string request;
    request.reserve(1024);

    char szBuf[512];
    while (request.find("\r\n\r\n") == std::string::npos) {
        int nRecv = recv(hClient, szBuf, sizeof(szBuf) - 1, 0);
        if (nRecv <= 0) {
            return false;
        }
        request.append(szBuf, nRecv);
        if (request.size() > 8192) {
            return false; // ヘッダが大きすぎる
        }
    }

    // Sec-WebSocket-Key ヘッダを探す
    std::string clientKey;
    const char *kKeyHeader = "Sec-WebSocket-Key:";
    size_t nKeyPos = request.find(kKeyHeader);
    if (nKeyPos == std::string::npos) {
        // 大文字小文字の違いに対応
        const char *kKeyHeaderLower = "sec-websocket-key:";
        // ヘッダ名を小文字に変換して再検索
        std::string lowerReq = request;
        for (size_t i = 0; i < lowerReq.size(); ++i) {
            lowerReq[i] = static_cast<char>(
                std::tolower(static_cast<unsigned char>(lowerReq[i])));
        }
        nKeyPos = lowerReq.find(kKeyHeaderLower);
        if (nKeyPos == std::string::npos) {
            return false;
        }
    }

    size_t nValueStart = request.find_first_not_of(" \t",
                             nKeyPos + strlen(kKeyHeader));
    if (nValueStart == std::string::npos) {
        return false;
    }
    size_t nValueEnd = request.find("\r\n", nValueStart);
    if (nValueEnd == std::string::npos) {
        return false;
    }
    clientKey = request.substr(nValueStart, nValueEnd - nValueStart);

    // 末尾の空白を除去
    while (!clientKey.empty() && (clientKey.back() == ' ' || clientKey.back() == '\t')) {
        clientKey.resize(clientKey.size() - 1);
    }

    // Sec-WebSocket-Accept キーを計算（WebSocketProtocol の共通実装を利用）
    std::string acceptKey = WebSocketProtocol::ComputeAcceptKey(clientKey);

    // 101 Switching Protocols レスポンスを返す
    std::string response =
        "HTTP/1.1 101 Switching Protocols\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Accept: " + acceptKey + "\r\n"
        "\r\n";

    return SendAll(hClient, response.c_str(), response.size());
}

// ============================================================
//  WebSocketフレーム処理
// ============================================================

bool CWebSocketBridge::ReadWsFrame(SOCKET hSocket,
                                   std::vector<unsigned char> &outPayload,
                                   int &outOpcode)
{
    outPayload.clear();
    outOpcode = 0;

    // 先頭2バイトを読む
    unsigned char header[2];
    if (!RecvAll(hSocket, header, 2)) {
        return false;
    }

    // FINビットとオペコードを取得
    // bool bFin = (header[0] & 0x80) != 0; // 今は使わないが将来のために残す
    outOpcode = header[0] & 0x0F;
    bool bMask          = (header[1] & 0x80) != 0;
    unsigned long long payloadLen = header[1] & 0x7F;

    // ペイロード長の拡張フィールドを読む
    if (payloadLen == 126) {
        // 次の2バイトがBig-Endianサイズ
        unsigned char ext[2];
        if (!RecvAll(hSocket, ext, 2)) {
            return false;
        }
        payloadLen = (static_cast<unsigned long long>(ext[0]) << 8)
                   |  static_cast<unsigned long long>(ext[1]);
    } else if (payloadLen == 127) {
        // 次の8バイトがBig-Endianサイズ
        unsigned char ext[8];
        if (!RecvAll(hSocket, ext, 8)) {
            return false;
        }
        payloadLen = 0;
        for (int i = 0; i < 8; ++i) {
            payloadLen = (payloadLen << 8) | static_cast<unsigned long long>(ext[i]);
        }
    }

    // マスキングキーを読む（ブラウザ→サーバー方向は必ずMASK=1）
    unsigned char maskKey[4] = { 0, 0, 0, 0 };
    if (bMask) {
        if (!RecvAll(hSocket, maskKey, 4)) {
            return false;
        }
    }

    // ペイロードが大きすぎる場合は拒否（64MBを上限とする）
    if (payloadLen > 64 * 1024 * 1024ULL) {
        return false;
    }

    // ペイロードを読む
    if (payloadLen > 0) {
        outPayload.resize(static_cast<size_t>(payloadLen));
        if (!RecvAll(hSocket, &outPayload[0], static_cast<size_t>(payloadLen))) {
            return false;
        }

        // マスクを解除する
        if (bMask) {
            for (size_t i = 0; i < outPayload.size(); ++i) {
                outPayload[i] ^= maskKey[i % 4];
            }
        }
    }

    return true;
}

bool CWebSocketBridge::SendWsFrame(SOCKET hSocket,
                                   const unsigned char *pData,
                                   size_t nLength,
                                   int opcode)
{
    // サーバー→クライアント方向はマスクなし
    unsigned char headerBuf[10];
    size_t nHeaderLen = 0;

    // FIN=1, RSV=0, opcode
    headerBuf[nHeaderLen++] = static_cast<unsigned char>(0x80 | (opcode & 0x0F));

    // MASK=0, payloadLen
    if (nLength < 126) {
        headerBuf[nHeaderLen++] = static_cast<unsigned char>(nLength);
    } else if (nLength <= 0xFFFF) {
        headerBuf[nHeaderLen++] = 126;
        headerBuf[nHeaderLen++] = static_cast<unsigned char>((nLength >> 8) & 0xFF);
        headerBuf[nHeaderLen++] = static_cast<unsigned char>( nLength       & 0xFF);
    } else {
        headerBuf[nHeaderLen++] = 127;
        for (int i = 7; i >= 0; --i) {
            headerBuf[nHeaderLen++] = static_cast<unsigned char>((nLength >> (i * 8)) & 0xFF);
        }
    }

    // ヘッダ送信
    if (!SendAll(hSocket, reinterpret_cast<const char *>(headerBuf), nHeaderLen)) {
        return false;
    }

    // ペイロード送信
    if ((nLength > 0) && (pData != NULL)) {
        if (!SendAll(hSocket, reinterpret_cast<const char *>(pData), nLength)) {
            return false;
        }
    }

    return true;
}

// ============================================================
//  双方向ブリッジループ
// ============================================================

void CWebSocketBridge::BridgeLoop(SOCKET hWsClient, SOCKET hTcpSock)
{
    // TCP受信バッファ（パケット境界管理）
    std::vector<unsigned char> tcpRecvBuf;
    tcpRecvBuf.reserve(4096);

    while (!m_bStop.load()) {
        // WebSocketソケットとTCPソケットの両方を監視
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(hWsClient, &readSet);
        FD_SET(hTcpSock,  &readSet);

        timeval tv;
        tv.tv_sec  = 0;
        tv.tv_usec = 500000; // 500ms タイムアウト

        int nReady = select(0, &readSet, NULL, NULL, &tv);
        if (nReady == SOCKET_ERROR) {
            break;
        }

        if (nReady == 0) {
            continue; // タイムアウト→次のループへ
        }

        // --- WebSocket側からデータを受信 → TCPへ転送 ---
        if (FD_ISSET(hWsClient, &readSet)) {
            std::vector<unsigned char> payload;
            int nOpcode = 0;

            if (!ReadWsFrame(hWsClient, payload, nOpcode)) {
                break; // 接続切断やエラー
            }

            if (nOpcode == WS_OPCODE_CLOSE) {
                // Closeフレームを受けたらループ終了
                // エコーとして Closeフレームを送り返す
                SendWsFrame(hWsClient, NULL, 0, WS_OPCODE_CLOSE);
                break;
            } else if (nOpcode == WS_OPCODE_PING) {
                // PingにはPongで応答
                if (!payload.empty()) {
                    SendWsFrame(hWsClient, &payload[0], payload.size(), WS_OPCODE_PONG);
                } else {
                    SendWsFrame(hWsClient, NULL, 0, WS_OPCODE_PONG);
                }
            } else if ((nOpcode == WS_OPCODE_BINARY) || (nOpcode == WS_OPCODE_TEXT)) {
                // バイナリ/テキストデータをそのままTCPへ転送
                if (!payload.empty()) {
                    if (!SendAll(hTcpSock,
                                 reinterpret_cast<const char *>(&payload[0]),
                                 payload.size())) {
                        break;
                    }
                }
            }
            // 継続フレームは今回の実装では非対応（単純パススルー用途）
        }

        // --- TCP側からデータを受信 → WebSocketフレームへ変換して送信 ---
        if (FD_ISSET(hTcpSock, &readSet)) {
            char szBuf[4096];
            int nRecv = recv(hTcpSock, szBuf, sizeof(szBuf), 0);
            if (nRecv == 0) {
                break; // TCP接続が正常に閉じた
            }
            if (nRecv == SOCKET_ERROR) {
                int nErr = WSAGetLastError();
                if ((nErr == WSAEINTR) || (nErr == WSAEWOULDBLOCK)) {
                    continue;
                }
                break;
            }

            // 受信データをバッファに追加
            size_t nOldSize = tcpRecvBuf.size();
            tcpRecvBuf.resize(nOldSize + static_cast<size_t>(nRecv));
            memcpy(&tcpRecvBuf[nOldSize], szBuf, static_cast<size_t>(nRecv));

            // PACKETINFOヘッダ（8バイト: dwSize 4byte + dwCRC 4byte）に従い
            // 完全なパケット単位で WebSocketフレームに変換して送信する
            while (tcpRecvBuf.size() >= kPacketInfoSize) {
                // PACKETINFO 構造: [dwSize(4)] [dwCRC(4)]
                // dwSize はペイロードのみのサイズ（ヘッダ 8 バイトを含まない）
                DWORD dwPayloadSize = 0;
                memcpy(&dwPayloadSize, &tcpRecvBuf[0], sizeof(DWORD));

                DWORD dwTotalSize = dwPayloadSize + kPacketInfoSize;

                if (tcpRecvBuf.size() < static_cast<size_t>(dwTotalSize)) {
                    // まだパケット全体が揃っていない
                    break;
                }

                // 1パケット分が揃った → WebSocketバイナリフレームで送信
                SendWsFrame(hWsClient, &tcpRecvBuf[0], static_cast<size_t>(dwTotalSize),
                            WS_OPCODE_BINARY);

                // 送信済み分をバッファから除去
                tcpRecvBuf.erase(tcpRecvBuf.begin(),
                                 tcpRecvBuf.begin() + static_cast<ptrdiff_t>(dwTotalSize));
            }
        }
    }
}

// ============================================================
//  ユーティリティ
// ============================================================

bool CWebSocketBridge::SendAll(SOCKET hSocket, const char *pData, size_t nLength)
{
    size_t nTotalSent = 0;
    while (nTotalSent < nLength) {
        int nSent = send(hSocket,
                         pData + nTotalSent,
                         static_cast<int>(nLength - nTotalSent),
                         0);
        if (nSent == SOCKET_ERROR) {
            int nErr = WSAGetLastError();
            if ((nErr == WSAEINTR) || (nErr == WSAEWOULDBLOCK)) {
                continue;
            }
            return false;
        }
        if (nSent == 0) {
            return false;
        }
        nTotalSent += static_cast<size_t>(nSent);
    }
    return true;
}

bool CWebSocketBridge::RecvAll(SOCKET hSocket, unsigned char *pBuf, size_t nLength)
{
    size_t nTotalRecv = 0;
    while (nTotalRecv < nLength) {
        int nRecv = recv(hSocket,
                         reinterpret_cast<char *>(pBuf) + nTotalRecv,
                         static_cast<int>(nLength - nTotalRecv),
                         0);
        if (nRecv == 0) {
            return false; // 接続が閉じられた
        }
        if (nRecv == SOCKET_ERROR) {
            int nErr = WSAGetLastError();
            if ((nErr == WSAEINTR) || (nErr == WSAEWOULDBLOCK)) {
                continue;
            }
            return false;
        }
        nTotalRecv += static_cast<size_t>(nRecv);
    }
    return true;
}
