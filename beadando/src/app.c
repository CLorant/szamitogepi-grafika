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
        "L.O.P.A.S.",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (app->window == NULL) {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    SDL_SetRelativeMouseMode(SDL_TRUE);

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
        printf("[WARNING] Could not enable VSync: %s\n", SDL_GetError());
    }

    app->manual.enabled = false;
    app->manual.charmap_id = load_texture("assets/textures/charmap.png");
    app->manual.text = read_manual("assets/manual.txt");
    app->manual.scroll = 0.0f;
    app->manual.line_height = 1.0f;
    update_manual_display_params(app);

    init_camera(&(app->camera));
    reshape(app, width, height);
    init_scene(&(app->scene));

    app->is_dragging = false;
    app->is_fullscreen = true;
    app->is_running = true;
    
    SDL_SetWindowFullscreen(app->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void init_opengl() {
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.0, 0.0, 0.0, 1.0);

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

    update_manual_display_params(app);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(app->camera.fov, app->camera.aspect_ratio, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void handle_app_events(App* app) {
    SDL_Event event;

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
                set_camera_vertical_speed(&(app->camera), 1);
                break;
            case SDL_SCANCODE_E:
                set_camera_vertical_speed(&(app->camera), -1);
                break;
            case SDL_SCANCODE_F1:
                handle_manual(app);
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
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN: {
            if (app->manual.enabled) continue;

            float hit_dist = 0.0f;
            int cx = app->window_width / 2;
            int cy = app->window_height / 2;
            int hit_id = select_object_at(
                &app->scene,
                &app->camera,
                cx, cy,
                &hit_dist
            );

            app->scene.selected_object_id = hit_id;
            if (hit_id >= 0) {
                app->drag_distance = hit_dist;
                app->drag_offset_x = app->drag_offset_y = 0.0f;
                app->is_dragging = true;
            }
            break;
        }
        case SDL_MOUSEMOTION: {
            if (app->manual.enabled) continue;

            int dx = -event.motion.xrel;
            int dy = -event.motion.yrel;

            Uint32 buttons = SDL_GetMouseState(NULL, NULL);
            if (buttons & SDL_BUTTON_LMASK) {
                rotate_camera(&app->camera, dx, dy);
                handle_move_selected_object(app, dx, dy);
            }
            else if (buttons & SDL_BUTTON_RMASK) {
                rotate_selected_object(&app->scene,
                    dy * 0.5f,
                    dx * 0.5f);
            }
            else {
                rotate_camera(&app->camera, dx, dy);
            }

        } break;
        case SDL_MOUSEBUTTONUP:
            app->is_dragging = false;
            app->scene.selected_object_id = -1;
            break;
        case SDL_MOUSEWHEEL: {
            if (app->manual.enabled) {
                app->manual.scroll += event.wheel.y * app->manual.line_height;
            }
            else if (app->camera.is_orbital) {
                app->camera.orbital_radius += event.wheel.y;

                if (app->camera.orbital_radius < 1.0) {
                    app->camera.orbital_radius = 1.0;
                }
                else if (app->camera.orbital_radius > 20.0) {
                    app->camera.orbital_radius = 20.0;
                }
            }
            else {
                handle_mouse_wheel(app, event.wheel.y);
            }

        } break;
        case SDL_QUIT:
            app->is_running = false;
            break;
        default:
            break;
        }
    }
}

void update_app(App* app) {
    double current_time;
    double elapsed_time;

    current_time = (double)SDL_GetTicks() / 1000;
    elapsed_time = current_time - app->uptime;
    app->uptime = current_time;

    update_camera(&(app->camera), elapsed_time);
    update_scene(&(app->scene), elapsed_time);

    if (app->is_dragging) {
        handle_move_selected_object(app, 0, 0);
    }
}

void render_app(App* app) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    set_view(&(app->camera));
    if (!app->manual.enabled) {
        render_scene(&(app->scene));
        draw_crosshair(app);
    }
    else {
       draw_manual(app);
    }

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

void update_manual_display_params(App* app) {
    float aspect_ratio = app->camera.aspect_ratio;
    float base_aspect = 16.0f / 9.0f;
    float aspect_scale = aspect_ratio / base_aspect;
    float height_scale = (float)app->window_height / 1080.0f;
    float narrow_screen_factor = powf(base_aspect / fmaxf(aspect_ratio, 0.5f), 0.95f);
    
    app->manual.distance = 20.0f * height_scale * narrow_screen_factor;
    app->manual.start_x = -17.0f * aspect_scale * narrow_screen_factor;
    app->manual.start_y = 20.0f * height_scale;
}

void handle_manual(App* app) {
    if (!app->manual.enabled) {
        app->manual.prev_cam_position = app->camera.position;
        app->manual.prev_cam_rotation = app->camera.rotation;
        app->manual.prev_cam_orbital = app->camera.is_orbital;

        app->camera.is_orbital = false;
        app->camera.rotation = (Vec3){ 0, 0, 0 };
        set_camera_speed(&app->camera, 0);
        set_camera_side_speed(&app->camera, 0);
        set_camera_vertical_speed(&app->camera, 0);
        update_camera_basis(&app->camera);
    }
    else {
        app->camera.position = app->manual.prev_cam_position;
        app->camera.rotation = app->manual.prev_cam_rotation;
        app->camera.is_orbital = app->manual.prev_cam_orbital;
        update_camera_basis(&app->camera);
    }

    app->manual.enabled = !app->manual.enabled;
}

void handle_move_selected_object(App* app, int dx, int dy) {
    const float DRAG_SPEED = 0.005f;

    if (!app->is_dragging || app->scene.selected_object_id < 0) 
        return;

    app->drag_offset_x += dx * DRAG_SPEED;
    app->drag_offset_y += dy * DRAG_SPEED;

    update_camera_basis(&app->camera);
    
    Vec3 base = vec3_add(
        app->camera.position,
        vec3_scale(app->camera.basis.forward, app->drag_distance)
    );

    Vec3 lateral = vec3_add(
        vec3_scale(app->camera.basis.right, app->drag_offset_x),
        vec3_scale(app->camera.basis.up,    app->drag_offset_y)
    );

    Vec3 target = vec3_add(base, lateral);

    move_selected_object(&app->scene, target);
}

void handle_mouse_wheel(App* app, int y_wheel) {
    const float SCROLL_SPEED = 0.5f;

    app->drag_distance += y_wheel * SCROLL_SPEED;
    app->drag_distance = fminf(fmaxf(app->drag_distance, 0.5f), 3.0f);

    if (app->is_dragging) {
        handle_move_selected_object(app, 0, 0);
    }
}

void draw_crosshair(const App* app) {
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

void draw_manual(const App* app) {
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