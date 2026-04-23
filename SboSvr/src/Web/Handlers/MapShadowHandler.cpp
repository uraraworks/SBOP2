#include "stdafx.h"
#include "MapShadowHandler.h"

#include <sstream>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "LibInfo/LibInfoMapShadow.h"
#include "Info/InfoMapShadow.h"
#include "Info/InfoAnime.h"
#include "UraraSockTCPSBO.h"
#include "Packet/MAP/PacketMAP_MAPSHADOW.h"
#include "Packet/MAP/PacketMAP_DELETEMAPSHADOW.h"

namespace
{

// クエリ文字列から指定キーの値を取得する
bool TryGetQueryParam(const std::string &path, const std::string &key, int &outValue)
{
        size_t nQueryPos = path.find('?');
        if (nQueryPos == std::string::npos) {
                return false;
        }

        size_t nPos = nQueryPos + 1;
        while (nPos < path.size()) {
                size_t nAmp = path.find('&', nPos);
                size_t nEnd = (nAmp == std::string::npos) ? path.size() : nAmp;
                size_t nEqual = path.find('=', nPos);
                if ((nEqual != std::string::npos) && (nEqual < nEnd)) {
                        std::string k = path.substr(nPos, nEqual - nPos);
                        if (k == key) {
                                std::string v = path.substr(nEqual + 1, nEnd - (nEqual + 1));
                                if (!v.empty()) {
                                        char *pEnd = NULL;
                                        long val = std::strtol(v.c_str(), &pEnd, 10);
                                        if (pEnd != NULL && *pEnd == '\0') {
                                                outValue = static_cast<int>(val);
                                                return true;
                                        }
                                }
                        }
                }
                if (nAmp == std::string::npos) {
                        break;
                }
                nPos = nAmp + 1;
        }
        return false;
}

// アニメーションコマ配列を JSON に出力する
void AppendAnimeFramesJson(std::ostringstream &oss, const CInfoMapShadow *pInfo)
{
        oss << "\"animeFrames\":[";
        int nCount = pInfo->m_paAnimeInfo ? static_cast<int>(pInfo->m_paAnimeInfo->GetSize()) : 0;
        for (int i = 0; i < nCount; ++i) {
                const PCInfoAnime pAnime = pInfo->m_paAnimeInfo->GetAt(i);
                if (pAnime == NULL) {
                        continue;
                }
                if (i > 0) {
                        oss << ',';
                }
                oss << '{';
                oss << "\"wait\":" << static_cast<int>(pAnime->m_byWait) << ',';
                oss << "\"level\":" << static_cast<int>(pAnime->m_byLevel) << ',';
                oss << "\"grpIdBase\":" << static_cast<int>(pAnime->m_wGrpIDBase) << ',';
                oss << "\"grpIdPile\":" << static_cast<int>(pAnime->m_wGrpIDPile);
                oss << '}';
        }
        oss << ']';
}

// 影 1 件分を JSON に出力する
void AppendShadowJson(std::ostringstream &oss, const CInfoMapShadow *pInfo)
{
        oss << '{';
        oss << "\"id\":" << pInfo->m_dwShadowID << ',';
        oss << "\"viewType\":" << static_cast<int>(pInfo->m_byViewType) << ',';
        oss << "\"animeType\":" << static_cast<int>(pInfo->m_byAnimeType) << ',';
        oss << "\"animeCount\":" << static_cast<int>(pInfo->m_byAnimeCount) << ',';
        oss << "\"level\":" << static_cast<int>(pInfo->m_byLevel) << ',';
        oss << "\"light\":" << (pInfo->m_bLight ? "true" : "false") << ',';
        oss << "\"grpId\":" << static_cast<int>(pInfo->m_wGrpID) << ',';
        oss << "\"viewPos\":{\"x\":" << pInfo->m_ptViewPos.x << ",\"y\":" << pInfo->m_ptViewPos.y << "},";
        AppendAnimeFramesJson(oss, pInfo);
        oss << '}';
}

// JSON からフィールドを pInfo に適用する
void ApplyJsonToShadow(const std::string &json, CInfoMapShadow *pInfo)
{
        int nVal = 0;
        bool bVal = false;

        if (JsonUtils::TryGetInt(json, "viewType", nVal)) {
                pInfo->m_byViewType = static_cast<BYTE>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "animeType", nVal)) {
                pInfo->m_byAnimeType = static_cast<BYTE>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "animeCount", nVal)) {
                pInfo->m_byAnimeCount = static_cast<BYTE>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "level", nVal)) {
                pInfo->m_byLevel = static_cast<BYTE>(nVal);
        }
        if (JsonUtils::TryGetBool(json, "light", bVal)) {
                pInfo->m_bLight = bVal ? TRUE : FALSE;
        }
        if (JsonUtils::TryGetInt(json, "grpId", nVal)) {
                pInfo->m_wGrpID = static_cast<WORD>(nVal);
        }

        // viewPos オブジェクトのパース
        size_t nPosKey = JsonUtils::FindKey(json, "viewPos");
        if (nPosKey != std::string::npos) {
                size_t nBrace = json.find('{', nPosKey);
                size_t nClose = json.find('}', nBrace);
                if (nBrace != std::string::npos && nClose != std::string::npos) {
                        std::string posJson = json.substr(nBrace, nClose - nBrace + 1);
                        int nx = 0, ny = 0;
                        if (JsonUtils::TryGetInt(posJson, "x", nx)) { pInfo->m_ptViewPos.x = nx; }
                        if (JsonUtils::TryGetInt(posJson, "y", ny)) { pInfo->m_ptViewPos.y = ny; }
                }
        }

        // animeFrames 配列のパース（"animeFrames":[{...},{...}]）
        size_t nFramesKey = JsonUtils::FindKey(json, "animeFrames");
        if (nFramesKey == std::string::npos) {
                return;
        }
        size_t nArrayStart = json.find('[', nFramesKey);
        size_t nArrayEnd = json.find(']', nArrayStart);
        if (nArrayStart == std::string::npos || nArrayEnd == std::string::npos) {
                return;
        }

        // 既存コマをクリアして再構築
        pInfo->DeleteAllAnime();

        size_t nPos = nArrayStart + 1;
        while (nPos < nArrayEnd) {
                size_t nObjStart = json.find('{', nPos);
                if (nObjStart == std::string::npos || nObjStart >= nArrayEnd) {
                        break;
                }
                size_t nObjEnd = json.find('}', nObjStart);
                if (nObjEnd == std::string::npos || nObjEnd > nArrayEnd) {
                        break;
                }
                std::string frameJson = json.substr(nObjStart, nObjEnd - nObjStart + 1);

                pInfo->AddAnime();
                int nIdx = pInfo->GetAnimeCount() - 1;
                PCInfoAnime pAnime = pInfo->GetAnimePtr(nIdx);
                if (pAnime != NULL) {
                        if (JsonUtils::TryGetInt(frameJson, "wait", nVal)) {
                                pAnime->m_byWait = static_cast<BYTE>(nVal);
                        }
                        if (JsonUtils::TryGetInt(frameJson, "level", nVal)) {
                                pAnime->m_byLevel = static_cast<BYTE>(nVal);
                        }
                        if (JsonUtils::TryGetInt(frameJson, "grpIdBase", nVal)) {
                                pAnime->m_wGrpIDBase = static_cast<WORD>(nVal);
                        }
                        if (JsonUtils::TryGetInt(frameJson, "grpIdPile", nVal)) {
                                pAnime->m_wGrpIDPile = static_cast<WORD>(nVal);
                        }
                }

                nPos = nObjEnd + 1;
        }
}

} // namespace

