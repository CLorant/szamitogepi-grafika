#ifndef ROOM_H
#define ROOM_H

#include "config.h"
#include "physics.h"
#include "utils.h"
#include <GL/gl.h>
#include <stdbool.h>

typedef struct Scene Scene;

/**
 * Place the rooms in world-space.
 */
typedef struct RoomPlacement {
    int idx;
    bool placed;
    int gx;
    int gy;
} RoomPlacement;

/**
 * A room of the scene.
 */
typedef struct Room {
    int id;
    char name[64];
    Vec3 dimension;
    Vec3 position;
    GLuint floor_tex;
    GLuint ceiling_tex;
    GLuint wall_tex;
    Connection connections[DIR_COUNT];
    float wall_thickness;
    float door_width;
    float door_height;
    GLuint display_list;
} Room;

/**
 *  Add a room to the scene.
 */
void add_room(Scene* scene, RoomConfig* config);

/**
 * Calculate the world position of an object within a room.
 */
Vec3 calculate_world_position(Room* room, ObjectConfig* config);

/**
 * Calculate the position within a room.
 */
Vec3 constrain_to_room(Vec3 position, Room* room, Vec3 mesh_half_ext);

/**
 * Use BFS to place the rooms in world-space by connection.
 */
void place_rooms_by_connections(Scene* scene);

/**
 * Determine if two rooms need a connector wall.
 */
bool needs_connector(Room* room, Room* neighbor, Direction dir);

#endif /* ROOM_H */
