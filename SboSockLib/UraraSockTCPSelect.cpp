/// @file UraraSockTCPSelect.cpp
/// @brief select ベースの TCP 通信クラス 実装ファイル
/// @copyright Copyright(C)URARA-works
///
/// 既存実装(UraraSockTCP.cpp)は WSAAsyncSelect + 隠しウィンドウで
/// イベントを受けているため、ウィンドウが無いと動かせない。
/// こちらは select() で多重化し、ウィンドウに依存しない。
///
/// ワイヤプロトコルは既存実装と完全に同じでなければならない。
/// - フレーミング: [DWORD dwSize][DWORD dwCRC][本体]
/// - プリチェック: 接続直後にサーバーが4バイトのチャレンジを送り、
///                 クライアントは (値 & key) * key を 32bit で返す。
///                 この最初のパケットだけ CRC を検証しない。
/// - CRC-32(RFC1952)
/// - zlib: 128バイト以上で圧縮。先頭 0xFF + 元サイズ4バイト + 圧縮データ
/// - 優先度キュー3本(High/Middle/Low)を High から順に掃き出す

#include "StdAfx.h"
#include "UraraSockTCPSelect.h"

#ifdef _WIN32

#include "crc.h"
#include "CWinsockStart.h"
#include "myZlib/myZlib.h"
#include <vector>
#include <deque>
#include <mutex>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       do { if ((p) != NULL) { delete (p);     (p) = NULL; } } while (0)
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) do { if ((p) != NULL) { delete[] (p);   (p) = NULL; } } while (0)
#endif

// 定数は既存実装と揃える

#define URARASOCKSEL_MAXCONNECT     200
#define URARASOCKSEL_IDBASE         100000
#define URARASOCKSEL_RECVBUFSIZE    (1024 * 64)
#define URARASOCKSEL_SOCKBUFSIZE    (1024 * 64)
#define URARASOCKSEL_USEZLIBSIZE    (128)
#define URARASOCKSEL_TIME_KEEPALIVE 60000
#define URARASOCKSEL_SELECT_WAIT_US 50000   // 起床用ソケットがあるので長めでよい

// フレームヘッダ(既存実装と同一レイアウト)

typedef struct _URARASOCKSEL_PACKETINFO
{
    DWORD dwSize;
    DWORD dwCRC;
} URARASOCKSEL_PACKETINFO;

// メインスレッドから select スレッドへ渡す指示
//
// 既存実装が PostMessage で行っていたスレッド越えを、
// FIFO のコマンドキューで置き換える。順序が保たれることが重要。

enum URARASOCKSEL_CMDTYPE
{
    URARASOCKSEL_CMD_SEND = 0,  // 送信キューへ追加
    URARASOCKSEL_CMD_CANCEL,    // 送信キャンセル
    URARASOCKSEL_CMD_DELETE,    // クライアントを切断
};

typedef struct _URARASOCKSEL_CMD
{
    URARASOCKSEL_CMDTYPE Type;
    DWORD  dwID;        // URARASOCK_SENDALL または ID
    BYTE   byPriority;
    PBYTE  pData;       // フレーム済みデータ(所有権はキュー)
    DWORD  dwSize;
} URARASOCKSEL_CMD;

class CUraraSockTCPSelect;

// 接続1本ぶんの状態

class CUraraSockTCPSelectSlot
{
public:
    CUraraSockTCPSelectSlot(void);
    ~CUraraSockTCPSelectSlot(void);

    BOOL Create(SOCKET socket, DWORD dwAddr, WORD wPeerPort, DWORD dwID);
    void Destroy(void);
    BOOL IsValid(void) const { return m_socket != INVALID_SOCKET; }
    BOOL HasSendData(void) const;

    void AddSendData(const BYTE *pFramed, DWORD dwSize, BYTE byPriority);
    void CancelSend(void);

    BOOL OnWritable(void);                              // FALSE なら切断
    BOOL OnReadable(CUraraSockTCPSelect *pOwner);       // FALSE なら切断

    void RenewThrowghPut(void);
    DWORD GetThrowghPutSend(void) const { return m_dwThrowghPutSend; }
    DWORD GetThrowghPutRecv(void) const { return m_dwThrowghPutRecv; }
    DWORD GetQueCount(void) const;
    DWORD GetIPAddress(void) const { return m_dwAddr; }
    DWORD GetPeerPort(void) const { return static_cast<DWORD>(m_wPeerPort); }

public:
    SOCKET m_socket;
    DWORD  m_dwSockID;
    DWORD  m_dwTimeLastRecv;
    DWORD  m_dwPreCheck;    // サーバーが送ったチャレンジ値
    BOOL   m_bPreCheck;     // プリチェック済みか

private:
    std::vector<BYTE> *SelectSendBuffer(void);

private:
    CCRC  *m_pCrc;
    DWORD  m_dwAddr;
    WORD   m_wPeerPort;    // 相手ポート番号(ホストバイトオーダー)

    // 受信の途中状態
    PBYTE  m_pRecvBuffer;
    PBYTE  m_pRecvTmp;
    DWORD  m_dwRecvSize;
    DWORD  m_dwRecvSizeTarget;
    DWORD  m_dwRecvCrc;
    BOOL   m_bRecvHeader;   // ヘッダ受信中か

    // 送信の途中状態(優先度ごとにバイト列として保持する)
    std::vector<BYTE> m_SendHi;
    std::vector<BYTE> m_SendMid;
    std::vector<BYTE> m_SendLow;
    DWORD  m_dwSendPos;

    // スループット計測
    DWORD  m_dwSendDataSize;
    DWORD  m_dwRecvDataSize;
    DWORD  m_dwThrowghPutSend;
    DWORD  m_dwThrowghPutRecv;
    DWORD  m_dwTimeThrowghPut;
};

