#include "StdAfx.h"
#include "MapPartsHandler.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cwchar>
#include <iomanip>

#include "lodepng.h"

#include "MgrData.h"
#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "SpriteSheetHandler.h"
#include "Web/AdminWsHub.h"
#include "Web/MapPartsHistory.h"
#include "Info/InfoMapParts.h"
#include "Info/InfoMapBase.h"
#include "LibInfo/LibInfoMapParts.h"
#include "LibInfo/LibInfoMapBase.h"
#include "UraraSockTCPSBO.h"
#include "Packet/MAP/PacketMAP_MAPPARTS.h"
#include "Packet/MAP/PacketMAP_DELETEPARTS.h"
#include "Packet/MAP/PacketMAP_SETPARTS.h"
#include "../../Platform/SvrPlatform.h"

namespace
{
static const int kTileSize = 16;
static const int kSheetTileWidth = 32;
static const int kSheetTileHeight = 32;
static const char *kSheetBaseUrl = "/api/assets/map-parts/sheets";

static std::string RemoveUtf8Bom(std::string text)
{
        if (text.size() >= 3 &&
            static_cast<unsigned char>(text[0]) == 0xEF &&
            static_cast<unsigned char>(text[1]) == 0xBB &&
            static_cast<unsigned char>(text[2]) == 0xBF) {
                text.erase(0, 3);
        }
        return text;
}

// アニメーションコマ配列を JSON に出力する
static void AppendAnimeFramesJson(std::ostringstream &oss, const CInfoMapParts *pInfo)
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
}

CMapPartsResourceProvider::CMapPartsResourceProvider()
        : m_sheetCount(-1)
{
}

CMapPartsResourceProvider::~CMapPartsResourceProvider()
{
        std::lock_guard<std::mutex> lock(m_mutex);
        m_sheetCache.clear();
}

// シート画像の取得は、スプライト配信(/api/assets/sprites, CGrpResourceProvider)の
// "mapParts" カテゴリに任せる。以前はここで SboGrpData.dll のリソースだけを読んでいたため、
// DLL が無い Linux(ステージング)ではマップパーツ配置画面のシートが 404 になり、
// パーツ画像が表示されなかった(マップパーツ編集画面は /api/assets/sprites 経由なので表示されていた)。
// CGrpResourceProvider は 画像ストア(DB) → ファイル(res/) → DLL の順に探し、
// パレット0の透過化も同じように行う。画像エディタでの編集も反映される。
static const char *kMapPartsCategoryKey = "mapParts";

bool CMapPartsResourceProvider::IsAvailable()
{
        return GetSheetCount() > 0;
}

bool CMapPartsResourceProvider::GetSheetPng(int sheetIndex, std::vector<unsigned char> &outData)
{
        std::lock_guard<std::mutex> lock(m_mutex);
        return LoadSheetLocked(sheetIndex, outData);
}

int CMapPartsResourceProvider::GetSheetCount()
{
        return CGrpResourceProvider::GetInstance().GetSheetCount(kMapPartsCategoryKey);
}

bool CMapPartsResourceProvider::LoadSheetLocked(int sheetIndex, std::vector<unsigned char> &outData)
{
        if (sheetIndex < 0) {
                return false;
        }
        std::string etag;
        return CGrpResourceProvider::GetInstance().GetSheetPng(kMapPartsCategoryKey, sheetIndex, outData, etag);
}

CMapPartsListHandler::CMapPartsListHandler(CMgrData *pMgrData, std::shared_ptr<CMapPartsResourceProvider> provider)
        : m_pMgrData(pMgrData)
        , m_provider(provider)
{
}

void CMapPartsListHandler::Handle(const HttpRequest &request, HttpResponse &response)
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

