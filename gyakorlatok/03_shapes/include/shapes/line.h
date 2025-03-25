#ifndef LINE_H
#define LINE_H

#include <stdbool.h>
#include "structs/line_struct.h"

void set_line(Line* line, Point p1, Point p2, Color color);

bool push_line(Line* stack, int* stack_pointer, Line line);

bool pop_line(Line* stack, int* stack_pointer, Line* line);

#endif