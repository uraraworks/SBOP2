#include "StdAfx.h"
#include "SpriteSheetHandler.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cwchar>
#include <iomanip>
#include <sstream>

#include "lodepng.h"

#include "Web/AuthProvider.h"
#include "Web/GrpImageStore.h"
#include "Web/JsonUtils.h"
#include "../../Platform/SvrPlatform.h"

namespace
{

// パス末尾のサフィックス種別。
// /history (GET, 履歴一覧) と /revert (POST, 復元) の2種のみを認識する。
enum ESpriteSheetPathSuffix
{
    SpriteSheetSuffixNone = 0,
    SpriteSheetSuffixHistory,
    SpriteSheetSuffixRevert,
};

// CSpriteSheetHandler / CSpriteSheetUploadHandler / CSpriteSheetHistoryHandler /
// CSpriteSheetRevertHandler / CSpriteSheetDeleteHandler 共通のパス解析ロジック。
// /api/assets/sprites/{categoryKey}/{sheetIndex}[.png][/history|/revert]
//
// 末尾に /history や /revert 以外の余計なセグメントが付いている場合や、
// 未知のサフィックスが付いている場合は false を返す（誤って基本パスとして
// 食われることを防ぐ）。
bool ParseSpriteSheetPathEx(
    const std::string &pathPrefix,
    const std::string &path,
    std::string &outKey,
    int &outIndex,
    ESpriteSheetPathSuffix &outSuffix)
{
    outSuffix = SpriteSheetSuffixNone;

    std::string fullPath = path;
    size_t queryPos = fullPath.find('?');
    if (queryPos != std::string::npos) {
        fullPath = fullPath.substr(0, queryPos);
    }

    if (fullPath.size() <= pathPrefix.size()) {
        return false;
    }

    std::string rest = fullPath.substr(pathPrefix.size());
    // rest = "{categoryKey}/{sheetIndex}[.png][/history|/revert]"

    size_t slashPos = rest.find('/');
    if (slashPos == std::string::npos || slashPos == 0) {
        return false;
    }

    outKey = rest.substr(0, slashPos);
    std::string remaining = rest.substr(slashPos + 1);
    // remaining = "{sheetIndex}[.png][/history|/revert]"

    std::string indexStr = remaining;
    std::string suffixStr;
    size_t nextSlash = remaining.find('/');
    if (nextSlash != std::string::npos) {
        indexStr = remaining.substr(0, nextSlash);
        suffixStr = remaining.substr(nextSlash + 1);
        // サフィックスの後ろにさらにスラッシュが続くパスは未知のものとして拒否する
        if (suffixStr.find('/') != std::string::npos) {
            return false;
        }
    }

    // 末尾 .png を除去（サフィックス無しのときのみ意味を持つが、旧仕様のまま許容する）
    if (indexStr.size() > 4) {
        std::string ext = indexStr.substr(indexStr.size() - 4);
        for (size_t i = 0; i < ext.size(); ++i) {
            ext[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(ext[i])));
        }
        if (ext == ".png") {
            indexStr.erase(indexStr.size() - 4);
        }
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

    if (!suffixStr.empty()) {
        if (suffixStr == "history") {
            outSuffix = SpriteSheetSuffixHistory;
        } else if (suffixStr == "revert") {
            outSuffix = SpriteSheetSuffixRevert;
        } else {
            // 未知のサフィックスは 404 として扱う
            return false;
        }
    }

    return true;
}

// サフィックス無しのみを許可する従来互換のラッパー。
// CSpriteSheetHandler（サフィックス無し部分）/ CSpriteSheetUploadHandler /
// CSpriteSheetDeleteHandler が使う。サフィックス付きパスは false（従来は
// 誤って基本パスとして食われていたが、これを修正する）。
bool ParseSpriteSheetPath(
    const std::string &pathPrefix,
    const std::string &path,
    std::string &outKey,
    int &outIndex)
{
    ESpriteSheetPathSuffix suffix = SpriteSheetSuffixNone;
    if (!ParseSpriteSheetPathEx(pathPrefix, path, outKey, outIndex, suffix)) {
        return false;
    }
    return suffix == SpriteSheetSuffixNone;
}

// char* (ASCII 前提) を wstring に変換する簡易ヘルパ。
// レイアウト定義テーブル (Common/GrpLayout.h) は char で持つが、
// リソース名まわり(BuildResourceName 等)は wstring で扱っているためここで変換する
// （res/ からのファイル読み込み(TryLoadFromFileLocked)や DLL リソース読み出し
// (SboPlatform::LoadEmbeddedPng)は char* をそのまま使うため ToStringA で戻して渡す）。
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

// CSpriteSheetHistoryHandler / CSpriteSheetRevertHandler / CSpriteSheetDeleteHandler
// 共通の認証チェック。CSpriteSheetUploadHandler と同じ作法（IMAGE_EDIT を要求）。
bool AuthorizeImageEdit(
    const HttpRequest &request,
    CMgrData *pMgrData,
    AuthProvider::AuthContext &outContext,
    HttpResponse &response)
{
    AuthProvider::AuthStatus authStatus = AuthProvider::Authenticate(request, pMgrData, outContext);
    if (authStatus == AuthProvider::AuthStatusBackendUnavailable) {
        response.statusLine = "HTTP/1.1 503 Service Unavailable";
        response.SetJsonBody("{\"error\":\"backend_unavailable\"}");
        return false;
    }
    if (authStatus != AuthProvider::AuthStatusOk) {
        response.statusLine = "HTTP/1.1 401 Unauthorized";
        response.SetJsonBody("{\"error\":\"unauthorized\"}");
        return false;
    }
    if (!AuthProvider::HasRole(outContext, "IMAGE_EDIT")) {
        response.statusLine = "HTTP/1.1 403 Forbidden";
        response.SetJsonBody(AuthProvider::BuildForbiddenBody("IMAGE_EDIT"));
        return false;
    }
    return true;
}

// ToWString の逆変換（ASCII 前提）。BuildResourceName が生成する wstring は
// 元々 char テーブルから ToWString したものなのでラウンドトリップできる。
std::string ToStringA(const std::wstring &src)
{
    std::string out;
    out.reserve(src.size());
    for (std::wstring::const_iterator it = src.begin(); it != src.end(); ++it) {
        out.push_back(static_cast<char>(static_cast<unsigned char>(*it & 0xFF)));
    }
    return out;
}

} // namespace

