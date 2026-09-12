/// @file TestProxyIpRegistry.cpp
/// @brief ProxyIpRegistry / ProxyHeaderParser のテスト
/// @copyright Copyright(C)URARA-works
///
/// WebSocketBridgeは同一プロセス内でゲームTCPへ127.0.0.1として繋ぎ直すため、
/// ゲーム側から見た相手IPは常にloopbackになってしまう。この対応表と
/// ヘッダ解析関数が正しく動くことをここで検証する。

#include "StdAfx.h"
#include "TestFramework.h"
#include "../SboSvr/src/Web/ProxyIpRegistry.h"
#include "../SboSvr/src/Web/ProxyHeaderParser.h"
#include <thread>
#include <vector>

// テスト間で登録が残らないよう、使うポート番号はテストごとに変える
static unsigned short NextRegistryPort(void)
{
    static unsigned short s_wPort = 40000;
    return s_wPort ++;
}

// IPv4のドット表記からネットワークバイトオーダーのDWORDを組み立てる(テスト用)
static unsigned long MakeIpNet(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
    unsigned long dwResult = 0;
    unsigned char *p = reinterpret_cast<unsigned char *>(&dwResult);
    p[0] = a; p[1] = b; p[2] = c; p[3] = d;
    return dwResult;
}

//////////////////////////////////////////////////////////////////////
// ProxyIpRegistry

TEST(Registry_登録前は見つからない)
{
    unsigned long dwOut = 0;
    unsigned short wPort = NextRegistryPort();

    CHECK(ProxyIpRegistry::Lookup(wPort, dwOut) == false);
}

TEST(Registry_登録すると見つかる)
{
    unsigned short wPort = NextRegistryPort();
    unsigned long dwIp = MakeIpNet(203, 0, 113, 42);
    unsigned long dwOut = 0;

    ProxyIpRegistry::Register(wPort, dwIp);
    CHECK(ProxyIpRegistry::Lookup(wPort, dwOut) == true);
    CHECK_EQ((long long)dwIp, (long long)dwOut);

    ProxyIpRegistry::Unregister(wPort);
}

TEST(Registry_解除すると見つからなくなる)
{
    unsigned short wPort = NextRegistryPort();
    unsigned long dwOut = 0;

    ProxyIpRegistry::Register(wPort, MakeIpNet(198, 51, 100, 7));
    ProxyIpRegistry::Unregister(wPort);

    CHECK(ProxyIpRegistry::Lookup(wPort, dwOut) == false);
}

TEST(Registry_同じポートに再登録すると上書きされる)
{
    unsigned short wPort = NextRegistryPort();
    unsigned long dwOut = 0;

    ProxyIpRegistry::Register(wPort, MakeIpNet(10, 0, 0, 1));
    ProxyIpRegistry::Register(wPort, MakeIpNet(10, 0, 0, 2));

    CHECK(ProxyIpRegistry::Lookup(wPort, dwOut) == true);
    CHECK_EQ((long long)MakeIpNet(10, 0, 0, 2), (long long)dwOut);

    ProxyIpRegistry::Unregister(wPort);
}

TEST(Registry_並行アクセスしてもクラッシュせず件数が合う)
{
    const int kThreads = 8;
    const int kPerThread = 50;
    unsigned short wBasePort = NextRegistryPort();
    std::vector<std::thread> vecThreads;

    // ポート範囲を丸ごと予約する(以降のテストと重ならないよう連番で進めておく)
    for (int i = 1; i < kThreads * kPerThread; ++i) {
        NextRegistryPort();
    }

    for (int t = 0; t < kThreads; ++t) {
        vecThreads.push_back(std::thread([t, wBasePort, kPerThread]() {
            for (int i = 0; i < kPerThread; ++i) {
                unsigned short wPort = static_cast<unsigned short>(wBasePort + t * kPerThread + i);
                ProxyIpRegistry::Register(wPort, MakeIpNet(127, 0, 0, 1));
            }
        }));
    }
    for (size_t i = 0; i < vecThreads.size(); ++i) {
        vecThreads[i].join();
    }

    int nFound = 0;
    for (int i = 0; i < kThreads * kPerThread; ++i) {
        unsigned short wPort = static_cast<unsigned short>(wBasePort + i);
        unsigned long dwOut = 0;
        if (ProxyIpRegistry::Lookup(wPort, dwOut)) {
            ++nFound;
        }
        ProxyIpRegistry::Unregister(wPort);
    }
    CHECK_EQ(kThreads * kPerThread, nFound);
}

TEST(Registry_IsLoopbackIPv4_127系はtrue)
{
    CHECK(ProxyIpRegistry::IsLoopbackIPv4(MakeIpNet(127, 0, 0, 1)) == true);
    CHECK(ProxyIpRegistry::IsLoopbackIPv4(MakeIpNet(127, 255, 0, 9)) == true);
}

TEST(Registry_IsLoopbackIPv4_127以外はfalse)
{
    CHECK(ProxyIpRegistry::IsLoopbackIPv4(MakeIpNet(192, 168, 1, 1)) == false);
    CHECK(ProxyIpRegistry::IsLoopbackIPv4(MakeIpNet(8, 8, 8, 8)) == false);
    CHECK(ProxyIpRegistry::IsLoopbackIPv4(MakeIpNet(126, 255, 255, 255)) == false);
}