// select ベースの通信クラス

class CUraraSockTCPSelect : public CUraraSockTCP
{
public:
    CUraraSockTCPSelect(void);
    virtual ~CUraraSockTCPSelect(void);

    void  DeleteRecvData(PBYTE pData) override;
    void  Destroy(void) override;
    void  SetNotifySink(PFURARASOCKNOTIFY pfNotify, void *pUserData) override;
    BOOL  Host(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, DWORD dwCount) override;
    BOOL  Connect(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, LPCSTR pszAddr) override;
    void  DeleteClient(DWORD dwID) override;
    void  SendCancel(DWORD dwID = 0) override;
    void  Send(PBYTE pData, DWORD dwSize, BYTE byPriority = URARASOCK_SENDPRIORITY_MIDDLE) override;
    void  SendTo(DWORD dwID, PBYTE pData, DWORD dwSize, BYTE byPriority = URARASOCK_SENDPRIORITY_MIDDLE) override;
    DWORD GetThrowghPutSend(DWORD dwID) override;
    DWORD GetThrowghPutRecv(DWORD dwID) override;
    DWORD GetQueCount(DWORD dwID) override;
    DWORD GetIPAddress(DWORD dwID) override;
    DWORD GetPeerPort(DWORD dwID) override;

    // スロットから呼ばれる
    void  OnRecvPacket(DWORD dwIndex, PBYTE pData, DWORD dwSize);
    BOOL  OnPreCheck(DWORD dwIndex, DWORD dwAnswer);    // FALSE なら切断
    void  NotifyParent(UINT uMsgOffset, WPARAM wParam, LPARAM lParam);

private:
    static void __cdecl ThreadEntry(void *pParam);
    void  ThreadMain(void);

    BOOL  CreateWakeupSocket(void);
    void  CloseWakeupSocket(void);
    void  Wakeup(void);
    void  DrainWakeup(void);

    void  PushCommand(URARASOCKSEL_CMD *pCmd);
    void  ProcCommands(void);
    void  ClearCommands(void);

    void  MakeFrame(PBYTE pData, DWORD dwSize, PBYTE *ppFrame, DWORD *pdwFrameSize);
    void  OnAccept(void);
    void  CloseSlot(DWORD dwIndex);
    void  CheckKeepalive(void);

private:
    CCRC              *m_pCrc;
    CWinsockStart      m_Winsock;
    CmyZlib            m_Zlib;

    SOCKET             m_socket;        // 待ち受けソケット
    SOCKET             m_sockWakeup;    // select を起こすための UDP ソケット
    SOCKADDR_IN        m_addrWakeup;

    WORD               m_wPort;
    DWORD              m_dwMaxConnectCount;
    DWORD              m_dwConnectCount;
    DWORD              m_dwPreCheckKey;

    HANDLE             m_hThread;
    HANDLE             m_hReady;
    volatile BOOL      m_bStop;
    BOOL               m_bHosting;

    PFURARASOCKNOTIFY  m_pfNotify;
    void              *m_pNotifyUserData;

    std::mutex         m_CritCmd;
    std::deque<URARASOCKSEL_CMD *> m_deqCmd;

    CUraraSockTCPSelectSlot *m_pSlot;
};

//////////////////////////////////////////////////////////////////////
// CUraraSockTCPSelectSlot
//////////////////////////////////////////////////////////////////////

CUraraSockTCPSelectSlot::CUraraSockTCPSelectSlot(void)
    : m_socket(INVALID_SOCKET)
    , m_dwSockID(0)
    , m_dwTimeLastRecv(0)
    , m_dwPreCheck(0)
    , m_bPreCheck(FALSE)
    , m_pCrc(NULL)
    , m_dwAddr(0)
    , m_wPeerPort(0)
    , m_pRecvBuffer(NULL)
    , m_pRecvTmp(NULL)
    , m_dwRecvSize(0)
    , m_dwRecvSizeTarget(0)
    , m_dwRecvCrc(0)
    , m_bRecvHeader(TRUE)
    , m_dwSendPos(0)
    , m_dwSendDataSize(0)
    , m_dwRecvDataSize(0)
    , m_dwThrowghPutSend(0)
    , m_dwThrowghPutRecv(0)
    , m_dwTimeThrowghPut(0)
{
    m_pCrc = new CCRC;
}

CUraraSockTCPSelectSlot::~CUraraSockTCPSelectSlot(void)
{
    Destroy();
    SAFE_DELETE(m_pCrc);
}

BOOL CUraraSockTCPSelectSlot::Create(SOCKET socket, DWORD dwAddr, WORD wPeerPort, DWORD dwID)
{
    u_long ulNonBlock = 1;

    if (socket == INVALID_SOCKET) {
        return FALSE;
    }

    if (ioctlsocket(socket, FIONBIO, &ulNonBlock) == SOCKET_ERROR) {
        return FALSE;
    }

    m_socket           = socket;
    m_dwAddr           = dwAddr;
    m_wPeerPort        = wPeerPort;
    m_dwSockID         = dwID;
    m_dwTimeLastRecv   = GetTickCount();
    m_bPreCheck        = FALSE;
    m_dwPreCheck       = 0;
    m_dwSendPos        = 0;
    m_dwRecvSize       = 0;
    m_dwRecvSizeTarget = 0;
    m_dwRecvCrc        = 0;
    m_bRecvHeader      = TRUE;
    m_dwSendDataSize   = 0;
    m_dwRecvDataSize   = 0;
    m_dwThrowghPutSend = 0;
    m_dwThrowghPutRecv = 0;
    m_dwTimeThrowghPut = GetTickCount();

    m_pRecvBuffer = new BYTE[URARASOCKSEL_RECVBUFSIZE];
    m_pRecvTmp    = new BYTE[sizeof(URARASOCKSEL_PACKETINFO)];

    return TRUE;
}

