#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

char* trim(char* str) {
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    
    char* end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    *(end+1) = 0;
    
    return str;
}

bool parse_bool(const char* str) {
    return (strcmp(str, "true") == 0 || strcmp(str, "1") == 0);
}

int parse_csv_line(char* line, char* tokens[], int max_tokens) {
    int count = 0;
    char* token = strtok(line, ",");
    
    while (token != NULL && count < max_tokens) {
        tokens[count++] = trim(token);
        token = strtok(NULL, ",");
    }
    
    return count;
}

// Helper to parse a Vec3 from tokens
Vec3 parse_vec3(char* tokens[], int start_idx, Vec3 defaults) {
    Vec3 v = defaults;
    if (tokens[start_idx]) v.x = atof(tokens[start_idx]);
    if (tokens[start_idx+1]) v.y = atof(tokens[start_idx+1]);
    if (tokens[start_idx+2]) v.z = atof(tokens[start_idx+2]);
    return v;
}

// Helper to parse a Vec4 from tokens
Vec4 parse_vec4(char* tokens[], int start_idx, Vec4 defaults) {
    Vec4 v = defaults;
    if (tokens[start_idx]) v.x = atof(tokens[start_idx]);
    if (tokens[start_idx+1]) v.y = atof(tokens[start_idx+1]);
    if (tokens[start_idx+2]) v.z = atof(tokens[start_idx+2]);
    if (tokens[start_idx+3]) v.w = atof(tokens[start_idx+3]);
    return v;
}

// Helper to parse a Color from tokens
ColorRGBA parse_color(char* tokens[], int start_idx, ColorRGBA defaults) {
    ColorRGBA c = defaults;
    if (tokens[start_idx]) c.red = atof(tokens[start_idx]);
    if (tokens[start_idx+1]) c.green = atof(tokens[start_idx+1]);
    if (tokens[start_idx+2]) c.blue = atof(tokens[start_idx+2]);
    if (tokens[start_idx+3]) c.alpha = atof(tokens[start_idx+3]);
    return c;
}

void read_object_config(const char* filename, ObjectConfig* obj_config, int* obj_config_count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open model config file");
        return;
    }

    char line[1024];
    int line_num = 0;
    char* tokens[32]; // Enough for all fields
    
    Vec3 zero_vec = (Vec3){0, 0, 0};
    Vec3 unit_vec = (Vec3){1, 1, 1};

    while (fgets(line, sizeof(line), file)) {
        line_num++;
        if (line_num == 1 || strlen(trim(line)) == 0) continue;
        
        int token_count = parse_csv_line(line, tokens, 32);
        if (token_count < 5) continue;
        
        ObjectConfig config = {0};
        
        strncpy(config.name, tokens[0], sizeof(config.name)-1);
        strncpy(config.model_path, tokens[1], sizeof(config.model_path)-1);
        
        if (tokens[2] && strcmp(tokens[2], "NA") != 0) {
            strncpy(config.texture_path, tokens[2], sizeof(config.texture_path)-1);
        }
        
        config.position = parse_vec3(tokens, 3, zero_vec);
        config.rotation = parse_vec3(tokens, 6, zero_vec);
        config.scale = parse_vec3(tokens, 9, unit_vec);
        
        if (token_count > 12) {
            config.is_static = parse_bool(tokens[12]);
        }
        
        obj_config[(*obj_config_count)++] = config;
    }
    
    fclose(file);
}

void read_light_config(const char* filename, Lighting* light_config, int* light_config_count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open light config file");
        return;
    }

    char line[1024];
    int line_num = 0;
    char* tokens[32];
    
    ColorRGBA default_color = (ColorRGBA){1, 1, 1, 1};
    Vec4 default_position = (Vec4){0, 0, 0, 1};

    while (fgets(line, sizeof(line), file)) {
        line_num++;
        if (line_num == 1 || strlen(trim(line)) == 0) continue;
        
        int token_count = parse_csv_line(line, tokens, 32);
        if (token_count < 16) continue;
        
        Lighting config = {0};
        
        config.ambient = parse_color(tokens, 0, default_color);
        config.diffuse = parse_color(tokens, 4, default_color);
        config.specular = parse_color(tokens, 8, default_color);
        config.position = parse_vec4(tokens, 12, default_position);
        
        if (token_count > 16) {
            config.brightness = atof(tokens[16]);
        }
        
        light_config[(*light_config_count)++] = config;
    }
    
    fclose(file);
}

void print_object_configs(ObjectConfig* obj_config, int obj_config_count) {
    printf("Loaded %d model configurations:\n\n", obj_config_count);
    
    for (int i = 0; i < obj_config_count; i++) {
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
        
        printf("  Position: [%.3f, %.3f, %.3f]\n", 
               config.position.x, config.position.y, config.position.z);
        printf("  Rotation: [%.3f, %.3f, %.3f]\n", 
               config.rotation.x, config.rotation.y, config.rotation.z);
        printf("  Scale: [%.3f, %.3f, %.3f]\n", 
               config.scale.x, config.scale.y, config.scale.z);
        printf("  Static: %s\n", config.is_static ? "true" : "false");
        printf("\n");
    }
}

void print_light_configs(Lighting* light_config, int light_config_count) {
    printf("Loaded %d light configurations:\n\n", light_config_count);
    
    for (int i = 0; i < light_config_count; i++) {
        Lighting config = light_config[i];
        
        printf("Light %d:\n", i + 1);
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
        printf("\n");
    }
}