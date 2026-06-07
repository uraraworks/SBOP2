#include "StdAfx.h"
#include "AuditLog.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <deque>
#include <mutex>

namespace AuditLog
{
Entry::Entry()
        : id(0)
        , adminLevel(0)
        , statusCode(0)
        , bodySize(0)
{
}
}

namespace
{
// 最大保持件数（リングバッファ）
const size_t kDefaultCapacity = 500;
// bodyDigest の最大長（マルチバイト考慮せず単純切り詰め）
const size_t kBodyDigestMaxLen = 256;

std::mutex                      g_mutex;
std::deque<AuditLog::Entry>     g_entries;
unsigned long long              g_nextId = 1;

std::string BuildTimestampUtc()
{
        // ISO8601 UTC "YYYY-MM-DDTHH:MM:SSZ"
        std::time_t now = std::time(NULL);
        std::tm tmUtc;
#if defined(_WIN32)
        gmtime_s(&tmUtc, &now);
#else
        tmUtc = *std::gmtime(&now);
#endif
        char szBuf[32];
        std::snprintf(szBuf, sizeof(szBuf), "%04d-%02d-%02dT%02d:%02d:%02dZ",
                tmUtc.tm_year + 1900, tmUtc.tm_mon + 1, tmUtc.tm_mday,
                tmUtc.tm_hour, tmUtc.tm_min, tmUtc.tm_sec);
        return std::string(szBuf);
}

// 制御文字を可視化（改行/タブは空白へ）し、bodyDigest を安全な文字列にする
std::string BuildBodyDigest(const std::string &body)
{
        if (body.empty()) {
                return std::string();
        }
        size_t nLen = body.size();
        if (nLen > kBodyDigestMaxLen) {
                nLen = kBodyDigestMaxLen;
        }
        std::string out;
        out.reserve(nLen);
        for (size_t i = 0; i < nLen; ++i) {
                unsigned char ch = static_cast<unsigned char>(body[i]);
                if (ch == '\r' || ch == '\n' || ch == '\t') {
                        out.push_back(' ');
                } else if (ch < 0x20) {
                        out.push_back('?');
                } else {
                        out.push_back(static_cast<char>(ch));
                }
        }
        if (body.size() > kBodyDigestMaxLen) {
                out.append("...");
        }
        return out;
}

bool StartsWith(const std::string &text, const char *pszPrefix)
{
        size_t nLen = std::strlen(pszPrefix);
        if (text.size() < nLen) {
                return false;
        }
        return text.compare(0, nLen, pszPrefix) == 0;
}
}

namespace AuditLog
{
bool ShouldAudit(const std::string &method, const std::string &path)
{
        // 変更系メソッドのみ対象
        if (method != "POST" && method != "PUT" && method != "DELETE") {
                return false;
        }
        // /api/ 配下のみ
        if (!StartsWith(path, "/api/")) {
                return false;
        }
        // ノイズ除去（選択状態の頻繁な更新と、監査ログ自身の GET 等は対象外）
        if (StartsWith(path, "/api/selection")) {
                return false;
        }
        if (StartsWith(path, "/api/audit-logs")) {
                return false;
        }
        return true;
}

void Record(
        const std::string &method,
        const std::string &path,
        int statusCode,
        const std::string &actor,
        const std::string &accountId,
        int adminLevel,
        const std::string &requestBody)
{
        // 2xx のみ記録（失敗リクエストはノイズになるので最小実用セットでは除外）
        // TODO: 失敗リクエストの記録・フィルタも後続で検討
        if (statusCode < 200 || statusCode >= 300) {
                return;
        }

        Entry entry;
        entry.timestamp = BuildTimestampUtc();
        entry.actor = actor.empty() ? std::string("-") : actor;
        entry.accountId = accountId;
        entry.adminLevel = adminLevel;
        entry.method = method;
        entry.path = path;
        entry.statusCode = statusCode;
        entry.bodyDigest = BuildBodyDigest(requestBody);
        entry.bodySize = requestBody.size();

        std::lock_guard<std::mutex> lock(g_mutex);
        entry.id = g_nextId++;
        g_entries.push_back(entry);
        while (g_entries.size() > kDefaultCapacity) {
                g_entries.pop_front();
        }
}

std::vector<Entry> Query(
        const std::string &actorFilter,
        const std::string &methodFilter,
        const std::string &pathPrefixFilter,
        size_t maxCount)
{
        std::vector<Entry> result;
        std::lock_guard<std::mutex> lock(g_mutex);

        // 新しい順に走査してフィルタを適用
        for (std::deque<Entry>::const_reverse_iterator it = g_entries.rbegin();
             it != g_entries.rend(); ++it) {
                if (!actorFilter.empty() && it->actor != actorFilter) {
                        continue;
                }
                if (!methodFilter.empty() && it->method != methodFilter) {
                        continue;
                }
                if (!pathPrefixFilter.empty()) {
                        if (it->path.size() < pathPrefixFilter.size()) {
                                continue;
                        }
                        if (it->path.compare(0, pathPrefixFilter.size(), pathPrefixFilter) != 0) {
                                continue;
                        }
                }
                result.push_back(*it);
                if (maxCount > 0 && result.size() >= maxCount) {
                        break;
                }
        }
        return result;
}

size_t Size()
{
        std::lock_guard<std::mutex> lock(g_mutex);
        return g_entries.size();
}

size_t Capacity()
{
        return kDefaultCapacity;
}
}
