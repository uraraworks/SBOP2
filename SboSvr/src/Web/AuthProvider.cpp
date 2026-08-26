#include "StdAfx.h"
#include "AuthProvider.h"

#include <cctype>
#include <sstream>

#include "MgrData.h"
#include "LibInfo/LibInfoAccount.h"
#include "Info/InfoAccount.h"
#include "Web/JsonUtils.h"
#include "Web/SessionStore.h"

namespace AdminRoleCatalog
{
std::vector<std::string> ResolveRoles(int nAdminLevel);
}

namespace
{
std::string TrimCopy(const std::string &text)
{
        size_t nStart = 0;
        while ((nStart < text.size()) && std::isspace(static_cast<unsigned char>(text[nStart]))) {
                ++nStart;
        }

        size_t nEnd = text.size();
        while ((nEnd > nStart) && std::isspace(static_cast<unsigned char>(text[nEnd - 1]))) {
                --nEnd;
        }

        return text.substr(nStart, nEnd - nStart);
}

bool TryGetCookieValue(const std::string &cookies, const std::string &name, std::string &outValue)
{
        outValue.clear();
        size_t nPos = 0;
        while (nPos < cookies.size()) {
                size_t nEnd = cookies.find(';', nPos);
                if (nEnd == std::string::npos) {
                        nEnd = cookies.size();
                }

                std::string pair = cookies.substr(nPos, nEnd - nPos);
                size_t nEqual = pair.find('=');
                if (nEqual != std::string::npos) {
                        std::string key = TrimCopy(pair.substr(0, nEqual));
                        if (key == name) {
                                outValue = TrimCopy(pair.substr(nEqual + 1));
                                return true;
                        }
                }

                if (nEnd == cookies.size()) {
                        break;
                }
                nPos = nEnd + 1;
        }
        return false;
}
}

namespace AuthProvider
{
AuthContext::AuthContext()
        : authenticated(false)
        , adminLevel(0)
{
}

AuthStatus Authenticate(const HttpRequest &request, CMgrData *pMgrData, AuthContext &outContext)
{
        outContext = AuthContext();

        std::string token;
        if (!TryGetSessionCookie(request, token)) {
                return AuthStatusMissingSession;
        }

        SessionStore::SessionInfo sessionInfo;
        if (!SessionStore::Touch(token, sessionInfo)) {
                return AuthStatusMissingSession;
        }

        if (pMgrData == NULL) {
                        return AuthStatusBackendUnavailable;
        }

        CLibInfoAccount *pAccountLib = pMgrData->GetLibInfoAccount();
        if (pAccountLib == NULL) {
                return AuthStatusBackendUnavailable;
        }

        pAccountLib->Enter();
        PCInfoAccount pAccount = pAccountLib->GetPtr(sessionInfo.dwAccountID);
        if (pAccount == NULL) {
                pAccountLib->Leave();
                return AuthStatusAccountNotFound;
        }
        // ログイン拒否フラグが立ったアカウントは、既存セッションであっても即座に無効化する
        if (pAccount->m_bDisable) {
                pAccountLib->Leave();
                SessionStore::Destroy(token);
                return AuthStatusAccountNotFound;
        }

        AuthContext context;
        context.authenticated = true;
        context.sessionId = token;
        context.loginId = (LPCSTR)pAccount->m_strAccount;
        {
                std::ostringstream oss;
                oss << pAccount->m_dwAccountID;
                context.accountId = oss.str();
        }
        context.adminLevel = pAccount->m_nAdminLevel;
        context.roles = AdminRoleCatalog::ResolveRoles(pAccount->m_nAdminLevel);

        pAccountLib->Leave();

        outContext = context;
        return AuthStatusOk;
}

bool TryGetSessionCookie(const HttpRequest &request, std::string &outToken)
{
        outToken.clear();

        const char *pszCookie = request.FindHeader("Cookie");
        if (pszCookie == NULL) {
                return false;
        }

        std::string cookieHeader = pszCookie;
        std::string token;
        if (!TryGetCookieValue(cookieHeader, "SESSID", token) || token.empty()) {
                return false;
        }

        outToken = token;
        return true;
}

bool HasRole(const AuthContext &context, const char *pszRoleId)
{
        if ((pszRoleId == NULL) || !context.authenticated) {
                return false;
        }
        for (std::vector<std::string>::const_iterator it = context.roles.begin(); it != context.roles.end(); ++it) {
                if (*it == pszRoleId) {
                        return true;
                }
        }
        return false;
}

std::string BuildForbiddenBody(const char *pszRequiredRole)
{
        std::ostringstream oss;
        oss << "{\"error\":\"forbidden\"";
        if ((pszRequiredRole != NULL) && (pszRequiredRole[0] != '\0')) {
                oss << ",\"requiredRole\":\"" << JsonUtils::Escape(pszRequiredRole) << "\"";
        }
        oss << '}';
        return oss.str();
}
}
