#include "scene.h"
#include <SDL2/SDL.h>
#include <obj/load.h>
#include <obj/draw.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glu.h>

// Temp config, will be loaded from a file
static const ObjectConfig MODEL_CONFIGS[] = {
    {
        .name = "cat",
        .model_path = "assets/models/cat.obj",
        .position = {0, 2, 0},
        .rotation = {90, 0, 0},
        .is_static = false
    },
    {
        .name = "cube",
        .model_path = "assets/models/cube.obj",
        .texture_path = "assets/textures/cube.png",
        .position = {2, 0, 1},
        .scale = {0.5, 0.5, 0.5},
        .is_static = false
    },
    {
        .name = "deer",
        .model_path = "assets/models/deer.obj",
        .position = {2, 2, 0},
        .rotation = {90, 0, 0},
        .scale = {0.15, 0.15, 0.15},
        .is_static = false
    },
    {
        .name = "duck",
        .model_path = "assets/models/duck.obj",
        .position = {0, -2, 0},
        .scale = {0.25, 0.25, 0.25},
        .is_static = false
    },
    {
        .name = "gun",
        .model_path = "assets/models/gun.obj",
        .position = {-4, -4, 1},
        .rotation = {90, 0, 0},
        .scale = {0.25, 0.25, 0.25},
        .is_static = false
    },
    {
        .name = "hare",
        .model_path = "assets/models/hare.obj",
        .position = {-4, 0, 0},
        .scale = {0.05, 0.05, 0.05},
        .is_static = false
    },
    {
        .name = "house",
        .model_path = "assets/models/house.obj",
        .position = {10, 0, 0},
        .rotation = {90, 0, 0},
        .scale = {0.025, 0.025, 0.025},
        .is_static = true
    },
    {
        .name = "porsche",
        .model_path = "assets/models/porsche.obj",
        .position = {0, -4, 0.5},
        .rotation = {90, 90, 0},
        .scale = {0.75, 0.75, 0.75},
        .is_static = false
    },
    {
        .name = "raptor",
        .model_path = "assets/models/raptor.obj",
        .position = {-2, 0, 0},
        .scale = {1.5, 1.5, 1.5},
        .is_static = false
    }
};

// Temp config, will be loaded from a file
static const Lighting LIGHT_CONFIGS[] = {
    {
        .ambient = {
            .red = 1.0,
            .green = 0.0,
            .blue = 0.0,
            .alpha = 1.0
        },
        .diffuse = {
            .red = 0.8,
            .green = 0.8,
            .blue = 0.8,
            .alpha = 1.0
        },
        .specular = {
            .red = 0.0,
            .green = 0.0,
            .blue = 1.0,
            .alpha = 1.0
        },
        .position = {
            .x = 1.0,
            .y = 5.0,
            .z = 10.0,
            .w = 1.0
        },
        .brightness = 0.8
    }
};

void init_scene(Scene* scene) {
    set_rgb(&scene->material.ambient, 0.0, 0.0, 0.0);
    set_rgb(&scene->material.diffuse, 0.8, 0.8, 0.8);
    set_rgb(&scene->material.specular, 0.0, 0.0, 0.0);
    scene->material.shininess = 80.0;

    scene->max_lights = 16;
    scene->lights = (Lighting*)malloc(scene->max_lights * sizeof(Lighting));
    scene->light_count = 0;
    for (size_t i = 0; i < sizeof(LIGHT_CONFIGS) / sizeof(LIGHT_CONFIGS[0]); i++) {
        add_light(scene, &LIGHT_CONFIGS[i]);
    }

    scene->max_objects = 64;
    scene->objects = (Object*)malloc(scene->max_objects * sizeof(Object));
    scene->object_count = 0;
    scene->selected_object_id = -1;
    for (size_t i = 0; i < sizeof(MODEL_CONFIGS) / sizeof(MODEL_CONFIGS[0]); i++) {
        add_object(scene, &MODEL_CONFIGS[i]);
    }

    printf("Scene initialized with %d objects\n", scene->object_count);
}

