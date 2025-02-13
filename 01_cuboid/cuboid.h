#ifndef CUBOID_H
#define CUBOID_H

typedef struct Cuboid {
    float length;
    float width;
    float height;
} Cuboid;

void get_positive_float(const char *prompt, float *value);

void set_size(Cuboid* cuboid);

float calc_volume(Cuboid cuboid);

float calc_surface(Cuboid cuboid);

int has_square_face(Cuboid cuboid);

#endif