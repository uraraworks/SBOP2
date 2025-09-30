#include "stdafx.h"
#include "StaticFileHandler.h"

#include <algorithm>
#include <cctype>

#if defined(_WIN32)
#include <windows.h>
#endif

namespace
{
const unsigned long long kMaxStaticFileSize = 2ULL * 1024ULL * 1024ULL;
}

CStaticFileHandler::CStaticFileHandler(const std::wstring &rootDirectory, const std::wstring &defaultDocument, const std::string &mountPath)
        : m_rootDirectory(rootDirectory)
        , m_defaultDocument(defaultDocument)
        , m_mountPath(mountPath)
{
        if (!m_mountPath.empty() && (m_mountPath[m_mountPath.size() - 1] != '/')) {
                m_mountPath.push_back('/');
        }
        if (!m_defaultDocument.empty()) {
                if ((m_defaultDocument[0] == L'/') || (m_defaultDocument[0] == L'\\')) {
                        m_defaultDocument.erase(m_defaultDocument.begin());
                }
        }
}

void CStaticFileHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        std::wstring filePath;
        std::string relativePath;
        if (!BuildFilePath(request.path, filePath, relativePath)) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_path\"}");
                return;
        }

        std::string content;
        if (!LoadFile(filePath, content)) {
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        response.statusLine = "HTTP/1.1 200 OK";
        response.body.swap(content);
        response.SetHeader("Content-Type", DetermineContentType(relativePath));
}

bool CStaticFileHandler::BuildFilePath(const std::string &requestPath, std::wstring &outPath, std::string &outRelativePath) const
{
        std::string normalized = NormalizeRequestPath(requestPath);
        if (ContainsParentReference(normalized)) {
                return false;
        }

        std::string relative;
        if (!m_mountPath.empty() && m_mountPath != "/") {
                if (normalized.size() < m_mountPath.size()) {
                        return false;
                }
                if (normalized.compare(0, m_mountPath.size(), m_mountPath) != 0) {
                        return false;
                }
                relative = normalized.substr(m_mountPath.size());
        } else {
                if (!normalized.empty() && (normalized[0] == '/')) {
                        relative = normalized.substr(1);
                } else {
                        relative = normalized;
                }
        }

        if (relative.empty()) {
                if (m_defaultDocument.empty()) {
                        return false;
                }
                std::wstring defaultDoc = m_defaultDocument;
                std::wstring base = m_rootDirectory;
                if (!base.empty()) {
                        wchar_t last = base[base.size() - 1];
                        if ((last != L'\\') && (last != L'/')) {
                                base.push_back(L'\\');
                        }
                }
                outPath = base + defaultDoc;
                outRelativePath = ToUtf8(defaultDoc);
                return true;
        }

        if (!relative.empty() && (relative[relative.size() - 1] == '/')) {
                if (m_defaultDocument.empty()) {
                        return false;
                }
                std::string defaultDocUtf8 = ToUtf8(m_defaultDocument);
                relative.append(defaultDocUtf8);
        }

        std::wstring wideRelative;
        wideRelative.reserve(relative.size());
        for (size_t i = 0; i < relative.size(); ++i) {
                char ch = relative[i];
                if (ch == '\\') {
                        return false;
                }
                if (ch == '/') {
                        wideRelative.push_back(L'\\');
                } else {
                        wideRelative.push_back(static_cast<unsigned char>(ch));
                }
        }

        std::wstring base = m_rootDirectory;
        if (!base.empty()) {
                wchar_t last = base[base.size() - 1];
                if ((last != L'\\') && (last != L'/')) {
                        base.push_back(L'\\');
                }
        }
        outPath = base + wideRelative;
        outRelativePath = relative;
        return true;
}

bool CStaticFileHandler::LoadFile(const std::wstring &path, std::string &outContent) const
{
#if !defined(_WIN32)
        (void)path;
        (void)outContent;
        return false;
#else
        HANDLE hFile = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
                return false;
        }

        LARGE_INTEGER size;
        if (!GetFileSizeEx(hFile, &size)) {
                CloseHandle(hFile);
                return false;
        }
        if (size.QuadPart < 0) {
                CloseHandle(hFile);
                return false;
        }
        if (static_cast<unsigned long long>(size.QuadPart) > kMaxStaticFileSize) {
                CloseHandle(hFile);
                return false;
        }

        outContent.resize(static_cast<size_t>(size.QuadPart));
        DWORD dwRead = 0;
        DWORD dwTotalRead = 0;
        while (dwTotalRead < outContent.size()) {
                DWORD dwToRead = static_cast<DWORD>(outContent.size() - dwTotalRead);
                if (!ReadFile(hFile, &outContent[dwTotalRead], dwToRead, &dwRead, NULL)) {
                        CloseHandle(hFile);
                        outContent.clear();
                        return false;
                }
                if (dwRead == 0) {
                        break;
                }
                dwTotalRead += dwRead;
        }
        CloseHandle(hFile);
        if (dwTotalRead != outContent.size()) {
                outContent.resize(dwTotalRead);
        }
        return true;
#endif
}

std::string CStaticFileHandler::DetermineContentType(const std::string &relativePath) const
{
        size_t nDot = relativePath.find_last_of('.');
        std::string extension;
        if (nDot != std::string::npos) {
                extension = relativePath.substr(nDot);
                std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char ch) {
                        return static_cast<char>(std::tolower(ch));
                });
        }

        if ((extension == ".html") || (extension == ".htm")) {
                return "text/html; charset=utf-8";
        }
        if (extension == ".css") {
                return "text/css; charset=utf-8";
        }
        if (extension == ".js") {
                return "application/javascript; charset=utf-8";
        }
        if (extension == ".json") {
                return "application/json; charset=utf-8";
        }
        if ((extension == ".png") || (extension == ".apng")) {
                return "image/png";
        }
        if ((extension == ".jpg") || (extension == ".jpeg")) {
                return "image/jpeg";
        }
        if (extension == ".svg") {
                return "image/svg+xml";
        }
        if (extension == ".ico") {
                return "image/x-icon";
        }
        if (extension == ".txt") {
                return "text/plain; charset=utf-8";
        }
        return "application/octet-stream";
}

std::string CStaticFileHandler::NormalizeRequestPath(const std::string &path)
{
        size_t nQuery = path.find('?');
        if (nQuery != std::string::npos) {
                return path.substr(0, nQuery);
        }
        return path;
}

bool CStaticFileHandler::ContainsParentReference(const std::string &path)
{
        if (path.find("..") == std::string::npos) {
                return false;
        }

        size_t nPos = 0;
        while (nPos < path.size()) {
                size_t nDot = path.find("..", nPos);
                if (nDot == std::string::npos) {
                        break;
                }
                bool bSafePrefix = (nDot == 0) || (path[nDot - 1] == '/') || (path[nDot - 1] == '\\');
                size_t nSuffix = nDot + 2;
                bool bSafeSuffix = (nSuffix >= path.size()) || (path[nSuffix] == '/') || (path[nSuffix] == '\\');
                if (bSafePrefix && bSafeSuffix) {
                        return true;
                }
                nPos = nDot + 2;
        }
        return false;
}

std::string CStaticFileHandler::ToUtf8(const std::wstring &text)
{
        std::string result;
        result.reserve(text.size());
        for (size_t i = 0; i < text.size(); ++i) {
                wchar_t ch = text[i];
                if (ch >= 0 && ch <= 0x7F) {
                        result.push_back(static_cast<char>(ch));
                } else {
                        result.push_back('?');
                }
        }
        return result;
}
