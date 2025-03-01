#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "structs/draw_mode.h"
#include "structs/color.h"

// SDL inicializálása (ablak és renderer létrehozása)
bool init_sdl(SDL_Window** window, SDL_Renderer** renderer);

// SDL takarítása (ablak és renderer megsemmisítése)
void cleanup_sdl(SDL_Window* window, SDL_Renderer* renderer);

#endif