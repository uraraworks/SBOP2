/// @file TextRenderer.h
/// @brief SDL_ttf テキスト描画クラス 定義ファイル
/// @date 2026/04/05

#pragma once

#include <SDL_ttf.h>

// 前方宣言
class CImg32;

// フォントID（既存HFONTメンバに1:1対応）
enum FontId {
    FONTID_PGOTHIC_16_BOLD = 0,    // WindowBase::m_hFont (MS Pゴシック 16 ULTRABOLD)
    FONTID_PGOTHIC_12_NORMAL,      // WindowBase::m_hFont12 (MS Pゴシック 12 NORMAL)
    FONTID_PGOTHIC_12_BOLD,        // WindowBase::m_hFont12Bold (MS Pゴシック 12 ULTRABOLD)
    FONTID_PGOTHIC_14_BOLD,        // WindowBase::m_hFont14 (MS Pゴシック 14 ULTRABOLD)
    FONTID_GOTHIC_16_BOLD,         // WindowBase::m_hFont16 (MS ゴシック 16 ULTRABOLD)
    FONTID_GOTHIC_16_NORMAL,       // WindowBase::m_hFont16Normal (MS ゴシック 16 NORMAL)
    FONTID_GOTHIC_12_NORMAL,       // LayerBase::m_hFont (MS ゴシック 12 NORMAL)
    FONTID_GOTHIC_32_NORMAL,       // LayerMap::m_hFont32 (MS ゴシック 32 NORMAL)
    FONTID_COUNT
};

// テキスト描画クラス（シングルトン）
class CTextRenderer
{
public:
    static CTextRenderer &Instance();

    BOOL Init();
    void Destroy();

    // CImg32 のピクセルバッファへ直接テキスト描画
    void DrawText(CImg32 *pDst, FontId fontId, int x, int y, LPCTSTR pStr, COLORREF color);

    // 縁取り付きテキスト描画（4方向オフセット）
    void DrawTextOutlined(CImg32 *pDst, FontId fontId, int x, int y, LPCTSTR pStr,
                          COLORREF color, COLORREF colorFrame);

    // 太い縁取り付きテキスト描画（20方向オフセット）
    void DrawTextOutlinedThick(CImg32 *pDst, FontId fontId, int x, int y, LPCTSTR pStr,
                               COLORREF colorFrame, COLORREF color);

    // 右詰めテキスト描画（縁取り付き）
    void DrawTextRight(CImg32 *pDst, FontId fontId, int x, int y, int cx, int cy,
                       LPCTSTR pStr, COLORREF color);

    // テキストサイズ取得
    BOOL GetTextSize(FontId fontId, LPCTSTR pStr, int *pWidth, int *pHeight);

private:
    CTextRenderer();
    ~CTextRenderer();
    CTextRenderer(const CTextRenderer &) = delete;
    CTextRenderer &operator=(const CTextRenderer &) = delete;

    // LPCTSTR → UTF-8 変換
    std::string ToUtf8(LPCTSTR pStr);

    // SDL_Surface のピクセルを CImg32 のピクセルバッファにアルファブレンドコピー
    void BlitSurfaceToImg32(CImg32 *pDst, SDL_Surface *pSurf, int dstX, int dstY);

    // フォントをロードする
    TTF_Font *LoadFont(const char *pszFontName, int nFaceIndex, int nSize, int nStyle);

private:
    BOOL m_bInitialized;
    TTF_Font *m_apFont[FONTID_COUNT];
};
