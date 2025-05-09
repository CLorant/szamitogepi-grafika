#include "config.h"
#include "scene.h"
#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

static json_object* parse_json_file(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("[ERROR] Cannot open %s\n", filename);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* data = malloc(len + 1);
    fread(data, 1, len, fp);
    data[len] = '\0';
    fclose(fp);

    json_object* root = json_tokener_parse(data);
    free(data);
    return root;
}

static void json_get_string_field(json_object* obj, const char* field, char* dest, size_t dest_size) {
    json_object* field_obj = json_object_object_get(obj, field);
    if (field_obj) {
        const char* str = json_object_get_string(field_obj);
        if (str) {
            strncpy(dest, str, dest_size - 1);
            dest[dest_size - 1] = '\0';
        }
    }
}

static float json_get_float_field(json_object* obj, const char* field, float default_value) {
    json_object* field_obj = json_object_object_get(obj, field);
    return field_obj ? (float)json_object_get_double(field_obj) : default_value;
}

static int json_get_int_field(json_object* obj, const char* field, int default_value) {
    json_object* field_obj = json_object_object_get(obj, field);
    return field_obj ? json_object_get_int(field_obj) : default_value;
}

static bool json_get_bool_field(json_object* obj, const char* field, bool default_value) {
    json_object* field_obj = json_object_object_get(obj, field);
    return field_obj ? json_object_get_boolean(field_obj) : default_value;
}

static void json_get_vec3_field(json_object* obj, const char* field, Vec3* vec) {
    json_object* array = json_object_object_get(obj, field);
    if (array && json_object_is_type(array, json_type_array) && 
        json_object_array_length(array) >= 3) {
        vec->x = (float)json_object_get_double(json_object_array_get_idx(array, 0));
        vec->y = (float)json_object_get_double(json_object_array_get_idx(array, 1));
        vec->z = (float)json_object_get_double(json_object_array_get_idx(array, 2));
    }
}

static void json_get_vec4_field(json_object* obj, const char* field, Vec4* vec) {
    json_object* array = json_object_object_get(obj, field);
    if (array && json_object_is_type(array, json_type_array) && 
        json_object_array_length(array) >= 4) {
        vec->x = (float)json_object_get_double(json_object_array_get_idx(array, 0));
        vec->y = (float)json_object_get_double(json_object_array_get_idx(array, 1));
        vec->z = (float)json_object_get_double(json_object_array_get_idx(array, 2));
        vec->w = (float)json_object_get_double(json_object_array_get_idx(array, 3));
    }
}

static void json_get_rgba_field(json_object* obj, const char* field, ColorRGBA* color) {
    json_object* array = json_object_object_get(obj, field);
    if (array && json_object_is_type(array, json_type_array) && 
        json_object_array_length(array) >= 4) {
        color->red = (float)json_object_get_double(json_object_array_get_idx(array, 0));
        color->green = (float)json_object_get_double(json_object_array_get_idx(array, 1));
        color->blue = (float)json_object_get_double(json_object_array_get_idx(array, 2));
        color->alpha = (float)json_object_get_double(json_object_array_get_idx(array, 3));
    }
}

static void parse_room_connections(json_object* item, RoomConfig* cfg) {
    json_object* conns = json_object_object_get(item, "connections");
    if (!conns || !json_object_is_type(conns, json_type_array)) return;

    int m = json_object_array_length(conns);
    cfg->connection_count = m;

    for (int d = 0; d < DIR_COUNT; d++) {
        cfg->connections[d].room[0] = '\0';
        cfg->connections[d].dir = d;
    }

    for (int c = 0; c < m; c++) {
        json_object* conn = json_object_array_get_idx(conns, c);
        
        const char* peer = json_object_get_string(json_object_object_get(conn, "room"));
        const char* dir_str = json_object_get_string(json_object_object_get(conn, "dir"));
        
        Direction dir = DIR_NORTH;
        if (dir_str) {
            if (strcmp(dir_str, "north") == 0)  dir = DIR_NORTH;
            else if (strcmp(dir_str, "east") == 0) dir = DIR_EAST;
            else if (strcmp(dir_str, "south") == 0) dir = DIR_SOUTH;
            else if (strcmp(dir_str, "west") == 0) dir = DIR_WEST;
        }
        
        if (peer) {
            strncpy(cfg->connections[dir].room, peer, sizeof(cfg->connections[dir].room) - 1);
            cfg->connections[dir].dir = dir;
        }
    }
}

