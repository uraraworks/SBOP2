#include "stdafx.h"
#include "MapObjectHandler.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstring>

#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "MgrData.h"
#include "LibInfo/LibInfoMapBase.h"
#include "LibInfo/LibInfoMapObject.h"
#include "LibInfo/LibInfoMapObjectData.h"
#include "LibInfo/LibInfoMapParts.h"
#include "Info/InfoMapBase.h"
#include "Info/InfoMapObjectData.h"
#include "Info/InfoMapObject.h"
#include "Info/InfoMapParts.h"
#include "myLib/myString.h"
#include "UraraSockTCPSBO.h"
#include "Packet/MAP/PacketMAP_MAPOBJECT.h"
#include "Packet/MAP/PacketMAP_MAPOBJECTDATA.h"

namespace
{
std::string RemoveUtf8Bom(std::string text)
{
        if (text.size() >= 3 &&
            static_cast<unsigned char>(text[0]) == 0xEF &&
            static_cast<unsigned char>(text[1]) == 0xBB &&
            static_cast<unsigned char>(text[2]) == 0xBF) {
                text.erase(0, 3);
        }
        return text;
}

#ifndef _UNICODE
bool TryConvertToUtf8(const char *pszSource, int srcLength, UINT codePage, std::string &out)
{
        if ((pszSource == NULL) || (srcLength <= 0) || (codePage == 0)) {
                return false;
        }

        DWORD flags = (codePage == CP_UTF8) ? MB_ERR_INVALID_CHARS : 0;
        int wideLength = MultiByteToWideChar(codePage, flags, pszSource, srcLength, NULL, 0);
        if (wideLength <= 0) {
                return false;
        }

        std::vector<wchar_t> wide(static_cast<size_t>(wideLength));
        int converted = MultiByteToWideChar(codePage, flags, pszSource, srcLength, &wide[0], wideLength);
        if (converted <= 0) {
                return false;
        }

        if (!wide.empty() && wide.back() == L'\0') {
                wide.pop_back();
                converted = static_cast<int>(wide.size());
        }

        int utf8Length = WideCharToMultiByte(CP_UTF8, 0, &wide[0], converted, NULL, 0, NULL, NULL);
        if (utf8Length <= 0) {
                return false;
        }

        out.resize(static_cast<size_t>(utf8Length));
        WideCharToMultiByte(CP_UTF8, 0, &wide[0], converted, &out[0], utf8Length, NULL, NULL);
        return true;
}
#endif

std::string RemoveControlCharacters(std::string text)
{
        text.erase(
                std::remove_if(
                        text.begin(),
                        text.end(),
                        [](unsigned char ch) {
                                if (ch == '\n' || ch == '\r' || ch == '\t') {
                                        return false;
                                }
                                return ch < 0x20;
                        }),
                text.end());
        return text;
}

std::string ToUtf8String(const CmyString &value)
{
#ifdef _UNICODE
        CStringA utf8 = TStringToUtf8(static_cast<LPCTSTR>(value));
        if (utf8.IsEmpty()) {
                return std::string();
        }
        std::string converted(utf8.GetString(), static_cast<size_t>(utf8.GetLength()));
        return RemoveControlCharacters(RemoveUtf8Bom(converted));
#else
        LPCSTR pszSource = static_cast<LPCSTR>(value);
        if ((pszSource == NULL) || (pszSource[0] == '\0')) {
                return std::string();
        }

        int srcLength = static_cast<int>(std::strlen(pszSource));
        if (srcLength <= 0) {
                return std::string();
        }

        const UINT candidates[] = { CP_UTF8, 932u, CP_ACP };
        std::string utf8;
        for (size_t i = 0; i < sizeof(candidates) / sizeof(candidates[0]); ++i) {
                if (TryConvertToUtf8(pszSource, srcLength, candidates[i], utf8)) {
                        return RemoveControlCharacters(RemoveUtf8Bom(utf8));
                }
        }

        return RemoveControlCharacters(RemoveUtf8Bom(std::string(pszSource, static_cast<size_t>(srcLength))));
#endif
}

/* UTF-8 文字列を内部文字コード (MBCS: CP932 / Unicode: UTF-16) に変換して CmyString へ代入 */
bool SetStringFromUtf8(CmyString &out, const std::string &utf8)
{
        if (utf8.empty()) {
                out = _T("");
                return true;
        }
#ifdef _UNICODE
        int wideLen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), NULL, 0);
        if (wideLen <= 0) {
                return false;
        }
        std::vector<wchar_t> wide(static_cast<size_t>(wideLen) + 1, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), &wide[0], wideLen);
        out = &wide[0];
