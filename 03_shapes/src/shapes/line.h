#ifndef LINE_H
#define LINE_H

#include <stdbool.h>
#include "structs/line_struct.h"

// Vonal beállítása (kezdőpont, végpont, szín)
void set_line(Line* line, Point p1, Point p2, Color color);

// Vonal hozzáadása a veremhez
bool push_line(Line* stack, int* stack_pointer, Line line);

// Vonal eltávolítása a veremből
bool pop_line(Line* stack, int* stack_pointer, Line* line);

#endif