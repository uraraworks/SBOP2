#pragma once

#include "UraraSockTCP.h"

class CPacketBase;

// ソケット制御のラッパークラス
class CUraraSockTCPSBO {
public:
    CUraraSockTCPSBO(void);
    virtual ~CUraraSockTCPSBO(void);

    void    DeleteRecvData  (PBYTE pData);
    void    Destroy         (void);
    BOOL    Host            (HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, DWORD dwCount);
    BOOL    Connect         (HWND hWndParent, DWORD dwMsgBase, DWORD dwKey, WORD wPort, LPCSTR pszAddr);
    void    DeleteClient    (DWORD dwID);
    void    SendCancel      (DWORD dwID = 0);
    void    Send            (CPacketBase *pPacket);
    void    SendTo          (DWORD dwID, CPacketBase *pPacket);
    DWORD   GetThrowghPutSend(DWORD dwID);
    DWORD   GetThrowghPutRecv(DWORD dwID);
    DWORD   GetQueCount     (DWORD dwID);
    DWORD   GetIPAddress    (DWORD dwID);

protected:
    CUraraSockTCP   *m_pSock;   // 実際のソケットクラス
};

