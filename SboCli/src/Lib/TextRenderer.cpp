/// @file TextRenderer.cpp
/// @brief SDL_ttf テキスト描画クラス 実装ファイル
/// @date 2026/04/05

#include "stdafx.h"
#include <string>
#include <vector>
#include <SDL_ttf.h>
#include "Img32.h"
#include "TextRenderer.h"


CTextRenderer &CTextRenderer::Instance()
{
    static CTextRenderer s_Instance;
    return s_Instance;
}


CTextRenderer::CTextRenderer()
{
    m_bInitialized = FALSE;
    for (int i = 0; i < FONTID_COUNT; i++) {
        m_apFont[i] = NULL;
    }
}


CTextRenderer::~CTextRenderer()
{
    Destroy();
}


// フォントをロードする
// pszFontName: フォントパス
// nFaceIndex: TTC内のフェースインデックス
// nSize: ピクセルサイズ
// nStyle: TTF_STYLE_NORMAL or TTF_STYLE_BOLD
TTF_Font *CTextRenderer::LoadFont(const char *pszFontName, int nFaceIndex, int nSize, int nStyle)
{
    TTF_Font *pFont = TTF_OpenFontIndex(pszFontName, nSize, nFaceIndex);
    if (pFont == NULL) {
        return NULL;
    }
    TTF_SetFontStyle(pFont, nStyle);
    TTF_SetFontHinting(pFont, TTF_HINTING_NORMAL);
    return pFont;
}


BOOL CTextRenderer::Init()
{
    if (m_bInitialized) {
        return TRUE;
    }

    if (TTF_Init() < 0) {
        return FALSE;
    }

    // Windows: msgothic.ttc を使用
    // Index 0 = MS ゴシック（等幅）
    // Index 1 = MS Pゴシック（プロポーショナル）
    const char *pszFontPath = "C:\\Windows\\Fonts\\msgothic.ttc";

    // フォールバック用パス（プロジェクト内）
    // TODO: OSSフォント配置時にフォールバック実装

    // WindowBase 系フォント
    m_apFont[FONTID_PGOTHIC_16_BOLD]   = LoadFont(pszFontPath, 1, 16, TTF_STYLE_BOLD);
    m_apFont[FONTID_PGOTHIC_12_NORMAL] = LoadFont(pszFontPath, 1, 12, TTF_STYLE_NORMAL);
    m_apFont[FONTID_PGOTHIC_12_BOLD]   = LoadFont(pszFontPath, 1, 12, TTF_STYLE_BOLD);
    m_apFont[FONTID_PGOTHIC_14_BOLD]   = LoadFont(pszFontPath, 1, 14, TTF_STYLE_BOLD);
    m_apFont[FONTID_GOTHIC_16_BOLD]    = LoadFont(pszFontPath, 0, 16, TTF_STYLE_BOLD);
    m_apFont[FONTID_GOTHIC_16_NORMAL]  = LoadFont(pszFontPath, 0, 16, TTF_STYLE_NORMAL);

    // LayerBase / InfoCharCli 系
    m_apFont[FONTID_GOTHIC_12_NORMAL]  = LoadFont(pszFontPath, 0, 12, TTF_STYLE_NORMAL);

    // LayerMap 系
    m_apFont[FONTID_GOTHIC_32_NORMAL]  = LoadFont(pszFontPath, 0, 32, TTF_STYLE_NORMAL);

    // ロード失敗チェック
    for (int i = 0; i < FONTID_COUNT; i++) {
        if (m_apFont[i] == NULL) {
            // フォントのロードに失敗 — 全て閉じて失敗を返す
            Destroy();
            return FALSE;
        }
    }

    m_bInitialized = TRUE;
    return TRUE;
}


void CTextRenderer::Destroy()
{
    for (int i = 0; i < FONTID_COUNT; i++) {
        if (m_apFont[i]) {
            TTF_CloseFont(m_apFont[i]);
            m_apFont[i] = NULL;
        }
    }

    if (m_bInitialized) {
        TTF_Quit();
        m_bInitialized = FALSE;
    }
}


