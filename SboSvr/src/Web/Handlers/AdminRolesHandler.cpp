#include "stdafx.h"
#include "AdminRolesHandler.h"

#include <cstdlib>
#include <sstream>

#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "GlobalDefine.h"
#include "LibInfo/LibInfoAccount.h"
#include "Info/InfoAccount.h"

namespace
{
const RoleDefinition g_roles[] = {
        {"SERVER_VIEW", "サーバー閲覧", "サーバー状態カードを閲覧できます。", {"SERVER_DASHBOARD"}, 1},
        {"ACCOUNT_CREATE", "アカウント登録", "新規アカウントの作成が可能です。", {"ACCOUNT_CREATE"}, 1},
        {"ROLE_UPDATE", "ロール更新", "管理者ロールの更新操作を実施できます。", {"ROLE_MANAGEMENT"}, 1},
        {"SERVER_ADMIN", "サーバー管理者", "サーバー再起動などの管理操作を行えます。", {"SERVER_CONTROL"}, 1},
};
const size_t g_roleCount = sizeof(g_roles) / sizeof(g_roles[0]);
}

CAdminRolesListHandler::CAdminRolesListHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CAdminRolesListHandler::Handle(const HttpRequest & /*request*/, HttpResponse &response)
{
        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(BuildResponse());
}

std::string CAdminRolesListHandler::BuildResponse() const
{
        std::ostringstream oss;
        oss << '[';
        for (size_t i = 0; i < g_roleCount; ++i) {
                if (i > 0) {
                        oss << ',';
                }
                oss << '{';
                oss << "\"id\":\"" << JsonUtils::Escape(g_roles[i].pszId) << "\",";
                oss << "\"name\":\"" << JsonUtils::Escape(g_roles[i].pszName) << "\",";
                oss << "\"description\":\"" << JsonUtils::Escape(g_roles[i].pszDescription) << "\",";
                oss << "\"featureFlags\":[";
                for (size_t j = 0; j < g_roles[i].nFeatureFlagCount; ++j) {
                        if (j > 0) {
                                oss << ',';
                        }
                        oss << "\"" << JsonUtils::Escape(g_roles[i].pszFeatureFlags[j]) << "\"";
                }
                oss << "]";
                oss << '}';
        }
        oss << ']';
        return oss.str();
}

CAdminRolesUpdateHandler::CAdminRolesUpdateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CAdminRolesUpdateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        CLibInfoAccount *pAccountLib = GetAccountLibrary();
        if (pAccountLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        DWORD dwAccountId = 0;
        if (!ParseAccountId(request.path, dwAccountId)) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_account_id\"}");
                return;
        }

        std::vector<std::string> roles;
        if (!ParseRoles(request.body, roles)) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_roles\"}");
                return;
        }

        if (!ValidateRoles(roles)) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"unknown_role\"}");
                return;
        }

        int nAdminLevel = DetermineAdminLevel(roles);

        pAccountLib->Enter();
        PCInfoAccount pAccount = pAccountLib->GetPtr(dwAccountId);
        if (pAccount == NULL) {
                pAccountLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"account_not_found\"}");
                return;
        }

        pAccount->m_nAdminLevel = nAdminLevel;
        pAccountLib->Leave();

        response.statusLine = "HTTP/1.1 204 No Content";
        response.body.clear();
}

CLibInfoAccount *CAdminRolesUpdateHandler::GetAccountLibrary() const
{
        if (m_pMgrData == NULL) {
                return NULL;
        }
        return m_pMgrData->GetLibInfoAccount();
}

bool CAdminRolesUpdateHandler::ParseAccountId(const std::string &path, DWORD &outAccountId) const
{
        size_t nQueryPos = path.find('?');
        if (nQueryPos == std::string::npos) {
                return false;
        }
        size_t nPos = nQueryPos + 1;
        while (nPos < path.size()) {
                size_t nAmp = path.find('&', nPos);
                size_t nEnd = (nAmp == std::string::npos) ? path.size() : nAmp;
                size_t nEqual = path.find('=', nPos);
                if ((nEqual != std::string::npos) && (nEqual < nEnd)) {
                        std::string key = path.substr(nPos, nEqual - nPos);
                        if (key == "accountId") {
                                std::string value = path.substr(nEqual + 1, nEnd - (nEqual + 1));
                                if (value.empty()) {
                                        return false;
                                }
                                char *pEnd = NULL;
                                unsigned long id = std::strtoul(value.c_str(), &pEnd, 10);
                                if ((pEnd == NULL) || (*pEnd != '\0')) {
                                        return false;
                                }
                                outAccountId = static_cast<DWORD>(id);
                                return true;
                        }
                }
                if (nAmp == std::string::npos) {
                        break;
                }
                nPos = nAmp + 1;
        }
        return false;
}

bool CAdminRolesUpdateHandler::ParseRoles(const std::string &body, std::vector<std::string> &outRoles) const
{
        return JsonUtils::TryGetStringArray(body, "roles", outRoles);
}

bool CAdminRolesUpdateHandler::ValidateRoles(const std::vector<std::string> &roles) const
{
        for (std::vector<std::string>::const_iterator it = roles.begin(); it != roles.end(); ++it) {
                bool bFound = false;
                for (size_t i = 0; i < g_roleCount; ++i) {
                        if (*it == g_roles[i].pszId) {
                                bFound = true;
                                break;
                        }
                }
                if (!bFound) {
                        return false;
                }
        }
        return true;
}

int CAdminRolesUpdateHandler::DetermineAdminLevel(const std::vector<std::string> &roles) const
{
        for (std::vector<std::string>::const_iterator it = roles.begin(); it != roles.end(); ++it) {
                if ((*it == "SERVER_ADMIN") || (*it == "ACCOUNT_CREATE") || (*it == "ROLE_UPDATE")) {
                        return ADMINLEVEL_ALL;
                }
        }
        return ADMINLEVEL_NONE;
}

