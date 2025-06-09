#ifndef EXTRACTION_H
#define EXTRACTION_H

#include "utils.h"
#include <GL/gl.h>
#include <stdbool.h>

typedef struct Scene Scene;
typedef struct Object Object;
typedef struct Lighting Lighting;

typedef struct Extraction {
    Object** object;
    Vec3 collection_zone_center;
    float collection_radius;
    int target_percentage;
    int collected_value;
    bool is_completed;
    Lighting* target_light;
    GLuint font_texture_id;
} Extraction;

/**
 * Initialze the extraction core values.
 */
void init_extraction(Scene* scene);

/**
 * Update the extraction status based on objects in the collection zone.
 */
void update_extraction(Scene* scene);

/**
 * Check if an object is within the extraction zone.
 */
bool is_object_in_extraction_zone(const Extraction* extraction, const Object* obj);

/**
 * Change the extraction-status light to red / green
 */
void set_extraction_light_status(Extraction* extraction, bool success);

/**
 * Sum up all dynamic, active object values
 */
int calculate_total_object_value(const Scene* scene);

#endif /* EXTRACTION_H */
