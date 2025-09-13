#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>
#include <process.h>
#include <winsock.h>
#include "crc.h"
#include "CUraraSockTCP.h"

// コンストラクタ
CUraraSockTCP::CUraraSockTCP(void)
{
    m_pCrc = new CCRC;
    InitData();
}

// デストラクタ
CUraraSockTCP::~CUraraSockTCP(void)
{
    Destroy();
    SAFE_DELETE(m_pCrc);
}

// 受信データ解放
void CUraraSockTCP::DeleteRecvData(PBYTE pData)
{
    SAFE_DELETE_ARRAY(pData);
}

// データ初期化
void CUraraSockTCP::InitData(void)
{
    m_hWndParent = NULL;
    m_hWnd = NULL;
    m_hThread = NULL;
    m_hEvent = NULL;
    m_wPort = 0;
    m_dwConnectCount = 0;
    m_dwMaxConnectCount = 1;
    m_dwSendID = 0;
    m_pSlot = NULL;
    m_byMode = URARASOCKMODE_SREVER;
    ZeroMemory(&m_socket, sizeof(m_socket));
    ZeroMemory(&m_sockAddr, sizeof(m_sockAddr));
}

// 後始末
void CUraraSockTCP::Destroy(void)
{
    if (m_hThread != INVALID_HANDLE_VALUE && m_hThread != NULL) {
        if (IsWindow(m_hWnd)) {
            PostMessage(m_hWnd, WM_CLOSE, 0, 0);
        }
        WaitForSingleObject(m_hThread, INFINITE);
    }

    if (m_hEvent) {
        CloseHandle(m_hEvent);
    }
    InitData();
}

// 接続待ち開始
BOOL CUraraSockTCP::Host(HWND hWndParent, WORD wPort, DWORD dwCount)
{
    if (m_hWndParent) {
        return FALSE;
    }
    if (hWndParent == NULL) {
        return FALSE;
    }

    m_hWndParent = hWndParent;
    m_wPort = wPort;
    m_dwMaxConnectCount = dwCount;
    m_byMode = URARASOCKMODE_SREVER;

    m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if(!m_hEvent){
        return FALSE;
    }

    m_hThread = (HANDLE)_beginthread(ThreadEntry, 0, this);
    if (m_hThread == INVALID_HANDLE_VALUE) {
        return FALSE;
    }

    WaitForSingleObject(m_hEvent, INFINITE);

    return TRUE;
}

// サーバーへ接続
BOOL CUraraSockTCP::Connect(HWND hWndParent, WORD wPort, LPSTR pszAddr)
{
    if (m_hWndParent) {
        return FALSE;
    }
    if (hWndParent == NULL) {
        return FALSE;
    }

    struct hostent *clHostent;
    struct in_addr iaHost;

    ZeroMemory(&iaHost, sizeof(iaHost));

    iaHost.s_addr = inet_addr(pszAddr);
    if (iaHost.s_addr == INADDR_NONE) {
        clHostent = gethostbyname(pszAddr);
    } else {
        clHostent = gethostbyaddr((const char *)&iaHost, strlen(pszAddr), AF_INET);
    }

    if (clHostent == NULL) {
        return FALSE;
    }
    m_sockAddr.sin_addr.s_addr = *(DWORD *)(clHostent->h_addr);

    m_hWndParent = hWndParent;
    m_wPort = wPort;
    m_byMode = URARASOCKMODE_CLIENT;

    m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if(!m_hEvent){
        return FALSE;
    }

    m_hThread = (HANDLE)_beginthread(ThreadEntry, 0, this);
    if (m_hThread == INVALID_HANDLE_VALUE) {
        return FALSE;
    }

    WaitForSingleObject(m_hEvent, INFINITE);

    return TRUE;
}

// クライアント切断
void CUraraSockTCP::DeleteClient(DWORD dwID)
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
    if (dwID - URARASOCK_IDBASE >= m_dwMaxConnectCount) {
        return;
    }

    PostMessage(m_hWnd, WM_SOCKEVENT + (dwID - URARASOCK_IDBASE),
            (WPARAM)m_pSlot[dwID - URARASOCK_IDBASE].m_socket, (LPARAM)MAKELONG(FD_CLOSE, 0));
}

// 送信キャンセル
void CUraraSockTCP::SendCancel(DWORD dwID)
{
    if (dwID < URARASOCK_IDBASE) {
        return;
    }
    if (m_dwConnectCount == 0) {
        return;
    }

    PostMessage(m_hWnd, WM_SOCKCANCELQUE, (WPARAM)(dwID - URARASOCK_IDBASE), 0);
}

