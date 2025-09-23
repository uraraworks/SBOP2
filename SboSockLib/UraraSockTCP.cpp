#include "StdAfx.h"

#include "UraraSockTCP.h"
#include "crc.h"
#include "CWinsockStart.h"
#ifndef URARASOCK_USEZLIB
#define URARASOCK_USEZLIB
#endif
#ifdef URARASOCK_USEZLIB
#include "myZlib/myZlib.h"
#endif
#include "myArray.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       do { if ((p) != NULL) { delete (p);     (p) = NULL; } } while (0)
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) do { if ((p) != NULL) { delete[] (p);   (p) = NULL; } } while (0)
#endif

#define WNDCLNAMESVR            _T("TCPMainWindowSvr")
#define WNDCLNAMECLI            _T("TCPMainWindowCli")

#define URARASOCK_MAXCONNECT    200
#define URARASOCK_IDBASE        100000
#define URARASOCK_RECVBUFSIZE   (1024 * 64)
#define URARASOCK_USEZLIBSIZE   (128)

#define TIMERID_KEEPALIVE       0
#define TIMERID_KEEPALIVE_CLI   1
#define TIME_KEEPALIVE          60000
#define TIME_KEEPALIVE_CLI      30000

enum
{
    WM_SOCKADDQUE = WM_APP + 200,
    WM_SOCKCANCELQUE,
    WM_SOCKPRECHECK,
    WM_INTERNAL_SEND,
    WM_INTERNAL_RECV,
    WM_SOCKEVENT,
};

enum
{
    URARASOCKMODE_SREVER = 0,
    URARASOCKMODE_CLIENT,
};

typedef struct _URARASOCK_PACKETINFO
{
    DWORD dwSize;
    DWORD dwCRC;
} URARASOCK_PACKETINFO, *PURARASOCK_PACKETINFO;

typedef struct _URARASOCK_QUEINFO
{
    URARASOCK_PACKETINFO PacketInfo;
    BYTE   byPriority;
    PBYTE  pData;
    DWORD  dwSize;
    DWORD  dwTimeMake;
    DWORD  dwTimeOut;
} URARASOCK_QUEINFO, *PURARASOCK_QUEINFO;

typedef struct _URARASOCK_ADDQUEINFO
{
    BYTE   byPriority;
    PBYTE  pData;
    DWORD  dwSize;
    DWORD  dwTimeOut;
    DWORD  dwCRC;
} URARASOCK_ADDQUEINFO, *PURARASOCK_ADDQUEINFO;

class CUraraSockTCPImplSlot
{
public:
    CUraraSockTCPImplSlot(void);
    virtual ~CUraraSockTCPImplSlot(void);

    BOOL Create(SOCKET socket, DWORD dwAddr, HWND hWndParent, DWORD dwID);
    void AddQue(PURARASOCK_ADDQUEINFO pQueAdd);
    void Combine(std::vector<PURARASOCK_QUEINFO> *pQue);
    void CancelQue(void);
    void Destroy(void);
    DWORD GetThrowghPutSend(void);
    DWORD GetThrowghPutRecv(void);
    DWORD GetQueCount(void);
    DWORD GetIPAddress(void);

    void OnFD_WRITE(void);
    void OnFD_READ(void);

public:
    SOCKET      m_socket;
    DWORD       m_dwSockID;
    DWORD       m_dwTimeLastRecv;
    DWORD       m_dwPreCheck;
    BOOL        m_bPreCheck;

private:
    void RenewThrowghPut(void);

private:
    CCRC                                        *m_pCrc;
    SOCKADDR_IN                                  m_sockAddr;
    WORD                                         m_wPort;
    DWORD                                        m_dwTimeConnect;
    DWORD                                        m_dwSendSize;
    DWORD                                        m_dwRecvSize;
    DWORD                                        m_dwRecvSizeTarget;
    DWORD                                        m_dwRecvCrc;
    DWORD                                        m_dwSendDataSize;
    DWORD                                        m_dwRecvDataSize;
    DWORD                                        m_dwThrowghPutSend;
    DWORD                                        m_dwThrowghPutRecv;
    DWORD                                        m_dwTimeThrowghPut;
    PBYTE                                        m_pRecvTmp;
    PBYTE                                        m_pRecvBuffer;
    HWND                                         m_hWndParent;
    std::vector<PURARASOCK_QUEINFO> *m_SendQueInfo;
    std::vector<PURARASOCK_QUEINFO>  m_QueInfoHi;
    std::vector<PURARASOCK_QUEINFO>  m_QueInfoMid;
    std::vector<PURARASOCK_QUEINFO>  m_QueInfoLow;
};

class CUraraSockTCPImpl : public CUraraSockTCP
{
public:
    CUraraSockTCPImpl(void);
    virtual ~CUraraSockTCPImpl(void);

    void DeleteRecvData(PBYTE pData) override;
    void Destroy(void) override;
    BOOL Host(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, DWORD dwCount) override;
    BOOL Connect(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, LPCSTR pszAddr) override;
    void DeleteClient(DWORD dwID) override;
    void SendCancel(DWORD dwID = 0) override;
    void Send(PBYTE pData, DWORD dwSize, BYTE byPriority = URARASOCK_SENDPRIORITY_MIDDLE) override;
    void SendTo(DWORD dwID, PBYTE pData, DWORD dwSize, BYTE byPriority = URARASOCK_SENDPRIORITY_MIDDLE) override;
    DWORD GetThrowghPutSend(DWORD dwID) override;
    DWORD GetThrowghPutRecv(DWORD dwID) override;
    DWORD GetQueCount(DWORD dwID) override;
    DWORD GetIPAddress(DWORD dwID) override;

private:
    static void __cdecl ThreadEntry(void *pParam);
    BOOL ThreadInit(void);
    void ThreadMain(void);
    void ThreadExit(void);

