#include "StdAfx.h"
#include "SpriteSheetHandler.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cwchar>
#include <iomanip>
#include <sstream>

#include "lodepng.h"

// ---------------------------------------------------------------------------
// カテゴリ定義テーブル
//
// !! MgrGrpData.cpp の GetGrpSize / GetGrpCountX / GetGrpCountY /
//    読込ループと同期が必須 !!
//
// key 文字列は ImageCatalogHandler.cpp の kCategories[] と一致させること。
// リソース名パターンは sprintf_s の書式と同じ wchar_t 版（%02d = 01 始まり）。
// ---------------------------------------------------------------------------

namespace
{

// キャラ体の固定リソース名（body/earの組、NULL終端）
static const wchar_t *const kBodyNames[] = {
    L"IDP_BODY_BST", L"IDP_BODY_BST_EAR",
    L"IDP_BODY_DRK", L"IDP_BODY_DRK_EAR",
    L"IDP_BODY_ELF", L"IDP_BODY_ELF_EAR",
    L"IDP_BODY_HUM", NULL,
    NULL
};

// 2x2 キャラ体の固定リソース名
static const wchar_t *const k2x2BodyNames[] = {
    L"IDP_2X2_BODY_HUM", NULL,
    NULL
};

// !! MgrGrpData.cpp と同期必須 !!
// cellSize / countX / countY は GetGrpSize / GetGrpCountX / GetGrpCountY より転記
static const SSpriteCategoryDef kSpriteCategories[] = {
    // idMain                 key            リソース名パターン       固定名配列    cellSize  countX  countY
    { GRPIDMAIN_CHAR,         "char",        NULL,                    kBodyNames,   16,       32,     1  },
    { GRPIDMAIN_NPC,          "npc",         L"IDP_NPC_%02d",        NULL,         16,       16,     32 },
    { GRPIDMAIN_WEAPON,       "weapon",      L"IDP_WEAPON_%02d",     NULL,         32,       32,     18 },
    { GRPIDMAIN_WEAPON_BOW,   "weaponBow",   L"IDP_WEAPON_BOW_%02d", NULL,         32,       20,     18 },
    { GRPIDMAIN_WEAPON_GLOVE, "weaponGlove", L"IDP_WEAPON_GLOVE_%02d", NULL,       32,       20,     18 },
    { GRPIDMAIN_WEAPON_ETC,   "weaponEtc",   L"IDP_WEAPON_ETC_%02d", NULL,         32,       20,     18 },
    { GRPIDMAIN_EFFECT32,     "effect32",    L"IDP_EFC_32_%02d",     NULL,         32,       16,     16 },
    { GRPIDMAIN_EFFECT64,     "effect64",    L"IDP_EFC_64_%02d",     NULL,         64,       8,      8  },
    { GRPIDMAIN_EFCBALLOON,   "efcBalloon",  L"IDP_BALLOON_%02d",    NULL,         16,       10,     15 },
    { GRPIDMAIN_ICON32,       "icon32",      L"IDP_ICON",            NULL,         16,       20,     20 },
    { GRPIDMAIN_2X2_CHAR,     "char2x2",     NULL,                   k2x2BodyNames,32,       16,     8  },
    { GRPIDMAIN_2X2_CLOTH,    "cloth2x2",    L"IDP_2X2_CLOTH_%02d", NULL,         32,       16,     8  },
    { GRPIDMAIN_2X2_EYE,      "eye2x2",      L"IDP_2X2_EYE_%02d",   NULL,         32,       12,     8  },
    { GRPIDMAIN_2X2_HAIR,     "hair2x2",     L"IDP_2X2_HAIR_%02d",  NULL,         32,       16,     8  },
    { GRPIDMAIN_2X2_SPCLOTH,  "spCloth2x2",  L"IDP_2X2_SP_CLOTH_%02d", NULL,      32,       16,     8  },
    { GRPIDMAIN_2X2_SPHAIR,   "spHair2x2",   L"IDP_2X2_SP_HAIR_%02d", NULL,       32,       16,     8  },
    { GRPIDMAIN_2X2_ARMS,     "arms2x2",     L"IDP_2X2_ARMS_%02d",  NULL,         24,       12,     25 },
    { GRPIDMAIN_2X2_SHIELD,   "shield2x2",   L"IDP_2X2_SHIELD_%02d",NULL,         16,       5,      25 },
    { GRPIDMAIN_2X2_ARMSSP,   "armsSp2x2",   L"IDP_2X2_ARMSSP_%02d",NULL,         24,       11,     25 },
    { GRPIDMAIN_2X2_BOW,      "bow2x2",      L"IDP_2X2_BOW_%02d",   NULL,         24,       11,     25 },
    { GRPIDMAIN_2X2_NPC,      "npc2x2",      L"IDP_2X2_NPC_%02d",   NULL,         32,       16,     8  },
    // マップパーツ / マップ影は GRPIDMAIN_* に列挙値が無いため 0 を使用。
    // key は ImageCatalogHandler.cpp の kCategories[] と一致させること。
    // cellSize / countX / countY は MgrGrpData.cpp の読込ループ相当の設定:
    //   マップパーツ: 16px セル、32x32 タイル / シート
    //   マップ影    : 16px セル、32x32 タイル / シート（IDP_MAP_01 と同サイズ）
    { 0,                      "mapParts",    L"IDP_MAP_%02d",        NULL,         16,       32,     32 },
    { 0,                      "mapShadow",   L"IDP_MAPSHADOW_%02d",  NULL,         16,       32,     32 },
    // アイテム地面画像: IDP_ITEM_%02d (512x512, 16px セル, 横32×縦32)
    { 0,                      "item",        L"IDP_ITEM_%02d",       NULL,         16,       32,     32 },
};

static const size_t kSpriteCategoryCount = sizeof(kSpriteCategories) / sizeof(kSpriteCategories[0]);

} // namespace

