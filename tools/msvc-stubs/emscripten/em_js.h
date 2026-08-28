#pragma once

// ブラウザ版プリフライトの cl.exe 構文チェック専用スタブ。
// 本物の emscripten/em_js.h は EM_JS(ret, name, args, ...) で
// JS 本体を埋め込んだ関数を Emscripten ランタイム側に生成するが、
// ここでは構文チェックが目的なので JS 本体（カンマを含む可変長引数）は
// 捨てて、プロトタイプ宣言だけを残す。
//
// 使用例: EM_JS(void, sbop2_ls_set, (const char *key, const char *value), { ... });
//   -> extern "C" void sbop2_ls_set(const char *key, const char *value);
//
// args は呼び出し側で既に括弧付きなので、name と並べるとそのまま
// 関数宣言の引数リストになる。

#if defined(__cplusplus)
#define EM_JS_EXTERN extern "C"
#else
#define EM_JS_EXTERN extern
#endif

#define EM_JS(ret, name, args, ...) EM_JS_EXTERN ret name args;
#define EM_ASYNC_JS(ret, name, args, ...) EM_JS_EXTERN ret name args;
