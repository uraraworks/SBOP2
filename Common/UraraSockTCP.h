/* Copyright(C)URARA-works 2003-2005 */
/* ========================================================================= */
/* CUraraSockTCP.h                                                            */
/* 汎用TCP通信クラス                                                         */
/* ========================================================================= */

#ifndef CURARASOCKTCP_H
#define CURARASOCKTCP_H

#define URARASOCK_USEZLIB

#include "CWinsockStart.h"
#ifdef URARASOCK_USEZLIB
#include "myZlib/myZlib.h"
#endif

#include "myArray.h"

class CCRC;

// 定数定義
#define WNDCLNAMESVR    "TCPMainWindowSvr"
#define WNDCLNAMECLI    "TCPMainWindowCli"
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

#define URARASOCK_MAXCONNECT    200
#define URARASOCK_IDBASE        100000
#define URARASOCK_SENDALL       0xFFFFFFFF
#define URARASOCK_PRECHECK      0x56BB3E5E
#define URARASOCK_RECVBUFSIZE   (1024*64)
#define URARASOCK_USEZLIBSIZE   (128)

#define TIMERID_KEEPALIVE       0
#define TIMERID_KEEPALIVE_CLI   1
#define TIME_KEEPALIVE          60000
#define TIME_KEEPALIVE_CLI      30000

// ウィンドウメッセージ
enum {
    WM_SOCKADDQUE = WM_APP + 200,
    WM_SOCKCANCELQUE,
    WM_SOCKPRECHECK,

    WM_URARASOCK_HOST,
    WM_URARASOCK_ADDCLIENT,
    WM_URARASOCK_DECCLIENT,
    WM_URARASOCK_CONNECT,
    WM_URARASOCK_DISCONNECT,
    WM_URARASOCK_RECV,

    WM_URARASOCK_SEND,
    WM_SOCKEVENT,
};

// 送信優先順位
enum URARASOCK_SENDPRIORITY {
    URARASOCK_SENDPRIORITY_HIGH = 0,
    URARASOCK_SENDPRIORITY_MIDDLE,
    URARASOCK_SENDPRIORITY_LOW,
};

// 動作モード
enum {
    URARASOCKMODE_SREVER = 0,
    URARASOCKMODE_CLIENT,
};

// 構造体
typedef struct _URARASOCK_PACKETINFO {
    DWORD dwSize;
    DWORD dwCRC;
} URARASOCK_PACKETINFO, *PURARASOCK_PACKETINFO;

typedef struct _URARASOCK_QUEINFO {
    URARASOCK_PACKETINFO PacketInfo;
    BYTE  byPriority;
    PBYTE pData;
    DWORD dwSize;
    DWORD dwTimeMake;
    DWORD dwTimeOut;
} URARASOCK_QUEINFO, *PURARASOCK_QUEINFO;

typedef struct _URARASOCK_ADDQUEINFO {
    BYTE  byPriority;
    PBYTE pData;
    DWORD dwSize;
    DWORD dwTimeOut;
    DWORD dwCRC;
} URARASOCK_ADDQUEINFO, *PURARASOCK_ADDQUEINFO;

// スロットクラス
class CUraraSockTCPSlot {
public:
    CUraraSockTCPSlot(void);
    virtual ~CUraraSockTCPSlot(void);

    BOOL Create(SOCKET, DWORD, HWND, DWORD);
    void AddQue(PURARASOCK_ADDQUEINFO);
    void Combine(CmyArray<PURARASOCK_QUEINFO, PURARASOCK_QUEINFO> *);
    void CancelQue(void);
    void Destroy(void);
    DWORD GetThrowghPutSend(void);
    DWORD GetThrowghPutRecv(void);
    DWORD GetQueCount(void);
    DWORD GetIPAddress(void);

