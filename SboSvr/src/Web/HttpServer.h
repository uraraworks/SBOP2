#pragma once

#include <string>

#include "ApiRouter.h"

class CMgrData;
class CMainFrame;

class CHttpServer
{
public:
        CHttpServer();
        ~CHttpServer();

        bool    Start(unsigned short wPort);
        void    Stop();
        void    SetMgrData(CMgrData *pMgrData);
        void    SetMainFrame(CMainFrame *pMainFrame);

private:
        static unsigned __stdcall ThreadProc(void *lpParam);

        void    Run();
        bool    InitializeWinsock();
        bool    CreateListener();
        void    CloseListener();
        void    ProcessLoop();
        void    HandleAccept();
        void    HandleClient(SOCKET hClient, bool &outTransferred);
        bool    ParseHttpRequest(const std::string &rawRequest, HttpRequest &outRequest);
        static std::string Trim(const std::string &text);
        void    SendResponse(SOCKET hClient, const HttpResponse &response);
        bool    SendAll(SOCKET hSocket, const char *pData, size_t nLength);
        void    RegisterDefaultHandlers();
        bool    ResolveWebRootPath(std::wstring &outPath) const;

        /// @brief /ws/admin WebSocket アップグレード要求を処理する
        /// ハンドシェイク成功後はソケットを CAdminWsHub に渡し、HttpServer は所有権を手放す。
        /// @return ソケットを Hub に移譲した場合 true（HandleAccept は closesocket しない）
        bool    HandleAdminWsUpgrade(SOCKET hClient, const std::string &rawHeaders);

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
        CMainFrame     *m_pMainFrame;
};
