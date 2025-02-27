#include "line.h"

#include <SDL2/SDL.h>
#include <stdbool.h>

void get_click_coord(Point* point) {
    bool quit = false;
    SDL_Event e;
    int x_mouse, y_mouse;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_MOUSEMOTION) {
                SDL_GetGlobalMouseState(&x_mouse, &y_mouse);
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetGlobalMouseState(&x_mouse, &y_mouse);
                point->x = x_mouse;
                point->y = y_mouse;
                quit = true;
            }
        }
    }
}


void set_color(Color* color, int red, int green, int blue) {
    color->red = red;
    color->green = green;
    color->blue = blue;
}

void set_line(Line* line, Point p1, Point p2, Color color) {
    line->point1 = p1;
    line->point2 = p2;
    line->color = color;
}