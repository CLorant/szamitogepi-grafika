#ifndef CIRCLE_H
#define CIRCLE_H

typedef struct Brick {
    double length;
    double width;
    double height;
} Brick;

void set_size(Cuboid* cuboid);

float calc_volume(Cuboid cuboid);

float calc_surface(Cuboid cuboid);

int has_square_face(Cuboid cuboid);

#endif