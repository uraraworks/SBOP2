#pragma once

#include "ApiRouter.h"

class CMgrData;

class CHttpServer
{
public:
        CHttpServer();
        ~CHttpServer();

        bool    Start(unsigned short wPort);
        void    Stop();
        void    SetMgrData(CMgrData *pMgrData);

private:
        static unsigned __stdcall ThreadProc(void *lpParam);

        void    Run();
        bool    InitializeWinsock();
        bool    CreateListener();
        void    CloseListener();
        void    ProcessLoop();
        void    HandleAccept();
        void    HandleClient(SOCKET hClient);
        bool    ParseHttpRequest(const std::string &rawRequest, HttpRequest &outRequest);
        static std::string Trim(const std::string &text);
        void    SendResponse(SOCKET hClient, const HttpResponse &response);
        bool    SendAll(SOCKET hSocket, const char *pData, size_t nLength);
        void    RegisterDefaultHandlers();

        SOCKET          m_hListen;
        HANDLE          m_hThread;
        HANDLE          m_hStopEvent;
        HANDLE          m_hStartedEvent;
        unsigned short  m_wPort;
        WSADATA         m_wsaData;
        bool            m_bInitSucceeded;
        CApiRouter      m_router;
        bool            m_bHandlersRegistered;
        CMgrData       *m_pMgrData;
};
