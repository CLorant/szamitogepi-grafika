#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"
#include "utils.h"
#include "physics.h"
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
    Vec3 position;
    Vec3 rotation;
    Material* material;
    PhysicsBody physics_body;
    GLuint texture_id; // maybe the scene struct should have this field?
    GLuint display_list;
} Object;

/**
 * Scene containing a fallback material, light sources and objects.
 */
typedef struct Scene {
    Material material;
    Lighting* lights;
    int light_count;
    Object* objects;
    int object_count;
    int selected_object_id;
    PhysicsWorld* physics_world;
} Scene;

/**
 * Initialize the scene by setting material, lights, objects.
 */
void init_scene(Scene* scene);

/**
 * Add a new light source to the scene.
 */
void add_light(Scene* scene, Lighting* config);

/**
 * Add a new object to the scene
 */
void add_object(Scene* scene, ObjectConfig* config);

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
 * Synchronize physics and rendering
 */
void sync_physics_transforms(Scene* scene);

/**
 * Position objects on the ground initially
 */
void place_objects_on_ground(Scene* scene);

/**
 * Check if mouse coordinates intersect with an object
 * Returns object ID or -1 if no object was hit
 */
int select_object_at(Scene* scene, Camera* camera, int mouse_x, int mouse_y);

/**
 * Move the currently selected object
 */
void move_selected_object(Scene* scene, Vec3 delta);

/**
 * Rotate the currently selected object
 */
void rotate_selected_object(Scene* scene, float rx, float ry);

/**
 * Reset rotation of selected object
 */
void reset_selected_object_rotation(Scene* scene);

/**
 * Apply physics forces to objects
 */
void apply_physics_forces(Scene* scene, float elapsed_time);

/**
 * Free all resources used by the scene
 */
void free_scene(Scene* scene);

#endif /* SCENE_H */