// ---------------------------------------------------------------------------
// CGrpResourceProvider
// ---------------------------------------------------------------------------

CGrpResourceProvider::CGrpResourceProvider()
    : m_hModule(NULL)
{
}

CGrpResourceProvider::~CGrpResourceProvider()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_hModule != NULL) {
        FreeLibrary(m_hModule);
        m_hModule = NULL;
    }
    m_sheetCache.clear();
    m_sheetCountCache.clear();
}

// static
CGrpResourceProvider &CGrpResourceProvider::GetInstance()
{
    static CGrpResourceProvider s_instance;
    return s_instance;
}

bool CGrpResourceProvider::GetSheetPng(
    const std::string &categoryKey,
    int sheetIndex,
    std::vector<unsigned char> &outData)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!EnsureLibraryLocked()) {
        return false;
    }

    const SSpriteCategoryDef *pCat = FindCategory(categoryKey);
    if (pCat == NULL) {
        return false;
    }

    std::pair<std::string, int> cacheKey(categoryKey, sheetIndex);
    std::map<std::pair<std::string, int>, std::vector<unsigned char> >::const_iterator it =
        m_sheetCache.find(cacheKey);
    if (it != m_sheetCache.end()) {
        outData = it->second;
        return true;
    }

    if (!LoadSheetLocked(*pCat, sheetIndex, outData)) {
        return false;
    }
    m_sheetCache.insert(std::make_pair(cacheKey, outData));
    return true;
}

int CGrpResourceProvider::GetSheetCount(const std::string &categoryKey)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!EnsureLibraryLocked()) {
        return 0;
    }

    const SSpriteCategoryDef *pCat = FindCategory(categoryKey);
    if (pCat == NULL) {
        return 0;
    }

    std::map<std::string, int>::const_iterator it = m_sheetCountCache.find(categoryKey);
    if (it != m_sheetCountCache.end()) {
        return it->second;
    }

    // リソース存在プローブで遅延カウント
    int count = 0;
    if (pCat->apszFixedNames != NULL) {
        // 固定名テーブル: NULL になるまで body/ear の組で走査
        for (int i = 0; pCat->apszFixedNames[i] != NULL; i += 2) {
            HRSRC hResInfo = FindResourceW(m_hModule, pCat->apszFixedNames[i], L"PNG");
            if (hResInfo == NULL) {
                break;
            }
            ++count;
        }
    } else if (pCat->pszResourcePattern != NULL) {
        // 単一固定名（パターンに %02d が含まれない場合は1枚）
        std::wstring pattern(pCat->pszResourcePattern);
        if (pattern.find(L'%') == std::wstring::npos) {
            HRSRC hResInfo = FindResourceW(m_hModule, pattern.c_str(), L"PNG");
            count = (hResInfo != NULL) ? 1 : 0;
        } else {
            while (true) {
                wchar_t szName[64] = {};
                _snwprintf_s(szName, _countof(szName), _TRUNCATE, pCat->pszResourcePattern, count + 1);
                HRSRC hResInfo = FindResourceW(m_hModule, szName, L"PNG");
                if (hResInfo == NULL) {
                    break;
                }
                ++count;
            }
        }
    }

    m_sheetCountCache.insert(std::make_pair(categoryKey, count));
    return count;
}

