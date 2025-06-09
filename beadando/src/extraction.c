#include "extraction.h"
#include "scene.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "physics.h"

void init_extraction(Scene* scene) {
    Lighting* start_light = find_light_by_name(scene, "start_light");

    memset(&scene->extraction, 0, sizeof(Extraction));
    
    scene->extraction.object = (Object**)calloc(2, sizeof(Object*));
    scene->extraction.object[0] = find_object_by_name(scene, "extraction_platform");
    scene->extraction.object[1] = find_object_by_name(scene, "extraction_screen");
    
    if (!scene->extraction.object[0] || !scene->extraction.object[1]) {
        printf("[ERROR]: Could not find extraction objects!\n");
        return;
    }
    
    scene->extraction.collection_zone_center = scene->extraction.object[0]->position;
    scene->extraction.collection_zone_center.z += 0.5f;
    scene->extraction.collection_radius = 2.0f;
    scene->extraction.collected_value = 0;
    scene->extraction.is_completed = false;
    scene->extraction.target_light = start_light;
    scene->extraction.target_percentage = 60 + (rand() % 31);
    scene->extraction.font_texture_id = load_texture("assets/textures/charmap.png");
    printf("Target: %d%% of total object value\n", scene->extraction.target_percentage);
}


bool is_object_in_extraction_zone(const Extraction* extraction, const Object* obj) {
    if (!obj->is_active || obj->is_static) return false;

    Vec3 distance = vec3_substract(obj->position, extraction->collection_zone_center);
    float dist_squared = vec3_dot(distance, distance);
    return dist_squared <= (extraction->collection_radius * extraction->collection_radius);
}

int calculate_total_object_value(const Scene* scene) {
    int total = 0;
    for (int i = 0; i < scene->object_count; i++) {
        if (scene->objects[i].is_active && !scene->objects[i].is_static) {
            total += scene->objects[i].value;
        }
    }
    return total;
}

void set_extraction_light_status(Extraction* extraction, bool success) {
    if (!extraction->target_light) return;

    if (success) {
        extraction->target_light->diffuse.red = 0.0f;
        extraction->target_light->diffuse.green = 1.0f;
        extraction->target_light->diffuse.blue = 0.0f;
        printf("Extraction successful! Light turned green.\n");
    }
    else {
        extraction->target_light->diffuse.red = 1.0f;
        extraction->target_light->diffuse.green = 0.0f;
        extraction->target_light->diffuse.blue = 0.0f;
    }
}

void update_extraction(Scene* scene) {
    Extraction* extraction = &scene->extraction;

    int current_value = 0;
    for (int i = 0; i < scene->object_count; i++) {
        scene->objects[i].in_extraction = is_object_in_extraction_zone(extraction, &scene->objects[i]);
        if (scene->objects[i].in_extraction) {
            current_value += scene->objects[i].value;
        }
    }

    extraction->collected_value = current_value;

    int total_value = calculate_total_object_value(scene);
    int target_value = (total_value * extraction->target_percentage) / 100;

    bool was_completed = extraction->is_completed;
    extraction->is_completed = (current_value >= target_value);

    if (extraction->is_completed != was_completed) {
        set_extraction_light_status(extraction, extraction->is_completed);
    }
}
