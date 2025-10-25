#include "stdafx.h"
#include "MapPartsHandler.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cwchar>
#include <iomanip>

#include "MgrData.h"
#include "Web/AuthProvider.h"
#include "Web/JsonUtils.h"
#include "Info/InfoMapParts.h"
#include "LibInfo/LibInfoMapParts.h"

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
}

CMapPartsResourceProvider::CMapPartsResourceProvider()
        : m_hModule(NULL)
        , m_sheetCount(-1)
{
}

CMapPartsResourceProvider::~CMapPartsResourceProvider()
{
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_hModule != NULL) {
                FreeLibrary(m_hModule);
                m_hModule = NULL;
        }
        m_sheetCache.clear();
}

bool CMapPartsResourceProvider::IsAvailable()
{
        std::lock_guard<std::mutex> lock(m_mutex);
        return EnsureLibraryLocked();
}

bool CMapPartsResourceProvider::GetSheetPng(int sheetIndex, std::vector<unsigned char> &outData)
{
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!EnsureLibraryLocked()) {
                return false;
        }

        std::map<int, std::vector<unsigned char> >::const_iterator it = m_sheetCache.find(sheetIndex);
        if (it != m_sheetCache.end()) {
                outData = it->second;
                return true;
        }

        if (!LoadSheetLocked(sheetIndex, outData)) {
                return false;
        }
        m_sheetCache.insert(std::make_pair(sheetIndex, outData));
        return true;
}

int CMapPartsResourceProvider::GetSheetCount()
{
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!EnsureLibraryLocked()) {
                return 0;
        }
        if (m_sheetCount >= 0) {
                return m_sheetCount;
        }

        int count = 0;
        while (true) {
                std::wstring resourceName;
                wchar_t szName[32] = {};
                _snwprintf_s(szName, _countof(szName), _TRUNCATE, L"IDP_MAP_%02d", count + 1);
                resourceName.assign(szName);
                HRSRC hResInfo = FindResourceW(m_hModule, resourceName.c_str(), L"PNG");
                if (hResInfo == NULL) {
                        break;
                }
                ++count;
        }
        m_sheetCount = count;
        return m_sheetCount;
}

bool CMapPartsResourceProvider::EnsureLibraryLocked()
{
        if (m_hModule != NULL) {
                return true;
        }

        std::wstring modulePath;
        if (!ResolveLibraryPath(modulePath)) {
                return false;
        }
        HMODULE hLoaded = LoadLibraryW(modulePath.c_str());
        if (hLoaded == NULL) {
                return false;
        }
        m_hModule = hLoaded;
        return true;
}

bool CMapPartsResourceProvider::ResolveLibraryPath(std::wstring &outPath) const
{
        wchar_t szModulePath[MAX_PATH];
        DWORD dwLength = GetModuleFileNameW(NULL, szModulePath, MAX_PATH);
        if ((dwLength == 0) || (dwLength >= MAX_PATH)) {
                return false;
        }

        wchar_t *pSlash = wcsrchr(szModulePath, L'\\');
        if (pSlash != NULL) {
                *(pSlash + 1) = L'\0';
        }

        std::wstring candidate = szModulePath;
        candidate.append(L"SboGrpData.dll");

        DWORD dwAttributes = GetFileAttributesW(candidate.c_str());
        if (dwAttributes == INVALID_FILE_ATTRIBUTES) {
                outPath.assign(L"SboGrpData.dll");
                return true;
        }
        outPath = candidate;
        return true;
}

bool CMapPartsResourceProvider::LoadSheetLocked(int sheetIndex, std::vector<unsigned char> &outData)
{
        if (sheetIndex < 0) {
                return false;
        }

        wchar_t szName[32] = {};
        _snwprintf_s(szName, _countof(szName), _TRUNCATE, L"IDP_MAP_%02d", sheetIndex + 1);
        HRSRC hResInfo = FindResourceW(m_hModule, szName, L"PNG");
        if (hResInfo == NULL) {
                return false;
        }
        HGLOBAL hRes = LoadResource(m_hModule, hResInfo);
        if (hRes == NULL) {
                return false;
        }
        DWORD dwResourceSize = SizeofResource(m_hModule, hResInfo);
        if (dwResourceSize == 0) {
                return false;
        }
        const BYTE *pResourceData = static_cast<const BYTE *>(LockResource(hRes));
        if (pResourceData == NULL) {
                return false;
        }

        outData.assign(pResourceData, pResourceData + dwResourceSize);
        return true;
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
        oss << "}";

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
        response.SetHeader("Cache-Control", "public, max-age=60");
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
