#ifndef SCENE_H
#define SCENE_H

#include "config.h"
#include "camera.h"
#include "texture.h"
#include "utils.h"
#include "physics.h"
#include <obj/model.h>
#include <limits.h>

/**
 * An object of the scene.
 */
typedef struct Object {
    int id;
    char name[64];
    bool is_active;
    bool is_static;
    bool is_interacted;
    int value;
    Model model;
    Vec3 position;
    Vec3 rotation;
    Material* material;
    PhysicsBody physics_body;
    GLuint texture_id;
    GLuint display_list;
} Object;

/**
 * Place the rooms in world-space.
 */
typedef struct RoomPlacement {
    int idx;
    bool placed;
    int gx;
    int gy;
} RoomPlacement;

/**
 * A room of the scene.
 */
typedef struct Room {
    int id;
    char name[64];
    Vec3 dimension;
    Vec3 position;
    GLuint floor_tex;
    GLuint ceiling_tex;
    GLuint wall_tex;
    bool opening[DIR_COUNT];
    float wall_thickness;
    float door_width;
    float door_height;
    GLuint display_list;
} Room;

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
} Scene;

/**
 * Initialize the scene by setting material, lights, rooms and objects.
 */
void init_scene(Scene* scene);

/**
 * Add a new light source to the scene.
 */
void add_light(Scene* scene, Lighting* config);

/**
 * Add a new room to the scene.
 */
void add_room(Scene* scene, RoomConfig* config);

/**
 * Add a new object to the scene.
 */
void add_object(Scene* scene, ObjectConfig* config);

/**
 * Use BFS to place the rooms in world-space by connection.
 */
void place_rooms_by_connections(Scene* scene, RoomConfig* configs, int config_count);

/**
 * Initialize the object
 */
void place_objects_on_ground(Scene* scene);

/**
 * Sync the physics transformations for rendering.
 */
void sync_physics_transforms(Scene* scene);

/**
 * Find a room by name.
 */
Room* find_room_by_name(Scene* scene, const char* name);

/**
 * Find an object by name.
 */
Object* find_object_by_name(Scene* scene, const char* name);

/**
 * Find object by ID.
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
int select_object_at(Scene* scene, Camera* camera, int mx, int my, float* out_distance);

/**
 * Move the currently selected object.
 */
void move_selected_object(Scene* scene, Vec3 delta);

/**
 * Rotate the currently selected object.
 */
void rotate_selected_object(Scene* scene, float rx, float ry);

/**
 * Free all resources used by the scene
 */
void free_scene(Scene* scene);

#endif /* SCENE_H */