// ---------------------------------------------------------------------------
// CGrpResourceProvider
// ---------------------------------------------------------------------------

CGrpResourceProvider::CGrpResourceProvider()
{
}

CGrpResourceProvider::~CGrpResourceProvider()
{
    std::lock_guard<std::mutex> lock(m_mutex);
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
    std::vector<unsigned char> &outData,
    std::string &outETag)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    const SGrpLayoutDef *pCat = FindCategory(categoryKey);
    if (pCat == NULL) {
        return false;
    }

    std::pair<std::string, int> cacheKey(categoryKey, sheetIndex);
    std::map<std::pair<std::string, int>, SSheetCacheEntry>::const_iterator it =
        m_sheetCache.find(cacheKey);
    if (it != m_sheetCache.end()) {
        outData = it->second.png;
        outETag = it->second.etag;
        return true;
    }

    std::vector<unsigned char> data;
    std::string etag;
    if (!LoadSheetLocked(*pCat, sheetIndex, data, etag)) {
        return false;
    }

    std::wstring resourceName;
    BuildResourceName(*pCat, sheetIndex, resourceName);

    SSheetCacheEntry entry;
    entry.png = data;
    entry.etag = etag;
    entry.resName = ToStringA(resourceName);
    m_sheetCache.insert(std::make_pair(cacheKey, entry));

    outData = std::move(data);
    outETag = etag;
    return true;
}

void CGrpResourceProvider::InvalidateCache(const char *pszResName)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (pszResName == NULL || pszResName[0] == '\0') {
        m_sheetCache.clear();
        m_sheetCountCache.clear();
        return;
    }
    std::string target(pszResName);
    std::map<std::pair<std::string, int>, SSheetCacheEntry>::iterator it = m_sheetCache.begin();
    while (it != m_sheetCache.end()) {
        if (it->second.resName == target) {
            std::map<std::pair<std::string, int>, SSheetCacheEntry>::iterator toErase = it;
            ++it;
            m_sheetCache.erase(toErase);
        } else {
            ++it;
        }
    }
    // resName 単位ではどのカテゴリのシート数が変わったか安価に特定できないため、
    // シート数キャッシュは全破棄する（次回 GetSheetCount で再プローブされるだけで、
    // 画像ストア導入前の DLL プローブと比べてもコスト差は小さい）。
    m_sheetCountCache.clear();
}

bool CGrpResourceProvider::SheetExistsLocked(const std::wstring &resourceName)
{
    // 読み取り側（LoadSheetLocked）と同じ判定順・同じ存在条件：
    // 画像ストア(DB) → ファイル(res/) → DLL リソースのいずれかで見つかれば「存在する」。
    std::vector<unsigned char> dummyData;
    std::string dummyETag;
    return TryLoadFromImageStoreLocked(resourceName, dummyData, dummyETag) ||
           TryLoadFromFileLocked(resourceName, dummyData, dummyETag) ||
           TryLoadFromDllLocked(resourceName, dummyData, dummyETag);
}

int CGrpResourceProvider::GetSheetCount(const std::string &categoryKey)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    const SGrpLayoutDef *pCat = FindCategory(categoryKey);
    if (pCat == NULL) {
        return 0;
    }

    std::map<std::string, int>::const_iterator it = m_sheetCountCache.find(categoryKey);
    if (it != m_sheetCountCache.end()) {
        return it->second;
    }

    // 存在確認プローブで遅延カウント。BuildResourceName が固定名テーブル/番号パターンの
    // 両方を sheetIndex から一意に導出できるため、経路を問わずインデックスを 0 から
    // 順に走査し、見つからない番号が出た時点で打ち切る（連番カテゴリの挙動を維持）。
    // 番号パターンのカテゴリでは BuildResourceName は常に成功するため、
    // SheetExistsLocked が万一常に true を返す状況になると無限ループでサーバーが
    // 停止する（単一スレッド設計のため全リクエストが止まる）。暴走時の保険として
    // sanity cap を設ける。通常のカテゴリは数枚〜数十枚なので実運用でこの上限が
    // 効くことはない。
    static const int kMaxSheetProbe = 4096;
    int count = 0;
    while (count < kMaxSheetProbe) {
        std::wstring resourceName;
        if (!BuildResourceName(*pCat, count, resourceName)) {
            break;
        }
        if (!SheetExistsLocked(resourceName)) {
            break;
        }
        ++count;
    }
    if (count >= kMaxSheetProbe) {
        char szMsg[128];
        _snprintf_s(szMsg, _countof(szMsg), _TRUNCATE,
            "CGrpResourceProvider::GetSheetCount: probe limit reached (category=%s)\n",
            categoryKey.c_str());
        SboPlatform::WriteDebugLine(szMsg);
    }

    m_sheetCountCache.insert(std::make_pair(categoryKey, count));
    return count;
}