void add_light(Scene* scene, const Lighting* config) {
    if (scene->light_count >= scene->max_lights) {
        scene->max_lights *= 2;
        scene->lights = realloc(scene->lights, scene->max_lights * sizeof(Lighting));
    }
    Lighting new_light = *config;
    new_light.enabled  = true;
    new_light.slot = GL_LIGHT0 + scene->light_count;
    scene->lights[scene->light_count] = new_light;

    glEnable(GL_LIGHT0 + new_light.slot);
    scene->light_count++;
}

void add_object(Scene* scene, const ObjectConfig* config) {
    if (scene->object_count >= scene->max_objects) {
        scene->max_objects *= 2;
        Object* new_objects = (Object*)realloc(scene->objects, scene->max_objects * sizeof(Object));
        scene->objects = new_objects;
    }

    Object* obj = &scene->objects[scene->object_count];
    memset(obj, 0, sizeof(Object));

    obj->id = scene->object_count;
    strncpy(obj->name, config->name, sizeof(obj->name) - 1);
    obj->name[sizeof(obj->name) - 1] = '\0';
    obj->material = &scene->material;
    obj->is_active = true;
    obj->is_static = config->is_static;

    transform_init(&obj->static_transform);
    transform_init(&obj->anim_transform);

    obj->static_transform.position = config->position;
    obj->static_transform.rotation = config->rotation;
    obj->static_transform.scale = config->scale;

    if (obj->static_transform.scale.x == 0 && obj->static_transform.scale.y == 0 && obj->static_transform.scale.z == 0) {
        obj->static_transform.scale.x = 1.0f;
        obj->static_transform.scale.y = 1.0f;
        obj->static_transform.scale.z = 1.0f;
    }

    load_model(&obj->model, config->model_path);
    obj->texture_id = (config->texture_path != NULL) ? load_texture(config->texture_path) : 0;

    calculate_bounding_box(obj);
    printf("Calculated bounding box for %s:\nMin: (%f, %f, %f)\nMax: (%f, %f, %f)\n",
        config->name,
        obj->bounding.min.x, obj->bounding.min.y, obj->bounding.min.z,
        obj->bounding.max.x, obj->bounding.max.y, obj->bounding.max.z);

    obj->display_list = glGenLists(1);
    glNewList(obj->display_list, GL_COMPILE);
    draw_model(&obj->model);
    glEndList();

    scene->object_count++;

    printf("Object added to scene!\nID: %d\nName: %s\n\n", obj->id, config->name);
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
    for (int i = 0; i < scene->light_count; i++) {
        scene->lights[i].brightness = SDL_clamp(scene->lights[i].brightness + amount, 0.1f, 2.0f);
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
        light->diffuse.red,
        light->diffuse.green,
        light->diffuse.blue,
        light->ambient.alpha
    };

    float specular_light[] = {
        light->specular.red,
        light->specular.green,
        light->specular.blue,
        light->ambient.alpha
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
}

void set_material(const Material* material) {
    float ambient_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene* scene, double elapsed_time) {
    static float total_time = 0.0f;
    total_time += elapsed_time;

    for (int i = 0; i < scene->light_count; i++) {
        if (scene->lights[i].enabled) {
            float brightness = scene->lights[i].brightness;
            ColorRGB anim = sine_animate_color(total_time, i, brightness);

            scene->lights[i].diffuse.red = anim.red;
            scene->lights[i].diffuse.green = anim.green;
            scene->lights[i].diffuse.blue = anim.blue;
        }
    }

    static float angle = 0.0f;
    angle += 30.0f * elapsed_time;

    for (int i = 0; i < scene->object_count; i++) {
        Object* obj = &scene->objects[i];

        if (!obj->is_active || scene->selected_object_id == obj->id) {
            continue;
        }

        reset_rotation(&obj->anim_transform);
        set_rotation(&obj->anim_transform, 0, 0, angle);
    }
}

void render_scene(const Scene* scene) {
    glDisable(GL_LIGHTING);
    draw_origin(1);
    draw_checkerboard(4, 1);
    glEnable(GL_LIGHTING);

    set_material(&scene->material);

    for (int i = 0; i < scene->light_count; i++) {
        if (scene->lights[i].enabled) {
            glEnable(GL_LIGHT0 + scene->lights[i].slot);
            set_lighting(i, &scene->lights[i]);
        }
        else {
            glDisable(GL_LIGHT0 + scene->lights[i].slot);
        }
    }

    for (int i = 0; i < scene->object_count; i++) {
        Object* obj = &scene->objects[i];
        if (!obj->is_active) continue;

        glPushMatrix();

        glTranslatef(
            obj->static_transform.position.x,
            obj->static_transform.position.y,
            obj->static_transform.position.z
        );

        if (!obj->is_static) {
            glTranslatef(
                obj->anim_transform.position.x,
                obj->anim_transform.position.y,
                obj->anim_transform.position.z
            );
            glRotatef(obj->anim_transform.rotation.x, 1, 0, 0);
            glRotatef(obj->anim_transform.rotation.y, 0, 1, 0);
            glRotatef(obj->anim_transform.rotation.z, 0, 0, 1);
            glScalef(
                obj->anim_transform.scale.x,
                obj->anim_transform.scale.y,
                obj->anim_transform.scale.z
            );
        }

        glRotatef(obj->static_transform.rotation.x, 1, 0, 0);
        glRotatef(obj->static_transform.rotation.y, 0, 1, 0);
        glRotatef(obj->static_transform.rotation.z, 0, 0, 1);
        glScalef(
            obj->static_transform.scale.x,
            obj->static_transform.scale.y,
            obj->static_transform.scale.z
        );

        glBindTexture(GL_TEXTURE_2D, obj->texture_id);
        glCallList(obj->display_list);

        if (scene->selected_object_id == obj->id) {
            draw_bounding_box(obj);
        }

        glPopMatrix();
    }
}

bool slab_hit(float origin, float dir, float min_box, float max_box, float* tmin, float* tmax) {
    float inv_dir = 1.0f / dir;
    float t0 = (min_box - origin) * inv_dir;
    float t1 = (max_box - origin) * inv_dir;
    if (inv_dir < 0.0f) {
        float tmp = t0;
        t0 = t1;
        t1 = tmp;
    }
    *tmin = fmaxf(t0, *tmin);
    *tmax = fminf(t1, *tmax);

    return *tmax > *tmin;
}

int select_object_at(Scene* scene, Camera* camera, int mouse_x, int mouse_y) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    float normalized_x = (2.0f * mouse_x) / viewport[2] - 1.0f;
    float normalized_y = 1.0f - (2.0f * mouse_y) / viewport[3];
    float h_angle = degree_to_radian(camera->rotation.z);
    float v_angle = degree_to_radian(camera->rotation.x);

    Vec3 forward = {
        .x = cosf(h_angle) * cosf(v_angle),
        .y = sinf(h_angle) * cosf(v_angle),
        .z = sinf(v_angle)
    };

    Vec3 right = {
        .x = -sinf(h_angle),
        .y = cosf(h_angle),
        .z = 0
    };

    Vec3 up = {
        .x = -cosf(h_angle) * sinf(v_angle),
        .y = -sinf(h_angle) * sinf(v_angle),
        .z = cosf(v_angle)
    };

    float fov_h = 0.16f;
    float fov_v = 0.12f;

    Vec3 rayDir = {
        .x = forward.x + normalized_x * right.x * fov_h + normalized_y * up.x * fov_v,
        .y = forward.y + normalized_x * right.y * fov_h + normalized_y * up.y * fov_v,
        .z = forward.z + normalized_x * right.z * fov_h + normalized_y * up.z * fov_v
    };
    vec3_normalize(&rayDir);

    Vec3 rayOrigin = camera->position;
    float closest_dist = INFINITY;
    int hit_id = -1;

    for (int i = 0; i < scene->object_count; i++) {
        Object* obj = &scene->objects[i];
        if (!obj->is_active || obj->is_static) continue;

        BoundingBox box = obj->bounding;
        box.min = vec3_multiply(box.min, obj->static_transform.scale);
        box.max = vec3_multiply(box.max, obj->static_transform.scale);
        box.min = vec3_add(box.min, obj->static_transform.position);
        box.max = vec3_add(box.max, obj->static_transform.position);

        float tmin = 0.0f;
        float tmax = 100000.0f;

        if (!slab_hit(rayOrigin.x, rayDir.x, box.min.x, box.max.x, &tmin, &tmax)) continue;
        if (!slab_hit(rayOrigin.y, rayDir.y, box.min.y, box.max.y, &tmin, &tmax)) continue;
        if (!slab_hit(rayOrigin.z, rayDir.z, box.min.z, box.max.z, &tmin, &tmax)) continue;

        if (tmin < closest_dist) {
            closest_dist = tmin;
            hit_id = obj->id;
        }
    }

    if (fabs(closest_dist) != INFINITY) {
        printf("Selected object: %d (distance: %f)\n", hit_id, closest_dist);
    }
    else {
        printf("No hit object\n");
    }

    return hit_id;
}

