/// @file DebugFixtureGuard.cpp
/// @brief /api/debug/fixture (テスト準備API) の接続元判定 実装ファイル

#include "StdAfx.h"
#include "DebugFixtureGuard.h"
#include "HttpTypes.h"

namespace DebugFixtureGuard
{
bool IsAllowedRequest(const HttpRequest &request)
{
        // IIS(ARR)等のリバースプロキシはこれらのヘッダーを付与してから中継する。
        // 直接のloopback接続からは(クライアントが偽装しない限り)付かないため、
        // 存在するだけでプロキシ経由の疑いありとして拒否する。
        if (request.FindHeader("X-Forwarded-For") != NULL) {
                return false;
        }
        if (request.FindHeader("X-Real-IP") != NULL) {
                return false;
        }

        return (request.clientIp == "127.0.0.1") || (request.clientIp == "::1");
}
}