    void InitData(void);
    static LRESULT CALLBACK MainProcEntry(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    BOOL MainOnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
    void MainOnDestroy(HWND hWnd);
    void MainOnSockEvent(HWND hWnd, int nData, DWORD dwID);
    void OnSockACCEPT(void);
    void OnConnect(void);
    void OnSockClose(DWORD dwID);
    void OnSockWrite(DWORD dwID);
    void OnSockRead(DWORD dwID);
    void SendInternal(DWORD dwID, PBYTE pData, DWORD dwSize, DWORD dwTimeOut, BYTE byPriority);
    void PostParent(UINT uMsgOffset, WPARAM wParam, LPARAM lParam);

private:
    CCRC                    *m_pCrc;
    CWinsockStart            m_Winsock;
    SOCKADDR_IN              m_sockAddr;
    SOCKET                   m_socket;
    HANDLE                   m_hThread;
    HANDLE                   m_hEvent;
    WORD                     m_wPort;
    DWORD                    m_dwConnectCount;
    DWORD                    m_dwMaxConnectCount;
    HWND                     m_hWndParent;
    HWND                     m_hWnd;
    BYTE                     m_byMode;
    DWORD                    m_dwMsgBase;
    DWORD                    m_dwPreCheckKey;
    CUraraSockTCPImplSlot   *m_pSlot;
#ifdef URARASOCK_USEZLIB
    CmyZlib                  m_Zlib;
#endif
};

CUraraSockTCPImplSlot::CUraraSockTCPImplSlot(void)
    : m_socket(INVALID_SOCKET)
    , m_dwSockID(0)
    , m_dwTimeLastRecv(0)
    , m_dwPreCheck(0)
    , m_bPreCheck(FALSE)
    , m_pCrc(new CCRC)
    , m_wPort(0)
    , m_dwTimeConnect(0)
    , m_dwSendSize(0)
    , m_dwRecvSize(0)
    , m_dwRecvSizeTarget(0)
    , m_dwRecvCrc(0)
    , m_dwSendDataSize(0)
    , m_dwRecvDataSize(0)
    , m_dwThrowghPutSend(0)
    , m_dwThrowghPutRecv(0)
    , m_dwTimeThrowghPut(0)
    , m_pRecvTmp(NULL)
    , m_pRecvBuffer(NULL)
    , m_hWndParent(NULL)
    , m_SendQueInfo(NULL)
{
    ZeroMemory(&m_sockAddr, sizeof(m_sockAddr));
}

CUraraSockTCPImplSlot::~CUraraSockTCPImplSlot(void)
{
    Destroy();
    SAFE_DELETE(m_pCrc);
}

BOOL CUraraSockTCPImplSlot::Create(SOCKET socket, DWORD dwAddr, HWND hWndParent, DWORD dwID)
{
    if (m_socket != INVALID_SOCKET) {
        return FALSE;
    }
    if (hWndParent == NULL) {
        return FALSE;
    }

    m_socket = socket;
    m_dwSockID = dwID;
    m_hWndParent = hWndParent;
    m_sockAddr.sin_addr.s_addr = dwAddr;
    m_dwTimeLastRecv = m_dwTimeConnect = GetTickCount();
    m_pRecvBuffer = new BYTE[URARASOCK_RECVBUFSIZE];

    if (WSAAsyncSelect(m_socket, hWndParent, WM_SOCKEVENT + dwID, FD_READ | FD_WRITE | FD_CLOSE) != 0) {
        return FALSE;
    }

    return TRUE;
}

void CUraraSockTCPImplSlot::AddQue(PURARASOCK_ADDQUEINFO pQueAdd)
{
    if ((m_socket == INVALID_SOCKET) || (pQueAdd == NULL)) {
        return;
    }

    DWORD dwSize = pQueAdd->dwSize + sizeof(URARASOCK_PACKETINFO);
    PBYTE pTmp = new BYTE[dwSize];

    PURARASOCK_PACKETINFO pPacketInfo = reinterpret_cast<PURARASOCK_PACKETINFO>(pTmp);
    pPacketInfo->dwSize = pQueAdd->dwSize;
    pPacketInfo->dwCRC = pQueAdd->dwCRC;
    if (pQueAdd->dwSize && pQueAdd->pData) {
        CopyMemory(pTmp + sizeof(URARASOCK_PACKETINFO), pQueAdd->pData, pQueAdd->dwSize);
    }

    PURARASOCK_QUEINFO pQue = new URARASOCK_QUEINFO;
    pQue->PacketInfo.dwSize = pQueAdd->dwSize;
    pQue->PacketInfo.dwCRC = pQueAdd->dwCRC;
    pQue->byPriority = pQueAdd->byPriority;
    pQue->pData = pTmp;
    pQue->dwSize = dwSize;
    pQue->dwTimeMake = GetTickCount();
    pQue->dwTimeOut = pQueAdd->dwTimeOut;

    switch (pQue->byPriority) {
    case URARASOCK_SENDPRIORITY_HIGH:
        m_QueInfoHi.push_back(pQue);
        Combine(&m_QueInfoHi);
        break;
    case URARASOCK_SENDPRIORITY_MIDDLE:
        m_QueInfoMid.push_back(pQue);
        Combine(&m_QueInfoMid);
        break;
    case URARASOCK_SENDPRIORITY_LOW:
    default:
        m_QueInfoLow.push_back(pQue);
        Combine(&m_QueInfoLow);
        break;
    }

    PostMessage(m_hWndParent, WM_INTERNAL_SEND, static_cast<WPARAM>(m_dwSockID), 0);
}

void CUraraSockTCPImplSlot::Combine(std::vector<PURARASOCK_QUEINFO> *pQue)
{
    if (pQue == NULL) {
        return;
    }
    if (pQue == m_SendQueInfo) {
        return;
    }
    int nCount = pQue->size();
    if (nCount <= 1) {
        return;
    }

    PURARASOCK_QUEINFO pQueNew = new URARASOCK_QUEINFO;
    ZeroMemory(pQueNew, sizeof(URARASOCK_QUEINFO));

    for (int i = 0; i < nCount; ++i) {
        PURARASOCK_QUEINFO pOld = pQue->at(i);
        if (pOld == NULL) {
            continue;
        }
        DWORD dwNewSize = pQueNew->dwSize + pOld->dwSize;
        PBYTE pNewData = new BYTE[dwNewSize];
        if (pQueNew->pData && pQueNew->dwSize) {
            CopyMemory(pNewData, pQueNew->pData, pQueNew->dwSize);
        }
        if (pOld->pData && pOld->dwSize) {
            CopyMemory(pNewData + pQueNew->dwSize, pOld->pData, pOld->dwSize);
        }
        BYTE byPriority = pOld->byPriority;
        SAFE_DELETE_ARRAY(pQueNew->pData);
        SAFE_DELETE_ARRAY(pOld->pData);
        SAFE_DELETE(pOld);
        pQueNew->pData = pNewData;
        pQueNew->dwSize = dwNewSize;
        pQueNew->byPriority = byPriority;
    }
    pQue->clear();
    pQueNew->dwTimeMake = GetTickCount();
    pQue->push_back(pQueNew);
}

void CUraraSockTCPImplSlot::CancelQue(void)
{
    int nCount = m_QueInfoHi.size();
    for (int i = 0; i < nCount; ++i) {
        PURARASOCK_QUEINFO pInfo = m_QueInfoHi[i];
        SAFE_DELETE_ARRAY(pInfo->pData);
        SAFE_DELETE(pInfo);
    }
    m_QueInfoHi.clear();

    nCount = m_QueInfoMid.size();
    for (int i = 0; i < nCount; ++i) {
        PURARASOCK_QUEINFO pInfo = m_QueInfoMid[i];
        SAFE_DELETE_ARRAY(pInfo->pData);
        SAFE_DELETE(pInfo);
    }
    m_QueInfoMid.clear();

    nCount = m_QueInfoLow.size();
    for (int i = 0; i < nCount; ++i) {
        PURARASOCK_QUEINFO pInfo = m_QueInfoLow[i];
        SAFE_DELETE_ARRAY(pInfo->pData);
        SAFE_DELETE(pInfo);
    }
    m_QueInfoLow.clear();
}

void CUraraSockTCPImplSlot::Destroy(void)
{
    if (m_socket != INVALID_SOCKET) {
        shutdown(m_socket, SD_BOTH);
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
    }

    CancelQue();

    m_wPort = 0;
    m_dwTimeConnect = 0;
    m_dwTimeLastRecv = 0;
    m_dwSendSize = 0;
    m_dwRecvSize = 0;
    m_dwRecvSizeTarget = 0;
    m_dwRecvCrc = 0;
    m_dwSendDataSize = 0;
    m_dwRecvDataSize = 0;
    m_dwThrowghPutSend = 0;
    m_dwThrowghPutRecv = 0;
    m_dwTimeThrowghPut = 0;
    SAFE_DELETE_ARRAY(m_pRecvTmp);
    SAFE_DELETE_ARRAY(m_pRecvBuffer);
    m_SendQueInfo = NULL;
    m_bPreCheck = FALSE;
    m_dwPreCheck = 0;
    m_hWndParent = NULL;
    m_dwSockID = 0;
}

void CUraraSockTCPImplSlot::RenewThrowghPut(void)
{
    DWORD dwNow = GetTickCount();
    if ((dwNow - m_dwTimeThrowghPut) < 1000) {
        return;
    }
    m_dwTimeThrowghPut = dwNow;
    m_dwThrowghPutSend = m_dwSendDataSize;
    m_dwThrowghPutRecv = m_dwRecvDataSize;
    m_dwSendDataSize = 0;
    m_dwRecvDataSize = 0;
}

DWORD CUraraSockTCPImplSlot::GetThrowghPutSend(void)
{
    RenewThrowghPut();
    return m_dwThrowghPutSend;
}

DWORD CUraraSockTCPImplSlot::GetThrowghPutRecv(void)
{
    RenewThrowghPut();
    return m_dwThrowghPutRecv;
}

DWORD CUraraSockTCPImplSlot::GetQueCount(void)
{
    return static_cast<DWORD>(m_QueInfoHi.size() + m_QueInfoMid.size() + m_QueInfoLow.size());
}

DWORD CUraraSockTCPImplSlot::GetIPAddress(void)
{
    return m_sockAddr.sin_addr.S_un.S_addr;
}

void CUraraSockTCPImplSlot::OnFD_WRITE(void)
{
    if (m_socket == INVALID_SOCKET) {
        CancelQue();
        return;
    }

    while (true) {
        PURARASOCK_QUEINFO pQue = NULL;
        if (m_SendQueInfo == NULL) {
            if (m_QueInfoHi.size()) {
                m_SendQueInfo = &m_QueInfoHi;
            } else if (m_QueInfoMid.size()) {
                m_SendQueInfo = &m_QueInfoMid;
            } else if (m_QueInfoLow.size()) {
                m_SendQueInfo = &m_QueInfoLow;
            } else {
                break;
            }
            pQue = m_SendQueInfo->at(0);
            if (pQue == NULL) {
                m_SendQueInfo = NULL;
                break;
            }
            if (pQue->dwTimeOut && (GetTickCount() > pQue->dwTimeMake + pQue->dwTimeOut)) {
                SAFE_DELETE_ARRAY(pQue->pData);
                SAFE_DELETE(pQue);
                m_SendQueInfo->erase(m_SendQueInfo->begin());
                m_SendQueInfo = NULL;
                m_dwSendSize = 0;
                continue;
            }
        }

        if (m_SendQueInfo == NULL || m_SendQueInfo->size() <= 0) {
            break;
        }

        pQue = m_SendQueInfo->at(0);
        if (pQue == NULL) {
            m_SendQueInfo = NULL;
            break;
        }

        int nSendSize = static_cast<int>(pQue->dwSize - m_dwSendSize);
        int nRet = send(m_socket, reinterpret_cast<const char *>(pQue->pData + m_dwSendSize), nSendSize, 0);
        if (nRet > 0) {
            m_dwSendDataSize += nRet;
            RenewThrowghPut();
            m_dwSendSize += nRet;
            if (m_dwSendSize >= pQue->dwSize) {
                SAFE_DELETE_ARRAY(pQue->pData);
                SAFE_DELETE(pQue);
                m_SendQueInfo->erase(m_SendQueInfo->begin());
                m_SendQueInfo = NULL;
                m_dwSendSize = 0;
            }
        } else {
            DWORD dwError = WSAGetLastError();
            if (dwError != WSAEWOULDBLOCK) {
                PostMessage(m_hWndParent, WM_SOCKEVENT + m_dwSockID, static_cast<WPARAM>(m_socket), MAKELONG(FD_CLOSE, 0));
            }
            break;
        }
    }
}

void CUraraSockTCPImplSlot::OnFD_READ(void)
{
    if (m_socket == INVALID_SOCKET) {
        CancelQue();
        return;
    }

    DWORD dwRecvSize = 0;
    while (true) {
        int nRecvSize = static_cast<int>(URARASOCK_RECVBUFSIZE - dwRecvSize);
        int nRet = recv(m_socket, reinterpret_cast<char *>(m_pRecvBuffer + dwRecvSize), nRecvSize, 0);
        if (nRet > 0) {
            dwRecvSize += nRet;
            m_dwRecvDataSize += nRet;
            RenewThrowghPut();
            m_dwTimeLastRecv = GetTickCount();
        } else {
            DWORD dwError = WSAGetLastError();
            if (dwError != 0 && dwError != WSAEWOULDBLOCK) {
                PostMessage(m_hWndParent, WM_SOCKEVENT + m_dwSockID, static_cast<WPARAM>(m_socket), MAKELONG(FD_CLOSE, 0));
                return;
            }
            break;
        }
    }

    DWORD dwPos = 0;
    while (dwPos < dwRecvSize) {
        if (m_pRecvTmp == NULL) {
            m_pRecvTmp = new BYTE[sizeof(URARASOCK_PACKETINFO)];
            m_dwRecvSizeTarget = 0;
            m_dwRecvCrc = 0;
            m_dwRecvSize = 0;
        }

        int nNeed = (m_dwRecvSizeTarget == 0)
                        ? static_cast<int>(sizeof(URARASOCK_PACKETINFO) - m_dwRecvSize)
                        : static_cast<int>(m_dwRecvSizeTarget - m_dwRecvSize);
        int nRemain = static_cast<int>(dwRecvSize - dwPos);
        if (nNeed > nRemain) {
            nNeed = nRemain;
        }
        if (nNeed > 0) {
            CopyMemory(m_pRecvTmp + m_dwRecvSize, m_pRecvBuffer + dwPos, nNeed);
            dwPos += nNeed;
            m_dwRecvSize += nNeed;
        } else {
            break;
        }

        if (m_dwRecvSizeTarget == 0 && m_dwRecvSize >= sizeof(URARASOCK_PACKETINFO)) {
            PURARASOCK_PACKETINFO pInfo = reinterpret_cast<PURARASOCK_PACKETINFO>(m_pRecvTmp);
            m_dwRecvSizeTarget = pInfo->dwSize;
            m_dwRecvCrc = pInfo->dwCRC;
            m_dwRecvSize = 0;
            SAFE_DELETE_ARRAY(m_pRecvTmp);
            m_pRecvTmp = (m_dwRecvSizeTarget ? new BYTE[m_dwRecvSizeTarget] : NULL);

            if (!m_bPreCheck && (m_dwRecvSizeTarget != sizeof(DWORD))) {
                PostMessage(m_hWndParent, WM_SOCKEVENT + m_dwSockID, static_cast<WPARAM>(m_socket), MAKELONG(FD_CLOSE, 0));
                return;
            }
        }

        if (m_dwRecvSizeTarget && m_dwRecvSize >= m_dwRecvSizeTarget) {
            if (!m_bPreCheck) {
                if (m_dwRecvSize != sizeof(DWORD)) {
                    SAFE_DELETE_ARRAY(m_pRecvTmp);
                    PostMessage(m_hWndParent, WM_SOCKEVENT + m_dwSockID, static_cast<WPARAM>(m_socket), MAKELONG(FD_CLOSE, 0));
                    return;
                }
                PostMessage(m_hWndParent, WM_SOCKPRECHECK, reinterpret_cast<WPARAM>(m_pRecvTmp), static_cast<LPARAM>(m_dwSockID + URARASOCK_IDBASE));
            } else {
                if ((m_dwRecvCrc == 0) || (m_dwRecvCrc != m_pCrc->GetCRC(m_pRecvTmp, m_dwRecvSizeTarget))) {
                    SAFE_DELETE_ARRAY(m_pRecvTmp);
                    PostMessage(m_hWndParent, WM_SOCKEVENT + m_dwSockID, static_cast<WPARAM>(m_socket), MAKELONG(FD_CLOSE, 0));
                    return;
                }
                PostMessage(m_hWndParent, WM_INTERNAL_RECV, reinterpret_cast<WPARAM>(m_pRecvTmp), static_cast<LPARAM>(m_dwSockID + URARASOCK_IDBASE));
            }
            m_dwRecvSizeTarget = 0;
            m_dwRecvCrc = 0;
            m_dwRecvSize = 0;
            m_pRecvTmp = NULL;
        }
    }
}

CUraraSockTCPImpl::CUraraSockTCPImpl(void)
    : m_pCrc(new CCRC)
    , m_socket(INVALID_SOCKET)
    , m_hThread(NULL)
    , m_hEvent(NULL)
    , m_wPort(0)
    , m_dwConnectCount(0)
    , m_dwMaxConnectCount(1)
    , m_hWndParent(NULL)
    , m_hWnd(NULL)
    , m_byMode(URARASOCKMODE_SREVER)
    , m_dwMsgBase(0)
    , m_dwPreCheckKey(0)
    , m_pSlot(NULL)
{
    ZeroMemory(&m_sockAddr, sizeof(m_sockAddr));
}

CUraraSockTCPImpl::~CUraraSockTCPImpl(void)
{
    Destroy();
    SAFE_DELETE(m_pCrc);
}

void CUraraSockTCPImpl::DeleteRecvData(PBYTE pData)
{
    SAFE_DELETE_ARRAY(pData);
}

void CUraraSockTCPImpl::InitData(void)
{
    m_hWndParent = NULL;
    m_hWnd = NULL;
    m_wPort = 0;
    m_dwConnectCount = 0;
    m_dwMaxConnectCount = 1;
    m_byMode = URARASOCKMODE_SREVER;
    m_dwMsgBase = 0;
    m_dwPreCheckKey = 0;
    m_pSlot = NULL;
    m_hThread = NULL;
    m_hEvent = NULL;
    ZeroMemory(&m_sockAddr, sizeof(m_sockAddr));
    m_socket = INVALID_SOCKET;
}

void CUraraSockTCPImpl::Destroy(void)
{
    if (m_hThread && m_hThread != reinterpret_cast<HANDLE>(-1)) {
        if (IsWindow(m_hWnd)) {
            PostMessage(m_hWnd, WM_CLOSE, 0, 0);
        }
        WaitForSingleObject(m_hThread, INFINITE);
        m_hThread = NULL;
    }

    if (m_hEvent) {
        CloseHandle(m_hEvent);
        m_hEvent = NULL;
    }

    InitData();
}

BOOL CUraraSockTCPImpl::Host(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, DWORD dwCount)
{
    if ((m_hWndParent != NULL) || (hWndParent == NULL)) {
        return FALSE;
    }

    m_hWndParent = hWndParent;
    m_dwMsgBase = dwMsgBase;
    m_dwPreCheckKey = dwKey;
    m_wPort = wPort;
    m_dwMaxConnectCount = (dwCount == 0) ? 1 : dwCount;
    m_byMode = URARASOCKMODE_SREVER;

    m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (m_hEvent == NULL) {
        InitData();
        return FALSE;
    }

    m_hThread = reinterpret_cast<HANDLE>(_beginthread(ThreadEntry, 0, this));
    if (m_hThread == reinterpret_cast<HANDLE>(-1) || m_hThread == NULL) {
        CloseHandle(m_hEvent);
        m_hEvent = NULL;
        InitData();
        return FALSE;
    }

    WaitForSingleObject(m_hEvent, INFINITE);
    return IsWindow(m_hWnd);
}

BOOL CUraraSockTCPImpl::Connect(HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, LPCSTR pszAddr)
{
    if ((m_hWndParent != NULL) || (hWndParent == NULL) || (pszAddr == NULL)) {
        return FALSE;
    }

    DWORD dwAddr = m_Winsock.GetIPaddr(pszAddr);
    if (dwAddr == 0) {
        return FALSE;
    }

    m_sockAddr.sin_addr.s_addr = dwAddr;
    m_hWndParent = hWndParent;
    m_dwMsgBase = dwMsgBase;
    m_dwPreCheckKey = dwKey;
    m_wPort = wPort;
    m_byMode = URARASOCKMODE_CLIENT;

    m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (m_hEvent == NULL) {
        InitData();
        return FALSE;
    }

    m_hThread = reinterpret_cast<HANDLE>(_beginthread(ThreadEntry, 0, this));
    if (m_hThread == reinterpret_cast<HANDLE>(-1) || m_hThread == NULL) {
        CloseHandle(m_hEvent);
        m_hEvent = NULL;
        InitData();
        return FALSE;
    }

    WaitForSingleObject(m_hEvent, INFINITE);
    return IsWindow(m_hWnd);
}

void CUraraSockTCPImpl::DeleteClient(DWORD dwID)
{
    if (m_byMode != URARASOCKMODE_SREVER) {
        return;
    }
    if (dwID < URARASOCK_IDBASE) {
        return;
    }
    if (m_dwConnectCount == 0) {
        return;
    }
    DWORD dwIndex = dwID - URARASOCK_IDBASE;
    if (dwIndex >= m_dwMaxConnectCount) {
        return;
    }
    PostMessage(m_hWnd, WM_SOCKEVENT + dwIndex, static_cast<WPARAM>(m_pSlot[dwIndex].m_socket), MAKELONG(FD_CLOSE, 0));
}

void CUraraSockTCPImpl::SendCancel(DWORD dwID)
{
    if (m_dwConnectCount == 0) {
        return;
    }
    if (dwID < URARASOCK_IDBASE) {
        return;
    }
    DWORD dwIndex = dwID - URARASOCK_IDBASE;
    if (dwIndex >= m_dwMaxConnectCount) {
        return;
    }
    PostMessage(m_hWnd, WM_SOCKCANCELQUE, static_cast<WPARAM>(dwIndex), 0);
}

void CUraraSockTCPImpl::Send(PBYTE pData, DWORD dwSize, BYTE byPriority)
{
    SendInternal(URARASOCK_IDBASE, pData, dwSize, 0, byPriority);
}

void CUraraSockTCPImpl::SendTo(DWORD dwID, PBYTE pData, DWORD dwSize, BYTE byPriority)
{
    SendInternal(dwID, pData, dwSize, 0, byPriority);
}

DWORD CUraraSockTCPImpl::GetThrowghPutSend(DWORD dwID)
{
    if ((dwID < URARASOCK_IDBASE) || (m_pSlot == NULL)) {
        return 0;
    }
    DWORD dwIndex = dwID - URARASOCK_IDBASE;
    if (dwIndex >= m_dwMaxConnectCount) {
        return 0;
    }
    return m_pSlot[dwIndex].GetThrowghPutSend();
}

DWORD CUraraSockTCPImpl::GetThrowghPutRecv(DWORD dwID)
{
    if ((dwID < URARASOCK_IDBASE) || (m_pSlot == NULL)) {
        return 0;
    }
    DWORD dwIndex = dwID - URARASOCK_IDBASE;
    if (dwIndex >= m_dwMaxConnectCount) {
        return 0;
    }
    return m_pSlot[dwIndex].GetThrowghPutRecv();
}

DWORD CUraraSockTCPImpl::GetQueCount(DWORD dwID)
{
    if ((dwID < URARASOCK_IDBASE) || (m_pSlot == NULL)) {
        return 0;
    }
    DWORD dwIndex = dwID - URARASOCK_IDBASE;
    if (dwIndex >= m_dwMaxConnectCount) {
        return 0;
    }
    return m_pSlot[dwIndex].GetQueCount();
}

DWORD CUraraSockTCPImpl::GetIPAddress(DWORD dwID)
{
    if ((dwID < URARASOCK_IDBASE) || (m_pSlot == NULL)) {
        return 0;
    }
    DWORD dwIndex = dwID - URARASOCK_IDBASE;
    if (dwIndex >= m_dwMaxConnectCount) {
        return 0;
    }
    return m_pSlot[dwIndex].GetIPAddress();
}

void __cdecl CUraraSockTCPImpl::ThreadEntry(void *pParam)
{
    CUraraSockTCPImpl *pThis = reinterpret_cast<CUraraSockTCPImpl *>(pParam);
    BOOL bInit = pThis->ThreadInit();
    SetEvent(pThis->m_hEvent);
    if (bInit) {
        pThis->ThreadMain();
    }
    pThis->ThreadExit();
}

BOOL CUraraSockTCPImpl::ThreadInit(void)
{
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpfnWndProc = MainProcEntry;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = (m_byMode == URARASOCKMODE_SREVER) ? WNDCLNAMESVR : WNDCLNAMECLI;

    if (!RegisterClassEx(&wc)) {
        return FALSE;
    }

    m_hWnd = CreateWindow(wc.lpszClassName, wc.lpszClassName, WS_OVERLAPPEDWINDOW,
                          0, 0, 0, 0, NULL, NULL, wc.hInstance, this);
    return (m_hWnd != NULL);
}

void CUraraSockTCPImpl::ThreadMain(void)
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void CUraraSockTCPImpl::ThreadExit(void)
{
    LPCTSTR pszClass = (m_byMode == URARASOCKMODE_SREVER) ? WNDCLNAMESVR : WNDCLNAMECLI;
    UnregisterClass(pszClass, GetModuleHandle(NULL));
    m_hWnd = NULL;
}

LRESULT CALLBACK CUraraSockTCPImpl::MainProcEntry(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_CREATE) {
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams));
    }

    CUraraSockTCPImpl *pThis = reinterpret_cast<CUraraSockTCPImpl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (pThis) {
        return pThis->WndProc(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CUraraSockTCPImpl::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_CREATE:
        if (!MainOnCreate(hWnd, reinterpret_cast<LPCREATESTRUCT>(lParam))) {
            return -1;
        }
        break;
    case WM_DESTROY:
        MainOnDestroy(hWnd);
        break;
    case WM_TIMER:
        if (wParam == TIMERID_KEEPALIVE) {
            DWORD dwTime = GetTickCount();
            for (DWORD i = 0, count = m_dwMaxConnectCount; i < count; ++i) {
                if (m_pSlot[i].m_socket == INVALID_SOCKET) {
                    continue;
                }
                if (dwTime > m_pSlot[i].m_dwTimeLastRecv + TIME_KEEPALIVE) {
                    PostMessage(hWnd, WM_SOCKEVENT + i, static_cast<WPARAM>(m_pSlot[i].m_socket), MAKELONG(FD_CLOSE, 0));
                }
            }
        } else if (wParam == TIMERID_KEEPALIVE_CLI) {
            Send(NULL, 0, URARASOCK_SENDPRIORITY_MIDDLE);
        }
        break;
    case WM_SOCKADDQUE:
        {
            PURARASOCK_ADDQUEINFO pInfo = reinterpret_cast<PURARASOCK_ADDQUEINFO>(wParam);
            if (pInfo == NULL) {
                break;
            }
            if (lParam == URARASOCK_SENDALL) {
                DWORD nCount = 0;
                for (DWORD i = 0; i < m_dwMaxConnectCount; ++i) {
                    if (m_pSlot[i].m_socket == INVALID_SOCKET) {
                        continue;
                    }
                    m_pSlot[i].AddQue(pInfo);
                    if (++nCount >= m_dwConnectCount) {
                        break;
                    }
                }
            } else {
                DWORD dwIndex = static_cast<DWORD>(lParam) - URARASOCK_IDBASE;
                if (dwIndex < m_dwMaxConnectCount) {
                    m_pSlot[dwIndex].AddQue(pInfo);
                }
            }
            SAFE_DELETE_ARRAY(pInfo->pData);
            SAFE_DELETE(pInfo);
        }
        break;
    case WM_SOCKCANCELQUE:
        if (m_pSlot != NULL) {
            DWORD dwIndex = static_cast<DWORD>(wParam);
            if (dwIndex < m_dwMaxConnectCount) {
                m_pSlot[dwIndex].CancelQue();
            }
        }
        break;
    case WM_SOCKPRECHECK:
        {
            PBYTE pData = reinterpret_cast<PBYTE>(wParam);
            DWORD dwData = 0;
            CopyMemory(&dwData, pData, sizeof(DWORD));
            DWORD dwID = static_cast<DWORD>(lParam);
            if (dwID < URARASOCK_IDBASE) {
                SAFE_DELETE_ARRAY(pData);
                break;
            }
            DWORD dwIndex = dwID - URARASOCK_IDBASE;
            if (dwIndex >= m_dwMaxConnectCount) {
                SAFE_DELETE_ARRAY(pData);
                break;
            }
            if (m_byMode == URARASOCKMODE_SREVER) {
                DWORD dwTmp = m_pSlot[dwIndex].m_dwPreCheck;
                if (m_dwPreCheckKey) {
                    dwTmp &= m_dwPreCheckKey;
                    dwTmp *= m_dwPreCheckKey;
                }
                if (!m_dwPreCheckKey || dwData == dwTmp) {
                    m_pSlot[dwIndex].m_bPreCheck = TRUE;
                    PostParent(WM_URARASOCK_ADDCLIENT, 0, lParam);
                } else {
                    DeleteClient(dwID);
                }
            } else {
                if (m_dwPreCheckKey) {
                    dwData &= m_dwPreCheckKey;
                    dwData *= m_dwPreCheckKey;
                } else {
                    dwData = 0;
                }
                Send(reinterpret_cast<PBYTE>(&dwData), sizeof(DWORD), URARASOCK_SENDPRIORITY_MIDDLE);
                m_pSlot[dwIndex].m_bPreCheck = TRUE;
                PostParent(WM_URARASOCK_CONNECT, 0, 0);
            }
            SAFE_DELETE_ARRAY(pData);
        }
        break;
    case WM_INTERNAL_RECV:
        {
            PBYTE pData = reinterpret_cast<PBYTE>(wParam);
#ifdef URARASOCK_USEZLIB
            if (pData && pData[0] == 0xFF) {
                DWORD dwSourceSize = 0;
                CopyMemory(&dwSourceSize, &pData[1], 4);
                LPBYTE pTmp = new BYTE[dwSourceSize];
                m_Zlib.InflateInit();
                DWORD dwRet = m_Zlib.Inflate(&pData[5], dwSourceSize, pTmp, dwSourceSize);
                m_Zlib.InflateEnd();
                if (dwRet) {
                    SAFE_DELETE_ARRAY(pData);
                    pData = pTmp;
                } else {
                    SAFE_DELETE_ARRAY(pTmp);
                    SAFE_DELETE_ARRAY(pData);
                    break;
                }
            }
#endif
            PostParent(WM_URARASOCK_RECV, reinterpret_cast<WPARAM>(pData), lParam);
        }
        break;
    case WM_INTERNAL_SEND:
        if (m_pSlot != NULL) {
            DWORD dwIndex = static_cast<DWORD>(wParam);
            if (dwIndex < m_dwMaxConnectCount) {
                CUraraSockTCPImplSlot *pSlot = &m_pSlot[dwIndex];
                PostMessage(hWnd, WM_SOCKEVENT + pSlot->m_dwSockID, static_cast<WPARAM>(pSlot->m_socket), MAKELONG(FD_WRITE, 0));
            }
        }
        break;
    default:
        if (message >= WM_SOCKEVENT && message < WM_SOCKEVENT + URARASOCK_MAXCONNECT) {
            MainOnSockEvent(hWnd, static_cast<int>(lParam), message - WM_SOCKEVENT);
        } else {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    }
    return 0;
}

BOOL CUraraSockTCPImpl::MainOnCreate(HWND hWnd, LPCREATESTRUCT)
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == INVALID_SOCKET) {
        return FALSE;
    }
    int nBuffer = 1024 * 64;
    setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char *>(&nBuffer), sizeof(nBuffer));
    setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char *>(&nBuffer), sizeof(nBuffer));

    if (m_byMode == URARASOCKMODE_SREVER) {
        ZeroMemory(&m_sockAddr, sizeof(m_sockAddr));
        m_sockAddr.sin_family = AF_INET;
        m_sockAddr.sin_port = htons(m_wPort);
        m_sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(m_socket, reinterpret_cast<LPSOCKADDR>(&m_sockAddr), sizeof(m_sockAddr)) == SOCKET_ERROR) {
            return FALSE;
        }
        if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR) {
            return FALSE;
        }
        if (WSAAsyncSelect(m_socket, hWnd, WM_SOCKEVENT, FD_ACCEPT | FD_READ | FD_WRITE | FD_CLOSE) != 0) {
            return FALSE;
        }
        m_pSlot = new CUraraSockTCPImplSlot[m_dwMaxConnectCount];
        PostParent(WM_URARASOCK_HOST, 0, 0);
        SetTimer(hWnd, TIMERID_KEEPALIVE, TIME_KEEPALIVE, NULL);
    } else {
        m_sockAddr.sin_family = AF_INET;
        m_sockAddr.sin_port = htons(m_wPort);
        if (WSAAsyncSelect(m_socket, hWnd, WM_SOCKEVENT, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE) != 0) {
            return FALSE;
        }
        if (connect(m_socket, reinterpret_cast<struct sockaddr *>(&m_sockAddr), sizeof(m_sockAddr)) != 0) {
            if (WSAGetLastError() != WSAEWOULDBLOCK) {
                return FALSE;
            }
        }
        m_pSlot = new CUraraSockTCPImplSlot;
        m_dwMaxConnectCount = 1;
        SetTimer(hWnd, TIMERID_KEEPALIVE_CLI, TIME_KEEPALIVE_CLI, NULL);
    }
    return TRUE;
}

