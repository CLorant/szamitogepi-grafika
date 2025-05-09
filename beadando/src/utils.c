#include "utils.h"
#include <math.h>
#include <string.h>
#include <GL/gl.h>

bool slab_hit(float origin, float dir, float min_box, float max_box, float* tmin, float* tmax) {
    float inv_dir = 1.0f / dir;
    float t0 = (min_box - origin) * inv_dir;
    float t1 = (max_box - origin) * inv_dir;
    if (inv_dir < 0.0f) {
        float tmp = t0;
        t0 = t1;
        t1 = tmp;
    }
    *tmin = fmaxf(t0, *tmin);
    *tmax = fminf(t1, *tmax);

    return *tmax > *tmin;
}

void get_delta(int dir, int* dx, int* dy) {
    switch (dir) {
    case DIR_NORTH: *dx = 0; *dy = 1; break;
    case DIR_EAST:  *dx = 1; *dy = 0; break;
    case DIR_SOUTH: *dx = 0; *dy = -1; break;
    case DIR_WEST:  *dx = -1; *dy = 0; break;
    default:        *dx = 0; *dy = 0; break;
    }
}

ColorRGB sine_animate_color(float time, int index) {
    float r = (sinf(time * 0.5f + index * 2.0f) + 1.0f) * 0.5f;
    float g = (sinf(time * 0.8f + index * 3.0f) + 1.0f) * 0.5f;
    float b = (sinf(time * 1.2f + index * 4.0f) + 1.0f) * 0.5f;

    ColorRGB out = {
        .red = fminf(r, 1.0f),
        .green = fminf(g, 1.0f),
        .blue = fminf(b, 1.0f)
    };

    return out;
}

Vec3 vec3_min(Vec3 a, Vec3 b) {
    return (Vec3) { fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z) };
}

Vec3 vec3_max(Vec3 a, Vec3 b) {
    return (Vec3) { fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z) };
}

Vec3 vec3_multiply(Vec3 a, Vec3 b) {
    return (Vec3) { a.x* b.x, a.y* b.y, a.z* b.z };
}

Vec3 vec3_add(Vec3 a, Vec3 b) {
    return (Vec3) { a.x + b.x, a.y + b.y, a.z + b.z };
}

Vec3 vec3_substract(Vec3 a, Vec3 b) {
    return (Vec3) { a.x - b.x, a.y - b.y, a.z - b.z };
}

Vec3 vec3_scale(Vec3 v, float scale) {
    return (Vec3) { v.x* scale, v.y* scale, v.z* scale };
}

float vec3_length(Vec3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float vec3_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

void vec3_normalize(Vec3* v) {
    float len = sqrtf(vec3_dot(*v, *v));
    if (len > EPSILON) {
        v->x /= len;
        v->y /= len;
        v->z /= len;
    }
}

double degree_to_radian(double degree) {
    return degree * M_PI / 180.0;
}
