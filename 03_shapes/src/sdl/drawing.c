#include <math.h>

#include "structs/draw_state.h"
#include "structs/draw_mode.h"
#include "drawing.h"
#include "consts.h"
#include "shapes/line.h"
#include "shapes/rectangle.h"
#include "shapes/circle.h"
#include "palette.h"

Line line_stack[MAX_LINE_COUNT];
int line_stack_pointer = -1;
Rectangle rect_stack[MAX_RECT_COUNT];
int rect_stack_pointer = -1;
Circle circle_stack[MAX_CIRCLE_COUNT];
int circle_stack_pointer = -1;
bool circle_selected[MAX_CIRCLE_COUNT] = {false};
bool dragging = false;
int dragged_circle_index = -1;

DrawState draw_state = STATE_IDLE;
Point p1, p2;

void draw_rects(Rectangle* stack, int stack_pointer, SDL_Renderer* renderer) {
    for (int i = 0; i <= stack_pointer; i++) {
        Rectangle r = stack[i];
        SDL_Rect sdlRect = { r.p1.x, r.p1.y, r.p2.x - r.p1.x, r.p2.y - r.p1.y };
        SDL_SetRenderDrawColor(renderer, r.color.r, r.color.g, r.color.b, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &sdlRect);
    }
}

void draw_lines(Line* stack, int stack_pointer, SDL_Renderer* renderer) {
    for (int i = 0; i <= stack_pointer; i++) {
        Line l = stack[i];
        SDL_SetRenderDrawColor(renderer, l.color.r, l.color.g, l.color.b, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, l.p1.x, l.p1.y, l.p2.x, l.p2.y);
    }
}

void draw_circle(SDL_Renderer* renderer, Circle c) {
    Line lines[CIRCLE_SEGMENTS];
    int line_count = 0;

    for (int i = 0; i < CIRCLE_SEGMENTS; i++) {
        float angle = 2.0f * M_PI * i / CIRCLE_SEGMENTS;
        float next_angle = 2.0f * M_PI * (i + 1) / CIRCLE_SEGMENTS;

        Point p1 = {
            .x = c.center.x + (int)(c.radius * cosf(angle)),
            .y = c.center.y + (int)(c.radius * sinf(angle))
        };

        Point p2 = {
            .x = c.center.x + (int)(c.radius * cosf(next_angle)),
            .y = c.center.y + (int)(c.radius * sinf(next_angle))
        };

        set_line(&lines[line_count], p1, p2, c.color);
        line_count++;
    }

    draw_lines(lines, line_count - 1, renderer);
}

void draw_circles(Circle* stack, int stack_pointer, SDL_Renderer* renderer) {
    for (int i = 0; i <= stack_pointer; i++) {
        Circle c = stack[i];
        SDL_SetRenderDrawColor(renderer, c.color.r, c.color.g, c.color.b, SDL_ALPHA_OPAQUE);
        draw_circle(renderer, c);

        if (circle_selected[i]) {
            SDL_SetRenderDrawColor(renderer, c.color.r, c.color.g, c.color.b, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(renderer, c.center.x - CROSSHAIR_SIZE, c.center.y, c.center.x + CROSSHAIR_SIZE, c.center.y);
            SDL_RenderDrawLine(renderer, c.center.x, c.center.y - CROSSHAIR_SIZE, c.center.x, c.center.y + CROSSHAIR_SIZE);
        }
    }
}

void draw_preview(SDL_Renderer* renderer, DrawMode current_mode, Color current_color) {
    if (draw_state == STATE_FIRST_CLICK) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        SDL_SetRenderDrawColor(renderer, current_color.r, current_color.g, current_color.b, SDL_ALPHA_OPAQUE);

        switch (current_mode) {
            case MODE_LINE: {
                SDL_RenderDrawLine(renderer, p1.x, p1.y, mx, my);
                break;
            }
            case MODE_RECTANGLE: {
                SDL_Rect preview_rect = { p1.x, p1.y, mx - p1.x, my - p1.y };
                SDL_RenderDrawRect(renderer, &preview_rect);
                break;
            }
            case MODE_CIRCLE: {
                int dx = mx - p1.x;
                int dy = my - p1.y;
                int radius = (int)sqrt(dx * dx + dy * dy);

                Circle preview_circle = {
                    .center = p1,
                    .radius = radius,
                    .color = current_color
                };

                draw_circle(renderer, preview_circle);
                break;
            }
        }
    }
}

void draw_scene(SDL_Renderer* renderer, DrawMode current_mode, Color current_color) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    draw_rects(rect_stack, rect_stack_pointer, renderer);
    draw_lines(line_stack, line_stack_pointer, renderer);
    draw_circles(circle_stack, circle_stack_pointer, renderer);
    draw_palette(renderer);
    draw_preview(renderer, current_mode, current_color);

    SDL_RenderPresent(renderer);
}