#include "shapes/rectangle.h"
#include "core/consts.h"

void set_rect(Rectangle* rect, Point p1, Point p2, Color color) {
    rect->p1 = p1;
    rect->p2 = p2;
    rect->color = color;
}

bool push_rect(Rectangle* stack, int* stack_pointer, Rectangle rect) {
    if (*stack_pointer < MAX_RECT_COUNT - 1) {
        (*stack_pointer)++;
        stack[*stack_pointer] = rect;
        return true;
    }
    return false;
}

bool pop_rect(Rectangle* stack, int* stack_pointer, Rectangle* rect) {
    if (*stack_pointer >= 0) {
        *rect = stack[*stack_pointer];
        (*stack_pointer)--;
        return true;
    }
    return false;
}