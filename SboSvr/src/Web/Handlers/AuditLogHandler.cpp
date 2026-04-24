#include "stdafx.h"
#include "AuditLogHandler.h"

#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Web/AuthProvider.h"
#include "Web/AuditLog.h"
#include "Web/JsonUtils.h"

CAuditLogListHandler::CAuditLogListHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

namespace
{
// URL デコード（%XX と '+' を空白へ）。簡易実装。
std::string UrlDecode(const std::string &text)
{
        std::string out;
        out.reserve(text.size());
        for (size_t i = 0; i < text.size(); ++i) {
                char ch = text[i];
                if (ch == '+') {
                        out.push_back(' ');
                } else if (ch == '%' && (i + 2) < text.size()) {
                        auto hexVal = [](char c) -> int {
                                if (c >= '0' && c <= '9') return c - '0';
                                if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
                                if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
                                return -1;
                        };
                        int h1 = hexVal(text[i + 1]);
                        int h2 = hexVal(text[i + 2]);
                        if (h1 >= 0 && h2 >= 0) {
                                out.push_back(static_cast<char>((h1 << 4) | h2));
                                i += 2;
                        } else {
                                out.push_back(ch);
                        }
                } else {
                        out.push_back(ch);
                }
        }
        return out;
}

// クエリ文字列から key=value を抽出。見つからなければ空文字。
std::string GetQueryParam(const std::string &path, const std::string &key)
{
        size_t nQuery = path.find('?');
        if (nQuery == std::string::npos) {
                return std::string();
        }
        std::string qs = path.substr(nQuery + 1);
        size_t nPos = 0;
        while (nPos < qs.size()) {
                size_t nEnd = qs.find('&', nPos);
                if (nEnd == std::string::npos) {
                        nEnd = qs.size();
                }
                std::string pair = qs.substr(nPos, nEnd - nPos);
                size_t nEq = pair.find('=');
                if (nEq != std::string::npos) {
                        std::string k = pair.substr(0, nEq);
                        std::string v = pair.substr(nEq + 1);
                        if (k == key) {
                                return UrlDecode(v);
                        }
                }
                if (nEnd == qs.size()) {
                        break;
                }
                nPos = nEnd + 1;
        }
        return std::string();
}
}

void CAuditLogListHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        // 認証（管理画面操作と同等の扱い）
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }
        if (authStatus != AuthProvider::AuthStatusOk) {
                response.statusLine = "HTTP/1.1 401 Unauthorized";
                response.SetJsonBody("{\"error\":\"unauthorized\"}");
                return;
        }
        // ロールチェック（監査ログ閲覧は admin 権限必須）
        if (!AuthProvider::HasRole(authContext, "admin")) {
                response.statusLine = "HTTP/1.1 403 Forbidden";
                response.SetJsonBody(AuthProvider::BuildForbiddenBody("admin"));
                return;
        }

        // クエリパラメータ取得
        std::string actorFilter  = GetQueryParam(request.path, "actor");
        std::string methodFilter = GetQueryParam(request.path, "method");
        std::string pathFilter   = GetQueryParam(request.path, "path");
        std::string limitStr     = GetQueryParam(request.path, "limit");

        size_t maxCount = 100; // 既定
        if (!limitStr.empty()) {
                long v = std::strtol(limitStr.c_str(), NULL, 10);
                if (v > 0 && v <= 500) {
                        maxCount = static_cast<size_t>(v);
                } else if (v > 500) {
                        maxCount = 500;
                }
        }

        std::vector<AuditLog::Entry> entries = AuditLog::Query(
                actorFilter, methodFilter, pathFilter, maxCount);

        // JSON 組み立て
        std::ostringstream oss;
        oss << "{\"capacity\":" << AuditLog::Capacity()
            << ",\"total\":" << AuditLog::Size()
            << ",\"count\":" << entries.size()
            << ",\"entries\":[";
        for (size_t i = 0; i < entries.size(); ++i) {
                const AuditLog::Entry &e = entries[i];
                if (i > 0) oss << ",";
                oss << "{";
                oss << "\"id\":" << e.id;
                oss << ",\"timestamp\":\"" << JsonUtils::Escape(e.timestamp) << "\"";
                oss << ",\"actor\":\"" << JsonUtils::Escape(e.actor) << "\"";
                oss << ",\"accountId\":\"" << JsonUtils::Escape(e.accountId) << "\"";
                oss << ",\"adminLevel\":" << e.adminLevel;
                oss << ",\"method\":\"" << JsonUtils::Escape(e.method) << "\"";
                oss << ",\"path\":\"" << JsonUtils::Escape(e.path) << "\"";
                oss << ",\"statusCode\":" << e.statusCode;
                oss << ",\"bodyDigest\":\"" << JsonUtils::Escape(e.bodyDigest) << "\"";
                oss << ",\"bodySize\":" << e.bodySize;
                oss << "}";
        }
        oss << "]}";

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}
