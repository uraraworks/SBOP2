#include "stdafx.h"
#include "AuthSessionHandler.h"

#include <sstream>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "AdminRolesHandler.h"
#include "MgrData.h"
#include "GlobalDefine.h"
#include "LibInfo/LibInfoAccount.h"
#include "Info/InfoAccount.h"

namespace
{
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

std::string BuildCookieValue(const std::string &loginId)
{
        std::ostringstream oss;
        oss << "SESSID=" << loginId << "; Path=/; SameSite=Lax; HttpOnly";
        return oss.str();
}

std::string BuildExpiredCookieValue()
{
        return "SESSID=; Path=/; SameSite=Lax; HttpOnly; Max-Age=0";
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
                response.statusLine = "HTTP/1.1 401 Unauthorized";
                response.SetJsonBody("{\"error\":\"unauthorized\"}");
                return;
        }
        if (pAccount->m_nAdminLevel <= ADMINLEVEL_NONE) {
                pAccountLib->Leave();
                response.statusLine = "HTTP/1.1 403 Forbidden";
                response.SetJsonBody("{\"error\":\"forbidden\",\"required\":\"admin\"}");
                return;
        }

        context.authenticated = true;
        context.sessionId = loginId;
        context.loginId = (LPCSTR)pAccount->m_strAccount;
        {
                std::ostringstream oss;
                oss << pAccount->m_dwAccountID;
                context.accountId = oss.str();
        }
        context.adminLevel = pAccount->m_nAdminLevel;
        context.roles = AdminRoleCatalog::ResolveRoles(pAccount->m_nAdminLevel);
        pAccountLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetHeader("Set-Cookie", BuildCookieValue(context.loginId));
        response.SetJsonBody(BuildAuthBody(context));
}

void CAuthLogoutHandler::Handle(const HttpRequest & /*request*/, HttpResponse &response)
{
        response.statusLine = "HTTP/1.1 200 OK";
        response.SetHeader("Set-Cookie", BuildExpiredCookieValue());
        response.SetJsonBody("{\"ok\":true}");
}
