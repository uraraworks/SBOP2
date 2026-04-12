#include "stdafx.h"
#include "WebSocketBridge.h"

#include <string>
#include <sstream>
#include <cstring>
#include <process.h>
#include <cstdlib>
#include <wincrypt.h>
#pragma comment(lib, "advapi32.lib")

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

/// WebSocketハンドシェイクで使うGUID（RFC 6455）
const char *kWsGuid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

/// PACKETINFO ヘッダサイズ（dwCmd 4byte + dwSize 4byte）
const DWORD kPacketInfoSize = 8;

/// セッションスレッドの受信タイムアウト（ミリ秒）
const DWORD kSessionTimeoutMs = 30000;

// ============================================================
//  SHA-1 自前実装（RFC 3174）
// ============================================================

/// @brief 32bit左回転
inline unsigned int RotLeft32(unsigned int val, int bits)
{
    return (val << bits) | (val >> (32 - bits));
}

/// @brief SHA-1ハッシュを計算する
void Sha1Internal(const unsigned char *pData, size_t nLength, unsigned char hash[20])
{
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    DWORD dwHashLen = 20;

    ZeroMemory(hash, 20);
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        return;
    }
    if (!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash)) {
        CryptReleaseContext(hProv, 0);
        return;
    }
    CryptHashData(hHash, pData, (DWORD)nLength, 0);
    CryptGetHashParam(hHash, HP_HASHVAL, hash, &dwHashLen, 0);
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
}

// ============================================================
//  Base64 自前実装
// ============================================================

std::string Base64EncodeInternal(const unsigned char *pData, size_t nLength)
{
    static const char kTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;
    result.reserve(((nLength + 2) / 3) * 4);

    for (size_t i = 0; i < nLength; i += 3) {
        unsigned int val = static_cast<unsigned int>(pData[i]) << 16;
        if (i + 1 < nLength) {
            val |= static_cast<unsigned int>(pData[i + 1]) << 8;
        }
        if (i + 2 < nLength) {
            val |= static_cast<unsigned int>(pData[i + 2]);
        }

        result += kTable[(val >> 18) & 0x3F];
        result += kTable[(val >> 12) & 0x3F];
        result += (i + 1 < nLength) ? kTable[(val >> 6) & 0x3F] : '=';
        result += (i + 2 < nLength) ? kTable[ val       & 0x3F] : '=';
    }
    return result;
}

} // anonymous namespace

// ============================================================
//  CWebSocketBridge 実装
// ============================================================

CWebSocketBridge::CWebSocketBridge()
    : m_hListen(INVALID_SOCKET)
    , m_hThread(NULL)
    , m_hStopEvent(NULL)
    , m_hStartedEvent(NULL)
    , m_wWsPort(0)
    , m_wTcpPort(0)
    , m_bInitSucceeded(false)
{
}

CWebSocketBridge::~CWebSocketBridge()
{
    Stop();
}

bool CWebSocketBridge::Start(unsigned short wWsPort, unsigned short wTcpPort)
{
    if (m_hThread != NULL) {
        return false;
    }

    m_wWsPort  = wWsPort;
    m_wTcpPort = wTcpPort;

    m_hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (m_hStopEvent == NULL) {
        return false;
    }

    m_hStartedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (m_hStartedEvent == NULL) {
        CloseHandle(m_hStopEvent);
        m_hStopEvent = NULL;
        return false;
    }

    m_bInitSucceeded = false;

    unsigned uThreadId = 0;
    m_hThread = reinterpret_cast<HANDLE>(
        _beginthreadex(NULL, 0, ThreadProc, this, 0, &uThreadId));
    if (m_hThread == NULL) {
        CloseHandle(m_hStartedEvent);
        m_hStartedEvent = NULL;
        CloseHandle(m_hStopEvent);
        m_hStopEvent = NULL;
        return false;
    }

    // 起動完了を最大5秒待つ
    DWORD dwWait = WaitForSingleObject(m_hStartedEvent, 5000);
    if ((dwWait != WAIT_OBJECT_0) || !m_bInitSucceeded) {
        Stop();
        return false;
    }

    return true;
}

void CWebSocketBridge::Stop()
{
    if (m_hThread != NULL) {
        if (m_hStopEvent != NULL) {
            SetEvent(m_hStopEvent);
        }
        WaitForSingleObject(m_hThread, 5000);
        CloseHandle(m_hThread);
        m_hThread = NULL;
    }

    if (m_hStopEvent != NULL) {
        CloseHandle(m_hStopEvent);
        m_hStopEvent = NULL;
    }

    if (m_hStartedEvent != NULL) {
        CloseHandle(m_hStartedEvent);
        m_hStartedEvent = NULL;
    }
}

// ------------------------------------------------------------
// スレッドエントリポイント
// ------------------------------------------------------------

unsigned __stdcall CWebSocketBridge::ThreadProc(void *lpParam)
{
    CWebSocketBridge *pBridge = reinterpret_cast<CWebSocketBridge *>(lpParam);
    if (pBridge != NULL) {
        pBridge->Run();
    }
    return 0;
}