// サーバーへデータ送信
void CUraraSockTCP::Send(PBYTE pData, DWORD dwSize)
{
    SendTo(URARASOCK_IDBASE, pData, dwSize, 0, URARASOCK_SENDPRIORITY_MIDDLE);
}

void CUraraSockTCP::Send(PBYTE pData, DWORD dwSize, BYTE byPriority)
{
    SendTo(URARASOCK_IDBASE, pData, dwSize, 0, byPriority);
}

void CUraraSockTCP::Send(PBYTE pData, DWORD dwSize, DWORD dwTimeOut)
{
    SendTo(URARASOCK_IDBASE, pData, dwSize, dwTimeOut, URARASOCK_SENDPRIORITY_MIDDLE);
}

void CUraraSockTCP::Send(PBYTE pData, DWORD dwSize, DWORD dwTimeOut, BYTE byPriority)
{
    SendTo(URARASOCK_IDBASE, pData, dwSize, dwTimeOut, byPriority);
}

// クライアントへデータ送信
void CUraraSockTCP::SendTo(DWORD dwID, PBYTE pData, DWORD dwSize)
{
    SendTo(dwID, pData, dwSize, 0, URARASOCK_SENDPRIORITY_MIDDLE);
}

void CUraraSockTCP::SendTo(DWORD dwID, PBYTE pData, DWORD dwSize, BYTE byPriority)
{
    SendTo(dwID, pData, dwSize, 0, byPriority);
}

void CUraraSockTCP::SendTo(DWORD dwID, PBYTE pData, DWORD dwSize, DWORD dwTimeOut)
{
    SendTo(dwID, pData, dwSize, dwTimeOut, URARASOCK_SENDPRIORITY_MIDDLE);
}

void CUraraSockTCP::SendTo(DWORD dwID, PBYTE pData, DWORD dwSize, DWORD dwTimeOut, BYTE byPriority)
{
    if (dwID != URARASOCK_SENDALL) {
        if (dwID < URARASOCK_IDBASE) {
            return;
        }
        if (dwID - URARASOCK_IDBASE >= m_dwMaxConnectCount) {
            return;
        }
    }
    if (pData == NULL) {
        return;
    }
    if (m_dwConnectCount == 0) {
        return;
    }

    PBYTE pDataSend = NULL;
#ifdef URARASOCK_USEZLIB
    if (dwSize >= URARASOCK_USEZLIBSIZE) {
        LPBYTE pTmp = new BYTE[dwSize + 5];
        ZeroMemory(pTmp, dwSize + 5);
        m_Zlib.DeflateInit();
        DWORD dwRet = m_Zlib.Deflate(pData, dwSize, &pTmp[5], dwSize);
        if (dwRet) {
            pTmp[0] = 0xFF;
            CopyMemory(&pTmp[1], &dwSize, 4);
            pDataSend = pTmp;
            dwSize = dwRet + 5;
        } else {
            SAFE_DELETE_ARRAY(pTmp);
            pDataSend = new BYTE[dwSize];
            CopyMemory(pDataSend, pData, dwSize);
        }
        m_Zlib.DeflateEnd();
    } else {
#endif
        if (dwSize) {
            pDataSend = new BYTE[dwSize];
            CopyMemory(pDataSend, pData, dwSize);
        }
#ifdef URARASOCK_USEZLIB
    }
#endif

    PURARASOCK_ADDQUEINFO pQue = new URARASOCK_ADDQUEINFO;
    pQue->byPriority = byPriority;
    pQue->pData = pDataSend;
    pQue->dwSize = dwSize;
    pQue->dwTimeOut = dwTimeOut;
    pQue->dwCRC = m_pCrc->GetCRC(pDataSend, dwSize);

    PostMessage(m_hWnd, WM_SOCKADDQUE, (WPARAM)pQue, dwID);
}

// スループット取得
DWORD CUraraSockTCP::GetThrowghPutSend(DWORD dwID)
{
    if (dwID < URARASOCK_IDBASE) {
        return 0;
    }
    return m_pSlot[dwID - URARASOCK_IDBASE].GetThrowghPutSend();
}

DWORD CUraraSockTCP::GetThrowghPutRecv(DWORD dwID)
{
    if (dwID < URARASOCK_IDBASE) {
        return 0;
    }
    return m_pSlot[dwID - URARASOCK_IDBASE].GetThrowghPutRecv();
}

DWORD CUraraSockTCP::GetQueCount(DWORD dwID)
{
    if (dwID < URARASOCK_IDBASE) {
        return 0;
    }
    return m_pSlot[dwID - URARASOCK_IDBASE].GetQueCount();
}

