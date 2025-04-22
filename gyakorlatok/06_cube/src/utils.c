#include "utils.h"
#include <math.h>
#include <string.h>
#include <GL/gl.h>

void set_rgb(ColorRGB* dest, float red, float green, float blue) {
    dest->red = red;
    dest->green = green;
    dest->blue = blue;
}

void set_rgba(ColorRGBA* dest, float red, float green, float blue, float alpha) {
    dest->red = red;
    dest->green = green;
    dest->blue = blue;
    dest->alpha = alpha;
}

ColorRGB sine_animate_color(float time, int index, float brightness) {
    float r = (sinf(time * 0.5f + index * 2.0f) + 1.0f) * 0.5f;
    float g = (sinf(time * 0.8f + index * 3.0f) + 1.0f) * 0.5f;
    float b = (sinf(time * 1.2f + index * 4.0f) + 1.0f) * 0.5f;

    r *= brightness;
    g *= brightness;
    b *= brightness;

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

Vec3 vec3_multiply(Vec3 v, Vec3 scale) {
    return (Vec3) { v.x* scale.x, v.y* scale.y, v.z* scale.z };
}

Vec3 vec3_add(Vec3 a, Vec3 b) {
    return (Vec3) { a.x + b.x, a.y + b.y, a.z + b.z };
}

float vec3_dot(Vec3* a, Vec3* b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

void vec3_normalize(Vec3* v) {
    float len = sqrtf(vec3_dot(v, v));
    if (len > EPSILON) {
        v->x /= len;
        v->y /= len;
        v->z /= len;
    }
}

double degree_to_radian(double degree) {
    return degree * M_PI / 180.0;
}

void mat_identity(int size, float m[size][size]) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

void mat_multiply(int s, float result[s][s], float a[s][s], float b[s][s]) {
    int i, j, k;
    for (i = 0; i < s; ++i) {
        for (j = 0; j < s; ++j) {
            result[i][j] = 0.0f;
            for (k = 0; k < 4; ++k) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void mat4_mul_inplace(Mat4 m, Mat4 op) {
    Mat4 tmp;
    mat_multiply(4, tmp, m, op);
    memcpy(m, tmp, sizeof(Mat4));
}

void mat4_translate(Mat4 m, float x, float y, float z) {
    Mat4 translate = {
        {1, 0, 0, x},
        {0, 1, 0, y},
        {0, 0, 1, z},
        {0, 0, 0, 1}
    };
    mat4_mul_inplace(m, translate);
}

void mat4_scale(Mat4 m, float x, float y, float z) {
    Mat4 scale = {
        {x, 0, 0, 0},
        {0, y, 0, 0},
        {0, 0, z, 0},
        {0, 0, 0, 1}
    };
    mat4_mul_inplace(m, scale);
}

void mat4_rotate(Mat4 m, float angle_degrees, float x, float y, float z) {
    Mat3 rot;
    calculate_rotation_matrix(rot, angle_degrees, x, y, z);

    Mat4 temp;
    mat_identity(4, temp);

    memcpy(&temp[0][0], &rot[0][0], sizeof(float) * 3);
    memcpy(&temp[1][0], &rot[1][0], sizeof(float) * 3);
    memcpy(&temp[2][0], &rot[2][0], sizeof(float) * 3);

    mat4_mul_inplace(m, temp);
}

void calculate_rotation_matrix(Mat3 result, float angle_degrees, float x, float y, float z) {
    float angle = degree_to_radian(angle_degrees);
    float c = cosf(angle);
    float s = sinf(angle);
    float len = sqrtf(x * x + y * y + z * z);

    if (len < EPSILON) {
        mat_identity(3, result);
        return;
    }

    x /= len;
    y /= len;
    z /= len;

    result[0][0] = c + (1 - c) * x * x;
    result[0][1] = (1 - c) * x * y - s * z;
    result[0][2] = (1 - c) * x * z + s * y;

    result[1][0] = (1 - c) * y * x + s * z;
    result[1][1] = c + (1 - c) * y * y;
    result[1][2] = (1 - c) * y * z - s * x;

    result[2][0] = (1 - c) * z * x - s * y;
    result[2][1] = (1 - c) * z * y + s * x;
    result[2][2] = c + (1 - c) * z * z;
}

void update_transform_matrix(Transform* t) {
    if (!t->matrix_dirty) {
        return;
    }

    mat_identity(4, t->matrix);

    if (t->position.x != 0.0f || t->position.y != 0.0f || t->position.z != 0.0f) {
        Mat4 translate_matrix;
        mat_identity(4, translate_matrix);
        translate_matrix[0][3] = t->position.x;
        translate_matrix[1][3] = t->position.y;
        translate_matrix[2][3] = t->position.z;

        Mat4 temp;
        mat_multiply(4, temp, translate_matrix, t->matrix);
        memcpy(t->matrix, temp, sizeof(Mat4));
    }

    if (t->rotation.z != 0.0f) {
        mat4_rotate(t->matrix, t->rotation.z, 0.0f, 0.0f, 1.0f);
    }
    if (t->rotation.y != 0.0f) {
        mat4_rotate(t->matrix, t->rotation.y, 0.0f, 1.0f, 0.0f);
    }
    if (t->rotation.x != 0.0f) {
        mat4_rotate(t->matrix, t->rotation.x, 1.0f, 0.0f, 0.0f);
    }

    if (t->scale.x != 1.0f || t->scale.y != 1.0f || t->scale.z != 1.0f) {
        Mat4 scale_matrix;
        mat_identity(4, scale_matrix);
        scale_matrix[0][0] = t->scale.x;
        scale_matrix[1][1] = t->scale.y;
        scale_matrix[2][2] = t->scale.z;

        Mat4 temp;
        mat_multiply(4, temp, t->matrix, scale_matrix);
        memcpy(t->matrix, temp, sizeof(Mat4));
    }

    t->matrix_dirty = false;
}

void transform_init(Transform* transform) {
    transform->position = (Vec3){ 0.0f, 0.0f, 0.0f };
    transform->rotation = (Vec3){ 0.0f, 0.0f, 0.0f };
    transform->scale = (Vec3){ 1.0f, 1.0f, 1.0f };

    transform->position_changed = false;
    transform->rotation_changed = false;
    transform->scale_changed = false;
    transform->matrix_dirty = true;

    mat_identity(4, transform->matrix);
}

void mark_dirty(Transform* t, bool pos, bool rot, bool scl) {
    if (pos) t->position_changed = true;
    if (rot) t->rotation_changed = true;
    if (scl) t->scale_changed = true;
    t->matrix_dirty = true;
}

void set_position(Transform* t, float x, float y, float z) {
    t->position = (Vec3){ x, y, z };
    mark_dirty(t, true, false, false);
}

void set_rotation(Transform* t, float x, float y, float z) {
    t->rotation = (Vec3){ x, y, z };
    mark_dirty(t, false, true, false);
}

void set_scale(Transform* t, float x, float y, float z) {
    t->scale = (Vec3){ x, y, z };
    mark_dirty(t, false, false, true);
}

void reset_position(Transform* t) {
    t->position = (Vec3){ 0.0f, 0.0f, 0.0f };
    mark_dirty(t, true, false, false);
}

void reset_rotation(Transform* t) {
    t->rotation = (Vec3){ 0.0f, 0.0f, 0.0f };
    mark_dirty(t, false, true, false);
}

void reset_scale(Transform* t) {
    t->scale = (Vec3){ 1.0f, 1.0f, 1.0f };
    mark_dirty(t, false, false, true);
}

void reset_transform(Transform* t) {
    reset_position(t);
    reset_rotation(t);
    reset_scale(t);
}