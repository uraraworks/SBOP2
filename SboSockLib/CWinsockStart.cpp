#include "StdAfx.h"
#include "CWinsockStart.h"

CWinsockStart::CWinsockStart(void)
{
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif
    // POSIX: ソケット初期化不要
}

CWinsockStart::~CWinsockStart(void)
{
#ifdef _WIN32
    WSACleanup();
#endif
}

DWORD CWinsockStart::GetIPaddr(const char *pName)
{
    if ((pName == NULL) || (*pName == '\0')) {
        return 0;
    }

    struct in_addr addr;
    memset(&addr, 0, sizeof(addr));
    if (inet_pton(AF_INET, pName, &addr) == 1) {
        return addr.s_addr;
    }

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    struct addrinfo *result = NULL;
    DWORD dwAddr = 0;
    if (getaddrinfo(pName, NULL, &hints, &result) != 0) {
        return 0;
    }

    for (struct addrinfo *ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        if ((ptr->ai_family == AF_INET) && (ptr->ai_addrlen >= sizeof(struct sockaddr_in))) {
            const struct sockaddr_in *sin = reinterpret_cast<const struct sockaddr_in *>(ptr->ai_addr);
            dwAddr = sin->sin_addr.s_addr;
            break;
        }
    }

    freeaddrinfo(result);
    return dwAddr;
}

BOOL CWinsockStart::GetMyPcAddr(char *szName, int iLen,
#ifdef _WIN32
    IN_ADDR *pAddr
#else
    struct in_addr *pAddr
#endif
)
{
    if ((szName == NULL) || (iLen <= 0) || (pAddr == NULL)) {
        return FALSE;
    }

    memset(szName, 0, iLen);
    memset(pAddr, 0, sizeof(*pAddr));

    if (gethostname(szName, iLen - 1) != 0) {
        return FALSE;
    }

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    struct addrinfo *result = NULL;
    if (getaddrinfo(szName, NULL, &hints, &result) != 0) {
        return FALSE;
    }

    BOOL bSuccess = FALSE;
    for (struct addrinfo *ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        if ((ptr->ai_family == AF_INET) && (ptr->ai_addrlen >= sizeof(struct sockaddr_in))) {
            const struct sockaddr_in *sin = reinterpret_cast<const struct sockaddr_in *>(ptr->ai_addr);
            *pAddr = sin->sin_addr;
            bSuccess = TRUE;
            break;
        }
    }

    freeaddrinfo(result);
    return bSuccess;
}