#else
        int wideLen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), NULL, 0);
        if (wideLen <= 0) {
                return false;
        }
        std::vector<wchar_t> wide(static_cast<size_t>(wideLen) + 1, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), static_cast<int>(utf8.size()), &wide[0], wideLen);

        int mbLen = WideCharToMultiByte(932, 0, &wide[0], wideLen, NULL, 0, NULL, NULL);
        if (mbLen <= 0) {
                return false;
        }
        std::vector<char> mb(static_cast<size_t>(mbLen) + 1, '\0');
        WideCharToMultiByte(932, 0, &wide[0], wideLen, &mb[0], mbLen, NULL, NULL);
        out = &mb[0];
#endif
        return true;
}

/* CInfoMapObject の内容を JSON に出力する */
void BuildTemplateJson(std::ostringstream &oss, const CInfoMapObject *pInfo)
{
        oss << '{';
        oss << "\"objectId\":" << pInfo->m_dwObjectID << ',';
        oss << "\"name\":\"" << JsonUtils::Escape(ToUtf8String(pInfo->m_strName)) << "\",";
        oss << "\"attribute\":" << pInfo->m_dwAttr << ',';
        oss << "\"hasCollision\":" << (pInfo->m_bHit ? "true" : "false") << ',';
        oss << "\"hideY\":" << pInfo->m_nHideY << ',';
        oss << "\"width\":" << pInfo->m_sizeGrp.cx << ',';
        oss << "\"height\":" << pInfo->m_sizeGrp.cy;
        oss << '}';
}

/* JSON から CInfoMapObject へ値を適用する */
void ApplyJsonToTemplate(const std::string &json, CInfoMapObject *pInfo)
{
        std::string name;
        if (JsonUtils::TryGetString(json, "name", name)) {
                SetStringFromUtf8(pInfo->m_strName, name);
        }
        int nVal = 0;
        if (JsonUtils::TryGetInt(json, "attribute", nVal)) {
                pInfo->m_dwAttr = static_cast<DWORD>(nVal);
        }
        bool bVal = false;
        if (JsonUtils::TryGetBool(json, "hasCollision", bVal)) {
                pInfo->m_bHit = bVal ? TRUE : FALSE;
        }
        if (JsonUtils::TryGetInt(json, "hideY", nVal)) {
                pInfo->m_nHideY = nVal;
        }
        if (JsonUtils::TryGetInt(json, "width", nVal)) {
                pInfo->m_sizeGrp.cx = nVal;
        }
        if (JsonUtils::TryGetInt(json, "height", nVal)) {
                pInfo->m_sizeGrp.cy = nVal;
        }
}
}

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
        CLibInfoMapParts *pPartsLib = m_pMgrData->GetLibInfoMapParts();
        if ((pMapLib == NULL) || (pObjectLib == NULL)) {
                return "{\"maps\":[]}";
        }

        pMapLib->Enter();
        pObjectLib->Enter();
        if (pPartsLib != NULL) {
                pPartsLib->Enter();
        }

        std::ostringstream oss;
        oss << "{";
        oss << "\"tileSize\":16,";
        oss << "\"sheetTileWidth\":32,";
        oss << "\"sheetTileHeight\":32,";
        oss << "\"sheetBaseUrl\":\"/api/assets/map-parts/sheets\",";
        oss << "\"maps\":[";
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
                AppendMapJson(oss, pMap, pObjectLib, pPartsLib);
        }
        oss << "]}";

        if (pPartsLib != NULL) {
                pPartsLib->Leave();
        }
        pObjectLib->Leave();
        pMapLib->Leave();

        return oss.str();
}

