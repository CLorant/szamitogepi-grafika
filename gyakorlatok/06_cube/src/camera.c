#include "camera.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

void init_camera(Camera* camera) {
    camera->position = (Vec3){0.0, 0.0, 1.0};
    camera->rotation = (Vec3){0.0, 0.0, 0.0};
    camera->speed = (Vec3){0.0, 0.0, 0.0};
    camera->is_preview_visible = false;
    camera->is_orbital = false;
    camera->orbital_radius = 3.0;
    camera->fov = 50.0;
}

void update_camera(Camera* camera, double time) {
    if (!camera->is_orbital) {
        double angle = degree_to_radian(camera->rotation.z);
        double side_angle = degree_to_radian(camera->rotation.z + 90.0);
        
        camera->position.x += cos(angle) * camera->speed.y * time;
        camera->position.y += sin(angle) * camera->speed.y * time;
        camera->position.x += cos(side_angle) * camera->speed.x * time;
        camera->position.y += sin(side_angle) * camera->speed.x * time;

        camera->position.z += camera->speed.z * time;
    }

    camera->rotation.y += camera->rotation_speed.y * time;
    if (camera->rotation.y < 0) {
        camera->rotation.y += 360.0;
    }
    if (camera->rotation.y > 360.0) {
        camera->rotation.y -= 360.0;
    }
}

void set_view(const Camera* camera) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (camera->is_orbital) {
        float azimuth = degree_to_radian(camera->rotation.z);
        float elevation = degree_to_radian(-camera->rotation.x);

        float x = camera->orbital_radius * cos(azimuth) * cos(elevation);
        float y = camera->orbital_radius * sin(azimuth) * cos(elevation);
        float z = camera->orbital_radius * sin(elevation);

        gluLookAt(x, y, z, 0, 0, 0, 0, 0, 1);
    }
    else {
        glRotatef(-(camera->rotation.x + 90), 1.0, 0, 0);
        glRotatef(-(camera->rotation.z - 90), 0, 0, 1.0);
        glTranslatef(-camera->position.x, -camera->position.y, -camera->position.z);
    }
}

void rotate_camera(Camera* camera, double horizontal, double vertical) {
    camera->rotation.z += camera->is_orbital ? -horizontal :  horizontal;
    camera->rotation.x += camera->is_orbital ? -vertical :  vertical;

    if (camera->rotation.x > 90.0) {
        camera->rotation.x = 90.0;
    }
    else if (camera->rotation.x < -90.0) {
        camera->rotation.x = -90.0;
    }

    if (camera->rotation.z < 0) {
        camera->rotation.z += 360.0;
    }
    else if (camera->rotation.z > 360.0) {
        camera->rotation.z -= 360.0;
    }
}

void set_camera_speed(Camera* camera, double speed) {
    camera->speed.y = speed;
}

void set_camera_side_speed(Camera* camera, double speed) {
    camera->speed.x = speed;
}

void set_camera_vertical_speed(Camera* camera, double speed) {
    camera->speed.z = speed;
}

void adjust_fov(Camera* camera, float amount) {
    camera->fov = fminf(fmaxf(camera->fov + amount, 30.0f), 120.0f);
}

void show_texture_preview() {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-1, 1, -3);
    glTexCoord2f(1, 0);
    glVertex3f(1, 1, -3);
    glTexCoord2f(1, 1);
    glVertex3f(1, -1, -3);
    glTexCoord2f(0, 1);
    glVertex3f(-1, -1, -3);
    glEnd();

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}