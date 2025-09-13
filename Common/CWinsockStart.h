#pragma once

#include <winsock2.h>

// Winsock の初期化と終了処理を行うクラス
class CWinsockStart {
public:
    CWinsockStart(void);
    ~CWinsockStart(void);

    DWORD GetIPaddr(char *pName);
    BOOL GetMyPcAddr(char *szName, int iLen, IN_ADDR *pAddr);
};