void CMapObjectListHandler::AppendMapJson(std::ostringstream &oss, const CInfoMapBase *pMap, CLibInfoMapObject *pObjectLib, CLibInfoMapParts *pPartsLib) const
{
        oss << '{';
        oss << "\"id\":" << pMap->m_dwMapID << ',';
        std::string idHex = FormatHex(pMap->m_dwMapID);
        if (!idHex.empty()) {
                oss << "\"idHex\":\"" << idHex << "\",";
        } else {
                oss << "\"idHex\":null,";
        }

        std::string mapName = ToUtf8String(pMap->m_strMapName);
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
        oss << "],";

        /* タイルデータ (各座標のベースグラフィックID) */
        oss << "\"tiles\":[";
        int nTileCount = pMap->m_sizeMap.cx * pMap->m_sizeMap.cy;
        if (pMap->m_pwMap != NULL && pPartsLib != NULL && nTileCount > 0) {
                for (int i = 0; i < nTileCount; ++i) {
                        if (i > 0) {
                                oss << ',';
                        }
                        WORD wPartsID = pMap->m_pwMap[i];
                        const CInfoMapParts *pParts = static_cast<const CInfoMapParts *>(pPartsLib->GetPtr(static_cast<DWORD>(wPartsID)));
                        if (pParts != NULL) {
                                oss << pParts->m_wGrpIDBase;
                        } else {
                                oss << 0;
                        }
                }
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
                std::string objectName = ToUtf8String(pObject->m_strName);
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

/* ========================================================================= */
/* CMapObjectTemplateListHandler                                              */
/* ========================================================================= */

CMapObjectTemplateListHandler::CMapObjectTemplateListHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapObjectTemplateListHandler::Handle(const HttpRequest &request, HttpResponse &response)
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

std::string CMapObjectTemplateListHandler::BuildResponseJson() const
{
        if (m_pMgrData == NULL) {
                return "{\"templates\":[]}";
        }
        CLibInfoMapObject *pObjectLib = m_pMgrData->GetLibInfoMapObject();
        if (pObjectLib == NULL) {
                return "{\"templates\":[]}";
        }

        pObjectLib->Enter();

        std::ostringstream oss;
        oss << "{\"templates\":[";
        bool first = true;
        int nCount = pObjectLib->GetCount();
        for (int i = 0; i < nCount; ++i) {
                const CInfoMapObject *pInfo = static_cast<const CInfoMapObject *>(pObjectLib->GetPtr(i));
                if (pInfo == NULL) {
                        continue;
                }
                if (!first) {
                        oss << ',';
                }
                first = false;
                BuildTemplateJson(oss, pInfo);
        }
        oss << "]}";

        pObjectLib->Leave();
        return oss.str();
}

/* ========================================================================= */
/* CMapObjectTemplateCreateHandler                                            */
/* ========================================================================= */

CMapObjectTemplateCreateHandler::CMapObjectTemplateCreateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapObjectTemplateCreateHandler::Handle(const HttpRequest &request, HttpResponse &response)
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

        CLibInfoMapObject *pObjectLib = m_pMgrData->GetLibInfoMapObject();
        CUraraSockTCPSBO  *pSock      = m_pMgrData->GetSock();
        if (pObjectLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pObjectLib->Enter();

        PCInfoMapObject pInfo = static_cast<PCInfoMapObject>(pObjectLib->GetNew());
        if (pInfo == NULL) {
                pObjectLib->Leave();
                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"failed_to_create_template\"}");
                return;
        }

        ApplyJsonToTemplate(request.body, pInfo);
        pObjectLib->Add(pInfo);

        CPacketMAP_MAPOBJECT packet;
        packet.Make(pInfo);
        pSock->SendTo(0, &packet);

        std::ostringstream oss;
        BuildTemplateJson(oss, pInfo);

        pObjectLib->Leave();

        response.statusLine = "HTTP/1.1 201 Created";
        response.SetJsonBody(oss.str());
}

/* ========================================================================= */
/* CMapObjectTemplateUpdateHandler                                            */
/* ========================================================================= */

CMapObjectTemplateUpdateHandler::CMapObjectTemplateUpdateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapObjectTemplateUpdateHandler::Handle(const HttpRequest &request, HttpResponse &response)
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

        CLibInfoMapObject *pObjectLib = m_pMgrData->GetLibInfoMapObject();
        CUraraSockTCPSBO  *pSock      = m_pMgrData->GetSock();
        if (pObjectLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nObjectId = 0;
        if (!JsonUtils::TryGetInt(request.body, "objectId", nObjectId) || nObjectId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_objectId\"}");
                return;
        }

        pObjectLib->Enter();

        PCInfoMapObject pInfo = static_cast<PCInfoMapObject>(pObjectLib->GetPtr(static_cast<DWORD>(nObjectId)));
        if (pInfo == NULL) {
                pObjectLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"template_not_found\"}");
                return;
        }

        ApplyJsonToTemplate(request.body, pInfo);

        CPacketMAP_MAPOBJECT packet;
        packet.Make(pInfo);
        pSock->SendTo(0, &packet);

        std::ostringstream oss;
        BuildTemplateJson(oss, pInfo);

        pObjectLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

