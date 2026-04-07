/// @file UraraSockTCPWebSocket.cpp
/// @brief WebSocket版TCP通信クラス実装（Emscripten用）

#include "StdAfx.h"

#ifdef __EMSCRIPTEN__

#include <SDL.h>
#include <cstring>
#include <cstdlib>
#include "crc.h"
#include "myLib/myZlib/myZlib.h"
#include "UraraSockTCPWebSocket.h"

// 128バイト以上の場合にzlib圧縮を使用する閾値
#define URARASOCK_USEZLIBSIZE   128

// PACKETINFO構造体（SboSockLib内部と同一フォーマット）
#pragma pack(push, 1)
struct WS_PACKETINFO {
    DWORD dwSize;   // ペイロードサイズ
    DWORD dwCRC;    // ペイロードのCRC
};
#pragma pack(pop)

static CCRC g_wsCrc;  // CRC計算用

// コンストラクタ

CUraraSockTCPWebSocket::CUraraSockTCPWebSocket()
    : m_socket(0)
    , m_bConnected(FALSE)
    , m_pfNotify(NULL)
    , m_pNotifyUserData(NULL)
    , m_dwPreCheckKey(0)
{
}

// デストラクタ

CUraraSockTCPWebSocket::~CUraraSockTCPWebSocket()
{
    Destroy();
}

// 受信データを解放

void CUraraSockTCPWebSocket::DeleteRecvData(PBYTE pData)
{
    if (pData) {
        free(pData);
    }
}

// 後始末

void CUraraSockTCPWebSocket::Destroy(void)
{
    if (m_socket) {
        emscripten_websocket_close(m_socket, 1000, "destroy");
        emscripten_websocket_delete(m_socket);
        m_socket = 0;
    }
    m_bConnected = FALSE;
}

// サーバーとして待ち受け（ブラウザではサポートしない）

BOOL CUraraSockTCPWebSocket::Host(HWND, DWORD, DWORD, WORD, DWORD)
{
    // ブラウザはサーバーとして動作しない
    return FALSE;
}

// サーバーへ接続

BOOL CUraraSockTCPWebSocket::Connect(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, LPCSTR pszAddr)
{
    (void)hWndParent;
    (void)dwMsgBase;

    m_dwPreCheckKey = dwKey;

    // WebSocket URLを構築（ws://addr:port）
    char szUrl[512];
    snprintf(szUrl, sizeof(szUrl), "ws://%s:%d", pszAddr, (int)wPort);

    EmscriptenWebSocketCreateAttributes attr;
    emscripten_websocket_init_create_attributes(&attr);
    attr.url = szUrl;
    attr.protocols = NULL;
    attr.createOnMainThread = EM_TRUE;

    m_socket = emscripten_websocket_new(&attr);
    if (m_socket <= 0) {
        SDL_Log("[WebSocket] 接続作成失敗: %s", szUrl);
        return FALSE;
    }

    // イベントハンドラを登録
    emscripten_websocket_set_onopen_callback(m_socket, this, OnOpen);
    emscripten_websocket_set_onmessage_callback(m_socket, this, OnMessage);
    emscripten_websocket_set_onclose_callback(m_socket, this, OnClose);
    emscripten_websocket_set_onerror_callback(m_socket, this, OnError);

    SDL_Log("[WebSocket] 接続開始: %s", szUrl);
    return TRUE;
}

// クライアントを切断（クライアント側では使用しない）

void CUraraSockTCPWebSocket::DeleteClient(DWORD) {}

// 送信キャンセル（クライアント側では使用しない）

void CUraraSockTCPWebSocket::SendCancel(DWORD) {}

// サーバーへデータ送信

