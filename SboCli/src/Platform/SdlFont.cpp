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
    int drawHeight;   // 実際に描かれる文字の高さ（ピクセル、2 倍後）
    int yOffset;      // 描画時の Y シフト量（負値: 上に詰める）
    bool bold;
};

// ゲーム画面は 240x240 ドットの絵を 2 倍で表示している。文字も 1 ドット = 2x2 ピクセルで描き、
// 描画位置も 2 ピクセル単位にそろえて、絵とドットの大きさを合わせる。
static const int SDLFONT_DOT = 2;

int SdlFontPixelScale()
{
    return SDLFONT_DOT;
}

// グローバル状態
static bool s_bInitialized = false;
static std::string s_fontDir;
static std::map<void*, SdlFontEntry> s_fontMap;
static std::map<void*, SdlDCContext> s_dcMap;
static uintptr_t s_nextFontId = 1;
static uintptr_t s_nextDCId = 1;

// フォントキャッシュ: (セル高, 太字) が同じなら同じハンドルを使い回す。
// CWindowBase はコンストラクタで 6 個フォントを作るため、キャッシュが無いと
// ウィンドウを開くたびにフォントファイルを開き直すことになる。
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

    // アンチエイリアスの中間色を消し、1 ドットを SDLFONT_DOT 倍に拡大する
    SDL_Surface* pConv = SDL_ConvertSurfaceFormat(pSurf, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(pSurf);
    if (!pConv) return nullptr;
    SDL_Surface* pScaled = SDL_CreateRGBSurfaceWithFormat(
        0, pConv->w * SDLFONT_DOT, pConv->h * SDLFONT_DOT, 32, SDL_PIXELFORMAT_ARGB8888);
    if (!pScaled) {
        SDL_FreeSurface(pConv);
        return nullptr;
    }
    SDL_LockSurface(pConv);
    SDL_LockSurface(pScaled);
    for (int y = 0; y < pScaled->h; y++) {
        Uint32* pDst = (Uint32*)((Uint8*)pScaled->pixels + y * pScaled->pitch);
        const Uint32* pSrc = (const Uint32*)((const Uint8*)pConv->pixels + (y / SDLFONT_DOT) * pConv->pitch);
        for (int x = 0; x < pScaled->w; x++) {
            Uint32 c = pSrc[x / SDLFONT_DOT];
            pDst[x] = ((c >> 24) >= 128) ? (c | 0xFF000000u) : 0;
        }
    }
    SDL_UnlockSurface(pScaled);
    SDL_UnlockSurface(pConv);
    SDL_FreeSurface(pConv);
    pSurf = pScaled;

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

    // 要求サイズからピクセルフォントを選ぶ（いずれも 2 倍で描く）
    //   14px 以下 → 小: 美咲ゴシック第2（8 ドット → 16px）
    //   15〜23px  → 中: PixelMplus10（10 ドット → 20px）
    //   24px 以上 → 大: PixelMplus12（12 ドット → 24px）
    // 太字は 2 倍にすると潰れて読みにくいので、太字指定でも通常の太さで描く。
    const char* pszFile;
    int nPt;
    if (height <= 14) {
        pszFile = "misaki_gothic_2nd.ttf";
        nPt = 8;
    } else if (height < 24) {
        pszFile = "PixelMplus10-Regular.ttf";
        nPt = 10;
    } else {
        pszFile = "PixelMplus12-Regular.ttf";
        nPt = 12;
    }
    std::string path = s_fontDir;
    if (!path.empty() && path.back() != '/' && path.back() != '\\') {
        path += "/";
    }
    path += pszFile;

    TTF_Font* pFont = TTF_OpenFont(path.c_str(), nPt);
    if (!pFont) {
        SDL_Log("SdlFontCreate: TTF_OpenFont failed: %s (path=%s, height=%d)",
                TTF_GetError(), path.c_str(), height);
        return nullptr;
    }
    // ピクセルフォントのヒンティングで形が崩れないようにする
    TTF_SetFontHinting(pFont, TTF_HINTING_MONO);

    // 文字の上端を描画位置にそろえる。要求より小さくなる大サイズだけ上下中央に寄せる。
    int drawHeight = nPt * SDLFONT_DOT;
    int yOffset = 0;
    if (drawHeight < height) {
        yOffset = ((height - drawHeight) / 2) & ~(SDLFONT_DOT - 1);
    }

    // エントリ登録
    void* id = (void*)(s_nextFontId++);
    SdlFontEntry entry;
    entry.pFont = pFont;
    entry.height = height;
    entry.drawHeight = drawHeight;
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
    // 2 ピクセル = 1 ドットの格子にそろえる
    int nDrawX = x & ~(SDLFONT_DOT - 1);
    int nDrawY = (y + entry->yOffset) & ~(SDLFONT_DOT - 1);
    BlitSurfaceToBuffer(pSurf, ctx->pBits, ctx->stride,
                         ctx->width, ctx->height, nDrawX, nDrawY);

    // pSurf はキャッシュが所有するため解放しない
    return true;
}

// 縁取り付きテキスト描画
bool SdlFontTextOutFramed(void* hDC, int x, int y, const wchar_t* pStr, int nLen,
                          unsigned long color, unsigned long colorFrame, bool bThick)
{
    SdlDCContext* ctx = SdlDCGet(hDC);
    if (!ctx) return false;

    // 美咲（8 ドット）は字画が詰まっているので、斜めまで縁取ると字がつぶれて読めなくなる。
    // 太めの指定でも小さい文字は上下左右だけにする
    if (bThick && ctx->currentFont) {
        const SdlFontEntry* entry = GetFontEntry(ctx->currentFont);
        if (entry && (entry->drawHeight <= 8 * SDLFONT_DOT)) {
            bThick = false;
        }
    }

    // 先に格子へそろえてから 1 ドットずつずらす（ずらし方が左右で偏らないように）
    x &= ~(SDLFONT_DOT - 1);
    y &= ~(SDLFONT_DOT - 1);
    ctx->textColor = colorFrame;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if ((dx == 0) && (dy == 0)) continue;
            if (!bThick && (dx != 0) && (dy != 0)) continue;
            SdlFontTextOut(hDC, x + dx * SDLFONT_DOT, y + dy * SDLFONT_DOT, pStr, nLen);
        }
    }
    ctx->textColor = color;
    return SdlFontTextOut(hDC, x, y, pStr, nLen);
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
    // 2 ピクセル = 1 ドットの格子にそろえる
    int nDrawX = x & ~(SDLFONT_DOT - 1);
    int nDrawY = (y + entry->yOffset) & ~(SDLFONT_DOT - 1);
    BlitSurfaceToBuffer(pSurf, ctx->pBits, ctx->stride,
                         ctx->width, ctx->height, nDrawX, nDrawY);

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
    if (pWidth) *pWidth = w * SDLFONT_DOT;
    // 実際に描かれる文字の高さを返す
    if (pHeight) *pHeight = entry->drawHeight;
    return true;
}

#endif // !_WINDLL