std::string CMapPartsListHandler::BuildResponseJson() const
{
        if (m_pMgrData == NULL) {
                return "{\"tileSize\":16,\"sheetTileWidth\":32,\"sheetTileHeight\":32,\"parts\":[],\"sheets\":[],\"sheetBaseUrl\":\"/api/assets/map-parts/sheets\"}";
        }

        CLibInfoMapParts *pPartsLib = m_pMgrData->GetLibInfoMapParts();
        if (pPartsLib == NULL) {
                return "{\"tileSize\":16,\"sheetTileWidth\":32,\"sheetTileHeight\":32,\"parts\":[],\"sheets\":[],\"sheetBaseUrl\":\"/api/assets/map-parts/sheets\"}";
        }

        std::set<int> sheetSet;
        std::ostringstream oss;
        oss << "{\"tileSize\":" << kTileSize
            << ",\"sheetTileWidth\":" << kSheetTileWidth
            << ",\"sheetTileHeight\":" << kSheetTileHeight
            << ",\"sheetBaseUrl\":\"" << JsonUtils::Escape(std::string(kSheetBaseUrl)) << "\"";

        int sheetCount = 0;
        if (m_provider) {
                sheetCount = m_provider->GetSheetCount();
        }
        oss << ",\"sheetCount\":" << sheetCount;

        oss << ",\"parts\":[";

        pPartsLib->Enter();
        bool first = true;
        int nCount = pPartsLib->GetCount();
        for (int i = 0; i < nCount; ++i) {
                const CInfoMapParts *pInfo = static_cast<const CInfoMapParts *>(pPartsLib->GetPtr(i));
                if (pInfo == NULL) {
                        continue;
                }
                if (!first) {
                        oss << ',';
                }
                first = false;
                AppendPartJson(oss, pInfo, sheetSet);
        }
        pPartsLib->Leave();

        oss << "]";

        oss << ",\"sheets\":[";
        bool firstSheet = true;
        for (std::set<int>::const_iterator it = sheetSet.begin(); it != sheetSet.end(); ++it) {
                if (!firstSheet) {
                        oss << ',';
                }
                firstSheet = false;
                oss << *it;
        }
        oss << "]}";

        return RemoveUtf8Bom(oss.str());
}

void CMapPartsListHandler::AppendPartJson(std::ostringstream &oss, const CInfoMapParts *pInfo, std::set<int> &sheetSet) const
{
        oss << '{';
        oss << "\"partsId\":" << pInfo->m_dwPartsID << ',';
        oss << "\"viewType\":" << static_cast<unsigned int>(pInfo->m_byViewType) << ',';
        oss << "\"animeType\":" << static_cast<unsigned int>(pInfo->m_byAnimeType) << ',';
        oss << "\"animeCount\":" << static_cast<unsigned int>(pInfo->m_byAnimeCount) << ',';
        oss << "\"level\":" << static_cast<unsigned int>(pInfo->m_byLevel) << ',';

        oss << "\"viewPosition\":{";
        oss << "\"x\":" << pInfo->m_ptViewPos.x << ',';
        oss << "\"y\":" << pInfo->m_ptViewPos.y << "},";

        oss << "\"flags\":{";
        oss << "\"block\":" << ((pInfo->m_dwPartsType & BIT_PARTSHIT_BLOCK) ? "true" : "false") << ',';
        oss << "\"pile\":" << ((pInfo->m_dwPartsType & BIT_PARTSHIT_PILE) ? "true" : "false") << ',';
        oss << "\"pileBack\":" << ((pInfo->m_dwPartsType & BIT_PARTSHIT_PILEBACK) ? "true" : "false") << ',';
        oss << "\"fishing\":" << ((pInfo->m_dwPartsType & BIT_PARTSHIT_FISHING) ? "true" : "false") << ',';
        oss << "\"drawLast\":" << ((pInfo->m_dwPartsType & BIT_PARTSHIT_DRAWLAST) ? "true" : "false") << ',';
        oss << "\"counter\":" << ((pInfo->m_dwPartsType & BIT_PARTSHIT_COUNTER) ? "true" : "false") << ',';
        oss << "\"blockDirections\":{";
        oss << "\"up\":" << ((pInfo->m_byBlockDirection & BIT_PARTSBLOCK_UP) ? "true" : "false") << ',';
        oss << "\"down\":" << ((pInfo->m_byBlockDirection & BIT_PARTSBLOCK_DOWN) ? "true" : "false") << ',';
        oss << "\"left\":" << ((pInfo->m_byBlockDirection & BIT_PARTSBLOCK_LEFT) ? "true" : "false") << ',';
        oss << "\"right\":" << ((pInfo->m_byBlockDirection & BIT_PARTSBLOCK_RIGHT) ? "true" : "false");
        oss << "}";
        oss << "},";

        oss << "\"sprites\":{";
        AppendSpriteJson(oss, "base", pInfo->m_wGrpIDBase, sheetSet);
        oss << ',';
        AppendSpriteJson(oss, "overlay", pInfo->m_wGrpIDPile, sheetSet);
        oss << "},";

        oss << "\"movement\":{";
        oss << "\"direction\":";
        if (pInfo->m_byMoveDirection & BIT_PARTSMOVE_UP) {
                oss << "\"up\"";
        } else if (pInfo->m_byMoveDirection & BIT_PARTSMOVE_DOWN) {
                oss << "\"down\"";
        } else if (pInfo->m_byMoveDirection & BIT_PARTSMOVE_LEFT) {
                oss << "\"left\"";
        } else if (pInfo->m_byMoveDirection & BIT_PARTSMOVE_RIGHT) {
                oss << "\"right\"";
        } else {
                oss << "null";
        }
        oss << "},";

        AppendAnimeFramesJson(oss, pInfo);

        oss << '}';
}

