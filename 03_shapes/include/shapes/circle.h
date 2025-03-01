#ifndef CIRCLE_H
#define CIRCLE_H

#include <stdbool.h>
#include "structs/circle_struct.h"

// Kör beállítása (középpont, sugár, szín)
void set_circle(Circle* circle, Point center, float radius, Color color);

// Kör hozzáadása a veremhez
bool push_circle(Circle* stack, int* stack_pointer, Circle circle);

// Kör eltávolítása a veremből
bool pop_circle(Circle* stack, int* stack_pointer, Circle* circle);

#endif