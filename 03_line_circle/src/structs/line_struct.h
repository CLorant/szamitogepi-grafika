#ifndef LINE_STRUCT_H
#define LINE_STRUCT_H

#include "point.h"
#include "color.h"

typedef struct Line {
    Point p1, p2;
    Color color;
} Line;

#endif