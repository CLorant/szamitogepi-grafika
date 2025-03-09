#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "structs/shape_type.h"
#include "structs/point.h"
#include "structs/color.h"
#include "structs/draw_mode.h"
#include "structs/draw_state.h"

// Vonal, téglalap, kör létrehozása és hozzáadása a veremhez
void handle_shape_creation(ShapeType type, Point p1, Point p2, Color color, SDL_Window* window);

// Egérkattintás kezelése
void handle_mouse_click(int mx, int my, Color* current_color, DrawMode current_mode, SDL_Window* window);

// Egérmozgás kezelése
void handle_mouse_move(int mx, int my);

// Egérgomb lenyomásának kezelése
void handle_mouse_down(int mx, int my);

// Egérgomb felengedésének kezelése
void handle_mouse_up();

// Billentyűzet bemenet kezelése
void handle_keyboard_input(SDL_Keycode key, DrawMode* current_mode);

// Események feldolgozása (pl. egér, billentyűzet)
void process_events(bool* need_run, DrawMode* current_mode, Color* current_color, SDL_Window* window);

#endif