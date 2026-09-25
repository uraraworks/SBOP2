/// @file SjisConvert.cpp
/// @brief SJIS→wchar_t 変換実装
/// @date 2026/04/14

#include "SjisConvert.h"

#if defined(_WIN32)

#include <windows.h>

/// SJIS バイト列を wstring に変換する（Windows 実装）
std::wstring SjisToWstring(const char* pszSrc, int nSrcLen)
{
    if (pszSrc == nullptr || nSrcLen == 0) {
        return std::wstring();
    }

    int srcLen = (nSrcLen < 0) ? -1 : nSrcLen;
    int required = MultiByteToWideChar(932, 0, pszSrc, srcLen, nullptr, 0);
    if (required <= 0) {
        return std::wstring();
    }

    std::wstring result;
    result.resize(required);
    int converted = MultiByteToWideChar(932, 0, pszSrc, srcLen, &result[0], required);
    if (converted <= 0) {
        return std::wstring();
    }

    result.resize((srcLen < 0) ? (converted - 1) : converted);
    return result;
}

/// wstring を SJIS バイト列に変換する（Windows 実装）
std::string WstringToSjis(const wchar_t* pszSrc, int nSrcLen)
{
    if (pszSrc == nullptr || nSrcLen == 0) {
        return std::string();
    }
    int srcLen = (nSrcLen < 0) ? -1 : nSrcLen;
    int required = WideCharToMultiByte(932, 0, pszSrc, srcLen, nullptr, 0, nullptr, nullptr);
    if (required <= 0) {
        return std::string();
    }
    std::string result;
    result.resize(required);
    int converted = WideCharToMultiByte(932, 0, pszSrc, srcLen, &result[0], required, nullptr, nullptr);
    if (converted <= 0) {
        return std::string();
    }
    result.resize((srcLen < 0) ? (converted - 1) : converted);
    return result;
}

/// SJIS バイト列を UTF-8 文字列に変換する（Windows 実装）
std::string SjisBytesToUtf8(const char* pszSrc)
{
    if (pszSrc == nullptr || pszSrc[0] == '\0') {
        return std::string();
    }
    // SJIS → UTF-16
    int nWide = MultiByteToWideChar(932, 0, pszSrc, -1, nullptr, 0);
    if (nWide <= 0) {
        return std::string();
    }
    std::wstring wstr(nWide, L'\0');
    MultiByteToWideChar(932, 0, pszSrc, -1, &wstr[0], nWide);
    // NULL 終端文字を除く
    if (!wstr.empty() && wstr.back() == L'\0') {
        wstr.pop_back();
    }
    // UTF-16 → UTF-8
    int nUtf8 = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (nUtf8 <= 0) {
        return std::string();
    }
    std::string result(nUtf8, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], nUtf8, nullptr, nullptr);
    if (!result.empty() && result.back() == '\0') {
        result.pop_back();
    }
    return result;
}

#else

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

/// JavaScript 側で UTF-8 文字列を SJIS バイト列に変換する。
/// 初回呼び出し時に TextDecoder('shift-jis') で 全 SJIS バイトシーケンスを舐めて
/// Unicode コードポイント → SJIS バイト列 の逆引きマップを作る。
/// 戻り値: 確保された SJIS バイト列の先頭ポインタ（呼び出し側が free すること）
///         outLen にバイト数を書き込む（NULL 終端は含まない）
EM_JS(char*, utf8_to_sjis_js, (const char* utf8Ptr, int utf8Len, int* outLen), {
    if (!Module._sjisReverseMap) {
        var map = new Map();
        var decoder = new TextDecoder('shift-jis', { fatal: false });
        // 1 バイト SJIS（ASCII および半角カナ）
        for (var b = 0; b < 0x100; b++) {
            // 2 バイト SJIS の lead byte 領域はスキップ
            if ((b >= 0x81 && b <= 0x9F) || (b >= 0xE0 && b <= 0xFC)) {
                continue;
            }
            var bytes = new Uint8Array([b]);
            var ch = decoder.decode(bytes);
            if (ch.length === 1) {
                var cp = ch.charCodeAt(0);
                if (cp !== 0xFFFD) {
                    map.set(cp, [b]);
                }
            }
        }
        // 2 バイト SJIS
        for (var lead = 0x81; lead <= 0xFC; lead++) {
            if (lead > 0x9F && lead < 0xE0) continue;
            for (var trail = 0x40; trail <= 0xFC; trail++) {
                if (trail === 0x7F) continue;
                var bytes2 = new Uint8Array([lead, trail]);
                var ch2 = decoder.decode(bytes2);
                if (ch2.length === 1) {
                    var cp2 = ch2.charCodeAt(0);
                    if (cp2 !== 0xFFFD && !map.has(cp2)) {
                        map.set(cp2, [lead, trail]);
                    }
                }
            }
        }
        Module._sjisReverseMap = map;
    }
    var utf8Bytes = new Uint8Array(Module.HEAPU8.buffer, utf8Ptr, utf8Len);
    var str = new TextDecoder('utf-8', { fatal: false }).decode(utf8Bytes);
    var out = [];
    for (var i = 0; i < str.length; i++) {
        var c = str.charCodeAt(i);
        var mapped = Module._sjisReverseMap.get(c);
        if (mapped) {
            for (var j = 0; j < mapped.length; j++) out.push(mapped[j]);
        } else {
            // SJIS に存在しない文字は '?' に置換
            out.push(0x3F);
        }
    }
    var buf = Module._malloc(out.length + 1);
    Module.HEAPU8.set(out, buf);
    Module.HEAPU8[buf + out.length] = 0;
    HEAP32[outLen >> 2] = out.length;
    return buf;
});

