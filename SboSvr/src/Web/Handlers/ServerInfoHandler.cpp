#include "stdafx.h"
#include "ServerInfoHandler.h"

#include <ctime>
#include <cstdio>
#include <sstream>

#include "Web/JsonUtils.h"
#include "MgrData.h"

CServerInfoHandler::CServerInfoHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CServerInfoHandler::Handle(const HttpRequest & /*request*/, HttpResponse &response)
{
        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(BuildResponseJson());
}

std::string CServerInfoHandler::BuildResponseJson() const
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

        std::ostringstream oss;
        oss << "{\"updatedAt\":\"" << GetTimestamp() << "\",";
        oss << "\"items\":[{";
        oss << "\"serverId\":\"world-01\",";
        oss << "\"displayName\":\"メインワールド\",";
        oss << "\"status\":\"ONLINE\",";
        oss << "\"onlinePlayers\":" << nOnlinePlayers << ',';
        if (!lastPatchVersion.empty()) {
                oss << "\"lastPatchVersion\":\"" << JsonUtils::Escape(lastPatchVersion) << "\",";
        } else {
                oss << "\"lastPatchVersion\":null,";
        }
        oss << "\"metrics\":null";
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