void calculate_bounding_box(Object* obj) {
    if (obj->model.n_vertices == 0) return;

    Vertex init_vertex = obj->model.vertices[0];
    obj->bounding.min = obj->bounding.max = (Vec3){
        init_vertex.x,
        init_vertex.y,
        init_vertex.z,
    };

    for (int i = 1; i < obj->model.n_vertices; ++i) {
        Vertex current_vertex = obj->model.vertices[i];
        Vec3 v = (Vec3){
            current_vertex.x, current_vertex.y, current_vertex.z
        };
        obj->bounding.min = vec3_min(obj->bounding.min, v);
        obj->bounding.max = vec3_max(obj->bounding.max, v);
    }
}

void draw_bounding_box(Object* obj) {
    if (!obj->is_active || obj->is_static || obj->model.n_vertices == 0) {
        return;
    }

    BoundingBox box = obj->bounding;
    float min_x = box.min.x, min_y = box.min.y, min_z = box.min.z;
    float max_x = box.max.x, max_y = box.max.y, max_z = box.max.z;

    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.0f);
    glLineWidth(2.0f);

    glBegin(GL_LINE_LOOP);
    glVertex3f(min_x, min_y, min_z);
    glVertex3f(max_x, min_y, min_z);
    glVertex3f(max_x, max_y, min_z);
    glVertex3f(min_x, max_y, min_z);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f(min_x, min_y, max_z);
    glVertex3f(max_x, min_y, max_z);
    glVertex3f(max_x, max_y, max_z);
    glVertex3f(min_x, max_y, max_z);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(min_x, min_y, min_z);
    glVertex3f(min_x, min_y, max_z);
    glVertex3f(max_x, min_y, min_z);
    glVertex3f(max_x, min_y, max_z);
    glVertex3f(max_x, max_y, min_z);
    glVertex3f(max_x, max_y, max_z);
    glVertex3f(min_x, max_y, min_z);
    glVertex3f(min_x, max_y, max_z);
    glEnd();

    glEnable(GL_LIGHTING);
    glLineWidth(1.0f);
}

