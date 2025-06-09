#include "camera.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>

void init_camera(Camera* camera) {
    camera->position = (Vec3){0.0, 0.0, 1.25};
    camera->rotation = (Vec3){0.0, 0.0, 90.0};
    camera->speed = (Vec3){0.0, 0.0, 0.0};
    camera->rotation_speed = (Vec3){0.0, 0.0, 0.0};
    camera->viewport = (Viewport){0, 0, 0, 0};
    camera->fov = 60.0;
    camera->is_orbital = false;
    camera->orbital_radius = 3.0;
    camera->half_extents = (Vec3){ 0.25f, 0.25f, camera->position.z * 0.5 };
    camera->physics_body.body = NULL;
    camera->physics_body.geom = NULL;
}

void init_camera_physics(PhysicsWorld* pw, Camera* camera) {
    physics_create_box(pw, &camera->physics_body, 1.0, camera->position, camera->half_extents);
    camera->physics_body.user_data = camera;
    dBodySetGravityMode(camera->physics_body.body, 0);
    dBodySetFiniteRotationMode(camera->physics_body.body, 1);
    dBodySetLinearDamping(camera->physics_body.body, 0.1);
    dBodySetAngularDamping(camera->physics_body.body, 0.9);
    dBodySetAutoDisableFlag(camera->physics_body.body, 0);
}

void update_camera(Camera* camera, double time) {
    if (!camera->is_orbital) {
        update_camera_basis(camera);

        Vec3 horizontal_forward = {
            camera->basis.forward.x,
            camera->basis.forward.y,
            0.0f
        };
        vec3_normalize(&horizontal_forward);

        Vec3 v_f = vec3_scale(horizontal_forward, camera->speed.y);
        Vec3 v_s = vec3_scale(camera->basis.right, camera->speed.x);
        Vec3 world_vel = vec3_add(v_f, v_s);

        dBodySetLinearVel(camera->physics_body.body,
                          world_vel.x,
                          world_vel.y,
                          0.0);

        const dReal* p = dBodyGetPosition(camera->physics_body.body);
        camera->position.x = p[0];
        camera->position.y = p[1];

        camera->position.z += camera->speed.z * time;
        dBodySetPosition(camera->physics_body.body,
                         camera->position.x,
                         camera->position.y,
                         camera->position.z);
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

        float x = camera->orbital_radius * cos(azimuth) * cos(elevation) + camera->position.x;
        float y = camera->orbital_radius * sin(azimuth) * cos(elevation) + camera->position.y;
        float z = camera->orbital_radius * sin(elevation) + camera->position.z;

        gluLookAt(x, y, z, camera->position.x, camera->position.y, camera->position.z, 0, 0, 1);
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

void update_camera_basis(Camera* camera) {
    float h = degree_to_radian(camera->rotation.z);
    float v = degree_to_radian(camera->rotation.x);

    camera->basis.forward = (Vec3){cosf(h) * cosf(v), sinf(h) * cosf(v), sinf(v)};
    vec3_normalize(&camera->basis.forward);

    camera->basis.right = (Vec3){-sinf(h), cosf(h), 0};
    vec3_normalize(&camera->basis.right);

    camera->basis.up = (Vec3){-cosf(h) * sinf(v), -sinf(h) * sinf(v), cosf(v)};
    vec3_normalize(&camera->basis.up);
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