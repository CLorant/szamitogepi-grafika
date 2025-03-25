#ifndef DRAWING_H
#define DRAWING_H

#include <SDL2/SDL.h>
#include "structs/line_struct.h"
#include "structs/rect_struct.h"
#include "structs/circle_struct.h"
#include "structs/draw_mode.h"

void draw_rects(Rectangle* stack, int stack_pointer, SDL_Renderer* renderer);

void draw_lines(Line* stack, int stack_pointer, SDL_Renderer* renderer);

void draw_circle(SDL_Renderer* renderer, Circle c);

void draw_circles(Circle* stack, int stack_pointer, SDL_Renderer* renderer);

void draw_preview(SDL_Renderer* renderer, DrawMode current_mode, Color current_color);

/**
 * Renders the entire interface (shapes, palette, preview)
 */
void draw_scene(SDL_Renderer* renderer, DrawMode current_mode, Color current_color);

#endif