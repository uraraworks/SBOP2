#include "stdafx.h"
#include "HttpServer.h"

#include <string>
#include <sstream>
#include <cstring>

CHttpServer::CHttpServer()
        : m_hListen(INVALID_SOCKET)
        , m_hListenEvent(WSA_INVALID_EVENT)
        , m_hThread(NULL)
        , m_hStopEvent(NULL)
        , m_wPort(0)
        , m_hStartedEvent(NULL)
        , m_bInitSucceeded(false)
{
        ZeroMemory(&m_wsaData, sizeof(m_wsaData));
}

CHttpServer::~CHttpServer()
{
        Stop();
}

bool CHttpServer::Start(unsigned short wPort)
{
        if (m_hThread != NULL) {
                return false;
        }

        m_wPort = wPort;

        m_hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (m_hStopEvent == NULL) {
                return false;
        }

        m_hStartedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (m_hStartedEvent == NULL) {
                CloseHandle(m_hStopEvent);
                m_hStopEvent = NULL;
                return false;
        }

        m_bInitSucceeded = false;

        m_hThread = CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
        if (m_hThread == NULL) {
                CloseHandle(m_hStartedEvent);
                m_hStartedEvent = NULL;
                CloseHandle(m_hStopEvent);
                m_hStopEvent = NULL;
                return false;
        }

        DWORD dwWait = WaitForSingleObject(m_hStartedEvent, 5000);
        if ((dwWait != WAIT_OBJECT_0) || (m_bInitSucceeded == false)) {
                Stop();
                return false;
        }

        return true;
}

void CHttpServer::Stop()
{
        if (m_hThread != NULL) {
                if (m_hStopEvent != NULL) {
                        SetEvent(m_hStopEvent);
                }
                WaitForSingleObject(m_hThread, 5000);
                CloseHandle(m_hThread);
                m_hThread = NULL;
        }

        if (m_hStopEvent != NULL) {
                CloseHandle(m_hStopEvent);
                m_hStopEvent = NULL;
        }

        if (m_hStartedEvent != NULL) {
                CloseHandle(m_hStartedEvent);
                m_hStartedEvent = NULL;
        }
}

DWORD WINAPI CHttpServer::ThreadProc(LPVOID lpParam)
{
        CHttpServer *pServer = reinterpret_cast<CHttpServer *>(lpParam);
        if (pServer != NULL) {
                pServer->Run();
        }
        return 0;
}

void CHttpServer::Run()
{
        bool bWinsockStarted = false;

        if (InitializeWinsock()) {
                bWinsockStarted = true;

                if (CreateListener()) {
                        m_bInitSucceeded = true;
                        SetEvent(m_hStartedEvent);
                        ProcessLoop();
                } else {
                        SetEvent(m_hStartedEvent);
                }
        } else {
                SetEvent(m_hStartedEvent);
        }

        CloseListener();

        if (bWinsockStarted) {
                WSACleanup();
        }
}

bool CHttpServer::InitializeWinsock()
{
        int nResult = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
        if (nResult != 0) {
                return false;
        }
        return true;
}

bool CHttpServer::CreateListener()
{
        m_hListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_hListen == INVALID_SOCKET) {
                return false;
        }

        DWORD dwReuse = 1;
        setsockopt(m_hListen, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&dwReuse), sizeof(dwReuse));

        sockaddr_in service;
        ZeroMemory(&service, sizeof(service));
        service.sin_family = AF_INET;
        service.sin_addr.s_addr = htonl(INADDR_ANY);
        service.sin_port = htons(m_wPort);

        if (bind(m_hListen, reinterpret_cast<const sockaddr *>(&service), sizeof(service)) == SOCKET_ERROR) {
                return false;
        }

        if (listen(m_hListen, SOMAXCONN) == SOCKET_ERROR) {
                return false;
        }

        m_hListenEvent = WSACreateEvent();
        if (m_hListenEvent == WSA_INVALID_EVENT) {
                return false;
        }

        if (WSAEventSelect(m_hListen, m_hListenEvent, FD_ACCEPT) == SOCKET_ERROR) {
                return false;
        }

        return true;
}