void CUraraSockTCPWebSocket::Send(PBYTE pData, DWORD dwSize, BYTE byPriority)
{
    (void)byPriority;

    if (!m_bConnected || m_socket <= 0) {
        return;
    }

    // zlib圧縮（128バイト以上の場合）
    PBYTE pDataSend = NULL;
    DWORD dwSendSize = dwSize;

    if (dwSize >= URARASOCK_USEZLIBSIZE && pData != NULL) {
        PBYTE pTmp = (PBYTE)malloc(dwSize + 5);
        if (pTmp) {
            memset(pTmp, 0, dwSize + 5);
            CmyZlib zlib;
            zlib.DeflateInit();
            DWORD dwRet = zlib.Deflate(pData, dwSize, &pTmp[5], dwSize);
            zlib.DeflateEnd();
            if (dwRet != 0) {
                // 圧縮成功: [0xFF][元サイズ(4B)][圧縮データ...]
                pTmp[0] = 0xFF;
                memcpy(&pTmp[1], &dwSize, sizeof(DWORD));
                pDataSend = pTmp;
                dwSendSize = dwRet + 5;
            } else {
                free(pTmp);
            }
        }
    }

    if (pDataSend == NULL && dwSize > 0) {
        pDataSend = (PBYTE)malloc(dwSize);
        if (pDataSend && pData) {
            memcpy(pDataSend, pData, dwSize);
        }
        dwSendSize = dwSize;
    }

    // PACKETINFO + ペイロードをバイナリフレームとして送信
    DWORD dwTotalSize = sizeof(WS_PACKETINFO) + dwSendSize;
    PBYTE pFrame = (PBYTE)malloc(dwTotalSize);
    if (pFrame) {
        WS_PACKETINFO* pInfo = (WS_PACKETINFO*)pFrame;
        pInfo->dwSize = dwSendSize;
        pInfo->dwCRC = g_wsCrc.GetCRC(pDataSend, dwSendSize);
        if (pDataSend && dwSendSize > 0) {
            memcpy(pFrame + sizeof(WS_PACKETINFO), pDataSend, dwSendSize);
        }

        emscripten_websocket_send_binary(m_socket, pFrame, dwTotalSize);
        free(pFrame);
    }

    if (pDataSend) {
        free(pDataSend);
    }
}

// クライアントへデータ送信（クライアント側ではSendと同じ）

void CUraraSockTCPWebSocket::SendTo(DWORD dwID, PBYTE pData, DWORD dwSize, BYTE byPriority)
{
    (void)dwID;
    Send(pData, dwSize, byPriority);
}

DWORD CUraraSockTCPWebSocket::GetThrowghPutSend(DWORD) { return 0; }
DWORD CUraraSockTCPWebSocket::GetThrowghPutRecv(DWORD) { return 0; }
DWORD CUraraSockTCPWebSocket::GetQueCount(DWORD) { return 0; }
DWORD CUraraSockTCPWebSocket::GetIPAddress(DWORD) { return 0; }

// 通知コールバックを設定

void CUraraSockTCPWebSocket::SetNotifySink(PFURARASOCKNOTIFY pfNotify, void* pUserData)
{
    m_pfNotify = pfNotify;
    m_pNotifyUserData = pUserData;
}

// 親への通知ヘルパー

void CUraraSockTCPWebSocket::NotifyParent(UINT uMsgOffset, WPARAM wParam, LPARAM lParam)
{
    if (m_pfNotify) {
        m_pfNotify(m_pNotifyUserData, uMsgOffset, wParam, lParam);
    }
}

// --- WebSocketイベントハンドラ ---

// 接続成功時のコールバック
EM_BOOL CUraraSockTCPWebSocket::OnOpen(int, const EmscriptenWebSocketOpenEvent*, void* userData)
{
    CUraraSockTCPWebSocket* pThis = (CUraraSockTCPWebSocket*)userData;
    pThis->m_bConnected = TRUE;
    SDL_Log("[WebSocket] 接続完了");

    // 接続完了通知を上位に送る
    // 実際のプリチェックは OnMessage で最初の受信データとして処理
    pThis->NotifyParent(WM_URARASOCK_CONNECT, 0, 0);
    return EM_TRUE;
}

