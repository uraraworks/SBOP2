/// @file SdlFont.cpp
/// @brief SDL_ttfベースのフォント管理・テキスト描画 実装
/// @date 2026/04/06

#if !defined(_WINDLL)

#include "SdlFont.h"
#if !defined(_WIN32)
#include "TCharCompat.h"
#endif
#include "SjisConvert.h"
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <codecvt>
#include <locale>

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
    // 全フォントを閉じる
    for (auto& pair : s_fontMap) {
        if (pair.second.pFont) {
            TTF_CloseFont(pair.second.pFont);
        }
    }
    s_fontMap.clear();
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

    return id;
}

// フォント破棄
void SdlFontDestroy(void* hFont)
{
    auto it = s_fontMap.find(hFont);
    if (it != s_fontMap.end()) {
        if (it->second.pFont) {
            TTF_CloseFont(it->second.pFont);
        }
        s_fontMap.erase(it);
    }
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

    // COLORREFからSDL_Colorに変換（COLORREF: 0x00BBGGRR）
    SDL_Color color;
    color.r = (unsigned char)(ctx->textColor & 0xFF);
    color.g = (unsigned char)((ctx->textColor >> 8) & 0xFF);
    color.b = (unsigned char)((ctx->textColor >> 16) & 0xFF);
    color.a = 255;

    // テキストをサーフェスにレンダリング
    SDL_Surface* pSurf = TTF_RenderUTF8_Blended(pFont, utf8.c_str(), color);
    if (!pSurf) return false;

    // CImg32バッファに転送（yOffset でサーフェス底をセル底に合わせる）
    BlitSurfaceToBuffer(pSurf, ctx->pBits, ctx->stride,
                         ctx->width, ctx->height, x, y + entry->yOffset);

    SDL_FreeSurface(pSurf);
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

    SDL_Color color;
    color.r = (unsigned char)(ctx->textColor & 0xFF);
    color.g = (unsigned char)((ctx->textColor >> 8) & 0xFF);
    color.b = (unsigned char)((ctx->textColor >> 16) & 0xFF);
    color.a = 255;

    SDL_Surface* pSurf = TTF_RenderUTF8_Blended(pFont, text.c_str(), color);
    if (!pSurf) return false;

    // CImg32バッファに転送（yOffset でサーフェス底をセル底に合わせる）
    BlitSurfaceToBuffer(pSurf, ctx->pBits, ctx->stride,
                         ctx->width, ctx->height, x, y + entry->yOffset);

    SDL_FreeSurface(pSurf);
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
