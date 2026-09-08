/// @file SdlFont.cpp
/// @brief SDL_ttfベースのフォント管理・テキスト描画 実装
/// @date 2026/04/06

#if !defined(_WINDLL)

#include "SdlFont.h"
#if !defined(_WIN32)
#include "../../../Common/Platform/TCharCompat.h"
#endif
#include "../../../Common/Platform/SjisConvert.h"
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <codecvt>
#include <locale>
#include <map>
#include <string>
#include <utility>

// フォントエントリ
struct SdlFontEntry {
    TTF_Font* pFont;
    int height;       // 呼び出し側が要求したセル高（ピクセル）
    int yOffset;      // 描画時の Y シフト量（負値: 上に詰める）
    bool bold;
};

// グローバル状態
static bool s_bInitialized = false;
static std::string s_fontDir;
static std::map<void*, SdlFontEntry> s_fontMap;
static std::map<void*, SdlDCContext> s_dcMap;
static uintptr_t s_nextFontId = 1;
static uintptr_t s_nextDCId = 1;

// フォントキャッシュ: (セル高, 太字) が同じなら同じハンドルを使い回す。
// CWindowBase はコンストラクタで 6 個フォントを作るため、キャッシュが無いと
// ウィンドウを開くたびに 16〜17MB の CJK フォントを開き直すことになる。
static std::map<std::pair<int, bool>, void*> s_fontKeyMap;

// テキスト描画結果キャッシュ
// 同一の (フォント, 色, 文字列) を毎フレーム再ラスタライズしないための表。
// 縁取り描画(CWindowBase::TextOut2)は同じ文字列を 5 回描くため、これだけでも大きい。
struct SdlTextCacheKey {
    void*         hFont;
    unsigned long color;
    std::string   text;

    bool operator<(const SdlTextCacheKey& other) const
    {
        if (hFont != other.hFont) return hFont < other.hFont;
        if (color != other.color) return color < other.color;
        return text < other.text;
    }
};
static std::map<SdlTextCacheKey, SDL_Surface*> s_textCache;
// 上限。超えたら丸ごと捨てる（LRU 管理のコストを避けるための単純な方式）
static const size_t SDL_TEXT_CACHE_MAX = 256;

static void SdlTextCacheClear()
{
    for (auto& pair : s_textCache) {
        if (pair.second) {
            SDL_FreeSurface(pair.second);
        }
    }
    s_textCache.clear();
}

// キャッシュ済みのレンダリング結果を返す。返した SDL_Surface は
// キャッシュが所有するので、呼び出し側で SDL_FreeSurface してはいけない。
static SDL_Surface* SdlTextCacheGet(void* hFont, TTF_Font* pFont,
                                    unsigned long color, const std::string& text)
{
    SdlTextCacheKey key;
    key.hFont = hFont;
    key.color = color;
    key.text  = text;

    auto it = s_textCache.find(key);
    if (it != s_textCache.end()) {
        return it->second;
    }

    SDL_Color sdlColor;
    sdlColor.r = (unsigned char)(color & 0xFF);
    sdlColor.g = (unsigned char)((color >> 8) & 0xFF);
    sdlColor.b = (unsigned char)((color >> 16) & 0xFF);
    sdlColor.a = 255;

    SDL_Surface* pSurf = TTF_RenderUTF8_Blended(pFont, text.c_str(), sdlColor);
    if (!pSurf) {
        return nullptr;
    }

    // 挿入前に上限判定する（この時点で pSurf はまだ表に入っていない）
    if (s_textCache.size() >= SDL_TEXT_CACHE_MAX) {
        SdlTextCacheClear();
    }
    s_textCache[key] = pSurf;
    return pSurf;
}

// ワイド文字列→UTF-8変換
// Emscripten では wchar_t が 32bit (UTF-32) のため codecvt_utf8_utf16 は使えない。
// Win32 版は 16bit (UTF-16) だが WstringToUtf8 はサロゲートペアも扱えるので両対応。
static std::string WideToUtf8(const wchar_t* pStr, int nLen)
{
    if (!pStr || nLen <= 0) return std::string();
#if defined(_WIN32)
    std::wstring ws(pStr, nLen);
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(ws);
#else
    return WstringToUtf8(pStr, static_cast<size_t>(nLen));
#endif
}

// フォントシステム初期化
bool SdlFontInit(const char* fontDir)
{
    if (s_bInitialized) return true;
    if (TTF_Init() < 0) {
        SDL_Log("SdlFontInit: TTF_Init failed: %s", TTF_GetError());
        return false;
    }
    s_fontDir = fontDir ? fontDir : "";
    s_bInitialized = true;
    SDL_Log("SdlFontInit: initialized, fontDir=%s", s_fontDir.c_str());
    return true;
}

