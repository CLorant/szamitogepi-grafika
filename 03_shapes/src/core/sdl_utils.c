#include <stdio.h>
#include "sdl_utils.h"
#include "structs/draw_state.h"
#include "consts.h"

bool init_sdl(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf(ERR_INIT_SDL_DESC, SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!*window) {
        printf(ERR_CREATE_WINDOW_DESC, SDL_GetError());
        SDL_Quit();
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        char err_message[100];
        sprintf(err_message, ERR_CREATE_RENDERER_DESC, SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, ERR_CRITICAL_TITLE, err_message, *window);
        printf(err_message);
        SDL_DestroyWindow(*window);
        SDL_Quit();

        return false;
    }

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, START_TITLE, START_DESC, *window);

    return true;
}

void cleanup_sdl(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}