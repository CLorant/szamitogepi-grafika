#include "room.h"
#include "scene.h"
#include <string.h>
#include <stdio.h>
#include "draw.h"
#include "physics.h"

void add_room(Scene* scene, RoomConfig* config) {
    Room* room = &scene->rooms[scene->room_count];
    room->id = scene->room_count;
    strncpy(room->name, config->name, sizeof(room->name) - 1);
    room->dimension = config->dimension;
    room->position = (Vec3){ 0,0,0 };
    
    for (int d = 0; d < DIR_COUNT; d++) {
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

Vec3 calculate_world_position(Room* room, ObjectConfig* config) {
    float half_width = room->dimension.x * 0.5f;
    float half_length = room->dimension.y * 0.5f;

    Vec3 world = {
        room->position.x + config->offset.x * half_width,
        room->position.y + config->offset.y * half_length,
        room->position.z + config->offset.z
    };

    return world;
}

Vec3 constrain_to_room(Vec3 position, Room* room, Vec3 mesh_half_ext) {
    float half_width = room->dimension.x * 0.5f;
    float half_length = room->dimension.y * 0.5f;

    float min_x = room->position.x - half_width + 0.05f + mesh_half_ext.x;
    float max_x = room->position.x + half_width - 0.05f - mesh_half_ext.x;
    float min_y = room->position.y - half_length + 0.05f + mesh_half_ext.y;
    float max_y = room->position.y + half_length - 0.05f - mesh_half_ext.y;

    position.x = fminf(fmaxf(position.x, min_x), max_x);
    position.y = fminf(fmaxf(position.y, min_y), max_y);

    return position;
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