// データ受信時のコールバック
EM_BOOL CUraraSockTCPWebSocket::OnMessage(int, const EmscriptenWebSocketMessageEvent* event, void* userData)
{
    CUraraSockTCPWebSocket* pThis = (CUraraSockTCPWebSocket*)userData;

    if (!event->isText && event->numBytes > 0) {
        const BYTE* pRaw = (const BYTE*)event->data;
        DWORD dwRawSize = (DWORD)event->numBytes;

        // PACKETINFO ヘッダを検証
        if (dwRawSize < sizeof(WS_PACKETINFO)) {
            SDL_Log("[WebSocket] 受信データが小さすぎる: %u bytes", dwRawSize);
            return EM_TRUE;
        }

        const WS_PACKETINFO* pInfo = (const WS_PACKETINFO*)pRaw;
        DWORD dwPayloadSize = pInfo->dwSize;
        DWORD dwExpectedCrc = pInfo->dwCRC;

        if (sizeof(WS_PACKETINFO) + dwPayloadSize > dwRawSize) {
            SDL_Log("[WebSocket] パケットサイズ不正: payload=%u, raw=%u", dwPayloadSize, dwRawSize);
            return EM_TRUE;
        }

        const BYTE* pPayload = pRaw + sizeof(WS_PACKETINFO);

        // CRC検証（0は検証スキップ）
        DWORD dwActualCrc = g_wsCrc.GetCRC((PBYTE)pPayload, dwPayloadSize);
        if (dwExpectedCrc != 0 && dwActualCrc != dwExpectedCrc) {
            SDL_Log("[WebSocket] CRC不一致: expected=%08X, actual=%08X", dwExpectedCrc, dwActualCrc);
            return EM_TRUE;
        }

        // zlib展開（先頭バイトが0xFFの場合は圧縮データ）
        PBYTE pResult = NULL;
        DWORD dwResultSize = 0;

        if (dwPayloadSize >= 5 && pPayload[0] == 0xFF) {
            // 圧縮データ: [0xFF][元サイズ(4B)][圧縮データ...]
            DWORD dwOrigSize = 0;
            memcpy(&dwOrigSize, &pPayload[1], sizeof(DWORD));
            pResult = (PBYTE)malloc(dwOrigSize);
            if (pResult) {
                CmyZlib zlib;
                zlib.InflateInit();
                DWORD dwInflated = zlib.Inflate((PBYTE)&pPayload[5], dwPayloadSize - 5, pResult, dwOrigSize);
                zlib.InflateEnd();
                if (dwInflated == 0) {
                    free(pResult);
                    pResult = NULL;
                } else {
                    dwResultSize = dwOrigSize;
                }
            }
        }

        if (pResult == NULL) {
            // 非圧縮データ
            pResult = (PBYTE)malloc(dwPayloadSize);
            if (pResult) {
                memcpy(pResult, pPayload, dwPayloadSize);
                dwResultSize = dwPayloadSize;
            }
        }

        if (pResult) {
            // WM_URARASOCK_RECV 通知
            // wParam = データポインタ（DeleteRecvData()で解放される）
            // lParam = セッションID（クライアントは常に0）
            (void)dwResultSize;
            pThis->NotifyParent(WM_URARASOCK_RECV, (WPARAM)pResult, 0);
        }
    }

    return EM_TRUE;
}

// 切断時のコールバック
EM_BOOL CUraraSockTCPWebSocket::OnClose(int, const EmscriptenWebSocketCloseEvent* event, void* userData)
{
    CUraraSockTCPWebSocket* pThis = (CUraraSockTCPWebSocket*)userData;
    pThis->m_bConnected = FALSE;
    SDL_Log("[WebSocket] 切断: code=%d", event->code);
    pThis->NotifyParent(WM_URARASOCK_DISCONNECT, 0, 0);
    return EM_TRUE;
}

// エラー発生時のコールバック
EM_BOOL CUraraSockTCPWebSocket::OnError(int, const EmscriptenWebSocketErrorEvent*, void* userData)
{
    CUraraSockTCPWebSocket* pThis = (CUraraSockTCPWebSocket*)userData;
    SDL_Log("[WebSocket] エラー発生");
    pThis->m_bConnected = FALSE;
    pThis->NotifyParent(WM_URARASOCK_DISCONNECT, 0, 0);
    return EM_TRUE;
}

#endif // __EMSCRIPTEN__
