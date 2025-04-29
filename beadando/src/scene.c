#include "scene.h"
#include "config.h"
#include <obj/load.h>
#include <obj/draw.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void init_scene(Scene* scene) {
    scene->material.ambient = (ColorRGB){ 0.0, 0.0, 0.0 };
    scene->material.diffuse = (ColorRGB){ 0.8, 0.8, 0.8 };
    scene->material.specular = (ColorRGB){ 0.0, 0.0, 0.0 };
    scene->material.shininess = 80.0;

    int light_config_count = 0;
    Lighting light_configs[MAX_LIGHTS];
    
    read_light_config("config/light_config.csv", light_configs, &light_config_count);
    print_light_configs(light_configs, light_config_count);

    scene->lights = calloc(light_config_count, sizeof(Lighting));
    scene->light_count = 0;
    for (int i = 0; i < light_config_count; i++) {
        add_light(scene, &light_configs[i]);
    }

    dReal gravity[] = { 0.0, 0.0, -9.81 };
    scene->physics_world = init_physics(gravity);
    physics_create_ground_plane(scene->physics_world);

    int obj_config_count = 0;
    ObjectConfig obj_configs[MAX_OBJECTS];

    read_object_config("config/object_config.csv", obj_configs, &obj_config_count);
    print_object_configs(obj_configs, obj_config_count);

    scene->objects = calloc(obj_config_count, sizeof(Object));
    scene->object_count = 0;
    scene->selected_object_id = -1;
    for (int i = 0; i < obj_config_count; i++) {
        add_object(scene, &obj_configs[i]);
    }

    place_objects_on_ground(scene);

    printf("Scene initialized with %d objects\n", scene->object_count);
}

void add_light(Scene* scene, Lighting* config) {
    Lighting new_light = *config;
    new_light.enabled = true;
    new_light.slot = scene->light_count;
    scene->lights[scene->light_count++] = new_light;

    glEnable(GL_LIGHT0 + new_light.slot);
}

void add_object(Scene* scene, ObjectConfig* config) {
    Object* obj = &scene->objects[scene->object_count];
    memset(obj, 0, sizeof(Object));
    obj->id = scene->object_count;
    obj->is_active = true;
    obj->is_static = config->is_static;
    obj->material = &scene->material;
    strncpy(obj->name, config->name, sizeof(obj->name)-1);

    obj->position = config->position;
    obj->rotation = config->rotation;

    
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

    calculate_mesh_aabb(&obj->model, &mesh_min, &mesh_max);
    Vec3 half_ext = {
        (mesh_max.x - mesh_min.x) * 0.5f,
        (mesh_max.y - mesh_min.y) * 0.5f,
        (mesh_max.z - mesh_min.z) * 0.5f
    };

    if (config->texture_path[0] != '\0') {
        obj->texture_id = load_texture(config->texture_path);
    }
    else {
        printf("[WARNING]: No texture set for object '%s'.\n", obj->name);
        obj->texture_id = 0;
    }
    

    if (config->is_static) {
        dGeomID geom = dCreateBox(scene->physics_world->space, half_ext.x * 2, half_ext.y * 2, half_ext.z * 2);
        calculate_mesh_aabb(&obj->model, &mesh_min, &mesh_max);
    
        dGeomSetPosition(geom,
            config->position.x,
            config->position.y,
            config->position.z - mesh_min.z
        );
    
        obj->physics_body.geom = geom;
        obj->physics_body.body = NULL; 
    
        const dReal* pos = dGeomGetPosition(geom);
        obj->display_list = glGenLists(1);
        glNewList(obj->display_list, GL_COMPILE);
            glBindTexture(GL_TEXTURE_2D, obj->texture_id);
            glPushMatrix();
                glTranslatef(pos[0], pos[1], pos[2]);
                draw_model(&obj->model);
            glPopMatrix();
        glEndList();
    }
    else {
        float mass = half_ext.x * half_ext.y * half_ext.z;
        obj->physics_body = physics_create_box(
            scene->physics_world,
            mass,
            obj->position,
            half_ext
        );
    
        obj->display_list = glGenLists(1);
        glNewList(obj->display_list, GL_COMPILE);
            glBindTexture(GL_TEXTURE_2D, obj->texture_id);
            draw_model(&obj->model);
        glEndList();
    }
    
    scene->object_count++;
    printf("Added object %d: %s\n\n", obj->id, obj->name);
}

