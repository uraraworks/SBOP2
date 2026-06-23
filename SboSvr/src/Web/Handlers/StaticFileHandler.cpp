#include "StdAfx.h"
#include "StaticFileHandler.h"

#include <algorithm>
#include <cctype>
#include <cstdio>

#if defined(_WIN32)
#include <windows.h>
#endif

namespace
{
const unsigned long long kMaxStaticFileSize = 64ULL * 1024ULL * 1024ULL;

// 曜日・月の英語短縮名（RFC 7231 HTTP-date 生成用）
static const char* const kDayNames[7]   = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char* const kMonthNames[12] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

} // anonymous namespace

// ---------------------------------------------------------------------------
// コンストラクタ
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// Handle
// ---------------------------------------------------------------------------
void CStaticFileHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        std::wstring filePath;
        std::string relativePath;
        if (!BuildFilePath(request.path, filePath, relativePath)) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"invalid_path\"}");
                return;
        }

        // --- Phase 1: ファイルのサイズ・mtime だけを取得（本文は読まない）---
        FileMetaInfo meta = {};
        if (!StatFile(filePath, meta)) {
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        const std::string cacheControl = DetermineCacheControl(relativePath);
        const std::string contentType  = DetermineContentType(relativePath);

        // ETag / Last-Modified を生成（meta が有効な場合のみ）
        std::string etag;
        std::string lastModified;
        if (meta.valid) {
                etag         = BuildETag(meta);
                lastModified = BuildLastModified(meta);
        }

        // --- Phase 2: 条件付きリクエスト判定（LoadFile より前）---
        bool send304 = false;
        if (meta.valid) {
                // If-None-Match が優先
                const char* ifNoneMatch = request.FindHeader("If-None-Match");
                if (ifNoneMatch && !etag.empty()) {
                        if (std::string(ifNoneMatch) == etag) {
                                send304 = true;
                        }
                } else {
                        // If-Modified-Since（文字列完全一致による簡易比較）
                        // ※ 自分が返した Last-Modified を次回受け取って比較する往復用途には十分
                        const char* ifModifiedSince = request.FindHeader("If-Modified-Since");
                        if (ifModifiedSince && !lastModified.empty()) {
                                if (std::string(ifModifiedSince) == lastModified) {
                                        send304 = true;
                                }
                        }
                }
        }

        if (send304) {
                response.statusLine = "HTTP/1.1 304 Not Modified";
                response.SetHeader("Cache-Control", cacheControl);
                if (!etag.empty())         { response.SetHeader("ETag",          etag); }
                if (!lastModified.empty()) { response.SetHeader("Last-Modified",  lastModified); }
                // 304 は body 無し。Content-Length=0 をセット。LoadFile は呼ばない。
                response.SetHeader("Content-Length", "0");
                return;
        }

        // --- Phase 3: 200 確定後に初めてファイル本文を読む ---
        std::string content;
        if (!LoadFile(filePath, content)) {
                // StatFile 後にファイルが消えるレース等 → 404
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"not_found\"}");
                return;
        }

        response.statusLine = "HTTP/1.1 200 OK";
        response.body.swap(content);
        response.SetHeader("Content-Type",  contentType);
        response.SetHeader("Cache-Control", cacheControl);
        if (!etag.empty())         { response.SetHeader("ETag",         etag); }
        if (!lastModified.empty()) { response.SetHeader("Last-Modified", lastModified); }
}

// ---------------------------------------------------------------------------
// BuildFilePath
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// StatFile: ファイル本文を読まず、サイズ・mtime のみ取得する軽量メソッド
// kMaxStaticFileSize 超過時は false を返す（配信拒否）。
// ---------------------------------------------------------------------------
bool CStaticFileHandler::StatFile(const std::wstring &path, FileMetaInfo &outMeta) const
{
#if !defined(_WIN32)
        (void)path;
        (void)outMeta;
        return false;
#else
        outMeta.valid    = false;
        outMeta.fileSize = 0;

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

        outMeta.fileSize = static_cast<unsigned long long>(size.QuadPart);

        // mtime 取得（失敗しても valid=false のまま → ETag なし・200 配信は続行）
        FILETIME ftCreate, ftAccess, ftWrite;
        if (GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite)) {
                outMeta.mtime = ftWrite;
                outMeta.valid = true;
        }

        CloseHandle(hFile);
        return true;
#endif
}