void CUraraSockTCPImpl::MainOnDestroy(HWND hWnd)
{
    if (m_pSlot != NULL) {
        if (m_byMode == URARASOCKMODE_SREVER) {
            delete[] m_pSlot;
        } else {
            delete m_pSlot;
        }
        m_pSlot = NULL;
    }

    if (m_socket != INVALID_SOCKET) {
        shutdown(m_socket, SD_BOTH);
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
    }

    KillTimer(hWnd, TIMERID_KEEPALIVE);
    KillTimer(hWnd, TIMERID_KEEPALIVE_CLI);
    PostQuitMessage(0);
}

void CUraraSockTCPImpl::MainOnSockEvent(HWND, int nData, DWORD dwID)
{
    int nEvent = WSAGETSELECTEVENT(nData);
    int nError = WSAGETSELECTERROR(nData);
    if (nError) {
        OnSockClose(dwID);
        return;
    }

    switch (nEvent) {
    case FD_ACCEPT:
        OnSockACCEPT();
        break;
    case FD_CONNECT:
        OnConnect();
        break;
    case FD_WRITE:
        OnSockWrite(dwID);
        break;
    case FD_READ:
        OnSockRead(dwID);
        break;
    case FD_CLOSE:
        OnSockClose(dwID);
        break;
    }
}

