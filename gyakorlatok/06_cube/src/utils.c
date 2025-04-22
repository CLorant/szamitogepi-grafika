#include "scene.h"
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

void calculate_bounding_box(Object* obj) {
    if (obj->model.n_vertices == 0) return;

    Vertex init_vertex = obj->model.vertices[0];
    obj->bounding.min = obj->bounding.max = (Vec3){
        init_vertex.x,
        init_vertex.y,
        init_vertex.z,
    };

    for (int i = 1; i < obj->model.n_vertices; ++i) {
        Vertex current_vertex = obj->model.vertices[i];
        Vec3 v = (Vec3){
            current_vertex.x, current_vertex.y, current_vertex.z
        };
        obj->bounding.min = vec3_min(obj->bounding.min, v);
        obj->bounding.max = vec3_max(obj->bounding.max, v);
    }
}

void set_lighting(int slot, const Lighting* light) {
    GLenum light_enum = GL_LIGHT0 + slot;

    float ambient_light[] = {
        light->ambient.red,
        light->ambient.green,
        light->ambient.blue,
        light->ambient.alpha
    };

    float diffuse_light[] = {
        light->diffuse.red,
        light->diffuse.green,
        light->diffuse.blue,
        light->ambient.alpha
    };

    float specular_light[] = {
        light->specular.red,
        light->specular.green,
        light->specular.blue,
        light->ambient.alpha
    };

    float light_position[] = {
        light->position.x,
        light->position.y,
        light->position.z,
        light->position.w
    };

    glLightfv(light_enum, GL_AMBIENT, ambient_light);
    glLightfv(light_enum, GL_DIFFUSE, diffuse_light);
    glLightfv(light_enum, GL_SPECULAR, specular_light);
    glLightfv(light_enum, GL_POSITION, light_position);
}

void set_material(const Material* material) {
    float ambient_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void draw_origin(float size) {
    glBegin(GL_LINES);

    glColor3f(size, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(size, 0, 0);

    glColor3f(0, size, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, size, 0);

    glColor3f(0, 0, size);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, size);

    glEnd();
}

void draw_checkerboard(int size, float square_size) {
    glBegin(GL_QUADS);

    for (int i = -size; i < size; i++) {
        for (int j = -size; j < size; j++) {
            if ((i + j) % 2 == 0) {
                glColor3f(0.9, 0.9, 0.9);
            }
            else {
                glColor3f(0.1, 0.1, 0.1);
            }

            float x = i * square_size;
            float y = j * square_size;

            glVertex3f(x, y, 0);
            glVertex3f(x + square_size, y, 0);
            glVertex3f(x + square_size, y + square_size, 0);
            glVertex3f(x, y + square_size, 0);
        }
    }

    glEnd();
}

void draw_bounding_box(Object* obj) {
    if (!obj->is_active || obj->is_static || obj->model.n_vertices == 0) {
        return;
    }

    BoundingBox box = obj->bounding;
    float min_x = box.min.x, min_y = box.min.y, min_z = box.min.z;
    float max_x = box.max.x, max_y = box.max.y, max_z = box.max.z;

    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.0f);
    glLineWidth(2.0f);

    glBegin(GL_LINE_LOOP);
    glVertex3f(min_x, min_y, min_z);
    glVertex3f(max_x, min_y, min_z);
    glVertex3f(max_x, max_y, min_z);
    glVertex3f(min_x, max_y, min_z);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f(min_x, min_y, max_z);
    glVertex3f(max_x, min_y, max_z);
    glVertex3f(max_x, max_y, max_z);
    glVertex3f(min_x, max_y, max_z);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(min_x, min_y, min_z);
    glVertex3f(min_x, min_y, max_z);
    glVertex3f(max_x, min_y, min_z);
    glVertex3f(max_x, min_y, max_z);
    glVertex3f(max_x, max_y, min_z);
    glVertex3f(max_x, max_y, max_z);
    glVertex3f(min_x, max_y, min_z);
    glVertex3f(min_x, max_y, max_z);
    glEnd();

    glEnable(GL_LIGHTING);
    glLineWidth(1.0f);
}

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

void transform_init(Transform* transform) {
    transform->position = (Vec3){0.0f, 0.0f, 0.0f};
    transform->rotation = (Vec3){0.0f, 0.0f, 0.0f};
    transform->scale = (Vec3){1.0f, 1.0f, 1.0f};
}

void set_position(Transform* t, float x, float y, float z) {
    t->position = (Vec3){x, y, z};
}

void set_rotation(Transform* t, float x, float y, float z) {
    t->rotation = (Vec3){x, y, z};
}

void set_scale(Transform* t, float x, float y, float z) {
    t->scale = (Vec3){x, y, z};
}

void reset_position(Transform* t) {
    t->position = (Vec3){ 0.0f, 0.0f, 0.0f };
}

void reset_rotation(Transform* t) {
    t->rotation = (Vec3){ 0.0f, 0.0f, 0.0f };
}

void reset_scale(Transform* t) {
    t->scale = (Vec3){ 1.0f, 1.0f, 1.0f };
}

void reset_transform(Transform* t) {
    reset_position(t);
    reset_rotation(t);
    reset_scale(t);
}