#ifndef CIRCLE_STRUCT_H
#define CIRCLE_STRUCT_H

#include "point.h"
#include "color.h"

typedef struct Circle {
	Point center;
    float radius;
    Color color;
} Circle;

#endif