#include "app.h"
#include <stdio.h>
#include <math.h>

#include <SDL2/SDL_image.h>

void init_app(App* app, int width, int height) {
    int error_code;
    int inited_loaders;

    app->is_running = false;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    app->window = SDL_CreateWindow(
        "Models",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (app->window == NULL) {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    inited_loaders = IMG_Init(IMG_INIT_PNG);
    if (inited_loaders == 0) {
        printf("[ERROR] IMG initialization error: %s\n", IMG_GetError());
        return;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL) {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return;
    }
    
    init_opengl();
    printf("[INFO] OpenGL version: %s\n", glGetString(GL_VERSION));

    if (SDL_GL_SetSwapInterval(1) != 0) {
        fprintf(stderr, "[WARNING] Could not enable VSync: %s\n", SDL_GetError());
    }
    
    init_camera(&(app->camera));
    reshape(app, width, height);
    init_scene(&(app->scene));

    app->is_running = true;
    app->is_fullscreen = false;
}

void init_opengl() {
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.1, 0.1, 0.1, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1.0);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void reshape(App* app, GLsizei width, GLsizei height) {
    app->window_width = width;
    app->window_height = height;
    app->camera.viewport.width = width;
    app->camera.viewport.height = height;
    app->camera.aspect_ratio = (float)width / height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(app->camera.fov, app->camera.aspect_ratio, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void handle_app_events(App* app) {
    SDL_Event event;
    static bool is_mouse_down = false;
    static int mouse_x = 0;
    static int mouse_y = 0;
    int x, y;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                reshape(app, event.window.data1, event.window.data2);
                break;
            default:
                break;
            }
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                app->is_running = false;
                break;
            case SDL_SCANCODE_W:
                set_camera_speed(&(app->camera), 1);
                break;
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), -1);
                break;
            case SDL_SCANCODE_A:
                set_camera_side_speed(&(app->camera), 1);
                break;
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), -1);
                break;
            case SDL_SCANCODE_Q:
                if (app->camera.is_orbital) {
                    app->camera.orbital_radius -= 0.1;

                    if (app->camera.orbital_radius < 1.0) {
                        app->camera.orbital_radius = 1.0;
                    }
                }
                else {
                    set_camera_vertical_speed(&(app->camera), 1);
                }
                break;
            case SDL_SCANCODE_E:
                if (app->camera.is_orbital) {
                    app->camera.orbital_radius += 0.1;

                    if (app->camera.orbital_radius > 10.0) {
                        app->camera.orbital_radius = 10.0;
                    }
                }
                else {
                    set_camera_vertical_speed(&(app->camera), -1);
                }
                break;
            case SDL_SCANCODE_F2:
                adjust_fov(&app->camera, -5.0f);
                reshape(app, app->window_width, app->window_height);
                break;
            case SDL_SCANCODE_F3:
                adjust_fov(&app->camera, 5.0f);
                reshape(app, app->window_width, app->window_height);
                break;
            case SDL_SCANCODE_F4:
                app->camera.is_orbital = !app->camera.is_orbital;
                break;
            case SDL_SCANCODE_F5:
                adjust_brightness(&app->scene, 0.1f);
                break;
            case SDL_SCANCODE_F6:
                adjust_brightness(&app->scene, -0.1f);
                break;
            case SDL_SCANCODE_F11:
                app->is_fullscreen = !app->is_fullscreen;
                SDL_SetWindowFullscreen(
                    app->window,
                    app->is_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0
                );
                break;
            case SDL_SCANCODE_F12:
                app->scene.selected_object_id = -1;
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_Q:
            case SDL_SCANCODE_E:
                set_camera_vertical_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_J:
            case SDL_SCANCODE_L:
                set_camera_speed(&(app->camera), 0);
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            is_mouse_down = true;
            SDL_GetMouseState(&mouse_x, &mouse_y);
            app->scene.selected_object_id = select_object_at(&(app->scene), &(app->camera), mouse_x, mouse_y);
            break;
        case SDL_MOUSEMOTION: {
            SDL_GetMouseState(&x, &y);
            int dx = mouse_x - x;
            int dy = mouse_y - y;
        
            if (is_mouse_down) {
                handle_drag_motion(app, dx, dy);
            }
            else {
                rotate_camera(&app->camera, dx, dy);
            }
        
            mouse_x = x;
            mouse_y = y;
        } break;
        case SDL_MOUSEBUTTONUP:
            is_mouse_down = false;
            app->scene.selected_object_id = -1;
            break;
        case SDL_MOUSEWHEEL: {
            handle_mouse_wheel(app, event.wheel.y);
        } break;
        case SDL_QUIT:
            app->is_running = false;
            break;
        default:
            break;
        }
    }
}

/// TODO: The user should be able to rotate the camera while moving with the object.
void handle_drag_motion(App* app, int dx, int dy) {
    const float DRAG_SPEED = 0.005f;
    
    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK) {
        update_camera_basis(&app->camera);
            
        Vec3 delta = {
            .x = app->camera.basis.right.x * dx * DRAG_SPEED + app->camera.basis.up.x * dy * DRAG_SPEED,
            .y = app->camera.basis.right.y * dx * DRAG_SPEED + app->camera.basis.up.y * dy * DRAG_SPEED,
            .z = app->camera.basis.right.z * dx * DRAG_SPEED + app->camera.basis.up.z * dy * DRAG_SPEED,
        };
        move_selected_object(&app->scene, delta);
    }
    else if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_RMASK) {
        rotate_selected_object(&app->scene, dy * 0.5f, dx * 0.5f);
    }
}

void handle_mouse_wheel(App* app, int y_wheel) {
    const float SCROLL_SPEED = 0.1f;

    update_camera_basis(&app->camera);
    
    Vec3 delta = {
        .x = app->camera.basis.forward.x * y_wheel * SCROLL_SPEED,
        .y = app->camera.basis.forward.y * y_wheel * SCROLL_SPEED,
        .z = app->camera.basis.forward.z * y_wheel * SCROLL_SPEED,
    };
    move_selected_object(&app->scene, delta);
}

void update_app(App* app) {
    double current_time;
    double elapsed_time;

    current_time = (double)SDL_GetTicks() / 1000;
    elapsed_time = current_time - app->uptime;
    app->uptime = current_time;

    update_camera(&(app->camera), elapsed_time);
    update_scene(&(app->scene), elapsed_time);
}

void render_app(App* app) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    set_view(&(app->camera));
    render_scene(&(app->scene));
    glPopMatrix();

    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App* app) {
    if (app->gl_context != NULL) {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL) {
        SDL_DestroyWindow(app->window);
    }

    SDL_Quit();
}
