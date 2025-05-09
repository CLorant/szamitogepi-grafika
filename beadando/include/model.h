#ifndef MODEL_H
#define MODEL_H

#include "utils.h"
#include <obj/model.h>

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
 * Set the current material.
 */
void set_material(const Material* material);

#endif /* MODEL_H */