/* ========================================================================= */
/* CMapObjectTemplateDeleteHandler                                            */
/* ========================================================================= */

CMapObjectTemplateDeleteHandler::CMapObjectTemplateDeleteHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapObjectTemplateDeleteHandler::Handle(const HttpRequest &request, HttpResponse &response)
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

        CLibInfoMapObject *pObjectLib = m_pMgrData->GetLibInfoMapObject();
        CUraraSockTCPSBO  *pSock      = m_pMgrData->GetSock();
        if (pObjectLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nObjectId = 0;
        if (!JsonUtils::TryGetInt(request.body, "objectId", nObjectId) || nObjectId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_objectId\"}");
                return;
        }

        DWORD dwObjectId = static_cast<DWORD>(nObjectId);

        pObjectLib->Enter();

        if (pObjectLib->GetPtr(dwObjectId) == NULL) {
                pObjectLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"template_not_found\"}");
                return;
        }

        pObjectLib->Delete(dwObjectId);

        /* 削除後は全テンプレートを再送して各クライアントに反映 */
        CPacketMAP_MAPOBJECT packet;
        packet.Make(pObjectLib);
        pSock->SendTo(0, &packet);

        pObjectLib->Leave();

        std::ostringstream oss;
        oss << "{\"deleted\":" << dwObjectId << "}";
        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

/* ========================================================================= */
/* CMapPlacementCreateHandler                                                 */
/* ========================================================================= */

CMapPlacementCreateHandler::CMapPlacementCreateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapPlacementCreateHandler::Handle(const HttpRequest &request, HttpResponse &response)
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
        int nObjectId = 0;
        if (!JsonUtils::TryGetInt(request.body, "objectId", nObjectId) || nObjectId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_objectId\"}");
                return;
        }
        int nX = 0, nY = 0;
        JsonUtils::TryGetInt(request.body, "x", nX);
        JsonUtils::TryGetInt(request.body, "y", nY);

        pMapLib->Enter();

        CInfoMapBase *pMap = static_cast<CInfoMapBase *>(pMapLib->GetPtr(static_cast<DWORD>(nMapId)));
        if (pMap == NULL || pMap->m_pLibInfoMapObjectData == NULL) {
                pMapLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"map_not_found\"}");
                return;
        }

        CLibInfoMapObjectData *pPlacementLib = pMap->m_pLibInfoMapObjectData;
        pPlacementLib->Enter();

        PCInfoMapObjectData pPlacement = static_cast<PCInfoMapObjectData>(pPlacementLib->GetNew());
        if (pPlacement == NULL) {
                pPlacementLib->Leave();
                pMapLib->Leave();
                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"failed_to_create_placement\"}");
                return;
        }

        pPlacement->m_dwObjectID = static_cast<DWORD>(nObjectId);
        pPlacement->m_ptPos.x    = nX;
        pPlacement->m_ptPos.y    = nY;
        pPlacementLib->Add(pPlacement);

        DWORD dwDataId = pPlacement->m_dwDataID;
        DWORD dwMapId  = static_cast<DWORD>(nMapId);

        CPacketMAP_MAPOBJECTDATA packet;
        packet.Make(dwMapId, pPlacement);
        pSock->SendTo(0, &packet);

        pPlacementLib->Leave();
        pMapLib->Leave();

        std::ostringstream oss;
        oss << "{\"dataId\":" << dwDataId << ",\"mapId\":" << dwMapId << ",\"objectId\":" << nObjectId << ",\"x\":" << nX << ",\"y\":" << nY << "}";
        response.statusLine = "HTTP/1.1 201 Created";
        response.SetJsonBody(oss.str());
}