DWORD CUraraSockTCP::GetIPAddress(DWORD dwID)
{
    if (dwID < URARASOCK_IDBASE) {
        return 0;
    }
    return m_pSlot[dwID - URARASOCK_IDBASE].GetIPAddress();
}

// スレッドエントリ
void __cdecl CUraraSockTCP::ThreadEntry(void* pParam)
{
    CUraraSockTCP* pThis = (CUraraSockTCP*)pParam;

    BOOL bResult = pThis->ThreadInit();
    SetEvent(pThis->m_hEvent);

    if (bResult) {
        pThis->ThreadMain();
    }

    pThis->ThreadExit();
}

// スレッド初期化
BOOL CUraraSockTCP::ThreadInit()
{
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(wc));
    wc.hInstance = GetModuleHandle(NULL);
    wc.cbSize = sizeof(wc);
    wc.lpszClassName = (m_byMode == URARASOCKMODE_SREVER) ? WNDCLNAMESVR : WNDCLNAMECLI;
    wc.lpfnWndProc = MainProcEntry;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wc)) {
        return FALSE;
    }

    if (m_byMode == URARASOCKMODE_SREVER) {
        m_hWnd = CreateWindow(WNDCLNAMESVR, WNDCLNAMESVR,
                WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL), this);
    } else {
        m_hWnd = CreateWindow(WNDCLNAMECLI, WNDCLNAMECLI,
                WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL), this);
    }
    if (m_hWnd == NULL) {
        return FALSE;
    }
    return TRUE;
}

