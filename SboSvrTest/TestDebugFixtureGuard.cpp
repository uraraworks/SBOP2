/// @file TestDebugFixtureGuard.cpp
/// @brief DebugFixtureGuard::IsAllowedRequest のテスト
/// @copyright Copyright(C)URARA-works
///
/// /api/debug/fixture(テスト準備API、_DEBUG限定)は loopbackからの直接
/// リクエストだけを受け付ける。IIS等のプロキシ経由や別ホストからの
/// アクセスを404として弾けることをここで検証する。

#include "StdAfx.h"
#include "TestFramework.h"
#include "../SboSvr/src/Web/DebugFixtureGuard.h"
#include "../SboSvr/src/Web/HttpTypes.h"

namespace
{
    HttpRequest MakeRequest(const std::string &strClientIp)
    {
        HttpRequest request;
        request.method = "POST";
        request.path = "/api/debug/fixture";
        request.clientIp = strClientIp;
        return request;
    }

    void AddHeader(HttpRequest &request, const char *pszName, const char *pszValue)
    {
        HttpHeader header;
        header.name = pszName;
        header.value = pszValue;
        request.headers.push_back(header);
    }
}

TEST(DebugFixtureGuard_loopbackIPv4かつヘッダ無しなら許可)
{
    HttpRequest request = MakeRequest("127.0.0.1");
    CHECK(DebugFixtureGuard::IsAllowedRequest(request) == true);
}

TEST(DebugFixtureGuard_loopbackIPv6かつヘッダ無しなら許可)
{
    HttpRequest request = MakeRequest("::1");
    CHECK(DebugFixtureGuard::IsAllowedRequest(request) == true);
}

TEST(DebugFixtureGuard_loopback以外は拒否)
{
    HttpRequest request = MakeRequest("192.168.1.10");
    CHECK(DebugFixtureGuard::IsAllowedRequest(request) == false);
}

TEST(DebugFixtureGuard_clientIp未設定は拒否)
{
    HttpRequest request = MakeRequest("");
    CHECK(DebugFixtureGuard::IsAllowedRequest(request) == false);
}

TEST(DebugFixtureGuard_loopbackでもXForwardedForがあれば拒否)
{
    // IIS(ARR)等のリバースプロキシ経由。ProxyIpRegistry::IsLoopbackIPv4の仕組みで
    // clientIpがloopbackのまま偽装できるケースを想定し、ヘッダーの存在だけで拒否する。
    HttpRequest request = MakeRequest("127.0.0.1");
    AddHeader(request, "X-Forwarded-For", "203.0.113.5");
    CHECK(DebugFixtureGuard::IsAllowedRequest(request) == false);
}

TEST(DebugFixtureGuard_loopbackでもXRealIPがあれば拒否)
{
    HttpRequest request = MakeRequest("127.0.0.1");
    AddHeader(request, "X-Real-IP", "203.0.113.5");
    CHECK(DebugFixtureGuard::IsAllowedRequest(request) == false);
}

TEST(DebugFixtureGuard_ヘッダ名は大文字小文字を区別しない)
{
    HttpRequest request = MakeRequest("127.0.0.1");
    AddHeader(request, "x-forwarded-for", "203.0.113.5");
    CHECK(DebugFixtureGuard::IsAllowedRequest(request) == false);
}

TEST(DebugFixtureGuard_loopback以外かつヘッダ有りも拒否)
{
    HttpRequest request = MakeRequest("203.0.113.5");
    AddHeader(request, "X-Forwarded-For", "203.0.113.5");
    CHECK(DebugFixtureGuard::IsAllowedRequest(request) == false);
}
