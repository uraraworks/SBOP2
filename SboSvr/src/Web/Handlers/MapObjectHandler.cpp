#include "stdafx.h"
#include "MapObjectHandler.h"

#include <iomanip>
#include <sstream>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "LibInfo/LibInfoMapBase.h"
#include "LibInfo/LibInfoMapObject.h"
#include "LibInfo/LibInfoMapObjectData.h"
#include "Info/InfoMapBase.h"
#include "Info/InfoMapObjectData.h"
#include "Info/InfoMapObject.h"

CMapObjectListHandler::CMapObjectListHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapObjectListHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }
        // 開発・検証目的でマップオブジェクトの参照と編集を行う際にログインを必須としないよう、
        // 認証失敗やロール不足を検出してもレスポンスは 200 を返す。
        // TODO: 本番運用に合わせて適切なアクセス制御を復活させること。

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(BuildResponseJson());
}

std::string CMapObjectListHandler::BuildResponseJson() const
{
        if (m_pMgrData == NULL) {
                return "{\"maps\":[]}";
        }

        CLibInfoMapBase *pMapLib = m_pMgrData->GetLibInfoMap();
        CLibInfoMapObject *pObjectLib = m_pMgrData->GetLibInfoMapObject();
        if ((pMapLib == NULL) || (pObjectLib == NULL)) {
                return "{\"maps\":[]}";
        }

        pMapLib->Enter();
        pObjectLib->Enter();

        std::ostringstream oss;
        oss << "{\"maps\":[";
        bool firstMap = true;
        int nMapCount = pMapLib->GetCount();
        for (int i = 0; i < nMapCount; ++i) {
                const CInfoMapBase *pMap = static_cast<const CInfoMapBase *>(pMapLib->GetPtr(i));
                if (pMap == NULL) {
                        continue;
                }
                if (!firstMap) {
                        oss << ',';
                }
                firstMap = false;
                AppendMapJson(oss, pMap, pObjectLib);
        }
        oss << "]}";

        pObjectLib->Leave();
        pMapLib->Leave();

        return oss.str();
}

void CMapObjectListHandler::AppendMapJson(std::ostringstream &oss, const CInfoMapBase *pMap, CLibInfoMapObject *pObjectLib) const
{
        oss << '{';
        oss << "\"id\":" << pMap->m_dwMapID << ',';
        std::string idHex = FormatHex(pMap->m_dwMapID);
        if (!idHex.empty()) {
                oss << "\"idHex\":\"" << idHex << "\",";
        } else {
                oss << "\"idHex\":null,";
        }

        std::string mapName = static_cast<LPCSTR>(pMap->m_strMapName);
        oss << "\"name\":\"" << JsonUtils::Escape(mapName) << "\",";
        oss << "\"width\":" << pMap->m_sizeMap.cx << ',';
        oss << "\"height\":" << pMap->m_sizeMap.cy << ',';
        oss << "\"weatherType\":" << pMap->m_dwWeatherType << ',';
        oss << "\"weatherLabel\":\"" << JsonUtils::Escape(ResolveWeatherLabel(pMap->m_dwWeatherType)) << "\",";
        oss << "\"battleEnabled\":" << (pMap->m_bEnableBattle ? "true" : "false") << ',';
        oss << "\"recoveryEnabled\":" << (pMap->m_bRecovery ? "true" : "false") << ',';

        oss << "\"objects\":[";
        bool firstObject = true;
        CLibInfoMapObjectData *pPlacementLib = pMap->m_pLibInfoMapObjectData;
        if (pPlacementLib != NULL) {
                pPlacementLib->Enter();
                int nPlacementCount = pPlacementLib->GetCount();
                for (int i = 0; i < nPlacementCount; ++i) {
                        const CInfoMapObjectData *pPlacement = static_cast<const CInfoMapObjectData *>(pPlacementLib->GetPtr(i));
                        if (pPlacement == NULL) {
                                continue;
                        }
                        if (!firstObject) {
                                oss << ',';
                        }
                        firstObject = false;
                        AppendObjectJson(oss, pPlacement, pObjectLib);
                }
                pPlacementLib->Leave();
        }
        oss << ']';
        oss << '}';
}

void CMapObjectListHandler::AppendObjectJson(std::ostringstream &oss, const CInfoMapObjectData *pPlacement, CLibInfoMapObject *pObjectLib) const
{
        oss << '{';
        oss << "\"placementId\":" << pPlacement->m_dwDataID << ',';
        oss << "\"objectId\":" << pPlacement->m_dwObjectID << ',';
        std::string objectIdHex = FormatHex(pPlacement->m_dwObjectID);
        if (!objectIdHex.empty()) {
                oss << "\"objectIdHex\":\"" << objectIdHex << "\",";
        } else {
                oss << "\"objectIdHex\":null,";
        }
        oss << "\"x\":" << pPlacement->m_ptPos.x << ',';
        oss << "\"y\":" << pPlacement->m_ptPos.y;

        const CInfoMapObject *pObject = NULL;
        if (pObjectLib != NULL) {
                pObject = static_cast<const CInfoMapObject *>(pObjectLib->GetPtr(pPlacement->m_dwObjectID));
        }

        if (pObject != NULL) {
                std::string objectName = static_cast<LPCSTR>(pObject->m_strName);
                oss << ",\"name\":\"" << JsonUtils::Escape(objectName) << "\"";
                oss << ",\"attribute\":" << pObject->m_dwAttr;
                std::string attributeHex = FormatHex(pObject->m_dwAttr);
                if (!attributeHex.empty()) {
                        oss << ",\"attributeHex\":\"" << attributeHex << "\"";
                } else {
                        oss << ",\"attributeHex\":null";
                }
                oss << ",\"hasCollision\":" << (pObject->m_bHit ? "true" : "false");
                oss << ",\"size\":{\"width\":" << pObject->m_sizeGrp.cx << ",\"height\":" << pObject->m_sizeGrp.cy << "}";
        } else {
                oss << ",\"name\":null";
                oss << ",\"attribute\":null";
                oss << ",\"attributeHex\":null";
                oss << ",\"hasCollision\":false";
                oss << ",\"size\":null";
        }

        oss << '}';
}

std::string CMapObjectListHandler::FormatHex(unsigned long value)
{
        std::ostringstream oss;
        oss << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << value;
        std::string text = oss.str();
        if (text.empty()) {
                return std::string();
        }
        return std::string("0x") + text;
}

std::string CMapObjectListHandler::ResolveWeatherLabel(unsigned long weatherType)
{
        // VC++ の既定文字コード (Shift_JIS) と衝突しないよう、UTF-8 を明示したエスケープシーケンスで返す。
        switch (weatherType) {
        case WEATHERTYPE_NONE:
                return std::string("\xE6\x8C\x87\xE5\xAE\x9A\xE3\x81\xAA\xE3\x81\x97");
        case WEATHERTYPE_CLOUD:
                return std::string("\xE9\x9B\xB2");
        case WEATHERTYPE_MISTY:
                return std::string("\xE9\x9C\xA7");
        case WEATHERTYPE_SNOW:
                return std::string("\xE9\x9B\xAA");
        default:
                break;
        }
        return std::string("\xE4\xB8\x8D\xE6\x98\x8E");
}

