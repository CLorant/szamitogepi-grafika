#ifndef UTILS_H
#define UTILS_H

#include <obj/model.h>
#include <stdbool.h>
#include <GL/gl.h>

#define EPSILON 1e-6f
#define MAX_LIGHTS 8
#define MAX_OBJECTS 64
#define MAX_ROOMS 32

typedef struct Object Object;
typedef struct Room Room;
typedef struct PhysicsBody PhysicsBody;

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
 * Direction enum with counter
 */
typedef enum Direction {
    DIR_NORTH,
    DIR_EAST,
    DIR_SOUTH,
    DIR_WEST,
    DIR_COUNT
} Direction;

/**
 * Room connections
 */
typedef struct RoomConn {
    char room[64];
    int dir;
} RoomConn;

/**
 * Light source with ambient, diffuse, specular color specifications and position.
 */
typedef struct Lighting
{
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
 * Material with ambient, diffuse, specular color specifications and shininess.
 */
typedef struct Material {
    ColorRGB ambient;
    ColorRGB diffuse;
    ColorRGB specular;
    float shininess;
} Material;

/**
 * Scale the vertices of a model.
 */
void scale_model(Model* model, Vec3 scale);

/**
 * Rotate the vertices of a model.
 */
void rotate_model(Model* model, Vec3 rotation);

/**
 * Calculate the bounding box of a model.
 */
void calculate_mesh_aabb(Model* model, Vec3* out_min, Vec3* out_max);

/**
 * Check if any of the x, y, z slabs of the object was hit.
 */
bool slab_hit(float origin, float dir, float min_box, float max_box, float *tmin, float *tmax);

/**
 * Get the delta (-1, 0, 1) of a direction.
 */
void get_delta(int dir, int* dx, int* dy);

/**
 * Set a light source of the scene.
 */
void set_lighting(int slot, const Lighting* light);

/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Draw a room of the scene.
 */
void draw_room(Room* room);

/**
 * Draw a line of text.
 */
void draw_string(GLuint charmap_id, const char* s, float start_x, float start_y, float line_height);

/**
 * Draw a yellow outline around an object's bounding.
 */
void draw_bounding_box(PhysicsBody* pb);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin(float size);

/**
 * Apply a sine function to an RBG color based on time and light source index.
 */
ColorRGB sine_animate_color(float time, int index);

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
 * Subtract 2 three dimensional vectors.
 */
Vec3 vec3_substract(Vec3 a, Vec3 b);

/**
 * Scale a three dimensional vector.
 */
Vec3 vec3_scale(Vec3 v, float scale);

/**
 * Get the length of a three dimensional vector.
 */
float vec3_length(Vec3 v);

/**
 * Get the dot product of 2 three dimensional vectors.
 */
float vec3_dot(Vec3 a, Vec3 b);

/**
 * Normalize a three dimensional vector.
 */
void vec3_normalize(Vec3* v);

/**
 * Calculate radian from degree.
 */
double degree_to_radian(double degree);

#endif /* UTILS_H */