void place_objects_on_ground(Scene* scene) {
    Vec3 zero_velocity = (Vec3){0, 0, 0};

    for (int i = 0; i < scene->object_count; ++i) {
        Object* obj = &scene->objects[i];
        
        if (obj->is_static) continue;

        Vec3 C[8];
        physics_get_obb_corners(&obj->physics_body, C);
        float min_z = C[0].z;
        for (int i = 1; i < 8; ++i)
            if (C[i].z < min_z) min_z = C[i].z;

        Vec3 pos;
        physics_get_position(&obj->physics_body, &pos);
        pos.z += -min_z;
        physics_set_position(&obj->physics_body, pos);
        physics_set_linear_velocity(&obj->physics_body, zero_velocity);
        physics_set_angular_velocity(&obj->physics_body, zero_velocity);
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

Object* find_object_by_name(Scene* scene, const char* name) {
    for (int i = 0; i < scene->object_count; i++) {
        if (strcmp(scene->objects[i].name, name) == 0) {
            return &scene->objects[i];
        }
    }
    return NULL;
}

Object* find_object_by_id(Scene* scene, int id) {
    if (id >= 0 && id < scene->object_count) {
        return &scene->objects[id];
    }
    return NULL;
}

void adjust_brightness(Scene* scene, float amount) {
    float brightness;
    for (int i = 0; i < scene->light_count; i++) {
        if (!scene->lights[i].enabled) continue;

        brightness = scene->lights[i].brightness + amount;
        scene->lights[i].brightness = fminf(fmaxf(brightness, 0.1f), 1.0f);
    }
}

void update_lighting(Scene* scene, float total_time) {
    for (int i = 0; i < scene->light_count; i++) {
        if (!scene->lights[i].enabled) continue;

        if (strcmp(scene->lights[i].name, "disco") == 0) {
            ColorRGB c = sine_animate_color(total_time, i);
            scene->lights[i].diffuse = (ColorRGBA){ c.red, c.green, c.blue, 1.0f };
        }
    }
}

void update_scene(Scene* scene, double elapsed_time) {
    static float total_time = 0.0f;
    total_time += elapsed_time;

    update_lighting(scene, total_time);
    physics_simulate(scene->physics_world, elapsed_time);
    sync_physics_transforms(scene);
}

void render_scene(const Scene* scene) {
    glDisable(GL_LIGHTING);
        glBindTexture(GL_TEXTURE_2D, 0);
        draw_origin(1);
        draw_checkerboard(4, 1);
    glEnable(GL_LIGHTING);
    
    set_material(&scene->material);

    for (int i = 0; i < scene->light_count; i++) {
        if (scene->lights[i].enabled) {
            glEnable(GL_LIGHT0 + scene->lights[i].slot);
            set_lighting(i, &scene->lights[i]);
        }
        else {
            glDisable(GL_LIGHT0 + scene->lights[i].slot);
        }
    }

    for (int i = 0; i < scene->object_count; i++) {
        Object* obj = &scene->objects[i];
        if (!obj->is_active) continue;

        glPushMatrix();

        if (obj->is_static) {
            glCallList(obj->display_list);
        }
        else {
            glTranslatef(
                obj->position.x,
                obj->position.y,
                obj->position.z
            );
            
            glRotatef(obj->rotation.x, 1, 0, 0);
            glRotatef(obj->rotation.y, 0, 1, 0);
            glRotatef(obj->rotation.z, 0, 0, 1);

            glCallList(obj->display_list);
        }

        glPopMatrix();

        if (scene->selected_object_id == obj->id) {
            physics_draw_obb(&obj->physics_body);
        }
    }
}

int select_object_at(Scene* scene, Camera* cam, int mx, int my) {
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

    int hit  = -1;
    float best = FLT_MAX;
    for (int i = 0; i < scene->object_count; ++i) {
        Object* obj = &scene->objects[i];
        if (!obj->is_active || obj->is_static) continue;

        float t;
        if (ray_intersect_obb(cam->position, dir, &obj->physics_body, &t) && t < best) {
            best = t;
            hit = obj->id;
        }
    }

    if (hit >= 0) {
        printf("Selected object: %s (ID: %d, Distance: %.2f)\n", scene->objects[hit].name, hit, best);
        physics_wake_up(&scene->objects[hit].physics_body);
    }
    else {
        printf("No hit object\n");
    }

    return hit;
}

void move_selected_object(Scene* scene, Vec3 delta) {
    Object* obj = find_object_by_id(scene, scene->selected_object_id);
    if (!obj || obj->is_static) return;

    Vec3 pos;
    physics_get_position(&obj->physics_body, &pos);
    Vec3 target = vec3_add(pos, vec3_scale(delta, 5.0f));
    Vec3 vel = vec3_substract(target, pos);
    vel = vec3_scale(vel, 5.0f);
    physics_set_linear_velocity(&obj->physics_body, vel);
    physics_wake_up(&obj->physics_body);
}

void rotate_selected_object(Scene* scene, float rx, float ry) {
    Object* obj = find_object_by_id(scene, scene->selected_object_id);
    if (!obj || obj->is_static) return;

    Vec3 ang = { rx * 2.0f, ry * 2.0f, 0 };
    physics_set_angular_velocity(&obj->physics_body, ang);
    physics_wake_up(&obj->physics_body);
}

void free_scene(Scene* scene) {
    if (scene->objects != NULL) {
        for (int i = 0; i < scene->object_count; i++) {
            glDeleteLists(scene->objects[i].display_list, 1);
            glDeleteTextures(1, &scene->objects[i].texture_id);
            free_model(&scene->objects[i].model);
        }
        free(scene->lights);
        free(scene->objects);
    }
}