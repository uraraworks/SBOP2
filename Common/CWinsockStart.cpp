#include <winsock2.h>
#include "CWinsockStart.h"

// コンストラクタで Winsock を初期化
CWinsockStart::CWinsockStart(void)
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 0), &wsaData);
}

// デストラクタで終了処理
CWinsockStart::~CWinsockStart(void)
{
    WSACleanup();
}

// 文字列から IP アドレスを取得
DWORD CWinsockStart::GetIPaddr(char *pName)
{
    struct hostent *clHostent;
    struct in_addr iaHost;

    ZeroMemory(&iaHost, sizeof(iaHost));

    iaHost.s_addr = inet_addr(pName);
    if (iaHost.s_addr == INADDR_NONE) {
        clHostent = gethostbyname(pName);
    } else {
        clHostent = gethostbyaddr((const char *)&iaHost, strlen(pName), AF_INET);
    }

    if (clHostent == NULL) {
        return 0;
    }
    return *(DWORD *)(clHostent->h_addr);
}

// 自PCのアドレスを取得
BOOL CWinsockStart::GetMyPcAddr(char *szName, int iLen, IN_ADDR *pAddr)
{
    PHOSTENT phe;

    ZeroMemory(szName, iLen);
    ZeroMemory(pAddr, sizeof(IN_ADDR));

    gethostname(szName, iLen - 1);
    phe = gethostbyname(szName);
    if (phe == NULL) {
        return FALSE;
    }

    CopyMemory((char *)pAddr, phe->h_addr, sizeof(IN_ADDR));
    return TRUE;
}