bool CGrpResourceProvider::ResolveResourceName(
    const std::string &categoryKey,
    int sheetIndex,
    std::string &outResName) const
{
    const SGrpLayoutDef *pCat = FindCategory(categoryKey);
    if (pCat == NULL) {
        return false;
    }
    std::wstring resourceName;
    if (!BuildResourceName(*pCat, sheetIndex, resourceName)) {
        return false;
    }
    outResName = ToStringA(resourceName);
    return true;
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
    // _snwprintf_s は MSVC 方言で em++ に無いため、Common/Platform/TCharCompat.h
    // が非Windows 向けに用意している _stprintf_s(バッファサイズ付き) を使う
    // (Windows では実体の MSVC 版が使われるので挙動は変わらない)。
    wchar_t szName[64] = {};
    _stprintf_s(szName, _countof(szName), pattern.c_str(), sheetIndex + cat.nFirstResourceIndex);
    outName = szName;
    return true;
}

bool CGrpResourceProvider::LoadSheetLocked(
    const SGrpLayoutDef &cat,
    int sheetIndex,
    std::vector<unsigned char> &outData,
    std::string &outETag)
{
    if (sheetIndex < 0) {
        return false;
    }

    std::wstring resourceName;
    if (!BuildResourceName(cat, sheetIndex, resourceName)) {
        return false;
    }

    // 3段フォールバック: 画像ストア(DB) → ファイル(res/) → DLL リソース。
    // いずれの経路でも生 PNG バイト列を取得したら、同じ MakeTransparentPng に通す。
    std::vector<unsigned char> rawPng;
    std::string etag;
    bool bFound =
        TryLoadFromImageStoreLocked(resourceName, rawPng, etag) ||
        TryLoadFromFileLocked(resourceName, rawPng, etag) ||
        TryLoadFromDllLocked(resourceName, rawPng, etag);
    if (!bFound) {
        return false;
    }

    if (!MakeTransparentPng(rawPng.data(), rawPng.size(), outData)) {
        return false;
    }
    outETag = etag;
    return true;
}

bool CGrpResourceProvider::TryLoadFromImageStoreLocked(
    const std::wstring &resourceName,
    std::vector<unsigned char> &outRawPng,
    std::string &outETag)
{
    // resourceName は ASCII 前提（BuildResourceName が char テーブルから作る）ため char へ戻す
    std::string resNameA = ToStringA(resourceName);
    return CGrpImageStore::GetInstance().GetPng(resNameA.c_str(), outRawPng, outETag);
}

bool CGrpResourceProvider::TryLoadFromFileLocked(
    const std::wstring &resourceName,
    std::vector<unsigned char> &outRawPng,
    std::string &outETag)
{
    std::string resNameA = ToStringA(resourceName);
    const char *pszRelFileName = GrpLayout_GetResFileName(resNameA.c_str());
    if (pszRelFileName == NULL) {
        return false;
    }

    // 実行ファイルのディレクトリを基準に候補パスを走査する
    // （SboCli/src/MgrGrpData.cpp の FindSboGrpResBasePath と同じ流儀）
    // 実行ファイルのディレクトリ取得は SboPlatform::GetExeDirectory() に集約済み
    // (末尾は区切り文字で終わる)
    std::string strModulePath = SboPlatform::GetExeDirectory();

    // kGrpResFileTable (Common/GrpLayout.cpp) は Windows 形式('\\'区切り)で
    // ファイル名を持つため、非Windows でも fopen が解釈できるよう
    // SboPlatform::GetPathSeparator() に合わせて変換する
    // （SboCli/src/MgrGrpData.cpp の GetFileNameForResource と同じ流儀）。
    std::string relFileName = pszRelFileName;
    char chSep = SboPlatform::GetPathSeparator();
    if (chSep != '\\') {
        for (std::string::iterator it = relFileName.begin(); it != relFileName.end(); ++it) {
            if (*it == '\\') {
                *it = chSep;
            }
        }
    }

    std::string strBaseCandidates[4];
    strBaseCandidates[0] = std::string(".") + chSep + "SboGrpData" + chSep + "res" + chSep;
    strBaseCandidates[1] = std::string("..") + chSep + "SboGrpData" + chSep + "res" + chSep;
    strBaseCandidates[2] = std::string("..") + chSep + ".." + chSep + "SboGrpData" + chSep + "res" + chSep;
    strBaseCandidates[3] = std::string("..") + chSep + ".." + chSep + ".." + chSep + "SboGrpData" + chSep + "res" + chSep;

    for (size_t i = 0; i < _countof(strBaseCandidates); ++i) {
        std::string candidatePath = strModulePath;
        candidatePath.append(strBaseCandidates[i]);
        candidatePath.append(relFileName);

        FILE *pFile = fopen(candidatePath.c_str(), "rb");
        if (pFile == NULL) {
            continue;
        }

        // ファイルサイズ取得(SaveLoadInfoBase.cpp と同じ流儀)
        fseek(pFile, 0, SEEK_END);
        long lSize = ftell(pFile);
        if (lSize <= 0) {
            fclose(pFile);
            continue;
        }
        fseek(pFile, 0, SEEK_SET);

        std::vector<unsigned char> fileData(static_cast<size_t>(lSize));
        size_t nRead = fread(fileData.data(), 1, fileData.size(), pFile);
        fclose(pFile);
        if (nRead != fileData.size()) {
            continue;
        }

        outRawPng = std::move(fileData);

        // ETag: "gf-<サイズの16進>"
        // (旧実装は mtime も含めていたが、SboPlatform に mtime 取得手段が無いため
        //  サイズのみに簡略化。res/ は差し替えが稀で、差し替え時はファイルサイズも
        //  ほぼ変わるため実害は小さい)
        char szEtag[64];
        _snprintf_s(szEtag, _countof(szEtag), _TRUNCATE, "\"gf-%llx\"",
                    static_cast<unsigned long long>(outRawPng.size()));
        outETag = szEtag;
        return true;
    }

    return false;
}

