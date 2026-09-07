#include "StdAfx.h"
#include "GrpDraftHandler.h"

#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

#include "Web/AuthProvider.h"
#include "Web/GrpImageStore.h"
#include "Web/JsonUtils.h"

namespace
{

// 下書き 1 件の PNG は 32x32 のパレット画像で 1KB 前後。
// 壊れた/悪意あるリクエストで巨大な blob を DB に入れないよう上限を設ける。
// （HTTP のボディ上限 64KB より内側なので実質の歯止めはこちら）
const size_t kMaxDraftPngBytes = 48 * 1024;

// 名前が無制限に長いと一覧が壊れるので切る（UTF-8 バイト数）
const size_t kMaxDraftNameBytes = 200;

// ---------------------------------------------------------------------------
// Base64
// PNG はバイナリなので JSON に載せるために base64 を通す。
// このプロジェクトには共通のデコーダが無いため最小実装を置く。
// ---------------------------------------------------------------------------

const char *const kBase64Chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string Base64Encode(const unsigned char *pData, size_t nLength)
{
    std::string out;
    out.reserve(((nLength + 2) / 3) * 4);

    size_t i = 0;
    while (i + 2 < nLength) {
        unsigned int nTriple = (pData[i] << 16) | (pData[i + 1] << 8) | pData[i + 2];
        out.push_back(kBase64Chars[(nTriple >> 18) & 0x3F]);
        out.push_back(kBase64Chars[(nTriple >> 12) & 0x3F]);
        out.push_back(kBase64Chars[(nTriple >> 6)  & 0x3F]);
        out.push_back(kBase64Chars[nTriple & 0x3F]);
        i += 3;
    }

    if (i < nLength) {
        unsigned int nTriple = pData[i] << 16;
        bool bHasSecond = (i + 1 < nLength);
        if (bHasSecond) {
            nTriple |= (pData[i + 1] << 8);
        }
        out.push_back(kBase64Chars[(nTriple >> 18) & 0x3F]);
        out.push_back(kBase64Chars[(nTriple >> 12) & 0x3F]);
        out.push_back(bHasSecond ? kBase64Chars[(nTriple >> 6) & 0x3F] : '=');
        out.push_back('=');
    }
    return out;
}

int Base64Value(char ch)
{
    if (ch >= 'A' && ch <= 'Z') return ch - 'A';
    if (ch >= 'a' && ch <= 'z') return ch - 'a' + 26;
    if (ch >= '0' && ch <= '9') return ch - '0' + 52;
    if (ch == '+') return 62;
    if (ch == '/') return 63;
    return -1;  // '=' や空白を含め、それ以外は無効
}

// 成功したら true。空白は読み飛ばし、'=' 以降は終端とみなす。
bool Base64Decode(const std::string &text, std::vector<unsigned char> &outData)
{
    outData.clear();
    outData.reserve((text.size() / 4) * 3);

    unsigned int nBuffer = 0;
    int nBits = 0;
    for (size_t i = 0; i < text.size(); ++i) {
        char ch = text[i];
        if (ch == '=') {
            break;
        }
        if (ch == '\r' || ch == '\n' || ch == ' ' || ch == '\t') {
            continue;
        }
        int nValue = Base64Value(ch);
        if (nValue < 0) {
            return false;
        }
        nBuffer = (nBuffer << 6) | static_cast<unsigned int>(nValue);
        nBits += 6;
        if (nBits >= 8) {
            nBits -= 8;
            outData.push_back(static_cast<unsigned char>((nBuffer >> nBits) & 0xFF));
        }
    }
    return true;
}

// ---------------------------------------------------------------------------

void SendJsonError(HttpResponse &response, const char *pszStatusLine, const char *pszError)
{
    response.statusLine = pszStatusLine;
    std::string body = "{\"error\":\"";
    body += pszError;
    body += "\"}";
    response.SetJsonBody(body);
}

// IMAGE_EDIT を要求する共通の認証チェック（SpriteSheetHandler と同じ作法）
bool AuthorizeImageEdit(const HttpRequest &request, CMgrData *pMgrData,
                        AuthProvider::AuthContext &outContext, HttpResponse &response)
{
    AuthProvider::AuthStatus authStatus =
        AuthProvider::Authenticate(request, pMgrData, outContext);
    if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
        SendJsonError(response, "HTTP/1.1 503 Service Unavailable", "backend_unavailable");
        return false;
    }
    if (authStatus != AuthProvider::AuthStatusOk) {
        SendJsonError(response, "HTTP/1.1 401 Unauthorized", "unauthorized");
        return false;
    }
    if (!AuthProvider::HasRole(outContext, "IMAGE_EDIT")) {
        response.statusLine = "HTTP/1.1 403 Forbidden";
        response.SetJsonBody(AuthProvider::BuildForbiddenBody("IMAGE_EDIT"));
        return false;
    }
    return true;
}

