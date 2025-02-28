#ifndef PALETTE_H
#define PALETTE_H

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "structs/color.h"

void draw_palette(SDL_Renderer* renderer);

bool is_cursor_over_palette(int mx, int my);

void get_color_from_palette(Color* color, int mx, int my);

#endif