void read_room_config(const char* filename, RoomConfig* room_configs, int* room_count) {
    json_object* root = parse_json_file(filename);
    if (!root || !json_object_is_type(root, json_type_array)) {
        if (root) json_object_put(root);
        return;
    }

    int n = json_object_array_length(root);
    for (int i = 0; i < n && *room_count < MAX_ROOMS; i++) {
        json_object* item = json_object_array_get_idx(root, i);
        RoomConfig cfg = {0};

        json_get_string_field(item, "name", cfg.name, sizeof(cfg.name));
        json_get_vec3_field(item, "dimension", &cfg.dimension);
        json_get_string_field(item, "floor_tex", cfg.floor_tex_path, sizeof(cfg.floor_tex_path));
        json_get_string_field(item, "ceiling_tex", cfg.ceiling_tex_path, sizeof(cfg.ceiling_tex_path));
        json_get_string_field(item, "wall_tex", cfg.wall_tex_path, sizeof(cfg.wall_tex_path));

        parse_room_connections(item, &cfg);

        room_configs[(*room_count)++] = cfg;
    }

    json_object_put(root);
}

void read_object_config(const char* filename, ObjectConfig* object_configs, int* object_count) {
    json_object* root = parse_json_file(filename);
    if (!root || !json_object_is_type(root, json_type_array)) {
        if (root) json_object_put(root);
        return;
    }

    int n = json_object_array_length(root);
    for (int i = 0; i < n && *object_count < MAX_OBJECTS; i++) {
        json_object* item = json_object_array_get_idx(root, i);
        ObjectConfig cfg = {0};

        json_get_string_field(item, "name", cfg.name, sizeof(cfg.name));
        json_get_string_field(item, "model_path", cfg.model_path, sizeof(cfg.model_path));
        json_get_string_field(item, "texture_path", cfg.texture_path, sizeof(cfg.texture_path));
        json_get_string_field(item, "room_name", cfg.room_name, sizeof(cfg.room_name));

        json_get_vec3_field(item, "offset", &cfg.offset);
        json_get_vec3_field(item, "rotation", &cfg.rotation);
        json_get_vec3_field(item, "scale", &cfg.scale);

        cfg.mass = json_get_float_field(item, "mass", 0.0f);
        cfg.value = json_get_int_field(item, "value", 0);
        cfg.is_static = json_get_bool_field(item, "is_static", false);

        object_configs[(*object_count)++] = cfg;
    }

    json_object_put(root);
}

void read_light_config(const char* filename, Lighting* light_configs, int* light_count) {
    json_object* root = parse_json_file(filename);
    if (!root || !json_object_is_type(root, json_type_array)) {
        if (root) json_object_put(root);
        return;
    }

    int n = json_object_array_length(root);
    for (int i = 0; i < n && *light_count < MAX_LIGHTS; i++) {
        json_object* item = json_object_array_get_idx(root, i);
        Lighting cfg = {0};

        cfg.enabled = true;
        cfg.slot = -1;

        json_get_string_field(item, "name", cfg.name, sizeof(cfg.name));
        cfg.brightness = json_get_float_field(item, "brightness", 1.0f);

        json_get_rgba_field(item, "ambient", &cfg.ambient);
        json_get_rgba_field(item, "diffuse", &cfg.diffuse);
        json_get_rgba_field(item, "specular", &cfg.specular);

        cfg.is_spotlight = false;
        cfg.cutoff = 180.0f;
        cfg.exponent = 0.0f;
        cfg.direction = (Vec3){0.0f, -1.0f, 0.0f};
        cfg.room_name[0] = '\0';

        if (strcmp(cfg.name, "global") == 0) {
            json_get_vec4_field(item, "position", &cfg.position);
            cfg.is_spotlight = false;
        }
        else {
            cfg.is_spotlight = true;
            
            json_get_string_field(item, "room", cfg.room_name, sizeof(cfg.room_name));
            
            json_object* direction_obj = json_object_object_get(item, "direction");
            if (direction_obj && json_object_is_type(direction_obj, json_type_array)) {
                json_get_vec3_field(item, "direction", &cfg.direction);
            }
            
            cfg.cutoff = json_get_float_field(item, "cutoff", 30.0f);
            cfg.exponent = json_get_float_field(item, "exponent", 10.0f);
            
            cfg.position = (Vec4){0.0f, 0.0f, 0.0f, 1.0f};
        }

        light_configs[(*light_count)++] = cfg;
    }

    json_object_put(root);
}

char* read_manual(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("[ERROR] Cannot open manual file\n");
        return NULL;
    }
    
    char** lines = NULL;
    int line_count = 0;
    size_t max_line_length = 0;
    
    char line_buffer[1024];
    while (fgets(line_buffer, sizeof(line_buffer), file)) {
        size_t len = strlen(line_buffer);
        
        if (len > 0 && line_buffer[len - 1] == '\n') {
            line_buffer[len - 1] = '\0';
            len--;
        }
        
        if (len > max_line_length) {
            max_line_length = len;
        }
        
        lines = realloc(lines, (line_count + 1) * sizeof(char*));
        lines[line_count] = strdup(line_buffer);
        line_count++;
    }
    
    size_t total_size = (max_line_length + 1) * line_count + 1;
    
    char* result = malloc(total_size);
    if (!result) {
        for (int i = 0; i < line_count; i++) {
            free(lines[i]);
        }
        free(lines);
        fclose(file);
        return NULL;
    }
    
    char* current = result;
    
    for (int i = line_count - 1; i >= 0; i--) {
        int line_len = strlen(lines[i]);
        
        strcpy(current, lines[i]);
        current += line_len;
        
        for (size_t j = line_len; j < max_line_length; j++) {
            *current++ = ' ';
        }
        
        *current++ = '\n';
        
        free(lines[i]);
    }
    
    *current = '\0';
    
    free(lines);
    fclose(file);
    return result;
}