// LPCTSTR → UTF-8 変換
std::string CTextRenderer::ToUtf8(LPCTSTR pStr)
{
#ifdef UNICODE
    if (pStr == NULL) {
        return std::string();
    }
    int nLen = (int)wcslen(pStr);
    if (nLen <= 0) {
        return std::string();
    }
    int nUtf8Len = WideCharToMultiByte(CP_UTF8, 0, pStr, nLen, NULL, 0, NULL, NULL);
    if (nUtf8Len <= 0) {
        return std::string();
    }
    std::string strUtf8(nUtf8Len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, pStr, nLen, &strUtf8[0], nUtf8Len, NULL, NULL);
    return strUtf8;
#else
    // ANSI → UTF-8: まず ANSI → Wide → UTF-8
    if (pStr == NULL) {
        return std::string();
    }
    int nLen = (int)strlen(pStr);
    if (nLen <= 0) {
        return std::string();
    }
    int nWideLen = MultiByteToWideChar(CP_ACP, 0, pStr, nLen, NULL, 0);
    if (nWideLen <= 0) {
        return std::string();
    }
    std::vector<wchar_t> aWide(nWideLen);
    MultiByteToWideChar(CP_ACP, 0, pStr, nLen, &aWide[0], nWideLen);
    int nUtf8Len = WideCharToMultiByte(CP_UTF8, 0, &aWide[0], nWideLen, NULL, 0, NULL, NULL);
    if (nUtf8Len <= 0) {
        return std::string();
    }
    std::string strUtf8(nUtf8Len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, &aWide[0], nWideLen, &strUtf8[0], nUtf8Len, NULL, NULL);
    return strUtf8;
#endif
}


// SDL_Surface のピクセルを CImg32 のピクセルバッファにアルファブレンドコピー
// CImg32: ボトムアップDIB、ピクセル順 [B][G][R][A]（リトルエンディアンで ARGB8888）
// SDL_Surface (TTF_RenderUTF8_Blended): ARGB8888 フォーマット
void CTextRenderer::BlitSurfaceToImg32(CImg32 *pDst, SDL_Surface *pSurf, int dstX, int dstY)
{
    if (pDst == NULL || pSurf == NULL) {
        return;
    }

    LPBYTE pDstBits = pDst->GetBits();
    if (pDstBits == NULL) {
        return;
    }

    int nDstW = pDst->Width();
    int nDstH = pDst->Height();
    int nSrcW = pSurf->w;
    int nSrcH = pSurf->h;

    if (nDstW <= 0 || nDstH <= 0 || nSrcW <= 0 || nSrcH <= 0) {
        return;
    }

    // クリッピング
    int sx0 = 0, sy0 = 0;
    int dx0 = dstX, dy0 = dstY;
    int copyW = nSrcW, copyH = nSrcH;

    if (dx0 < 0) { sx0 = -dx0; copyW += dx0; dx0 = 0; }
    if (dy0 < 0) { sy0 = -dy0; copyH += dy0; dy0 = 0; }
    if (dx0 + copyW > nDstW) { copyW = nDstW - dx0; }
    if (dy0 + copyH > nDstH) { copyH = nDstH - dy0; }

    if (copyW <= 0 || copyH <= 0) {
        return;
    }

    // SDL_Surface のピクセルフォーマット情報
    SDL_PixelFormat *pFmt = pSurf->format;
    int nSrcBpp = pFmt->BytesPerPixel;

    // CImg32 のストライド（1行のバイト数）
    int nDstStride = nDstW * 4;

    SDL_LockSurface(pSurf);

    for (int y = 0; y < copyH; y++) {
        // CImg32 はボトムアップDIB: Y=0 がバッファの先頭で画像の「下端」
        // 画面座標 (dy0 + y) に描画 → バッファ行 = (nDstH - 1 - (dy0 + y))
        int nDstRow = nDstH - 1 - (dy0 + y);
        LPBYTE pDstLine = pDstBits + nDstRow * nDstStride + dx0 * 4;

        LPBYTE pSrcLine = (LPBYTE)pSurf->pixels + (sy0 + y) * pSurf->pitch + sx0 * nSrcBpp;

        for (int x = 0; x < copyW; x++) {
            // ソースピクセルの取得（ARGB8888 前提）
            DWORD dwSrcPixel;
            if (nSrcBpp == 4) {
                dwSrcPixel = *(DWORD *)(pSrcLine + x * 4);
            } else {
                continue;
            }

            // ARGB8888 からコンポーネント取得
            BYTE srcA = (BYTE)((dwSrcPixel >> pFmt->Ashift) & 0xFF);
            BYTE srcR = (BYTE)((dwSrcPixel >> pFmt->Rshift) & 0xFF);
            BYTE srcG = (BYTE)((dwSrcPixel >> pFmt->Gshift) & 0xFF);
            BYTE srcB = (BYTE)((dwSrcPixel >> pFmt->Bshift) & 0xFF);

            if (srcA == 0) {
                continue;
            }

            // デスティネーションピクセル（BGRA順）
            LPBYTE pDstPx = pDstLine + x * 4;
            BYTE dstB = pDstPx[0];
            BYTE dstG = pDstPx[1];
            BYTE dstR = pDstPx[2];
            // pDstPx[3] は CImg32 では常に 0x00

            if (srcA == 255) {
                // 不透明: 直接書き込み
                pDstPx[0] = srcB;
                pDstPx[1] = srcG;
                pDstPx[2] = srcR;
            } else {
                // アルファブレンド
                BYTE invA = (BYTE)(255 - srcA);
                pDstPx[0] = (BYTE)((srcB * srcA + dstB * invA) / 255);
                pDstPx[1] = (BYTE)((srcG * srcA + dstG * invA) / 255);
                pDstPx[2] = (BYTE)((srcR * srcA + dstR * invA) / 255);
            }
        }
    }

    SDL_UnlockSurface(pSurf);
}