void CUraraSockTCPImpl::OnSockACCEPT(void)
{
    int nIndex = -1;
    for (DWORD i = 0; i < m_dwMaxConnectCount; ++i) {
        if (m_pSlot[i].m_socket == INVALID_SOCKET) {
            nIndex = static_cast<int>(i);
            break;
        }
    }
    if (nIndex == -1) {
        return;
    }

    SOCKADDR_IN addr;
    ZeroMemory(&addr, sizeof(addr));
    int nLen = sizeof(addr);
    SOCKET hSocket = accept(m_socket, reinterpret_cast<sockaddr *>(&addr), &nLen);
    if (hSocket == INVALID_SOCKET) {
        return;
    }

    if (!m_pSlot[nIndex].Create(hSocket, addr.sin_addr.s_addr, m_hWnd, nIndex)) {
        closesocket(hSocket);
        return;
    }
    DWORD dwData = GetTickCount();
    m_pSlot[nIndex].m_dwPreCheck = dwData;
    ++m_dwConnectCount;
    SendInternal(nIndex + URARASOCK_IDBASE, reinterpret_cast<PBYTE>(&dwData), sizeof(DWORD), 0, URARASOCK_SENDPRIORITY_MIDDLE);
}

void CUraraSockTCPImpl::OnConnect(void)
{
    if (m_pSlot == NULL) {
        return;
    }
    if (!m_pSlot->Create(m_socket, m_sockAddr.sin_addr.s_addr, m_hWnd, 0)) {
        return;
    }
    ++m_dwConnectCount;
}

