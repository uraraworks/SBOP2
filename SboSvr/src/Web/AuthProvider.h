#pragma once

#include "Web/HttpTypes.h"

#include <string>
#include <vector>

class CMgrData;

namespace AuthProvider
{
struct AuthContext
{
        AuthContext();

        bool                    authenticated;
        std::string             sessionId;
        std::string             loginId;
        std::string             accountId;
        int                     adminLevel;
        std::vector<std::string> roles;
};

enum AuthStatus
{
        AuthStatusOk,
        AuthStatusMissingSession,
        AuthStatusBackendUnavailable,
        AuthStatusAccountNotFound
};

AuthStatus Authenticate(const HttpRequest &request, CMgrData *pMgrData, AuthContext &outContext);
bool HasRole(const AuthContext &context, const char *pszRoleId);
std::string BuildForbiddenBody(const char *pszRequiredRole);
}