void CMapPartsListHandler::AppendSpriteJson(std::ostringstream &oss, const char *pszKey, unsigned int grpId, std::set<int> &sheetSet) const
{
        oss << "\"" << pszKey << "\":";
        if (grpId == 0) {
                oss << "null";
                return;
        }

        unsigned int sheet = grpId / 1024U;
        unsigned int tile = grpId % 1024U;
        unsigned int tileX = tile % kSheetTileWidth;
        unsigned int tileY = tile / kSheetTileWidth;
        sheetSet.insert(static_cast<int>(sheet));

        oss << '{';
        oss << "\"sheet\":" << sheet << ',';
        oss << "\"tile\":" << tile << ',';
        oss << "\"tileX\":" << tileX << ',';
        oss << "\"tileY\":" << tileY;
        oss << '}';
}

CMapPartsSheetHandler::CMapPartsSheetHandler(std::shared_ptr<CMapPartsResourceProvider> provider, std::string pathPrefix)
        : m_provider(provider)
        , m_pathPrefix(pathPrefix)
{
}

void CMapPartsSheetHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        if (!m_provider) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"resource_unavailable\"}");
                return;
        }

        int sheetIndex = -1;
        if (!TryParseSheetIndex(request.path, sheetIndex)) {
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"sheet_not_found\"}");
                return;
        }

        std::vector<unsigned char> png;
        if (!m_provider->GetSheetPng(sheetIndex, png)) {
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"sheet_not_found\"}");
                return;
        }

        response.statusLine = "HTTP/1.1 200 OK";
        response.body.assign(reinterpret_cast<const char *>(png.data()), reinterpret_cast<const char *>(png.data()) + png.size());
        response.SetHeader("Content-Type", "image/png");
        response.SetHeader("Cache-Control", "public, max-age=86400");
}

