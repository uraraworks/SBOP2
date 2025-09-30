#pragma once

#include "Web/ApiHandler.h"

#include <vector>

class CMgrData;
class CLibInfoAccount;
class CInfoAccount;

struct RoleDefinition
{
        const char *pszId;
        const char *pszName;
        const char *pszDescription;
        const char *pszFeatureFlags[3];
        size_t      nFeatureFlagCount;
};

namespace AdminRoleCatalog
{
const RoleDefinition *GetRoles(size_t &outCount);
bool Contains(const std::string &roleId);
int DetermineAdminLevel(const std::vector<std::string> &roles);
std::vector<std::string> ResolveRoles(int nAdminLevel);
bool ValidateExclusiveConstraints(CLibInfoAccount *pAccountLib, DWORD dwExcludeAccountId, int nAdminLevel, std::string &outConflictRole);
}

class CAdminRolesListHandler : public IApiHandler
{
public:
        explicit CAdminRolesListHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        std::string BuildResponse() const;
        CMgrData *m_pMgrData;
};

class CAdminRolesUpdateHandler : public IApiHandler
{
public:
        explicit CAdminRolesUpdateHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CLibInfoAccount *GetAccountLibrary() const;
        bool ParseAccountId(const std::string &path, DWORD &outAccountId) const;
        bool ParseRoles(const std::string &body, std::vector<std::string> &outRoles) const;
        bool ParseComment(const std::string &body, std::string &outComment) const;
        int DetermineAdminLevel(const std::vector<std::string> &roles) const;
        bool ValidateRoles(const std::vector<std::string> &roles) const;
        bool ValidateExclusiveConstraints(CLibInfoAccount *pAccountLib, DWORD dwAccountId, int nAdminLevel, std::string &outConflictRole) const;
        void EmitAuditTrace(DWORD dwAccountId, const std::vector<std::string> &roles, const std::string &comment) const;
        CMgrData *m_pMgrData;
};

