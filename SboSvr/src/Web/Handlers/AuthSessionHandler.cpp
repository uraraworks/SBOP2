#include "StdAfx.h"
#include "AuthSessionHandler.h"

#include <map>
#include <mutex>
#include <sstream>
#include <utility>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "Web/SessionStore.h"
#include "Web/Handlers/SelectionHandler.h"
#include "AdminRolesHandler.h"
#include "MgrData.h"
#include "GlobalDefine.h"
#include "LibInfo/LibInfoAccount.h"
#include "Info/InfoAccount.h"
#include "TextOutput.h"

namespace
{
/// ブルートフォース対策: ログインIDごとの連続失敗回数と最終失敗時刻
const int kMaxFailCount = 5;
const time_t kLockoutSeconds = 300;
const size_t kMaxFailEntries = 512;

struct FailInfo
{
        int     nFailCount;
        time_t  timeLastFail;
};

std::map<std::string, FailInfo> &GetFailMap()
{
        static std::map<std::string, FailInfo> failMap;
        return failMap;
}

std::mutex &GetFailMutex()
{
        static std::mutex mtx;
        return mtx;
}

/// @brief ロック中かどうかを判定する。ロック中なら残り秒数を返す。
bool IsLockedOut(const std::string &loginId, int &outRetryAfterSeconds)
{
        std::lock_guard<std::mutex> lock(GetFailMutex());
        std::map<std::string, FailInfo>::iterator it = GetFailMap().find(loginId);
        if (it == GetFailMap().end()) {
                return false;
        }

        if (it->second.nFailCount < kMaxFailCount) {
                return false;
        }

        time_t now = time(NULL);
        time_t elapsed = now - it->second.timeLastFail;
        if (elapsed >= kLockoutSeconds) {
                return false;
        }

        outRetryAfterSeconds = static_cast<int>(kLockoutSeconds - elapsed);
        return true;
}

/// @brief ログイン失敗を記録する。
void RecordFailure(const std::string &loginId)
{
        std::lock_guard<std::mutex> lock(GetFailMutex());
        std::map<std::string, FailInfo> &failMap = GetFailMap();

        if ((failMap.size() >= kMaxFailEntries) && (failMap.find(loginId) == failMap.end())) {
                std::map<std::string, FailInfo>::iterator oldestIt = failMap.end();
                for (std::map<std::string, FailInfo>::iterator it = failMap.begin(); it != failMap.end(); ++it) {
                        if ((oldestIt == failMap.end()) || (it->second.timeLastFail < oldestIt->second.timeLastFail)) {
                                oldestIt = it;
                        }
                }
                if (oldestIt != failMap.end()) {
                        failMap.erase(oldestIt);
                }
        }

        FailInfo &info = failMap[loginId];
        info.nFailCount++;
        info.timeLastFail = time(NULL);
}

/// @brief ログイン成功時に失敗履歴をクリアする。
void ClearFailure(const std::string &loginId)
{
        std::lock_guard<std::mutex> lock(GetFailMutex());
        GetFailMap().erase(loginId);
}

std::string BuildAuthBody(const AuthProvider::AuthContext &context)
{
        std::ostringstream oss;
        oss << "{";
        oss << "\"authenticated\":" << (context.authenticated ? "true" : "false") << ",";
        oss << "\"loginId\":\"" << JsonUtils::Escape(context.loginId) << "\",";
        oss << "\"accountId\":\"" << JsonUtils::Escape(context.accountId) << "\",";
        oss << "\"adminLevel\":" << context.adminLevel << ",";
        oss << "\"roles\":[";
        for (size_t i = 0; i < context.roles.size(); ++i) {
                if (i > 0) {
                        oss << ",";
                }
                oss << "\"" << JsonUtils::Escape(context.roles[i]) << "\"";
        }
        oss << "]";
        oss << "}";
        return oss.str();
}

std::string BuildAnonymousAuthBody()
{
        return "{\"authenticated\":false,\"loginId\":\"\",\"accountId\":\"\",\"adminLevel\":0,\"roles\":[]}";
}

std::string BuildCookieValue(const std::string &token, BOOL bSecure)
{
        std::ostringstream oss;
        oss << "SESSID=" << token << "; Path=/; SameSite=Lax; HttpOnly; Max-Age=28800";
        if (bSecure) {
                oss << "; Secure";
        }
        return oss.str();
}

std::string BuildExpiredCookieValue(BOOL bSecure)
{
        std::ostringstream oss;
        oss << "SESSID=; Path=/; SameSite=Lax; HttpOnly; Max-Age=0";
        if (bSecure) {
                oss << "; Secure";
        }
        return oss.str();
}
}

