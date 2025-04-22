#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"
#include <stdbool.h>

/**
 * Camera, as a moving point with direction.
 */
typedef struct Camera {
    Vec3 position;
    Vec3 rotation;
    Vec3 speed;
    Vec3 rotation_speed;
    bool is_preview_visible;
    bool is_orbital;
    float orbital_radius;  
} Camera;

/**
 * Initialize the camera to the start position.
 */
void init_camera(Camera* camera);

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
 * Show the texture preview.
 */
void show_texture_preview();

#endif /* CAMERA_H */
