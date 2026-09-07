#include "StdAfx.h"
#include "StaticFileHandler.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <sys/stat.h>

#include "../../Platform/SvrPlatform.h"

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
        outMeta.valid    = false;
        outMeta.fileSize = 0;
        outMeta.mtime    = 0;

#if !defined(_WIN32)
        (void)path;
        return false;
#else
        // サイズ・mtime のみが要る軽量経路。stat() 系に統一しておくことで
        // FILETIME(100ns/Windows epoch)を触らずに済み、非Windows(stat()実装)
        // と同じ型(time_t/秒)で扱える。
        struct _stat64 st;
        if (_wstat64(path.c_str(), &st) != 0) {
                return false;
        }
        if (st.st_size < 0) {
                return false;
        }
        if (static_cast<unsigned long long>(st.st_size) > kMaxStaticFileSize) {
                return false;
        }

        outMeta.fileSize = static_cast<unsigned long long>(st.st_size);
        outMeta.mtime    = st.st_mtime;
        outMeta.valid    = true;
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

        // HTML / スクリプト / スタイルは毎回 ETag 検証（即時反映）。
        // 管理画面の JS・CSS は頻繁に直すので、max-age を効かせると
        // 修正が最大 1 時間ブラウザに届かず「直したのに変わらない」を招く。
        // no-cache でも中身が変わっていなければ 304 が返るだけなので実質無料。
        // （ゲームクライアントの sbocli-title.js も 211KB 程度で、
        //   45MB の本体は .data 側なのでキャッシュ効果は落ちない）
        if ((extension == ".html") || (extension == ".htm") ||
            (extension == ".js")   || (extension == ".mjs") ||
            (extension == ".css")) {
                return "no-cache";
        }
        // 大容量バイナリは 1 時間キャッシュ
        if ((extension == ".wasm") || (extension == ".data")) {
                return "public, max-age=3600";
        }
        // その他も無難に 1 時間
        return "public, max-age=3600";
}

// ---------------------------------------------------------------------------
// BuildETag: nginx 風 "<hex_mtime>-<hex_size>" （ダブルクォート含む強い ETag）
// ---------------------------------------------------------------------------
/*static*/
std::string CStaticFileHandler::BuildETag(const FileMetaInfo &meta)
{
        // mtime(time_t、秒単位) と size を hex 化する（nginx 準拠）。
        // 旧実装は FILETIME(100ns 単位)の下位 32bit を使っていたが、
        // Last-Modified 側がそもそも秒精度までしか運べないため、
        // ETag も秒精度で揃えて実害は無い(同一ファイルが1秒未満で
        // 書き換わる想定は無い)。
        char buf[64];
        std::snprintf(buf, sizeof(buf), "\"%llx-%llx\"",
                      static_cast<unsigned long long>(meta.mtime),
                      static_cast<unsigned long long>(meta.fileSize));
        return std::string(buf);
}

// ---------------------------------------------------------------------------
// BuildLastModified: RFC 7231 HTTP-date に整形（例: Wed, 21 Oct 2026 07:28:00 GMT）
// ---------------------------------------------------------------------------
/*static*/
std::string CStaticFileHandler::BuildLastModified(const FileMetaInfo &meta)
{
        SboPlatform::GMTIME gt;
        if (!SboPlatform::GmTimeUtc(meta.mtime, &gt)) {
                return std::string();
        }
        if ((gt.nWeekDay < 0) || (gt.nWeekDay > 6) || (gt.nMonth < 1) || (gt.nMonth > 12)) {
                return std::string();
        }

        char buf[64];
        std::snprintf(buf, sizeof(buf),
                      "%s, %02d %s %04d %02d:%02d:%02d GMT",
                      kDayNames[gt.nWeekDay],
                      gt.nDay,
                      kMonthNames[gt.nMonth - 1],
                      gt.nYear,
                      gt.nHour,
                      gt.nMinute,
                      gt.nSecond);
        return std::string(buf);
}

// ---------------------------------------------------------------------------
// ParseHttpDate: RFC 7231 IMF-fixdate ("Ddd, DD Mon YYYY HH:MM:SS GMT") を
// time_t(UTC) へ変換する。If-Modified-Since は文字列完全一致で比較しており
// 通常経路では呼ばれないが、往復一致(Build→Parse)をテストで保証するために
// 実装を持つ。
// ---------------------------------------------------------------------------
/*static*/
bool CStaticFileHandler::ParseHttpDate(const std::string &httpDate, std::time_t &outTime)
{
        // 例: "Wed, 21 Oct 2026 07:28:00 GMT"
        char szDay[4] = {0};
        char szMonth[4] = {0};
        int nDay = 0, nYear = 0, nHour = 0, nMinute = 0, nSecond = 0;

        if (std::sscanf(httpDate.c_str(), "%3[A-Za-z], %2d %3[A-Za-z] %4d %2d:%2d:%2d GMT",
                         szDay, &nDay, szMonth, &nYear, &nHour, &nMinute, &nSecond) != 7) {
                return false;
        }

        int nMonth = -1;
        for (int i = 0; i < 12; ++i) {
                if (std::strcmp(szMonth, kMonthNames[i]) == 0) {
                        nMonth = i + 1;
                        break;
                }
        }
        if (nMonth < 0) {
                return false;
        }
        if ((nDay < 1) || (nDay > 31) || (nHour < 0) || (nHour > 23) ||
            (nMinute < 0) || (nMinute > 59) || (nSecond < 0) || (nSecond > 60)) {
                return false;
        }

        SboPlatform::GMTIME gt;
        std::memset(&gt, 0, sizeof(gt));
        gt.nYear   = nYear;
        gt.nMonth  = nMonth;
        gt.nDay    = nDay;
        gt.nHour   = nHour;
        gt.nMinute = nMinute;
        gt.nSecond = nSecond;

        std::time_t t = 0;
        if (!SboPlatform::TimeGmUtc(gt, &t)) {
                return false;
        }
        outTime = t;
        return true;
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