void CTextRenderer::DrawText(CImg32 *pDst, FontId fontId, int x, int y, LPCTSTR pStr, COLORREF color)
{
    if (pDst == NULL || pStr == NULL || !m_bInitialized) {
        return;
    }
    if (fontId < 0 || fontId >= FONTID_COUNT || m_apFont[fontId] == NULL) {
        return;
    }

    std::string strUtf8 = ToUtf8(pStr);
    if (strUtf8.empty()) {
        return;
    }

    // COLORREF: 0x00BBGGRR
    SDL_Color sdlColor;
    sdlColor.r = (Uint8)(color & 0xFF);
    sdlColor.g = (Uint8)((color >> 8) & 0xFF);
    sdlColor.b = (Uint8)((color >> 16) & 0xFF);
    sdlColor.a = 255;

    SDL_Surface *pSurf = TTF_RenderUTF8_Blended(m_apFont[fontId], strUtf8.c_str(), sdlColor);
    if (pSurf == NULL) {
        return;
    }

    BlitSurfaceToImg32(pDst, pSurf, x, y);
    SDL_FreeSurface(pSurf);
}


void CTextRenderer::DrawTextOutlined(CImg32 *pDst, FontId fontId, int x, int y, LPCTSTR pStr,
                                     COLORREF color, COLORREF colorFrame)
{
    // 4方向縁取り（WindowBase::TextOut2 と同等）
    DrawText(pDst, fontId, x - 1, y, pStr, colorFrame);
    DrawText(pDst, fontId, x + 1, y, pStr, colorFrame);
    DrawText(pDst, fontId, x, y - 1, pStr, colorFrame);
    DrawText(pDst, fontId, x, y + 1, pStr, colorFrame);
    DrawText(pDst, fontId, x, y, pStr, color);
}


