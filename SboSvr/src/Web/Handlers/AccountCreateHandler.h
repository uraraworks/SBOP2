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
        bool ValidateInput(const std::string &loginId, const std::string &password) const;
        int DetermineAdminLevel(const std::vector<std::string> &requestedRoles) const;
        std::string BuildResponse(const CInfoAccount *pAccount, const std::string &displayName, const std::vector<std::string> &roles) const;
        std::vector<std::string> BuildEffectiveRoles(int nAdminLevel) const;
        std::string FormatTimestamp(DWORD dwSeconds) const;
        CLibInfoAccount *GetAccountLibrary() const;

        CMgrData *m_pMgrData;
};

