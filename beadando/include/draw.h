#ifndef DRAW_H
#define DRAW_H

#include "utils.h"
#include <GL/gl.h>

typedef struct App App;
typedef struct Scene Scene;
typedef struct Object Object;
typedef struct Room Room;
typedef struct PhysicsBody PhysicsBody;

/**
 * Draw a crosshair in the center of the screen.
 */
void draw_crosshair(App* app);

/**
 * Draw the manual.
 */
void draw_manual(App* app);

/**
 * Draw a line of text.
 */
void draw_string(GLuint tex, const char* s, float start_x, float start_y, float line_height);

/**
 * Draw a quad with texture coordinates.
 */
void draw_textured_quad(float normal[3], float tex_coords[4][2], float vertices[4][3]);

/**
 * Draw a room of the scene.
 */
void draw_room(Room* room, Scene* scene);

/**
 * Draw an outline around an object's bounding.
 */
void draw_bounding_box(PhysicsBody* pb);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin(float size);

#endif /* DRAW_H */