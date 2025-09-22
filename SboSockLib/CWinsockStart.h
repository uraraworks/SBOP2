#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

class CWinsockStart {
public:
    CWinsockStart(void);
    ~CWinsockStart(void);

    DWORD GetIPaddr(const char *pName);
    BOOL GetMyPcAddr(char *szName, int iLen, IN_ADDR *pAddr);
};
