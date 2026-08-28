#pragma once

// このヘッダはブラウザ版プリフライトの cl.exe 構文チェック専用スタブ。
// 以前はリポジトリルートの emscripten/emscripten.h に置かれていたが、
// そのパスが Get-IncludeArgs 経由で em++ のインクルードパスにも入ってしまい、
// 本物の Emscripten ヘッダを常に shadow してしまう問題があったため
// tools/msvc-stubs 配下（cl.exe 分岐のみが参照する場所）へ移動した。
// cl.exe 経路からしか到達しないので、#if defined(__EMSCRIPTEN__) の
// 実体分岐は不要。MSVC 用の空スタブ実装だけを残す。

#if defined(__cplusplus)
extern "C" {
#endif

typedef void (*em_arg_callback_func)(void *);
typedef void (*em_callback_func)(void);

#if !defined(EMSCRIPTEN_KEEPALIVE)
#define EMSCRIPTEN_KEEPALIVE
#endif

// 構文チェック用の空スタブ（一度だけ func を呼んでリターン）
static inline void emscripten_set_main_loop_arg(em_arg_callback_func func, void *arg, int fps, int simulate_infinite_loop)
{
    (void)fps;
    (void)simulate_infinite_loop;
    if (func != 0) {
        func(arg);
    }
}

static inline void emscripten_cancel_main_loop(void)
{
}

#if defined(__cplusplus)
}
#endif