bool CMapPartsSheetHandler::TryParseSheetIndex(const std::string &path, int &sheetIndex) const
{
        std::string fullPath = path;
        size_t queryPos = fullPath.find('?');
        if (queryPos != std::string::npos) {
                fullPath = fullPath.substr(0, queryPos);
        }

        if (fullPath.size() <= m_pathPrefix.size()) {
                return false;
        }

        std::string suffix = fullPath.substr(m_pathPrefix.size());
        if (suffix.empty()) {
                return false;
        }

        size_t slashPos = suffix.find('/');
        if (slashPos != std::string::npos) {
                suffix = suffix.substr(0, slashPos);
        }

        if (suffix.size() > 4) {
                std::string extension = suffix.substr(suffix.size() - 4);
                for (size_t i = 0; i < extension.size(); ++i) {
                        extension[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(extension[i])));
                }
                if (extension == ".png") {
                        suffix.erase(suffix.size() - 4);
                }
        }

        if (suffix.empty()) {
                return false;
        }

        char *pEnd = NULL;
        long value = std::strtol(suffix.c_str(), &pEnd, 10);
        if ((pEnd == NULL) || (*pEnd != '\0')) {
                return false;
        }
        if (value < 0 || value > 1000) {
                return false;
        }
        sheetIndex = static_cast<int>(value);
        return true;
}

namespace
{
void ParsePartsFlags(const std::string &json, DWORD &dwPartsType, BYTE &byBlockDirection)
{
        bool bFlag = false;
        dwPartsType = 0;
        byBlockDirection = 0;

        if (JsonUtils::TryGetBool(json, "block", bFlag) && bFlag) {
                dwPartsType |= BIT_PARTSHIT_BLOCK;
        }
        if (JsonUtils::TryGetBool(json, "pile", bFlag) && bFlag) {
                dwPartsType |= BIT_PARTSHIT_PILE;
        }
        if (JsonUtils::TryGetBool(json, "pileBack", bFlag) && bFlag) {
                dwPartsType |= BIT_PARTSHIT_PILEBACK;
        }
        if (JsonUtils::TryGetBool(json, "fishing", bFlag) && bFlag) {
                dwPartsType |= BIT_PARTSHIT_FISHING;
        }
        if (JsonUtils::TryGetBool(json, "drawLast", bFlag) && bFlag) {
                dwPartsType |= BIT_PARTSHIT_DRAWLAST;
        }
        if (JsonUtils::TryGetBool(json, "counter", bFlag) && bFlag) {
                dwPartsType |= BIT_PARTSHIT_COUNTER;
        }

        if (JsonUtils::TryGetBool(json, "blockUp", bFlag) && bFlag) {
                byBlockDirection |= BIT_PARTSBLOCK_UP;
        }
        if (JsonUtils::TryGetBool(json, "blockDown", bFlag) && bFlag) {
                byBlockDirection |= BIT_PARTSBLOCK_DOWN;
        }
        if (JsonUtils::TryGetBool(json, "blockLeft", bFlag) && bFlag) {
                byBlockDirection |= BIT_PARTSBLOCK_LEFT;
        }
        if (JsonUtils::TryGetBool(json, "blockRight", bFlag) && bFlag) {
                byBlockDirection |= BIT_PARTSBLOCK_RIGHT;
        }
}

void ParseMoveDirection(const std::string &json, BYTE &byMoveDirection)
{
        byMoveDirection = 0;
        std::string direction;
        if (!JsonUtils::TryGetString(json, "moveDirection", direction)) {
                return;
        }
        if (direction == "up") {
                byMoveDirection = BIT_PARTSMOVE_UP;
        } else if (direction == "down") {
                byMoveDirection = BIT_PARTSMOVE_DOWN;
        } else if (direction == "left") {
                byMoveDirection = BIT_PARTSMOVE_LEFT;
        } else if (direction == "right") {
                byMoveDirection = BIT_PARTSMOVE_RIGHT;
        }
}

void ApplyJsonToPartsInfo(const std::string &json, CInfoMapParts *pInfo)
{
        int nVal = 0;
        if (JsonUtils::TryGetInt(json, "viewType", nVal)) {
                pInfo->m_byViewType = static_cast<BYTE>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "animeType", nVal)) {
                pInfo->m_byAnimeType = static_cast<BYTE>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "level", nVal)) {
                pInfo->m_byLevel = static_cast<BYTE>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "grpIdBase", nVal)) {
                pInfo->m_wGrpIDBase = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "grpIdPile", nVal)) {
                pInfo->m_wGrpIDPile = static_cast<WORD>(nVal);
        }
        if (JsonUtils::TryGetInt(json, "viewPositionX", nVal)) {
                pInfo->m_ptViewPos.x = nVal;
        }
        if (JsonUtils::TryGetInt(json, "viewPositionY", nVal)) {
                pInfo->m_ptViewPos.y = nVal;
        }

        DWORD dwPartsType = 0;
        BYTE byBlockDirection = 0;
        ParsePartsFlags(json, dwPartsType, byBlockDirection);
        pInfo->m_dwPartsType = dwPartsType;
        pInfo->m_byBlockDirection = byBlockDirection;

        BYTE byMoveDirection = 0;
        ParseMoveDirection(json, byMoveDirection);
        pInfo->m_byMoveDirection = byMoveDirection;

        // animeFrames 配列のパース（"animeFrames":[{...},{...}]）
        size_t nFramesKey = JsonUtils::FindKey(json, "animeFrames");
        if (nFramesKey != std::string::npos) {
                size_t nArrayStart = json.find('[', nFramesKey);
                size_t nArrayEnd = json.find(']', nArrayStart);
                if (nArrayStart != std::string::npos && nArrayEnd != std::string::npos) {
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
        }
}

