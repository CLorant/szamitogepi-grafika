#ifndef CIRCLE_H
#define CIRCLE_H

#include <stdbool.h>
#include "structs/circle_struct.h"

void set_circle(Circle* circle, Point center, float radius, Color color);

bool push_circle(Circle* stack, int* stack_pointer, Circle circle);

bool pop_circle(Circle* stack, int* stack_pointer, Circle* circle);

#endif