#ifndef OBJECT_H
#define OBJECT_H

#include "config.h" 
#include "model.h"  
#include "physics.h"
#include "utils.h"  
#include "camera.h" 
#include <GL/gl.h>  
#include <stdbool.h>
#include <obj/load.h>

typedef struct Scene Scene;

/**
 * An object of the scene.
 */
typedef struct Object {
    int id;
    char name[64];
    bool is_active;
    bool is_static;
    bool is_interacted;
    bool in_extraction;
    int value;
    Model model;
    Vec3 position;
    Vec3 rotation;
    Material material;
    PhysicsBody physics_body;
    GLuint texture_id;
    GLuint display_list;
} Object;

/**
 * Add an object to the scene.
 */
void add_object(Scene* scene, ObjectConfig* config);

/**
 * Prepare the model of an object.
 */
void load_and_prepare_model(Object* obj, ObjectConfig* config);

/**
 * Sync the physics transformations for rendering.
 */
void sync_physics_transforms(Scene* scene);

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

#endif /* OBJECT_H */