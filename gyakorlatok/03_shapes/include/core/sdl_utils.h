#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "structs/draw_mode.h"
#include "structs/color.h"

bool init_sdl(SDL_Window** window, SDL_Renderer** renderer);

void cleanup_sdl(SDL_Window* window, SDL_Renderer* renderer);

#endif