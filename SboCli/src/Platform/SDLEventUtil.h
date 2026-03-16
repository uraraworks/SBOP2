/* Copyright(C)URARA-works 2026 */
/* ========================================================================= */
/* File        : SDLEventUtil.h                                               */
/* Contents    : SDL event utility helpers                                    */
/* Created     : 2026/03/16                                                   */
/* ========================================================================= */

#pragma once

#define SDL_MAIN_HANDLED
#include <SDL.h>

inline void PushSDLQuitEvent(void)
{
	SDL_Event ev;

	ZeroMemory (&ev, sizeof (ev));
	ev.type = SDL_QUIT;
	SDL_PushEvent (&ev);
}

/* Copyright(C)URARA-works 2026 */