bool CGrpResourceProvider::GetCategoryLayout(
    const char *pszKey,
    int &nCellSize,
    int &nCountX,
    int &nCountY) const
{
    const SSpriteCategoryDef *pCat = FindCategory(std::string(pszKey));
    if (pCat == NULL) {
        return false;
    }
    nCellSize = pCat->nCellSize;
    nCountX   = pCat->nCountX;
    nCountY   = pCat->nCountY;
    return true;
}

bool CGrpResourceProvider::EnsureLibraryLocked()
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

bool CGrpResourceProvider::ResolveLibraryPath(std::wstring &outPath) const
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

const SSpriteCategoryDef *CGrpResourceProvider::FindCategory(const std::string &key) const
{
    for (size_t i = 0; i < kSpriteCategoryCount; ++i) {
        if (key == kSpriteCategories[i].pszKey) {
            return &kSpriteCategories[i];
        }
    }
    return NULL;
}

bool CGrpResourceProvider::BuildResourceName(
    const SSpriteCategoryDef &cat,
    int sheetIndex,
    std::wstring &outName) const
{
    if (cat.apszFixedNames != NULL) {
        // 固定名テーブル: sheetIndex*2 番目が本体名
        int idx = sheetIndex * 2;
        if (cat.apszFixedNames[idx] == NULL) {
            return false;
        }
        outName = cat.apszFixedNames[idx];
        return true;
    }
    if (cat.pszResourcePattern == NULL) {
        return false;
    }
    std::wstring pattern(cat.pszResourcePattern);
    if (pattern.find(L'%') == std::wstring::npos) {
        // 固定名（IDP_ICON 等）
        if (sheetIndex != 0) {
            return false;
        }
        outName = pattern;
        return true;
    }
    // 番号付き（1始まり）
    wchar_t szName[64] = {};
    _snwprintf_s(szName, _countof(szName), _TRUNCATE, cat.pszResourcePattern, sheetIndex + 1);
    outName = szName;
    return true;
}

bool CGrpResourceProvider::LoadSheetLocked(
    const SSpriteCategoryDef &cat,
    int sheetIndex,
    std::vector<unsigned char> &outData)
{
    if (sheetIndex < 0) {
        return false;
    }

    std::wstring resourceName;
    if (!BuildResourceName(cat, sheetIndex, resourceName)) {
        return false;
    }

    HRSRC hResInfo = FindResourceW(m_hModule, resourceName.c_str(), L"PNG");
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

    return MakeTransparentPng(pResourceData, static_cast<size_t>(dwResourceSize), outData);
}

