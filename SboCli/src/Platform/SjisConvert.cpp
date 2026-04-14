/// @file SjisConvert.cpp
/// @brief Emscripten 環境向け SJIS→wchar_t 変換実装
/// @date 2026/04/14
///
/// Win32 環境ではこのファイルはコンパイルされない（BrowserCompat.h 経由の
/// !_WIN32 ガードで保護）。Emscripten ではブラウザの TextDecoder API を使い
/// SJIS (CP932) バイト列を UTF-8 経由で wstring に変換する。

#if !defined(_WIN32)

#include "SjisConvert.h"
#include "TCharCompat.h"

#if defined(__EMSCRIPTEN__)
#include <emscripten/em_js.h>

/// JavaScript の TextDecoder('shift-jis') を使って SJIS バイト列を UTF-8 に変換する。
/// 戻り値は Emscripten ヒープ上に mallocc されたポインタ。呼び出し元が free すること。
EM_JS(char*, sjis_bytes_to_utf8_js, (const char* ptr, int len), {
    // WASM メモリから SJIS バイト列をコピー
    var sjisBytes = new Uint8Array(Module.HEAPU8.buffer, ptr, len);
    // TextDecoder で SJIS → UTF-16 string
    var decoder = new TextDecoder('shift-jis', { fatal: false });
    var str = decoder.decode(sjisBytes);
    // UTF-16 → UTF-8 バイト列
    var encoder = new TextEncoder();
    var utf8Bytes = encoder.encode(str);
    // Emscripten ヒープに確保してコピー
    var buf = Module._malloc(utf8Bytes.length + 1);
    Module.HEAPU8.set(utf8Bytes, buf);
    Module.HEAPU8[buf + utf8Bytes.length] = 0;
    return buf;
});

/// SJIS バイト列を wstring に変換する（Emscripten 実装）
std::wstring SjisToWstring(const char* pszSrc, int nSrcLen)
{
    if (pszSrc == nullptr || nSrcLen == 0) {
        return std::wstring();
    }
    int srcLen = (nSrcLen < 0) ? static_cast<int>(strlen(pszSrc)) : nSrcLen;
    if (srcLen <= 0) {
        return std::wstring();
    }

    // JavaScript TextDecoder 経由で SJIS → UTF-8 に変換
    char* pszUtf8 = sjis_bytes_to_utf8_js(pszSrc, srcLen);
    if (pszUtf8 == nullptr) {
        return std::wstring();
    }

    // UTF-8 → wstring
    std::wstring result = Utf8ToWstring(pszUtf8, strlen(pszUtf8));
    free(pszUtf8);
    return result;
}

#else
// !__EMSCRIPTEN__ かつ !_WIN32 の環境（Linux 等）向けのフォールバック
// 現状このプロジェクトではそのような環境はないが念のため UTF-8 として扱う
std::wstring SjisToWstring(const char* pszSrc, int nSrcLen)
{
    if (pszSrc == nullptr) {
        return std::wstring();
    }
    size_t srcLen = (nSrcLen < 0) ? strlen(pszSrc) : static_cast<size_t>(nSrcLen);
    return Utf8ToWstring(pszSrc, srcLen);
}
#endif // __EMSCRIPTEN__

#endif // !_WIN32