void BuildSinglePartJson(std::ostringstream &oss, const CInfoMapParts *pInfo)
{
        oss << '{';
        oss << "\"partsId\":" << pInfo->m_dwPartsID << ',';
        oss << "\"viewType\":" << static_cast<unsigned int>(pInfo->m_byViewType) << ',';
        oss << "\"animeType\":" << static_cast<unsigned int>(pInfo->m_byAnimeType) << ',';
        oss << "\"animeCount\":" << static_cast<unsigned int>(pInfo->m_byAnimeCount) << ',';
        oss << "\"level\":" << static_cast<unsigned int>(pInfo->m_byLevel) << ',';
        oss << "\"grpIdBase\":" << static_cast<unsigned int>(pInfo->m_wGrpIDBase) << ',';
        oss << "\"grpIdPile\":" << static_cast<unsigned int>(pInfo->m_wGrpIDPile) << ',';

        oss << "\"viewPosition\":{";
        oss << "\"x\":" << pInfo->m_ptViewPos.x << ',';
        oss << "\"y\":" << pInfo->m_ptViewPos.y << "},";

        oss << "\"flags\":{";
        oss << "\"block\":" << ((pInfo->m_dwPartsType & BIT_PARTSHIT_BLOCK) ? "true" : "false") << ',';
        oss << "\"pile\":" << ((pInfo->m_dwPartsType & BIT_PARTSHIT_PILE) ? "true" : "false") << ',';
        oss << "\"pileBack\":" << ((pInfo->m_dwPartsType & BIT_PARTSHIT_PILEBACK) ? "true" : "false") << ',';
        oss << "\"fishing\":" << ((pInfo->m_dwPartsType & BIT_PARTSHIT_FISHING) ? "true" : "false") << ',';
        oss << "\"drawLast\":" << ((pInfo->m_dwPartsType & BIT_PARTSHIT_DRAWLAST) ? "true" : "false") << ',';
        oss << "\"counter\":" << ((pInfo->m_dwPartsType & BIT_PARTSHIT_COUNTER) ? "true" : "false") << ',';
        oss << "\"blockDirections\":{";
        oss << "\"up\":" << ((pInfo->m_byBlockDirection & BIT_PARTSBLOCK_UP) ? "true" : "false") << ',';
        oss << "\"down\":" << ((pInfo->m_byBlockDirection & BIT_PARTSBLOCK_DOWN) ? "true" : "false") << ',';
        oss << "\"left\":" << ((pInfo->m_byBlockDirection & BIT_PARTSBLOCK_LEFT) ? "true" : "false") << ',';
        oss << "\"right\":" << ((pInfo->m_byBlockDirection & BIT_PARTSBLOCK_RIGHT) ? "true" : "false");
        oss << "}},";

        oss << "\"movement\":{\"direction\":";
        if (pInfo->m_byMoveDirection & BIT_PARTSMOVE_UP) {
                oss << "\"up\"";
        } else if (pInfo->m_byMoveDirection & BIT_PARTSMOVE_DOWN) {
                oss << "\"down\"";
        } else if (pInfo->m_byMoveDirection & BIT_PARTSMOVE_LEFT) {
                oss << "\"left\"";
        } else if (pInfo->m_byMoveDirection & BIT_PARTSMOVE_RIGHT) {
                oss << "\"right\"";
        } else {
                oss << "null";
        }
        oss << "},";

        AppendAnimeFramesJson(oss, pInfo);

        oss << '}';
}
}

