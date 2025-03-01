#ifndef DRAWING_H
#define DRAWING_H

#include <SDL2/SDL.h>
#include "structs/line_struct.h"
#include "structs/rect_struct.h"
#include "structs/circle_struct.h"
#include "structs/draw_mode.h"

// Téglalapok rajzolása
void draw_rects(Rectangle* stack, int stack_pointer, SDL_Renderer* renderer);

// Vonalak rajzolása
void draw_lines(Line* stack, int stack_pointer, SDL_Renderer* renderer);

// Egy kör rajzolása
void draw_circle(SDL_Renderer* renderer, Circle c);

// Körök rajzolása
void draw_circles(Circle* stack, int stack_pointer, SDL_Renderer* renderer);

// Előnézet rajzolása (pl. vonal, téglalap, kör)
void draw_preview(SDL_Renderer* renderer, DrawMode current_mode, Color current_color);

// Teljes jelenet rajzolása (alakzatok, paletta, előnézet)
void draw_scene(SDL_Renderer* renderer, DrawMode current_mode, Color current_color);

#endif