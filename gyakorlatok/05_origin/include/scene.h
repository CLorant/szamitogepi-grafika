#ifndef SCENE_H
#define SCENE_H

#include "camera.h"

typedef struct Scene {
    float sphere_rotation;
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Update the scene.
 */
void update_scene(Scene* scene);

/**
 * Render the scene objects.
 */
void render_scene(const Scene* scene);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin(float size);

/**
 * Draw a tri colored triangle.
 */
void draw_triangle(float x, float y, float z, float size);

/**
 * Draw a low tessalated sphere.
 */
void draw_sphere(float x, float y, float z, float radius, float rotation_angle);

/**
 * Draw a checkerboard at the bottom of origin.
 */
void draw_checkerboard(int size, float square_size);

/**
 * Draw a cylinder using triangle strip.
 */
void draw_cylinder(float radius, float height, int segments);

/**
 * Draw a cone using triangle fan.
 */
void draw_cone(float radius, float height, int segments);

#endif /* SCENE_H */