#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <stdbool.h>
#include "structs/rect_struct.h"

// Téglalap beállítása (sarokpontok, szín)
void set_rect(Rectangle* rect, Point p1, Point p2, Color color);

// Téglalap hozzáadása a veremhez
bool push_rect(Rectangle* stack, int* stack_pointer, Rectangle rect);

// Téglalap eltávolítása a veremből
bool pop_rect(Rectangle* stack, int* stack_pointer, Rectangle* rect);

#endif