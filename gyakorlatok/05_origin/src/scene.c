#include "scene.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

const float ORIGIN_SIZE = 1;

const float TRI_X = 0.2;
const float TRI_Y = 0.2;
const float TRI_Z = 0.2;
const float TRI_SIZE = 0.1;

const float SPHERE_X = 0.5;
const float SPHERE_Y = 0.5;
const float SPHERE_Z = 0.5;
const float SPHERE_RADI = 0.1;

const int SQUARE_COUNT = 4;
const float SQUARE_SIZE = 0.25;

void init_scene(Scene* scene)
{
    scene->sphere_rotation = 0;
}

void update_scene(Scene* scene)
{
    scene->sphere_rotation += 1;
    if (scene->sphere_rotation >= 360) {
        scene->sphere_rotation -= 360;
    }
}

void render_scene(const Scene* scene)
{
    draw_origin(ORIGIN_SIZE);

    draw_triangle(TRI_X, TRI_Y, TRI_Z, TRI_SIZE);

    draw_sphere(SPHERE_X, SPHERE_Y, SPHERE_Z, SPHERE_RADI, scene->sphere_rotation);

    draw_checkerboard(SQUARE_COUNT, SQUARE_SIZE);

    glPushMatrix();
    glTranslatef(-1.5f, 0, 0);
    draw_cylinder(0.3f, 1.0f, 32);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5f, 0, 0);
    draw_cone(0.4f, 1.2f, 32);
    glPopMatrix();
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

void draw_triangle(float x, float y, float z, float size)
{
    glBegin(GL_TRIANGLES);

    glColor3f(1, 1, 0);
    glVertex3f(x, size + y, z);

    glColor3f(0, 1, 1);
    glVertex3f(-size + x, -size + y, z);

    glColor3f(1, 0, 1);
    glVertex3f(size + x, -size + y, z);

    glEnd();
}

void draw_sphere(float x, float y, float z, float radius, float rotation)
{
    glColor3f(0.5, 0.5, 0.5);
    
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotation, 0, 1, 0);
    GLUquadric* quad = gluNewQuadric();
    gluQuadricDrawStyle(quad, GLU_FILL);
    gluSphere(quad, radius, 16, 16);
    gluDeleteQuadric(quad);
    glPopMatrix();
}

void draw_checkerboard(int size, float square_size)
{
    glBegin(GL_QUADS);

    for(int i = -size; i < size; i++) {
        for(int j = -size; j < size; j++) {
            if((i + j) % 2 == 0) {
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

void draw_cylinder(float radius, float height, int segments)
{
    glBegin(GL_TRIANGLE_STRIP);

    glColor3f(0.8f, 0.2f, 0.2f);

    for(int i = 0; i <= segments; i++) {
        float theta = 2.0f * M_PI * i / segments;
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        
        glVertex3f(x, y, 0);
        glVertex3f(x, y, height);
    }

    glEnd();

    glBegin(GL_TRIANGLE_FAN);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, height);

    for(int i = 0; i <= segments; i++) {
        float theta = 2.0f * M_PI * i / segments;
        glVertex3f(radius * cos(theta), radius * sin(theta), height);
    }

    glEnd();
}

void draw_cone(float radius, float height, int segments)
{
    glBegin(GL_TRIANGLE_FAN);

    glColor3f(0.2f, 0.8f, 0.2f);
    glVertex3f(0, 0, height);

    for(int i = 0; i <= segments; i++) {
        float theta = 2.0f * M_PI * i / segments;
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        glVertex3f(x, y, 0);
    }

    glEnd();

    
    glBegin(GL_TRIANGLE_FAN);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);

    for(int i = 0; i <= segments; i++) {
        float theta = 2.0f * M_PI * i / segments;
        glVertex3f(radius * cos(theta), radius * sin(theta), 0);
    }

    glEnd();
}