void CUraraSockTCPSelectSlot::Destroy(void)
{
    if (m_socket != INVALID_SOCKET) {
        shutdown(m_socket, SD_BOTH);
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
    }
    SAFE_DELETE_ARRAY(m_pRecvBuffer);
    SAFE_DELETE_ARRAY(m_pRecvTmp);

    m_SendHi.clear();
    m_SendMid.clear();
    m_SendLow.clear();
    m_dwSendPos   = 0;
    m_bPreCheck   = FALSE;
    m_bRecvHeader = TRUE;
}

BOOL CUraraSockTCPSelectSlot::HasSendData(void) const
{
    return (m_SendHi.size() || m_SendMid.size() || m_SendLow.size()) ? TRUE : FALSE;
}

DWORD CUraraSockTCPSelectSlot::GetQueCount(void) const
{
    DWORD dwCount = 0;

    if (m_SendHi.size())  { dwCount ++; }
    if (m_SendMid.size()) { dwCount ++; }
    if (m_SendLow.size()) { dwCount ++; }
    return dwCount;
}

void CUraraSockTCPSelectSlot::AddSendData(const BYTE *pFramed, DWORD dwSize, BYTE byPriority)
{
    std::vector<BYTE> *pBuf;

    if ((m_socket == INVALID_SOCKET) || (pFramed == NULL) || (dwSize == 0)) {
        return;
    }

    switch (byPriority) {
    case URARASOCK_SENDPRIORITY_HIGH:   pBuf = &m_SendHi;  break;
    case URARASOCK_SENDPRIORITY_MIDDLE: pBuf = &m_SendMid; break;
    default:                            pBuf = &m_SendLow; break;
    }

    pBuf->insert(pBuf->end(), pFramed, pFramed + dwSize);
}

void CUraraSockTCPSelectSlot::CancelSend(void)
{
    m_SendHi.clear();
    m_SendMid.clear();
    m_SendLow.clear();
    m_dwSendPos = 0;
}

// 送信中のバッファを選ぶ
//
// 優先度の高いものから掃き出す。送信途中のバッファがある場合は
// それを継続する(途中で別の優先度に切り替えるとフレームが壊れる)。

std::vector<BYTE> *CUraraSockTCPSelectSlot::SelectSendBuffer(void)
{
    if (m_dwSendPos > 0) {
        // 送信途中。どのバッファの途中かは、先頭から消費している
        // バッファが1つだけであることで一意に定まる。
        if (m_SendHi.size())  { return &m_SendHi; }
        if (m_SendMid.size()) { return &m_SendMid; }
        if (m_SendLow.size()) { return &m_SendLow; }
        return NULL;
    }
    if (m_SendHi.size())  { return &m_SendHi; }
    if (m_SendMid.size()) { return &m_SendMid; }
    if (m_SendLow.size()) { return &m_SendLow; }
    return NULL;
}

BOOL CUraraSockTCPSelectSlot::OnWritable(void)
{
    if (m_socket == INVALID_SOCKET) {
        return FALSE;
    }

    while (true) {
        std::vector<BYTE> *pBuf = SelectSendBuffer();
        if (pBuf == NULL) {
            break;
        }
        if (m_dwSendPos >= pBuf->size()) {
            pBuf->clear();
            m_dwSendPos = 0;
            continue;
        }

        int nSize = static_cast<int>(pBuf->size() - m_dwSendPos);
        int nRet  = send(m_socket, reinterpret_cast<const char *>(&(*pBuf)[m_dwSendPos]), nSize, 0);
        if (nRet > 0) {
            m_dwSendDataSize += nRet;
            RenewThrowghPut();
            m_dwSendPos += nRet;
            if (m_dwSendPos >= pBuf->size()) {
                pBuf->clear();
                m_dwSendPos = 0;
            }
        } else {
            DWORD dwError = WSAGetLastError();
            if (dwError != WSAEWOULDBLOCK) {
                return FALSE;
            }
            break;
        }
    }
    return TRUE;
}

