#include "stdafx.h"
#include "HttpServer.h"

#include <string>
#include <sstream>
#include <cstring>
#include <process.h>
#include <cctype>
#include <memory>

#include "HttpTypes.h"
#include "Handlers/HealthHandler.h"

CHttpServer::CHttpServer()
        : m_hListen(INVALID_SOCKET)
        , m_hThread(NULL)
        , m_hStopEvent(NULL)
        , m_wPort(0)
        , m_hStartedEvent(NULL)
        , m_bInitSucceeded(false)
        , m_bHandlersRegistered(false)
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

        RegisterDefaultHandlers();

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

        unsigned uThreadId = 0;
        m_hThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, ThreadProc, this, 0, &uThreadId));
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

unsigned __stdcall CHttpServer::ThreadProc(void *lpParam)
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

        return true;
}

void CHttpServer::CloseListener()
{
        if (m_hListen != INVALID_SOCKET) {
                closesocket(m_hListen);
                m_hListen = INVALID_SOCKET;
        }
}

void CHttpServer::ProcessLoop()
{
        while (WaitForSingleObject(m_hStopEvent, 0) == WAIT_TIMEOUT) {
                fd_set readSet;
                FD_ZERO(&readSet);
                FD_SET(m_hListen, &readSet);

                timeval tv;
                tv.tv_sec = 0;
                tv.tv_usec = 500000; /* 500ms */

                int nReady = select(0, &readSet, NULL, NULL, &tv);
                if (nReady == SOCKET_ERROR) {
                        break;
                }

                if (nReady == 0) {
                        continue;
                }

                if (FD_ISSET(m_hListen, &readSet)) {
                        HandleAccept();
                }
        }
}

void CHttpServer::HandleAccept()
{
        SOCKET hClient = accept(m_hListen, NULL, NULL);
        if (hClient == INVALID_SOCKET) {
                return;
        }

        DWORD dwTimeout = 5000;
        setsockopt(hClient, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&dwTimeout), sizeof(dwTimeout));
        setsockopt(hClient, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char *>(&dwTimeout), sizeof(dwTimeout));

        HandleClient(hClient);
        closesocket(hClient);
}

void CHttpServer::HandleClient(SOCKET hClient)
{
        std::string request;
        request.reserve(512);

        char szBuffer[512];
        while (request.find("\r\n\r\n") == std::string::npos) {
                int nRecv = recv(hClient, szBuffer, sizeof(szBuffer), 0);
                if (nRecv == 0) {
                        shutdown(hClient, SD_BOTH);
                        return;
                }

                if (nRecv == SOCKET_ERROR) {
                        int nErr = WSAGetLastError();
                        if ((nErr == WSAEINTR) || (nErr == WSAEWOULDBLOCK)) {
                                continue;
                        }
                        if (nErr == WSAETIMEDOUT) {
                                HttpResponse timeoutResponse;
                                timeoutResponse.statusLine = "HTTP/1.1 408 Request Timeout";
                                timeoutResponse.SetJsonBody("{\"error\":\"timeout\"}");
                                timeoutResponse.EnsureContentLength();
                                SendResponse(hClient, timeoutResponse);
                        }
                        shutdown(hClient, SD_BOTH);
                        return;
                }

                request.append(szBuffer, nRecv);
                if (request.size() > 8192) {
                        HttpResponse tooLarge;
                        tooLarge.statusLine = "HTTP/1.1 413 Payload Too Large";
                        tooLarge.SetJsonBody("{\"error\":\"request_too_large\"}");
                        tooLarge.EnsureContentLength();
                        SendResponse(hClient, tooLarge);
                        shutdown(hClient, SD_BOTH);
                        return;
                }
        }

        HttpRequest httpRequest;
        if (!ParseHttpRequest(request, httpRequest)) {
                HttpResponse badRequest;
                badRequest.statusLine = "HTTP/1.1 400 Bad Request";
                badRequest.SetJsonBody("{\"error\":\"bad_request\"}");
                badRequest.EnsureContentLength();
                SendResponse(hClient, badRequest);
                shutdown(hClient, SD_BOTH);
                return;
        }

        HttpResponse httpResponse;
        std::string allowedMethods;
        CApiRouter::RouteResult result = m_router.Dispatch(httpRequest, httpResponse, &allowedMethods);

        if (result == CApiRouter::RouteMethodNotAllowed) {
                httpResponse.statusLine = "HTTP/1.1 405 Method Not Allowed";
                httpResponse.SetJsonBody("{\"error\":\"method_not_allowed\"}");
                if (!allowedMethods.empty()) {
                        httpResponse.SetHeader("Allow", allowedMethods);
                }
        } else if (result == CApiRouter::RouteNotFound) {
                httpResponse.statusLine = "HTTP/1.1 404 Not Found";
                httpResponse.SetJsonBody("{\"error\":\"not_found\"}");
        }

        httpResponse.EnsureContentLength();
        SendResponse(hClient, httpResponse);
        shutdown(hClient, SD_BOTH);
}