void rotate_selected_object(Scene* scene, float x_rotation, float y_rotation) {
    Object* obj = find_object_by_id(scene, scene->selected_object_id);
    if (obj == NULL) {
        return;
    }

    Vec3 current_rotation = obj->anim_transform.rotation;
    set_rotation(&obj->anim_transform,
        current_rotation.x + x_rotation,
        current_rotation.y + y_rotation,
        current_rotation.z);

    obj->anim_transform.matrix_dirty = true;
}

void reset_selected_object_rotation(Scene* scene) {
    Object* obj = find_object_by_id(scene, scene->selected_object_id);
    if (obj == NULL) {
        return;
    }

    reset_rotation(&obj->anim_transform);
}

void draw_origin(float size) {
    glBegin(GL_LINES);

    glColor3f(size, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(size, 0, 0);

    glColor3f(0, size, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, size, 0);

    glColor3f(0, 0, size);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, size);

    glEnd();
}

void draw_checkerboard(int size, float square_size) {
    glBegin(GL_QUADS);

    for (int i = -size; i < size; i++) {
        for (int j = -size; j < size; j++) {
            if ((i + j) % 2 == 0) {
                glColor3f(0.9, 0.9, 0.9);
            }
            else {
                glColor3f(0.1, 0.1, 0.1);
            }

            float x = i * square_size;
            float y = j * square_size;

            glVertex3f(x, y, 0);
            glVertex3f(x + square_size, y, 0);
            glVertex3f(x + square_size, y + square_size, 0);
            glVertex3f(x, y + square_size, 0);
        }
    }

    glEnd();
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