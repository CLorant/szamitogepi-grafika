#ifndef SCENE_H
#define SCENE_H

#include "config.h"
#include "camera.h"
#include "texture.h"
#include "utils.h"
#include "physics.h"
#include "model.h"
#include <obj/model.h>
#include "room.h"
#include "lighting.h"
#include "object.h"
#include "extraction.h"
#include <limits.h>

/**
 * Scene containing light sources, rooms and objects.
 */
typedef struct Scene {
    Material material;
    Lighting* lights;
    int light_count;
    Room* rooms;
    int room_count;
    Object* objects;
    int object_count;
    int selected_object_id;
    PhysicsWorld physics_world;
    Extraction extraction;
} Scene;

/**
 * Initialize the scene by setting material, lights, rooms and objects.
 */
void init_scene(Scene* scene);

/**
 * Find a room by name.
 */
Room* find_room_by_name(Scene* scene, const char* name);

/**
 * Find a light source by name.
 */
Lighting* find_light_by_name(Scene* scene, const char* name);

/**
 * Find an object by name.
 */
Object* find_object_by_name(Scene* scene, const char* name);

/**
 * Find object by ID.
 */
Object* find_object_by_id(Scene* scene, int id);

/**
 * 
 */
void create_static_physics_and_display(Object* obj, Scene* scene);

/**
 * 
 */
void create_dynamic_physics_and_display(Object* obj, Scene* scene, float mass);

/**
 * Update the scene.
 */
void update_scene(Scene* scene, double elapsed_time);

/**
 * Render the scene objects.
 */
void render_scene(const Scene* scene);

/**
 * Free all resources used by the scene
 */
void free_scene(Scene* scene);

#endif /* SCENE_H */