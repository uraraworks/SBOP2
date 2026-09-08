/// @file TestStaticFileHandler.cpp
/// @brief 静的ファイル配信(ETag/Last-Modified)のテスト
/// @copyright Copyright(C)URARA-works
///
/// StaticFileHandler の更新時刻は FILETIME(Windows専用) から time_t へ
/// 移植した(project_static_file_http_cache 参照)。Web版45MBを ETag + 304
/// でキャッシュする経路そのものなので、ここが壊れるとキャッシュが効かなく
/// なる(常に本文を再送する)か、逆に更新したファイルが反映されなくなる。
///
/// 確認すること。
/// 1. BuildLastModified が書式(RFC 7231 IMF-fixdate)どおりに整形すること。
/// 2. BuildLastModified → ParseHttpDate が往復一致すること(秒精度)。
/// 3. Handle() が実ファイルに対して 200→ETag/Last-Modified発行→
///    If-None-Match/If-Modified-Since 付き再リクエストで 304 を返すこと
///    (3段構成: stat→304判定→本文読込 を壊していないことの確認)。
/// 4. 事前圧縮ファイル(.br/.gz)がある場合、Accept-Encoding に応じて
///    そちらを配信し Content-Encoding / Vary を返すこと。
///    サーバーは実行時圧縮をしないため、ここが壊ると常に非圧縮配信に戻る。

#include "StdAfx.h"
#include "TestFramework.h"
#include "Handlers/StaticFileHandler.h"
#include "HttpTypes.h"
#include <cstdio>
#include <string>

namespace
{
    // テスト用の一時ファイルを作る
    class CTempFile
    {
    public:
        explicit CTempFile(const char *pszContent)
        {
            char szDir[MAX_PATH];
            char szPath[MAX_PATH];

            GetTempPathA(MAX_PATH, szDir);
            GetTempFileNameA(szDir, "sfh", 0, szPath);
            m_strPath = szPath;

            FILE *pFile = fopen(m_strPath.c_str(), "wb");
            if (pFile) {
                if (pszContent != NULL) {
                    fwrite(pszContent, 1, strlen(pszContent), pFile);
                }
                fclose(pFile);
            }
        }
        ~CTempFile(void)
        {
            DeleteFileA(m_strPath.c_str());
        }
        const std::string &Path(void) const { return m_strPath; }

    private:
        std::string m_strPath;
    };

    // 事前圧縮ファイル(<元のパス>.br など)を作る。中身は圧縮済みバイト列を
    // 模した任意のデータでよい(ハンドラは中身を解釈せずそのまま返すため)。
    class CTempSidecar
    {
    public:
        CTempSidecar(const std::string &strBasePath, const char *pszExt, const char *pszContent)
        {
            m_strPath = strBasePath + pszExt;

            FILE *pFile = fopen(m_strPath.c_str(), "wb");
            if (pFile) {
                if (pszContent != NULL) {
                    fwrite(pszContent, 1, strlen(pszContent), pFile);
                }
                fclose(pFile);
            }
        }
        ~CTempSidecar(void)
        {
            DeleteFileA(m_strPath.c_str());
        }

    private:
        std::string m_strPath;
    };

    // レスポンスヘッダを名前で引く(見つからなければ空文字)
    std::string FindResponseHeader(const HttpResponse &res, const char *pszName)
    {
        for (size_t i = 0; i < res.headers.size(); ++i) {
            if (res.headers[i].name == pszName) {
                return res.headers[i].value;
            }
        }
        return std::string();
    }

    // "C:\...\sfhXXXX.tmp" → root="C:\...", mount="/", file="sfhXXXX.tmp"
    void SplitDirAndName(const std::string &strFullPath, std::wstring &outDir, std::string &outName)
    {
        std::string::size_type nPos = strFullPath.find_last_of("\\/");
        std::string strDir  = (nPos == std::string::npos) ? "" : strFullPath.substr(0, nPos);
        std::string strName = (nPos == std::string::npos) ? strFullPath : strFullPath.substr(nPos + 1);

        outDir.assign(strDir.begin(), strDir.end());
        outName = strName;
    }
}

//////////////////////////////////////////////////////////////////////
// BuildLastModified / ParseHttpDate
//////////////////////////////////////////////////////////////////////

