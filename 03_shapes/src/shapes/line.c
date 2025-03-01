#include "line.h"
#include "sdl/consts.h"

void set_line(Line* line, Point p1, Point p2, Color color) {
    line->p1 = p1;
    line->p2 = p2;
    line->color = color;
}

bool push_line(Line* stack, int* stack_pointer, Line line) {
    if (*stack_pointer < MAX_LINE_COUNT - 1) {
        (*stack_pointer)++;
        stack[*stack_pointer] = line;
        return true;
    }
    return false;
}

bool pop_line(Line* stack, int* stack_pointer, Line* line) {
    if (*stack_pointer >= 0) {
        *line = stack[*stack_pointer];
        (*stack_pointer)--;
        return true;
    }
    return false;
}