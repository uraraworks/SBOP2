#include "StdAfx.h"
#include "HealthHandler.h"
#include "MgrData.h"

CHealthHandler::CHealthHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CHealthHandler::Handle(const HttpRequest & /*request*/, HttpResponse &response)
{
        const unsigned int nOnlinePlayers = m_pMgrData != NULL ? m_pMgrData->GetOnline() : 0;
        CStringA body;
        body.Format("{\"status\":\"ok\",\"onlinePlayers\":%u}", nOnlinePlayers);

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetHeader("Access-Control-Allow-Origin", "*");
        response.SetHeader("Cache-Control", "no-store");
        response.SetJsonBody((LPCSTR)body);
}