void print_room_configs(RoomConfig* room_config, int room_count) {
    printf("Loaded %d room configurations:\n\n", room_count);
    
    for (int i = 0; i < room_count; i++) {
        RoomConfig config = room_config[i];
        
        printf("Room %d:\n", i + 1);
        printf("  Name: %s\n", config.name);
        printf("  Dimensions: [%.3f, %.3f, %.3f]\n", 
               config.dimension.x, config.dimension.y, config.dimension.z);
        printf("  Floor Texture: %s\n", 
               strlen(config.floor_tex_path) > 0 ? config.floor_tex_path : "(none)");
        printf("  Ceiling Texture: %s\n", 
               strlen(config.ceiling_tex_path) > 0 ? config.ceiling_tex_path : "(none)");
        printf("  Wall Texture: %s\n", 
               strlen(config.wall_tex_path) > 0 ? config.wall_tex_path : "(none)");
        
        printf("  Connections (%d):\n", config.connection_count);
        if (config.connection_count > 0) {
            for (int c = 0; c < config.connection_count; c++) {
                const char* dir_str = "";
                switch (config.connections[c].dir) {
                    case 0: dir_str = "north"; break;
                    case 1: dir_str = "east"; break;
                    case 2: dir_str = "south"; break;
                    case 3: dir_str = "west"; break;
                    default: dir_str = "unknown"; break;
                }
                printf("    - %s (%s)\n", config.connections[c].room, dir_str);
            }
        }
        else {
            printf("    (none)\n");
        }

        printf("\n");
    }
}

void print_object_configs(ObjectConfig* obj_config, int config_count) {
    printf("Loaded %d model configurations:\n\n", config_count);
    
    for (int i = 0; i < config_count; i++) {
        ObjectConfig config = obj_config[i];
        
        printf("Model %d:\n", i + 1);
        printf("  Name: %s\n", config.name);
        printf("  Model Path: %s\n", config.model_path);
        
        if (strlen(config.texture_path) > 0) {
            printf("  Texture Path: %s\n", config.texture_path);
        }
        else {
            printf("  Texture Path: (none)\n");
        }
        
        printf("  Offset: [%.3f, %.3f, %.3f]\n", 
               config.offset.x, config.offset.y, config.offset.z);
        printf("  Rotation: [%.3f, %.3f, %.3f]\n", 
               config.rotation.x, config.rotation.y, config.rotation.z);
        printf("  Scale: [%.3f, %.3f, %.3f]\n", 
               config.scale.x, config.scale.y, config.scale.z);
        printf("  Mass: %f\n", config.mass); 
        printf("  Value: %d\n", config.value);
        printf("  Room: %s\n", config.room_name);
        printf("  Static: %s\n", config.is_static ? "true" : "false");
        printf("\n");
    }
}

void print_light_configs(Lighting* light_config, int light_config_count) {
    printf("Loaded %d light configurations:\n\n", light_config_count);
    
    for (int i = 0; i < light_config_count; i++) {
        Lighting config = light_config[i];
        
        printf("Light %d:\n", i + 1);
        printf("  Name: %s\n", config.name);
        printf("  Type: %s\n", config.is_spotlight ? "Spotlight" : "Global");
        printf("  Ambient: [%.2f, %.2f, %.2f, %.2f]\n", 
               config.ambient.red, config.ambient.green, 
               config.ambient.blue, config.ambient.alpha);
        printf("  Diffuse: [%.2f, %.2f, %.2f, %.2f]\n", 
               config.diffuse.red, config.diffuse.green, 
               config.diffuse.blue, config.diffuse.alpha);
        printf("  Specular: [%.2f, %.2f, %.2f, %.2f]\n", 
               config.specular.red, config.specular.green, 
               config.specular.blue, config.specular.alpha);
        printf("  Position: [%.2f, %.2f, %.2f, %.2f]\n", 
               config.position.x, config.position.y, 
               config.position.z, config.position.w);
        printf("  Brightness: %.2f\n", config.brightness);
        
        if (config.is_spotlight) {
            printf("  Direction: [%.2f, %.2f, %.2f]\n",
                   config.direction.x, config.direction.y, config.direction.z);
            printf("  Cutoff: %.2f degrees\n", config.cutoff);
            printf("  Exponent: %.2f\n", config.exponent);
            printf("  Room: %s\n", config.room_name);
        }
        
        printf("\n");
    }
}