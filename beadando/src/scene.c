#include "scene.h"
#include "draw.h"
#include <obj/load.h>
#include <obj/draw.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void init_scene(Scene* scene) {
    scene->material.ambient = (ColorRGB){ 0.2, 0.2, 0.2 };
    scene->material.diffuse = (ColorRGB){ 0.8, 0.8, 0.8 };
    scene->material.specular = (ColorRGB){ 0.2, 0.2, 0.2 };
    scene->material.shininess = 20.0;
    set_material(&scene->material);

    init_physics(&scene->physics_world);

    int room_config_count = 0;
    RoomConfig room_configs[MAX_ROOMS];
    read_room_config("config/room_config.json", room_configs, &room_config_count);
    print_room_configs(room_configs, room_config_count);

    scene->rooms = calloc(room_config_count, sizeof(Room));
    scene->room_count = 0;
    for (int i = 0; i < room_config_count; i++) {
        add_room(scene, &room_configs[i]);
    }

    place_rooms_by_connections(scene);

    int light_config_count = 0;
    Lighting light_configs[MAX_LIGHTS];
    read_light_config("config/light_config.json", light_configs, &light_config_count);

    scene->lights = calloc(light_config_count, sizeof(Lighting));
    scene->light_count = 0;
    for (int i = 0; i < light_config_count; i++) {
        add_light(scene, &light_configs[i]);
    }

    print_light_configs(scene->lights, scene->light_count);

    int obj_config_count = 0;
    ObjectConfig obj_configs[MAX_OBJECTS];
    read_object_config("config/object_config.json", obj_configs, &obj_config_count);
    print_object_configs(obj_configs, obj_config_count);

    scene->objects = calloc(obj_config_count, sizeof(Object));
    scene->object_count = 0;
    scene->selected_object_id = -1;
    
    for (int i = 0; i < obj_config_count; i++) {
        add_object(scene, &obj_configs[i]);
    }
    
    init_extraction(scene);

    for (int i = 0; i < scene->object_count; i++) {
        ObjectConfig* config = find_object_config_by_name(obj_configs, obj_config_count, scene->objects[i].name);
        if (config) {
            if (scene->objects[i].is_static) {
                create_static_physics_and_display(&scene->objects[i], scene);
            } else {
                create_dynamic_physics_and_display(&scene->objects[i], scene, config->mass);
            }
        }
    }

    physics_create_ground_plane(&scene->physics_world);
    dReal gravity[] = { 0.0, 0.0, -8.0 };
    physics_init_gravity(&scene->physics_world, gravity);

    printf("Scene initialized with %d lights, %d objects in %d rooms\n",
        scene->light_count, scene->object_count, scene->room_count);
}

void create_static_physics_and_display(Object* obj, Scene* scene) {
    Vec3 mesh_min, mesh_max;
    calculate_mesh_aabb(&obj->model, &mesh_min, &mesh_max);
    obj->position.z = fmaxf(obj->position.z - mesh_min.z, 0.01f);

    Vec3 mesh_half_ext = {
        fmaxf((mesh_max.x - mesh_min.x) * 0.5f, 0.01f),
        fmaxf((mesh_max.y - mesh_min.y) * 0.5f, 0.01f),
        fmaxf((mesh_max.z - mesh_min.z) * 0.5f, 0.01f)
    };

    dGeomID geom = dCreateBox(scene->physics_world.space,
        mesh_half_ext.x * 2,
        mesh_half_ext.y * 2,
        mesh_half_ext.z * 2);
    dGeomSetPosition(geom, obj->position.x, obj->position.y, obj->position.z);

    obj->physics_body.geom = geom;
    obj->physics_body.body = NULL;

    obj->display_list = glGenLists(1);
    glNewList(obj->display_list, GL_COMPILE);
    glBindTexture(GL_TEXTURE_2D, obj->texture_id);
    glPushMatrix();
    glTranslatef(obj->position.x, obj->position.y, obj->position.z);
    draw_model(&obj->model);
    glPopMatrix();
    glEndList();
}

void create_dynamic_physics_and_display(Object* obj, Scene* scene, float mass) {
    Vec3 mesh_min, mesh_max;
    calculate_mesh_aabb(&obj->model, &mesh_min, &mesh_max);
    obj->position.z = fmaxf(obj->position.z - mesh_min.z, 0.01f);

    Vec3 mesh_half_ext = {
        fmaxf((mesh_max.x - mesh_min.x) * 0.5f, 0.01f),
        fmaxf((mesh_max.y - mesh_min.y) * 0.5f, 0.01f),
        fmaxf((mesh_max.z - mesh_min.z) * 0.5f, 0.01f)
    };
    
    physics_create_box(&scene->physics_world, &obj->physics_body, mass, obj->position, mesh_half_ext);
    obj->physics_body.user_data = obj;

    obj->display_list = glGenLists(1);
    glNewList(obj->display_list, GL_COMPILE);
    glBindTexture(GL_TEXTURE_2D, obj->texture_id);
    draw_model(&obj->model);
    glEndList();
}

Room* find_room_by_name(Scene* scene, const char* name) {
    for (int i = 0; i < scene->room_count; i++) {
        if (strcmp(scene->rooms[i].name, name) == 0) {
            return &scene->rooms[i];
        }
    }
    return NULL;
}

Lighting* find_light_by_name(Scene* scene, const char* name) {
    for (int i = 0; i < scene->light_count; i++) {
        if (strcmp(scene->lights[i].name, name) == 0) {
            return &scene->lights[i];
        }
    }
    return NULL;
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

void update_scene(Scene* scene, double elapsed_time) {
    static float total_time = 0.0f;
    total_time += elapsed_time;

    update_lighting(scene, total_time);
    physics_simulate(&scene->physics_world, elapsed_time);
    sync_physics_transforms(scene);
    update_extraction(scene);
}

void render_scene(const Scene* scene) {
    for (int i = 0; i < scene->light_count; i++) {
        if (scene->lights[i].enabled) {
            glEnable(GL_LIGHT0 + scene->lights[i].slot);
            set_lighting(i, &scene->lights[i]);
        }
        else {
            glDisable(GL_LIGHT0 + scene->lights[i].slot);
        }
    }

    for (int i = 0; i < scene->room_count; i++) {
        glCallList(scene->rooms[i].display_list);
    }

    draw_extraction_status(scene);

    for (int i = 0; i < scene->object_count; i++) {
        Object* obj = &scene->objects[i];
        if (!obj->is_active) continue;
        
        set_material(&obj->material);

        if (obj->is_static) {
            glCallList(obj->display_list);
        }
        else {
            const dReal* M = dBodyGetRotation(scene->objects[i].physics_body.body);
            GLfloat mat[16] = {
                (GLfloat)M[0], (GLfloat)M[4], (GLfloat)M[8],  0,
                (GLfloat)M[1], (GLfloat)M[5], (GLfloat)M[9],  0,
                (GLfloat)M[2], (GLfloat)M[6], (GLfloat)M[10], 0,
                0,             0,             0,              1
            };

            glPushMatrix();
            glTranslatef(
                obj->position.x,
                obj->position.y,
                obj->position.z
            );
            glMultMatrixf(mat);

            glCallList(obj->display_list);
            glPopMatrix();
        }
        
        if (scene->selected_object_id == obj->id) {
            draw_bounding_box(&obj->physics_body);
        }
    }
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