bool CHttpServer::SendAll(SOCKET hSocket, const char *pData, size_t nLength)
{
        size_t nTotalSent = 0;
        while (nTotalSent < nLength) {
                int nSent = send(hSocket, pData + nTotalSent, static_cast<int>(nLength - nTotalSent), 0);
                if (nSent == SOCKET_ERROR) {
                        int nErr = WSAGetLastError();
                        if ((nErr == WSAEINTR) || (nErr == WSAETIMEDOUT) || (nErr == WSAEWOULDBLOCK)) {
                                continue;
                        }
                        return false;
                }

                if (nSent == 0) {
                        return false;
                }

                nTotalSent += static_cast<size_t>(nSent);
        }

        return true;
}

bool CHttpServer::ParseHttpRequest(const std::string &rawRequest, HttpRequest &outRequest)
{
        size_t nHeaderEnd = rawRequest.find("\r\n\r\n");
        if (nHeaderEnd == std::string::npos) {
                return false;
        }

        size_t nFirstLineEnd = rawRequest.find("\r\n");
        if (nFirstLineEnd == std::string::npos) {
                return false;
        }

        std::string requestLine = rawRequest.substr(0, nFirstLineEnd);
        std::istringstream iss(requestLine);
        iss >> outRequest.method >> outRequest.path >> outRequest.version;

        if (outRequest.method.empty() || outRequest.path.empty()) {
                return false;
        }

        outRequest.headers.clear();

        size_t nHeaderPos = nFirstLineEnd + 2;
        while (nHeaderPos < nHeaderEnd) {
                size_t nLineEnd = rawRequest.find("\r\n", nHeaderPos);
                if ((nLineEnd == std::string::npos) || (nLineEnd > nHeaderEnd)) {
                        nLineEnd = nHeaderEnd;
                }

                std::string headerLine = rawRequest.substr(nHeaderPos, nLineEnd - nHeaderPos);
                size_t nColonPos = headerLine.find(':');
                if (nColonPos != std::string::npos) {
                        std::string name = Trim(headerLine.substr(0, nColonPos));
                        std::string value = Trim(headerLine.substr(nColonPos + 1));
                        if (!name.empty()) {
                                HttpHeader header;
                                header.name = name;
                                header.value = value;
                                outRequest.headers.push_back(header);
                        }
                }

                if (nLineEnd >= nHeaderEnd) {
                        break;
                }
                nHeaderPos = nLineEnd + 2;
        }

        size_t nBodyPos = nHeaderEnd + 4;
        if (nBodyPos < rawRequest.size()) {
                outRequest.body = rawRequest.substr(nBodyPos);
        } else {
                outRequest.body.clear();
        }

        return true;
}

std::string CHttpServer::Trim(const std::string &text)
{
        size_t nStart = 0;
        while ((nStart < text.size()) && std::isspace(static_cast<unsigned char>(text[nStart]))) {
                ++nStart;
        }

        size_t nEnd = text.size();
        while ((nEnd > nStart) && std::isspace(static_cast<unsigned char>(text[nEnd - 1]))) {
                --nEnd;
        }

        return text.substr(nStart, nEnd - nStart);
}

void CHttpServer::SendResponse(SOCKET hClient, const HttpResponse &response)
{
        std::ostringstream oss;
        oss << response.statusLine << "\r\n";
        for (std::vector<HttpHeader>::const_iterator it = response.headers.begin(); it != response.headers.end(); ++it) {
                oss << it->name << ": " << it->value << "\r\n";
        }
        oss << "\r\n";

        std::string headerBlock = oss.str();
        if (!headerBlock.empty()) {
                SendAll(hClient, headerBlock.c_str(), headerBlock.size());
        }

        if (!response.body.empty()) {
                SendAll(hClient, response.body.c_str(), response.body.size());
        }
}

void CHttpServer::RegisterDefaultHandlers()
{
        if (m_bHandlersRegistered) {
                return;
        }

        std::unique_ptr<IApiHandler> healthHandler(new CHealthHandler());
        m_router.Register("GET", "/health", std::move(healthHandler));
        m_bHandlersRegistered = true;
}
