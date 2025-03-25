#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <stdbool.h>
#include "structs/rect_struct.h"

void set_rect(Rectangle* rect, Point p1, Point p2, Color color);

bool push_rect(Rectangle* stack, int* stack_pointer, Rectangle rect);

bool pop_rect(Rectangle* stack, int* stack_pointer, Rectangle* rect);

#endif