CMapPartsUpdateHandler::CMapPartsUpdateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapPartsUpdateHandler::Handle(const HttpRequest &request, HttpResponse &response)
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

        CLibInfoMapParts *pPartsLib = m_pMgrData->GetLibInfoMapParts();
        CUraraSockTCPSBO *pSock = m_pMgrData->GetSock();
        if (pPartsLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        const std::string &json = request.body;
        int nPartsId = 0;
        if (!JsonUtils::TryGetInt(json, "partsId", nPartsId) || nPartsId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_partsId\"}");
                return;
        }

        pPartsLib->Enter();

        PCInfoMapParts pInfo = (PCInfoMapParts)pPartsLib->GetPtr(static_cast<DWORD>(nPartsId));
        if (pInfo == NULL) {
                pPartsLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"parts_not_found\"}");
                return;
        }

        ApplyJsonToPartsInfo(json, pInfo);

        CPacketMAP_MAPPARTS packet;
        packet.Make(pInfo);
        pSock->SendTo(0, &packet);

        std::ostringstream oss;
        BuildSinglePartJson(oss, pInfo);

        pPartsLib->Leave();

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

CMapPartsCreateHandler::CMapPartsCreateHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapPartsCreateHandler::Handle(const HttpRequest &request, HttpResponse &response)
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

        CLibInfoMapParts *pPartsLib = m_pMgrData->GetLibInfoMapParts();
        CUraraSockTCPSBO *pSock = m_pMgrData->GetSock();
        if (pPartsLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        pPartsLib->Enter();

        PCInfoMapParts pInfo = (PCInfoMapParts)pPartsLib->GetNew();
        if (pInfo == NULL) {
                pPartsLib->Leave();
                response.statusLine = "HTTP/1.1 500 Internal Server Error";
                response.SetJsonBody("{\"error\":\"failed_to_create_parts\"}");
                return;
        }

        ApplyJsonToPartsInfo(request.body, pInfo);
        pPartsLib->Add(pInfo);

        CPacketMAP_MAPPARTS packet;
        packet.Make(pInfo);
        pSock->SendTo(0, &packet);

        std::ostringstream oss;
        BuildSinglePartJson(oss, pInfo);

        pPartsLib->Leave();

        response.statusLine = "HTTP/1.1 201 Created";
        response.SetJsonBody(oss.str());
}

