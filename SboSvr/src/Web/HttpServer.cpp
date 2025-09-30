#include "stdafx.h"
#include "HttpServer.h"

#include <string>
#include <sstream>
#include <cstring>
#include <process.h>
#include <cctype>
#include <memory>
#include <cstdlib>

#include "HttpTypes.h"
#include "Handlers/HealthHandler.h"
#include "Handlers/ServerInfoHandler.h"
#include "Handlers/AccountCreateHandler.h"
#include "Handlers/AdminRolesHandler.h"
#include "Handlers/MapObjectHandler.h"
#include "Handlers/StaticFileHandler.h"
#include "MgrData.h"

namespace
{
const size_t kMaxHeaderSize = 8192;
const size_t kMaxBodySize = 65536;

std::string TrimCopy(const std::string &text)
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

bool EqualsIgnoreCase(const std::string &lhs, const char *pszRhs)
{
        if (pszRhs == NULL) {
                return false;
        }
        size_t nLen = lhs.size();
        for (size_t i = 0; i < nLen; ++i) {
                unsigned char chL = static_cast<unsigned char>(lhs[i]);
                unsigned char chR = static_cast<unsigned char>(pszRhs[i]);
                if (chR == '\0') {
                        return false;
                }
                int upperL = std::toupper(static_cast<unsigned char>(chL));
                int upperR = std::toupper(static_cast<unsigned char>(chR));
                if (upperL != upperR) {
                        return false;
                }
        }
        return pszRhs[nLen] == '\0';
}

enum ContentLengthParseResult
{
        ContentLengthNotPresent,
        ContentLengthValid,
        ContentLengthInvalid
};

ContentLengthParseResult TryParseContentLength(const std::string &request, size_t nHeaderEnd, size_t &outLength)
{
        outLength = 0;
        if (nHeaderEnd == std::string::npos) {
                return ContentLengthNotPresent;
        }

        size_t nLineStart = request.find("\r\n");
        if (nLineStart == std::string::npos) {
                return ContentLengthNotPresent;
        }
        nLineStart += 2; /* skip request line */

        while (nLineStart < nHeaderEnd) {
                size_t nLineEnd = request.find("\r\n", nLineStart);
                if ((nLineEnd == std::string::npos) || (nLineEnd > nHeaderEnd)) {
                        nLineEnd = nHeaderEnd;
                }

                std::string headerLine = request.substr(nLineStart, nLineEnd - nLineStart);
                size_t nColon = headerLine.find(':');
                if (nColon != std::string::npos) {
                        std::string name = TrimCopy(headerLine.substr(0, nColon));
                        if (EqualsIgnoreCase(name, "Content-Length")) {
                                std::string value = TrimCopy(headerLine.substr(nColon + 1));
                                if (value.empty()) {
                                        return ContentLengthInvalid;
                                }
                                char *pEnd = NULL;
                                unsigned long length = std::strtoul(value.c_str(), &pEnd, 10);
                                if ((pEnd == NULL) || (*pEnd != '\0')) {
                                        return ContentLengthInvalid;
                                }
                                outLength = static_cast<size_t>(length);
                                return ContentLengthValid;
                        }
                }

                if (nLineEnd >= nHeaderEnd) {
                        break;
                }
                nLineStart = nLineEnd + 2;
        }

        return ContentLengthNotPresent;
}
}

CHttpServer::CHttpServer()
        : m_hListen(INVALID_SOCKET)
        , m_hThread(NULL)
        , m_hStopEvent(NULL)
        , m_wPort(0)
        , m_hStartedEvent(NULL)
        , m_bInitSucceeded(false)
        , m_bHandlersRegistered(false)
        , m_pMgrData(NULL)
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

