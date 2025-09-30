#include "stdafx.h"
#include "AccountCreateHandler.h"

#include <algorithm>
#include <cctype>
#include <ctime>
#include <cstring>
#include <cstdio>
#include <sstream>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "AdminRolesHandler.h"
#include "MgrData.h"
#include "SBOGlobal.h"
#include "GlobalDefine.h"
#include "LibInfo/LibInfoAccount.h"
#include "Info/InfoAccount.h"

CAccountCreateHandler::CAccountCreateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
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
}

void CAccountCreateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
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
        if (!AuthProvider::HasRole(authContext, "ACCOUNT_CREATE")) {
                response.statusLine = "HTTP/1.1 403 Forbidden";
                response.SetJsonBody(AuthProvider::BuildForbiddenBody("ACCOUNT_CREATE"));
                return;
        }

        CLibInfoAccount *pAccountLib = GetAccountLibrary();
        if (pAccountLib == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        std::string loginId;
        std::string password;
        std::string displayName;
        std::string email;
        bool bEmailProvided = false;
        std::vector<std::string> requestedRoles;

        bool bHasLogin = JsonUtils::TryGetString(request.body, "loginId", loginId);
        bool bHasPassword = JsonUtils::TryGetString(request.body, "password", password);
        bool bHasDisplayName = JsonUtils::TryGetString(request.body, "displayName", displayName);
        bool bHasRoles = JsonUtils::TryGetStringArray(request.body, "roles", requestedRoles);

        if (JsonUtils::FindKey(request.body, "email") != std::string::npos) {
                if (JsonUtils::IsNull(request.body, "email")) {
                        bEmailProvided = true;
                        email.clear();
                } else if (JsonUtils::TryGetString(request.body, "email", email)) {
                        bEmailProvided = true;
                } else {
                        std::vector<ValidationIssue> errorDetails;
                        ValidationIssue issue;
                        issue.field = "email";
                        issue.message = "email must be a string or null.";
                        errorDetails.push_back(issue);
                        response.statusLine = "HTTP/1.1 400 Bad Request";
                        response.SetJsonBody(BuildValidationErrorResponse("invalid_payload", errorDetails));
                        return;
                }
        }

        if (!bHasLogin || !bHasPassword || !bHasDisplayName || !bHasRoles) {
                std::vector<ValidationIssue> errorDetails;
                if (!bHasLogin) {
                        ValidationIssue issue;
                        issue.field = "loginId";
                        issue.message = "loginId is required.";
                        errorDetails.push_back(issue);
                }
                if (!bHasPassword) {
                        ValidationIssue issue;
                        issue.field = "password";
                        issue.message = "password is required.";
                        errorDetails.push_back(issue);
                }
                if (!bHasDisplayName) {
                        ValidationIssue issue;
                        issue.field = "displayName";
                        issue.message = "displayName is required.";
                        errorDetails.push_back(issue);
                }
                if (!bHasRoles) {
                        ValidationIssue issue;
                        issue.field = "roles";
                        issue.message = "roles must be an array of strings.";
                        errorDetails.push_back(issue);
                }
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody(BuildValidationErrorResponse("invalid_payload", errorDetails));
                return;
        }

        loginId = TrimCopy(loginId);
        password = TrimCopy(password);
        displayName = TrimCopy(displayName);
        if (bEmailProvided) {
                email = TrimCopy(email);
        }

        std::vector<ValidationIssue> errors;
        std::vector<ValidationIssue> passwordPolicyErrors;
        std::vector<std::string> normalizedRoles;
        if (NormalizeRoles(requestedRoles, normalizedRoles, errors)) {
                requestedRoles.swap(normalizedRoles);
        }

        ValidateInput(loginId, password, displayName, requestedRoles, email, bEmailProvided, errors, passwordPolicyErrors);

        if (!errors.empty()) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody(BuildValidationErrorResponse("validation_failed", errors));
                return;
        }

        if (!passwordPolicyErrors.empty()) {
                response.statusLine = "HTTP/1.1 422 Unprocessable Entity";
                response.SetJsonBody(BuildValidationErrorResponse("password_policy_violation", passwordPolicyErrors));
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

        int nAdminLevel = DetermineAdminLevel(requestedRoles);
        std::string conflictRole;
        if (!AdminRoleCatalog::ValidateExclusiveConstraints(pAccountLib, 0, nAdminLevel, conflictRole)) {
                pAccountLib->Leave();
                response.statusLine = "HTTP/1.1 409 Conflict";
                std::ostringstream oss;
                oss << "{\"error\":\"role_conflict\",\"conflictRole\":\"" << JsonUtils::Escape(conflictRole) << "\"}";
                response.SetJsonBody(oss.str());
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
        pAccount->m_nAdminLevel = nAdminLevel;

        pAccountLib->Add((PCInfoBase)pAccount);
        std::vector<std::string> effectiveRoles = AdminRoleCatalog::ResolveRoles(pAccount->m_nAdminLevel);
        std::string responseBody = BuildResponse(pAccount, displayName, effectiveRoles, email, bEmailProvided);
        pAccountLib->Leave();

        response.statusLine = "HTTP/1.1 201 Created";
        response.SetJsonBody(responseBody);
}

bool CAccountCreateHandler::ValidateInput(const std::string &loginId,
        const std::string &password,
        const std::string &displayName,
        const std::vector<std::string> &roles,
        const std::string &email,
        bool bEmailProvided,
        std::vector<ValidationIssue> &outGeneralErrors,
        std::vector<ValidationIssue> &outPasswordPolicyErrors) const
{
        ValidationIssue issue;
        if (!ValidateLoginId(loginId, issue)) {
                outGeneralErrors.push_back(issue);
        }

        ValidationIssue passwordPolicy;
        if (!ValidatePassword(password, passwordPolicy)) {
                outPasswordPolicyErrors.push_back(passwordPolicy);
        }

        ValidationIssue displayIssue;
        if (!ValidateDisplayName(displayName, displayIssue)) {
                outGeneralErrors.push_back(displayIssue);
        }

        if (roles.empty()) {
                ValidationIssue roleIssue;
                roleIssue.field = "roles";
                roleIssue.message = "roles must contain at least one entry.";
                outGeneralErrors.push_back(roleIssue);
        }

        if (bEmailProvided) {
                ValidationIssue emailIssue;
                if (!ValidateEmail(email, emailIssue)) {
                        outGeneralErrors.push_back(emailIssue);
                }
        }

        return outGeneralErrors.empty() && outPasswordPolicyErrors.empty();
}

bool CAccountCreateHandler::ValidateLoginId(const std::string &loginId, ValidationIssue &outIssue) const
{
        if (loginId.empty()) {
                outIssue.field = "loginId";
                outIssue.message = "loginId must not be empty.";
                return false;
        }
        if (loginId.size() < 4 || loginId.size() > 32) {
                outIssue.field = "loginId";
                outIssue.message = "loginId length must be between 4 and 32 characters.";
                return false;
        }
        for (size_t i = 0; i < loginId.size(); ++i) {
                unsigned char ch = static_cast<unsigned char>(loginId[i]);
                if (std::isalnum(ch)) {
                        continue;
                }
                if ((ch == '-') || (ch == '_') || (ch == '.') || (ch == '@')) {
                        continue;
                }
                outIssue.field = "loginId";
                outIssue.message = "loginId contains invalid characters.";
                return false;
        }
        return true;
}

bool CAccountCreateHandler::ValidatePassword(const std::string &password, ValidationIssue &outPolicyIssue) const
{
        if (password.size() < 12 || password.size() > 64) {
                outPolicyIssue.field = "password";
                outPolicyIssue.message = "password length must be between 12 and 64 characters.";
                return false;
        }
        return true;
}

bool CAccountCreateHandler::ValidateDisplayName(const std::string &displayName, ValidationIssue &outIssue) const
{
        if (displayName.empty()) {
                outIssue.field = "displayName";
                outIssue.message = "displayName must not be empty.";
                return false;
        }
        if (displayName.size() > 64) {
                outIssue.field = "displayName";
                outIssue.message = "displayName must be 64 characters or fewer.";
                return false;
        }
        return true;
}

bool CAccountCreateHandler::ValidateEmail(const std::string &email, ValidationIssue &outIssue) const
{
        if (email.empty()) {
                return true;
        }
        if (email.size() > 128) {
                outIssue.field = "email";
                outIssue.message = "email must be 128 characters or fewer.";
                return false;
        }
        if (email.find(' ') != std::string::npos) {
                outIssue.field = "email";
                outIssue.message = "email must not contain spaces.";
                return false;
        }
        size_t atPos = email.find('@');
        if ((atPos == std::string::npos) || (atPos == 0) || (atPos == email.size() - 1)) {
                outIssue.field = "email";
                outIssue.message = "email must contain '@' separating local and domain parts.";
                return false;
        }
        size_t dotPos = email.find('.', atPos + 1);
        if ((dotPos == std::string::npos) || (dotPos == atPos + 1) || (dotPos == email.size() - 1)) {
                outIssue.field = "email";
                outIssue.message = "email domain must contain a dot.";
                return false;
        }
        return true;
}

bool CAccountCreateHandler::NormalizeRoles(const std::vector<std::string> &inputRoles, std::vector<std::string> &outRoles, std::vector<ValidationIssue> &outErrors) const
{
        outRoles.clear();
        for (std::vector<std::string>::const_iterator it = inputRoles.begin(); it != inputRoles.end(); ++it) {
                if (!AdminRoleCatalog::Contains(*it)) {
                        ValidationIssue issue;
                        issue.field = "roles";
                        issue.message = "Unknown role: " + *it;
                        outErrors.push_back(issue);
                        continue;
                }
                if (std::find(outRoles.begin(), outRoles.end(), *it) == outRoles.end()) {
                        outRoles.push_back(*it);
                }
        }
        return outErrors.empty();
}

std::string CAccountCreateHandler::BuildValidationErrorResponse(const std::string &errorCode, const std::vector<ValidationIssue> &issues) const
{
        std::ostringstream oss;
        oss << "{\"error\":\"" << JsonUtils::Escape(errorCode) << "\",\"details\":[";
        for (size_t i = 0; i < issues.size(); ++i) {
                if (i > 0) {
                        oss << ',';
                }
                oss << "{\"field\":\"" << JsonUtils::Escape(issues[i].field) << "\",\"message\":\"" << JsonUtils::Escape(issues[i].message) << "\"}";
        }
        oss << "]}";
        return oss.str();
}

int CAccountCreateHandler::DetermineAdminLevel(const std::vector<std::string> &requestedRoles) const
{
        return AdminRoleCatalog::DetermineAdminLevel(requestedRoles);
}

std::string CAccountCreateHandler::BuildResponse(const CInfoAccount *pAccount, const std::string &displayName, const std::vector<std::string> &roles, const std::string &email, bool bEmailProvided) const
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
        if (bEmailProvided) {
                if (email.empty()) {
                        oss << "\"email\":null,";
                } else {
                        oss << "\"email\":\"" << JsonUtils::Escape(email) << "\",";
                }
        } else {
                oss << "\"email\":null,";
        }
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