void CUraraSockTCPImpl::OnSockClose(DWORD dwID)
{
    if (m_pSlot == NULL || dwID >= m_dwMaxConnectCount) {
        return;
    }
    if (m_pSlot[dwID].m_socket == INVALID_SOCKET) {
        return;
    }
    m_pSlot[dwID].Destroy();
    if (m_dwConnectCount > 0) {
        --m_dwConnectCount;
    }

    if (m_byMode == URARASOCKMODE_SREVER) {
        PostParent(WM_URARASOCK_DECCLIENT, 0, static_cast<LPARAM>(dwID + URARASOCK_IDBASE));
    } else {
        PostParent(WM_URARASOCK_DISCONNECT, 0, 0);
    }
}

void CUraraSockTCPImpl::OnSockWrite(DWORD dwID)
{
    if (m_pSlot == NULL) {
        return;
    }
    if (m_byMode == URARASOCKMODE_SREVER) {
        if (dwID < m_dwMaxConnectCount) {
            m_pSlot[dwID].OnFD_WRITE();
        }
    } else {
        m_pSlot[0].OnFD_WRITE();
    }
}

void CUraraSockTCPImpl::OnSockRead(DWORD dwID)
{
    if (m_pSlot == NULL) {
        return;
    }
    if (m_byMode == URARASOCKMODE_SREVER) {
        if (dwID < m_dwMaxConnectCount) {
            m_pSlot[dwID].OnFD_READ();
        }
    } else {
        m_pSlot[0].OnFD_READ();
    }
}

