#include "StdAfx.h"
#include "PublicAssetHandler.h"

#include <cstdio>
#include <sstream>
#include <string>
#include <vector>

#include "Web/GrpImageStore.h"
#include "crc.h" // CCRC: マニフェスト ETag 計算用

namespace
{

// resName の検証: 使ってよい文字は A-Z, 0-9, _ のみ。長さは 1〜64。
// 公開エンドポイント（認証なし）なので、SQL への受け渡し前に必ず通す。
// %xx エスケープのデコードは行わない（デコードするとパストラバーサルの入口になる。
// この文字集合チェックだけで弾けば十分）。
bool IsValidResName(const std::string &resName)
{
    if (resName.empty() || resName.size() > 64) {
        return false;
    }
    for (size_t i = 0; i < resName.size(); ++i) {
        unsigned char ch = static_cast<unsigned char>(resName[i]);
        bool bOk = (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || (ch == '_');
        if (!bOk) {
            return false;
        }
    }
    return true;
}

// path からプレフィックス以降（クエリ文字列を除く）を取り出す
std::string ExtractSuffix(const std::string &pathPrefix, const std::string &path)
{
    std::string fullPath = path;
    size_t queryPos = fullPath.find('?');
    if (queryPos != std::string::npos) {
        fullPath = fullPath.substr(0, queryPos);
    }
    if (fullPath.size() <= pathPrefix.size()) {
        return std::string();
    }
    if (fullPath.compare(0, pathPrefix.size(), pathPrefix) != 0) {
        return std::string();
    }
    return fullPath.substr(pathPrefix.size());
}

std::string EscapeJson(const std::string &src)
{
    std::string out;
    out.reserve(src.size());
    for (size_t i = 0; i < src.size(); ++i) {
        unsigned char ch = static_cast<unsigned char>(src[i]);
        switch (ch) {
        case '"':  out += "\\\""; break;
        case '\\': out += "\\\\"; break;
        default:   out += static_cast<char>(ch); break;
        }
    }
    return out;
}

} // namespace

// ---------------------------------------------------------------------------
// CPublicAssetManifestHandler
// ---------------------------------------------------------------------------

void CPublicAssetManifestHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
    std::vector<SGrpSheetSummary> list;
    // GetAllSummaries は DB 未作成でも false を返さず空リストで true を返す。
    CGrpImageStore::GetInstance().GetAllSummaries(list);

    // ETag: 件数 + 各行の resName+revision から安定した文字列を作り CRC32 を取る。
    // png バイト列そのものはここでは扱わない（重いので使わない）。
    std::ostringstream etagSrc;
    etagSrc << list.size();
    for (size_t i = 0; i < list.size(); ++i) {
        etagSrc << '|' << list[i].strResName << ':' << list[i].nRevision;
    }
    std::string etagSrcStr = etagSrc.str();

    CCRC crc;
    DWORD dwCrc = crc.GetCRC(
        reinterpret_cast<PBYTE>(const_cast<char *>(etagSrcStr.data())),
        static_cast<DWORD>(etagSrcStr.size()));

    char szEtag[64];
    std::snprintf(szEtag, sizeof(szEtag), "\"am-%08x-%zu\"", dwCrc, list.size());
    std::string etag = szEtag;

    const char *pszIfNoneMatch = request.FindHeader("If-None-Match");
    if (pszIfNoneMatch != NULL && etag == std::string(pszIfNoneMatch)) {
        response.statusLine = "HTTP/1.1 304 Not Modified";
        response.SetHeader("Cache-Control", "no-cache");
        response.SetHeader("ETag", etag);
        response.SetHeader("Content-Length", "0");
        return;
    }

    std::ostringstream oss;
    oss << "{\"sheets\":[";
    for (size_t i = 0; i < list.size(); ++i) {
        if (i > 0) {
            oss << ',';
        }
        const SGrpSheetSummary &s = list[i];
        oss << '{'
            << "\"resName\":\"" << EscapeJson(s.strResName) << "\","
            << "\"revision\":"  << s.nRevision << ','
            << "\"bytes\":"     << s.nBytes << ','
            << "\"updatedAt\":" << s.nUpdatedAt
            << '}';
    }
    oss << "]}";

    response.statusLine = "HTTP/1.1 200 OK";
    response.SetJsonBody(oss.str());
    response.SetHeader("Cache-Control", "no-cache");
    response.SetHeader("ETag", etag);
}

// ---------------------------------------------------------------------------
// CPublicAssetSpriteHandler
// ---------------------------------------------------------------------------

CPublicAssetSpriteHandler::CPublicAssetSpriteHandler(std::string pathPrefix)
    : m_pathPrefix(std::move(pathPrefix))
{
}

void CPublicAssetSpriteHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
    std::string resName = ExtractSuffix(m_pathPrefix, request.path);
    if (!IsValidResName(resName)) {
        response.statusLine = "HTTP/1.1 400 Bad Request";
        response.SetJsonBody("{\"error\":\"invalid_res_name\"}");
        return;
    }

    // このエンドポイントは「上書き分だけ」を配る（画像ストアのみを見る）。
    // 同梱分は res_name が grp_sheet に存在しないため 404 になる。
    // クライアントは自前で同梱アセットを持っている前提。
    std::vector<unsigned char> png;
    std::string etag;
    if (!CGrpImageStore::GetInstance().GetPng(resName.c_str(), png, etag)) {
        response.statusLine = "HTTP/1.1 404 Not Found";
        response.SetJsonBody("{\"error\":\"not_found\"}");
        return;
    }

    // GetPng は grp_sheet.png の BLOB をそのまま返す（透過加工なし）。
    // 管理用ハンドラのように MakeTransparentPng を通してはならない。

    if (!etag.empty()) {
        const char *pszIfNoneMatch = request.FindHeader("If-None-Match");
        if (pszIfNoneMatch != NULL && etag == std::string(pszIfNoneMatch)) {
            response.statusLine = "HTTP/1.1 304 Not Modified";
            response.SetHeader("Cache-Control", "no-cache");
            response.SetHeader("ETag", etag);
            response.SetHeader("Content-Length", "0");
            return;
        }
    }

    response.statusLine = "HTTP/1.1 200 OK";
    response.body.assign(
        reinterpret_cast<const char *>(png.data()),
        reinterpret_cast<const char *>(png.data()) + png.size());
    response.SetHeader("Content-Type", "image/png");
    response.SetHeader("Cache-Control", "no-cache");
    if (!etag.empty()) {
        response.SetHeader("ETag", etag);
    }
}