BOOL CUraraSockTCPSelectSlot::OnReadable(CUraraSockTCPSelect *pOwner)
{
    DWORD dwRecvSize = 0;
    DWORD dwPos;

    if (m_socket == INVALID_SOCKET) {
        return FALSE;
    }

    // 受信できるだけ読む
    while (dwRecvSize < URARASOCKSEL_RECVBUFSIZE) {
        int nSize = static_cast<int>(URARASOCKSEL_RECVBUFSIZE - dwRecvSize);
        int nRet  = recv(m_socket, reinterpret_cast<char *>(m_pRecvBuffer + dwRecvSize), nSize, 0);
        if (nRet > 0) {
            dwRecvSize += nRet;
            m_dwRecvDataSize += nRet;
            RenewThrowghPut();
            m_dwTimeLastRecv = GetTickCount();
            continue;
        }
        if (nRet == 0) {
            // 相手が正常に閉じた。WSAAsyncSelect 版は FD_CLOSE で
            // 検知していたが、select 版はここで判定しないと
            // 「読める」と報告され続けて空転する。
            return FALSE;
        }
        if (WSAGetLastError() != WSAEWOULDBLOCK) {
            return FALSE;
        }
        break;
    }

    // 溜まったバイト列をフレームへ切り分ける
    dwPos = 0;
    while (dwPos < dwRecvSize) {
        DWORD dwNeed;
        DWORD dwRemain;
        DWORD dwCopy;

        if (m_bRecvHeader) {
            dwNeed = sizeof(URARASOCKSEL_PACKETINFO) - m_dwRecvSize;
        } else {
            dwNeed = m_dwRecvSizeTarget - m_dwRecvSize;
        }
        dwRemain = dwRecvSize - dwPos;
        dwCopy   = (dwNeed < dwRemain) ? dwNeed : dwRemain;
        if (dwCopy == 0) {
            break;
        }

        CopyMemory(m_pRecvTmp + m_dwRecvSize, m_pRecvBuffer + dwPos, dwCopy);
        dwPos        += dwCopy;
        m_dwRecvSize += dwCopy;

        if (m_bRecvHeader) {
            if (m_dwRecvSize < sizeof(URARASOCKSEL_PACKETINFO)) {
                continue;
            }
            {
                URARASOCKSEL_PACKETINFO *pInfo =
                        reinterpret_cast<URARASOCKSEL_PACKETINFO *>(m_pRecvTmp);
                m_dwRecvSizeTarget = pInfo->dwSize;
                m_dwRecvCrc        = pInfo->dwCRC;
            }
            m_dwRecvSize  = 0;
            m_bRecvHeader = FALSE;

            // プリチェック前は 4 バイトのパケットしか受け付けない
            if ((m_bPreCheck == FALSE) && (m_dwRecvSizeTarget != sizeof(DWORD))) {
                return FALSE;
            }
            if (m_dwRecvSizeTarget > URARASOCKSEL_RECVBUFSIZE) {
                return FALSE;
            }

            SAFE_DELETE_ARRAY(m_pRecvTmp);
            if (m_dwRecvSizeTarget == 0) {
                // サイズ0は生存確認。通知せずヘッダ待ちへ戻る。
                m_pRecvTmp    = new BYTE[sizeof(URARASOCKSEL_PACKETINFO)];
                m_bRecvHeader = TRUE;
                continue;
            }
            m_pRecvTmp = new BYTE[m_dwRecvSizeTarget];
            continue;
        }

        if (m_dwRecvSize < m_dwRecvSizeTarget) {
            continue;
        }

        // 本体が揃った
        if (m_bPreCheck == FALSE) {
            // プリチェックのパケットだけは CRC を検証しない
            DWORD dwData = 0;
            CopyMemory(&dwData, m_pRecvTmp, sizeof(DWORD));
            SAFE_DELETE_ARRAY(m_pRecvTmp);
            m_pRecvTmp    = new BYTE[sizeof(URARASOCKSEL_PACKETINFO)];
            m_dwRecvSize  = 0;
            m_bRecvHeader = TRUE;

            if (pOwner->OnPreCheck(m_dwSockID, dwData) == FALSE) {
                return FALSE;
            }
            continue;
        }

        if ((m_dwRecvCrc == 0) || (m_dwRecvCrc != m_pCrc->GetCRC(m_pRecvTmp, m_dwRecvSizeTarget))) {
            return FALSE;
        }

        // 所有権を呼び出し側へ渡す
        pOwner->OnRecvPacket(m_dwSockID, m_pRecvTmp, m_dwRecvSizeTarget);
        m_pRecvTmp    = new BYTE[sizeof(URARASOCKSEL_PACKETINFO)];
        m_dwRecvSize  = 0;
        m_bRecvHeader = TRUE;
    }

    return TRUE;
}

void CUraraSockTCPSelectSlot::RenewThrowghPut(void)
{
    DWORD dwTime = GetTickCount();

    if (dwTime - m_dwTimeThrowghPut < 1000) {
        return;
    }
    m_dwThrowghPutSend = m_dwSendDataSize;
    m_dwThrowghPutRecv = m_dwRecvDataSize;
    m_dwSendDataSize   = 0;
    m_dwRecvDataSize   = 0;
    m_dwTimeThrowghPut = dwTime;
}

//////////////////////////////////////////////////////////////////////
// CUraraSockTCPSelect
//////////////////////////////////////////////////////////////////////

CUraraSockTCPSelect::CUraraSockTCPSelect(void)
    : m_pCrc(NULL)
    , m_socket(INVALID_SOCKET)
    , m_sockWakeup(INVALID_SOCKET)
    , m_wPort(0)
    , m_dwMaxConnectCount(0)
    , m_dwConnectCount(0)
    , m_dwPreCheckKey(0)
    , m_hThread(NULL)
    , m_hReady(NULL)
    , m_bStop(FALSE)
    , m_bHosting(FALSE)
    , m_pfNotify(NULL)
    , m_pNotifyUserData(NULL)
    , m_pSlot(NULL)
{
    ZeroMemory(&m_addrWakeup, sizeof(m_addrWakeup));
    m_pCrc = new CCRC;
}

CUraraSockTCPSelect::~CUraraSockTCPSelect(void)
{
    Destroy();
    SAFE_DELETE(m_pCrc);
}

void CUraraSockTCPSelect::DeleteRecvData(PBYTE pData)
{
    SAFE_DELETE_ARRAY(pData);
}

void CUraraSockTCPSelect::SetNotifySink(PFURARASOCKNOTIFY pfNotify, void *pUserData)
{
    m_pfNotify        = pfNotify;
    m_pNotifyUserData = pUserData;
}

void CUraraSockTCPSelect::NotifyParent(UINT uMsgOffset, WPARAM wParam, LPARAM lParam)
{
    if (m_pfNotify) {
        m_pfNotify(m_pNotifyUserData, uMsgOffset, wParam, lParam);
    }
}

// select を起こすための UDP ソケットを用意する
//
// Windows の select はソケットしか待てないため、自己宛の UDP を
// 起床用に使う。POSIX へ移す際は socketpair や eventfd に置き換えられる。

