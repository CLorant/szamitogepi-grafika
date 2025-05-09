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

    place_objects_on_ground(scene);
    physics_create_ground_plane(&scene->physics_world);

    dReal gravity[] = { 0.0, 0.0, -8.0 };
    physics_init_gravity(&scene->physics_world, gravity);

    printf("Scene initialized with %d lights, %d objects in %d rooms\n",
        scene->light_count, scene->object_count, scene->room_count);
}

void add_light(Scene* scene, Lighting* config) {
    Lighting new_light = *config;
    new_light.enabled = true;
    new_light.slot = scene->light_count;

    if (new_light.is_spotlight && new_light.room_name[0] != '\0') {
        Room* room = find_room_by_name(scene, new_light.room_name);
        if (room) {
            new_light.position = (Vec4){
                room->position.x,
                room->position.y,
                room->position.z + room->dimension.z,
                1.0f
            };

            printf("Positioned spotlight '%s' at [%.2f, %.2f, %.2f] in room '%s'\n",
                new_light.name,
                new_light.position.x,
                new_light.position.y,
                new_light.position.z,
                new_light.room_name);
        }
        else {
            printf("[WARNING]: Could not find room '%s' for light '%s'\n",
                new_light.room_name, new_light.name);
        }
    }

    scene->lights[scene->light_count++] = new_light;
    glEnable(GL_LIGHT0 + new_light.slot);
}

void add_room(Scene* scene, RoomConfig* config) {
    Room* room = &scene->rooms[scene->room_count];
    room->id = scene->room_count;
    strncpy(room->name, config->name, sizeof(room->name) - 1);
    room->dimension = config->dimension;
    room->position = (Vec3){ 0,0,0 };
    for (int d = 0; d < DIR_COUNT; ++d) {
        strncpy(room->connections[d].room,
            config->connections[d].room,
            sizeof(room->connections[d].room) - 1
        );
        room->connections[d].dir = config->connections[d].dir;
    }

    if (config->floor_tex_path[0] != '\0') {
        room->floor_tex = load_texture(config->floor_tex_path);
    }
    else {
        printf("[WARNING]: No texture set for floor in room '%s'.\n", room->name);
        room->floor_tex = 0;
    }

    if (config->ceiling_tex_path[0] != '\0') {
        room->ceiling_tex = load_texture(config->ceiling_tex_path);
    }
    else {
        printf("[WARNING]: No texture set for ceiling in room '%s'.\n", room->name);
        room->ceiling_tex = 0;
    }

    if (config->wall_tex_path[0] != '\0') {
        room->wall_tex = load_texture(config->wall_tex_path);
    }
    else {
        printf("[WARNING]: No texture set for walls in room '%s'.\n", room->name);
        room->wall_tex = 0;
    }

    room->wall_thickness = 0.1f;
    room->door_width = 1.5f;
    room->door_height = 2.0f;

    scene->room_count++;
    printf("Added room %d: %s  (%.1f x %.1f x %.1f)\n",
        room->id, room->name,
        room->dimension.x,
        room->dimension.y,
        room->dimension.z);
}

