#pragma once

#include "Web/ApiHandler.h"

class CMgrData;

class CServerInfoHandler : public IApiHandler
{
public:
        explicit CServerInfoHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        std::string BuildResponseJson(bool includeMetrics, const std::string &locale) const;
        std::string BuildMetricsJson(bool includeMetrics, unsigned int nOnlinePlayers) const;
        std::string ResolveDisplayName(const std::string &locale) const;
        void ParseQueryParameters(const std::string &path, bool &outIncludeMetrics, std::string &outLocale) const;
        static std::string GetTimestamp();
        CMgrData *m_pMgrData;
};

