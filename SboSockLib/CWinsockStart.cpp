#include "StdAfx.h"
#include "CWinsockStart.h"

CWinsockStart::CWinsockStart(void)
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 0), &wsaData);
}

CWinsockStart::~CWinsockStart(void)
{
    WSACleanup();
}

DWORD CWinsockStart::GetIPaddr(const char *pName)
{
    if ((pName == NULL) || (*pName == '\0')) {
        return 0;
    }

    IN_ADDR addr;
    ZeroMemory(&addr, sizeof(addr));
    if (InetPtonA(AF_INET, pName, &addr) == 1) {
        return addr.S_un.S_addr;
    }

    ADDRINFOA hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    PADDRINFOA result = NULL;
    DWORD dwAddr = 0;
    if (GetAddrInfoA(pName, NULL, &hints, &result) != 0) {
        return 0;
    }

    for (PADDRINFOA ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        if ((ptr->ai_family == AF_INET) && (ptr->ai_addrlen >= sizeof(sockaddr_in))) {
            const sockaddr_in *sin = reinterpret_cast<const sockaddr_in *>(ptr->ai_addr);
            dwAddr = sin->sin_addr.S_un.S_addr;
            break;
        }
    }

    FreeAddrInfoA(result);
    return dwAddr;
}

BOOL CWinsockStart::GetMyPcAddr(char *szName, int iLen, IN_ADDR *pAddr)
{
    if ((szName == NULL) || (iLen <= 0) || (pAddr == NULL)) {
        return FALSE;
    }

    ZeroMemory(szName, iLen);
    ZeroMemory(pAddr, sizeof(IN_ADDR));

    if (gethostname(szName, iLen - 1) != 0) {
        return FALSE;
    }

    ADDRINFOA hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    PADDRINFOA result = NULL;
    if (GetAddrInfoA(szName, NULL, &hints, &result) != 0) {
        return FALSE;
    }

    BOOL bSuccess = FALSE;
    for (PADDRINFOA ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        if ((ptr->ai_family == AF_INET) && (ptr->ai_addrlen >= sizeof(sockaddr_in))) {
            const sockaddr_in *sin = reinterpret_cast<const sockaddr_in *>(ptr->ai_addr);
            *pAddr = sin->sin_addr;
            bSuccess = TRUE;
            break;
        }
    }

    FreeAddrInfoA(result);
    return bSuccess;
}