// スレッドメイン
void CUraraSockTCP::ThreadMain()
{
    MSG Msg;

    while (GetMessage(&Msg, NULL, 0, 0)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
}

// スレッド終了
void CUraraSockTCP::ThreadExit()
{
    if (m_byMode == URARASOCKMODE_SREVER) {
        UnregisterClass(WNDCLNAMESVR, GetModuleHandle(NULL));
    } else {
        UnregisterClass(WNDCLNAMECLI, GetModuleHandle(NULL));
    }
}

// ウィンドウプロシージャエントリ
LRESULT CALLBACK CUraraSockTCP::MainProcEntry(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_CREATE) {
        SetWindowLong(hWnd, GWL_USERDATA, (LONG)((LPCREATESTRUCT)lParam)->lpCreateParams);
    }
    CUraraSockTCP* pThis = (CUraraSockTCP *)GetWindowLong(hWnd, GWL_USERDATA);
    if (pThis) {
        return pThis->WndProc(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// ウィンドウプロシージャ
LRESULT CUraraSockTCP::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    HANDLE_MSG(hWnd, WM_CREATE, MainOnCreate);
    HANDLE_MSG(hWnd, WM_DESTROY, MainOnDestroy);

    case WM_TIMER:
        if (wParam == TIMERID_KEEPALIVE) {
            DWORD dwTime = GetTickCount();
            for (int i = 0; i < (int)m_dwMaxConnectCount; i++) {
                if (m_pSlot[i].m_socket == INVALID_SOCKET) {
                    continue;
                }
            }
        } else if (wParam == TIMERID_KEEPALIVE_CLI) {
            Send(NULL, 0);
        }
        break;

    case WM_SOCKADDQUE:
        {
            PURARASOCK_ADDQUEINFO pQueInfo = (PURARASOCK_ADDQUEINFO)wParam;
            if (pQueInfo == NULL) {
                break;
            }
            if (lParam == URARASOCK_SENDALL) {
                int i, nCount = 0;
                for (i = 0; i < (int)m_dwMaxConnectCount; i++) {
                    if (nCount >= (int)m_dwConnectCount) {
                        break;
                    }
                    if (m_pSlot[i].m_socket != INVALID_SOCKET) {
                        m_pSlot[i].AddQue(pQueInfo);
                        nCount++;
                    }
                }
            } else {
                m_pSlot[lParam - URARASOCK_IDBASE].AddQue(pQueInfo);
            }
            SAFE_DELETE_ARRAY(pQueInfo->pData);
            SAFE_DELETE(pQueInfo);
        }
        break;

    case WM_SOCKCANCELQUE:
        m_pSlot[wParam].CancelQue();
        break;

    case WM_SOCKPRECHECK:
        {
            PBYTE pData = (PBYTE)wParam;
            DWORD dwData;
            CopyMemory(&dwData, pData, sizeof(DWORD));
            if (m_byMode == URARASOCKMODE_SREVER) {
                DWORD dwTmp = m_pSlot[lParam - URARASOCK_IDBASE].m_dwPreCheck;
                dwTmp &= URARASOCK_PRECHECK;
                dwTmp *= URARASOCK_PRECHECK;
                if (dwData == dwTmp) {
                    PostMessage(m_hWndParent, WM_URARASOCK_ADDCLIENT, 0, lParam);
                    m_pSlot[lParam - URARASOCK_IDBASE].m_bPreCheck = TRUE;
                } else {
                    DeleteClient(lParam);
                }
            } else {
                dwData &= URARASOCK_PRECHECK;
                dwData *= URARASOCK_PRECHECK;
                Send((PBYTE)&dwData, sizeof(DWORD));
                m_pSlot[lParam - URARASOCK_IDBASE].m_bPreCheck = TRUE;
                PostMessage(m_hWndParent, WM_URARASOCK_CONNECT, 0, 0);
            }
            SAFE_DELETE_ARRAY(pData);
        }
        break;

    case WM_URARASOCK_RECV:
#ifdef URARASOCK_USEZLIB
        {
            PBYTE pData = (PBYTE)wParam;
            if (*pData == 0xFF) {
                DWORD dwSourceSize;
                CopyMemory(&dwSourceSize, &pData[1], 4);
                LPBYTE pTmp = new BYTE[dwSourceSize];
                m_Zlib.InflateInit();
                DWORD dwRet = m_Zlib.Inflate(&pData[5], dwSourceSize, pTmp, dwSourceSize);
                if (dwRet) {
                    SAFE_DELETE_ARRAY(pData);
                    wParam = (WPARAM)pTmp;
                } else {
                    m_Zlib.InflateEnd();
                    break;
                }
                m_Zlib.InflateEnd();
            }
        }
#endif
        PostMessage(m_hWndParent, WM_URARASOCK_RECV, wParam, lParam);
        break;

    case WM_URARASOCK_SEND:
        {
            CUraraSockTCPSlot *pSlot;
            pSlot = &m_pSlot[wParam];
            PostMessage(hWnd, WM_SOCKEVENT + pSlot->m_dwSockID,
                    (WPARAM)pSlot->m_socket, (LPARAM)MAKELONG(FD_WRITE, 0));
        }
        break;

    default:
        if ((message >= WM_SOCKEVENT) && (message < WM_SOCKEVENT + URARASOCK_MAXCONNECT)) {
            MainOnSockEvent(hWnd, (int)lParam, message - WM_SOCKEVENT);
        } else {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    return 0;
}

// WM_CREATE
BOOL CUraraSockTCP::MainOnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == INVALID_SOCKET) {
        return FALSE;
    }
    int nTmp, nTmpSize = sizeof(nTmpSize);
    nTmp = 1024 * 64;
    setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (const char *)&nTmp, nTmpSize);
    setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (const char *)&nTmp, nTmpSize);

    if (m_byMode == URARASOCKMODE_SREVER) {
        ZeroMemory(&m_sockAddr, sizeof(m_sockAddr));
        m_sockAddr.sin_family = AF_INET;
        m_sockAddr.sin_port = htons(m_wPort);
        m_sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(m_socket, (LPSOCKADDR)&m_sockAddr, sizeof(m_sockAddr)) == SOCKET_ERROR) {
            return FALSE;
        }
        if (listen(m_socket, 1) == SOCKET_ERROR) {
            return FALSE;
        }
        if (WSAAsyncSelect(m_socket, hWnd, WM_SOCKEVENT, FD_ACCEPT | FD_READ | FD_WRITE | FD_CLOSE)) {
            return FALSE;
        }
        m_pSlot = new CUraraSockTCPSlot[m_dwMaxConnectCount];
        PostMessage(m_hWndParent, WM_URARASOCK_HOST, 0, 0);
        SetTimer(hWnd, TIMERID_KEEPALIVE, TIME_KEEPALIVE, NULL);
        return TRUE;
    }

    m_sockAddr.sin_family = AF_INET;
    m_sockAddr.sin_port = htons(m_wPort);
    if (WSAAsyncSelect(m_socket, hWnd, WM_SOCKEVENT, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE)) {
        return FALSE;
    }
    if (connect(m_socket, (struct sockaddr*)&m_sockAddr, sizeof(m_sockAddr))) {
        if (GetLastError() != WSAEWOULDBLOCK) {
            return FALSE;
        }
    }
    m_pSlot = new CUraraSockTCPSlot;
    SetTimer(hWnd, TIMERID_KEEPALIVE_CLI, TIME_KEEPALIVE_CLI, NULL);
    return TRUE;
}