bool CGrpResourceProvider::TryLoadFromDllLocked(
    const std::wstring &resourceName,
    std::vector<unsigned char> &outRawPng,
    std::string &outETag)
{
    // DLL の探索・ロード・FindResourceW 等は Platform 層(SboPlatform::LoadEmbeddedPng)
    // に集約されている。resourceName は ASCII 前提（BuildResourceName が char テーブル
    // から作る）ため char へ戻して渡す。
    std::string resNameA = ToStringA(resourceName);
    if (!SboPlatform::LoadEmbeddedPng(resNameA.c_str(), outRawPng)) {
        return false;
    }

    // ETag: "gr-<サイズの16進>"
    char szEtag[64];
    _snprintf_s(szEtag, _countof(szEtag), _TRUNCATE, "\"gr-%llx\"",
                static_cast<unsigned long long>(outRawPng.size()));
    outETag = szEtag;
    return true;
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
            SboPlatform::WriteDebugLine(szMsg);
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
        SboPlatform::WriteDebugLine(szMsg);
        outData = std::move(rawPng);
        return true;
    }

    outData = std::move(encodedPng);
    return true;
}

// ---------------------------------------------------------------------------
// CSpriteSheetHandler
// ---------------------------------------------------------------------------

CSpriteSheetHandler::CSpriteSheetHandler(std::string pathPrefix, CMgrData *pMgrData)
    : m_pathPrefix(pathPrefix)
    , m_historyHandler(pathPrefix, pMgrData)
{
}

void CSpriteSheetHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
    // /history サフィックス付きパスはこのハンドラでは配信せず、履歴ハンドラへ委譲する。
    // CApiRouter は同一プレフィックス長のルートを1つしか選ばないため、
    // このハンドラ自身が両方のケースを内部で振り分ける（詳細はヘッダのコメント参照）。
    {
        std::string dummyKey;
        int dummyIndex = -1;
        ESpriteSheetPathSuffix suffix = SpriteSheetSuffixNone;
        if (ParseSpriteSheetPathEx(m_pathPrefix, request.path, dummyKey, dummyIndex, suffix) &&
            suffix == SpriteSheetSuffixHistory)
        {
            m_historyHandler.Handle(request, response);
            return;
        }
    }

    std::string categoryKey;
    int sheetIndex = -1;
    if (!TryParsePath(request.path, categoryKey, sheetIndex)) {
        response.statusLine = "HTTP/1.1 404 Not Found";
        response.SetJsonBody("{\"error\":\"sprite_not_found\"}");
        return;
    }

    std::vector<unsigned char> png;
    std::string etag;
    if (!CGrpResourceProvider::GetInstance().GetSheetPng(categoryKey, sheetIndex, png, etag)) {
        response.statusLine = "HTTP/1.1 404 Not Found";
        response.SetJsonBody("{\"error\":\"sprite_not_found\"}");
        return;
    }

    // If-None-Match が一致すれば 304（本文なし）
    if (!etag.empty()) {
        const char *pszIfNoneMatch = request.FindHeader("If-None-Match");
        if (pszIfNoneMatch != NULL && etag == std::string(pszIfNoneMatch)) {
            response.statusLine = "HTTP/1.1 304 Not Modified";
            response.SetHeader("Cache-Control", "no-cache");
            response.SetHeader("ETag", etag);
            response.SetHeader("Content-Length", "0");
            return;
        }
    }

    response.statusLine = "HTTP/1.1 200 OK";
    response.body.assign(
        reinterpret_cast<const char *>(png.data()),
        reinterpret_cast<const char *>(png.data()) + png.size());
    response.SetHeader("Content-Type", "image/png");
    // 画像ストア(DB)から差し替え可能になったため、期限付きキャッシュにはできない。
    // no-cache = 毎回 ETag で再検証し、変化が無ければ 304（本文なし）で済ませる。
    response.SetHeader("Cache-Control", "no-cache");
    if (!etag.empty()) {
        response.SetHeader("ETag", etag);
    }
}