CMapPartsDeleteHandler::CMapPartsDeleteHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapPartsDeleteHandler::Handle(const HttpRequest &request, HttpResponse &response)
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

        CLibInfoMapParts *pPartsLib = m_pMgrData->GetLibInfoMapParts();
        CLibInfoMapBase *pMapLib = m_pMgrData->GetLibInfoMap();
        CUraraSockTCPSBO *pSock = m_pMgrData->GetSock();
        if (pPartsLib == NULL || pMapLib == NULL || pSock == NULL) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        const std::string &json = request.body;
        int nPartsId = 0;
        if (!JsonUtils::TryGetInt(json, "partsId", nPartsId) || nPartsId <= 0) {
                response.statusLine = "HTTP/1.1 400 Bad Request";
                response.SetJsonBody("{\"error\":\"missing_or_invalid_partsId\"}");
                return;
        }

        DWORD dwPartsId = static_cast<DWORD>(nPartsId);

        pPartsLib->Enter();

        PCInfoMapParts pInfo = (PCInfoMapParts)pPartsLib->GetPtr(dwPartsId);
        if (pInfo == NULL) {
                pPartsLib->Leave();
                response.statusLine = "HTTP/1.1 404 Not Found";
                response.SetJsonBody("{\"error\":\"parts_not_found\"}");
                return;
        }

        pMapLib->DeleteParts(dwPartsId);
        pPartsLib->Delete(dwPartsId);

        CPacketMAP_DELETEPARTS packet;
        packet.Make(dwPartsId);
        pSock->SendTo(0, &packet);

        pPartsLib->Leave();

        std::ostringstream oss;
        oss << "{\"deleted\":" << dwPartsId << "}";
        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

// --- マップパーツ配置の Undo/Redo 履歴 API ---

namespace
{
// 履歴変化を管理画面の他クライアントへ WebSocket でブロードキャストする
void BroadcastHistoryChanged()
{
        int nUndoCount = 0, nRedoCount = 0;
        CMapPartsHistory::Instance().GetCounts(nUndoCount, nRedoCount);
        CAdminWsHub::Instance().BroadcastJson(
                std::string("{\"kind\":\"map_parts_history\",\"payload\":") +
                CMapPartsHistory::BuildCountsJson(nUndoCount, nRedoCount) +
                std::string("}"));
}

// Undo/Redo 共通の適用処理。dwPartsId を復元/再適用してゲーム側へブロードキャストする。
void ApplyHistoryEntry(CMgrData *pMgrData, const CMapPartsHistory::Entry &entry, DWORD dwPartsId)
{
        CLibInfoMapBase *pMapLib = pMgrData->GetLibInfoMap();
        CUraraSockTCPSBO *pSock = pMgrData->GetSock();
        if (pMapLib == NULL || pSock == NULL) {
                return;
        }

        PCInfoMapBase pInfoMap = (PCInfoMapBase)pMapLib->GetPtr(entry.mapId);
        if (pInfoMap == NULL) {
                return;
        }

        if (entry.pile) {
                pInfoMap->SetPartsPile(entry.x, entry.y, dwPartsId);
        } else {
                pInfoMap->SetParts(entry.x, entry.y, dwPartsId);
        }

        CPacketMAP_SETPARTS packet;
        packet.Make(entry.mapId, entry.x, entry.y, dwPartsId, entry.pile ? TRUE : FALSE);
        pSock->SendTo(0, &packet);
}
}

CMapPartsHistoryStatusHandler::CMapPartsHistoryStatusHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapPartsHistoryStatusHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
        AuthProvider::AuthContext authContext;
        AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, m_pMgrData, authContext);
        if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
                response.statusLine = "HTTP/1.1 503 Service Unavailable";
                response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
                return;
        }

        int nUndoCount = 0, nRedoCount = 0;
        CMapPartsHistory::Instance().GetCounts(nUndoCount, nRedoCount);

        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(CMapPartsHistory::BuildCountsJson(nUndoCount, nRedoCount));
}