void CHttpServer::CloseListener()
{
        if (m_hListenEvent != WSA_INVALID_EVENT) {
                WSACloseEvent(m_hListenEvent);
                m_hListenEvent = WSA_INVALID_EVENT;
        }

        if (m_hListen != INVALID_SOCKET) {
                closesocket(m_hListen);
                m_hListen = INVALID_SOCKET;
        }
}

void CHttpServer::ProcessLoop()
{
        HANDLE hEvents[2] = { m_hStopEvent, m_hListenEvent };

        while (true) {
                DWORD dwWait = WaitForMultipleObjects(2, hEvents, FALSE, 1000);
                if (dwWait == WAIT_OBJECT_0) {
                        break;
                }

                if (dwWait == WAIT_OBJECT_0 + 1) {
                        WSANETWORKEVENTS events;
                        if (WSAEnumNetworkEvents(m_hListen, m_hListenEvent, &events) == SOCKET_ERROR) {
                                break;
                        }

                        if ((events.lNetworkEvents & FD_ACCEPT) != 0) {
                                HandleAccept();
                        }
                }

                if (dwWait == WAIT_FAILED) {
                        break;
                }
        }
}

void CHttpServer::HandleAccept()
{
        while (true) {
                SOCKET hClient = accept(m_hListen, NULL, NULL);
                if (hClient == INVALID_SOCKET) {
                        int nErr = WSAGetLastError();
                        if (nErr == WSAEWOULDBLOCK) {
                                break;
                        }
                        break;
                }

                HandleClient(hClient);
                closesocket(hClient);
        }
}

void CHttpServer::HandleClient(SOCKET hClient)
{
        std::string request;
        request.reserve(512);

        char szBuffer[512];
        while (request.find("\r\n\r\n") == std::string::npos) {
                int nRecv = recv(hClient, szBuffer, sizeof(szBuffer), 0);
                if (nRecv <= 0) {
                        return;
                }

                request.append(szBuffer, nRecv);
                if (request.size() > 8192) {
                        SendJsonResponse(hClient, "HTTP/1.1 413 Payload Too Large", "{\"error\":\"request_too_large\"}");
                        shutdown(hClient, SD_BOTH);
                        return;
                }
        }

        size_t nPos = request.find("\r\n");
        if (nPos == std::string::npos) {
                SendJsonResponse(hClient, "HTTP/1.1 400 Bad Request", "{\"error\":\"bad_request\"}");
                shutdown(hClient, SD_BOTH);
                return;
        }

        std::string requestLine = request.substr(0, nPos);
        std::istringstream iss(requestLine);
        std::string method;
        std::string path;
        std::string version;
        iss >> method >> path >> version;

        if (method != "GET") {
                SendJsonResponse(hClient, "HTTP/1.1 405 Method Not Allowed", "{\"error\":\"method_not_allowed\"}");
                shutdown(hClient, SD_BOTH);
                return;
        }

        if (path == "/health") {
                SendJsonResponse(hClient, "HTTP/1.1 200 OK", "{\"status\":\"ok\"}");
                shutdown(hClient, SD_BOTH);
                return;
        }

        SendJsonResponse(hClient, "HTTP/1.1 404 Not Found", "{\"error\":\"not_found\"}");
        shutdown(hClient, SD_BOTH);
}

void CHttpServer::SendJsonResponse(SOCKET hClient, LPCSTR pszStatus, LPCSTR pszBody)
{
        size_t nBodyLength = strlen(pszBody);

        char szHeader[256];
        int nHeaderLength = _snprintf_s(
                szHeader,
                _countof(szHeader),
                _TRUNCATE,
                "%s\r\nContent-Type: application/json; charset=utf-8\r\nContent-Length: %u\r\nConnection: close\r\n\r\n",
                pszStatus,
                static_cast<unsigned int>(nBodyLength));

        if (nHeaderLength > 0) {
                send(hClient, szHeader, nHeaderLength, 0);
        }

        if (nBodyLength > 0) {
                send(hClient, pszBody, static_cast<int>(nBodyLength), 0);
        }
}
