#include "stdafx.h"
#include "MapEventHandler.h"

#include <sstream>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "LibInfo/LibInfoMapBase.h"
#include "LibInfo/LibInfoMapEvent.h"
#include "Info/InfoMapBase.h"
#include "Info/InfoMapEvent/InfoMapEvent.h"
#include "UraraSockTCPSBO.h"
#include "Packet/MAP/PacketMAP_MAPEVENT.h"
#include "Packet/MAP/PacketMAP_PARA1.h"
#include "Packet/Command.h"

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

// イベント種別ラベルを返す
const char *GetMapEventTypeLabel(int nType)
{
        switch (nType) {
        case MAPEVENTTYPE_NONE:        return "NONE";
        case MAPEVENTTYPE_MOVE:        return "MOVE";
        case MAPEVENTTYPE_MAPMOVE:     return "MAPMOVE";
        case MAPEVENTTYPE_TRASHBOX:    return "TRASHBOX";
        case MAPEVENTTYPE_INITSTATUS:  return "INITSTATUS";
        case MAPEVENTTYPE_GRPIDTMP:    return "GRPIDTMP";
        case MAPEVENTTYPE_LIGHT:       return "LIGHT";
        default:                       return "UNKNOWN";
        }
}

// 当たり判定種別ラベルを返す
const char *GetMapEventHitTypeLabel(int nHitType)
{
        switch (nHitType) {
        case MAPEVENTHITTYPE_MAPPOS:   return "MAPPOS";
        case MAPEVENTHITTYPE_CHARPOS:  return "CHARPOS";
        case MAPEVENTHITTYPE_AREA:     return "AREA";
        case MAPEVENTHITTYPE_MAPPOS2:  return "MAPPOS2";
        default:                       return "UNKNOWN";
        }
}

// 種別依存フィールドを JSON に出力する
void AppendDetailJson(std::ostringstream &oss, const CInfoMapEventBase *pEvent)
{
        oss << "\"detail\":{";

        switch (pEvent->m_nType) {
        case MAPEVENTTYPE_MOVE: {
                const CInfoMapEventMOVE *p = static_cast<const CInfoMapEventMOVE *>(pEvent);
                oss << "\"destX\":" << p->m_ptDst.x << ',';
                oss << "\"destY\":" << p->m_ptDst.y << ',';
                oss << "\"direction\":" << p->m_nDirection;
                break;
        }
        case MAPEVENTTYPE_MAPMOVE: {
                const CInfoMapEventMAPMOVE *p = static_cast<const CInfoMapEventMAPMOVE *>(pEvent);
                oss << "\"destMapId\":" << p->m_dwMapID << ',';
                oss << "\"destX\":" << p->m_ptDst.x << ',';
                oss << "\"destY\":" << p->m_ptDst.y << ',';
                oss << "\"direction\":" << p->m_nDirection;
                break;
        }
        case MAPEVENTTYPE_TRASHBOX:
                // 固有フィールドなし
                break;
        case MAPEVENTTYPE_INITSTATUS: {
                const CInfoMapEventINITSTATUS *p = static_cast<const CInfoMapEventINITSTATUS *>(pEvent);
                oss << "\"effectId\":" << p->m_dwEffectID;
                break;
        }
        case MAPEVENTTYPE_GRPIDTMP: {
                const CInfoMapEventGRPIDTMP *p = static_cast<const CInfoMapEventGRPIDTMP *>(pEvent);
                oss << "\"setType\":" << p->m_nSetType << ',';
                oss << "\"idMain\":" << p->m_dwIDMain << ',';
                oss << "\"idSub\":" << p->m_dwIDSub;
                break;
        }
        case MAPEVENTTYPE_LIGHT: {
                const CInfoMapEventLIGHT *p = static_cast<const CInfoMapEventLIGHT *>(pEvent);
                oss << "\"lightOn\":" << (p->m_bLightOn ? "true" : "false") << ',';
                oss << "\"time\":" << p->m_dwTime;
                break;
        }
        default:
                break;
        }

        oss << '}';
}

// イベント 1 件分を JSON に出力する
void AppendEventJson(std::ostringstream &oss, const CInfoMapEventBase *pEvent, DWORD dwMapId)
{
        oss << '{';
        oss << "\"id\":" << pEvent->m_dwMapEventID << ',';
        oss << "\"mapId\":" << dwMapId << ',';
        oss << "\"type\":" << pEvent->m_nType << ',';
        oss << "\"typeLabel\":\"" << GetMapEventTypeLabel(pEvent->m_nType) << "\",";
        oss << "\"soundId\":" << pEvent->m_dwSoundID << ',';
        oss << "\"hitType\":" << pEvent->m_nHitType << ',';
        oss << "\"hitTypeLabel\":\"" << GetMapEventHitTypeLabel(pEvent->m_nHitType) << "\",";
        oss << "\"hitDirection\":" << pEvent->m_nHitDirection << ',';
        oss << "\"pos\":{\"x\":" << pEvent->m_ptPos.x << ",\"y\":" << pEvent->m_ptPos.y << "},";
        oss << "\"pos2\":{\"x\":" << pEvent->m_ptPos2.x << ",\"y\":" << pEvent->m_ptPos2.y << "},";
        AppendDetailJson(oss, pEvent);
        oss << '}';
}

