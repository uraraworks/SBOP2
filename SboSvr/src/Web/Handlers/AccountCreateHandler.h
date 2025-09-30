#pragma once

#include "Web/ApiHandler.h"

#include <vector>

class CMgrData;
class CLibInfoAccount;
class CInfoAccount;

class CAccountCreateHandler : public IApiHandler
{
public:
        explicit CAccountCreateHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        struct ValidationIssue
        {
                std::string field;
                std::string message;
        };

        bool ValidateInput(const std::string &loginId,
                const std::string &password,
                const std::string &displayName,
                const std::vector<std::string> &roles,
                const std::string &email,
                bool bEmailProvided,
                std::vector<ValidationIssue> &outGeneralErrors,
                std::vector<ValidationIssue> &outPasswordPolicyErrors) const;
        bool ValidateLoginId(const std::string &loginId, ValidationIssue &outIssue) const;
        bool ValidatePassword(const std::string &password, ValidationIssue &outPolicyIssue) const;
        bool ValidateDisplayName(const std::string &displayName, ValidationIssue &outIssue) const;
        bool ValidateEmail(const std::string &email, ValidationIssue &outIssue) const;
        bool NormalizeRoles(const std::vector<std::string> &inputRoles, std::vector<std::string> &outRoles, std::vector<ValidationIssue> &outErrors) const;
        std::string BuildValidationErrorResponse(const std::string &errorCode, const std::vector<ValidationIssue> &issues) const;
        int DetermineAdminLevel(const std::vector<std::string> &requestedRoles) const;
        std::string BuildResponse(const CInfoAccount *pAccount, const std::string &displayName, const std::vector<std::string> &roles, const std::string &email, bool bEmailProvided) const;
        std::string FormatTimestamp(DWORD dwSeconds) const;
        CLibInfoAccount *GetAccountLibrary() const;

        CMgrData *m_pMgrData;
};