void CTextRenderer::DrawTextOutlinedThick(CImg32 *pDst, FontId fontId, int x, int y, LPCTSTR pStr,
                                          COLORREF colorFrame, COLORREF color)
{
    // 20方向縁取り（WindowBase::TextOut4 / LayerBase::TextOut3 と同等）
    DrawText(pDst, fontId, x - 2, y, pStr, colorFrame);
    DrawText(pDst, fontId, x - 1, y, pStr, colorFrame);
    DrawText(pDst, fontId, x - 1, y - 2, pStr, colorFrame);
    DrawText(pDst, fontId, x - 2, y - 1, pStr, colorFrame);
    DrawText(pDst, fontId, x - 1, y - 1, pStr, colorFrame);
    DrawText(pDst, fontId, x - 2, y + 1, pStr, colorFrame);
    DrawText(pDst, fontId, x - 1, y + 1, pStr, colorFrame);
    DrawText(pDst, fontId, x - 1, y + 2, pStr, colorFrame);

    DrawText(pDst, fontId, x + 2, y, pStr, colorFrame);
    DrawText(pDst, fontId, x + 1, y, pStr, colorFrame);
    DrawText(pDst, fontId, x + 1, y - 2, pStr, colorFrame);
    DrawText(pDst, fontId, x + 2, y - 1, pStr, colorFrame);
    DrawText(pDst, fontId, x + 1, y - 1, pStr, colorFrame);
    DrawText(pDst, fontId, x + 2, y + 1, pStr, colorFrame);
    DrawText(pDst, fontId, x + 1, y + 1, pStr, colorFrame);
    DrawText(pDst, fontId, x + 1, y + 2, pStr, colorFrame);

    DrawText(pDst, fontId, x, y - 2, pStr, colorFrame);
    DrawText(pDst, fontId, x, y - 1, pStr, colorFrame);
    DrawText(pDst, fontId, x, y + 2, pStr, colorFrame);
    DrawText(pDst, fontId, x, y + 1, pStr, colorFrame);

    DrawText(pDst, fontId, x, y, pStr, color);
}


void CTextRenderer::DrawTextRight(CImg32 *pDst, FontId fontId, int x, int y, int cx, int cy,
                                  LPCTSTR pStr, COLORREF color)
{
    if (pDst == NULL || pStr == NULL || !m_bInitialized) {
        return;
    }
    if (fontId < 0 || fontId >= FONTID_COUNT || m_apFont[fontId] == NULL) {
        return;
    }

    // テキスト幅を取得して右詰め位置を計算
    int nTextW = 0, nTextH = 0;
    if (!GetTextSize(fontId, pStr, &nTextW, &nTextH)) {
        return;
    }

    // 右詰め: テキストの右端が x + cx に来るようにする
    int nDrawX = x + cx - nTextW;

    // 縁取り（4方向、黒）
    COLORREF colorFrame = RGB(10, 10, 10);
    DrawText(pDst, fontId, nDrawX - 1, y, pStr, colorFrame);
    DrawText(pDst, fontId, nDrawX + 1, y, pStr, colorFrame);
    DrawText(pDst, fontId, nDrawX, y - 1, pStr, colorFrame);
    DrawText(pDst, fontId, nDrawX, y + 1, pStr, colorFrame);
    DrawText(pDst, fontId, nDrawX, y, pStr, color);
}


BOOL CTextRenderer::GetTextSize(FontId fontId, LPCTSTR pStr, int *pWidth, int *pHeight)
{
    if (pStr == NULL || !m_bInitialized) {
        return FALSE;
    }
    if (fontId < 0 || fontId >= FONTID_COUNT || m_apFont[fontId] == NULL) {
        return FALSE;
    }

    std::string strUtf8 = ToUtf8(pStr);
    if (strUtf8.empty()) {
        if (pWidth) *pWidth = 0;
        if (pHeight) *pHeight = 0;
        return TRUE;
    }

    int w = 0, h = 0;
    if (TTF_SizeUTF8(m_apFont[fontId], strUtf8.c_str(), &w, &h) < 0) {
        return FALSE;
    }

    if (pWidth) *pWidth = w;
    if (pHeight) *pHeight = h;
    return TRUE;
}