/* ========================================================================= */
/* CMapPlacementUpdateHandler                                                 */
/* ========================================================================= */

CMapPlacementUpdateHandler::CMapPlacementUpdateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapPlacementUpdateHandler::Handle(const HttpRequest &request, HttpResponse &response)
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
        int nDataId = 0;
        if (!JsonUtils::TryGetInt(request.body, "dataId", nDataId) || nDataId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_dataId\"}");
                return;
        }

        pMapLib->Enter();

        CInfoMapBase *pMap = static_cast<CInfoMapBase *>(pMapLib->GetPtr(static_cast<DWORD>(nMapId)));
        if (pMap == NULL || pMap->m_pLibInfoMapObjectData == NULL) {
                pMapLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"map_not_found\"}");
                return;
        }

        CLibInfoMapObjectData *pPlacementLib = pMap->m_pLibInfoMapObjectData;
        pPlacementLib->Enter();

        PCInfoMapObjectData pPlacement = static_cast<PCInfoMapObjectData>(pPlacementLib->GetPtr(static_cast<DWORD>(nDataId)));
        if (pPlacement == NULL) {
                pPlacementLib->Leave();
                pMapLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"placement_not_found\"}");
                return;
        }

        int nVal = 0;
        if (JsonUtils::TryGetInt(request.body, "objectId", nVal) && nVal > 0) {
                pPlacement->m_dwObjectID = static_cast<DWORD>(nVal);
        }
        if (JsonUtils::TryGetInt(request.body, "x", nVal)) {
                pPlacement->m_ptPos.x = nVal;
        }
        if (JsonUtils::TryGetInt(request.body, "y", nVal)) {
                pPlacement->m_ptPos.y = nVal;
        }

        DWORD dwMapId = static_cast<DWORD>(nMapId);

        CPacketMAP_MAPOBJECTDATA packet;
        packet.Make(dwMapId, pPlacement);
        pSock->SendTo(0, &packet);

        std::ostringstream oss;
        oss << "{\"dataId\":" << pPlacement->m_dwDataID << ",\"mapId\":" << dwMapId << ",\"objectId\":" << pPlacement->m_dwObjectID << ",\"x\":" << pPlacement->m_ptPos.x << ",\"y\":" << pPlacement->m_ptPos.y << "}";

        pPlacementLib->Leave();
        pMapLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

/* ========================================================================= */
/* CMapPlacementDeleteHandler                                                 */
/* ========================================================================= */

CMapPlacementDeleteHandler::CMapPlacementDeleteHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapPlacementDeleteHandler::Handle(const HttpRequest &request, HttpResponse &response)
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
        int nDataId = 0;
        if (!JsonUtils::TryGetInt(request.body, "dataId", nDataId) || nDataId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_dataId\"}");
                return;
        }

        DWORD dwMapId  = static_cast<DWORD>(nMapId);
        DWORD dwDataId = static_cast<DWORD>(nDataId);

        pMapLib->Enter();

        CInfoMapBase *pMap = static_cast<CInfoMapBase *>(pMapLib->GetPtr(dwMapId));
        if (pMap == NULL || pMap->m_pLibInfoMapObjectData == NULL) {
                pMapLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"map_not_found\"}");
                return;
        }

        CLibInfoMapObjectData *pPlacementLib = pMap->m_pLibInfoMapObjectData;
        pPlacementLib->Enter();

        if (pPlacementLib->GetPtr(dwDataId) == NULL) {
                pPlacementLib->Leave();
                pMapLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"placement_not_found\"}");
                return;
        }

        pPlacementLib->Delete(dwDataId);

        /* 削除後はマップの全配置を再送して各クライアントに反映 */
        CPacketMAP_MAPOBJECTDATA packet;
        packet.Make(dwMapId, pPlacementLib);
        pSock->SendTo(0, &packet);

        pPlacementLib->Leave();
        pMapLib->Leave();

        std::ostringstream oss;
        oss << "{\"deleted\":" << dwDataId << ",\"mapId\":" << dwMapId << "}";
        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