/// wstring を SJIS バイト列に変換する（Emscripten 実装）
std::string WstringToSjis(const wchar_t* pszSrc, int nSrcLen)
{
    if (pszSrc == nullptr) {
        return std::string();
    }
    size_t srcLen = (nSrcLen < 0) ? wcslen(pszSrc) : static_cast<size_t>(nSrcLen);
    if (srcLen == 0) {
        return std::string();
    }
    // 一旦 UTF-8 に変換してから JS 経由で SJIS にエンコード
    std::string utf8 = WstringToUtf8(pszSrc, srcLen);
    if (utf8.empty()) {
        return std::string();
    }
    int outLen = 0;
    char* pszSjis = utf8_to_sjis_js(utf8.c_str(), static_cast<int>(utf8.size()), &outLen);
    if (pszSjis == nullptr) {
        return std::string();
    }
    std::string result(pszSjis, outLen);
    free(pszSjis);
    return result;
}

/// SJIS バイト列を UTF-8 文字列に変換する（Emscripten 実装）
std::string SjisBytesToUtf8(const char* pszSrc)
{
    if (pszSrc == nullptr || pszSrc[0] == '\0') {
        return std::string();
    }
    int srcLen = static_cast<int>(strlen(pszSrc));
    // JavaScript TextDecoder('shift-jis') 経由で SJIS → UTF-8
    char* pszUtf8 = sjis_bytes_to_utf8_js(pszSrc, srcLen);
    if (pszUtf8 == nullptr) {
        return std::string();
    }
    std::string result(pszUtf8);
    free(pszUtf8);
    return result;
}

#else
// !__EMSCRIPTEN__ かつ !_WIN32 の環境（Linux のサーバー）向けの実装。
// glibc の iconv で CP932 と UTF-8 を相互変換する。空の DB で起動した時に
// 読む旧 .dat（CP932）や、CP932 を明示した変換がここを通る。
#include <iconv.h>
#include <cerrno>
#include <cstring>

namespace
{
/// iconv で変換する。変換できないバイト/文字は pszReplace に置き換えて続行する
/// （Windows の MultiByteToWideChar / WideCharToMultiByte が既定文字に置き換えるのに合わせる）。
std::string IconvConvert(const char* pszTo, const char* pszFrom, const char* pSrc, size_t srcLen, const char* pszReplace)
{
    std::string strOut;
    iconv_t cd = iconv_open(pszTo, pszFrom);
    if (cd == reinterpret_cast<iconv_t>(-1)) {
        return strOut;
    }

    char* pIn = const_cast<char*>(pSrc);
    size_t inLeft = srcLen;
    char buf[1024];
    while (inLeft > 0) {
        char* pOut = buf;
        size_t outLeft = sizeof(buf);
        size_t ret = iconv(cd, &pIn, &inLeft, &pOut, &outLeft);
        strOut.append(buf, sizeof(buf) - outLeft);
        if (ret != static_cast<size_t>(-1)) {
            continue;
        }
        if (errno == E2BIG) {
            continue;	// 出力バッファを空けて続ける
        }
        // EILSEQ / EINVAL: 変換できない1単位を置き換えて読み飛ばす
        strOut.append(pszReplace);
        size_t skip = 1;
        if (strcmp(pszFrom, "UTF-8") == 0) {
            // UTF-8 は1文字ぶん（先頭バイト＋継続バイト）を飛ばす
            while ((skip < inLeft) && ((static_cast<unsigned char>(pIn[skip]) & 0xC0) == 0x80)) {
                ++skip;
            }
        }
        if (skip > inLeft) {
            skip = inLeft;
        }
        pIn += skip;
        inLeft -= skip;
        iconv(cd, NULL, NULL, NULL, NULL);	// シフト状態を戻す
    }
    iconv_close(cd);
    return strOut;
}
}

std::wstring SjisToWstring(const char* pszSrc, int nSrcLen)
{
    if (pszSrc == nullptr) {
        return std::wstring();
    }
    size_t srcLen = (nSrcLen < 0) ? strlen(pszSrc) : static_cast<size_t>(nSrcLen);
    std::string strUtf8 = IconvConvert("UTF-8", "CP932", pszSrc, srcLen, "?");
    return Utf8ToWstring(strUtf8.c_str(), strUtf8.size());
}

std::string WstringToSjis(const wchar_t* pszSrc, int nSrcLen)
{
    if (pszSrc == nullptr) {
        return std::string();
    }
    size_t srcLen = (nSrcLen < 0) ? wcslen(pszSrc) : static_cast<size_t>(nSrcLen);
    std::string strUtf8 = WstringToUtf8(pszSrc, srcLen);
    return IconvConvert("CP932", "UTF-8", strUtf8.c_str(), strUtf8.size(), "?");
}

/// SJIS バイト列を UTF-8 文字列に変換する（Linux 実装）
std::string SjisBytesToUtf8(const char* pszSrc)
{
    if (pszSrc == nullptr) {
        return std::string();
    }
    return IconvConvert("UTF-8", "CP932", pszSrc, strlen(pszSrc), "?");
}
#endif // __EMSCRIPTEN__

#endif // _WIN32
