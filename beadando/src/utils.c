#include "scene.h"
#include "utils.h"
#include <math.h>
#include <string.h>
#include <GL/gl.h>

void scale_model(Model* model, Vec3 scale) {
    for (int i = 1; i <= model->n_vertices; ++i) {
        model->vertices[i].x *= scale.x;
        model->vertices[i].y *= scale.y;
        model->vertices[i].z *= scale.z;
    }
}

void rotate_model(Model* model, Vec3 rotation) {
    double rx = degree_to_radian(rotation.x);
    double ry = degree_to_radian(rotation.y);
    double rz = degree_to_radian(rotation.z);
    double cx = cos(rx), sx = sin(rx);
    double cy = cos(ry), sy = sin(ry);
    double cz = cos(rz), sz = sin(rz);

    for (int i = 1; i <= model->n_vertices; ++i) {
        Vertex* v = &model->vertices[i];
        double x = v->x, y = v->y, z = v->z;
    
        double y_rotx = y * cx - z * sx;
        double z_rotx = y * sx + z * cx;
        y = y_rotx;
        z = z_rotx;

        double x_roty = x * cy + z * sy;
        double z_roty = -x * sy + z * cy;
        x = x_roty;
        z = z_roty;

        double x_rotz = x * cz - y * sz;
        double y_rotz = x * sz + y * cz;
        x = x_rotz;
        y = y_rotz;

        v->x = x;
        v->y = y;
        v->z = z;
    }
}

void calculate_mesh_aabb(Model* model, Vec3* out_min, Vec3* out_max) {
    if (!model || model->n_vertices == 0) {
        *out_min = *out_max = (Vec3){0, 0, 0};
        return;
    }

    Vertex v0 = model->vertices[0];
    out_min->x = out_max->x = v0.x;
    out_min->y = out_max->y = v0.y;
    out_min->z = out_max->z = v0.z;

    for (int i = 1; i <= model->n_vertices; ++i) {
        Vertex v = model->vertices[i];
        if (v.x < out_min->x) out_min->x = v.x;
        if (v.y < out_min->y) out_min->y = v.y;
        if (v.z < out_min->z) out_min->z = v.z;

        if (v.x > out_max->x) out_max->x = v.x;
        if (v.y > out_max->y) out_max->y = v.y;
        if (v.z > out_max->z) out_max->z = v.z;
    }
}

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

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(size, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, size, 0);

    glColor3f(0, 0, 1);
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

Vec3 vec3_multiply(Vec3 a, Vec3 b) {
    return (Vec3) { a.x * b.x, a.y * b.y, a.z * b.z };
}

Vec3 vec3_add(Vec3 a, Vec3 b) {
    return (Vec3) { a.x + b.x, a.y + b.y, a.z + b.z };
}

Vec3 vec3_substract(Vec3 a, Vec3 b) {
    return (Vec3) { a.x - b.x, a.y - b.y, a.z - b.z };
}

Vec3 vec3_scale(Vec3 v, float scale) {
    return (Vec3) { v.x * scale, v.y * scale, v.z * scale };
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