// フォントシステム終了
void SdlFontShutdown()
{
    SdlTextCacheClear();
    // 全フォントを閉じる
    for (auto& pair : s_fontMap) {
        if (pair.second.pFont) {
            TTF_CloseFont(pair.second.pFont);
        }
    }
    s_fontMap.clear();
    s_fontKeyMap.clear();
    s_dcMap.clear();
    if (s_bInitialized) {
        TTF_Quit();
        s_bInitialized = false;
    }
}

// フォント作成
void* SdlFontCreate(int height, bool bold)
{
    if (!s_bInitialized) return nullptr;

    // 同じ (セル高, 太字) はハンドルを共有する
    std::pair<int, bool> cacheKey(height, bold);
    auto cached = s_fontKeyMap.find(cacheKey);
    if (cached != s_fontKeyMap.end()) {
        return cached->second;
    }

    // フォントファイルパスを決定
    std::string path = s_fontDir;
    if (!path.empty() && path.back() != '/' && path.back() != '\\') {
        path += "/";
    }
    path += bold ? "NotoSansCJKjp-Bold.otf" : "NotoSansCJKjp-Regular.otf";

    // フォントを開く（heightはピクセルサイズ）
    TTF_Font* pFont = TTF_OpenFont(path.c_str(), height);
    if (!pFont) {
        SDL_Log("SdlFontCreate: TTF_OpenFont failed: %s (path=%s, height=%d)",
                TTF_GetError(), path.c_str(), height);
        return nullptr;
    }

    // Noto Sans CJK は TTF_FontHeight が ptsize より大きくなる場合がある。
    // ptsize は要求値のまま使い、はみ出し分は描画時に Y オフセットで吸収する。
    int actualHeight = TTF_FontHeight(pFont);
    // 上下中央寄せ: はみ出し分の半分を上、半分を下に逃がす
    // 例: height=16, actual=22 → yOffset=-3（上3px・下3pxにオーバーフロー）
    int yOffset = (actualHeight > height) ? ((height - actualHeight) / 2) : 0;

    // エントリ登録
    void* id = (void*)(s_nextFontId++);
    SdlFontEntry entry;
    entry.pFont = pFont;
    entry.height = height;
    entry.yOffset = yOffset;
    entry.bold = bold;
    s_fontMap[id] = entry;
    s_fontKeyMap[cacheKey] = id;

    return id;
}

// フォント破棄
void SdlFontDestroy(void* hFont)
{
    // フォントは (セル高, 太字) 単位でキャッシュして共有するため、
    // 個々の所有者が閉じてはいけない。実際の解放は SdlFontShutdown で行う。
    // 使用されるセル高は 12/14/16/32 程度に限られるので、溜め込んでも問題ない。
    (void)hFont;
}

// DCコンテキスト登録
void* SdlDCRegister(unsigned char* pBits, int width, int height)
{
    void* id = (void*)(s_nextDCId++);
    SdlDCContext ctx;
    ctx.pBits = pBits;
    ctx.width = width;
    ctx.height = height;
    ctx.stride = width * 4;
    ctx.currentFont = nullptr;
    ctx.textColor = 0x00FFFFFF; // 白
    s_dcMap[id] = ctx;
    return id;
}

// DCコンテキスト解除
void SdlDCUnregister(void* hDC)
{
    s_dcMap.erase(hDC);
}

// DCコンテキスト取得
SdlDCContext* SdlDCGet(void* hDC)
{
    auto it = s_dcMap.find(hDC);
    if (it != s_dcMap.end()) {
        return &it->second;
    }
    return nullptr;
}

// TTF_Fontを取得するヘルパー
static TTF_Font* GetTTFFont(void* hFont)
{
    auto it = s_fontMap.find(hFont);
    if (it != s_fontMap.end()) {
        return it->second.pFont;
    }
    return nullptr;
}

// フォントエントリ全体を取得するヘルパー
static const SdlFontEntry* GetFontEntry(void* hFont)
{
    auto it = s_fontMap.find(hFont);
    return (it != s_fontMap.end()) ? &it->second : nullptr;
}