BOOL CUraraSockTCPSelect::CreateWakeupSocket(void)
{
    int nLen;
    u_long ulNonBlock = 1;
    SOCKADDR_IN addr;

    m_sockWakeup = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_sockWakeup == INVALID_SOCKET) {
        return FALSE;
    }

    ZeroMemory(&addr, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_port        = 0;   // 空きポートを OS に選ばせる
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(m_sockWakeup, reinterpret_cast<LPSOCKADDR>(&addr), sizeof(addr)) == SOCKET_ERROR) {
        closesocket(m_sockWakeup);
        m_sockWakeup = INVALID_SOCKET;
        return FALSE;
    }

    nLen = sizeof(m_addrWakeup);
    if (getsockname(m_sockWakeup, reinterpret_cast<LPSOCKADDR>(&m_addrWakeup), &nLen) == SOCKET_ERROR) {
        closesocket(m_sockWakeup);
        m_sockWakeup = INVALID_SOCKET;
        return FALSE;
    }

    ioctlsocket(m_sockWakeup, FIONBIO, &ulNonBlock);
    return TRUE;
}

void CUraraSockTCPSelect::CloseWakeupSocket(void)
{
    if (m_sockWakeup != INVALID_SOCKET) {
        closesocket(m_sockWakeup);
        m_sockWakeup = INVALID_SOCKET;
    }
}

void CUraraSockTCPSelect::Wakeup(void)
{
    BYTE byDummy = 0;

    if (m_sockWakeup == INVALID_SOCKET) {
        return;
    }
    sendto(m_sockWakeup, reinterpret_cast<const char *>(&byDummy), 1, 0,
           reinterpret_cast<LPSOCKADDR>(&m_addrWakeup), sizeof(m_addrWakeup));
}

void CUraraSockTCPSelect::DrainWakeup(void)
{
    BYTE byBuf[64];

    if (m_sockWakeup == INVALID_SOCKET) {
        return;
    }
    while (recv(m_sockWakeup, reinterpret_cast<char *>(byBuf), sizeof(byBuf), 0) > 0) {
        // 溜まっている分を捨てる
    }
}

void CUraraSockTCPSelect::PushCommand(URARASOCKSEL_CMD *pCmd)
{
    m_CritCmd.lock();
    m_deqCmd.push_back(pCmd);
    m_CritCmd.unlock();

    Wakeup();
}

void CUraraSockTCPSelect::ClearCommands(void)
{
    m_CritCmd.lock();
    while (!m_deqCmd.empty()) {
        URARASOCKSEL_CMD *pCmd = m_deqCmd.front();
        m_deqCmd.pop_front();
        SAFE_DELETE_ARRAY(pCmd->pData);
        SAFE_DELETE(pCmd);
    }
    m_CritCmd.unlock();
}

// 溜まった指示を処理する
//
// 既存実装の PostMessage と同じく FIFO で処理する。順序が変わると
// 送信の並びが崩れるため、まとめて取り出してから順に適用する。

void CUraraSockTCPSelect::ProcCommands(void)
{
    std::deque<URARASOCKSEL_CMD *> deqProc;

    m_CritCmd.lock();
    m_deqCmd.swap(deqProc);
    m_CritCmd.unlock();

    while (!deqProc.empty()) {
        URARASOCKSEL_CMD *pCmd = deqProc.front();
        deqProc.pop_front();

        switch (pCmd->Type) {
        case URARASOCKSEL_CMD_SEND:
            if (pCmd->dwID == URARASOCK_SENDALL) {
                for (DWORD i = 0; i < m_dwMaxConnectCount; i ++) {
                    if (m_pSlot[i].IsValid() == FALSE) {
                        continue;
                    }
                    m_pSlot[i].AddSendData(pCmd->pData, pCmd->dwSize, pCmd->byPriority);
                }
            } else if (pCmd->dwID >= URARASOCKSEL_IDBASE) {
                DWORD dwIndex = pCmd->dwID - URARASOCKSEL_IDBASE;
                if ((dwIndex < m_dwMaxConnectCount) && m_pSlot[dwIndex].IsValid()) {
                    m_pSlot[dwIndex].AddSendData(pCmd->pData, pCmd->dwSize, pCmd->byPriority);
                }
            }
            break;

        case URARASOCKSEL_CMD_CANCEL:
            if (pCmd->dwID >= URARASOCKSEL_IDBASE) {
                DWORD dwIndex = pCmd->dwID - URARASOCKSEL_IDBASE;
                if (dwIndex < m_dwMaxConnectCount) {
                    m_pSlot[dwIndex].CancelSend();
                }
            }
            break;

        case URARASOCKSEL_CMD_DELETE:
            if (pCmd->dwID >= URARASOCKSEL_IDBASE) {
                DWORD dwIndex = pCmd->dwID - URARASOCKSEL_IDBASE;
                if (dwIndex < m_dwMaxConnectCount) {
                    CloseSlot(dwIndex);
                }
            }
            break;
        }

        SAFE_DELETE_ARRAY(pCmd->pData);
        SAFE_DELETE(pCmd);
    }
}

// 送信データをフレーム化する
//
// 128バイト以上なら zlib で圧縮し、先頭に 0xFF と元サイズを付ける。
// そのうえで [dwSize][dwCRC] のヘッダを前置する。

