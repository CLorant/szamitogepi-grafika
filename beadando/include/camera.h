#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"
#include <stdbool.h>
#include <physics.h>

/**
 * Orientation of the camera.
 */
typedef struct Basis {
    Vec3 forward;
    Vec3 right;
    Vec3 up;
} Basis;

/**
 * The viewport of the Camera, where x, y are the top-left coordinates.
 */
typedef struct Viewport {
    int x;
    int y;
    int width;
    int height;
} Viewport;

/**
 * Camera, as a moving point with direction.
 */
typedef struct Camera {
    Vec3 position;
    Vec3 rotation;
    Vec3 speed;
    Vec3 rotation_speed;
    Basis basis;
    Viewport viewport;
    float fov;
    float aspect_ratio;
    float orbital_radius;
    bool is_orbital;
    PhysicsBody physics_body;
    Vec3 half_extents;
} Camera;

/**
 * Initialize the camera to the start position.
 */
void init_camera(Camera* camera);

/**
 * Initialize the camera for physics.
 */
void init_camera_physics(PhysicsWorld* pw, Camera* cam);

/**
 * Update the position of the camera.
 */
void update_camera(Camera* camera, double time);

/**
 * Apply the camera settings to the view transformation.
 */
void set_view(const Camera* camera);

/**
 * Set the horizontal and vertical rotation of the view angle.
 */
void rotate_camera(Camera* camera, double horizontal, double vertical);

/**
 * Update the forward / right / up basis of the camera.
 */
void update_camera_basis(Camera* camera);

/**
 * Set the speed of forward and backward motion.
 */
void set_camera_speed(Camera* camera, double speed);

/**
 * Set the speed of left and right side steps.
 */
void set_camera_side_speed(Camera* camera, double speed);

/**
 * Set the speed of vertical motion.
 */
void set_camera_vertical_speed(Camera* camera, double speed);

/**
 * Adjust the Field of View of the camera
 */
void adjust_fov(Camera* camera, float amount);

#endif /* CAMERA_H */
