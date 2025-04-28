#ifndef CONFIG_H
#define CONFIG_H

#include "utils.h"

/**
 * Read the configuration CSV for objects.
 */
void read_object_config(const char* filename, ObjectConfig* obj_config, int* obj_config_count);

/**
 * Read the configuration CSV for lights.
 */
void read_light_config(const char* filename, Lighting* light_config, int* light_config_count);

/**
 * Verifying configs.
 */
void print_object_configs(ObjectConfig* obj_config, int obj_config_count);

void print_light_configs(Lighting* light_config, int light_config_count);

#endif