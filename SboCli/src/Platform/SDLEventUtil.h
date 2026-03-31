/// @file SDLEventUtil.h
/// @brief SDL event utility helpers
/// @date 2026/03/16
/// @copyright Copyright(C)URARA-works 2026

#pragma once

#define SDL_MAIN_HANDLED
#include <SDL.h>

inline void PushSDLQuitEvent(void)
{
	SDL_Event ev;

	ZeroMemory(&ev, sizeof(ev));
	ev.type = SDL_QUIT;
	SDL_PushEvent(&ev);
}