TEST(静的配信_LastModifiedの書式が既知の値と一致する)
{
    // 2025-10-21 07:28:00 UTC (火曜日)
    // = 1761031680 (エポック秒。date -u -d @1761031680 で検算済み)
    FileMetaInfo meta = {};
    meta.valid    = true;
    meta.fileSize = 123;
    meta.mtime    = static_cast<std::time_t>(1761031680);

    std::string strLastModified = CStaticFileHandler::BuildLastModified(meta);
    CHECK(strLastModified == "Tue, 21 Oct 2025 07:28:00 GMT");
}

TEST(静的配信_LastModifiedとParseHttpDateが往復一致する)
{
    // 複数の時刻(うるう年・月末・年始をまたぐケースを含む)で往復させる
    std::time_t times[] = {
        static_cast<std::time_t>(0),           // 1970-01-01
        static_cast<std::time_t>(1000000000),  // 2001-09-09
        static_cast<std::time_t>(1735689599),  // 2024-12-31 23:59:59 (うるう年末)
        static_cast<std::time_t>(1761031680),  // 2026-10-21 07:28:00
    };

    for (size_t i = 0; i < sizeof(times) / sizeof(times[0]); ++i) {
        FileMetaInfo meta = {};
        meta.valid = true;
        meta.mtime = times[i];

        std::string strLastModified = CStaticFileHandler::BuildLastModified(meta);
        CHECK(strLastModified.empty() == false);

        std::time_t tParsed = 0;
        CHECK(CStaticFileHandler::ParseHttpDate(strLastModified, tParsed) != false);
        CHECK_EQ((long long)times[i], (long long)tParsed);
    }
}

TEST(静的配信_不正な日付文字列はパース失敗する)
{
    std::time_t tParsed = 0;
    CHECK(CStaticFileHandler::ParseHttpDate("", tParsed) == false);
    CHECK(CStaticFileHandler::ParseHttpDate("not a date", tParsed) == false);
    CHECK(CStaticFileHandler::ParseHttpDate("Wed, 21 Xyz 2026 07:28:00 GMT", tParsed) == false);
}

//////////////////////////////////////////////////////////////////////
// Handle() の3段構成(stat→304判定→本文読込)を壊していないことの確認
//////////////////////////////////////////////////////////////////////

TEST(静的配信_初回は200でETagとLastModifiedを返す)
{
    CTempFile file("hello world");
    std::wstring strDir;
    std::string strName;
    SplitDirAndName(file.Path(), strDir, strName);

    CStaticFileHandler handler(strDir, L"", "/");

    HttpRequest req;
    req.method = "GET";
    req.path = "/" + strName;

    HttpResponse res;
    handler.Handle(req, res);

    CHECK(res.statusLine.find("200") != std::string::npos);
    CHECK(res.body == "hello world");

    bool bHasETag = false;
    bool bHasLastModified = false;
    for (size_t i = 0; i < res.headers.size(); ++i) {
        if (res.headers[i].name == "ETag")         { bHasETag = true; }
        if (res.headers[i].name == "Last-Modified") { bHasLastModified = true; }
    }
    CHECK(bHasETag);
    CHECK(bHasLastModified);
}

TEST(静的配信_ETag一致で304を返す)
{
    CTempFile file("cache me");
    std::wstring strDir;
    std::string strName;
    SplitDirAndName(file.Path(), strDir, strName);

    CStaticFileHandler handler(strDir, L"", "/");

    // 1回目: 200 で ETag を受け取る
    HttpRequest req1;
    req1.method = "GET";
    req1.path = "/" + strName;
    HttpResponse res1;
    handler.Handle(req1, res1);
    CHECK(res1.statusLine.find("200") != std::string::npos);

    std::string strETag;
    for (size_t i = 0; i < res1.headers.size(); ++i) {
        if (res1.headers[i].name == "ETag") {
            strETag = res1.headers[i].value;
        }
    }
    CHECK(strETag.empty() == false);

    // 2回目: If-None-Match に同じ ETag を付けて再リクエスト → 304、本文なし
    HttpRequest req2;
    req2.method = "GET";
    req2.path = "/" + strName;
    HttpHeader h;
    h.name = "If-None-Match";
    h.value = strETag;
    req2.headers.push_back(h);

    HttpResponse res2;
    handler.Handle(req2, res2);

    CHECK(res2.statusLine.find("304") != std::string::npos);
    CHECK(res2.body.empty());
}

