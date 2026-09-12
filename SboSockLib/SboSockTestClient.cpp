/// @file SboSockTestClient.cpp
/// @brief SBO プロトコルを喋る同期クライアント 実装ファイル
/// @copyright Copyright(C)URARA-works

#include "StdAfx.h"
#include "SboSockTestClient.h"

#ifdef _WIN32

#include "crc.h"
#include "myZlib/myZlib.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       do { if ((p) != NULL) { delete (p);     (p) = NULL; } } while (0)
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) do { if ((p) != NULL) { delete[] (p);   (p) = NULL; } } while (0)
#endif

#define SBOSOCKTEST_USEZLIBSIZE (128)
#define SBOSOCKTEST_MAXPACKET   (1024 * 1024)

typedef struct _SBOSOCKTEST_PACKETINFO
{
    DWORD dwSize;
    DWORD dwCRC;
} SBOSOCKTEST_PACKETINFO;

CSboSockTestClient::CSboSockTestClient(void)
    : m_socket(INVALID_SOCKET)
    , m_pCrc(NULL)
    , m_pZlib(NULL)
{
    m_pCrc  = new CCRC;
    m_pZlib = new CmyZlib;
}

CSboSockTestClient::~CSboSockTestClient(void)
{
    Close();
    SAFE_DELETE(m_pCrc);
    SAFE_DELETE(m_pZlib);
}

BOOL CSboSockTestClient::IsConnected(void) const
{
    return (m_socket != INVALID_SOCKET) ? TRUE : FALSE;
}

WORD CSboSockTestClient::GetLocalPort(void) const
{
    SOCKADDR_IN addr;
    int nLen = sizeof(addr);

    if (m_socket == INVALID_SOCKET) {
        return 0;
    }
    ZeroMemory(&addr, sizeof(addr));
    if (getsockname(m_socket, reinterpret_cast<sockaddr *>(&addr), &nLen) == SOCKET_ERROR) {
        return 0;
    }
    return ntohs(addr.sin_port);
}

void CSboSockTestClient::Close(void)
{
    if (m_socket != INVALID_SOCKET) {
        shutdown(m_socket, SD_BOTH);
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
    }
}

void CSboSockTestClient::ShutdownSend(void)
{
    if (m_socket != INVALID_SOCKET) {
        shutdown(m_socket, SD_SEND);
    }
}

BOOL CSboSockTestClient::SendAll(const BYTE *pBuf, DWORD dwSize)
{
    DWORD dwSent = 0;

    while (dwSent < dwSize) {
        int nRet = send(m_socket, reinterpret_cast<const char *>(pBuf + dwSent),
                        static_cast<int>(dwSize - dwSent), 0);
        if (nRet <= 0) {
            return FALSE;
        }
        dwSent += nRet;
    }
    return TRUE;
}

BOOL CSboSockTestClient::RecvExact(BYTE *pBuf, DWORD dwSize, int nTimeoutMs)
{
    DWORD dwGot  = 0;
    DWORD dwEnd  = GetTickCount() + nTimeoutMs;

    while (dwGot < dwSize) {
        fd_set fdRead;
        timeval tv;
        int nRet;
        DWORD dwNow = GetTickCount();

        if (dwNow >= dwEnd) {
            return FALSE;
        }
        FD_ZERO(&fdRead);
        FD_SET(m_socket, &fdRead);
        tv.tv_sec  = 0;
        tv.tv_usec = 50000;

        nRet = select(0, &fdRead, NULL, NULL, &tv);
        if (nRet == SOCKET_ERROR) {
            return FALSE;
        }
        if (nRet == 0) {
            continue;
        }

        nRet = recv(m_socket, reinterpret_cast<char *>(pBuf + dwGot),
                    static_cast<int>(dwSize - dwGot), 0);
        if (nRet <= 0) {
            return FALSE;
        }
        dwGot += nRet;
    }
    return TRUE;
}

// 本体をフレーム化して送る
//
// 128バイト以上なら zlib 圧縮し、先頭に 0xFF と元サイズを付ける。
// そのうえで [dwSize][dwCRC] を前置する。

BOOL CSboSockTestClient::SendFrame(const BYTE *pBody, DWORD dwSize)
{
    std::vector<BYTE> vecFrame;
    SBOSOCKTEST_PACKETINFO Info;

    Info.dwSize = dwSize;
    Info.dwCRC  = m_pCrc->GetCRC(const_cast<PBYTE>(pBody), dwSize);

    vecFrame.resize(sizeof(SBOSOCKTEST_PACKETINFO) + dwSize);
    CopyMemory(&vecFrame[0], &Info, sizeof(Info));
    if (dwSize && pBody) {
        CopyMemory(&vecFrame[sizeof(Info)], pBody, dwSize);
    }

    return SendAll(&vecFrame[0], static_cast<DWORD>(vecFrame.size()));
}

