#include <SDL2/SDL.h>

#include "line.h"
#include "circle.c"

#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int argc, char* argv[]) {
    
    int error_code;
    SDL_Window* window;
    bool need_run;
    SDL_Event event;
    SDL_Renderer* renderer;
    int i;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return error_code;
    }

    window = SDL_CreateWindow(
        "Hello SDL!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    Point p1, p2;
    get_click_coord(&p1);
    get_click_coord(&p2);

    printf("Szakasz eleje: x=%d, y=%d\n", p1.x, p1.y);
    printf("Szakasz vége: x=%d, y=%d\n", p2.x, p2.y);
    
    Color color;
    set_color(&color, 255, 0, 0);
    Line line;
    set_line(&line, p1, p2, color);
    
    SDL_SetRenderDrawColor(renderer, line.color.red, line.color.green, line.color.blue, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, line.point1.x, line.point1.y, line.point2.x, line.point2.y);
    SDL_RenderPresent(renderer);

    need_run = true;
    while (need_run) {
        while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                case SDL_SCANCODE_Q:
                need_run = false;
                break;
            case SDL_QUIT:
                need_run = false;
                break;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}