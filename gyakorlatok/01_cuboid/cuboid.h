#ifndef CUBOID_H
#define CUBOID_H

#include <stdbool.h>

/**
 * Cuboid struct
 */
typedef struct Cuboid {
    float length;
    float width;
    float height;
} Cuboid;

/**
 * Gets a positive float value from user input
 */
void get_positive_float(const char *prompt, float *value);

/**
 * Sets the vertices for a cuboid struct
 */
void set_size(Cuboid* cuboid);

/**
 * Calculates the volume of a cuboid
 */
float calc_volume(Cuboid cuboid);


/**
 * Calculates the surface area of a cuboid
 */
float calc_surface(Cuboid cuboid);

/**
 * Checks if the cuboid has any square faces
 */
bool has_square_face(Cuboid cuboid);

#endif