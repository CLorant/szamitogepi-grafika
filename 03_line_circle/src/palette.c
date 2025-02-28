#include "palette.h"
#include <stdio.h>

#define PALETTE_WIDTH 300
#define PALETTE_HEIGHT 300
#define PALETTE_X (SCREEN_WIDTH - PALETTE_WIDTH - 10) 
#define PALETTE_Y (SCREEN_HEIGHT - PALETTE_HEIGHT - 10) 

void draw_palette(SDL_Renderer* renderer) {
    for (int x = 0; x < PALETTE_WIDTH; x++) {
        for (int y = 0; y < PALETTE_HEIGHT; y++) {
            Uint8 r = (x * 255) / PALETTE_WIDTH;
            Uint8 g = (y * 255) / PALETTE_HEIGHT;
            Uint8 b = 255 - ((x * 255) / PALETTE_WIDTH);

            SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(renderer, PALETTE_X + x, PALETTE_Y + y);
        }
    }
}

bool is_cursor_over_palette(int mx, int my) {
    return  mx >= PALETTE_X &&
            mx <= PALETTE_X + PALETTE_WIDTH &&
            my >= PALETTE_Y &&
            my <= PALETTE_Y + PALETTE_HEIGHT;
}

void get_color_from_palette(Color* color, int mx, int my) {
    if (is_cursor_over_palette(mx, my)) {
        color->r = ((mx - PALETTE_X) * 255) / PALETTE_WIDTH;
        color->g = ((my - PALETTE_Y) * 255) / PALETTE_HEIGHT;
        color->b = 255 - (((mx - PALETTE_X) * 255) / PALETTE_WIDTH);

        printf("Választott szín - R: %d, G: %d, B: %d\n", color->r, color->g, color->b);
    }
}
