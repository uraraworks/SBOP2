#pragma once

#include "Web/ApiHandler.h"

class CMgrData;

class CServerInfoHandler : public IApiHandler
{
public:
        explicit CServerInfoHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        std::string BuildResponseJson() const;
        static std::string GetTimestamp();
        CMgrData *m_pMgrData;
};