//////////////////////////////////////////////////////////////////////
// ProxyHeaderParser

TEST(HeaderParser_XFFの末尾値を取り出せる)
{
    // ARR等は末尾にしか実IPを付け足さない。左端はクライアントが自由に偽装できるため
    // 右端(最後の要素)だけを信用する。
    std::string sReq =
        "GET /ws HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "X-Forwarded-For: 1.2.3.4, 5.6.7.8\r\n"
        "\r\n";
    unsigned long dwOut = 0;

    CHECK(ProxyHeaderParser::ExtractClientIp(sReq, dwOut) == true);
    CHECK_EQ((long long)MakeIpNet(5, 6, 7, 8), (long long)dwOut);
}

TEST(HeaderParser_XFFが単一値のときはその値を使う)
{
    std::string sReq =
        "GET /ws HTTP/1.1\r\n"
        "X-Forwarded-For: 5.6.7.8\r\n"
        "\r\n";
    unsigned long dwOut = 0;

    CHECK(ProxyHeaderParser::ExtractClientIp(sReq, dwOut) == true);
    CHECK_EQ((long long)MakeIpNet(5, 6, 7, 8), (long long)dwOut);
}

TEST(HeaderParser_ヘッダ名は大文字小文字を区別しない)
{
    std::string sReq =
        "GET /ws HTTP/1.1\r\n"
        "x-forwarded-for: 198.51.100.9\r\n"
        "\r\n";
    unsigned long dwOut = 0;

    CHECK(ProxyHeaderParser::ExtractClientIp(sReq, dwOut) == true);
    CHECK_EQ((long long)MakeIpNet(198, 51, 100, 9), (long long)dwOut);
}

TEST(HeaderParser_X_Real_IPは信用しない)
{
    // X-Real-IP はIIS/ARRが付与しないヘッダーなのでクライアントの偽装がそのまま届く。
    // このヘッダーだけがあっても採用してはいけない。
    std::string sReq =
        "GET /ws HTTP/1.1\r\n"
        "X-Real-IP: 1.2.3.4\r\n"
        "\r\n";
    unsigned long dwOut = 0;

    CHECK(ProxyHeaderParser::ExtractClientIp(sReq, dwOut) == false);
}

TEST(HeaderParser_ヘッダが無ければfalse)
{
    std::string sReq =
        "GET /ws HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "\r\n";
    unsigned long dwOut = 0;

    CHECK(ProxyHeaderParser::ExtractClientIp(sReq, dwOut) == false);
}

TEST(HeaderParser_不正な値は拒否する)
{
    // IPv6、ポート付き、文字列など、この実装が対応しない値は無視してfalseになる
    std::string sReq =
        "GET /ws HTTP/1.1\r\n"
        "X-Forwarded-For: ::1\r\n"
        "\r\n";
    unsigned long dwOut = 0;

    CHECK(ProxyHeaderParser::ExtractClientIp(sReq, dwOut) == false);
}

TEST(HeaderParser_XFF末尾が不正なら左側にフォールバックしない)
{
    // 右端(プロキシが付けたはずの要素)が不正な場合、左側のクライアント申告値に
    // フォールバックしてはいけない(偽装の再侵入経路になる)。
    std::string sReq =
        "GET /ws HTTP/1.1\r\n"
        "X-Forwarded-For: 1.2.3.4, garbage\r\n"
        "\r\n";
    unsigned long dwOut = 0;

    CHECK(ProxyHeaderParser::ExtractClientIp(sReq, dwOut) == false);
}

TEST(HeaderParser_ポート付きは拒否する)
{
    std::string sReq =
        "GET /ws HTTP/1.1\r\n"
        "X-Forwarded-For: 1.2.3.4:5678\r\n"
        "\r\n";
    unsigned long dwOut = 0;

    CHECK(ProxyHeaderParser::ExtractClientIp(sReq, dwOut) == false);
}

TEST(HeaderParser_同名ヘッダーが複数行あれば最後の行の末尾を使う)
{
    std::string sReq =
        "GET /ws HTTP/1.1\r\n"
        "X-Forwarded-For: 1.2.3.4\r\n"
        "X-Forwarded-For: 5.6.7.8, 9.9.9.9\r\n"
        "\r\n";
    unsigned long dwOut = 0;

    CHECK(ProxyHeaderParser::ExtractClientIp(sReq, dwOut) == true);
    CHECK_EQ((long long)MakeIpNet(9, 9, 9, 9), (long long)dwOut);
}

TEST(HeaderParser_偽装的なヘッダ名の部分一致は誤検出しない)
{
    // "Foo-X-Forwarded-For:" のように行頭でない一致を拾わないことを確認する
    std::string sReq =
        "GET /ws HTTP/1.1\r\n"
        "Foo-X-Forwarded-For: 1.2.3.4\r\n"
        "\r\n";
    unsigned long dwOut = 0;

    CHECK(ProxyHeaderParser::ExtractClientIp(sReq, dwOut) == false);
}

TEST(HeaderParser_数値範囲外のオクテットは拒否する)
{
    std::string sReq =
        "GET /ws HTTP/1.1\r\n"
        "X-Forwarded-For: 999.1.1.1\r\n"
        "\r\n";
    unsigned long dwOut = 0;

    CHECK(ProxyHeaderParser::ExtractClientIp(sReq, dwOut) == false);
}