// WM_DESTROY
void CUraraSockTCP::MainOnDestroy(HWND hWnd)
{
    if (m_dwMaxConnectCount >= 2) {
        SAFE_DELETE_ARRAY(m_pSlot);
    } else {
        SAFE_DELETE(m_pSlot);
    }

    shutdown(m_socket, 2);
    closesocket(m_socket);

    KillTimer(hWnd, TIMERID_KEEPALIVE);
    KillTimer(hWnd, TIMERID_KEEPALIVE_CLI);
    PostQuitMessage(0);
}

// ソケットイベント
void CUraraSockTCP::MainOnSockEvent(HWND hWnd, int nData, DWORD dwID)
{
    int nEvent = WSAGETSELECTEVENT(nData),
        nError = WSAGETSELECTERROR(nData);

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

// クライアント接続受理
void CUraraSockTCP::OnSockACCEPT()
{
    int i, nNo = -1;
    for (i = 0; i < (int)m_dwMaxConnectCount; i++) {
        if (m_pSlot[i].m_socket == INVALID_SOCKET) {
            nNo = i;
            break;
        }
    }
    if (nNo == -1) {
        return;
    }

    SOCKADDR_IN sockAddr;
    ZeroMemory(&sockAddr, sizeof(sockAddr));
    int nLength = sizeof(sockAddr);
    SOCKET hSocket = accept(m_socket, (sockaddr*)&sockAddr, &nLength);
    if (hSocket == INVALID_SOCKET) {
        return;
    }

    m_pSlot[nNo].Create(hSocket, sockAddr.sin_addr.s_addr, m_hWnd, nNo);
    DWORD dwData = GetTickCount();
    m_pSlot[nNo].m_dwPreCheck = dwData;
    m_dwConnectCount++;

    SendTo((nNo + URARASOCK_IDBASE), (PBYTE)&dwData, sizeof(DWORD));
}

// サーバー接続完了
void CUraraSockTCP::OnConnect()
{
    m_pSlot->Create(m_socket, m_sockAddr.sin_addr.s_addr, m_hWnd, 0);
    m_dwConnectCount++;
}

// ソケット切断
void CUraraSockTCP::OnSockClose(DWORD dwID)
{
    if (m_pSlot[dwID].m_socket == INVALID_SOCKET) {
        return;
    }
    m_dwConnectCount--;
    m_pSlot[dwID].Destroy();

    if (m_byMode == URARASOCKMODE_SREVER) {
        PostMessage(m_hWndParent, WM_URARASOCK_DECCLIENT, 0, (LPARAM)(dwID + URARASOCK_IDBASE));
    } else {
        PostMessage(m_hWndParent, WM_URARASOCK_DISCONNECT, 0, 0);
    }
}

// 送信イベント
void CUraraSockTCP::OnSockWrite(DWORD dwID)
{
    if (m_byMode == URARASOCKMODE_SREVER) {
        m_pSlot[dwID].OnFD_WRITE();
    } else {
        m_pSlot->OnFD_WRITE();
    }
}

// 受信イベント
void CUraraSockTCP::OnSockRead(DWORD dwID)
{
    if (m_byMode == URARASOCKMODE_SREVER) {
        m_pSlot[dwID].OnFD_READ();
    } else {
        m_pSlot->OnFD_READ();
    }
}

/* 以下 CUraraSockTCPSlot クラス */

CUraraSockTCPSlot::CUraraSockTCPSlot(void)
{
    m_wPort = 0;
    m_dwSockID = 0;
    m_dwTimeConnect = 0;
    m_dwTimeLastRecv = 0;
    m_dwSendSize = 0;
    m_dwSendDataSize = 0;
    m_dwRecvDataSize = 0;
    m_dwThrowghPutSend = 0;
    m_dwThrowghPutRecv = 0;
    m_dwTimeThrowghPut = 0;
    m_hWndParent = NULL;
    m_hWnd = NULL;
    ZeroMemory(&m_sockAddr, sizeof(m_sockAddr));
    m_socket = INVALID_SOCKET;
    m_pRecvTmp = NULL;
    m_pRecvBuffer = NULL;
    m_SendQueInfo = NULL;
    m_bPreCheck = FALSE;
    m_dwPreCheck = 0;
    m_pCrc = new CCRC;
}

CUraraSockTCPSlot::~CUraraSockTCPSlot(void)
{
    Destroy();
    SAFE_DELETE(m_pCrc);
}

BOOL CUraraSockTCPSlot::Create(SOCKET socket, DWORD dwAddr, HWND hWndParent, DWORD dwID)
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

    if (WSAAsyncSelect(m_socket, hWndParent, WM_SOCKEVENT + dwID, FD_READ | FD_WRITE | FD_CLOSE)) {
        return FALSE;
    }

    return TRUE;
}