// ---------------------------------------------------------------------------
// CMapShadowListHandler  GET /api/maps/shadows
// ---------------------------------------------------------------------------

CMapShadowListHandler::CMapShadowListHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapShadowListHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(BuildResponseJson());
}

std::string CMapShadowListHandler::BuildResponseJson() const
{
        if (m_pMgrData == NULL) {
                return "{\"shadows\":[]}";
        }

        CLibInfoMapShadow *pLib = m_pMgrData->GetLibInfoMapShadow();
        if (pLib == NULL) {
                return "{\"shadows\":[]}";
        }

        pLib->Enter();

        std::ostringstream oss;
        oss << "{\"shadows\":[";
        bool first = true;
        int nCount = pLib->GetCount();
        for (int i = 0; i < nCount; ++i) {
                const CInfoMapShadow *pInfo = static_cast<const CInfoMapShadow *>(pLib->GetPtr(i));
                if (pInfo == NULL) {
                        continue;
                }
                if (!first) {
                        oss << ',';
                }
                first = false;
                AppendShadowJson(oss, pInfo);
        }
        oss << "]}";

        pLib->Leave();
        return oss.str();
}

// ---------------------------------------------------------------------------
// CMapShadowCreateHandler  POST /api/maps/shadows
// ---------------------------------------------------------------------------