void CUraraSockTCPSelect::MakeFrame(PBYTE pData, DWORD dwSize, PBYTE *ppFrame, DWORD *pdwFrameSize)
{
    PBYTE pBody     = NULL;
    DWORD dwBodySize = dwSize;
    DWORD dwCRC;
    DWORD dwFrameSize;
    PBYTE pFrame;

    *ppFrame      = NULL;
    *pdwFrameSize = 0;

    if ((dwSize >= URARASOCKSEL_USEZLIBSIZE) && (pData != NULL)) {
        PBYTE pTmp = new BYTE[dwSize + 5];
        ZeroMemory(pTmp, dwSize + 5);
        m_Zlib.DeflateInit();
        DWORD dwRet = m_Zlib.Deflate(pData, dwSize, &pTmp[5], dwSize);
        m_Zlib.DeflateEnd();
        if (dwRet != 0) {
            pTmp[0] = 0xFF;
            CopyMemory(&pTmp[1], &dwSize, sizeof(DWORD));
            pBody      = pTmp;
            dwBodySize = dwRet + 5;
        } else {
            SAFE_DELETE_ARRAY(pTmp);
        }
    }
    if ((pBody == NULL) && dwBodySize) {
        pBody = new BYTE[dwBodySize];
        if (pData) {
            CopyMemory(pBody, pData, dwBodySize);
        }
    }

    dwCRC       = m_pCrc->GetCRC(pBody, dwBodySize);
    dwFrameSize = sizeof(URARASOCKSEL_PACKETINFO) + dwBodySize;
    pFrame      = new BYTE[dwFrameSize];

    {
        URARASOCKSEL_PACKETINFO *pInfo = reinterpret_cast<URARASOCKSEL_PACKETINFO *>(pFrame);
        pInfo->dwSize = dwBodySize;
        pInfo->dwCRC  = dwCRC;
    }
    if (dwBodySize && pBody) {
        CopyMemory(pFrame + sizeof(URARASOCKSEL_PACKETINFO), pBody, dwBodySize);
    }
    SAFE_DELETE_ARRAY(pBody);

    *ppFrame      = pFrame;
    *pdwFrameSize = dwFrameSize;
}

BOOL CUraraSockTCPSelect::Host(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, DWORD dwCount)
{
    int nBuffer = URARASOCKSEL_SOCKBUFSIZE;
    u_long ulNonBlock = 1;
    SOCKADDR_IN addr;

    UNREFERENCED_PARAMETER(hWndParent);
    UNREFERENCED_PARAMETER(dwMsgBase);

    if (m_bHosting) {
        return FALSE;
    }
    if (m_pfNotify == NULL) {
        // ウィンドウを使わない実装なので、通知先が無いと何も届けられない
        return FALSE;
    }

    m_wPort             = wPort;
    m_dwPreCheckKey     = dwKey;
    m_dwMaxConnectCount = (dwCount == 0) ? 1 : dwCount;
    if (m_dwMaxConnectCount > URARASOCKSEL_MAXCONNECT) {
        m_dwMaxConnectCount = URARASOCKSEL_MAXCONNECT;
    }

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == INVALID_SOCKET) {
        return FALSE;
    }
    setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char *>(&nBuffer), sizeof(nBuffer));
    setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char *>(&nBuffer), sizeof(nBuffer));

    ZeroMemory(&addr, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(m_wPort);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(m_socket, reinterpret_cast<LPSOCKADDR>(&addr), sizeof(addr)) == SOCKET_ERROR) {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
        return FALSE;
    }
    if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR) {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
        return FALSE;
    }
    ioctlsocket(m_socket, FIONBIO, &ulNonBlock);

    if (CreateWakeupSocket() == FALSE) {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
        return FALSE;
    }

    m_pSlot = new CUraraSockTCPSelectSlot[m_dwMaxConnectCount];

    m_bStop  = FALSE;
    m_hReady = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hThread = reinterpret_cast<HANDLE>(_beginthread(ThreadEntry, 0, this));
    if ((m_hThread == reinterpret_cast<HANDLE>(-1)) || (m_hThread == NULL)) {
        CloseHandle(m_hReady);
        m_hReady = NULL;
        SAFE_DELETE_ARRAY(m_pSlot);
        CloseWakeupSocket();
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
        return FALSE;
    }
    WaitForSingleObject(m_hReady, INFINITE);
    CloseHandle(m_hReady);
    m_hReady = NULL;

    m_bHosting = TRUE;
    NotifyParent(WM_URARASOCK_HOST, 0, 0);
    return TRUE;
}

BOOL CUraraSockTCPSelect::Connect(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, LPCSTR pszAddr)
{
    UNREFERENCED_PARAMETER(hWndParent);
    UNREFERENCED_PARAMETER(dwMsgBase);
    UNREFERENCED_PARAMETER(dwKey);
    UNREFERENCED_PARAMETER(wPort);
    UNREFERENCED_PARAMETER(pszAddr);

    // クライアントモードは未対応。SboSvr はサーバーモードしか使わない。
    return FALSE;
}

void CUraraSockTCPSelect::Destroy(void)
{
    if (m_bHosting == FALSE) {
        return;
    }

    m_bStop = TRUE;
    Wakeup();
    if (m_hThread) {
        WaitForSingleObject(m_hThread, 5000);
        m_hThread = NULL;
    }

    ClearCommands();
    SAFE_DELETE_ARRAY(m_pSlot);
    CloseWakeupSocket();
    if (m_socket != INVALID_SOCKET) {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
    }
    m_dwConnectCount = 0;
    m_bHosting = FALSE;
}

void CUraraSockTCPSelect::ThreadEntry(void *pParam)
{
    CUraraSockTCPSelect *pThis = reinterpret_cast<CUraraSockTCPSelect *>(pParam);

    if (pThis) {
        pThis->ThreadMain();
    }
    _endthread();
}

