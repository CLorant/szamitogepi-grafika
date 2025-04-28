#ifndef PHYSICS_H
#define PHYSICS_H

#include <ode/ode.h>
#include "utils.h" 

// Maximum contacts per collision
#define MAX_CONTACTS 8

/**
 * Global physics for the application.
 */
typedef struct PhysicsWorld {
    dWorldID world;
    dSpaceID space;
    dJointGroupID contact_group;
    dReal gravity[3];
    dReal erp;
    dReal cfm;
    dReal surface_friction;
    dReal bounce;
    int quick_step_iterations;
} PhysicsWorld;

/**
 * Rigid body with collision geometry.
 */
typedef struct PhysicsBody {
    dBodyID body;
    dGeomID geom;
    void* user_data;
    bool is_active;
    bool is_sleeping;
    Vec3 dimensions;
} PhysicsBody;

/**
 * Bounding specification (for later).
 */
typedef enum PhysicsShapeType {
    PHYSICS_BOX,
    PHYSICS_SPHERE,
    PHYSICS_CAPSULE
} PhysicsShapeType;

/**
 * Physics simulation and contact parameters
 */
typedef struct PhysicsConfig {
    double erp;
    double cfm;
    double linear_damping;
    double angular_damping;
    double surface_friction;
    double bounce;
    double auto_disable_linear_threshold;
    double auto_disable_angular_threshold;
    int auto_disable_steps;
    int quick_step_iterations;
} PhysicsConfig;

/**
 * Initialize global physics.
 */
PhysicsWorld* init_physics(const dReal gravity[3]);

/**
 * Clean up physics.
 */
void physics_destroy(PhysicsWorld* pw);

/**
 * Retrieve and apply configurations.
 */
PhysicsConfig physics_default_config();
void physics_apply_config(PhysicsWorld* pw, PhysicsConfig* cfg);

/**
 * Add a dynamic or static box.
 */
PhysicsBody physics_create_box(PhysicsWorld* pw, double mass, Vec3 pos, Vec3 half_extents);

/**
 * Add a dynamic or static sphere.
 */
PhysicsBody physics_create_sphere(PhysicsWorld* pw, double mass, Vec3 pos, double radius);

/**
 * Add an infinite static ground plane.
 */
void physics_create_ground_plane(PhysicsWorld* pw);

/**
 * Advance the physics simulation by 'elapsed_time' seconds.
 */
void physics_simulate(PhysicsWorld* pw, double elapsed_time);

/**
 * Get the 9 corner coordinates of an OBB.
 */
bool physics_get_obb_corners(PhysicsBody* pb, Vec3 out_corners[8]);

/**
 * Check if the camera ray intersects with an OBB.
 */
bool ray_intersect_obb(Vec3 ray_o, Vec3 ray_d, PhysicsBody* pb, float* tmin_out);

/**
 * Draw a yellow outline around an object's bounding.
 */
void physics_draw_obb(PhysicsBody* pb);

/**
 * Get the Axis Aligned Bounding Box an object.
 */
void physics_get_aabb(PhysicsBody* pb, Vec3* min, Vec3* max);

/**
 * Simplified physics API.
 */
void physics_get_position(PhysicsBody* pb, Vec3* position);
void physics_get_rotation(PhysicsBody* pb, Vec3* rotation);
void physics_set_position(PhysicsBody* pb, Vec3 position);
void physics_set_rotation(PhysicsBody* pb, Vec3 rotation);

void physics_apply_force(PhysicsBody* pb, Vec3 force);
void physics_apply_torque(PhysicsBody* pb, Vec3 torque);
void physics_set_linear_velocity(PhysicsBody* pb, Vec3 velocity);
void physics_set_angular_velocity(PhysicsBody* pb, Vec3 velocity);

void physics_wake_up(PhysicsBody* pb);
void physics_disable_rotation(PhysicsBody* pb);

/**
 * Print the position and orientation of a body for debugging.
 */
void physics_log_body_state(PhysicsBody* pb);

#endif /* PHYSICS_H */