bool CGrpResourceProvider::MakeTransparentPng(
    const unsigned char *pSrc,
    size_t nSrcSize,
    std::vector<unsigned char> &outData)
{
    // MapPartsHandler と同じロジック:
    // lodepng でパレット付き PNG をデコードし、インデックス 0 を透過化して再エンコード。

    std::vector<unsigned char> rawPng(pSrc, pSrc + nSrcSize);

    lodepng::State state;
    state.decoder.color_convert = 0;

    std::vector<unsigned char> pixels;
    unsigned int imgW = 0, imgH = 0;
    unsigned int decErr = lodepng::decode(pixels, imgW, imgH, state, rawPng.data(), rawPng.size());
    if (decErr != 0 ||
        state.info_png.color.colortype != LCT_PALETTE ||
        state.info_png.color.palettesize == 0)
    {
        if (decErr != 0) {
            char szMsg[256];
            _snprintf_s(szMsg, _countof(szMsg), _TRUNCATE,
                "SpriteSheetHandler: lodepng decode failed (err=%u), using original PNG\n", decErr);
            OutputDebugStringA(szMsg);
        }
        outData = std::move(rawPng);
        return true;
    }

    state.info_png.color.palette[3] = 0;
    state.info_raw.palette[3]       = 0;
    state.info_raw.colortype        = LCT_PALETTE;
    state.info_raw.bitdepth         = 8;
    state.encoder.auto_convert      = 0;

    std::vector<unsigned char> encodedPng;
    unsigned int encErr = lodepng::encode(encodedPng, pixels, imgW, imgH, state);
    if (encErr != 0) {
        char szMsg[256];
        _snprintf_s(szMsg, _countof(szMsg), _TRUNCATE,
            "SpriteSheetHandler: lodepng encode failed (err=%u), using original PNG\n", encErr);
        OutputDebugStringA(szMsg);
        outData = std::move(rawPng);
        return true;
    }

    outData = std::move(encodedPng);
    return true;
}

// ---------------------------------------------------------------------------
// CSpriteSheetHandler
// ---------------------------------------------------------------------------

CSpriteSheetHandler::CSpriteSheetHandler(std::string pathPrefix)
    : m_pathPrefix(std::move(pathPrefix))
{
}

void CSpriteSheetHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
    std::string categoryKey;
    int sheetIndex = -1;
    if (!TryParsePath(request.path, categoryKey, sheetIndex)) {
        response.statusLine = "HTTP/1.1 404 Not Found";
        response.SetJsonBody("{\"error\":\"sprite_not_found\"}");
        return;
    }

    std::vector<unsigned char> png;
    if (!CGrpResourceProvider::GetInstance().GetSheetPng(categoryKey, sheetIndex, png)) {
        response.statusLine = "HTTP/1.1 404 Not Found";
        response.SetJsonBody("{\"error\":\"sprite_not_found\"}");
        return;
    }

    response.statusLine = "HTTP/1.1 200 OK";
    response.body.assign(
        reinterpret_cast<const char *>(png.data()),
        reinterpret_cast<const char *>(png.data()) + png.size());
    response.SetHeader("Content-Type", "image/png");
    response.SetHeader("Cache-Control", "public, max-age=86400");
}

bool CSpriteSheetHandler::TryParsePath(
    const std::string &path,
    std::string &outKey,
    int &outIndex) const
{
    // /api/assets/sprites/{categoryKey}/{sheetIndex}[.png]
    std::string fullPath = path;
    size_t queryPos = fullPath.find('?');
    if (queryPos != std::string::npos) {
        fullPath = fullPath.substr(0, queryPos);
    }

    if (fullPath.size() <= m_pathPrefix.size()) {
        return false;
    }

    std::string rest = fullPath.substr(m_pathPrefix.size());
    // rest = "{categoryKey}/{sheetIndex}[.png]"

    size_t slashPos = rest.find('/');
    if (slashPos == std::string::npos || slashPos == 0) {
        return false;
    }

    outKey = rest.substr(0, slashPos);
    std::string indexStr = rest.substr(slashPos + 1);

    // 末尾 .png を除去
    if (indexStr.size() > 4) {
        std::string ext = indexStr.substr(indexStr.size() - 4);
        for (size_t i = 0; i < ext.size(); ++i) {
            ext[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(ext[i])));
        }
        if (ext == ".png") {
            indexStr.erase(indexStr.size() - 4);
        }
    }

    // 末尾にさらにスラッシュがある場合は除去
    size_t trailingSlash = indexStr.find('/');
    if (trailingSlash != std::string::npos) {
        indexStr = indexStr.substr(0, trailingSlash);
    }

    if (indexStr.empty()) {
        return false;
    }

    char *pEnd = NULL;
    long value = std::strtol(indexStr.c_str(), &pEnd, 10);
    if ((pEnd == NULL) || (*pEnd != '\0')) {
        return false;
    }
    if (value < 0 || value > 1000) {
        return false;
    }
    outIndex = static_cast<int>(value);
    return true;
}
