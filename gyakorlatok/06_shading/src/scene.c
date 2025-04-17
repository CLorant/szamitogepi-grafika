#include "scene.h"

#include <obj/load.h>
#include <obj/draw.h>

void init_scene(Scene* scene)
{
    load_model(&(scene->cube), "assets/models/cube.obj");
    load_model(&(scene->cat), "assets/models/cat.obj");
    scene->texture_id = load_texture("assets/textures/cube.png");

    glBindTexture(GL_TEXTURE_2D, scene->texture_id);

    scene->material.ambient.red = 0.3;
    scene->material.ambient.green = 0.3;
    scene->material.ambient.blue = 0.3;

    scene->material.diffuse.red = 0.8;
    scene->material.diffuse.green = 0.8;
    scene->material.diffuse.blue = 0.8;

    scene->material.specular.red = 1.0;
    scene->material.specular.green = 1.0;
    scene->material.specular.blue = 1.0;

    scene->material.shininess = 80.0;
}

void set_lighting()
{
    float ambient_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float diffuse_light[] = { 1.0f, 1.0f, 1.0, 1.0f };
    float specular_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float position[] = { 1.0f, 5.0f, 10.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material* material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void transform_model(Model* model, float translate[3], float rotate[3], float scale[3]) {
    glPushMatrix();

    if (translate) {
        glTranslatef(translate[0], translate[1], translate[2]);
    }
    
    if (rotate) {
        glRotatef(rotate[0], 1.0f, 0.0f, 0.0f);
        glRotatef(rotate[1], 0.0f, 1.0f, 0.0f);
        glRotatef(rotate[2], 0.0f, 0.0f, 1.0f);
    }
    
    if (scale) {
        glScalef(scale[0], scale[1], scale[2]);
    }

    draw_model(model);
    glPopMatrix();
}

void update_scene(Scene* scene)
{
    
}

void render_scene(const Scene* scene)
{
    set_material(&(scene->material));
    set_lighting();
    draw_origin(1);
    transform_model(&(scene->cat), VEC3(0.5, 0.5, 0.5), NULL, NULL);
    transform_model(&(scene->cube), VEC3(-0.5, -0.5, -0.5), VEC3(45.0, 30.0, 0.0), NULL);
}

void draw_origin(float size)
{
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