TEST(静的配信_LastModified一致で304を返す)
{
    CTempFile file("cache me too");
    std::wstring strDir;
    std::string strName;
    SplitDirAndName(file.Path(), strDir, strName);

    CStaticFileHandler handler(strDir, L"", "/");

    HttpRequest req1;
    req1.method = "GET";
    req1.path = "/" + strName;
    HttpResponse res1;
    handler.Handle(req1, res1);
    CHECK(res1.statusLine.find("200") != std::string::npos);

    std::string strLastModified;
    for (size_t i = 0; i < res1.headers.size(); ++i) {
        if (res1.headers[i].name == "Last-Modified") {
            strLastModified = res1.headers[i].value;
        }
    }
    CHECK(strLastModified.empty() == false);

    HttpRequest req2;
    req2.method = "GET";
    req2.path = "/" + strName;
    HttpHeader h;
    h.name = "If-Modified-Since";
    h.value = strLastModified;
    req2.headers.push_back(h);

    HttpResponse res2;
    handler.Handle(req2, res2);

    CHECK(res2.statusLine.find("304") != std::string::npos);
    CHECK(res2.body.empty());
}

TEST(静的配信_brを受け入れるなら事前圧縮ファイルを返す)
{
    CTempFile file("hello world");
    CTempSidecar brFile(file.Path(), ".br", "BROTLI-BYTES");
    std::wstring strDir;
    std::string strName;
    SplitDirAndName(file.Path(), strDir, strName);

    CStaticFileHandler handler(strDir, L"", "/");

    HttpRequest req;
    req.method = "GET";
    req.path = "/" + strName;
    HttpHeader header;
    header.name = "Accept-Encoding";
    header.value = "gzip, deflate, br";
    req.headers.push_back(header);

    HttpResponse res;
    handler.Handle(req, res);

    CHECK(res.statusLine.find("200") != std::string::npos);
    CHECK(res.body == "BROTLI-BYTES");
    CHECK(FindResponseHeader(res, "Content-Encoding") == "br");
    CHECK(FindResponseHeader(res, "Vary") == "Accept-Encoding");
    // Content-Type は元のパスから決めるので圧縮しても変わらない
    CHECK(!FindResponseHeader(res, "Content-Type").empty());
}

TEST(静的配信_AcceptEncodingが無ければ非圧縮を返す)
{
    CTempFile file("hello world");
    CTempSidecar brFile(file.Path(), ".br", "BROTLI-BYTES");
    std::wstring strDir;
    std::string strName;
    SplitDirAndName(file.Path(), strDir, strName);

    CStaticFileHandler handler(strDir, L"", "/");

    HttpRequest req;
    req.method = "GET";
    req.path = "/" + strName;

    HttpResponse res;
    handler.Handle(req, res);

    CHECK(res.statusLine.find("200") != std::string::npos);
    CHECK(res.body == "hello world");
    CHECK(FindResponseHeader(res, "Content-Encoding").empty());
}

TEST(静的配信_brが無ければgzipにフォールバックする)
{
    CTempFile file("hello world");
    CTempSidecar gzFile(file.Path(), ".gz", "GZIP-BYTES");
    std::wstring strDir;
    std::string strName;
    SplitDirAndName(file.Path(), strDir, strName);

    CStaticFileHandler handler(strDir, L"", "/");

    HttpRequest req;
    req.method = "GET";
    req.path = "/" + strName;
    HttpHeader header;
    header.name = "Accept-Encoding";
    header.value = "gzip, deflate, br";
    req.headers.push_back(header);

    HttpResponse res;
    handler.Handle(req, res);

    CHECK(res.body == "GZIP-BYTES");
    CHECK(FindResponseHeader(res, "Content-Encoding") == "gzip");
}

TEST(静的配信_q0で拒否された圧縮は使わない)
{
    CTempFile file("hello world");
    CTempSidecar brFile(file.Path(), ".br", "BROTLI-BYTES");
    std::wstring strDir;
    std::string strName;
    SplitDirAndName(file.Path(), strDir, strName);

    CStaticFileHandler handler(strDir, L"", "/");

    HttpRequest req;
    req.method = "GET";
    req.path = "/" + strName;
    HttpHeader header;
    header.name = "Accept-Encoding";
    header.value = "br;q=0";
    req.headers.push_back(header);

    HttpResponse res;
    handler.Handle(req, res);

    CHECK(res.body == "hello world");
    CHECK(FindResponseHeader(res, "Content-Encoding").empty());
}