void CWebSocketBridge::Run()
{
    bool bWinsockStarted = false;
    WSADATA wsaData;
    ZeroMemory(&wsaData, sizeof(wsaData));

    // WSAStartupは参照カウント方式なので複数回呼んでOK
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0) {
        bWinsockStarted = true;

        if (CreateListener()) {
            m_bInitSucceeded = true;
            SetEvent(m_hStartedEvent);
            ProcessLoop();
        } else {
            SetEvent(m_hStartedEvent);
        }
    } else {
        SetEvent(m_hStartedEvent);
    }

    CloseListener();

    if (bWinsockStarted) {
        WSACleanup();
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

    if (bind(m_hListen, reinterpret_cast<const sockaddr *>(&service),
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
    while (WaitForSingleObject(m_hStopEvent, 0) == WAIT_TIMEOUT) {
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
        OutputDebugStringA("[WebSocketBridge] HandleAccept: accept FAILED\n");
        return;
    }
    OutputDebugStringA("[WebSocketBridge] HandleAccept: accepted\n");

    // セッションスレッドに引数を渡す
    WebSocketSessionArgs *pArgs = new WebSocketSessionArgs();
    pArgs->pBridge   = this;
    pArgs->hWsClient = hWsClient;

    unsigned uThreadId = 0;
    HANDLE hThread = reinterpret_cast<HANDLE>(
        _beginthreadex(NULL, 0, SessionThreadProc, pArgs, 0, &uThreadId));

    if (hThread == NULL) {
        // スレッド作成失敗時はここで後始末
        closesocket(hWsClient);
        delete pArgs;
        return;
    }

    // スレッドハンドルは不要なのでクローズ（スレッドは自律実行）
    CloseHandle(hThread);
}

// ------------------------------------------------------------
// セッションスレッド
// ------------------------------------------------------------

unsigned __stdcall CWebSocketBridge::SessionThreadProc(void *lpParam)
{
    WebSocketSessionArgs *pArgs = reinterpret_cast<WebSocketSessionArgs *>(lpParam);
    if (pArgs != NULL) {
        if (pArgs->pBridge != NULL) {
            pArgs->pBridge->HandleSession(pArgs->hWsClient);
        }
        delete pArgs;
    }
    return 0;
}

void CWebSocketBridge::HandleSession(SOCKET hWsClient)
{
    OutputDebugStringA("[WebSocketBridge] HandleSession: start\n");

    // ソケットのタイムアウトを設定
    DWORD dwTimeout = kSessionTimeoutMs;
    setsockopt(hWsClient, SOL_SOCKET, SO_RCVTIMEO,
               reinterpret_cast<const char *>(&dwTimeout), sizeof(dwTimeout));
    setsockopt(hWsClient, SOL_SOCKET, SO_SNDTIMEO,
               reinterpret_cast<const char *>(&dwTimeout), sizeof(dwTimeout));

    // 1. WebSocketハンドシェイク
    if (!PerformHandshake(hWsClient)) {
        OutputDebugStringA("[WebSocketBridge] HandleSession: handshake FAILED\n");
        closesocket(hWsClient);
        return;
    }
    OutputDebugStringA("[WebSocketBridge] HandleSession: handshake OK\n");

    // 2. localhost の TCPゲームポートへ接続
    SOCKET hTcpSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (hTcpSock == INVALID_SOCKET) {
        OutputDebugStringA("[WebSocketBridge] HandleSession: TCP socket create FAILED\n");
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
        OutputDebugStringA(szLog);
    }

    if (connect(hTcpSock, reinterpret_cast<const sockaddr *>(&tcpAddr),
                sizeof(tcpAddr)) == SOCKET_ERROR) {
        char szLog[128];
        wsprintfA(szLog, "[WebSocketBridge] TCP connect FAILED: WSA=%d\n", WSAGetLastError());
        OutputDebugStringA(szLog);
        closesocket(hTcpSock);
        closesocket(hWsClient);
        return;
    }

    OutputDebugStringA("[WebSocketBridge] HandleSession: TCP connected, starting bridge\n");

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

    // Sec-WebSocket-Accept キーを計算
    std::string acceptKey = ComputeAcceptKey(clientKey);

    // 101 Switching Protocols レスポンスを返す
    std::string response =
        "HTTP/1.1 101 Switching Protocols\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Accept: " + acceptKey + "\r\n"
        "\r\n";

    return SendAll(hClient, response.c_str(), response.size());
}

std::string CWebSocketBridge::ComputeAcceptKey(const std::string &clientKey)
{
    // クライアントキー + GUID を連結してSHA-1ハッシュを計算
    std::string combined = clientKey + kWsGuid;

    unsigned char hash[20];
    Sha1Internal(
        reinterpret_cast<const unsigned char *>(combined.c_str()),
        combined.size(),
        hash);

    // Base64エンコードして返す
    return Base64EncodeInternal(hash, 20);
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

    while (WaitForSingleObject(m_hStopEvent, 0) == WAIT_TIMEOUT) {
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

            // PACKETINFOヘッダ（8バイト: dwCmd 4byte + dwSize 4byte）に従い
            // 完全なパケット単位で WebSocketフレームに変換して送信する
            while (tcpRecvBuf.size() >= kPacketInfoSize) {
                // dwSize はヘッダ先頭から4バイト目（dwCmd が先）に格納されている
                // PACKETINFO 構造: [dwCmd(4)] [dwSize(4)]
                // dwSize はヘッダ8バイト + ペイロードの合計サイズ
                DWORD dwTotalSize = 0;
                memcpy(&dwTotalSize, &tcpRecvBuf[4], sizeof(DWORD));

                if (dwTotalSize < kPacketInfoSize) {
                    // 不正なパケット：バッファを破棄して終了
                    tcpRecvBuf.clear();
                    break;
                }

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

// ============================================================
//  CWebSocketBridge のラッパーメソッド（ヘッダとの対応）
// ============================================================

void CWebSocketBridge::Sha1(const unsigned char *pData, size_t nLength, unsigned char hash[20])
{
    Sha1Internal(pData, nLength, hash);
}

std::string CWebSocketBridge::Base64Encode(const unsigned char *pData, size_t nLength)
{
    return Base64EncodeInternal(pData, nLength);
}
