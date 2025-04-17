#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"

#include <obj/model.h>

typedef struct Scene
{
    Model cube;
    Model cat;
    Material material;
    GLuint texture_id;
} Scene;

/**
 * Macro for parameterizing transformations
 */
#define VEC3(x, y, z) (float[3]){x, y, z}

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting();

/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Apply transformations to the model.
 */
void transform_model(Model* model, float translate[3], float rotate[3], float scale[3]);

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

#endif /* SCENE_H */
