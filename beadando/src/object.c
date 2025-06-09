#include "object.h"
#include "scene.h"
#include <string.h>
#include <stdio.h>
#include "physics.h"
#include "utils.h"

void add_object(Scene* scene, ObjectConfig* config) {
    Object* obj = &scene->objects[scene->object_count];
    memset(obj, 0, sizeof(Object));

    obj->id = scene->object_count;
    obj->is_active = true;
    obj->is_static = config->is_static;
    obj->is_interacted = false;
    obj->in_extraction = false;
    obj->value = config->value;
    obj->material = scene->material;
    strncpy(obj->name, config->name, sizeof(obj->name) - 1);
    obj->rotation = config->rotation;
    load_and_prepare_model(obj, config);

    Room* room = find_room_by_name(scene, config->room_name);
    obj->position = calculate_world_position(room, config);
    obj->position = constrain_to_room(obj->position, room, mesh_half_extents(&obj->model));

    if (config->texture_path[0] != '\0') {
        obj->texture_id = load_texture(config->texture_path);
    }
    else {
        printf("[WARNING]: No texture set for object '%s'.\n", obj->name);
        obj->texture_id = 0;
    }

    scene->object_count++;
    printf("Added object %d: %s\n", obj->id, obj->name);
}

void load_and_prepare_model(Object* obj, ObjectConfig* config) {
    load_model(&obj->model, config->model_path);

    Vec3 mesh_min, mesh_max, center;
    calculate_mesh_aabb(&obj->model, &mesh_min, &mesh_max);
    center = vec3_scale(vec3_add(mesh_min, mesh_max), 0.5f);
    for (int i = 1; i <= obj->model.n_vertices; ++i) {
        obj->model.vertices[i].x -= center.x;
        obj->model.vertices[i].y -= center.y;
        obj->model.vertices[i].z -= center.z;
    }

    if (config->scale.x > 0) {
        scale_model(&obj->model, config->scale);
    }
    if (config->rotation.x || config->rotation.y || config->rotation.z) {
        rotate_model(&obj->model, config->rotation);
    }
}

void sync_physics_transforms(Scene* scene) {
    for (int i = 0; i < scene->object_count; ++i) {
        Object* object = &scene->objects[i];

        if (object->is_static) continue;

        physics_get_position(&object->physics_body, &object->position);
        physics_get_rotation(&object->physics_body, &object->rotation);
    }
}

int select_object_at(Scene* scene, Camera* cam, int mx, int my, float* out_distance) {
    float nx = (2.0f * mx) / cam->viewport.width - 1.0f;
    float ny = 1.0f - (2.0f * my) / cam->viewport.height;

    update_camera_basis(cam);
    float fovv = tanf(degree_to_radian(cam->fov) * 0.5f);
    float fovh = fovv * cam->aspect_ratio;

    Vec3 dir = {
        cam->basis.forward.x + nx * cam->basis.right.x * fovh + ny * cam->basis.up.x * fovv,
        cam->basis.forward.y + nx * cam->basis.right.y * fovh + ny * cam->basis.up.y * fovv,
        cam->basis.forward.z + nx * cam->basis.right.z * fovh + ny * cam->basis.up.z * fovv
    };
    vec3_normalize(&dir);

    int hit = -1;
    float best = FLT_MAX;
    for (int i = 0; i < scene->object_count; ++i) {
        Object* obj = &scene->objects[i];
        if (!obj->is_active || obj->is_static) continue;

        float t;
        if (ray_intersect_obb(cam->position, dir, &obj->physics_body, &t) && t < best) {
            if (t > 3.0f) continue;
            best = t;
            hit = obj->id;
            if (out_distance) {
                *out_distance = best;
            }
        }
    }

    if (hit >= 0) {
        printf("Selected object: %s (ID: %d, Distance: %.2f)\n", scene->objects[hit].name, hit, best);
        scene->objects[hit].is_interacted = true;
        physics_wake_up(&scene->objects[hit].physics_body);
    }
    else {
        printf("No hit object\n");
    }

    return hit;
}

void move_selected_object(Scene* scene, Vec3 target_position) {
    Object* obj = find_object_by_id(scene, scene->selected_object_id);
    if (!obj || obj->is_static) return;

    Vec3 current;
    physics_get_position(&obj->physics_body, &current);

    Vec3 velocity;
    physics_get_linear_velocity(&obj->physics_body, &velocity);

    Vec3 delta = vec3_substract(target_position, current);

    dMass mass_info;
    dBodyGetMass(obj->physics_body.body, &mass_info);
    float mass = (float)mass_info.mass;
    float inv_mass = (mass > 0.0001f ? 1.0f / mass : 1.0f);
    Vec3 vel = vec3_scale(delta, 5.0f * inv_mass);

    physics_set_linear_velocity(&obj->physics_body, vel);
}

void rotate_selected_object(Scene* scene, float rx, float ry) {
    Object* obj = find_object_by_id(scene, scene->selected_object_id);
    if (!obj || obj->is_static) return;

    Vec3 angle = { rx * 2.0f, ry * 2.0f, 0 };
    physics_set_angular_velocity(&obj->physics_body, angle);
}