void CUraraSockTCPSlot::AddQue(PURARASOCK_ADDQUEINFO pQueAdd)
{
    if (m_socket == INVALID_SOCKET) {
        return;
    }

    PURARASOCK_PACKETINFO pQueTmp;
    PURARASOCK_QUEINFO pQue;
    DWORD dwSize;
    PBYTE pTmp;

    dwSize = pQueAdd->dwSize + sizeof(URARASOCK_PACKETINFO);

    pTmp = new BYTE[dwSize];

    pQueTmp = (PURARASOCK_PACKETINFO)pTmp;
    pQueTmp->dwSize = pQueAdd->dwSize;
    pQueTmp->dwCRC  = pQueAdd->dwCRC;
    CopyMemory(pTmp + sizeof(URARASOCK_PACKETINFO), pQueAdd->pData, pQueAdd->dwSize);

    pQue = new URARASOCK_QUEINFO;

    pQue->PacketInfo.dwSize = pQueAdd->dwSize;
    pQue->dwSize = dwSize;
    pQue->pData = pTmp;
    pQue->byPriority = pQueAdd->byPriority;
    pQue->dwTimeMake = GetTickCount();
    pQue->dwTimeOut = pQueAdd->dwTimeOut;

    switch (pQue->byPriority) {
    case URARASOCK_SENDPRIORITY_HIGH:
        m_QueInfoHi.Add(pQue);
        Combine(&m_QueInfoHi);
        break;
    case URARASOCK_SENDPRIORITY_MIDDLE:
        m_QueInfoMid.Add(pQue);
        Combine(&m_QueInfoMid);
        break;
    case URARASOCK_SENDPRIORITY_LOW:
        m_QueInfoLow.Add(pQue);
        Combine(&m_QueInfoLow);
        break;
    }

    PostMessage(m_hWndParent, WM_URARASOCK_SEND, (WPARAM)m_dwSockID, 0);
}

void CUraraSockTCPSlot::Combine(CmyArray<PURARASOCK_QUEINFO, PURARASOCK_QUEINFO> *pQue)
{
    PURARASOCK_QUEINFO pQueTmp, pQueNew;
    PBYTE pTmp;
    int i, nCount;

    nCount = pQue->GetSize();
    if (nCount <= 1) {
        return;
    }

    pQueNew = new URARASOCK_QUEINFO;
    ZeroMemory(pQueNew, sizeof(URARASOCK_QUEINFO));

    for (i = 0; i < nCount; i++) {
        DWORD dwSize;

        dwSize = pQueNew->dwSize;
        pQueTmp = pQue->GetAt(i);

        dwSize += pQueTmp->dwSize;
        pTmp = new BYTE[dwSize];
        CopyMemory(pTmp, pQueNew->pData, pQueNew->dwSize);
        CopyMemory(pTmp + pQueNew->dwSize, pQueTmp->pData, pQueTmp->dwSize);
        pQueNew->dwSize = dwSize;
        pQueNew->byPriority = pQueTmp->byPriority;

        SAFE_DELETE(pQueNew->pData);
        SAFE_DELETE(pQueTmp->pData);
        SAFE_DELETE(pQueTmp);
        pQueNew->pData = pTmp;
    }
    pQue->RemoveAll();
    pQueNew->dwTimeMake = GetTickCount();
    pQue->Add(pQueNew);
}

void CUraraSockTCPSlot::CancelQue(void)
{
    int i, nCount;

    nCount = m_QueInfoHi.GetSize();
    for (i = 0; i < nCount; i++) {
        PURARASOCK_QUEINFO pInfo;
        pInfo = m_QueInfoHi[i];
        SAFE_DELETE_ARRAY(pInfo->pData);
        SAFE_DELETE(pInfo);
    }
    m_QueInfoHi.RemoveAll();

    nCount = m_QueInfoMid.GetSize();
    for (i = 0; i < nCount; i++) {
        PURARASOCK_QUEINFO pInfo;
        pInfo = m_QueInfoMid[i];
        SAFE_DELETE_ARRAY(pInfo->pData);
        SAFE_DELETE(pInfo);
    }
    m_QueInfoMid.RemoveAll();

    nCount = m_QueInfoLow.GetSize();
    for (i = 0; i < nCount; i++) {
        PURARASOCK_QUEINFO pInfo;
        pInfo = m_QueInfoLow[i];
        SAFE_DELETE_ARRAY(pInfo->pData);
        SAFE_DELETE(pInfo);
    }
    m_QueInfoLow.RemoveAll();
}

