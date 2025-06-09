#ifndef LIGHTING_H
#define LIGHTING_H

#include "utils.h"
#include "physics.h"
#include <stdbool.h>

typedef struct Scene Scene;

/**
 * Light source with ambient, diffuse, specular color specifications and position.
 */
typedef struct Lighting {
    bool enabled;
    int slot;
    char name[50];
    ColorRGBA ambient;
    ColorRGBA diffuse;
    ColorRGBA specular;
    Vec4 position;
    Vec3 direction;
    float cutoff;
    float exponent;
    float brightness;
    char room_name[64];
    bool is_spotlight;
} Lighting;


/**
 * Add a light source to the scene.
 */
void add_light(Scene* scene, Lighting* config);

/**
 * Enable/disable and upload one light to GL
 */
void set_lighting(int slot, const Lighting* light);

/**
 * Update the lights of the scene based on time.
 */
void update_lighting(Scene* scene, float elapsed_time);

/**
 * Adjust the global brightness of the app.
 */
void adjust_brightness(Scene* scene, float amount);

#endif /* LIGHT_H */
