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

DWORD CWinsockStart::GetIPaddr(char *pName)
{
    struct hostent *clHostent;
    struct in_addr iaHost;

    ZeroMemory(&iaHost, sizeof(iaHost));

    iaHost.s_addr = inet_addr(pName);
    if (iaHost.s_addr == INADDR_NONE) {
        clHostent = gethostbyname(pName);
    } else {
        clHostent = gethostbyaddr(reinterpret_cast<const char *>(&iaHost), sizeof(iaHost), AF_INET);
    }

    if (clHostent == NULL) {
        return 0;
    }
    return *reinterpret_cast<PDWORD>(clHostent->h_addr);
}

BOOL CWinsockStart::GetMyPcAddr(char *szName, int iLen, IN_ADDR *pAddr)
{
    PHOSTENT phe;

    ZeroMemory(szName, iLen);
    ZeroMemory(pAddr, sizeof(IN_ADDR));

    if (gethostname(szName, iLen - 1) != 0) {
        return FALSE;
    }
    phe = gethostbyname(szName);
    if (phe == NULL) {
        return FALSE;
    }

    CopyMemory(reinterpret_cast<char *>(pAddr), phe->h_addr, sizeof(IN_ADDR));
    return TRUE;
}