    void OnFD_WRITE(void);
    void OnFD_READ(void);

private:
    void RenewThrowghPut(void);

public:
    SOCKET  m_socket;
    DWORD   m_dwSockID;
    DWORD   m_dwTimeLastRecv;
    DWORD   m_dwPreCheck;
    BOOL    m_bPreCheck;

private:
    CCRC    *m_pCrc;
    SOCKADDR_IN m_sockAddr;
    WORD    m_wPort;
    DWORD   m_dwMsgID;
    DWORD   m_dwTimeConnect;
    DWORD   m_dwSendSize;
    DWORD   m_dwRecvSize;
    DWORD   m_dwRecvSizeTarget;
    DWORD   m_dwRecvCrc;
    DWORD   m_dwSendDataSize;
    DWORD   m_dwRecvDataSize;
    DWORD   m_dwThrowghPutSend;
    DWORD   m_dwThrowghPutRecv;
    DWORD   m_dwTimeThrowghPut;
    PBYTE   m_pRecvTmp;
    PBYTE   m_pRecvBuffer;
    HWND    m_hWndParent;
    HWND    m_hWnd;
    CmyArray<PURARASOCK_QUEINFO, PURARASOCK_QUEINFO> *m_SendQueInfo;
    CmyArray<PURARASOCK_QUEINFO, PURARASOCK_QUEINFO> m_QueInfoHi;
    CmyArray<PURARASOCK_QUEINFO, PURARASOCK_QUEINFO> m_QueInfoMid;
    CmyArray<PURARASOCK_QUEINFO, PURARASOCK_QUEINFO> m_QueInfoLow;
};

// メインクラス
class CUraraSockTCP {
public:
    CUraraSockTCP(void);
    virtual ~CUraraSockTCP(void);

    void DeleteRecvData(PBYTE pData);
    void Destroy(void);
    BOOL Host(HWND, WORD, DWORD);
    BOOL Connect(HWND, WORD, LPSTR);
    void DeleteClient(DWORD);
    void SendCancel(DWORD dwID = 0);
    void Send(PBYTE, DWORD);
    void Send(PBYTE, DWORD, BYTE);
    void Send(PBYTE, DWORD, DWORD);
    void Send(PBYTE, DWORD, DWORD, BYTE);
    void SendTo(DWORD, PBYTE, DWORD);
    void SendTo(DWORD, PBYTE, DWORD, BYTE);
    void SendTo(DWORD, PBYTE, DWORD, DWORD);
    void SendTo(DWORD, PBYTE, DWORD, DWORD, BYTE);
    DWORD GetThrowghPutSend(DWORD);
    DWORD GetThrowghPutRecv(DWORD);
    DWORD GetQueCount(DWORD);
    DWORD GetIPAddress(DWORD);

private:
    static void __cdecl ThreadEntry(void* pParam);
    BOOL ThreadInit();
    void ThreadMain();
    void ThreadExit();

    void InitData(void);
    static LRESULT CALLBACK MainProcEntry(HWND, UINT, WPARAM, LPARAM);
    LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
    BOOL MainOnCreate(HWND, LPCREATESTRUCT);
    void MainOnDestroy(HWND);
    void MainOnSockEvent(HWND, int, DWORD);
    void OnSockACCEPT(void);
    void OnConnect(void);
    void OnSockClose(DWORD);
    void OnSockWrite(DWORD);
    void OnSockRead(DWORD);

private:
    CCRC            *m_pCrc;
    CWinsockStart   m_Winsock;
    SOCKADDR_IN     m_sockAddr;
    SOCKET          m_socket;
    HANDLE          m_hThread;
    HANDLE          m_hEvent;
    WORD            m_wPort;
    DWORD           m_dwConnectCount;
    DWORD           m_dwSendID;
    DWORD           m_dwMaxConnectCount;
    HWND            m_hWndParent;
    HWND            m_hWnd;
    BYTE            m_byMode;
    CUraraSockTCPSlot *m_pSlot;
#ifdef URARASOCK_USEZLIB
    CmyZlib         m_Zlib;
#endif
};

#endif

/* Copyright(C)URARA-works 2003-2005 */

