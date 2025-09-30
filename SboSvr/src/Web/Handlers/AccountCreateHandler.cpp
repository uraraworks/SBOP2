#include "stdafx.h"
#include "AccountCreateHandler.h"

#include <ctime>
#include <cstring>
#include <cstdio>
#include <sstream>

#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "SBOGlobal.h"
#include "GlobalDefine.h"
#include "LibInfo/LibInfoAccount.h"
#include "Info/InfoAccount.h"

CAccountCreateHandler::CAccountCreateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CAccountCreateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        CLibInfoAccount *pAccountLib = GetAccountLibrary();
        if (pAccountLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        std::string loginId;
        std::string password;
        std::string displayName;
        std::vector<std::string> requestedRoles;

        if (!JsonUtils::TryGetString(request.body, "loginId", loginId) ||
                !JsonUtils::TryGetString(request.body, "password", password)) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_payload\"}");
                return;
        }

        if (JsonUtils::FindKey(request.body, "displayName") != std::string::npos) {
                JsonUtils::TryGetString(request.body, "displayName", displayName);
        }

        if (JsonUtils::FindKey(request.body, "roles") != std::string::npos) {
                if (!JsonUtils::TryGetStringArray(request.body, "roles", requestedRoles)) {
                        response.statusLine = "HTTP/1.1 400 Bad Request";
                        response.SetJsonBody("{\"error\":\"invalid_roles\"}");
                        return;
                }
        }

        if (!ValidateInput(loginId, password)) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"validation_failed\"}");
                return;
        }

        pAccountLib->Enter();

        PCInfoAccount pExisting = pAccountLib->GetPtr(loginId.c_str());
        if (pExisting != NULL) {
                pAccountLib->Leave();
                response.statusLine = "HTTP/1.1 409 Conflict";
                response.SetJsonBody("{\"error\":\"account_exists\"}");
                return;
        }

        PCInfoAccount pAccount = (PCInfoAccount)pAccountLib->GetNew();
        if (pAccount == NULL) {
                pAccountLib->Leave();
                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"allocation_failed\"}");
                return;
        }

        TrimViewString(pAccount->m_strAccount, loginId.c_str());
        TrimViewString(pAccount->m_strPassword, password.c_str());
        if (pAccount->m_strAccount.IsEmpty()) {
                SAFE_DELETE(pAccount);
                pAccountLib->Leave();
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_login_id\"}");
                return;
        }

        time_t currentTime = time(NULL);
        if (currentTime < 0) {
                currentTime = 0;
        }
        pAccount->m_dwTimeMakeAccount = static_cast<DWORD>(currentTime);
        pAccount->m_dwTimeLastLogin = 0;
        pAccount->m_dwLoginCount = 0;
        pAccount->m_bDisable = FALSE;
        pAccount->m_nAdminLevel = DetermineAdminLevel(requestedRoles);

        pAccountLib->Add((PCInfoBase)pAccount);
        std::vector<std::string> effectiveRoles = BuildEffectiveRoles(pAccount->m_nAdminLevel);
        std::string responseBody = BuildResponse(pAccount, displayName, effectiveRoles);
        pAccountLib->Leave();

        response.statusLine = "HTTP/1.1 201 Created";
        response.SetJsonBody(responseBody);
}

bool CAccountCreateHandler::ValidateInput(const std::string &loginId, const std::string &password) const
{
        if (loginId.size() < 4 || loginId.size() > 32) {
                return false;
        }
        if (password.size() < 8 || password.size() > 64) {
                return false;
        }
        return true;
}

int CAccountCreateHandler::DetermineAdminLevel(const std::vector<std::string> &requestedRoles) const
{
        for (std::vector<std::string>::const_iterator it = requestedRoles.begin(); it != requestedRoles.end(); ++it) {
                if ((*it == "SERVER_ADMIN") || (*it == "ACCOUNT_CREATE") || (*it == "ROLE_UPDATE")) {
                        return ADMINLEVEL_ALL;
                }
        }
        return ADMINLEVEL_NONE;
}

std::vector<std::string> CAccountCreateHandler::BuildEffectiveRoles(int nAdminLevel) const
{
        std::vector<std::string> roles;
        if (nAdminLevel == ADMINLEVEL_ALL) {
                roles.push_back("SERVER_ADMIN");
                roles.push_back("ACCOUNT_CREATE");
                roles.push_back("ROLE_UPDATE");
        } else {
                roles.push_back("SERVER_VIEW");
        }
        return roles;
}

std::string CAccountCreateHandler::BuildResponse(const CInfoAccount *pAccount, const std::string &displayName, const std::vector<std::string> &roles) const
{
        std::string accountIdStr;
        if (pAccount != NULL) {
                std::ostringstream oss;
                oss << pAccount->m_dwAccountID;
                accountIdStr = oss.str();
        }

        std::string resolvedDisplayName = displayName;
        if (resolvedDisplayName.empty() && pAccount != NULL) {
                resolvedDisplayName = (LPCSTR)pAccount->m_strAccount;
        }

        std::ostringstream oss;
        oss << "{\"accountId\":\"" << JsonUtils::Escape(accountIdStr) << "\",";
        oss << "\"loginId\":\"" << JsonUtils::Escape((LPCSTR)pAccount->m_strAccount) << "\",";
        oss << "\"displayName\":\"" << JsonUtils::Escape(resolvedDisplayName) << "\",";
        oss << "\"email\":null,";
        oss << "\"roles\":[";
        for (size_t i = 0; i < roles.size(); ++i) {
                if (i > 0) {
                        oss << ',';
                }
                oss << "\"" << JsonUtils::Escape(roles[i]) << "\"";
        }
        oss << "],";
        oss << "\"createdAt\":\"" << FormatTimestamp(pAccount->m_dwTimeMakeAccount) << "\"";
        oss << "}";
        return oss.str();
}

std::string CAccountCreateHandler::FormatTimestamp(DWORD dwSeconds) const
{
        time_t rawTime = static_cast<time_t>(dwSeconds);
        struct tm tmUtc;
        memset(&tmUtc, 0, sizeof(tmUtc));
#if defined(_WIN32)
        gmtime_s(&tmUtc, &rawTime);
#else
        tmUtc = *gmtime(&rawTime);
#endif
        char buffer[32];
        std::snprintf(buffer, sizeof(buffer), "%04d-%02d-%02dT%02d:%02d:%02dZ",
                tmUtc.tm_year + 1900,
                tmUtc.tm_mon + 1,
                tmUtc.tm_mday,
                tmUtc.tm_hour,
                tmUtc.tm_min,
                tmUtc.tm_sec);
        return buffer;
}

CLibInfoAccount *CAccountCreateHandler::GetAccountLibrary() const
{
        if (m_pMgrData == NULL) {
                return NULL;
        }
        return m_pMgrData->GetLibInfoAccount();
}

