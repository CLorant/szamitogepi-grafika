#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"
#include "utils.h"
#include <obj/model.h>

/**
 * An object of the scene.
 */
typedef struct Object {
    int id;
    char name[64];
    bool is_active;
    bool is_static;
    Model model;
    Transform static_transform;
    Transform anim_transform;
    Material* material;
    BoundingBox bounding;
    GLuint texture_id;
    GLuint display_list;
} Object;

/**
 * Scene containing a fallback material, light sources and objects.
 */
typedef struct Scene {
    Material material;
    Lighting* lights;
    int light_count;
    int max_lights;
    Object* objects;
    int object_count;
    int max_objects;
    int selected_object_id;
} Scene;

/**
 * Configuration for an Object.
 */
typedef struct ObjectConfig {
    char* name;
    char* model_path;
    char* texture_path;
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
    bool is_static;
} ObjectConfig;

/**
 * Initialize the scene by setting material, lights, objects.
 */
void init_scene(Scene* scene);

/**
 * Add a new light source to the scene.
 */
void add_light(Scene* scene, const Lighting* config);

/**
 * Add a new object to the scene
 */
void add_object(Scene* scene, const ObjectConfig* config);

/**
 * Find object by name
 * Returns NULL if not found
 */
Object* find_object_by_name(Scene* scene, const char* name);

/**
 * Find object by ID
 * Returns NULL if not found
 */
Object* find_object_by_id(Scene* scene, int id);

/**
 * Adjust the brightness by increasing or decreasing diffuse.
 */
void adjust_brightness(Scene* scene, float amount);

/**
 * Update the scene.
 */
void update_scene(Scene* scene, double elapsed_time);

/**
 * Render the scene objects.
 */
void render_scene(const Scene* scene);

/**
 * Check if mouse coordinates intersect with an object
 * Returns object ID or -1 if no object was hit
 */
int select_object_at(Scene* scene, Camera* camera, int mouse_x, int mouse_y);

/**
 * Rotate the currently selected object
 */
void rotate_selected_object(Scene* scene, float x_rotation, float y_rotation);

/**
 * Reset rotation of selected object
 */
void reset_selected_object_rotation(Scene* scene);

/**
 * Free all resources used by the scene
 */
void free_scene(Scene* scene);

#endif /* SCENE_H */