bool CSpriteSheetHandler::TryParsePath(
    const std::string &path,
    std::string &outKey,
    int &outIndex) const
{
    return ParseSpriteSheetPath(m_pathPrefix, path, outKey, outIndex);
}

// ---------------------------------------------------------------------------
// CSpriteSheetUploadHandler
// ---------------------------------------------------------------------------

CSpriteSheetUploadHandler::CSpriteSheetUploadHandler(std::string pathPrefix, CMgrData *pMgrData)
    : m_pathPrefix(std::move(pathPrefix))
    , m_pMgrData(pMgrData)
{
}

bool CSpriteSheetUploadHandler::TryParsePath(
    const std::string &path,
    std::string &outKey,
    int &outIndex) const
{
    return ParseSpriteSheetPath(m_pathPrefix, path, outKey, outIndex);
}

void CSpriteSheetUploadHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
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
    if (!AuthProvider::HasRole(authContext, "IMAGE_EDIT")) {
        response.statusLine = "HTTP/1.1 403 Forbidden";
        response.SetJsonBody(AuthProvider::BuildForbiddenBody("IMAGE_EDIT"));
        return;
    }

    std::string categoryKey;
    int sheetIndex = -1;
    if (!TryParsePath(request.path, categoryKey, sheetIndex)) {
        response.statusLine = "HTTP/1.1 404 Not Found";
        response.SetJsonBody("{\"error\":\"sprite_not_found\"}");
        return;
    }

    const char *pszContentType = request.FindHeader("Content-Type");
    bool bIsPng = false;
    if (pszContentType != NULL) {
        std::string contentType(pszContentType);
        // パラメータ（; charset=... 等）を無視して先頭一致で判定する
        size_t semiPos = contentType.find(';');
        if (semiPos != std::string::npos) {
            contentType = contentType.substr(0, semiPos);
        }
        for (size_t i = 0; i < contentType.size(); ++i) {
            contentType[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(contentType[i])));
        }
        // 前後空白除去
        size_t nStart = contentType.find_first_not_of(" \t");
        size_t nEnd = contentType.find_last_not_of(" \t");
        if (nStart != std::string::npos) {
            contentType = contentType.substr(nStart, nEnd - nStart + 1);
        }
        bIsPng = (contentType == "image/png");
    }
    if (!bIsPng) {
        response.statusLine = "HTTP/1.1 415 Unsupported Media Type";
        response.SetJsonBody("{\"error\":\"unsupported_media_type\"}");
        return;
    }

    if (request.body.empty()) {
        response.statusLine = "HTTP/1.1 400 Bad Request";
        response.SetJsonBody("{\"error\":\"empty_body\"}");
        return;
    }

    // HttpServer.cpp 側で 2MB を超えるボディはこの時点までに 413 で弾かれているが、
    // 念のためハンドラ側でも同じ上限を検証する。
    const size_t kMaxSpriteUploadBodySize = 2 * 1024 * 1024;
    if (request.body.size() > kMaxSpriteUploadBodySize) {
        response.statusLine = "HTTP/1.1 413 Payload Too Large";
        response.SetJsonBody("{\"error\":\"request_body_too_large\"}");
        return;
    }

    const unsigned char *pBodyData = reinterpret_cast<const unsigned char *>(request.body.data());
    size_t nBodySize = request.body.size();

    // PNG として妥当か・寸法は何かを、フルデコードして確認する。
    // inspect（ヘッダのみ）だと途中で切れた/壊れた PNG でもヘッダさえ正しければ
    // 通ってしまい、保存後に配信側の MakeTransparentPng が失敗してスプライトが
    // 404 で消える（アップロードは成功したのに絵が消える）最悪の失敗モードになるため、
    // ここでは必ずフルデコードする。デコード結果の画素は寸法確認以外に使わず破棄し、
    // 保存するのは受け取った生の PNG バイト列のまま（再エンコードしない）。
    // フルデコードして PNG として妥当か・寸法・カラータイプを確認する。
    // color_convert=0 でデコードすることで、MakeTransparentPng と同じ条件
    // （8bit パレット PNG であること）をここで検証できる。
    // 素材は全て 8bit パレット PNG（インデックス0=透過）が前提であり、
    // 見た目が同じ RGBA PNG 等を受理してしまうと、管理画面では正常に見えるのに
    // ゲームクライアント（Read256 が LCT_PALETTE 以外を拒否）だけが読めなくなる
    // という不具合を踏む。ここで入口から弾く。
    unsigned int imgW = 0, imgH = 0;
    {
        lodepng::State decodeState;
        decodeState.decoder.color_convert = 0;
        std::vector<unsigned char> decodedPixels;
        unsigned int decodeErr = lodepng::decode(decodedPixels, imgW, imgH, decodeState, pBodyData, nBodySize);
        if (decodeErr != 0) {
            response.statusLine = "HTTP/1.1 400 Bad Request";
            response.SetJsonBody("{\"error\":\"invalid_png\"}");
            return;
        }
        if (decodeState.info_png.color.colortype != LCT_PALETTE ||
            decodeState.info_png.color.palettesize == 0 ||
            decodeState.info_png.color.palettesize > 256)
        {
            response.statusLine = "HTTP/1.1 400 Bad Request";
            response.SetJsonBody(
                "{\"error\":\"not_palette_png\",\"message\":\""
                "8bitパレットPNG(インデックスカラー)で保存してください。"
                "インデックス0が透過色として扱われます。\"}");
            return;
        }
    }

    // 既存シートの差し替え or 新規シートの追加を判定する。
    // 現在配信中の画像が取得できればそれと同じ寸法を要求する（差し替え）。
    // 取得できなければ新規シートとみなし、同カテゴリの sheetIndex=0 を基準寸法に使う。
    bool bCreated = false;
    std::vector<unsigned char> currentPng;
    std::string currentEtag;
    bool bHasCurrent =
        CGrpResourceProvider::GetInstance().GetSheetPng(categoryKey, sheetIndex, currentPng, currentEtag);

    std::vector<unsigned char> referencePng;
    if (!bHasCurrent) {
        bCreated = true;
        std::string refEtag;
        if (!CGrpResourceProvider::GetInstance().GetSheetPng(categoryKey, 0, referencePng, refEtag)) {
            response.statusLine = "HTTP/1.1 400 Bad Request";
            response.SetJsonBody(
                "{\"error\":\"no_reference_sheet\",\"message\":\""
                "このカテゴリには基準となる既存シートがありません\"}");
            return;
        }
    }
    const std::vector<unsigned char> &basisPng = bHasCurrent ? currentPng : referencePng;

    unsigned int nExpectedWidth = 0, nExpectedHeight = 0;
    lodepng::State currentInspectState;
    unsigned int currentInspectErr = lodepng_inspect(
        &nExpectedWidth, &nExpectedHeight, &currentInspectState,
        basisPng.data(), basisPng.size());
    if (currentInspectErr != 0) {
        response.statusLine = "HTTP/1.1 500 Internal Server Error";
        response.SetJsonBody("{\"error\":\"current_sheet_invalid\"}");
        return;
    }

    if (imgW != nExpectedWidth || imgH != nExpectedHeight) {
        std::ostringstream oss;
        oss << "{\"error\":\"size_mismatch\","
            << "\"expected\":{\"width\":" << nExpectedWidth << ",\"height\":" << nExpectedHeight << "},"
            << "\"actual\":{\"width\":" << imgW << ",\"height\":" << imgH << "}}";
        response.statusLine = "HTTP/1.1 400 Bad Request";
        response.SetJsonBody(oss.str());
        return;
    }

    if (bCreated) {
        // 新規シートは連番が飛ばないことを要求する。
        // GetSheetCount の存在確認プローブは欠番に当たった時点で走査を打ち切るため、
        // 飛び番で追加すると以降のシートが二度と見えなくなってしまう。
        int nSheetCount = CGrpResourceProvider::GetInstance().GetSheetCount(categoryKey);
        if (sheetIndex < 0 || sheetIndex > nSheetCount) {
            std::ostringstream oss;
            oss << "{\"error\":\"index_out_of_range\","
                << "\"message\":\"シート番号は連番である必要があります。次に追加できる番号は "
                << nSheetCount << " です。\"}";
            response.statusLine = "HTTP/1.1 400 Bad Request";
            response.SetJsonBody(oss.str());
            return;
        }
    }

    std::string resName;
    if (!CGrpResourceProvider::GetInstance().ResolveResourceName(categoryKey, sheetIndex, resName)) {
        if (bCreated) {
            // 固定名テーブルのカテゴリ（char, char2x2 等）は配列長を超えると
            // リソース名を生成できない。番号パターンのカテゴリは常に生成できるため
            // ここに来るのは固定名カテゴリのみのはず。
            response.statusLine = "HTTP/1.1 400 Bad Request";
            response.SetJsonBody("{\"error\":\"cannot_add_sheet\"}");
        } else {
            response.statusLine = "HTTP/1.1 404 Not Found";
            response.SetJsonBody("{\"error\":\"sprite_not_found\"}");
        }
        return;
    }

    std::string putError;
    if (!CGrpImageStore::GetInstance().PutPng(
            resName.c_str(), pBodyData, nBodySize,
            static_cast<int>(imgW), static_cast<int>(imgH),
            authContext.loginId.c_str(), putError))
    {
        response.statusLine = "HTTP/1.1 500 Internal Server Error";
        response.SetJsonBody("{\"error\":\"" + JsonUtils::Escape(putError) + "\"}");
        return;
    }

    CGrpResourceProvider::GetInstance().InvalidateCache(resName.c_str());

    std::string etag;
    {
        std::vector<unsigned char> reread;
        // PutPng 直後に画像ストアから読み直し、実配信と同じ ETag を返す
        // （画像ストアの ETag 生成規則 "gs-<revision>-<size>" は GrpImageStore.cpp 内に閉じているため）
        CGrpImageStore::GetInstance().GetPng(resName.c_str(), reread, etag);
    }

    // ETag "\"gs-<revision>-<size>\"" から revision を取り出す
    // （PutPng は revision を outError 以外の形で返さないため、ここで再解析する）
    int nRevision = 0;
    {
        size_t nFirstDash = etag.find('-');
        if (nFirstDash != std::string::npos) {
            size_t nSecondDash = etag.find('-', nFirstDash + 1);
            if (nSecondDash != std::string::npos) {
                std::string revStr = etag.substr(nFirstDash + 1, nSecondDash - nFirstDash - 1);
                nRevision = std::atoi(revStr.c_str());
            }
        }
    }

    std::ostringstream oss;
    oss << "{\"resName\":\"" << JsonUtils::Escape(resName) << "\","
        << "\"revision\":" << nRevision << ","
        << "\"etag\":\"" << JsonUtils::Escape(etag) << "\","
        << "\"width\":" << imgW << ","
        << "\"height\":" << imgH << ","
        << "\"bytes\":" << nBodySize << ","
        << "\"created\":" << (bCreated ? "true" : "false") << "}";

    response.statusLine = "HTTP/1.1 200 OK";
    response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// CSpriteSheetHistoryHandler
