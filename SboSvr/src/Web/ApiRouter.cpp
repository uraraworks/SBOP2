#include "stdafx.h"
#include "ApiRouter.h"

#include <algorithm>
#include <cctype>

namespace
{
static std::string ToUpperCopy(const std::string &text)
{
        std::string result = text;
        std::transform(result.begin(), result.end(), result.begin(), [](unsigned char ch) {
                return static_cast<char>(std::toupper(ch));
        });
        return result;
}
}

CApiRouter::CApiRouter()
{
}

void CApiRouter::Register(const std::string &method, const std::string &path, std::unique_ptr<IApiHandler> handler)
{
        if (!handler) {
                return;
        }

        RouteEntry entry;
        entry.method = ToUpperCopy(method);
        entry.path = path;
        entry.prefix = false;
        entry.handler.swap(handler);
        m_routes.push_back(std::move(entry));
}

void CApiRouter::RegisterPrefix(const std::string &method, const std::string &pathPrefix, std::unique_ptr<IApiHandler> handler)
{
        if (!handler) {
                return;
        }

        RouteEntry entry;
        entry.method = ToUpperCopy(method);
        entry.path = pathPrefix;
        entry.prefix = true;
        entry.handler.swap(handler);
        m_routes.push_back(std::move(entry));
}

namespace
{
static std::string NormalizePath(const std::string &path)
{
        size_t nQueryPos = path.find('?');
        if (nQueryPos != std::string::npos) {
                return path.substr(0, nQueryPos);
        }
        return path;
}
}

CApiRouter::RouteResult CApiRouter::Dispatch(const HttpRequest &request, HttpResponse &response, std::string *pAllowedMethods) const
{
        bool bMethodMismatch = false;
        std::string upperMethod = ToUpperCopy(request.method);
        std::string requestPath = NormalizePath(request.path);

        if (pAllowedMethods != NULL) {
                pAllowedMethods->clear();
        }

        const RouteEntry *pBestPrefixMatch = NULL;
        size_t nBestPrefixLength = 0;

        for (std::vector<RouteEntry>::const_iterator it = m_routes.begin(); it != m_routes.end(); ++it) {
                bool bPathMatch = false;
                if (it->prefix) {
                        if (requestPath.size() >= it->path.size()) {
                                if (requestPath.compare(0, it->path.size(), it->path) == 0) {
                                        bPathMatch = true;
                                }
                        }
                } else if (it->path == requestPath) {
                        bPathMatch = true;
                }

                if (bPathMatch) {
                        bool bHandled = false;
                        if (pAllowedMethods != NULL) {
                                if (!pAllowedMethods->empty()) {
                                        pAllowedMethods->append(", ");
                                }
                                pAllowedMethods->append(it->method);
                        }
                        if (it->method == upperMethod) {
                                if (!it->prefix) {
                                        it->handler->Handle(request, response);
                                        return RouteHandled;
                                }
                                if ((pBestPrefixMatch == NULL) || (it->path.size() > nBestPrefixLength)) {
                                        pBestPrefixMatch = &(*it);
                                        nBestPrefixLength = it->path.size();
                                }
                                bHandled = true;
                        }
                        if (!bHandled) {
                                bMethodMismatch = true;
                        }
                }
        }

        if (pBestPrefixMatch != NULL) {
                pBestPrefixMatch->handler->Handle(request, response);
                return RouteHandled;
        }

        if (bMethodMismatch) {
                return RouteMethodNotAllowed;
        }
        return RouteNotFound;
}