// JSON から基底フィールドを pEvent に適用する
void ApplyBaseJsonToEvent(const std::string &json, CInfoMapEventBase *pEvent)
{
        int nVal = 0;
        if (JsonUtils::TryGetInt(json, "soundId", nVal)) {
                pEvent->m_dwSoundID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "hitType", nVal)) {
                pEvent->m_nHitType = nVal;
        }
        if (JsonUtils::TryGetInt(json, "hitDirection", nVal)) {
                pEvent->m_nHitDirection = nVal;
        }

        // pos オブジェクトのパース（"pos":{"x":N,"y":N}）
        size_t nPosKey = JsonUtils::FindKey(json, "pos");
        if (nPosKey != std::string::npos) {
                size_t nBrace = json.find('{', nPosKey);
                size_t nClose = json.find('}', nBrace);
                if (nBrace != std::string::npos && nClose != std::string::npos) {
                        std::string posJson = json.substr(nBrace, nClose - nBrace + 1);
                        int nx = 0, ny = 0;
                        if (JsonUtils::TryGetInt(posJson, "x", nx)) { pEvent->m_ptPos.x = nx; }
                        if (JsonUtils::TryGetInt(posJson, "y", ny)) { pEvent->m_ptPos.y = ny; }
                }
        }

        // pos2 オブジェクトのパース
        size_t nPos2Key = JsonUtils::FindKey(json, "pos2");
        if (nPos2Key != std::string::npos) {
                size_t nBrace = json.find('{', nPos2Key);
                size_t nClose = json.find('}', nBrace);
                if (nBrace != std::string::npos && nClose != std::string::npos) {
                        std::string posJson = json.substr(nBrace, nClose - nBrace + 1);
                        int nx = 0, ny = 0;
                        if (JsonUtils::TryGetInt(posJson, "x", nx)) { pEvent->m_ptPos2.x = nx; }
                        if (JsonUtils::TryGetInt(posJson, "y", ny)) { pEvent->m_ptPos2.y = ny; }
                }
        }
}

// JSON から detail フィールドを pEvent に適用する
void ApplyDetailJsonToEvent(const std::string &json, CInfoMapEventBase *pEvent)
{
        size_t nDetailKey = JsonUtils::FindKey(json, "detail");
        if (nDetailKey == std::string::npos) {
                return;
        }
        size_t nBrace = json.find('{', nDetailKey);
        size_t nClose = json.find('}', nBrace);
        if (nBrace == std::string::npos || nClose == std::string::npos) {
                return;
        }
        std::string detailJson = json.substr(nBrace, nClose - nBrace + 1);

        int nVal = 0;
        bool bVal = false;

        switch (pEvent->m_nType) {
        case MAPEVENTTYPE_MOVE: {
                CInfoMapEventMOVE *p = static_cast<CInfoMapEventMOVE *>(pEvent);
                if (JsonUtils::TryGetInt(detailJson, "destX", nVal)) { p->m_ptDst.x = nVal; }
                if (JsonUtils::TryGetInt(detailJson, "destY", nVal)) { p->m_ptDst.y = nVal; }
                if (JsonUtils::TryGetInt(detailJson, "direction", nVal)) { p->m_nDirection = nVal; }
                break;
        }
        case MAPEVENTTYPE_MAPMOVE: {
                CInfoMapEventMAPMOVE *p = static_cast<CInfoMapEventMAPMOVE *>(pEvent);
                if (JsonUtils::TryGetInt(detailJson, "destMapId", nVal)) { p->m_dwMapID = static_cast<DWORD>(nVal); }
                if (JsonUtils::TryGetInt(detailJson, "destX", nVal)) { p->m_ptDst.x = nVal; }
                if (JsonUtils::TryGetInt(detailJson, "destY", nVal)) { p->m_ptDst.y = nVal; }
                if (JsonUtils::TryGetInt(detailJson, "direction", nVal)) { p->m_nDirection = nVal; }
                break;
        }
        case MAPEVENTTYPE_INITSTATUS: {
                CInfoMapEventINITSTATUS *p = static_cast<CInfoMapEventINITSTATUS *>(pEvent);
                if (JsonUtils::TryGetInt(detailJson, "effectId", nVal)) { p->m_dwEffectID = static_cast<DWORD>(nVal); }
                break;
        }
        case MAPEVENTTYPE_GRPIDTMP: {
                CInfoMapEventGRPIDTMP *p = static_cast<CInfoMapEventGRPIDTMP *>(pEvent);
                if (JsonUtils::TryGetInt(detailJson, "setType", nVal)) { p->m_nSetType = nVal; }
                if (JsonUtils::TryGetInt(detailJson, "idMain", nVal)) { p->m_dwIDMain = static_cast<DWORD>(nVal); }
                if (JsonUtils::TryGetInt(detailJson, "idSub", nVal)) { p->m_dwIDSub = static_cast<DWORD>(nVal); }
                break;
        }
        case MAPEVENTTYPE_LIGHT: {
                CInfoMapEventLIGHT *p = static_cast<CInfoMapEventLIGHT *>(pEvent);
                if (JsonUtils::TryGetBool(detailJson, "lightOn", bVal)) { p->m_bLightOn = bVal ? TRUE : FALSE; }
                if (JsonUtils::TryGetInt(detailJson, "time", nVal)) { p->m_dwTime = static_cast<DWORD>(nVal); }
                break;
        }
        default:
                break;
        }
}

} // namespace

