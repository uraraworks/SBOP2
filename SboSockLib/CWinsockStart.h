#pragma once

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

class CWinsockStart {
public:
    CWinsockStart(void);
    ~CWinsockStart(void);

    DWORD GetIPaddr(const char *pName);
    BOOL GetMyPcAddr(char *szName, int iLen,
#ifdef _WIN32
        IN_ADDR *pAddr
#else
        struct in_addr *pAddr
#endif
    );
};