// リクエストボディから下書きのメタと PNG を取り出す。
// 失敗したらエラー名を outError に入れて false。
bool ParseDraftBody(const std::string &body, SGrpDraftEntry &outMeta,
                    std::vector<unsigned char> &outPng, std::string &outError)
{
    std::string pngBase64;
    if (!JsonUtils::TryGetString(body, "png", pngBase64) || pngBase64.empty()) {
        outError = "png_required";
        return false;
    }
    if (!Base64Decode(pngBase64, outPng) || outPng.empty()) {
        outError = "invalid_base64";
        return false;
    }
    if (outPng.size() > kMaxDraftPngBytes) {
        outError = "png_too_large";
        return false;
    }

    if (!JsonUtils::TryGetString(body, "catKey", outMeta.strCatKey) || outMeta.strCatKey.empty()) {
        outError = "cat_key_required";
        return false;
    }

    JsonUtils::TryGetString(body, "name", outMeta.strName);
    if (outMeta.strName.empty()) {
        outMeta.strName = "(名前なし)";
    }
    if (outMeta.strName.size() > kMaxDraftNameBytes) {
        outMeta.strName.resize(kMaxDraftNameBytes);
    }

    outMeta.nSheetIndex = 0;
    outMeta.nCol = 0;
    outMeta.nRow = 0;
    outMeta.nWidth = 0;
    outMeta.nHeight = 0;
    JsonUtils::TryGetInt(body, "sheetIndex", outMeta.nSheetIndex);
    JsonUtils::TryGetInt(body, "col", outMeta.nCol);
    JsonUtils::TryGetInt(body, "row", outMeta.nRow);
    JsonUtils::TryGetInt(body, "width", outMeta.nWidth);
    JsonUtils::TryGetInt(body, "height", outMeta.nHeight);

    if (outMeta.nSheetIndex < 0 || outMeta.nCol < 0 || outMeta.nRow < 0) {
        outError = "invalid_position";
        return false;
    }
    return true;
}

// 一覧・単体で共通のメタ部分を書き出す（末尾にカンマは付けない）
void AppendDraftMetaJson(std::ostringstream &oss, const SGrpDraftEntry &entry)
{
    oss << "\"id\":"          << entry.nId << ','
        << "\"name\":\""      << JsonUtils::Escape(entry.strName)   << "\","
        << "\"catKey\":\""    << JsonUtils::Escape(entry.strCatKey) << "\","
        << "\"sheetIndex\":"  << entry.nSheetIndex << ','
        << "\"col\":"         << entry.nCol << ','
        << "\"row\":"         << entry.nRow << ','
        << "\"width\":"       << entry.nWidth << ','
        << "\"height\":"      << entry.nHeight << ','
        << "\"updatedAt\":"   << entry.nUpdatedAt << ','
        << "\"updatedBy\":\"" << JsonUtils::Escape(entry.strUpdatedBy) << "\","
        << "\"bytes\":"       << static_cast<unsigned long long>(entry.nBytes);
}

} // namespace

// ---------------------------------------------------------------------------

CGrpDraftHandler::CGrpDraftHandler(std::string pathPrefix, CMgrData *pMgrData)
    : m_pathPrefix(pathPrefix)
    , m_pMgrData(pMgrData)
{
}

long long CGrpDraftHandler::ParseIdFromPath(const std::string &path) const
{
    // クエリ文字列は使わないが、付いていても無視できるように落としておく
    std::string cleanPath = path;
    size_t nQuery = cleanPath.find('?');
    if (nQuery != std::string::npos) {
        cleanPath = cleanPath.substr(0, nQuery);
    }

    if (cleanPath.size() < m_pathPrefix.size()) {
        return -1;
    }
    std::string rest = cleanPath.substr(m_pathPrefix.size());
    // "" と "/" は一覧・新規作成
    if (rest.empty() || rest == "/") {
        return 0;
    }
    if (rest[0] != '/') {
        return -1;
    }
    rest = rest.substr(1);
    if (rest.empty()) {
        return 0;
    }
    for (size_t i = 0; i < rest.size(); ++i) {
        if (rest[i] < '0' || rest[i] > '9') {
            return -1;
        }
    }
    // rest は上のループで数字のみと確認済みなので、strtoll の失敗系(errno/終端ポインタ)は
    // 見なくてよい。_atoi64 と同じく素直に数値へ変換するだけ。
    return strtoll(rest.c_str(), nullptr, 10);
}

void CGrpDraftHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
    AuthProvider::AuthContext authContext;
    if (!AuthorizeImageEdit(request, m_pMgrData, authContext, response)) {
        return;
    }

    long long nId = ParseIdFromPath(request.path);
    if (nId < 0) {
        SendJsonError(response, "HTTP/1.1 404 Not Found", "draft_not_found");
        return;
    }

    if (request.method == "GET") {
        if (nId == 0) {
            HandleList(request, response);
        } else {
            HandleGetOne(nId, response);
        }
        return;
    }
    if (request.method == "POST") {
        HandleCreate(request, response, authContext.loginId);
        return;
    }
    if (request.method == "PUT") {
        if (nId == 0) {
            SendJsonError(response, "HTTP/1.1 400 Bad Request", "draft_id_required");
            return;
        }
        HandleUpdate(nId, request, response, authContext.loginId);
        return;
    }
    if (request.method == "DELETE") {
        if (nId == 0) {
            SendJsonError(response, "HTTP/1.1 400 Bad Request", "draft_id_required");
            return;
        }
        HandleDelete(nId, response);
        return;
    }

    SendJsonError(response, "HTTP/1.1 405 Method Not Allowed", "method_not_allowed");
}

void CGrpDraftHandler::HandleList(const HttpRequest & /*request*/, HttpResponse &response)
{
    std::vector<SGrpDraftEntry> drafts;
    if (!CGrpImageStore::GetInstance().ListDrafts(drafts)) {
        SendJsonError(response, "HTTP/1.1 500 Internal Server Error", "list_failed");
        return;
    }

    std::ostringstream oss;
    oss << "{\"drafts\":[";
    for (size_t i = 0; i < drafts.size(); ++i) {
        if (i > 0) {
            oss << ',';
        }
        oss << '{';
        AppendDraftMetaJson(oss, drafts[i]);
        oss << '}';
    }
    oss << "]}";

    response.statusLine = "HTTP/1.1 200 OK";
    response.SetJsonBody(oss.str());
}

void CGrpDraftHandler::HandleGetOne(long long nId, HttpResponse &response)
{
    SGrpDraftEntry meta;
    std::vector<unsigned char> png;
    if (!CGrpImageStore::GetInstance().GetDraft(nId, meta, png)) {
        SendJsonError(response, "HTTP/1.1 404 Not Found", "draft_not_found");
        return;
    }

    std::ostringstream oss;
    oss << "{\"draft\":{";
    AppendDraftMetaJson(oss, meta);
    oss << ",\"png\":\"" << Base64Encode(png.data(), png.size()) << "\"";
    oss << "}}";

    response.statusLine = "HTTP/1.1 200 OK";
    response.SetJsonBody(oss.str());
}

void CGrpDraftHandler::HandleCreate(const HttpRequest &request, HttpResponse &response,
                                    const std::string &updatedBy)
{
    SGrpDraftEntry meta;
    std::vector<unsigned char> png;
    std::string parseError;
    if (!ParseDraftBody(request.body, meta, png, parseError)) {
        SendJsonError(response, "HTTP/1.1 400 Bad Request", parseError.c_str());
        return;
    }
    meta.strUpdatedBy = updatedBy;

    long long nNewId = 0;
    std::string storeError;
    if (!CGrpImageStore::GetInstance().CreateDraft(meta, png.data(), png.size(), nNewId, storeError)) {
        SendJsonError(response, "HTTP/1.1 500 Internal Server Error", storeError.c_str());
        return;
    }

    std::ostringstream oss;
    oss << "{\"id\":" << nNewId << "}";
    response.statusLine = "HTTP/1.1 200 OK";
    response.SetJsonBody(oss.str());
}

void CGrpDraftHandler::HandleUpdate(long long nId, const HttpRequest &request,
                                    HttpResponse &response, const std::string &updatedBy)
{
    SGrpDraftEntry meta;
    std::vector<unsigned char> png;
    std::string parseError;
    if (!ParseDraftBody(request.body, meta, png, parseError)) {
        SendJsonError(response, "HTTP/1.1 400 Bad Request", parseError.c_str());
        return;
    }
    meta.strUpdatedBy = updatedBy;

    std::string storeError;
    if (!CGrpImageStore::GetInstance().UpdateDraft(nId, meta, png.data(), png.size(), storeError)) {
        if (storeError == "draft_not_found") {
            SendJsonError(response, "HTTP/1.1 404 Not Found", "draft_not_found");
        } else {
            SendJsonError(response, "HTTP/1.1 500 Internal Server Error", storeError.c_str());
        }
        return;
    }

    std::ostringstream oss;
    oss << "{\"id\":" << nId << "}";
    response.statusLine = "HTTP/1.1 200 OK";
    response.SetJsonBody(oss.str());
}

void CGrpDraftHandler::HandleDelete(long long nId, HttpResponse &response)
{
    std::string storeError;
    if (!CGrpImageStore::GetInstance().DeleteDraft(nId, storeError)) {
        SendJsonError(response, "HTTP/1.1 500 Internal Server Error", storeError.c_str());
        return;
    }
    response.statusLine = "HTTP/1.1 200 OK";
    response.SetJsonBody("{\"ok\":true}");
}