CMapShadowCreateHandler::CMapShadowCreateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapShadowCreateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        if (m_pMgrData == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        CLibInfoMapShadow *pLib = m_pMgrData->GetLibInfoMapShadow();
        CUraraSockTCPSBO  *pSock = m_pMgrData->GetSock();
        if (pLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pLib->Enter();

        PCInfoMapShadow pNew = static_cast<PCInfoMapShadow>(pLib->GetNew());
        if (pNew == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"failed_to_create_shadow\"}");
                return;
        }

        ApplyJsonToShadow(request.body, pNew);
        pLib->Add(pNew);

        // クライアントへ通知（RecvProcADMIN_RENEWMAPSHADOW と同じパターン）
        CPacketMAP_MAPSHADOW packet;
        packet.Make(pNew);
        pSock->SendTo(0, &packet);

        std::ostringstream oss;
        AppendShadowJson(oss, pNew);

        pLib->Leave();

        response.statusLine = "HTTP/1.1 201 Created";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// CMapShadowUpdateHandler  PUT /api/maps/shadows
// ---------------------------------------------------------------------------

CMapShadowUpdateHandler::CMapShadowUpdateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapShadowUpdateHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        if (m_pMgrData == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        CLibInfoMapShadow *pLib = m_pMgrData->GetLibInfoMapShadow();
        CUraraSockTCPSBO  *pSock = m_pMgrData->GetSock();
        if (pLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nId = 0;
        if (!JsonUtils::TryGetInt(request.body, "id", nId) || nId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_id\"}");
                return;
        }

        pLib->Enter();

        PCInfoMapShadow pInfo = static_cast<PCInfoMapShadow>(pLib->GetPtr(static_cast<DWORD>(nId)));
        if (pInfo == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"shadow_not_found\"}");
                return;
        }

        ApplyJsonToShadow(request.body, pInfo);

        // クライアントへ通知（RecvProcADMIN_RENEWMAPSHADOW と同じパターン）
        CPacketMAP_MAPSHADOW packet;
        packet.Make(pInfo);
        pSock->SendTo(0, &packet);

        std::ostringstream oss;
        AppendShadowJson(oss, pInfo);

        pLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// CMapShadowDeleteHandler  DELETE /api/maps/shadows?id=<id>
// ---------------------------------------------------------------------------

CMapShadowDeleteHandler::CMapShadowDeleteHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapShadowDeleteHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        if (m_pMgrData == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        CLibInfoMapShadow *pLib = m_pMgrData->GetLibInfoMapShadow();
        CUraraSockTCPSBO  *pSock = m_pMgrData->GetSock();
        if (pLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nId = 0;
        if (!TryGetQueryParam(request.path, "id", nId) || nId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_id\"}");
                return;
        }

        DWORD dwId = static_cast<DWORD>(nId);

        pLib->Enter();

        if (pLib->GetPtr(dwId) == NULL) {
                pLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"shadow_not_found\"}");
                return;
        }

        pLib->Delete(dwId);

        // クライアントへ通知（RecvProcADMIN_MAP_DELETEMAPSHADOW と同じパターン）
        CPacketMAP_DELETEMAPSHADOW packet;
        packet.Make(dwId);
        pSock->SendTo(0, &packet);

        pLib->Leave();

        std::ostringstream oss;
        oss << "{\"deleted\":" << dwId << "}";
        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}
