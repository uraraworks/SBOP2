/// @file SdlFont.h
/// @brief SDL_ttfベースのフォント管理・テキスト描画（ピクセルフォントを 2 倍で描く）
/// @date 2026/04/06

#pragma once

#if !defined(_WINDLL)

#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include <string>

// DCコンテキスト情報（CImg32::Lock()で登録）
struct SdlDCContext {
    unsigned char* pBits;    // CImg32のピクセルバッファ（BGRA, ボトムアップ）
    int width;               // バッファ幅
    int height;              // バッファ高さ
    int stride;              // 1行あたりのバイト数（width * 4）
    void* currentFont;       // SelectObjectで設定された現在フォント
    unsigned long textColor; // SetTextColorで設定された色（COLORREF: 0x00BBGGRR）
};

// フォントシステム初期化・終了
bool SdlFontInit(const char* fontDir);
void SdlFontShutdown();

// フォント作成・破棄
// height: フォント高さ（ピクセル）、bold: 太字かどうか
void* SdlFontCreate(int height, bool bold);
void SdlFontDestroy(void* hFont);

// DCコンテキスト管理
void* SdlDCRegister(unsigned char* pBits, int width, int height);
void SdlDCUnregister(void* hDC);
SdlDCContext* SdlDCGet(void* hDC);

// テキスト描画（CImg32バッファに直接描画）
// TextOut互換: DCコンテキストの現在フォント・色を使う
bool SdlFontTextOut(void* hDC, int x, int y, const wchar_t* pStr, int nLen);
bool SdlFontTextOutA(void* hDC, int x, int y, const char* pStr, int nLen);

// 文字の 1 ドットの大きさ（ピクセル）。縁取りのずらし幅などに使う
int SdlFontPixelScale();

// 縁取り付きで描く（DC の現在フォントを使う）。縁は 1 ドット。
// bThick=false: 上下左右の 4 方向、true: 斜めも含む 8 方向
bool SdlFontTextOutFramed(void* hDC, int x, int y, const wchar_t* pStr, int nLen,
                          unsigned long color, unsigned long colorFrame, bool bThick);

// テキストサイズ取得
bool SdlFontGetTextExtent(void* hFont, const wchar_t* pStr, int nLen, int* pWidth, int* pHeight);

#endif // !_WINDLL
