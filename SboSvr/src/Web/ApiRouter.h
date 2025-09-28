#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ApiHandler.h"

class CApiRouter
{
public:
        enum RouteResult
        {
                RouteHandled,
                RouteMethodNotAllowed,
                RouteNotFound
        };

        CApiRouter();

        void Register(const std::string &method, const std::string &path, std::unique_ptr<IApiHandler> handler);
        RouteResult Dispatch(const HttpRequest &request, HttpResponse &response, std::string *pAllowedMethods) const;

private:
        struct RouteEntry
        {
                std::string                     method;
                std::string                     path;
                std::unique_ptr<IApiHandler>    handler;
        };

        std::vector<RouteEntry> m_routes;
};

