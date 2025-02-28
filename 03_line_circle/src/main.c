#include "shapes/line.h"
#include "shapes/rectangle.h"
#include "palette.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_TITLE            "3. gyakorlati feladat: Szakaszok rajzolása, Kör közelítése"

#define START_TITLE             "Instrukciók"
#define START_DESC              "1) Válasszon vonal(F1) vagy téglalap(F2) rajzolási módok közül.\n2) Kattintson a színpalettára színválasztáshoz.\n3) Kattintson az alakzat elkezdéséhez.\n4) Kattintson újra az alakzat befejezéséhez."

#define STACK_OVERFLOW_TITLE    "Stack overflow"
#define STACK_OVERFLOW_DESC     "Nem lehet több alakzatot menteni!"
#define STACK_UNDERFLOW_TITLE   "Stack underflow"
#define STACK_UNDERFLOW_DESC    "Nincs több mentett alakzat!"

#define MSG_START_SET           "\nKiinduló pont: (%3d, %3d)\nKattintson a %s befejezéséhez.\n"
#define MSG_END_SET             "\n%s beállítva!\n(%3d, %3d) -> (%3d, %3d)\n"
#define MSG_UNDO                "Alakzat visszatörölve\n"
#define MSG_REDO                "Alakzat visszaállítva\n"    
#define MSG_SWITCH_SHAPE        "Váltás %s alakzatra\n"       

typedef enum { MODE_LINE, MODE_RECTANGLE } DrawMode;

Line line_stack[MAX_LINE_COUNT];
Rectangle rect_stack[MAX_RECT_COUNT];
int line_stack_pointer = -1;
int rect_stack_pointer = -1;

int main(int argc, char* argv[]) {
    ///////// TEMP /////////
    (void)argc;
    (void)argv;
    ///////// TEMP /////////

    printf("\n%s\n\n", WINDOW_TITLE);

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, START_TITLE, START_DESC, window);

    bool firstClick = false, secondClick = false;
    Point p1, p2;
    Color current_color = {255, 0, 0};

    DrawMode current_mode = MODE_LINE;

    bool need_run = true;
    SDL_Event event;

    while (need_run) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    need_run = false;
                    break;

                case SDL_MOUSEBUTTONDOWN: {
                    int mx, my;
                    SDL_GetMouseState(&mx, &my);

                    if (is_cursor_over_palette(mx, my)) {
                        get_color_from_palette(&current_color, mx, my);
                    }
                    else {
                        if (!firstClick) {
                            p1.x = mx;
                            p1.y = my;
                            firstClick = true;
                            printf(MSG_START_SET, p1.x, p1.y, "vonal");
                        }
                        else if (!secondClick) {
                            p2.x = mx;
                            p2.y = my;
                            secondClick = true;

                            if (current_mode == MODE_LINE) {
                                Line newLine;
                                set_line(&newLine, p1, p2, current_color);
                                if (push_line(line_stack, &line_stack_pointer, newLine)) {
                                    printf(MSG_END_SET, "Vonal", p1.x, p1.y, p2.x, p2.y);
                                }
                                else {
                                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, STACK_OVERFLOW_TITLE, STACK_OVERFLOW_DESC, window);
                                }
                            }
                            else if (current_mode == MODE_RECTANGLE) {
                                Rectangle newRect;
                                set_rect(&newRect, p1, p2, current_color);
                                if (push_rect(rect_stack, &rect_stack_pointer, newRect)) {
                                    printf(MSG_END_SET, "Téglalap", p1.x, p1.y, p2.x, p2.y);
                                }
                                else {
                                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, STACK_OVERFLOW_TITLE, STACK_OVERFLOW_DESC, window);
                                }
                            }

                            firstClick = false;
                            secondClick = false;
                        }
                    }
                    break;
                }

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_q) {
                        need_run = false;
                    }
                    else if (event.key.keysym.sym == SDLK_z) { 
                        if (current_mode == MODE_LINE && line_stack_pointer >= 0) {
                            line_stack_pointer--;
                        }
                        else if (current_mode == MODE_RECTANGLE && rect_stack_pointer >= 0) {
                            rect_stack_pointer--;
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_F1) {
                        current_mode = MODE_LINE;
                        printf(MSG_SWITCH_SHAPE, "vonal");
                    }
                    else if (event.key.keysym.sym == SDLK_F2) {
                        current_mode = MODE_RECTANGLE;
                        printf(MSG_SWITCH_SHAPE, "téglalap");
                    }
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        // A téglalapok előszöri kirajzolása hogy ne rejtse el a szakaszokat
        for (int i = 0; i <= rect_stack_pointer; i++) {
            Rectangle r = rect_stack[i];
            SDL_Rect sdlRect = {
                r.p1.x, r.p1.y,
                r.p2.x - r.p1.x,
                r.p2.y - r.p1.y
            };
            SDL_SetRenderDrawColor(renderer, r.color.r, r.color.g, r.color.b, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(renderer, &sdlRect);
        }

        for (int i = 0; i <= line_stack_pointer; i++) {
            Line l = line_stack[i];
            SDL_SetRenderDrawColor(renderer, l.color.r, l.color.g, l.color.b, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(renderer, l.p1.x, l.p1.y, l.p2.x, l.p2.y);
        }

        draw_palette(renderer);

        if (firstClick && !secondClick) {
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            
            if (current_mode == MODE_LINE) {
                SDL_SetRenderDrawColor(renderer, current_color.r, current_color.g, current_color.b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLine(renderer, p1.x, p1.y, mx, my);
            }
            else if (current_mode == MODE_RECTANGLE) {
                SDL_Rect preview = { p1.x, p1.y, mx - p1.x, my - p1.y };
                SDL_SetRenderDrawColor(renderer, current_color.r, current_color.g, current_color.b, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(renderer, &preview);
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
