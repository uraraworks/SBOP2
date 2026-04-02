#pragma once

#if !defined(EMSCRIPTEN_KEEPALIVE)
#define EMSCRIPTEN_KEEPALIVE
#endif

#if defined(__cplusplus)
extern "C" {
#endif

typedef void (*em_arg_callback_func)(void *);

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