void CHttpServer::SetMgrData(CMgrData *pMgrData)
{
        m_pMgrData = pMgrData;
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
        request.reserve(1024);

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
                if (request.size() > kMaxHeaderSize) {
                        HttpResponse tooLarge;
                        tooLarge.statusLine = "HTTP/1.1 431 Request Header Fields Too Large";
                        tooLarge.SetJsonBody("{\"error\":\"request_headers_too_large\"}");
                        tooLarge.EnsureContentLength();
                        SendResponse(hClient, tooLarge);
                        shutdown(hClient, SD_BOTH);
                        return;
                }
        }

        size_t nHeaderEnd = request.find("\r\n\r\n");
        size_t nBodyStart = (nHeaderEnd == std::string::npos) ? request.size() : (nHeaderEnd + 4);

        size_t nContentLength = 0;
        ContentLengthParseResult lengthResult = TryParseContentLength(request, nHeaderEnd, nContentLength);
        if (lengthResult == ContentLengthInvalid) {
                HttpResponse badRequest;
                badRequest.statusLine = "HTTP/1.1 400 Bad Request";
                badRequest.SetJsonBody("{\"error\":\"invalid_content_length\"}");
                badRequest.EnsureContentLength();
                SendResponse(hClient, badRequest);
                shutdown(hClient, SD_BOTH);
                return;
        }

        if ((lengthResult == ContentLengthValid) && (nContentLength > kMaxBodySize)) {
                HttpResponse tooLarge;
                tooLarge.statusLine = "HTTP/1.1 413 Payload Too Large";
                tooLarge.SetJsonBody("{\"error\":\"request_body_too_large\"}");
                tooLarge.EnsureContentLength();
                SendResponse(hClient, tooLarge);
                shutdown(hClient, SD_BOTH);
                return;
        }

        size_t nExistingBody = (nBodyStart < request.size()) ? (request.size() - nBodyStart) : 0;
        if ((lengthResult == ContentLengthValid) && (nExistingBody < nContentLength)) {
                request.reserve(nBodyStart + nContentLength);
                while (nExistingBody < nContentLength) {
                        size_t nRemaining = nContentLength - nExistingBody;
                        size_t nToRead = (nRemaining < sizeof(szBuffer)) ? nRemaining : sizeof(szBuffer);
                        int nRecv = recv(hClient, szBuffer, static_cast<int>(nToRead), 0);
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
                        nExistingBody += static_cast<size_t>(nRecv);
                }
        }

        if ((lengthResult == ContentLengthValid) && (nBodyStart + nContentLength < request.size())) {
                request.resize(nBodyStart + nContentLength);
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
        return TrimCopy(text);
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

        std::unique_ptr<IApiHandler> serverHandler(new CServerInfoHandler(m_pMgrData));
        m_router.Register("GET", "/api/server", std::move(serverHandler));

        std::unique_ptr<IApiHandler> accountCreateHandler(new CAccountCreateHandler(m_pMgrData));
        m_router.Register("POST", "/api/accounts", std::move(accountCreateHandler));

        std::unique_ptr<IApiHandler> rolesListHandler(new CAdminRolesListHandler(m_pMgrData));
        m_router.Register("GET", "/api/admin/roles", std::move(rolesListHandler));

        std::unique_ptr<IApiHandler> rolesUpdateHandler(new CAdminRolesUpdateHandler(m_pMgrData));
        m_router.Register("PUT", "/api/admin/roles", std::move(rolesUpdateHandler));

        std::unique_ptr<IApiHandler> mapObjectsHandler(new CMapObjectListHandler(m_pMgrData));
        m_router.Register("GET", "/api/maps/objects", std::move(mapObjectsHandler));

        std::wstring webRoot;
        if (ResolveWebRootPath(webRoot)) {
                std::unique_ptr<IApiHandler> staticHandler(new CStaticFileHandler(webRoot, L"index.html", "/"));
                m_router.RegisterPrefix("GET", "/", std::move(staticHandler));
        }

        m_bHandlersRegistered = true;
}

bool CHttpServer::ResolveWebRootPath(std::wstring &outPath) const
{
#if !defined(_WIN32)
        (void)outPath;
        return false;
#else
        wchar_t szModulePath[MAX_PATH];
        DWORD dwLength = GetModuleFileNameW(NULL, szModulePath, MAX_PATH);
        if ((dwLength == 0) || (dwLength >= MAX_PATH)) {
                return false;
        }

        wchar_t *pSlash = wcsrchr(szModulePath, L'\\');
        if (pSlash == NULL) {
                return false;
        }
        *(pSlash + 1) = L'\0';

        std::wstring basePath = szModulePath;
        basePath.append(L"webroot");

        DWORD dwAttributes = GetFileAttributesW(basePath.c_str());
        if (dwAttributes == INVALID_FILE_ATTRIBUTES) {
                return false;
        }
        if ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
                return false;
        }

        outPath = basePath;
        return true;
#endif
}
