#pragma once

class CHttpServer
{
public:
    CHttpServer();
    ~CHttpServer();

    bool    Start(unsigned short wPort);
    void    Stop();

private:
    static unsigned __stdcall ThreadProc(void *lpParam);

    void    Run();
    bool    InitializeWinsock();
    bool    CreateListener();
    void    CloseListener();
    void    ProcessLoop();
    void    HandleAccept();
    void    HandleClient(SOCKET hClient);
    void    SendJsonResponse(SOCKET hClient, LPCSTR pszStatus, LPCSTR pszBody);
    bool    SendAll(SOCKET hSocket, const char *pData, size_t nLength);

    SOCKET          m_hListen;
    HANDLE          m_hThread;
    HANDLE          m_hStopEvent;
    HANDLE          m_hStartedEvent;
    unsigned short  m_wPort;
    WSADATA         m_wsaData;
    bool            m_bInitSucceeded;
};
