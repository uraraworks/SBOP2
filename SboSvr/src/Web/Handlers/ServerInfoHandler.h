#pragma once

#include "Web/ApiHandler.h"
#include "Web/ProcessMetrics.h"
#include <mutex>

class CMgrData;

class CServerInfoHandler : public IApiHandler
{
public:
        explicit CServerInfoHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        std::string BuildResponseJson(bool includeMetrics, const std::string &locale) const;
        std::string BuildMetricsJson(bool includeMetrics) const;
        std::string ResolveDisplayName(const std::string &locale) const;
        void ParseQueryParameters(const std::string &path, bool &outIncludeMetrics, std::string &outLocale) const;
        static std::string GetTimestamp();
        CMgrData *m_pMgrData;

        // CPU使用率は「前回問い合わせ時からのCPU時間の差分」で出すため、
        // 直近のサンプルを持ち回す(複数スレッドから呼ばれるため排他が要る)。
        mutable std::mutex m_metricsMutex;
        mutable bool m_bHasPrevCpuSample;
        mutable ProcessMetrics::Sample m_prevCpuSample;
};
