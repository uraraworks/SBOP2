#include "stdafx.h"
#include "SelectionHandler.h"

#include <cstdio>
#include <ctime>
#include <sstream>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"

// ============================================================
// CSelectionStore
// ============================================================

CSelectionStore &CSelectionStore::GetInstance()
{
    static CSelectionStore s_instance;
    return s_instance;
}

void CSelectionStore::Set(const std::string &sessionId, const Selection &selection)
{
    ChangeCallback cb;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_store[sessionId] = selection;
        cb = m_changeCallback;
    }
    // コールバックはロック外で呼ぶ（デッドロック防止）
    if (cb) {
        cb(sessionId, &selection);
    }
}

bool CSelectionStore::Get(const std::string &sessionId, Selection &outSelection) const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::map<std::string, Selection>::const_iterator it = m_store.find(sessionId);
    if (it == m_store.end()) {
        return false;
    }
    outSelection = it->second;
    return true;
}

void CSelectionStore::Clear(const std::string &sessionId)
{
    ChangeCallback cb;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_store.erase(sessionId);
        cb = m_changeCallback;
    }
    // コールバックはロック外で呼ぶ（デッドロック防止）
    if (cb) {
        cb(sessionId, NULL);
    }
}

void CSelectionStore::SetChangeCallback(ChangeCallback cb)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_changeCallback = cb;
}

// ============================================================
// CSelectionPickHandler — POST /api/selection/pick
// ============================================================

CSelectionPickHandler::CSelectionPickHandler(CMgrData *pMgrData)
    : m_pMgrData(pMgrData)
{
}

void CSelectionPickHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
    // 認証チェック
    AuthProvider::AuthContext authContext;
    AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
    if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
        response.statusLine = "HTTP/1.1 503 Service Unavailable";
        response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
        return;
    }
    if (authStatus != AuthProvider::AuthStatusOk) {
        response.statusLine = "HTTP/1.1 401 Unauthorized";
        response.SetJsonBody("{\"error\":\"unauthorized\"}");
        return;
    }
    // TODO: 管理者ロール要件の詳細は未決。現状は認証済みなら誰でも許可。
    //       必要に応じて AuthProvider::HasRole(authContext, "MAP_OBJECT_VIEW") 等を追加すること。

    Selection selection;
    if (!ParseSelection(request.body, selection)) {
        response.statusLine = "HTTP/1.1 400 Bad Request";
        response.SetJsonBody("{\"error\":\"invalid_request_body\"}");
        return;
    }

    selection.updatedAt = GetCurrentIso8601();
    CSelectionStore::GetInstance().Set(authContext.sessionId, selection);

    std::ostringstream oss;
    oss << "{\"selection\":" << BuildSelectionJson(selection) << "}";
    response.statusLine = "HTTP/1.1 200 OK";
    response.SetJsonBody(oss.str());
}

bool CSelectionPickHandler::ParseSelection(const std::string &body, Selection &outSelection) const
{
    // type フィールド（必須）
    std::string typeStr;
    if (!JsonUtils::TryGetString(body, "type", typeStr)) {
        return false;
    }
    if (typeStr == "map_cell") {
        outSelection.type = SelectionTypeMapCell;
    } else if (typeStr == "char") {
        outSelection.type = SelectionTypeChar;
    } else if (typeStr == "placement") {
        outSelection.type = SelectionTypePlacement;
    } else if (typeStr == "event") {
        outSelection.type = SelectionTypeEvent;
    } else {
        return false;
    }

    // mapId, x, y（必須）
    if (!JsonUtils::TryGetInt(body, "mapId", outSelection.nMapId)) {
        return false;
    }
    if (!JsonUtils::TryGetInt(body, "x", outSelection.nX)) {
        return false;
    }
    if (!JsonUtils::TryGetInt(body, "y", outSelection.nY)) {
        return false;
    }

    // refId（省略可）
    outSelection.bHasRefId = false;
    outSelection.nRefId    = 0;
    if (JsonUtils::FindKey(body, "refId") != std::string::npos) {
        if (!JsonUtils::IsNull(body, "refId")) {
            if (!JsonUtils::TryGetInt(body, "refId", outSelection.nRefId)) {
                return false;
            }
            outSelection.bHasRefId = true;
        }
    }

    return true;
}