void CUraraSockTCPImpl::SendInternal(DWORD dwID, PBYTE pData, DWORD dwSize, DWORD dwTimeOut, BYTE byPriority)
{
    if (m_pSlot == NULL || m_dwConnectCount == 0) {
        return;
    }
    if (dwID != URARASOCK_SENDALL) {
        if (dwID < URARASOCK_IDBASE) {
            return;
        }
        if ((dwID - URARASOCK_IDBASE) >= m_dwMaxConnectCount) {
            return;
        }
    }
    if (pData == NULL && dwSize != 0) {
        return;
    }

    PBYTE pDataSend = NULL;
#ifdef URARASOCK_USEZLIB
    if (dwSize >= URARASOCK_USEZLIBSIZE && pData != NULL) {
        LPBYTE pTmp = new BYTE[dwSize + 5];
        ZeroMemory(pTmp, dwSize + 5);
        m_Zlib.DeflateInit();
        DWORD dwRet = m_Zlib.Deflate(pData, dwSize, &pTmp[5], dwSize);
        m_Zlib.DeflateEnd();
        if (dwRet != 0) {
            pTmp[0] = 0xFF;
            CopyMemory(&pTmp[1], &dwSize, sizeof(DWORD));
            pDataSend = pTmp;
            dwSize = dwRet + 5;
        } else {
            SAFE_DELETE_ARRAY(pTmp);
        }
    }
#endif
    if (pDataSend == NULL && dwSize) {
        pDataSend = new BYTE[dwSize];
        if (pData && dwSize) {
            CopyMemory(pDataSend, pData, dwSize);
        }
    }

    PURARASOCK_ADDQUEINFO pQue = new URARASOCK_ADDQUEINFO;
    pQue->byPriority = byPriority;
    pQue->pData = pDataSend;
    pQue->dwSize = dwSize;
    pQue->dwTimeOut = dwTimeOut;
    pQue->dwCRC = m_pCrc->GetCRC(pDataSend, dwSize);

    PostMessage(m_hWnd, WM_SOCKADDQUE, reinterpret_cast<WPARAM>(pQue), dwID);
}

void CUraraSockTCPImpl::PostParent(UINT uMsgOffset, WPARAM wParam, LPARAM lParam)
{
    if (m_hWndParent) {
        PostMessage(m_hWndParent, m_dwMsgBase + uMsgOffset, wParam, lParam);
    }
}

extern "C" DLLURARASOCKTCP_API CUraraSockTCP *GetUraraSockTCP(void)
{
    return new CUraraSockTCPImpl();
}

extern "C" DLLURARASOCKTCP_API void ReleaseUraraSockTCP(CUraraSockTCP *&pSrc)
{
    if (pSrc) {
        delete pSrc;
        pSrc = NULL;
    }
}

