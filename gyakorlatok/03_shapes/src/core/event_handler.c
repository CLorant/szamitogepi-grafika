#include <stdio.h>
#include <math.h>
#include "event_handler.h"
#include "consts.h"
#include "shapes/line.h"
#include "shapes/rectangle.h"
#include "shapes/circle.h"
#include "palette.h"

void handle_shape_creation(ShapeType type, Point p1, Point p2, Color color, SDL_Window* window) {
    switch (type) {
        case SHAPE_LINE: {
            Line newLine;
            set_line(&newLine, p1, p2, color);
            if (push_line(line_stack, &line_stack_pointer, newLine)) {
                printf(MSG_END_SET, "Line", p1.x, p1.y, p2.x, p2.y);
            }
            else {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, ERR_RUN_TITLE, ERR_STACK_OVERFLOW_DESC, window);
            }
            break;
        }
        case SHAPE_RECTANGLE: {
            Rectangle newRect;
            set_rect(&newRect, p1, p2, color);
            if (push_rect(rect_stack, &rect_stack_pointer, newRect)) {
                printf(MSG_END_SET, "Rectangle", p1.x, p1.y, p2.x, p2.y);
            }
            else {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, ERR_RUN_TITLE, ERR_STACK_OVERFLOW_DESC, window);
            }
            break;
        }
        case SHAPE_CIRCLE: {
            float radius = sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
            Circle newCircle;
            set_circle(&newCircle, p1, radius, color);
            if (push_circle(circle_stack, &circle_stack_pointer, newCircle)) {
                printf(MSG_END_SET, "Circle", p1.x, p1.y, p2.x, p2.y);
            }
            else {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, ERR_RUN_TITLE, ERR_STACK_OVERFLOW_DESC, window);
            }
            break;
        }
    }
}

void handle_mouse_click(int mx, int my, Color* current_color, DrawMode current_mode, SDL_Window* window) {
    if (is_cursor_over_palette(mx, my)) {
        get_color_from_palette(current_color, mx, my);
    }
    else {
        if (draw_state == STATE_DRAGGING) {
            return;
        }

        if (draw_state == STATE_IDLE) {
            p1.x = mx;
            p1.y = my;
            draw_state = STATE_FIRST_CLICK;
            printf(MSG_START_SET, p1.x, p1.y, "shape");
        }
        else if (draw_state == STATE_FIRST_CLICK) {
            p2.x = mx;
            p2.y = my;
            draw_state = STATE_SECOND_CLICK;

            ShapeType shape_type;
            switch (current_mode) {
                case MODE_LINE: shape_type = SHAPE_LINE; break;
                case MODE_RECTANGLE: shape_type = SHAPE_RECTANGLE; break;
                case MODE_CIRCLE: shape_type = SHAPE_CIRCLE; break;
            }

            handle_shape_creation(shape_type, p1, p2, *current_color, window);
            draw_state = STATE_IDLE;
        }
    }
}

void handle_mouse_move(int mx, int my) {
    for (int i = 0; i <= circle_stack_pointer; i++) {
        Circle c = circle_stack[i];
        int dx = mx - c.center.x;
        int dy = my - c.center.y;
        int distance_squared = dx * dx + dy * dy;

        circle_selected[i] = (distance_squared <= c.radius * c.radius);
    }

    if (draw_state == STATE_DRAGGING && dragged_circle_index != -1) {
        circle_stack[dragged_circle_index].center.x = mx;
        circle_stack[dragged_circle_index].center.y = my;
    }
}

void handle_mouse_down(int mx, int my) {
    if (draw_state == STATE_IDLE) {
        for (int i = 0; i <= circle_stack_pointer; i++) {
            Circle c = circle_stack[i];
            int dx = mx - c.center.x;
            int dy = my - c.center.y;
            int distance_squared = dx * dx + dy * dy;

            if (distance_squared <= c.radius * c.radius) {
                draw_state = STATE_DRAGGING;
                dragged_circle_index = i;
                break;
            }
        }
    }
}

void handle_mouse_up() {
    if (draw_state == STATE_DRAGGING) {
        draw_state = STATE_IDLE;
        dragged_circle_index = -1;
    }
}

void handle_keyboard_input(SDL_Keycode key, DrawMode* current_mode) {
    if (key == SDLK_1) {
        *current_mode = MODE_LINE;
        printf(MSG_SWITCH_SHAPE, "line");
    }
    else if (key == SDLK_2) {
        *current_mode = MODE_RECTANGLE;
        printf(MSG_SWITCH_SHAPE, "rectangle");
    }
    else if (key == SDLK_3) {
        *current_mode = MODE_CIRCLE;
        printf(MSG_SWITCH_SHAPE, "circle");
    }
}

void process_events(bool* need_run, DrawMode* current_mode, Color* current_color, SDL_Window* window) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                *need_run = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (*current_mode == MODE_CIRCLE) {
                    handle_mouse_down(event.button.x, event.button.y);
                }
                if (draw_state != STATE_DRAGGING) {
                    handle_mouse_click(event.button.x, event.button.y, current_color, *current_mode, window);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (*current_mode == MODE_CIRCLE) {
                    handle_mouse_up();
                }
                break;
            case SDL_MOUSEMOTION:
                if (*current_mode == MODE_CIRCLE) {
                    handle_mouse_move(event.motion.x, event.motion.y);
                }
                break;
            case SDL_KEYDOWN:
                handle_keyboard_input(event.key.keysym.sym, current_mode);
                break;
        }
    }
}