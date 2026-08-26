#include "StdAfx.h"
#include "SpriteSheetHandler.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cwchar>
#include <iomanip>
#include <sstream>

#include "lodepng.h"

namespace
{

// char* (ASCII 前提) を wstring に変換する簡易ヘルパ。
// レイアウト定義テーブル (Common/GrpLayout.h) は char で持つが、
// FindResourceW / _snwprintf_s は wchar_t を要求するためここで変換する。
std::wstring ToWString(const char *pszSrc)
{
    std::wstring out;
    if (pszSrc == NULL) {
        return out;
    }
    for (const char *p = pszSrc; *p != '\0'; ++p) {
        out.push_back(static_cast<wchar_t>(static_cast<unsigned char>(*p)));
    }
    return out;
}

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

    const SGrpLayoutDef *pCat = FindCategory(categoryKey);
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

    const SGrpLayoutDef *pCat = FindCategory(categoryKey);
    if (pCat == NULL) {
        return 0;
    }

    std::map<std::string, int>::const_iterator it = m_sheetCountCache.find(categoryKey);
    if (it != m_sheetCountCache.end()) {
        return it->second;
    }

    // リソース存在プローブで遅延カウント
    int count = 0;
    if (pCat->ppszFixedNames != NULL) {
        // 固定名テーブル: NULL になるまで body/ear の組で走査
        for (int i = 0; pCat->ppszFixedNames[i] != NULL; i += 2) {
            std::wstring name = ToWString(pCat->ppszFixedNames[i]);
            HRSRC hResInfo = FindResourceW(m_hModule, name.c_str(), L"PNG");
            if (hResInfo == NULL) {
                break;
            }
            ++count;
        }
    } else if (pCat->pszResPattern != NULL) {
        // 単一固定名（パターンに %02d が含まれない場合は1枚）
        std::wstring pattern = ToWString(pCat->pszResPattern);
        if (pattern.find(L'%') == std::wstring::npos) {
            HRSRC hResInfo = FindResourceW(m_hModule, pattern.c_str(), L"PNG");
            count = (hResInfo != NULL) ? 1 : 0;
        } else {
            while (true) {
                wchar_t szName[64] = {};
                _snwprintf_s(szName, _countof(szName), _TRUNCATE, pattern.c_str(), count + 1);
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
    const SGrpLayoutDef *pCat = FindCategory(std::string(pszKey));
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

const SGrpLayoutDef *CGrpResourceProvider::FindCategory(const std::string &key) const
{
    const SGrpLayoutDef *pDef = GrpLayout_FindByKey(key.c_str());
    if (pDef == NULL) {
        return NULL;
    }
    // "none"（cellSize == 0）は疑似カテゴリの並び順維持のためだけの
    // プレースホルダで、旧 kSpriteCategories[] には存在しなかった。
    // 従来どおり「レイアウトなし」として扱う。
    if (pDef->nCellSize == 0) {
        return NULL;
    }
    return pDef;
}

bool CGrpResourceProvider::BuildResourceName(
    const SGrpLayoutDef &cat,
    int sheetIndex,
    std::wstring &outName) const
{
    if (cat.ppszFixedNames != NULL) {
        // 固定名テーブル: 偶数位置が本体名・NULL終端。
        // sheetIndex*2 が終端を超える範囲外参照を防ぐため、
        // 0 から idx まで偶数位置を走査し、終端(NULL)に達したら範囲外とみなす。
        int idx = sheetIndex * 2;
        for (int j = 0; j <= idx; j += 2) {
            if (cat.ppszFixedNames[j] == NULL) {
                return false;  // 終端に到達 = sheetIndex が範囲外
            }
        }
        outName = ToWString(cat.ppszFixedNames[idx]);
        return true;
    }
    if (cat.pszResPattern == NULL) {
        return false;
    }
    std::wstring pattern = ToWString(cat.pszResPattern);
    if (pattern.find(L'%') == std::wstring::npos) {
        // 固定名（IDP_ICON 等）
        if (sheetIndex != 0) {
            return false;
        }
        outName = pattern;
        return true;
    }
    // 番号付き
    wchar_t szName[64] = {};
    _snwprintf_s(szName, _countof(szName), _TRUNCATE, pattern.c_str(), sheetIndex + cat.nFirstResourceIndex);
    outName = szName;
    return true;
}

bool CGrpResourceProvider::LoadSheetLocked(
    const SGrpLayoutDef &cat,
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