CAuthMeHandler::CAuthMeHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CAuthMeHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        response.statusLine = "HTTP/1.1 200 OK";
        if (authStatus != AuthProvider::AuthStatusOk) {
                response.SetJsonBody(BuildAnonymousAuthBody());
                return;
        }

        response.SetJsonBody(BuildAuthBody(authContext));
}

CAdminLoginHandler::CAdminLoginHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CAdminLoginHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (m_pMgrData == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        std::string loginId;
        std::string password;
        if (!JsonUtils::TryGetString(request.body, "loginId", loginId) ||
            !JsonUtils::TryGetString(request.body, "password", password) ||
            loginId.empty()) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_request\"}");
                return;
        }

        int nRetryAfterSeconds = 0;
        if (IsLockedOut(loginId, nRetryAfterSeconds)) {
                response.statusLine = "HTTP/1.1 429 Too Many Requests";
                response.SetHeader("Retry-After", std::to_string(nRetryAfterSeconds));
                std::ostringstream oss;
                oss << "{\"error\":\"too_many_attempts\",\"retryAfterSeconds\":" << nRetryAfterSeconds << "}";
                response.SetJsonBody(oss.str());
                return;
        }

        CLibInfoAccount *pAccountLib = m_pMgrData->GetLibInfoAccount();
        if (pAccountLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        AuthProvider::AuthContext context;
        pAccountLib->Enter();
        PCInfoAccount pAccount = pAccountLib->GetPtr(loginId.c_str());
        if ((pAccount == NULL) || (pAccount->m_strPassword != password.c_str())) {
                pAccountLib->Leave();
                RecordFailure(loginId);
                if (m_pMgrData->GetLog() != NULL) {
                        m_pMgrData->GetLog()->Write("[AdminLogin] failed loginId=%s", loginId.c_str());
                }
                response.statusLine = "HTTP/1.1 401 Unauthorized";
                response.SetJsonBody("{\"error\":\"unauthorized\"}");
                return;
        }
        // ログイン拒否フラグが立っているアカウントは、権限に関わらず管理画面へ入れない
        if (pAccount->m_bDisable) {
                pAccountLib->Leave();
                RecordFailure(loginId);
                if (m_pMgrData->GetLog() != NULL) {
                        m_pMgrData->GetLog()->Write("[AdminLogin] disabled loginId=%s", loginId.c_str());
                }
                response.statusLine = "HTTP/1.1 403 Forbidden";
                response.SetJsonBody("{\"error\":\"forbidden\",\"required\":\"admin\"}");
                return;
        }
        if (pAccount->m_nAdminLevel <= ADMINLEVEL_NONE) {
                pAccountLib->Leave();
                RecordFailure(loginId);
                if (m_pMgrData->GetLog() != NULL) {
                        m_pMgrData->GetLog()->Write("[AdminLogin] forbidden loginId=%s", loginId.c_str());
                }
                response.statusLine = "HTTP/1.1 403 Forbidden";
                response.SetJsonBody("{\"error\":\"forbidden\",\"required\":\"admin\"}");
                return;
        }

        DWORD dwAccountID = pAccount->m_dwAccountID;
        std::string resolvedLoginId = (LPCSTR)pAccount->m_strAccount;

        context.authenticated = true;
        context.loginId = resolvedLoginId;
        {
                std::ostringstream oss;
                oss << dwAccountID;
                context.accountId = oss.str();
        }
        context.adminLevel = pAccount->m_nAdminLevel;
        context.roles = AdminRoleCatalog::ResolveRoles(pAccount->m_nAdminLevel);
        pAccountLib->Leave();

        std::string token = SessionStore::Create(dwAccountID, resolvedLoginId.c_str());
        if (token.empty()) {
                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"session_create_failed\"}");
                return;
        }
        context.sessionId = token;

        ClearFailure(loginId);
        if (m_pMgrData->GetLog() != NULL) {
                m_pMgrData->GetLog()->Write("[AdminLogin] success loginId=%s", resolvedLoginId.c_str());
        }

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetHeader("Set-Cookie", BuildCookieValue(token, m_pMgrData->GetCookieSecure()));
        response.SetJsonBody(BuildAuthBody(context));
}

CAuthLogoutHandler::CAuthLogoutHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CAuthLogoutHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        std::string token;
        if (AuthProvider::TryGetSessionCookie(request, token)) {
                SessionStore::Destroy(token);
                CSelectionStore::GetInstance().Clear(token);
        }

        BOOL bCookieSecure = (m_pMgrData != NULL) ? m_pMgrData->GetCookieSecure() : FALSE;

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetHeader("Set-Cookie", BuildExpiredCookieValue(bCookieSecure));
        response.SetJsonBody("{\"ok\":true}");
}
