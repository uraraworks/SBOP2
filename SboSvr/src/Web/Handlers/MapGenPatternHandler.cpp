#include "StdAfx.h"
#include "MapGenPatternHandler.h"

#include <sstream>
#include <string>
#include <vector>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "SaveLoad/SaveLoadInfoMapGenPattern.h"

// ---------------------------------------------------------------------------
// 内部ヘルパー（ファイル内 static 共通化）
// ---------------------------------------------------------------------------

namespace
{

// 認証チェック & MgrData null チェック（通っていれば true 返却）
bool CheckAuth(const HttpRequest &request, CMgrData *pMgrData, HttpResponse &response)
{
    AuthProvider::AuthContext authContext;
    AuthProvider::AuthStatus authStatus =
        AuthProvider::Authenticate(request, pMgrData, authContext);
    if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
        response.statusLine = "HTTP/1.1 503 Service Unavailable";
        response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
        return false;
    }
    if (pMgrData == NULL) {
        response.statusLine = "HTTP/1.1 503 Service Unavailable";
        response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
        return false;
    }
    return true;
}

// MapGenPatternRecord → JSON 1 エントリ出力
void AppendPatternJson(std::ostringstream &oss, const MapGenPatternRecord &rec)
{
    oss << '{';
    oss << "\"patternId\":"   << rec.nPatternID                            << ',';
    oss << "\"patternName\":\"" << JsonUtils::Escape(rec.strPatternName) << "\",";
    oss << "\"algoType\":"    << rec.nAlgoType                             << ',';
    // paramsJson / roleMapJson は JSON 文字列として格納されているが、
    // API レスポンス上は文字列値としてそのまま返す（S1: 中身を解釈しない）
    oss << "\"paramsJson\":\""  << JsonUtils::Escape(rec.strParamsJson)  << "\",";
    oss << "\"roleMapJson\":\"" << JsonUtils::Escape(rec.strRoleMapJson) << '"';
    oss << '}';
}

} // namespace

// ---------------------------------------------------------------------------
// GET /api/map-gen-patterns
// ---------------------------------------------------------------------------

CMapGenPatternListHandler::CMapGenPatternListHandler(CMgrData *pMgrData)
    : m_pMgrData(pMgrData)
{
}

void CMapGenPatternListHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
    if (!CheckAuth(request, m_pMgrData, response)) {
        return;
    }

    CSaveLoadInfoMapGenPattern db;
    std::vector<MapGenPatternRecord> records;
    int nCount = db.LoadAll(records);

    if (nCount < 0) {
        response.statusLine = "HTTP/1.1 500 Internal Server Error";
        response.SetJsonBody("{\"error\":\"db_error\"}");
        return;
    }

    std::ostringstream oss;
    oss << "{\"items\":[";
    bool bFirst = true;
    for (size_t i = 0; i < records.size(); ++i) {
        if (!bFirst) { oss << ','; }
        bFirst = false;
        AppendPatternJson(oss, records[i]);
    }
    oss << "]}";

    response.statusLine = "HTTP/1.1 200 OK";
    response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// POST /api/map-gen-patterns
// 必須: patternName, algoType
// 任意: paramsJson, roleMapJson
// 成功時: 201 Created + { patternId }
// ---------------------------------------------------------------------------

CMapGenPatternCreateHandler::CMapGenPatternCreateHandler(CMgrData *pMgrData)
    : m_pMgrData(pMgrData)
{
}

void CMapGenPatternCreateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
    if (!CheckAuth(request, m_pMgrData, response)) {
        return;
    }

    MapGenPatternRecord rec;
    rec.nPatternID   = 0;
    rec.nAlgoType    = 0;

    // patternName は必須
    if (!JsonUtils::TryGetString(request.body, "patternName", rec.strPatternName) ||
        rec.strPatternName.empty()) {
        response.statusLine = "HTTP/1.1 400 Bad Request";
        response.SetJsonBody("{\"error\":\"missing_patternName\"}");
        return;
    }

    JsonUtils::TryGetInt(request.body, "algoType", rec.nAlgoType);
    JsonUtils::TryGetString(request.body, "paramsJson",  rec.strParamsJson);
    JsonUtils::TryGetString(request.body, "roleMapJson", rec.strRoleMapJson);

    CSaveLoadInfoMapGenPattern db;
    int nNewId = 0;
    if (!db.Insert(rec, nNewId)) {
        response.statusLine = "HTTP/1.1 500 Internal Server Error";
        response.SetJsonBody("{\"error\":\"db_error\"}");
        return;
    }

    std::ostringstream oss;
    oss << "{\"patternId\":" << nNewId << "}";

    response.statusLine = "HTTP/1.1 201 Created";
    response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// PUT /api/map-gen-patterns
// 必須: patternId
// 任意: patternName, algoType, paramsJson, roleMapJson
// ---------------------------------------------------------------------------

CMapGenPatternUpdateHandler::CMapGenPatternUpdateHandler(CMgrData *pMgrData)
    : m_pMgrData(pMgrData)
{
}

void CMapGenPatternUpdateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
    if (!CheckAuth(request, m_pMgrData, response)) {
        return;
    }

    int nPatternId = 0;
    if (!JsonUtils::TryGetInt(request.body, "patternId", nPatternId) || nPatternId <= 0) {
        response.statusLine = "HTTP/1.1 400 Bad Request";
        response.SetJsonBody("{\"error\":\"missing_or_invalid_patternId\"}");
        return;
    }

    // まず現在の値を取得して上書きベースで更新する
    CSaveLoadInfoMapGenPattern db;
    std::vector<MapGenPatternRecord> records;
    db.LoadAll(records);

    MapGenPatternRecord *pTarget = NULL;
    for (size_t i = 0; i < records.size(); ++i) {
        if (records[i].nPatternID == nPatternId) {
            pTarget = &records[i];
            break;
        }
    }

    if (pTarget == NULL) {
        response.statusLine = "HTTP/1.1 404 Not Found";
        response.SetJsonBody("{\"error\":\"not_found\"}");
        return;
    }

    // 送られてきたフィールドだけ上書き
    JsonUtils::TryGetString(request.body, "patternName", pTarget->strPatternName);
    JsonUtils::TryGetInt   (request.body, "algoType",    pTarget->nAlgoType);
    JsonUtils::TryGetString(request.body, "paramsJson",  pTarget->strParamsJson);
    JsonUtils::TryGetString(request.body, "roleMapJson", pTarget->strRoleMapJson);

    if (!db.Update(*pTarget)) {
        response.statusLine = "HTTP/1.1 500 Internal Server Error";
        response.SetJsonBody("{\"error\":\"db_error\"}");
        return;
    }

    std::ostringstream oss;
    AppendPatternJson(oss, *pTarget);

    response.statusLine = "HTTP/1.1 200 OK";
    response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// DELETE /api/map-gen-patterns
// 必須: patternId
// ---------------------------------------------------------------------------

CMapGenPatternDeleteHandler::CMapGenPatternDeleteHandler(CMgrData *pMgrData)
    : m_pMgrData(pMgrData)
{
}

void CMapGenPatternDeleteHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
    if (!CheckAuth(request, m_pMgrData, response)) {
        return;
    }

    int nPatternId = 0;
    if (!JsonUtils::TryGetInt(request.body, "patternId", nPatternId) || nPatternId <= 0) {
        response.statusLine = "HTTP/1.1 400 Bad Request";
        response.SetJsonBody("{\"error\":\"missing_or_invalid_patternId\"}");
        return;
    }

    CSaveLoadInfoMapGenPattern db;
    if (!db.Delete(nPatternId)) {
        response.statusLine = "HTTP/1.1 404 Not Found";
        response.SetJsonBody("{\"error\":\"not_found\"}");
        return;
    }

    std::ostringstream oss;
    oss << "{\"deleted\":" << nPatternId << "}";

    response.statusLine = "HTTP/1.1 200 OK";
    response.SetJsonBody(oss.str());
}