BOOL CSboSockTestClient::Connect(LPCSTR pszAddr, WORD wPort, DWORD dwKey, BOOL bWrongAnswer)
{
    SOCKADDR_IN addr;
    BYTE byHeader[sizeof(SBOSOCKTEST_PACKETINFO)];
    SBOSOCKTEST_PACKETINFO *pInfo;
    DWORD dwChallenge = 0;
    DWORD dwAnswer;

    Close();

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == INVALID_SOCKET) {
        return FALSE;
    }

    ZeroMemory(&addr, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(wPort);
    addr.sin_addr.s_addr = inet_addr(pszAddr);

    if (connect(m_socket, reinterpret_cast<LPSOCKADDR>(&addr), sizeof(addr)) != 0) {
        Close();
        return FALSE;
    }

    // サーバーからチャレンジを受け取る
    if (RecvExact(byHeader, sizeof(byHeader), 3000) == FALSE) {
        Close();
        return FALSE;
    }
    pInfo = reinterpret_cast<SBOSOCKTEST_PACKETINFO *>(byHeader);
    if (pInfo->dwSize != sizeof(DWORD)) {
        Close();
        return FALSE;
    }
    if (RecvExact(reinterpret_cast<BYTE *>(&dwChallenge), sizeof(DWORD), 3000) == FALSE) {
        Close();
        return FALSE;
    }

    // (チャレンジ & キー) * キー を返す
    dwAnswer = dwChallenge;
    if (dwKey) {
        dwAnswer &= dwKey;
        dwAnswer *= dwKey;
    }
    if (bWrongAnswer) {
        dwAnswer = ~dwAnswer;
    }

    if (SendFrame(reinterpret_cast<BYTE *>(&dwAnswer), sizeof(DWORD)) == FALSE) {
        Close();
        return FALSE;
    }
    return TRUE;
}

BOOL CSboSockTestClient::SendPacket(const BYTE *pData, DWORD dwSize)
{
    if (m_socket == INVALID_SOCKET) {
        return FALSE;
    }

    if ((dwSize >= SBOSOCKTEST_USEZLIBSIZE) && (pData != NULL)) {
        std::vector<BYTE> vecComp;
        DWORD dwRet;

        vecComp.resize(dwSize + 5);
        ZeroMemory(&vecComp[0], vecComp.size());
        m_pZlib->DeflateInit();
        dwRet = m_pZlib->Deflate(const_cast<PBYTE>(pData), dwSize, &vecComp[5], dwSize);
        m_pZlib->DeflateEnd();
        if (dwRet != 0) {
            vecComp[0] = 0xFF;
            CopyMemory(&vecComp[1], &dwSize, sizeof(DWORD));
            return SendFrame(&vecComp[0], dwRet + 5);
        }
    }
    return SendFrame(pData, dwSize);
}

BOOL CSboSockTestClient::RecvPacket(std::vector<BYTE> *pvecOut, int nTimeoutMs)
{
    BYTE byHeader[sizeof(SBOSOCKTEST_PACKETINFO)];
    SBOSOCKTEST_PACKETINFO *pInfo;
    std::vector<BYTE> vecBody;

    if ((m_socket == INVALID_SOCKET) || (pvecOut == NULL)) {
        return FALSE;
    }
    pvecOut->clear();

    if (RecvExact(byHeader, sizeof(byHeader), nTimeoutMs) == FALSE) {
        return FALSE;
    }
    pInfo = reinterpret_cast<SBOSOCKTEST_PACKETINFO *>(byHeader);
    if (pInfo->dwSize > SBOSOCKTEST_MAXPACKET) {
        return FALSE;
    }
    if (pInfo->dwSize == 0) {
        // 生存確認。中身は無い。
        return TRUE;
    }

    vecBody.resize(pInfo->dwSize);
    if (RecvExact(&vecBody[0], pInfo->dwSize, nTimeoutMs) == FALSE) {
        return FALSE;
    }

    if (vecBody[0] == 0xFF) {
        DWORD dwSourceSize = 0;
        DWORD dwRet;

        CopyMemory(&dwSourceSize, &vecBody[1], sizeof(DWORD));
        if ((dwSourceSize == 0) || (dwSourceSize > SBOSOCKTEST_MAXPACKET)) {
            return FALSE;
        }
        pvecOut->resize(dwSourceSize);
        m_pZlib->InflateInit();
        dwRet = m_pZlib->Inflate(&vecBody[5], static_cast<DWORD>(vecBody.size() - 5),
                                 &(*pvecOut)[0], dwSourceSize);
        m_pZlib->InflateEnd();
        if (dwRet == 0) {
            pvecOut->clear();
            return FALSE;
        }
        return TRUE;
    }

    *pvecOut = vecBody;
    return TRUE;
}

BOOL CSboSockTestClient::WaitForDisconnect(int nTimeoutMs)
{
    DWORD dwEnd = GetTickCount() + nTimeoutMs;

    if (m_socket == INVALID_SOCKET) {
        return TRUE;
    }

    while (GetTickCount() < dwEnd) {
        fd_set fdRead;
        timeval tv;
        int nRet;
        char szTmp[256];

        FD_ZERO(&fdRead);
        FD_SET(m_socket, &fdRead);
        tv.tv_sec  = 0;
        tv.tv_usec = 50000;

        nRet = select(0, &fdRead, NULL, NULL, &tv);
        if (nRet == SOCKET_ERROR) {
            return TRUE;
        }
        if (nRet == 0) {
            continue;
        }
        nRet = recv(m_socket, szTmp, sizeof(szTmp), 0);
        if (nRet <= 0) {
            // 0 は正常クローズ
            return TRUE;
        }
        // 何か届いた場合は読み捨てて待ち続ける
    }
    return FALSE;
}

#endif // _WIN32
