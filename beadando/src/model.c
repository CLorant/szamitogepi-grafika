#include "model.h"
#include <GL/gl.h>
#include <math.h>

void vertex_normalize(Vertex* v) {
    double len = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
    if (len > 1e-6) {
        v->x /= len;
        v->y /= len;
        v->z /= len;
    }
}

void scale_model(Model* model, Vec3 scale) {
    for (int i = 1; i <= model->n_vertices; ++i) {
        model->vertices[i].x *= scale.x;
        model->vertices[i].y *= scale.y;
        model->vertices[i].z *= scale.z;
    }

    float inv_x = 1.0f / scale.x;
    float inv_y = 1.0f / scale.y;
    float inv_z = 1.0f / scale.z;

    for (int i = 1; i <= model->n_normals; ++i) {
        Vertex* n = &model->normals[i];
        n->x *= inv_x;
        n->y *= inv_y;
        n->z *= inv_z;
        vertex_normalize(n);
    }
}

void rotate_model(Model* model, Vec3 rotation) {
    double rx = degree_to_radian(rotation.x),
           ry = degree_to_radian(rotation.y),
           rz = degree_to_radian(rotation.z);
    double cx = cos(rx), sx = sin(rx);
    double cy = cos(ry), sy = sin(ry);
    double cz = cos(rz), sz = sin(rz);

    for (int i = 1; i <= model->n_vertices; ++i) {
        Vertex* v = &model->vertices[i];
        double x = v->x, y = v->y, z = v->z;
        double y1 = y * cx - z * sx, z1 = y * sx + z * cx;
        double x2 = x * cy + z1 * sy, z2 = -x * sy + z1 * cy;
        double x3 = x2 * cz - y1 * sz, y3 = x2 * sz + y1 * cz;
        v->x = x3; v->y = y3; v->z = z2;
    }

    for (int i = 1; i <= model->n_normals; ++i) {
        Vertex* n = &model->normals[i];
        double x = n->x, y = n->y, z = n->z;
        double y1 = y * cx - z * sx, z1 = y * sx + z * cx;
        double x2 = x * cy + z1 * sy, z2 = -x * sy + z1 * cy;
        double x3 = x2 * cz - y1 * sz, y3 = x2 * sz + y1 * cz;
        n->x = x3; n->y = y3; n->z = z2;
        vertex_normalize(n);
    }
}

void calculate_mesh_aabb(const Model* model, Vec3* out_min, Vec3* out_max) {
    if (!model || model->n_vertices == 0) {
        *out_min = *out_max = (Vec3){ 0, 0, 0 };
        return;
    }

    Vertex v0 = model->vertices[1];
    out_min->x = out_max->x = v0.x;
    out_min->y = out_max->y = v0.y;
    out_min->z = out_max->z = v0.z;

    for (int i = 2; i <= model->n_vertices; ++i) {
        Vertex v = model->vertices[i];
        if (!isfinite(v.x) || !isfinite(v.y) || !isfinite(v.z)) {
            continue;
        }
        
        if (v.x < out_min->x) out_min->x = v.x;
        if (v.y < out_min->y) out_min->y = v.y;
        if (v.z < out_min->z) out_min->z = v.z;

        if (v.x > out_max->x) out_max->x = v.x;
        if (v.y > out_max->y) out_max->y = v.y;
        if (v.z > out_max->z) out_max->z = v.z;
    }
}

Vec3 mesh_half_extents(const Model* model) {
    Vec3 min, max;
    calculate_mesh_aabb(model, &min, &max);
    return (Vec3){
        fmaxf((max.x - min.x)*0.5f, 0.01f),
        fmaxf((max.y - min.y)*0.5f, 0.01f),
        fmaxf((max.z - min.z)*0.5f, 0.01f)
    };
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