std::string CSelectionPickHandler::BuildSelectionJson(const Selection &selection) const
{
    static const char *const s_typeNames[] = {
        "map_cell",
        "char",
        "placement",
        "event",
    };

    std::ostringstream oss;
    oss << '{';
    oss << "\"type\":\"" << JsonUtils::Escape(s_typeNames[static_cast<int>(selection.type)]) << "\",";
    oss << "\"mapId\":" << selection.nMapId << ",";
    oss << "\"x\":" << selection.nX << ",";
    oss << "\"y\":" << selection.nY << ",";
    if (selection.bHasRefId) {
        oss << "\"refId\":" << selection.nRefId << ",";
    } else {
        oss << "\"refId\":null,";
    }
    oss << "\"updatedAt\":\"" << JsonUtils::Escape(selection.updatedAt) << "\"";
    oss << '}';
    return oss.str();
}

std::string CSelectionPickHandler::GetCurrentIso8601() const
{
    time_t now = time(NULL);
    struct tm tmBuf;
#if defined(_WIN32)
    gmtime_s(&tmBuf, &now);
#else
    gmtime_r(&now, &tmBuf);
#endif
    char szBuf[32];
    std::strftime(szBuf, sizeof(szBuf), "%Y-%m-%dT%H:%M:%SZ", &tmBuf);
    return std::string(szBuf);
}

// ============================================================
// CSelectionGetHandler — GET /api/selection
// ============================================================

CSelectionGetHandler::CSelectionGetHandler(CMgrData *pMgrData)
    : m_pMgrData(pMgrData)
{
}

void CSelectionGetHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
    // 認証チェック
    AuthProvider::AuthContext authContext;
    AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
    if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
        response.statusLine = "HTTP/1.1 503 Service Unavailable";
        response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
        return;
    }
    if (authStatus != AuthProvider::AuthStatusOk) {
        response.statusLine = "HTTP/1.1 401 Unauthorized";
        response.SetJsonBody("{\"error\":\"unauthorized\"}");
        return;
    }

    Selection selection;
    if (!CSelectionStore::GetInstance().Get(authContext.sessionId, selection)) {
        // 選択なし
        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody("{\"selection\":null}");
        return;
    }

    std::ostringstream oss;
    oss << "{\"selection\":" << BuildSelectionJson(selection) << "}";
    response.statusLine = "HTTP/1.1 200 OK";
    response.SetJsonBody(oss.str());
}

std::string CSelectionGetHandler::BuildSelectionJson(const Selection &selection) const
{
    static const char *const s_typeNames[] = {
        "map_cell",
        "char",
        "placement",
        "event",
    };

    std::ostringstream oss;
    oss << '{';
    oss << "\"type\":\"" << JsonUtils::Escape(s_typeNames[static_cast<int>(selection.type)]) << "\",";
    oss << "\"mapId\":" << selection.nMapId << ",";
    oss << "\"x\":" << selection.nX << ",";
    oss << "\"y\":" << selection.nY << ",";
    if (selection.bHasRefId) {
        oss << "\"refId\":" << selection.nRefId << ",";
    } else {
        oss << "\"refId\":null,";
    }
    oss << "\"updatedAt\":\"" << JsonUtils::Escape(selection.updatedAt) << "\"";
    oss << '}';
    return oss.str();
}

// ============================================================
// CSelectionDeleteHandler — DELETE /api/selection
// ============================================================

CSelectionDeleteHandler::CSelectionDeleteHandler(CMgrData *pMgrData)
    : m_pMgrData(pMgrData)
{
}

void CSelectionDeleteHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
    // 認証チェック
    AuthProvider::AuthContext authContext;
    AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
    if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
        response.statusLine = "HTTP/1.1 503 Service Unavailable";
        response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
        return;
    }
    if (authStatus != AuthProvider::AuthStatusOk) {
        response.statusLine = "HTTP/1.1 401 Unauthorized";
        response.SetJsonBody("{\"error\":\"unauthorized\"}");
        return;
    }

    CSelectionStore::GetInstance().Clear(authContext.sessionId);

    response.statusLine = "HTTP/1.1 200 OK";
    response.SetJsonBody("{\"cleared\":true}");
}
