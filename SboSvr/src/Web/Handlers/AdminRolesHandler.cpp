#include "stdafx.h"
#include "AdminRolesHandler.h"

#include <cstdlib>
#include <sstream>
#include <cstdio>
#if defined(_WIN32)
#include <windows.h>
#endif

#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "GlobalDefine.h"
#include "LibInfo/LibInfoAccount.h"
#include "Info/InfoAccount.h"

namespace
{
const RoleDefinition g_roles[] = {
        {"SERVER_VIEW", "Server View", "Allows viewing the server dashboard.", {"SERVER_DASHBOARD"}, 1},
        {"ACCOUNT_CREATE", "Account Create", "Allows creating new administrator accounts.", {"ACCOUNT_CREATE"}, 1},
        {"ROLE_UPDATE", "Role Update", "Allows updating administrator role assignments.", {"ROLE_MANAGEMENT"}, 1},
        {"SERVER_ADMIN", "Server Admin", "Allows performing privileged server control operations.", {"SERVER_CONTROL"}, 1},
};
const size_t g_roleCount = sizeof(g_roles) / sizeof(g_roles[0]);
}

namespace AdminRoleCatalog
{
const RoleDefinition *GetRoles(size_t &outCount)
{
        outCount = g_roleCount;
        return g_roles;
}

bool Contains(const std::string &roleId)
{
        for (size_t i = 0; i < g_roleCount; ++i) {
                if (roleId == g_roles[i].pszId) {
                        return true;
                }
        }
        return false;
}

int DetermineAdminLevel(const std::vector<std::string> &roles)
{
        for (std::vector<std::string>::const_iterator it = roles.begin(); it != roles.end(); ++it) {
                if ((*it == "SERVER_ADMIN") || (*it == "ACCOUNT_CREATE") || (*it == "ROLE_UPDATE")) {
                        return ADMINLEVEL_ALL;
                }
        }
        return ADMINLEVEL_NONE;
}

std::vector<std::string> ResolveRoles(int nAdminLevel)
{
        std::vector<std::string> roles;
        if (nAdminLevel == ADMINLEVEL_ALL) {
                roles.push_back("SERVER_ADMIN");
                roles.push_back("ACCOUNT_CREATE");
                roles.push_back("ROLE_UPDATE");
        }
        roles.push_back("SERVER_VIEW");
        return roles;
}

bool ValidateExclusiveConstraints(CLibInfoAccount *pAccountLib, DWORD dwExcludeAccountId, int nAdminLevel, std::string &outConflictRole)
{
        outConflictRole.clear();
        if ((pAccountLib == NULL) || (nAdminLevel != ADMINLEVEL_ALL)) {
                return true;
        }

        int nCount = pAccountLib->GetCount();
        for (int i = 0; i < nCount; ++i) {
                PCInfoAccount pOther = static_cast<PCInfoAccount>(pAccountLib->GetPtr(i));
                if ((pOther == NULL) || (pOther->m_dwAccountID == dwExcludeAccountId)) {
                        continue;
                }
                if (pOther->m_nAdminLevel == ADMINLEVEL_ALL) {
                        outConflictRole = "SERVER_ADMIN";
                        return false;
                }
        }
        return true;
}
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
        size_t nCount = 0;
        const RoleDefinition *pRoles = AdminRoleCatalog::GetRoles(nCount);
        for (size_t i = 0; i < nCount; ++i) {
                if (i > 0) {
                        oss << ',';
                }
                oss << '{';
                oss << "\"id\":\"" << JsonUtils::Escape(pRoles[i].pszId) << "\",";
                oss << "\"name\":\"" << JsonUtils::Escape(pRoles[i].pszName) << "\",";
                oss << "\"description\":\"" << JsonUtils::Escape(pRoles[i].pszDescription) << "\",";
                oss << "\"featureFlags\":[";
                for (size_t j = 0; j < pRoles[i].nFeatureFlagCount; ++j) {
                        if (j > 0) {
                                oss << ',';
                        }
                        oss << "\"" << JsonUtils::Escape(pRoles[i].pszFeatureFlags[j]) << "\"";
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

        std::string comment;
        if (!ParseComment(request.body, comment)) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_comment\"}");
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

        std::string conflictRole;
        if (!ValidateExclusiveConstraints(pAccountLib, dwAccountId, nAdminLevel, conflictRole)) {
                pAccountLib->Leave();
                response.statusLine = "HTTP/1.1 409 Conflict";
                if (!conflictRole.empty()) {
                        std::ostringstream oss;
                        oss << "{\"error\":\"role_conflict\",\"conflictRole\":\"" << JsonUtils::Escape(conflictRole) << "\"}";
                        response.SetJsonBody(oss.str());
                } else {
                        response.SetJsonBody("{\"error\":\"role_conflict\"}");
                }
                return;
        }

        pAccount->m_nAdminLevel = nAdminLevel;
        pAccountLib->Leave();

        response.statusLine = "HTTP/1.1 204 No Content";
        response.body.clear();

        EmitAuditTrace(dwAccountId, roles, comment);
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

bool CAdminRolesUpdateHandler::ParseComment(const std::string &body, std::string &outComment) const
{
        outComment.clear();
        if (JsonUtils::FindKey(body, "comment") == std::string::npos) {
                return true;
        }
        if (!JsonUtils::TryGetString(body, "comment", outComment)) {
                outComment.clear();
                return false;
        }
        if (outComment.size() > 256) {
                outComment.clear();
                return false;
        }
        return true;
}

bool CAdminRolesUpdateHandler::ValidateRoles(const std::vector<std::string> &roles) const
{
        for (std::vector<std::string>::const_iterator it = roles.begin(); it != roles.end(); ++it) {
                if (!AdminRoleCatalog::Contains(*it)) {
                        return false;
                }
        }
        return true;
}

int CAdminRolesUpdateHandler::DetermineAdminLevel(const std::vector<std::string> &roles) const
{
        return AdminRoleCatalog::DetermineAdminLevel(roles);
}

bool CAdminRolesUpdateHandler::ValidateExclusiveConstraints(CLibInfoAccount *pAccountLib, DWORD dwAccountId, int nAdminLevel, std::string &outConflictRole) const
{
        return AdminRoleCatalog::ValidateExclusiveConstraints(pAccountLib, dwAccountId, nAdminLevel, outConflictRole);
}

void CAdminRolesUpdateHandler::EmitAuditTrace(DWORD dwAccountId, const std::vector<std::string> &roles, const std::string &comment) const
{
        if (comment.empty()) {
                return;
        }

        std::ostringstream oss;
        oss << "[AdminRoles] accountId=" << dwAccountId << " roles=";
        for (size_t i = 0; i < roles.size(); ++i) {
                if (i > 0) {
                        oss << '|';
                }
                oss << roles[i];
        }
        oss << " comment=" << comment;
#if defined(_WIN32)
        OutputDebugStringA(oss.str().c_str());
#else
        std::fprintf(stderr, "%s\n", oss.str().c_str());
#endif
}