// ---------------------------------------------------------------------------
// CMapEventListHandler  GET /api/maps/events?mapId=<id>
// ---------------------------------------------------------------------------

CMapEventListHandler::CMapEventListHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapEventListHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }
        // TODO: 本番運用時は適切なロール判定を追加すること

        int nMapId = 0;
        if (!TryGetQueryParam(request.path, "mapId", nMapId) || nMapId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_mapId\"}");
                return;
        }

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(BuildResponseJson(static_cast<DWORD>(nMapId)));
}

std::string CMapEventListHandler::BuildResponseJson(DWORD dwMapId) const
{
        if (m_pMgrData == NULL) {
                return "{\"events\":[]}";
        }

        CLibInfoMapBase *pMapLib = m_pMgrData->GetLibInfoMap();
        if (pMapLib == NULL) {
                return "{\"events\":[]}";
        }

        pMapLib->Enter();

        CInfoMapBase *pMap = static_cast<CInfoMapBase *>(pMapLib->GetPtr(dwMapId));
        if (pMap == NULL || pMap->m_pLibInfoMapEvent == NULL) {
                pMapLib->Leave();
                return "{\"events\":[]}";
        }

        CLibInfoMapEvent *pEventLib = pMap->m_pLibInfoMapEvent;

        std::ostringstream oss;
        oss << "{\"mapId\":" << dwMapId << ",\"events\":[";
        bool first = true;
        int nCount = pEventLib->GetCount();
        for (int i = 0; i < nCount; ++i) {
                const CInfoMapEventBase *pEvent = static_cast<const CInfoMapEventBase *>(pEventLib->GetPtr(i));
                if (pEvent == NULL) {
                        continue;
                }
                if (!first) {
                        oss << ',';
                }
                first = false;
                AppendEventJson(oss, pEvent, dwMapId);
        }
        oss << "]}";

        pMapLib->Leave();
        return oss.str();
}

// ---------------------------------------------------------------------------
// CMapEventCreateHandler  POST /api/maps/events
// ---------------------------------------------------------------------------

CMapEventCreateHandler::CMapEventCreateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapEventCreateHandler::Handle(const HttpRequest &request, HttpResponse &response)
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

        CLibInfoMapBase  *pMapLib = m_pMgrData->GetLibInfoMap();
        CUraraSockTCPSBO *pSock   = m_pMgrData->GetSock();
        if (pMapLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nMapId = 0;
        if (!JsonUtils::TryGetInt(request.body, "mapId", nMapId) || nMapId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_mapId\"}");
                return;
        }

        int nType = 0;
        if (!JsonUtils::TryGetInt(request.body, "type", nType) ||
            nType < MAPEVENTTYPE_NONE || nType >= MAPEVENTTYPE_MAX) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_type\"}");
                return;
        }

        pMapLib->Enter();

        CInfoMapBase *pMap = static_cast<CInfoMapBase *>(pMapLib->GetPtr(static_cast<DWORD>(nMapId)));
        if (pMap == NULL || pMap->m_pLibInfoMapEvent == NULL) {
                pMapLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"map_not_found\"}");
                return;
        }

        CLibInfoMapEvent *pEventLib = pMap->m_pLibInfoMapEvent;

        PCInfoMapEventBase pNew = static_cast<PCInfoMapEventBase>(pEventLib->GetNew(nType));
        if (pNew == NULL) {
                pMapLib->Leave();
                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"failed_to_create_event\"}");
                return;
        }

        ApplyBaseJsonToEvent(request.body, pNew);
        ApplyDetailJsonToEvent(request.body, pNew);
        pEventLib->Add(pNew);
        pMap->RenewMapEvent();

        DWORD dwMapId = static_cast<DWORD>(nMapId);

        // クライアントへ通知
        CPacketMAP_MAPEVENT packet;
        packet.Make(dwMapId, pNew);
        pSock->SendTo(0, &packet);

        std::ostringstream oss;
        AppendEventJson(oss, pNew, dwMapId);

        pMapLib->Leave();

        response.statusLine = "HTTP/1.1 201 Created";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// CMapEventUpdateHandler  PUT /api/maps/events