void CUraraSockTCPSelect::ThreadMain(void)
{
    SetEvent(m_hReady);

    while (m_bStop == FALSE) {
        fd_set fdRead;
        fd_set fdWrite;
        timeval tv;
        int nRet;

        ProcCommands();

        FD_ZERO(&fdRead);
        FD_ZERO(&fdWrite);
        FD_SET(m_socket, &fdRead);
        FD_SET(m_sockWakeup, &fdRead);

        for (DWORD i = 0; i < m_dwMaxConnectCount; i ++) {
            if (m_pSlot[i].IsValid() == FALSE) {
                continue;
            }
            FD_SET(m_pSlot[i].m_socket, &fdRead);
            if (m_pSlot[i].HasSendData()) {
                FD_SET(m_pSlot[i].m_socket, &fdWrite);
            }
        }

        tv.tv_sec  = 0;
        tv.tv_usec = URARASOCKSEL_SELECT_WAIT_US;

        // 第1引数は Windows では無視される
        nRet = select(0, &fdRead, &fdWrite, NULL, &tv);
        if (nRet == SOCKET_ERROR) {
            break;
        }
        if (m_bStop) {
            break;
        }

        if (FD_ISSET(m_sockWakeup, &fdRead)) {
            DrainWakeup();
        }
        if (FD_ISSET(m_socket, &fdRead)) {
            OnAccept();
        }

        for (DWORD i = 0; i < m_dwMaxConnectCount; i ++) {
            if (m_pSlot[i].IsValid() == FALSE) {
                continue;
            }
            if (FD_ISSET(m_pSlot[i].m_socket, &fdWrite)) {
                if (m_pSlot[i].OnWritable() == FALSE) {
                    CloseSlot(i);
                    continue;
                }
            }
            if (m_pSlot[i].IsValid() == FALSE) {
                continue;
            }
            if (FD_ISSET(m_pSlot[i].m_socket, &fdRead)) {
                if (m_pSlot[i].OnReadable(this) == FALSE) {
                    CloseSlot(i);
                    continue;
                }
            }
        }

        CheckKeepalive();
    }

    // 残っている接続を閉じる
    for (DWORD i = 0; i < m_dwMaxConnectCount; i ++) {
        if (m_pSlot[i].IsValid()) {
            m_pSlot[i].Destroy();
        }
    }
}

void CUraraSockTCPSelect::OnAccept(void)
{
    int nIndex = -1;
    int nLen;
    SOCKADDR_IN addr;
    SOCKET hSocket;
    DWORD dwChallenge;
    PBYTE pFrame;
    DWORD dwFrameSize;

    for (DWORD i = 0; i < m_dwMaxConnectCount; i ++) {
        if (m_pSlot[i].IsValid() == FALSE) {
            nIndex = static_cast<int>(i);
            break;
        }
    }

    ZeroMemory(&addr, sizeof(addr));
    nLen    = sizeof(addr);
    hSocket = accept(m_socket, reinterpret_cast<sockaddr *>(&addr), &nLen);
    if (hSocket == INVALID_SOCKET) {
        return;
    }
    if (nIndex == -1) {
        // 空きが無い
        closesocket(hSocket);
        return;
    }

    if (m_pSlot[nIndex].Create(hSocket, addr.sin_addr.s_addr, ntohs(addr.sin_port), static_cast<DWORD>(nIndex)) == FALSE) {
        closesocket(hSocket);
        return;
    }

    // プリチェックのチャレンジを送る
    dwChallenge = GetTickCount();
    m_pSlot[nIndex].m_dwPreCheck = dwChallenge;
    m_dwConnectCount ++;

    MakeFrame(reinterpret_cast<PBYTE>(&dwChallenge), sizeof(DWORD), &pFrame, &dwFrameSize);
    m_pSlot[nIndex].AddSendData(pFrame, dwFrameSize, URARASOCK_SENDPRIORITY_MIDDLE);
    SAFE_DELETE_ARRAY(pFrame);
}

// プリチェックの応答を検証する

BOOL CUraraSockTCPSelect::OnPreCheck(DWORD dwIndex, DWORD dwAnswer)
{
    DWORD dwExpect;

    if (dwIndex >= m_dwMaxConnectCount) {
        return FALSE;
    }

    dwExpect = m_pSlot[dwIndex].m_dwPreCheck;
    if (m_dwPreCheckKey) {
        dwExpect &= m_dwPreCheckKey;
        dwExpect *= m_dwPreCheckKey;
    }
    if (m_dwPreCheckKey && (dwAnswer != dwExpect)) {
        return FALSE;
    }

    m_pSlot[dwIndex].m_bPreCheck = TRUE;
    NotifyParent(WM_URARASOCK_ADDCLIENT, 0, static_cast<LPARAM>(dwIndex + URARASOCKSEL_IDBASE));
    return TRUE;
}

// 受信したパケットを通知する
//
// pData の所有権は通知先へ渡る。解放は DeleteRecvData()。

void CUraraSockTCPSelect::OnRecvPacket(DWORD dwIndex, PBYTE pData, DWORD dwSize)
{
    UNREFERENCED_PARAMETER(dwSize);

    if (pData && (pData[0] == 0xFF)) {
        DWORD dwSourceSize = 0;
        CopyMemory(&dwSourceSize, &pData[1], sizeof(DWORD));
        if (dwSourceSize) {
            PBYTE pTmp = new BYTE[dwSourceSize];
            m_Zlib.InflateInit();
            DWORD dwRet = m_Zlib.Inflate(&pData[5], dwSourceSize, pTmp, dwSourceSize);
            m_Zlib.InflateEnd();
            if (dwRet) {
                SAFE_DELETE_ARRAY(pData);
                pData = pTmp;
            } else {
                SAFE_DELETE_ARRAY(pTmp);
                SAFE_DELETE_ARRAY(pData);
                return;
            }
        }
    }

    NotifyParent(WM_URARASOCK_RECV,
                 reinterpret_cast<WPARAM>(pData),
                 static_cast<LPARAM>(dwIndex + URARASOCKSEL_IDBASE));
}

