#include "StdAfx.h"
#include "ServerInfoHandler.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <iomanip>
#include <sstream>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "../../Platform/SvrPlatform.h"

namespace
{
        long long NowMonotonicMs()
        {
                return std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::steady_clock::now().time_since_epoch()).count();
        }
}

CServerInfoHandler::CServerInfoHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
        , m_bHasPrevCpuSample(false)
{
        m_prevCpuSample.nCpuTimeMs = 0;
        m_prevCpuSample.nWallTimeMs = 0;
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
                nOnlinePlayers = m_pMgrData->CountOnlineAccounts();
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
        oss << BuildMetricsJson(includeMetrics);
        oss << "}]}";
        return oss.str();
}

std::string CServerInfoHandler::GetTimestamp()
{
        SboPlatform::LOCALTIME st;
        SboPlatform::GetSystemTime(&st);
        char szTimestamp[32];
        std::snprintf(szTimestamp, sizeof(szTimestamp), "%04d-%02d-%02dT%02d:%02d:%02dZ",
                st.nYear,
                st.nMonth,
                st.nDay,
                st.nHour,
                st.nMinute,
                st.nSecond);
        return szTimestamp;
}

std::string CServerInfoHandler::BuildMetricsJson(bool includeMetrics) const
{
        if (!includeMetrics) {
                return "\"metrics\":null";
        }

        // cpuUsage: 前回問い合わせ時からのプロセスCPU時間の差分から算出する実測値。
        //           初回問い合わせ時など算出できない場合は null。
        // memoryUsage/memoryMB: 現在のプロセスのメモリ使用量(実測)。取れなければ null。
        // latencyMs: 実測する手段が既存コードに無いため、推測値を入れず常に null。
        bool bHasCpuUsage = false;
        double dCpuUsage = 0.0;
        bool bHasMemoryUsage = false;
        double dMemoryUsage = 0.0;
        bool bHasMemoryMB = false;
        double dMemoryMB = 0.0;

        SboPlatform::PROCESS_METRICS processMetrics;
        bool bHasProcessMetrics = SboPlatform::GetProcessMetrics(&processMetrics);

        if (bHasProcessMetrics) {
                ProcessMetrics::Sample curSample;
                curSample.nCpuTimeMs = processMetrics.nCpuTimeMs;
                curSample.nWallTimeMs = NowMonotonicMs();

                unsigned int nCpuCount = 0;
                bool bHasCpuCount = SboPlatform::GetLogicalCpuCount(&nCpuCount);

                {
                        std::lock_guard<std::mutex> lock(m_metricsMutex);
                        if (bHasCpuCount && m_bHasPrevCpuSample) {
                                bHasCpuUsage = ProcessMetrics::ComputeCpuUsagePercent(
                                        m_prevCpuSample, curSample, nCpuCount, &dCpuUsage);
                        }
                        m_prevCpuSample = curSample;
                        m_bHasPrevCpuSample = true;
                }

                dMemoryMB = static_cast<double>(processMetrics.nMemoryBytes) / (1024.0 * 1024.0);
                bHasMemoryMB = true;

                unsigned long long nTotalPhysBytes = 0;
                if (SboPlatform::GetSystemMemoryTotalBytes(&nTotalPhysBytes) && (nTotalPhysBytes > 0)) {
                        dMemoryUsage = static_cast<double>(processMetrics.nMemoryBytes) /
                                static_cast<double>(nTotalPhysBytes) * 100.0;
                        bHasMemoryUsage = true;
                }
        }

        std::ostringstream metrics;
        metrics.setf(std::ios::fixed);
        metrics << std::setprecision(1);
        metrics << "\"metrics\":{";
        metrics << "\"cpuUsage\":";
        if (bHasCpuUsage) { metrics << dCpuUsage; } else { metrics << "null"; }
        metrics << ',';
        metrics << "\"memoryUsage\":";
        if (bHasMemoryUsage) { metrics << dMemoryUsage; } else { metrics << "null"; }
        metrics << ',';
        metrics << "\"memoryMB\":";
        if (bHasMemoryMB) { metrics << dMemoryMB; } else { metrics << "null"; }
        metrics << ',';
        metrics << "\"latencyMs\":null";
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