// ---------------------------------------------------------------------------
// LoadFile（200 確定後に本文を読む。meta 取得は StatFile に移管済み）
// ---------------------------------------------------------------------------
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
        while (dwTotalRead < static_cast<DWORD>(outContent.size())) {
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
        if (dwTotalRead != static_cast<DWORD>(outContent.size())) {
                outContent.resize(dwTotalRead);
        }
        return true;
#endif
}

// ---------------------------------------------------------------------------
// DetermineContentType
// ---------------------------------------------------------------------------
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
        if (extension == ".wasm") {
                return "application/wasm";
        }
        if (extension == ".data") {
                return "application/octet-stream";
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

// ---------------------------------------------------------------------------
// DetermineCacheControl（拡張子で出し分け）
// ---------------------------------------------------------------------------
std::string CStaticFileHandler::DetermineCacheControl(const std::string &relativePath) const
{
        size_t nDot = relativePath.find_last_of('.');
        std::string extension;
        if (nDot != std::string::npos) {
                extension = relativePath.substr(nDot);
                std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char ch) {
                        return static_cast<char>(std::tolower(ch));
                });
        }

        // HTML は毎回 ETag 検証（即時反映）
        if ((extension == ".html") || (extension == ".htm")) {
                return "no-cache";
        }
        // 大容量バイナリは 1 時間キャッシュ
        if ((extension == ".wasm") || (extension == ".data") || (extension == ".js")) {
                return "public, max-age=3600";
        }
        // その他も無難に 1 時間
        return "public, max-age=3600";
}

// ---------------------------------------------------------------------------
// BuildETag: nginx 風 "<hex_mtime_low>-<hex_size>" （ダブルクォート含む強い ETag）
// ---------------------------------------------------------------------------
/*static*/
std::string CStaticFileHandler::BuildETag(const FileMetaInfo &meta)
{
        // FILETIME は 100ns 単位 ULARGE_INTEGER で 64bit。下位 32bit を hex 化（nginx 準拠）
        ULARGE_INTEGER ul;
        ul.LowPart  = meta.mtime.dwLowDateTime;
        ul.HighPart = meta.mtime.dwHighDateTime;

        char buf[64];
        // nginx は mtime (秒) と size を hex 化するが、ここでは 100ns 単位の下位 32bit を使用
        // 同一ファイルへの書き込み粒度が 100ns のため十分に衝突しにくい
        std::snprintf(buf, sizeof(buf), "\"%x-%llx\"",
                      ul.LowPart,
                      static_cast<unsigned long long>(meta.fileSize));
        return std::string(buf);
}

// ---------------------------------------------------------------------------
// BuildLastModified: RFC 7231 HTTP-date に整形（例: Wed, 21 Oct 2026 07:28:00 GMT）
// ---------------------------------------------------------------------------
/*static*/
std::string CStaticFileHandler::BuildLastModified(const FileMetaInfo &meta)
{
        SYSTEMTIME st;
        if (!FileTimeToSystemTime(&meta.mtime, &st)) {
                return std::string();
        }
        if (st.wDayOfWeek > 6 || st.wMonth < 1 || st.wMonth > 12) {
                return std::string();
        }

        char buf[64];
        std::snprintf(buf, sizeof(buf),
                      "%s, %02d %s %04d %02d:%02d:%02d GMT",
                      kDayNames[st.wDayOfWeek],
                      static_cast<int>(st.wDay),
                      kMonthNames[st.wMonth - 1],
                      static_cast<int>(st.wYear),
                      static_cast<int>(st.wHour),
                      static_cast<int>(st.wMinute),
                      static_cast<int>(st.wSecond));
        return std::string(buf);
}

// ---------------------------------------------------------------------------
// ParseHttpDate: 未使用（If-Modified-Since は文字列完全一致で比較するため）
// 将来的に厳密なパースが必要な場合に備えてスタブとして残す。
// ---------------------------------------------------------------------------
/*static*/
bool CStaticFileHandler::ParseHttpDate(const std::string &httpDate, FILETIME &outFt)
{
        (void)httpDate;
        (void)outFt;
        return false;
}

// ---------------------------------------------------------------------------
// NormalizeRequestPath
// ---------------------------------------------------------------------------
/*static*/
std::string CStaticFileHandler::NormalizeRequestPath(const std::string &path)
{
        size_t nQuery = path.find('?');
        if (nQuery != std::string::npos) {
                return path.substr(0, nQuery);
        }
        return path;
}

// ---------------------------------------------------------------------------
// ContainsParentReference
// ---------------------------------------------------------------------------
/*static*/
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

// ---------------------------------------------------------------------------
// ToUtf8
// ---------------------------------------------------------------------------
/*static*/
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