// ---------------------------------------------------------------------------

CSpriteSheetHistoryHandler::CSpriteSheetHistoryHandler(std::string pathPrefix, CMgrData *pMgrData)
    : m_pathPrefix(std::move(pathPrefix))
    , m_pMgrData(pMgrData)
{
}

void CSpriteSheetHistoryHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
    AuthProvider::AuthContext authContext;
    if (!AuthorizeImageEdit(request, m_pMgrData, authContext, response)) {
        return;
    }

    std::string categoryKey;
    int sheetIndex = -1;
    ESpriteSheetPathSuffix suffix = SpriteSheetSuffixNone;
    if (!ParseSpriteSheetPathEx(m_pathPrefix, request.path, categoryKey, sheetIndex, suffix) ||
        suffix != SpriteSheetSuffixHistory)
    {
        response.statusLine = "HTTP/1.1 404 Not Found";
        response.SetJsonBody("{\"error\":\"sprite_not_found\"}");
        return;
    }

    std::string resName;
    if (!CGrpResourceProvider::GetInstance().ResolveResourceName(categoryKey, sheetIndex, resName)) {
        response.statusLine = "HTTP/1.1 404 Not Found";
        response.SetJsonBody("{\"error\":\"sprite_not_found\"}");
        return;
    }

    int nRevision = 0, nWidth = 0, nHeight = 0;
    long long llUpdatedAt = 0;
    std::string strUpdatedBy;
    size_t nBytes = 0;
    bool bHasCurrent = CGrpImageStore::GetInstance().GetCurrentMeta(
        resName.c_str(), nRevision, nWidth, nHeight, llUpdatedAt, strUpdatedBy, nBytes);

    std::vector<SGrpSheetHistoryEntry> history;
    CGrpImageStore::GetInstance().GetHistory(resName.c_str(), history);

    std::ostringstream oss;
    oss << "{\"resName\":\"" << JsonUtils::Escape(resName) << "\","
        << "\"overridden\":" << (bHasCurrent ? "true" : "false") << ",";
    if (bHasCurrent) {
        oss << "\"current\":{"
            << "\"revision\":" << nRevision << ","
            << "\"width\":" << nWidth << ","
            << "\"height\":" << nHeight << ","
            << "\"bytes\":" << nBytes << ","
            << "\"updatedAt\":" << llUpdatedAt << ","
            << "\"updatedBy\":\"" << JsonUtils::Escape(strUpdatedBy) << "\"},";
    } else {
        oss << "\"current\":null,";
    }
    oss << "\"history\":[";
    for (size_t i = 0; i < history.size(); ++i) {
        if (i > 0) {
            oss << ",";
        }
        const SGrpSheetHistoryEntry &entry = history[i];
        oss << "{\"revision\":" << entry.nRevision << ","
            << "\"savedAt\":" << entry.nSavedAt << ","
            << "\"savedBy\":\"" << JsonUtils::Escape(entry.strSavedBy) << "\","
            << "\"bytes\":" << entry.nBytes << "}";
    }
    oss << "]}";

    response.statusLine = "HTTP/1.1 200 OK";
    response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// CSpriteSheetRevertHandler
