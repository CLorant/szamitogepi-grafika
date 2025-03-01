#ifndef CIRCLE_STRUCT_H
#define CIRCLE_STRUCT_H

#include "point.h"
#include "color.h"

// Kör struktúra (középpont, sugár, szín)
typedef struct Circle {
	Point center;
    float radius;
    Color color;
} Circle;

#endif