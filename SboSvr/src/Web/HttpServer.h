#pragma once

class CHttpServer
{
public:
    CHttpServer();
    ~CHttpServer();

    bool    Start(unsigned short wPort);
    void    Stop();

private:
    static DWORD WINAPI ThreadProc(LPVOID lpParam);

    void    Run();
    bool    InitializeWinsock();
    bool    CreateListener();
    void    CloseListener();
    void    ProcessLoop();
    void    HandleAccept();
    void    HandleClient(SOCKET hClient);
    void    SendJsonResponse(SOCKET hClient, LPCSTR pszStatus, LPCSTR pszBody);

    SOCKET          m_hListen;
    WSAEVENT        m_hListenEvent;
    HANDLE          m_hThread;
    HANDLE          m_hStopEvent;
    HANDLE          m_hStartedEvent;
    unsigned short  m_wPort;
    WSADATA         m_wsaData;
    bool            m_bInitSucceeded;
};
