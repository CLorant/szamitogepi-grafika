#include <SDL2/SDL.h>
#include <stdio.h>
#include <GL/gl.h>
#include <ctype.h>
#include "text.h"

const float TEXTURE_WIDTH = 512.0f;
const int CHAR_WIDTH = 32;
const int CHAR_HEIGHT = 64;
const int CHARS_PER_ROW = 16;

void init_font_texture_coords(Texture* texture) {
    float char_width = CHAR_WIDTH / TEXTURE_WIDTH;
    float char_height = CHAR_HEIGHT / TEXTURE_WIDTH;

    for (int i = 0; i < 128; i++) {
        if (i < CHAR_WIDTH) {
            texture->font_texture_coords[i] = (CharUV){0.0f, 0.0f, 0.0f, 0.0f};
        }
        else {
            int row = (i - CHAR_WIDTH) / CHARS_PER_ROW + 2;
            int col = (i - CHAR_WIDTH) % CHARS_PER_ROW;
            texture->font_texture_coords[i] = (CharUV){
                col * char_width,
                row * char_height,
                char_width,
                char_height
            };
        }
    }
}

GLuint load_texture(const char* filename) {
    SDL_Surface* surface = SDL_LoadBMP(filename);
    if (!surface) {
        printf("Failed to load texture: %s\n", SDL_GetError());
        return 0;
    }

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(surface);
    return texture_id;
}

void render_char(Texture* texture, float x, float y, unsigned char c) {
    if (!isprint(c)) {
        return;
    }

    CharUV uv = texture->font_texture_coords[(unsigned char)c];

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture->font_texture_id);

    glBegin(GL_QUADS);
    glTexCoord2f(uv.u, uv.v); glVertex2f(x, y);
    glTexCoord2f(uv.u + uv.w, uv.v); glVertex2f(x + CHAR_WIDTH, y);
    glTexCoord2f(uv.u + uv.w, uv.v + uv.h); glVertex2f(x + CHAR_WIDTH, y + CHAR_HEIGHT);
    glTexCoord2f(uv.u, uv.v + uv.h); glVertex2f(x, y + CHAR_HEIGHT);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void render_text(Texture* texture, float x, float y, const char* text) {
    float start_x = x;
    for (int i = 0; text[i] != '\0'; i++) {
        render_char(texture, start_x, y, (unsigned char)text[i]);
        start_x += CHAR_WIDTH;
    }
}