// ---------------------------------------------------------------------------

CMapEventUpdateHandler::CMapEventUpdateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapEventUpdateHandler::Handle(const HttpRequest &request, HttpResponse &response)
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

        CLibInfoMapBase  *pMapLib = m_pMgrData->GetLibInfoMap();
        CUraraSockTCPSBO *pSock   = m_pMgrData->GetSock();
        if (pMapLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nMapId = 0;
        if (!JsonUtils::TryGetInt(request.body, "mapId", nMapId) || nMapId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_mapId\"}");
                return;
        }

        int nEventId = 0;
        if (!JsonUtils::TryGetInt(request.body, "id", nEventId) || nEventId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_id\"}");
                return;
        }

        pMapLib->Enter();

        CInfoMapBase *pMap = static_cast<CInfoMapBase *>(pMapLib->GetPtr(static_cast<DWORD>(nMapId)));
        if (pMap == NULL || pMap->m_pLibInfoMapEvent == NULL) {
                pMapLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"map_not_found\"}");
                return;
        }

        CLibInfoMapEvent *pEventLib = pMap->m_pLibInfoMapEvent;

        PCInfoMapEventBase pEvent = static_cast<PCInfoMapEventBase>(
                pEventLib->GetPtr(static_cast<DWORD>(nEventId)));
        if (pEvent == NULL) {
                pMapLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"event_not_found\"}");
                return;
        }

        ApplyBaseJsonToEvent(request.body, pEvent);
        ApplyDetailJsonToEvent(request.body, pEvent);
        pMap->RenewMapEvent();

        DWORD dwMapId = static_cast<DWORD>(nMapId);

        // クライアントへ通知（RecvProcADMIN_MAP_RENEWEVENT と同じパターン）
        CPacketMAP_MAPEVENT packet;
        packet.Make(dwMapId, pEvent);
        pSock->SendTo(0, &packet);

        std::ostringstream oss;
        AppendEventJson(oss, pEvent, dwMapId);

        pMapLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// CMapEventDeleteHandler  DELETE /api/maps/events?mapId=<id>&id=<eventId>
// ---------------------------------------------------------------------------

CMapEventDeleteHandler::CMapEventDeleteHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapEventDeleteHandler::Handle(const HttpRequest &request, HttpResponse &response)
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

        CLibInfoMapBase  *pMapLib = m_pMgrData->GetLibInfoMap();
        CUraraSockTCPSBO *pSock   = m_pMgrData->GetSock();
        if (pMapLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nMapId = 0;
        if (!TryGetQueryParam(request.path, "mapId", nMapId) || nMapId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_mapId\"}");
                return;
        }

        int nEventId = 0;
        if (!TryGetQueryParam(request.path, "id", nEventId) || nEventId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_id\"}");
                return;
        }

        DWORD dwMapId   = static_cast<DWORD>(nMapId);
        DWORD dwEventId = static_cast<DWORD>(nEventId);

        pMapLib->Enter();

        CInfoMapBase *pMap = static_cast<CInfoMapBase *>(pMapLib->GetPtr(dwMapId));
        if (pMap == NULL || pMap->m_pLibInfoMapEvent == NULL) {
                pMapLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"map_not_found\"}");
                return;
        }

        CLibInfoMapEvent *pEventLib = pMap->m_pLibInfoMapEvent;

        if (pEventLib->GetPtr(dwEventId) == NULL) {
                pMapLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"event_not_found\"}");
                return;
        }

        pEventLib->Delete(dwEventId);
        pMap->RenewMapEvent();

        // クライアントへ通知（RecvProcADMIN_MAP_DELETEEVENT と同じパターン）
        CPacketMAP_PARA1 packet;
        packet.Make(SBOCOMMANDID_SUB_MAP_DELETEEVENT, dwMapId, dwEventId);
        pSock->SendTo(0, &packet);

        pMapLib->Leave();

        std::ostringstream oss;
        oss << "{\"deleted\":" << dwEventId << ",\"mapId\":" << dwMapId << "}";
        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}
