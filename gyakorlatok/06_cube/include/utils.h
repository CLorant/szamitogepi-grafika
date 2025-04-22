#ifndef UTILS_H
#define UTILS_H

#include <obj/model.h>
#include <stdbool.h>

typedef struct Object Object;

/**
 * Threshold to avoid division by near-zero values.
 */
#define EPSILON 1e-6f

/**
 * GLSL-like three dimensional vector.
 */
typedef struct Vec3 {
    float x;
    float y;
    float z;
} Vec3;

/**
 * Four dimensional vector in homogeneous coordinates.
 */
typedef struct Vec4 {
    float x;
    float y;
    float z;
    float w;
} Vec4;

/**
 * Color with RGB components.
 */
typedef struct ColorRGB {
    float red;
    float green;
    float blue;
} ColorRGB;

/**
 * Color with RGBA components.
 */
typedef struct ColorRGBA {
    float red;
    float green;
    float blue;
    float alpha;
} ColorRGBA;

/**
 * Light source with ambient, diffuse, specular color specifications and position.
 */
typedef struct Lighting {
    ColorRGBA ambient;
    ColorRGBA diffuse;
    ColorRGBA specular;
    Vec4 position;
    float brightness;
    bool enabled;
    int slot;
} Lighting;

/**
 * Material with ambient, diffuse, specular color specifications and shininess.
 */
typedef struct Material {
    ColorRGB ambient;
    ColorRGB diffuse;
    ColorRGB specular;
    float shininess;
} Material;

/**
 * Bounding box with min and max vectors.
 */
typedef struct BoundingBox {
    Vec3 min;
    Vec3 max;
} BoundingBox;

/**
 * Transform for handling static and animation transformations.
 */
typedef struct Transform {
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
} Transform;

/**
 * Check if any of the x, y, z slabs of the object was hit.
 */
bool slab_hit(float origin, float dir, float min_box, float max_box, float *tmin, float *tmax);

/**
 * Calculate the bounding box of a model.
 */
void calculate_bounding_box(Object* obj);

/**
 * Set a light source of the scene.
 */
void set_lighting(int slot, const Lighting* light);

/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin(float size);

/**
 * Draw a checkerboard pattern on the ground.
 */
void draw_checkerboard(int size, float square_size);

/**
 * Draw the bounding box of a model.
 */
void draw_bounding_box(Object* obj);

/**
 * Set an RGB color.
 */
void set_rgb(ColorRGB* dest, float red, float green, float blue);

/**
 * Set an RGBA color.
 */
void set_rgba(ColorRGBA* dest, float red, float green, float blue, float alpha);

/**
 * Apply a sine function to an RBG color based on time and light source index.
 */
ColorRGB sine_animate_color(float time, int index, float brightness);

/**
 * Get the min coordinates of 2 three dimensional vectors.
 */
Vec3 vec3_min(Vec3 a, Vec3 b);

/**
 * Get the max coordinates of 2 three dimensional vectors.
 */
Vec3 vec3_max(Vec3 a, Vec3 b);

/**
 * Multiply 2 three dimensional vectors.
 */
Vec3 vec3_multiply(Vec3 v, Vec3 scale);

/**
 * Add 2 three dimensional vectors.
 */
Vec3 vec3_add(Vec3 a, Vec3 b);

/**
 * Get the dot product of 2 three dimensional vectors.
 */
float vec3_dot(Vec3* a, Vec3* b);

/**
 * Normalize a three dimensional vector.
 */
void vec3_normalize(Vec3* v);

/**
 * Calculate radian from degree.
 */
double degree_to_radian(double degree);

/**
 * Initialize the transform structure.
 */
void transform_init(Transform* transform);

/**
 * Set the position, rotation, and scale of the transform.
 */
void set_position(Transform* transform, float x, float y, float z);
void set_rotation(Transform* transform, float x, float y, float z);
void set_scale(Transform* transform, float x, float y, float z);

/**
 * Reset specific components of a transform.
 */
void reset_position(Transform* transform);
void reset_rotation(Transform* transform);
void reset_scale(Transform* transform);
void reset_transform(Transform* transform);

#endif /* UTILS_H */