// ---------------------------------------------------------------------------

CSpriteSheetRevertHandler::CSpriteSheetRevertHandler(std::string pathPrefix, CMgrData *pMgrData)
    : m_pathPrefix(std::move(pathPrefix))
    , m_pMgrData(pMgrData)
{
}

void CSpriteSheetRevertHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
    AuthProvider::AuthContext authContext;
    if (!AuthorizeImageEdit(request, m_pMgrData, authContext, response)) {
        return;
    }

    std::string categoryKey;
    int sheetIndex = -1;
    ESpriteSheetPathSuffix suffix = SpriteSheetSuffixNone;
    if (!ParseSpriteSheetPathEx(m_pathPrefix, request.path, categoryKey, sheetIndex, suffix) ||
        suffix != SpriteSheetSuffixRevert)
    {
        response.statusLine = "HTTP/1.1 404 Not Found";
        response.SetJsonBody("{\"error\":\"sprite_not_found\"}");
        return;
    }

    int nRequestedRevision = -1;
    if (!JsonUtils::TryGetInt(request.body, "revision", nRequestedRevision) || nRequestedRevision <= 0) {
        response.statusLine = "HTTP/1.1 400 Bad Request";
        response.SetJsonBody("{\"error\":\"invalid_request\"}");
        return;
    }

    std::string resName;
    if (!CGrpResourceProvider::GetInstance().ResolveResourceName(categoryKey, sheetIndex, resName)) {
        response.statusLine = "HTTP/1.1 404 Not Found";
        response.SetJsonBody("{\"error\":\"sprite_not_found\"}");
        return;
    }

    std::vector<unsigned char> historyPng;
    if (!CGrpImageStore::GetInstance().GetHistoryPng(resName.c_str(), nRequestedRevision, historyPng)) {
        response.statusLine = "HTTP/1.1 404 Not Found";
        response.SetJsonBody("{\"error\":\"revision_not_found\"}");
        return;
    }

    // 履歴に入っている PNG は過去に PutPng でフルデコード検証済みのため、
    // ここでは寸法確認のためだけに inspect（ヘッダのみ）で十分。
    unsigned int imgW = 0, imgH = 0;
    lodepng::State inspectState;
    unsigned int inspectErr = lodepng_inspect(&imgW, &imgH, &inspectState, historyPng.data(), historyPng.size());
    if (inspectErr != 0) {
        response.statusLine = "HTTP/1.1 500 Internal Server Error";
        response.SetJsonBody("{\"error\":\"history_png_invalid\"}");
        return;
    }

    // 履歴を巻き戻すのではなく、指定 revision の内容を新しい版として追記保存する。
    // これで履歴が線形に保たれ、復元操作自体も後から取り消せる。
    std::string putError;
    if (!CGrpImageStore::GetInstance().PutPng(
            resName.c_str(), historyPng.data(), historyPng.size(),
            static_cast<int>(imgW), static_cast<int>(imgH),
            authContext.loginId.c_str(), putError))
    {
        response.statusLine = "HTTP/1.1 500 Internal Server Error";
        response.SetJsonBody("{\"error\":\"" + JsonUtils::Escape(putError) + "\"}");
        return;
    }

    CGrpResourceProvider::GetInstance().InvalidateCache(resName.c_str());

    std::string etag;
    {
        std::vector<unsigned char> reread;
        CGrpImageStore::GetInstance().GetPng(resName.c_str(), reread, etag);
    }

    int nNewRevision = 0;
    {
        size_t nFirstDash = etag.find('-');
        if (nFirstDash != std::string::npos) {
            size_t nSecondDash = etag.find('-', nFirstDash + 1);
            if (nSecondDash != std::string::npos) {
                std::string revStr = etag.substr(nFirstDash + 1, nSecondDash - nFirstDash - 1);
                nNewRevision = std::atoi(revStr.c_str());
            }
        }
    }

    std::ostringstream oss;
    oss << "{\"resName\":\"" << JsonUtils::Escape(resName) << "\","
        << "\"revision\":" << nNewRevision << ","
        << "\"etag\":\"" << JsonUtils::Escape(etag) << "\","
        << "\"width\":" << imgW << ","
        << "\"height\":" << imgH << ","
        << "\"bytes\":" << historyPng.size() << "}";

    response.statusLine = "HTTP/1.1 200 OK";
    response.SetJsonBody(oss.str());
}

