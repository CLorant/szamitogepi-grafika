#ifndef PALETTE_H
#define PALETTE_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "consts.h"
#include "structs/color.h"

// Paletta szélessége és magassága
#define PALETTE_WIDTH 300
#define PALETTE_HEIGHT 300

// Paletta pozíciója a képernyőn
#define PALETTE_X (SCREEN_WIDTH - PALETTE_WIDTH - 10) 
#define PALETTE_Y (SCREEN_HEIGHT - PALETTE_HEIGHT - 10) 

// Paletta rajzolása
void draw_palette(SDL_Renderer* renderer);

// Az egérmutató a palettán van-e
bool is_cursor_over_palette(int mx, int my);

// Szín kiválasztása a palettáról
void get_color_from_palette(Color* color, int mx, int my);

#endif