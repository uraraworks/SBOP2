/// @file TestMain.cpp
/// @brief テスト実行本体
/// @copyright Copyright(C)URARA-works

#include "StdAfx.h"
#include "TestFramework.h"
#include <string.h>

CTestRegistry &CTestRegistry::Get(void)
{
    static CTestRegistry s_Instance;
    return s_Instance;
}

void CTestRegistry::Add(const char *pszName, PFTESTFUNC pfFunc)
{
    ENTRY Entry;

    Entry.pszName = pszName;
    Entry.pfFunc  = pfFunc;
    m_vecEntry.push_back(Entry);
}

void CTestRegistry::Fail(const char *pszFile, int nLine, const char *pszMsg)
{
    const char *pszShort = strrchr(pszFile, '\\');

    pszShort = (pszShort != NULL) ? (pszShort + 1) : pszFile;
    printf("    NG  %s(%d): %s\n", pszShort, nLine, pszMsg);
    m_nFailedInCase ++;
}

int CTestRegistry::RunAll(const char *pszFilter)
{
    size_t i;
    int nPass = 0;
    int nFail = 0;

    for (i = 0; i < m_vecEntry.size(); i ++) {
        if ((pszFilter != NULL) && (strstr(m_vecEntry[i].pszName, pszFilter) == NULL)) {
            continue;
        }

        m_nFailedInCase = 0;
        printf("  %s\n", m_vecEntry[i].pszName);
        m_vecEntry[i].pfFunc();

        if (m_nFailedInCase == 0) {
            nPass ++;
        } else {
            nFail ++;
        }
    }

    printf("\n=== 成功 %d / 失敗 %d ===\n", nPass, nFail);
    return (nFail == 0) ? 0 : 1;
}

int main(int argc, char **argv)
{
    WSADATA wsaData;
    const char *pszFilter = (argc > 1) ? argv[1] : NULL;
    int nRet;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup に失敗しました\n");
        return 1;
    }

    printf("=== SboSvr 通信層テスト ===\n");
    nRet = CTestRegistry::Get().RunAll(pszFilter);

    WSACleanup();
    return nRet;
}