void CUraraSockTCPSlot::OnFD_WRITE(void)
{
    if (m_socket == INVALID_SOCKET) {
        CancelQue();
        return;
    }

    while (1) {
        int nRet, nSendSize;
        PURARASOCK_QUEINFO pQue;

        if (m_SendQueInfo == NULL) {
            if (m_QueInfoHi.GetSize()) {
                m_SendQueInfo = &m_QueInfoHi;
            } else if (m_QueInfoMid.GetSize()) {
                m_SendQueInfo = &m_QueInfoMid;
            } else if (m_QueInfoLow.GetSize()) {
                m_SendQueInfo = &m_QueInfoLow;
            } else {
                break;
            }
            pQue = m_SendQueInfo->GetAt(0);
            if (pQue->dwTimeOut && (GetTickCount() > pQue->dwTimeMake + pQue->dwTimeOut)) {
                SAFE_DELETE_ARRAY(pQue->pData);
                SAFE_DELETE(pQue);
                m_SendQueInfo->RemoveAt(0);
                m_SendQueInfo = NULL;
                m_dwSendSize = 0;
                continue;
            }
        }
        if (m_SendQueInfo->GetSize() <= 0) {
            break;
        }

        pQue = m_SendQueInfo->GetAt(0);
        if (pQue == NULL) {
            break;
        }
        nSendSize = pQue->dwSize - m_dwSendSize;
        nRet = send(m_socket, (char *)(pQue->pData + m_dwSendSize), nSendSize, 0);

        if (nRet > 0) {
            m_dwSendDataSize += nRet;
            RenewThrowghPut();
            m_dwSendSize += nRet;

            if (m_dwSendSize >= pQue->dwSize) {
                SAFE_DELETE_ARRAY(pQue->pData);
                SAFE_DELETE(pQue);
                m_SendQueInfo->RemoveAt(0);
                m_SendQueInfo = NULL;
                m_dwSendSize = 0;
            }

        } else {
            DWORD dwError = WSAGetLastError();
            if (dwError != WSAEWOULDBLOCK) {
                PostMessage(m_hWndParent, WM_SOCKEVENT + m_dwSockID, (WPARAM)m_socket, (LPARAM)MAKELONG(FD_CLOSE, 0));
            }
            break;
        }
    }
}

void CUraraSockTCPSlot::OnFD_READ(void)
{
    DWORD dwPos, dwRecvSize;

    if (m_socket == INVALID_SOCKET) {
        CancelQue();
        return;
    }

    dwRecvSize = 0;

    while (1) {
        int nRet, nRecvSize = URARASOCK_RECVBUFSIZE - dwRecvSize;
        nRet = recv(m_socket, (char *)(m_pRecvBuffer + dwRecvSize), nRecvSize, 0);
        if (nRet > 0) {
            m_dwRecvDataSize += nRet;
            RenewThrowghPut();
            dwRecvSize += nRet;
            m_dwTimeLastRecv = GetTickCount();

        } else {
            DWORD dwError = WSAGetLastError();
            if (dwError && dwError != WSAEWOULDBLOCK) {
                PostMessage(m_hWndParent, WM_SOCKEVENT + m_dwSockID, (WPARAM)m_socket, (LPARAM)MAKELONG(FD_CLOSE, 0));
                return;
            }
            break;
        }
    }

    dwPos = 0;

    while (1) {
        if (m_pRecvTmp == NULL) {
            m_pRecvTmp = new BYTE[sizeof(URARASOCK_PACKETINFO)];
            m_dwRecvSizeTarget = 0;
            m_dwRecvCrc = 0;
            m_dwRecvSize = 0;
        }

        int nRet, nTmp, nRecvSize = m_dwRecvSizeTarget - m_dwRecvSize;
        if (m_dwRecvSizeTarget == 0) {
            nRecvSize = sizeof(URARASOCK_PACKETINFO) - m_dwRecvSize;
        }

        nTmp = dwRecvSize - dwPos;
        if (nRecvSize > nTmp) {
            nRecvSize = nTmp;
        }
        if (nRecvSize > 0) {
            CopyMemory(m_pRecvTmp + m_dwRecvSize, m_pRecvBuffer + dwPos, nRecvSize);
            dwPos += nRecvSize;
        }
        nRet = nRecvSize;

        if (nRet > 0) {
            m_dwRecvSize += nRet;
            if (m_dwRecvSizeTarget == 0) {
                if (m_dwRecvSize >= sizeof(URARASOCK_PACKETINFO)) {
                    PURARASOCK_PACKETINFO pInfo = (PURARASOCK_PACKETINFO)m_pRecvTmp;
                    m_dwRecvSizeTarget = pInfo->dwSize;
                    m_dwRecvCrc = pInfo->dwCRC;
                    m_dwRecvSize = 0;
                    SAFE_DELETE_ARRAY(m_pRecvTmp);

                    if (m_bPreCheck == FALSE) {
                        if (m_dwRecvSizeTarget != sizeof(DWORD)) {
                            PostMessage(m_hWndParent, WM_SOCKEVENT + m_dwSockID, (WPARAM)m_socket, (LPARAM)MAKELONG(FD_CLOSE, 0));
                            goto Exit;
                        }
                    }

                    if (m_dwRecvSizeTarget) {
                        m_pRecvTmp = new BYTE[m_dwRecvSizeTarget];
                    }
                }

            } else if (m_dwRecvSize >= m_dwRecvSizeTarget) {
                if (m_bPreCheck == FALSE) {
                    if (m_dwRecvSize != sizeof(DWORD)) {
                        SAFE_DELETE_ARRAY(m_pRecvTmp);
                        PostMessage(m_hWndParent, WM_SOCKEVENT + m_dwSockID, (WPARAM)m_socket, (LPARAM)MAKELONG(FD_CLOSE, 0));
                        return;
                    }
                    PostMessage(m_hWndParent, WM_SOCKPRECHECK, (WPARAM)m_pRecvTmp, (LPARAM)(m_dwSockID + URARASOCK_IDBASE));

                } else {
                    if ((m_dwRecvCrc == 0) || (m_dwRecvCrc != m_pCrc->GetCRC(m_pRecvTmp, m_dwRecvSizeTarget))) {
                        SAFE_DELETE_ARRAY(m_pRecvTmp);
                        PostMessage(m_hWndParent, WM_SOCKEVENT + m_dwSockID, (WPARAM)m_socket, (LPARAM)MAKELONG(FD_CLOSE, 0));
                        return;
                    }
                    PostMessage(m_hWndParent, WM_URARASOCK_RECV, (WPARAM)m_pRecvTmp, (LPARAM)(m_dwSockID + URARASOCK_IDBASE));
                }
                m_dwRecvSizeTarget = 0;
                m_dwRecvCrc = 0;
                m_dwRecvSize = 0;
                m_pRecvTmp = NULL;
            }
        } else {
            break;
        }
    }
Exit:
    return;
}