// ---------------------------------------------------------------------------
// CSpriteSheetDeleteHandler
// ---------------------------------------------------------------------------

CSpriteSheetDeleteHandler::CSpriteSheetDeleteHandler(std::string pathPrefix, CMgrData *pMgrData)
    : m_pathPrefix(std::move(pathPrefix))
    , m_pMgrData(pMgrData)
{
}

void CSpriteSheetDeleteHandler::Handle(const HttpRequest &request, HttpResponse &response)
{
    AuthProvider::AuthContext authContext;
    if (!AuthorizeImageEdit(request, m_pMgrData, authContext, response)) {
        return;
    }

    // DELETE はサフィックス無しの基本パスのみを受け付ける。
    std::string categoryKey;
    int sheetIndex = -1;
    if (!ParseSpriteSheetPath(m_pathPrefix, request.path, categoryKey, sheetIndex)) {
        response.statusLine = "HTTP/1.1 404 Not Found";
        response.SetJsonBody("{\"error\":\"sprite_not_found\"}");
        return;
    }

    std::string resName;
    if (!CGrpResourceProvider::GetInstance().ResolveResourceName(categoryKey, sheetIndex, resName)) {
        response.statusLine = "HTTP/1.1 404 Not Found";
        response.SetJsonBody("{\"error\":\"sprite_not_found\"}");
        return;
    }

    std::string clearError;
    if (!CGrpImageStore::GetInstance().ClearOverride(resName.c_str(), clearError)) {
        response.statusLine = "HTTP/1.1 500 Internal Server Error";
        response.SetJsonBody("{\"error\":\"" + JsonUtils::Escape(clearError) + "\"}");
        return;
    }

    CGrpResourceProvider::GetInstance().InvalidateCache(resName.c_str());

    std::ostringstream oss;
    oss << "{\"resName\":\"" << JsonUtils::Escape(resName) << "\",\"overridden\":false}";

    response.statusLine = "HTTP/1.1 200 OK";
    response.SetJsonBody(oss.str());
}
