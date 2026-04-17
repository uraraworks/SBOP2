#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

typedef void (*em_arg_callback_func)(void *);
typedef void (*em_callback_func)(void);

#if defined(__EMSCRIPTEN__)

// Emscripten ビルド: 実体は Emscripten ランタイムが提供するため extern 宣言のみ。
// ローカルパス解決で誤ってこのヘッダが先に見つかっても、inline 空実装にならないようにする。
#if !defined(EMSCRIPTEN_KEEPALIVE)
#define EMSCRIPTEN_KEEPALIVE __attribute__((used))
#endif

void emscripten_set_main_loop_arg(em_arg_callback_func func, void *arg, int fps, int simulate_infinite_loop);
void emscripten_cancel_main_loop(void);

#else  // 非 Emscripten (Windows/MSVC など)

#if !defined(EMSCRIPTEN_KEEPALIVE)
#define EMSCRIPTEN_KEEPALIVE
#endif

// Windows ビルド用の空スタブ（一度だけ func を呼んでリターン）
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

#endif

#if defined(__cplusplus)
}
#endif
