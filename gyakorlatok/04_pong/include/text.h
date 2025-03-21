#ifndef TEXT_H
#define TEXT_H
#include <GL/gl.h>

typedef struct CharUV {
    float u, v;  // Top-left corner of the character in the texture
    float w, h;  // Width and height of the character
} CharUV;

typedef struct Texture {
    GLuint font_texture_id;
    CharUV font_texture_coords[128];
} Texture;

void init_font_texture_coords(Texture* texture);

GLuint load_texture(const char* filename);

void render_char(Texture* texture, float x, float y, unsigned char c);

void render_text(Texture* texture, float x, float y, const char* text);

#endif