void CUraraSockTCPSlot::Destroy(void)
{
    BOOL bSend;

    bSend = FALSE;
    if (m_socket != INVALID_SOCKET) {
        shutdown(m_socket, 2);
        closesocket(m_socket);
        bSend = TRUE;
    }

    CancelQue();

    m_wPort = 0;
    m_dwTimeConnect = 0;
    m_dwTimeLastRecv = 0;
    m_dwSendSize = 0;
    m_dwSendDataSize = 0;
    m_dwRecvDataSize = 0;
    m_dwThrowghPutSend = 0;
    m_dwThrowghPutRecv = 0;
    m_dwTimeThrowghPut = 0;
    m_hWnd = NULL;
    ZeroMemory(&m_sockAddr, sizeof(m_sockAddr));
    m_socket = INVALID_SOCKET;
    SAFE_DELETE_ARRAY(m_pRecvTmp);
    SAFE_DELETE_ARRAY(m_pRecvBuffer);
    m_SendQueInfo = NULL;
    m_bPreCheck = FALSE;
    m_dwPreCheck = 0;
    m_socket = INVALID_SOCKET;

    if (bSend) {
        PostMessage(m_hWndParent, WM_URARASOCK_DECCLIENT, 0, m_dwSockID);
    }

    m_hWndParent = NULL;
    m_dwSockID = 0;
}

void CUraraSockTCPSlot::RenewThrowghPut(void)
{
    if (abs((int)(GetTickCount() - m_dwTimeThrowghPut)) < 1000) {
        return;
    }
    m_dwTimeThrowghPut = GetTickCount();
    m_dwThrowghPutSend = m_dwSendDataSize;
    m_dwThrowghPutRecv = m_dwRecvDataSize;
    m_dwSendDataSize = 0;
    m_dwRecvDataSize = 0;
}

DWORD CUraraSockTCPSlot::GetThrowghPutSend(void)
{
    RenewThrowghPut();
    return m_dwThrowghPutSend;
}

DWORD CUraraSockTCPSlot::GetThrowghPutRecv(void)
{
    RenewThrowghPut();
    return m_dwThrowghPutRecv;
}

DWORD CUraraSockTCPSlot::GetQueCount(void)
{
    return (m_QueInfoHi.GetSize() + m_QueInfoMid.GetSize() + m_QueInfoLow.GetSize());
}

DWORD CUraraSockTCPSlot::GetIPAddress(void)
{
    return (DWORD)m_sockAddr.sin_addr.S_un.S_addr;
}

/* Copyright(C)URARA-works 2003-2005 */