void add_object(Scene* scene, ObjectConfig* config) {
    Object* obj = &scene->objects[scene->object_count];
    memset(obj, 0, sizeof(Object));
    obj->id = scene->object_count;
    obj->is_active = true;
    obj->is_static = config->is_static;
    obj->is_interacted = false;
    obj->value = config->value;
    obj->material = scene->material;
    strncpy(obj->name, config->name, sizeof(obj->name) - 1);

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
    Vec3 mesh_half_ext = {
        fmaxf((mesh_max.x - mesh_min.x) * 0.5f, 0.01f),
        fmaxf((mesh_max.y - mesh_min.y) * 0.5f, 0.01f),
        fmaxf((mesh_max.z - mesh_min.z) * 0.5f, 0.01f)
    };

    Room* room = find_room_by_name(scene, config->room_name);
    float half_width = room->dimension.x * 0.5f;
    float half_length = room->dimension.y * 0.5f;

    Vec3 world = {
        room->position.x + config->offset.x * half_width,
        room->position.y + config->offset.y * half_length,
        room->position.z + config->offset.z
    };

    obj->position = world;

    float min_x = room->position.x - half_width + 0.05f + mesh_half_ext.x;
    float max_x = room->position.x + half_width - 0.05f - mesh_half_ext.x;
    float min_y = room->position.y - half_length + 0.05f + mesh_half_ext.y;
    float max_y = room->position.y + half_length - 0.05f - mesh_half_ext.y;

    obj->position.x = fminf(fmaxf(obj->position.x, min_x), max_x);
    obj->position.y = fminf(fmaxf(obj->position.y, min_y), max_y);

    if (config->texture_path[0] != '\0') {
        obj->texture_id = load_texture(config->texture_path);
    }
    else {
        printf("[WARNING]: No texture set for object '%s'.\n", obj->name);
        obj->texture_id = 0;
    }

    if (obj->is_static) {
        dGeomID geom = dCreateBox(scene->physics_world.space, mesh_half_ext.x * 2, mesh_half_ext.y * 2, mesh_half_ext.z * 2);
        calculate_mesh_aabb(&obj->model, &mesh_min, &mesh_max);

        dGeomSetPosition(geom,
            obj->position.x,
            obj->position.y,
            obj->position.z - mesh_min.z
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
        obj->half_height = mesh_half_ext.z;
        physics_create_box(
            &scene->physics_world,
            &obj->physics_body,
            config->mass,
            obj->position,
            mesh_half_ext
        );
        dBodySetData(obj->physics_body.body, obj);
        obj->display_list = glGenLists(1);
        glNewList(obj->display_list, GL_COMPILE);
        glBindTexture(GL_TEXTURE_2D, obj->texture_id);
        draw_model(&obj->model);
        glEndList();
    }

    scene->object_count++;
    printf("Added object %d: %s\n\n", obj->id, obj->name);
}

bool needs_connector(Room* room, Room* neighbor, Direction dir) {
    if (room->dimension.z > neighbor->dimension.z) {
        return true;
    }

    if (dir == DIR_NORTH || dir == DIR_SOUTH) {
        return room->dimension.x > neighbor->dimension.x;
    }

    return room->dimension.y > neighbor->dimension.y;
}

void place_rooms_by_connections(Scene* scene) {
    for (int i = 0; i < scene->room_count; i++) {
        Room* room = &scene->rooms[i];

        for (int d = 0; d < DIR_COUNT; d++) {
            if (room->connections[d].room[0] == '\0') continue;

            for (int j = 0; j < scene->room_count; j++) {
                if (strcmp(scene->rooms[j].name, room->connections[d].room) == 0) {
                    room->connections[d].id = j;
                    break;
                }
            }
        }
    }

    RoomPlacement placement[MAX_ROOMS] = { {0} };
    for (int i = 0; i < scene->room_count; ++i) {
        placement[i].idx = i;
        placement[i].placed = false;
        placement[i].gx = 0;
        placement[i].gy = 0;
    }

    int start_idx = -1;
    for (int i = 0; i < scene->room_count; ++i) {
        if (strcmp(scene->rooms[i].name, "start_room") == 0) {
            start_idx = i;
            break;
        }
    }

    if (start_idx < 0) start_idx = 0;

    placement[start_idx].placed = true;
    placement[start_idx].gx = 0;
    placement[start_idx].gy = 0;

    int queue[MAX_ROOMS], qh = 0, qt = 0;
    queue[qt++] = start_idx;

    while (qh < qt) {
        int current_idx = queue[qh++];
        Room* current_room = &scene->rooms[current_idx];

        for (int d = 0; d < DIR_COUNT; d++) {
            if (current_room->connections[d].room[0] == '\0') continue;

            int neighbor_idx = current_room->connections[d].id;
            Direction dir = current_room->connections[d].dir;

            if (neighbor_idx < 0 || neighbor_idx >= scene->room_count) continue;

            Room* neighbor_room = &scene->rooms[neighbor_idx];

            if (!placement[neighbor_idx].placed) {
                int dx, dy;
                get_delta(dir, &dx, &dy);

                placement[neighbor_idx].gx = placement[current_idx].gx + dx;
                placement[neighbor_idx].gy = placement[current_idx].gy + dy;
                placement[neighbor_idx].placed = true;

                float shift_x = dx * (current_room->dimension.x * 0.5f + neighbor_room->dimension.x * 0.5f);
                float shift_y = dy * (current_room->dimension.y * 0.5f + neighbor_room->dimension.y * 0.5f);

                neighbor_room->position.x = current_room->position.x + shift_x;
                neighbor_room->position.y = current_room->position.y + shift_y;
                neighbor_room->position.z = current_room->position.z;

                printf("Placing %s at (%.1f,%.1f)\n",
                    neighbor_room->name, neighbor_room->position.x, neighbor_room->position.y);

                queue[qt++] = neighbor_idx;
            }
        }
    }

    for (int i = 0; i < scene->room_count; ++i) {
        Room* room = &scene->rooms[i];

        for (int d = 0; d < DIR_COUNT; ++d) {
            if (room->connections[d].room[0] == '\0') {
                physics_create_wall_filled(
                    &scene->physics_world,
                    room->position,
                    room->dimension,
                    (Direction)d,
                    room->wall_thickness
                );
            }
            else {
                int neighbor_idx = room->connections[d].id;
                if (neighbor_idx < 0 || neighbor_idx >= scene->room_count) continue;

                Room* neighbor_room = &scene->rooms[neighbor_idx];
                if (!needs_connector(room, neighbor_room, d)) continue;

                physics_create_wall_connector(
                    &scene->physics_world,
                    room->position,
                    room->dimension,
                    (Direction)d,
                    room->wall_thickness,
                    room->door_width,
                    room->door_height
                );
            }
        }

        scene->rooms[i].display_list = glGenLists(1);
        glNewList(scene->rooms[i].display_list, GL_COMPILE);
        draw_room(&scene->rooms[i], scene);
        glEndList();
    }
}

void place_objects_on_ground(Scene* scene) {
    Vec3 zero_velocity = (Vec3){ 0, 0, 0 };

    for (int i = 0; i < scene->object_count; i++) {
        Object* obj = &scene->objects[i];

        if (obj->is_static) continue;

        Vec3 C[8];
        physics_get_obb_corners(&obj->physics_body, C);
        float min_z = C[0].z;
        for (int j = 1; j < 8; j++) {
            if (C[j].z < min_z) min_z = C[j].z;
        }

        Vec3 pos;
        physics_get_position(&obj->physics_body, &pos);
        pos.z += (-min_z + obj->half_height + 0.25f);
        physics_set_position(&obj->physics_body, pos);
        physics_set_linear_velocity(&obj->physics_body, zero_velocity);
        physics_set_angular_velocity(&obj->physics_body, zero_velocity);
    }

    sync_physics_transforms(scene);
}

void sync_physics_transforms(Scene* scene) {
    for (int i = 0; i < scene->object_count; ++i) {
        Object* object = &scene->objects[i];

        if (object->is_static) continue;

        physics_get_position(&object->physics_body, &object->position);
        physics_get_rotation(&object->physics_body, &object->rotation);
    }
}

Room* find_room_by_name(Scene* scene, const char* name) {
    for (int i = 0; i < scene->room_count; i++) {
        if (strcmp(scene->rooms[i].name, name) == 0) {
            return &scene->rooms[i];
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

void adjust_brightness(Scene* scene, float amount) {
    float brightness;
    for (int i = 0; i < scene->light_count; i++) {
        if (!scene->lights[i].enabled) continue;

        brightness = scene->lights[i].brightness + amount;
        scene->lights[i].brightness = fminf(fmaxf(brightness, 0.1f), 2.0f);
    }
}

void set_lighting(int slot, const Lighting* light) {
    GLenum light_enum = GL_LIGHT0 + slot;

    float ambient_light[] = {
        light->ambient.red,
        light->ambient.green,
        light->ambient.blue,
        light->ambient.alpha
    };

    float diffuse_light[] = {
        light->diffuse.red * light->brightness,
        light->diffuse.green * light->brightness,
        light->diffuse.blue * light->brightness,
        light->diffuse.alpha
    };

    float specular_light[] = {
        light->specular.red,
        light->specular.green,
        light->specular.blue,
        light->specular.alpha
    };

    float light_position[] = {
        light->position.x,
        light->position.y,
        light->position.z,
        light->position.w
    };

    glLightfv(light_enum, GL_AMBIENT, ambient_light);
    glLightfv(light_enum, GL_DIFFUSE, diffuse_light);
    glLightfv(light_enum, GL_SPECULAR, specular_light);
    glLightfv(light_enum, GL_POSITION, light_position);

    if (light->is_spotlight) {
        float spot_direction[] = {
            light->direction.x,
            light->direction.y,
            light->direction.z
        };
        glLightfv(light_enum, GL_SPOT_DIRECTION, spot_direction);
        glLightf (light_enum, GL_SPOT_CUTOFF, light->cutoff);
        glLightf (light_enum, GL_SPOT_EXPONENT, light->exponent);
    }
}

void update_lighting(Scene* scene, float total_time) {
    for (int i = 0; i < scene->light_count; i++) {
        if (!scene->lights[i].enabled) continue;

        if (strcmp(scene->lights[i].name, "disco_light") == 0) {
            ColorRGB c = sine_animate_color(total_time, i);
            scene->lights[i].diffuse = (ColorRGBA){ c.red, c.green, c.blue, 1.0f };
        }
    }
}

void update_scene(Scene* scene, double elapsed_time) {
    static float total_time = 0.0f;
    total_time += elapsed_time;

    update_lighting(scene, total_time);
    physics_simulate(&scene->physics_world, elapsed_time);
    sync_physics_transforms(scene);
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

    dMass mInfo;
    dBodyGetMass(obj->physics_body.body, &mInfo);
    float mass = (float)mInfo.mass;
    float invMass = (mass > 0.0001f ? 1.0f / mass : 1.0f);
    Vec3 vel = vec3_scale(delta, 5.0f * invMass);

    physics_set_linear_velocity(&obj->physics_body, vel);
}

void rotate_selected_object(Scene* scene, float rx, float ry) {
    Object* obj = find_object_by_id(scene, scene->selected_object_id);
    if (!obj || obj->is_static) return;

    Vec3 ang = { rx * 2.0f, ry * 2.0f, 0 };
    physics_set_angular_velocity(&obj->physics_body, ang);
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