CMapPartsHistoryUndoHandler::CMapPartsHistoryUndoHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapPartsHistoryUndoHandler::Handle(const HttpRequest &request, HttpResponse &response)
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

        CMapPartsHistory::Entry entry;
        if (!CMapPartsHistory::Instance().Undo(entry)) {
                int nUndoCount = 0, nRedoCount = 0;
                CMapPartsHistory::Instance().GetCounts(nUndoCount, nRedoCount);
                std::ostringstream oss;
                oss << "{\"applied\":false,\"reason\":\"empty\",\"undoCount\":" << nUndoCount
                    << ",\"redoCount\":" << nRedoCount << "}";
                response.statusLine = "HTTP/1.1 200 OK";
                response.SetJsonBody(oss.str());
                return;
        }

        CLibInfoMapBase *pMapLib = m_pMgrData->GetLibInfoMap();
        if (pMapLib == NULL || pMapLib->GetPtr(entry.mapId) == NULL) {
                int nUndoCount = 0, nRedoCount = 0;
                CMapPartsHistory::Instance().GetCounts(nUndoCount, nRedoCount);
                std::ostringstream oss;
                oss << "{\"applied\":false,\"reason\":\"map_not_found\",\"undoCount\":" << nUndoCount
                    << ",\"redoCount\":" << nRedoCount << "}";
                response.statusLine = "HTTP/1.1 200 OK";
                response.SetJsonBody(oss.str());
                return;
        }

        // Undo: 変更前の値（oldPartsId）へ戻す
        ApplyHistoryEntry(m_pMgrData, entry, entry.oldPartsId);

        int nUndoCount = 0, nRedoCount = 0;
        CMapPartsHistory::Instance().GetCounts(nUndoCount, nRedoCount);
        BroadcastHistoryChanged();

        std::ostringstream oss;
        oss << "{\"applied\":true"
            << ",\"mapId\":" << entry.mapId
            << ",\"x\":" << entry.x
            << ",\"y\":" << entry.y
            << ",\"pile\":" << (entry.pile ? "true" : "false")
            << ",\"partsId\":" << entry.oldPartsId
            << ",\"undoCount\":" << nUndoCount
            << ",\"redoCount\":" << nRedoCount
            << "}";
        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}

CMapPartsHistoryRedoHandler::CMapPartsHistoryRedoHandler(CMgrData *pMgrData)
        : m_pMgrData(pMgrData)
{
}

void CMapPartsHistoryRedoHandler::Handle(const HttpRequest &request, HttpResponse &response)
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

        CMapPartsHistory::Entry entry;
        if (!CMapPartsHistory::Instance().Redo(entry)) {
                int nUndoCount = 0, nRedoCount = 0;
                CMapPartsHistory::Instance().GetCounts(nUndoCount, nRedoCount);
                std::ostringstream oss;
                oss << "{\"applied\":false,\"reason\":\"empty\",\"undoCount\":" << nUndoCount
                    << ",\"redoCount\":" << nRedoCount << "}";
                response.statusLine = "HTTP/1.1 200 OK";
                response.SetJsonBody(oss.str());
                return;
        }

        CLibInfoMapBase *pMapLib = m_pMgrData->GetLibInfoMap();
        if (pMapLib == NULL || pMapLib->GetPtr(entry.mapId) == NULL) {
                int nUndoCount = 0, nRedoCount = 0;
                CMapPartsHistory::Instance().GetCounts(nUndoCount, nRedoCount);
                std::ostringstream oss;
                oss << "{\"applied\":false,\"reason\":\"map_not_found\",\"undoCount\":" << nUndoCount
                    << ",\"redoCount\":" << nRedoCount << "}";
                response.statusLine = "HTTP/1.1 200 OK";
                response.SetJsonBody(oss.str());
                return;
        }

        // Redo: 変更後の値（newPartsId）を再適用する
        ApplyHistoryEntry(m_pMgrData, entry, entry.newPartsId);

        int nUndoCount = 0, nRedoCount = 0;
        CMapPartsHistory::Instance().GetCounts(nUndoCount, nRedoCount);
        BroadcastHistoryChanged();

        std::ostringstream oss;
        oss << "{\"applied\":true"
            << ",\"mapId\":" << entry.mapId
            << ",\"x\":" << entry.x
            << ",\"y\":" << entry.y
            << ",\"pile\":" << (entry.pile ? "true" : "false")
            << ",\"partsId\":" << entry.newPartsId
            << ",\"undoCount\":" << nUndoCount
            << ",\"redoCount\":" << nRedoCount
            << "}";
        response.statusLine = "HTTP/1.1 200 OK";
        response.SetJsonBody(oss.str());
}
