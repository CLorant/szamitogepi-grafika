#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include "game.h"
#include "text.h"

const int LEFT_PAD_SPEED = 400;

void init_game(Game* game, int width, int height)
{
    game->is_running = false;
    game->width = width;
    game->height = height;
    if (init_sdl(game) == false) {
        return;
    }
    init_opengl(game);
    init_pong(&(game->pong), width, height);

    Texture font_texture;
    font_texture.font_texture_id = load_texture("assets/textures/charmap.bmp");
    if (font_texture.font_texture_id == 0) {
        printf("Failed to load font texture!\n");
        return;
    }

    init_font_texture_coords(&font_texture);
    game->font_texture = font_texture; 

    game->is_running = true;
}

void destroy_game(Game* game)
{
    if (game->gl_context != NULL) {
        SDL_GL_DeleteContext(game->gl_context);
    }

    if (game->window != NULL) {
        SDL_DestroyWindow(game->window);
    }

    if (game->font_texture.font_texture_id != 0) {
        glDeleteTextures(1, &game->font_texture.font_texture_id);
    }

    SDL_Quit();
}

void handle_game_events(Game* game)
{
    SDL_Event event;
    int x;
    int y;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                game->is_running = false;
                break;
            case SDL_SCANCODE_W:
                set_left_pad_speed(&(game->pong), -LEFT_PAD_SPEED);
                break;
            case SDL_SCANCODE_S:
                set_left_pad_speed(&(game->pong), LEFT_PAD_SPEED);
                break;
            case SDL_SCANCODE_1:
                decrease_ball_radi(&(game->pong.ball));
                break;
            case SDL_SCANCODE_2:
                increase_ball_radi(&(game->pong.ball));
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
                set_left_pad_speed(&(game->pong), 0);
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&x, &y);
            move_ball(&(game->pong.ball), x, y);
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&x, &y);
            set_right_pad_position(&(game->pong), y);
            break;
        case SDL_QUIT:
            game->is_running = false;
            break;
        default:
            break;
        }
    }
}

void update_game(Game* game)
{
    double current_time;
    double elapsed_time;

    current_time = (double)SDL_GetTicks() / 1000;
    elapsed_time = current_time - game->last_update_time;
    game->last_update_time = current_time;

    update_pong(&(game->pong), elapsed_time);
}

void render_game(Game* game)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render_pong(&(game->pong), &(game->font_texture));
    SDL_GL_SwapWindow(game->window);
}

bool init_sdl(Game* game)
{
    int error_code;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return false;
    }

    game->window = SDL_CreateWindow(
        "Pong!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        game->width, game->height,
        SDL_WINDOW_OPENGL);
    if (game->window == NULL) {
        printf("[ERROR] Unable to create the application window!\n");
        return false;
    }

    game->gl_context = SDL_GL_CreateContext(game->window);
    if (game->gl_context == NULL) {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return false;
    }

    return true;
}

void init_opengl(Game* game)
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.1, 0.1, 0.1, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, game->width, game->height, 0, -200, 200);
    glViewport(0, 0, game->width, game->height);
}
