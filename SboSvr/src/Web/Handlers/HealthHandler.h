#pragma once

#include "Web/ApiHandler.h"

class CMgrData;

class CHealthHandler : public IApiHandler
{
public:
        explicit CHealthHandler(CMgrData *pMgrData);
        virtual void Handle(const HttpRequest &request, HttpResponse &response);

private:
        CMgrData *m_pMgrData;
};
