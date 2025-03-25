#include "camera.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>

void init_camera(Camera* camera)
{
    camera->position.x = 0.9;
    camera->position.y = 1.3;
    camera->position.z = 1.0;
    camera->rotation.x = 336.0;
    camera->rotation.y = 0.0;
    camera->rotation.z = 231.0;
    camera->speed.x = 0.0;
    camera->speed.y = 0.0;
    camera->speed.z = 0.0;
    camera->rotation_speed.x = 0.0;
    camera->rotation_speed.y = 0.0;
    camera->rotation_speed.z = 1.0;
}

void update_camera(Camera* camera, double time)
{
    double angle;
    double side_angle;

    angle = degree_to_radian(camera->rotation.z);
    side_angle = degree_to_radian(camera->rotation.z + 90.0);

    camera->position.x += cos(angle) * camera->speed.y * time;
    camera->position.y += sin(angle) * camera->speed.y * time;
    camera->position.x += cos(side_angle) * camera->speed.x * time;
    camera->position.y += sin(side_angle) * camera->speed.x * time;

    camera->position.z += camera->speed.z * time;
    camera->rotation.y += camera->rotation_speed.y * time;

    if (camera->rotation.y < 0) {
        camera->rotation.y += 360.0;
    }

    if (camera->rotation.y > 360.0) {
        camera->rotation.y -= 360.0;
    }
}

void set_view(const Camera* camera)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-(camera->rotation.x + 90), 1.0, 0, 0);
    glRotatef(-(camera->rotation.z - 90), 0, 0, 1.0);
    glRotatef(-(camera->rotation.y), 0, 1.0, 0);
    glTranslatef(-camera->position.x, -camera->position.y, -camera->position.z);
    /*
    const float radius = 3;
    float azimuth = degree_to_radian(camera->rotation.z);
    float elevation = degree_to_radian(camera->rotation.x);

    float x = radius * cos(azimuth) * cos(elevation);
    float y = radius * sin(azimuth) * cos(elevation);
    float z = radius * sin(elevation);

    gluLookAt(x, y, z, 0, 0, 0, 0, 0, 1);
    */
}

void rotate_camera(Camera* camera, double horizontal, double vertical)
{
    camera->rotation.z += horizontal;
    camera->rotation.x += vertical;

    if (camera->rotation.z < 0) {
        camera->rotation.z += 360.0;
    }

    if (camera->rotation.z > 360.0) {
        camera->rotation.z -= 360.0;
    }

    if (camera->rotation.x < 0) {
        camera->rotation.x += 360.0;
    }

    if (camera->rotation.x > 360.0) {
        camera->rotation.x -= 360.0;
    }
    /*
    const float sensitivity = 0.5f;
    camera->rotation.z += horizontal * sensitivity;
    camera->rotation.x += vertical * sensitivity;

    if (camera->rotation.x > 89.0f) camera->rotation.x = 89.0f;
    if (camera->rotation.x < -89.0f) camera->rotation.x = -89.0f;

    if (camera->rotation.z < 0) camera->rotation.z += 360.0f;
    if (camera->rotation.z >= 360.0f) camera->rotation.z -= 360.0f;
    */
}

void set_camera_speed(Camera* camera, double speed)
{
    camera->speed.y = speed;
}

void set_camera_side_speed(Camera* camera, double speed)
{
    camera->speed.x = speed;
}

void set_camera_vertical_speed(Camera* camera, double speed)
{
    camera->speed.z = speed;
}