#ifndef CONFIG_H
#define CONFIG_H

#include "utils.h"

typedef struct Lighting Lighting;

/**
 * Configuration for an object.
 */
typedef struct ObjectConfig {
    char name[64];
    char model_path[256];
    char texture_path[256];
    Vec3 offset;
    Vec3 rotation;
    Vec3 scale;
    float mass;
    int value;
    char room_name[64];
    bool is_static;
} ObjectConfig;

/**
 * Configuration for a room.
 */
typedef struct RoomConfig {
    char name[64];
    Vec3 dimension;
    char floor_tex_path[256];
    char ceiling_tex_path[256];
    char wall_tex_path[256];
    Connection connections[DIR_COUNT];
    int connection_count;
} RoomConfig;

/**
 * Read the configuration file for lights.
 */
void read_light_config(const char* filename, Lighting* light_config, int* light_config_count);

/**
 * Read the configuration file for rooms.
 */
void read_room_config(const char* filename, RoomConfig* room_config, int* room_config_count);

/**
 * Read the configuration file for objects.
 */
void read_object_config(const char* filename, ObjectConfig* obj_config, int* obj_config_count);

/**
 * Read the manual file and format it for rendering.
 */
char* read_manual(const char* filename);

/**
 * Find an object config by name.
 */
ObjectConfig* find_object_config_by_name(ObjectConfig* configs, int count, const char* name);

/**
 * Verifying configs.
 */
void print_light_configs(Lighting* light_config, int light_config_count);
void print_room_configs(RoomConfig* room_config, int room_config_count);
void print_object_configs(ObjectConfig* obj_config, int obj_config_count);

#endif  /* CONFIG_H */