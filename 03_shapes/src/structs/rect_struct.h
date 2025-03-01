#ifndef RECT_STRUCT_H
#define RECT_STRUCT_H

#include "point.h"
#include "color.h"

// Téglalap struktúra (sarokpontok, szín)
typedef struct Rectangle {
    Point p1, p2;
    Color color;
} Rectangle;

#endif