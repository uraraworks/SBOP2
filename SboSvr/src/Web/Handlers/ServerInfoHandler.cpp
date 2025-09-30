#include "stdafx.h"
#include "ServerInfoHandler.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <iomanip>
#include <sstream>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"

CServerInfoHandler::CServerInfoHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CServerInfoHandler::Handle(const HttpRequest &request, HttpResponse &response)
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
        if (!AuthProvider::HasRole(authContext, "SERVER_VIEW")) {
                response.statusLine = "HTTP/1.1 403 Forbidden";
                response.SetJsonBody(AuthProvider::BuildForbiddenBody("SERVER_VIEW"));
                return;
        }

        bool includeMetrics = false;
        std::string locale;
        ParseQueryParameters(request.path, includeMetrics, locale);

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(BuildResponseJson(includeMetrics, locale));
}

std::string CServerInfoHandler::BuildResponseJson(bool includeMetrics, const std::string &locale) const
{
        unsigned int nOnlinePlayers = 0;
        std::string lastPatchVersion;
        if (m_pMgrData != NULL) {
                nOnlinePlayers = m_pMgrData->GetOnline();
                LPCSTR pszVersion = m_pMgrData->GetClientVersion();
                if (pszVersion != NULL) {
                        lastPatchVersion = pszVersion;
                }
        }

        std::string displayName = ResolveDisplayName(locale);

        std::ostringstream oss;
        oss << "{\"updatedAt\":\"" << GetTimestamp() << "\",";
        oss << "\"items\":[{";
        oss << "\"serverId\":\"world-01\",";
        oss << "\"displayName\":\"" << JsonUtils::Escape(displayName) << "\",";
        oss << "\"status\":\"ONLINE\",";
        oss << "\"onlinePlayers\":" << nOnlinePlayers << ',';
        if (!lastPatchVersion.empty()) {
                oss << "\"lastPatchVersion\":\"" << JsonUtils::Escape(lastPatchVersion) << "\",";
        } else {
                oss << "\"lastPatchVersion\":null,";
        }
        oss << BuildMetricsJson(includeMetrics, nOnlinePlayers);
        oss << "}]}";
        return oss.str();
}

std::string CServerInfoHandler::GetTimestamp()
{
        SYSTEMTIME st;
        GetSystemTime(&st);
        char szTimestamp[32];
        std::snprintf(szTimestamp, sizeof(szTimestamp), "%04d-%02d-%02dT%02d:%02d:%02dZ",
                st.wYear,
                st.wMonth,
                st.wDay,
                st.wHour,
                st.wMinute,
                st.wSecond);
        return szTimestamp;
}

std::string CServerInfoHandler::BuildMetricsJson(bool includeMetrics, unsigned int nOnlinePlayers) const
{
        if (!includeMetrics) {
                return "\"metrics\":null";
        }

        double cpuUsage = std::fmin(95.0, 20.0 + static_cast<double>(nOnlinePlayers) * 1.5);
        double memoryUsage = std::fmin(92.0, 30.0 + static_cast<double>(nOnlinePlayers) * 1.3);
        double latency = 18.0 + static_cast<double>(nOnlinePlayers) * 0.4;

        std::ostringstream metrics;
        metrics.setf(std::ios::fixed);
        metrics << std::setprecision(1);
        metrics << "\"metrics\":{";
        metrics << "\"cpuUsage\":" << cpuUsage << ',';
        metrics << "\"memoryUsage\":" << memoryUsage << ',';
        metrics << "\"latencyMs\":" << latency;
        metrics << '}';
        return metrics.str();
}

std::string CServerInfoHandler::ResolveDisplayName(const std::string &locale) const
{
        std::string lowered = locale;
        std::transform(lowered.begin(), lowered.end(), lowered.begin(), [](unsigned char ch) {
                return static_cast<char>(std::tolower(ch));
        });

        if ((lowered == "ja-jp") || (lowered == "ja")) {
                return "Main World JP";
        }
        return "Main World";
}

void CServerInfoHandler::ParseQueryParameters(const std::string &path, bool &outIncludeMetrics, std::string &outLocale) const
{
        outIncludeMetrics = false;
        outLocale.clear();

        size_t nQueryPos = path.find('?');
        if (nQueryPos == std::string::npos) {
                return;
        }

        size_t nPos = nQueryPos + 1;
        while (nPos < path.size()) {
                size_t nAmp = path.find('&', nPos);
                size_t nEnd = (nAmp == std::string::npos) ? path.size() : nAmp;
                size_t nEqual = path.find('=', nPos);
                if ((nEqual != std::string::npos) && (nEqual < nEnd)) {
                        std::string key = path.substr(nPos, nEqual - nPos);
                        std::string value = path.substr(nEqual + 1, nEnd - (nEqual + 1));
                        if (key == "includeMetrics") {
                                std::string lowered;
                                lowered.reserve(value.size());
                                for (size_t i = 0; i < value.size(); ++i) {
                                        lowered.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(value[i]))));
                                }
                                if ((lowered == "true") || (lowered == "1")) {
                                        outIncludeMetrics = true;
                                } else if ((lowered == "false") || (lowered == "0")) {
                                        outIncludeMetrics = false;
                                }
                        } else if (key == "locale") {
                                outLocale = value;
                        }
                }
                if (nAmp == std::string::npos) {
                        break;
                }
                nPos = nAmp + 1;
        }
}

