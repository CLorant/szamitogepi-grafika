#ifndef UTILS_H
#define UTILS_H

#include <obj/model.h>
#include <stdbool.h>

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
 * Matrix 3x3.
 */
typedef float Mat3[3][3];

/**
 * Matrix 4x4.
 */
typedef float Mat4[4][4];

/**
 * Bounding box with min and max vectors.
 */
typedef struct BoundingBox {
    Vec3 min;
    Vec3 max;
} BoundingBox;

/**
 * Transform for handling static and animation transformations.
 * Has specific flags for preventing transformation errors and easier use.
 */
typedef struct Transform {
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
    bool position_changed;
    bool rotation_changed;
    bool scale_changed;
    bool matrix_dirty;
    Mat4 matrix;
} Transform;

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
 * Calculate the bounding box of a model.
 */
float calculate_model_bounding_radius(Model* model);

/**
 * Get an identity matrix for size dimensions.
 */
void mat_identity(int size, float m[size][size]);

/**
 * Multiply two matrices of s dimensions.
 */
void mat_multiply(int s, float result[s][s], float a[s][s], float b[s][s]);

/**
 * Multiply two 4x4 matrices in place.
 */
void mat4_mul_inplace(Mat4 m, Mat4 op);

/**
 * Translate / Shift a 4x4 matrix.
 */
void mat4_translate(Mat4 m, float x, float y, float z);

/**
 * Scale a 4x4 matrix.
 */
void mat4_scale(Mat4 m, float x, float y, float z);

/**
 * Rotate a 4x4 matrix.
 */
void mat4_rotate(Mat4 m, float angle_degrees, float x, float y, float z);

/**
 * Rotate a 3x3 matrix using the Rodrigues' rotation formula.
 * The rotation is cancelled in edge cases.
 */
void calculate_rotation_matrix(Mat3 result, float angle_degrees, float x, float y, float z);

/**
 * Update the transform matrix.
 */
void update_transform_matrix(Transform* t);

/**
 * Initialize the transform structure.
 */
void transform_init(Transform* transform);

/**
 * Mark transform components as dirty.
 */
void mark_dirty(Transform* t, bool pos, bool rot, bool scl);

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
