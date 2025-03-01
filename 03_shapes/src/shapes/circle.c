#include "shapes/circle.h"
#include "core/consts.h"

void set_circle(Circle* circle, Point center, float radius, Color color) {
    circle->center = center;
    circle->radius = radius;
    circle->color = color;
}

bool push_circle(Circle* stack, int* stack_pointer, Circle circle) {
    if (*stack_pointer < MAX_CIRCLE_COUNT - 1) {
        (*stack_pointer)++;
        stack[*stack_pointer] = circle;
        return true;
    }
    return false;
}

bool pop_circle(Circle* stack, int* stack_pointer, Circle* circle) {
    if (*stack_pointer >= 0) {
        *circle = stack[*stack_pointer];
        (*stack_pointer)--;
        return true;
    }
    return false;
}