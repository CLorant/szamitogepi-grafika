#include "draw.h"
#include "app.h"
#include "scene.h"
#include "physics.h"
#include <string.h>

void draw_crosshair(App* app) {
    float aspect_ratio = app->camera.aspect_ratio;
    float size = 0.02f;
    
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(0.5f, 0.5f, 0.5f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);

    glVertex2f(-size / aspect_ratio, 0.0f);
    glVertex2f(size / aspect_ratio, 0.0f);
    
    glVertex2f(0.0f, -size);
    glVertex2f(0.0f, size);
    
    glEnd();
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void draw_manual(App* app) {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -app->manual.distance);
    glTranslatef(0.0f, -app->manual.scroll, 0.0f);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    draw_string(
        app->manual.charmap_id,
        app->manual.text,
        app->manual.start_x,
        app->manual.start_y,
        app->manual.line_height
    );
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void draw_string(GLuint tex, const char* s, float start_x, float start_y, float line_height) {
    const float FONT_WIDTH = 1.0f / 16.0f;
    const float FONT_HEIGHT = 1.0f / 8.0f;
    const float CHAR_WIDTH = 0.5f;
    const float CHAR_HEIGHT = 1.0f;

    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    float x = start_x, y = start_y;
    for (int i = 0; s[i]; ++i) {
        char c = s[i];
        if (c == '\n') {
            x = start_x;
            y -= line_height;
            continue;
        }

        if (c >= 32 && c < 127) {
            int row = c / 16 + 1;
            int column = c % 16;
            float u = FONT_WIDTH * column;
            float v = FONT_HEIGHT * row;

            glTexCoord2f(u, v);
            glVertex3f(x, y, 0);

            glTexCoord2f(u + FONT_WIDTH, v);
            glVertex3f(x + CHAR_WIDTH, y, 0);

            glTexCoord2f(u + FONT_WIDTH, v - FONT_HEIGHT);
            glVertex3f(x + CHAR_WIDTH, y - CHAR_HEIGHT, 0);

            glTexCoord2f(u, v - FONT_HEIGHT);
            glVertex3f(x, y - CHAR_HEIGHT, 0);

            x += CHAR_WIDTH;
        }
    }
    glEnd();
}

void draw_textured_quad(float tex_coords[4][2], float vertices[4][3]) {
    glBegin(GL_QUADS);

    for (int i = 0; i < 4; i++) {
        glTexCoord2fv(tex_coords[i]);
        glVertex3fv(vertices[i]);
    }

    glEnd();
}

void draw_room(Room* room, Scene* scene) {
    float w = room->dimension.x * 0.5f;
    float l = room->dimension.y * 0.5f;
    float h = room->dimension.z;
    float px = room->position.x;
    float py = room->position.y;
    float pz = room->position.z;

    float door_h = room->door_height;
    float half_w = w * 0.5f;
    float half_d = room->door_width * 0.5f;

    float v1 = door_h / h;
    float u1, u2;

    float solid_uv[4][2] = { {0, 0}, {1, 0}, {1, 1}, {0, 1} };
    float top_uv[4][2] = { {0, v1}, {1, v1}, {1, 1}, {0, 1} };

    glBindTexture(GL_TEXTURE_2D, room->floor_tex);
    draw_textured_quad(
        solid_uv,
        (float[4][3]) {
            { px - w, py - l, pz },
            { px + w, py - l, pz },
            { px + w, py + l, pz },
            { px - w, py + l, pz }
    }
    );

    glBindTexture(GL_TEXTURE_2D, room->ceiling_tex);
    draw_textured_quad(
        solid_uv,
        (float[4][3]) {
            { px - w, py - l, pz + h },
            { px + w, py - l, pz + h },
            { px + w, py + l, pz + h },
            { px - w, py + l, pz + h }
    }
    );

    glBindTexture(GL_TEXTURE_2D, room->wall_tex);
    for (int d = 0; d < DIR_COUNT; d++) {
        if (room->connections[d].room[0] == '\0') {
            float vertices[4][3];

            switch (d) {
            case DIR_NORTH:
                memcpy(vertices, (float[4][3]) {
                    { px - w, py + l, pz },
                    { px + w, py + l, pz },
                    { px + w, py + l, pz + h },
                    { px - w, py + l, pz + h }
                }, sizeof(vertices));
                break;
            case DIR_EAST:
                memcpy(vertices, (float[4][3]) {
                    { px + w, py - l, pz },
                    { px + w, py + l, pz },
                    { px + w, py + l, pz + h },
                    { px + w, py - l, pz + h }
                }, sizeof(vertices));
                break;
            case DIR_SOUTH:
                memcpy(vertices, (float[4][3]) {
                    { px + w, py - l, pz },
                    { px - w, py - l, pz },
                    { px - w, py - l, pz + h },
                    { px + w, py - l, pz + h }
                }, sizeof(vertices));
                break;
            case DIR_WEST:
                memcpy(vertices, (float[4][3]) {
                    { px - w, py + l, pz },
                    { px - w, py - l, pz },
                    { px - w, py - l, pz + h },
                    { px - w, py + l, pz + h }
                }, sizeof(vertices));
                break;
            }

            draw_textured_quad(solid_uv, vertices);
        }
        else {
            int neighbor_idx = room->connections[d].id;
            if (neighbor_idx < 0 || neighbor_idx >= scene->room_count) continue;

            Room* neighbor_room = &scene->rooms[neighbor_idx];
            if (!needs_connector(room, neighbor_room, d)) continue;

            if (d == DIR_NORTH || d == DIR_SOUTH) {
                u1 = (half_w - half_d) / w;
                u2 = (half_w + half_d) / w;
            }
            else {
                u1 = (l - half_d) / (2 * l);
                u2 = (l + half_d) / (2 * l);
            }
            
            float left_uv[4][2] = { {0, 0}, {u1, 0}, {u1, v1}, {0, v1} };
            float right_uv[4][2] = { {u2, 0}, {1, 0}, {1, v1}, {u2, v1} };
            float top_vertices[4][3];
            float left_vertices[4][3];
            float right_vertices[4][3];

            switch (d) {
            case DIR_NORTH:
                memcpy(top_vertices, (float[4][3]) {
                    { px - w, py + l, pz + door_h },
                    { px + w, py + l, pz + door_h },
                    { px + w, py + l, pz + h },
                    { px - w, py + l, pz + h }
                }, sizeof(top_vertices));

                memcpy(left_vertices, (float[4][3]) {
                    { px - w, py + l, pz },
                    { px - half_d, py + l, pz },
                    { px - half_d, py + l, pz + door_h },
                    { px - w, py + l, pz + door_h }
                }, sizeof(left_vertices));

                memcpy(right_vertices, (float[4][3]) {
                    { px + half_d, py + l, pz },
                    { px + w, py + l, pz },
                    { px + w, py + l, pz + door_h },
                    { px + half_d, py + l, pz + door_h }
                }, sizeof(right_vertices));
                break;

            case DIR_EAST:
                memcpy(top_vertices, (float[4][3]) {
                    { px + w, py - l, pz + door_h },
                    { px + w, py + l, pz + door_h },
                    { px + w, py + l, pz + h },
                    { px + w, py - l, pz + h }
                }, sizeof(top_vertices));

                memcpy(left_vertices, (float[4][3]) {
                    { px + w, py - l, pz },
                    { px + w, py - half_d, pz },
                    { px + w, py - half_d, pz + door_h },
                    { px + w, py - l, pz + door_h }
                }, sizeof(left_vertices));

                memcpy(right_vertices, (float[4][3]) {
                    { px + w, py + half_d, pz },
                    { px + w, py + l, pz },
                    { px + w, py + l, pz + door_h },
                    { px + w, py + half_d, pz + door_h }
                }, sizeof(right_vertices));
                break;

            case DIR_SOUTH:
                memcpy(top_vertices, (float[4][3]) {
                    { px + w, py - l, pz + door_h },
                    { px - w, py - l, pz + door_h },
                    { px - w, py - l, pz + h },
                    { px + w, py - l, pz + h }
                }, sizeof(top_vertices));

                memcpy(left_vertices, (float[4][3]) {
                    { px + w, py - l, pz },
                    { px + half_d, py - l, pz },
                    { px + half_d, py - l, pz + door_h },
                    { px + w, py - l, pz + door_h }
                }, sizeof(left_vertices));

                memcpy(right_vertices, (float[4][3]) {
                    { px - half_d, py - l, pz },
                    { px - w, py - l, pz },
                    { px - w, py - l, pz + door_h },
                    { px - half_d, py - l, pz + door_h }
                }, sizeof(right_vertices));
                break;

            case DIR_WEST:
                memcpy(top_vertices, (float[4][3]) {
                    { px - w, py + l, pz + door_h },
                    { px - w, py - l, pz + door_h },
                    { px - w, py - l, pz + h },
                    { px - w, py + l, pz + h }
                }, sizeof(top_vertices));

                memcpy(left_vertices, (float[4][3]) {
                    { px - w, py + l, pz },
                    { px - w, py + half_d, pz },
                    { px - w, py + half_d, pz + door_h },
                    { px - w, py + l, pz + door_h }
                }, sizeof(left_vertices));

                memcpy(right_vertices, (float[4][3]) {
                    { px - w, py - half_d, pz },
                    { px - w, py - l, pz },
                    { px - w, py - l, pz + door_h },
                    { px - w, py - half_d, pz + door_h }
                }, sizeof(right_vertices));
                break;
            }

            draw_textured_quad(top_uv, top_vertices);
            draw_textured_quad(left_uv, left_vertices);
            draw_textured_quad(right_uv, right_vertices);
        }
    }
}

void draw_bounding_box(PhysicsBody* pb) {
    Vec3 C[8];
    if (!physics_get_obb_corners(pb, C)) return;

    static const int E[12][2] = {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    };

    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(1.0f, 1.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < 12; ++i) {
        Vec3 a = C[E[i][0]], b = C[E[i][1]];
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glLineWidth(1.0f);
}

void draw_origin(float size) {
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(size, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, size, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, size);

    glEnd();
}