void CUraraSockTCPSelect::CloseSlot(DWORD dwIndex)
{
    if (dwIndex >= m_dwMaxConnectCount) {
        return;
    }
    if (m_pSlot[dwIndex].IsValid() == FALSE) {
        return;
    }

    m_pSlot[dwIndex].Destroy();
    if (m_dwConnectCount > 0) {
        m_dwConnectCount --;
    }
    NotifyParent(WM_URARASOCK_DECCLIENT, 0, static_cast<LPARAM>(dwIndex + URARASOCKSEL_IDBASE));
}

// 一定時間受信が無い接続を切る

void CUraraSockTCPSelect::CheckKeepalive(void)
{
    DWORD dwTime = GetTickCount();

    for (DWORD i = 0; i < m_dwMaxConnectCount; i ++) {
        if (m_pSlot[i].IsValid() == FALSE) {
            continue;
        }
        if (dwTime > m_pSlot[i].m_dwTimeLastRecv + URARASOCKSEL_TIME_KEEPALIVE) {
            CloseSlot(i);
        }
    }
}

void CUraraSockTCPSelect::DeleteClient(DWORD dwID)
{
    URARASOCKSEL_CMD *pCmd = new URARASOCKSEL_CMD;

    pCmd->Type       = URARASOCKSEL_CMD_DELETE;
    pCmd->dwID       = dwID;
    pCmd->byPriority = URARASOCK_SENDPRIORITY_MIDDLE;
    pCmd->pData      = NULL;
    pCmd->dwSize     = 0;
    PushCommand(pCmd);
}

void CUraraSockTCPSelect::SendCancel(DWORD dwID)
{
    URARASOCKSEL_CMD *pCmd = new URARASOCKSEL_CMD;

    pCmd->Type       = URARASOCKSEL_CMD_CANCEL;
    pCmd->dwID       = dwID;
    pCmd->byPriority = URARASOCK_SENDPRIORITY_MIDDLE;
    pCmd->pData      = NULL;
    pCmd->dwSize     = 0;
    PushCommand(pCmd);
}

void CUraraSockTCPSelect::Send(PBYTE pData, DWORD dwSize, BYTE byPriority)
{
    // サーバーモードでは全員宛
    SendTo(URARASOCK_SENDALL, pData, dwSize, byPriority);
}

void CUraraSockTCPSelect::SendTo(DWORD dwID, PBYTE pData, DWORD dwSize, BYTE byPriority)
{
    PBYTE pFrame = NULL;
    DWORD dwFrameSize = 0;
    URARASOCKSEL_CMD *pCmd;

    if (m_bHosting == FALSE) {
        return;
    }
    if ((pData == NULL) && (dwSize != 0)) {
        return;
    }

    MakeFrame(pData, dwSize, &pFrame, &dwFrameSize);
    if (pFrame == NULL) {
        return;
    }

    pCmd = new URARASOCKSEL_CMD;
    pCmd->Type       = URARASOCKSEL_CMD_SEND;
    pCmd->dwID       = dwID;
    pCmd->byPriority = byPriority;
    pCmd->pData      = pFrame;
    pCmd->dwSize     = dwFrameSize;
    PushCommand(pCmd);
}

DWORD CUraraSockTCPSelect::GetThrowghPutSend(DWORD dwID)
{
    if ((m_pSlot == NULL) || (dwID < URARASOCKSEL_IDBASE)) {
        return 0;
    }
    {
        DWORD dwIndex = dwID - URARASOCKSEL_IDBASE;
        if (dwIndex >= m_dwMaxConnectCount) {
            return 0;
        }
        return m_pSlot[dwIndex].GetThrowghPutSend();
    }
}

DWORD CUraraSockTCPSelect::GetThrowghPutRecv(DWORD dwID)
{
    if ((m_pSlot == NULL) || (dwID < URARASOCKSEL_IDBASE)) {
        return 0;
    }
    {
        DWORD dwIndex = dwID - URARASOCKSEL_IDBASE;
        if (dwIndex >= m_dwMaxConnectCount) {
            return 0;
        }
        return m_pSlot[dwIndex].GetThrowghPutRecv();
    }
}

DWORD CUraraSockTCPSelect::GetQueCount(DWORD dwID)
{
    if ((m_pSlot == NULL) || (dwID < URARASOCKSEL_IDBASE)) {
        return 0;
    }
    {
        DWORD dwIndex = dwID - URARASOCKSEL_IDBASE;
        if (dwIndex >= m_dwMaxConnectCount) {
            return 0;
        }
        return m_pSlot[dwIndex].GetQueCount();
    }
}

DWORD CUraraSockTCPSelect::GetIPAddress(DWORD dwID)
{
    if ((m_pSlot == NULL) || (dwID < URARASOCKSEL_IDBASE)) {
        return 0;
    }
    {
        DWORD dwIndex = dwID - URARASOCKSEL_IDBASE;
        if (dwIndex >= m_dwMaxConnectCount) {
            return 0;
        }
        return m_pSlot[dwIndex].GetIPAddress();
    }
}

DWORD CUraraSockTCPSelect::GetPeerPort(DWORD dwID)
{
    if ((m_pSlot == NULL) || (dwID < URARASOCKSEL_IDBASE)) {
        return 0;
    }
    {
        DWORD dwIndex = dwID - URARASOCKSEL_IDBASE;
        if (dwIndex >= m_dwMaxConnectCount) {
            return 0;
        }
        return m_pSlot[dwIndex].GetPeerPort();
    }
}

CUraraSockTCP *GetUraraSockTCPSelect(void)
{
    return new CUraraSockTCPSelect();
}

#endif // _WIN32
