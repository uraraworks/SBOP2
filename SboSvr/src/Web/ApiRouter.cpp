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

        for (std::vector<RouteEntry>::const_iterator it = m_routes.begin(); it != m_routes.end(); ++it) {
                if (it->path == requestPath) {
                        if (pAllowedMethods != NULL) {
                                if (!pAllowedMethods->empty()) {
                                        pAllowedMethods->append(", ");
                                }
                                pAllowedMethods->append(it->method);
                        }
                        if (it->method == upperMethod) {
                                it->handler->Handle(request, response);
                                return RouteHandled;
                        }
                        bMethodMismatch = true;
                }
        }

        if (bMethodMismatch) {
                return RouteMethodNotAllowed;
        }
        return RouteNotFound;
}

