#include <SDL2/SDL.h>
#include <stdbool.h>

#include "sdl/sdl_utils.h"
#include "sdl/drawing.h"
#include "sdl/event_handler.h"
#include "sdl/consts.h"

#include "shapes/line.h"
#include "shapes/rectangle.h"

#define DEFAULT_MODE MODE_LINE
#define DEFAULT_COLOR {255, 0, 0}

int main(int argc, char* argv[]) {
    (void) argc; 
    (void) argv; // Nem használt paraméterek kezelése
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    if (!init_sdl(&window, &renderer)) {
        return 1;
    }

    DrawMode current_mode = DEFAULT_MODE;
    Color current_color = DEFAULT_COLOR;
    bool need_run = true;

    while (need_run) {
        process_events(&need_run, &current_mode, &current_color, window);
        draw_scene(renderer, current_mode, current_color);
    }

    cleanup_sdl(window, renderer);
    
    return 0;
}