// SDL_SurfaceのピクセルをCImg32バッファにブレンド描画
// CImg32: BGRA, ボトムアップ（行0=画像の最下行）
// SDL_Surface (TTF_RenderUTF8_Blended): ARGB, トップダウン
static void BlitSurfaceToBuffer(
    SDL_Surface* pSurf, unsigned char* pBits, int stride,
    int bufWidth, int bufHeight, int dx, int dy)
{
    if (!pSurf || !pBits) return;

    SDL_LockSurface(pSurf);

    int srcW = pSurf->w;
    int srcH = pSurf->h;
    int srcPitch = pSurf->pitch;
    unsigned char* pSrcPixels = (unsigned char*)pSurf->pixels;

    for (int sy = 0; sy < srcH; sy++) {
        int destY = dy + sy;
        if (destY < 0 || destY >= bufHeight) continue;

        // CImg32はボトムアップ: 論理y=0はバッファの最終行
        int bufRow = (bufHeight - 1) - destY;

        unsigned char* pDstRow = pBits + bufRow * stride;
        unsigned char* pSrcRow = pSrcPixels + sy * srcPitch;

        for (int sx = 0; sx < srcW; sx++) {
            int destX = dx + sx;
            if (destX < 0 || destX >= bufWidth) continue;

            // SDL_Surface（TTF_RenderUTF8_Blended）のピクセル形式
            // SDL_PIXELFORMAT_ARGB8888: [A][R][G][B] in memory (big-endian order)
            // ただしリトルエンディアンでは実際は [B][G][R][A] のバイト順
            unsigned char* pSrcPx = pSrcRow + sx * 4;
            unsigned char srcB = pSrcPx[0];
            unsigned char srcG = pSrcPx[1];
            unsigned char srcR = pSrcPx[2];
            unsigned char srcA = pSrcPx[3];

            if (srcA == 0) continue;

            // CImg32: BGRA (BI_RGB 32bit, LE) → [B][G][R][0x00]
            unsigned char* pDstPx = pDstRow + destX * 4;

            if (srcA == 255) {
                pDstPx[0] = srcB;
                pDstPx[1] = srcG;
                pDstPx[2] = srcR;
                // pDstPx[3] はCImg32では未使用（0x00固定）
            } else {
                // アルファブレンド
                unsigned int a = srcA;
                unsigned int ia = 255 - a;
                pDstPx[0] = (unsigned char)((srcB * a + pDstPx[0] * ia) / 255);
                pDstPx[1] = (unsigned char)((srcG * a + pDstPx[1] * ia) / 255);
                pDstPx[2] = (unsigned char)((srcR * a + pDstPx[2] * ia) / 255);
            }
        }
    }

    SDL_UnlockSurface(pSurf);
}

// テキスト描画（ワイド文字版）
bool SdlFontTextOut(void* hDC, int x, int y, const wchar_t* pStr, int nLen)
{
    SdlDCContext* ctx = SdlDCGet(hDC);
    if (!ctx || !ctx->currentFont) return false;

    const SdlFontEntry* entry = GetFontEntry(ctx->currentFont);
    if (!entry || !entry->pFont) return false;
    TTF_Font* pFont = entry->pFont;

    // ワイド文字をUTF-8に変換
    std::string utf8 = WideToUtf8(pStr, nLen);
    if (utf8.empty()) return false;

    // レンダリング結果はキャッシュから取得する（所有権はキャッシュ側）
    SDL_Surface* pSurf = SdlTextCacheGet(ctx->currentFont, pFont, ctx->textColor, utf8);
    if (!pSurf) return false;

    // CImg32バッファに転送（yOffset でサーフェス底をセル底に合わせる）
    BlitSurfaceToBuffer(pSurf, ctx->pBits, ctx->stride,
                         ctx->width, ctx->height, x, y + entry->yOffset);

    // pSurf はキャッシュが所有するため解放しない
    return true;
}

// テキスト描画（ANSI文字版）
bool SdlFontTextOutA(void* hDC, int x, int y, const char* pStr, int nLen)
{
    SdlDCContext* ctx = SdlDCGet(hDC);
    if (!ctx || !ctx->currentFont) return false;

    const SdlFontEntry* entry = GetFontEntry(ctx->currentFont);
    if (!entry || !entry->pFont) return false;
    TTF_Font* pFont = entry->pFont;

    // pStr は UTF-8 バイト列 (移行完了後)
    if (pStr == nullptr) return false;
    size_t srcLen = (nLen < 0) ? strlen(pStr) : static_cast<size_t>(nLen);
    if (srcLen == 0) return false;
    std::string text(pStr, srcLen);

    // レンダリング結果はキャッシュから取得する（所有権はキャッシュ側）
    SDL_Surface* pSurf = SdlTextCacheGet(ctx->currentFont, pFont, ctx->textColor, text);
    if (!pSurf) return false;

    // CImg32バッファに転送（yOffset でサーフェス底をセル底に合わせる）
    BlitSurfaceToBuffer(pSurf, ctx->pBits, ctx->stride,
                         ctx->width, ctx->height, x, y + entry->yOffset);

    // pSurf はキャッシュが所有するため解放しない
    return true;
}

// テキストサイズ取得
bool SdlFontGetTextExtent(void* hFont, const wchar_t* pStr, int nLen, int* pWidth, int* pHeight)
{
    const SdlFontEntry* entry = GetFontEntry(hFont);
    if (!entry || !entry->pFont || !pStr || nLen <= 0) return false;

    std::string utf8 = WideToUtf8(pStr, nLen);
    int w = 0, h = 0;
    if (TTF_SizeUTF8(entry->pFont, utf8.c_str(), &w, &h) != 0) {
        return false;
    }
    if (pWidth) *pWidth = w;
    // 呼び出し側のレイアウトとの整合のため、要求セル高を返す
    if (pHeight) *pHeight = entry->height;
    return true;
}

#endif // !_WINDLL
