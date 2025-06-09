#include "lighting.h"
#include "scene.h"
#include <string.h>
#include <stdio.h>
#include <GL/gl.h>
#include "utils.h"

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
        glLightf(light_enum, GL_SPOT_CUTOFF, light->cutoff);
        glLightf(light_enum, GL_SPOT_EXPONENT, light->exponent);
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

void adjust_brightness(Scene* scene, float amount) {
    float brightness;
    for (int i = 0; i < scene->light_count; i++) {
        if (!scene->lights[i].enabled) continue;

        brightness = scene->lights[i].brightness + amount;
        scene->lights[i].brightness = fminf(fmaxf(brightness, 0.1f), 2.0f);
    }
}