/// @file TestFramework.h
/// @brief 最小限のテスト基盤 定義ファイル
/// @copyright Copyright(C)URARA-works
///
/// 外部ライブラリに依存しない小さな仕組み。
/// TEST(名前) { ... } と書くと自動で登録され、実行される。
///
///   TEST(SomeName)
///   {
///       CHECK(条件);
///       CHECK_EQ(期待値, 実際の値);
///   }

#pragma once

#include <stdio.h>
#include <vector>

typedef void (*PFTESTFUNC)(void);

// テストの登録先

class CTestRegistry
{
public:
    struct ENTRY {
        const char *pszName;
        PFTESTFUNC  pfFunc;
    };

    static CTestRegistry &Get(void);

    void Add(const char *pszName, PFTESTFUNC pfFunc);
    int  RunAll(const char *pszFilter);

    void Fail(const char *pszFile, int nLine, const char *pszMsg);

private:
    CTestRegistry(void) : m_nFailedInCase(0) {}

private:
    std::vector<ENTRY> m_vecEntry;
    int m_nFailedInCase;
};

// 登録用のヘルパ

class CTestRegistrar
{
public:
    CTestRegistrar(const char *pszName, PFTESTFUNC pfFunc)
    {
        CTestRegistry::Get().Add(pszName, pfFunc);
    }
};

#define TEST(name)                                                      \
    static void TestFunc_##name(void);                                  \
    static CTestRegistrar g_TestReg_##name(#name, TestFunc_##name);     \
    static void TestFunc_##name(void)

#define CHECK(cond)                                                     \
    do {                                                                \
        if (!(cond)) {                                                  \
            CTestRegistry::Get().Fail(__FILE__, __LINE__, #cond);       \
            return;                                                     \
        }                                                               \
    } while (0)

#define CHECK_EQ(expect, actual)                                        \
    do {                                                                \
        if ((expect) != (actual)) {                                     \
            char szMsg_[256];                                           \
            _snprintf_s(szMsg_, sizeof (szMsg_), _TRUNCATE,             \
                    "%s == %s  (期待 %lld / 実際 %lld)",                \
                    #expect, #actual,                                   \
                    (long long)(expect), (long long)(actual));          \
            CTestRegistry::Get().Fail(__FILE__, __LINE__, szMsg_);      \
            return;                                                     \
        }                                                               \
    } while (0)
