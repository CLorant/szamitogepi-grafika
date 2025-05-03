#ifndef APP_H
#define APP_H

#include "camera.h"
#include "scene.h"
#include <ode/ode.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdbool.h>

/**
 * Show a text based manual.
 */
typedef struct Manual {
    bool enabled;
    float scroll;
    float line_height;
    float distance;
    float start_x;
    float start_y;
    char* text;
    GLuint charmap_id;
    Vec3 prev_cam_position;
    Vec3 prev_cam_rotation;
    bool prev_cam_orbital;
} Manual;

/**
 * Main application state container.
 */
typedef struct App {
    SDL_Window* window;
    int window_width;
    int window_height;
    SDL_GLContext gl_context;
    bool is_running;
    bool is_fullscreen;
    bool is_dragging;
    
    float drag_distance;
    float drag_offset_x;
    float drag_offset_y;
    double uptime;
    Manual manual;
    Camera camera;
    Scene scene;
} App;

/**
 * Initialize the application.
 */
void init_app(App* app, int width, int height);

/**
 * Initialize the OpenGL context.
 */
void init_opengl();

/**
 * Reshape the window.
 */
void reshape(App* app, GLsizei width, GLsizei height);

/**
 * Handle the events of the application.
 */
void handle_app_events(App* app);

/**
 * Update the application.
 */
void update_app(App* app);

/**
 * Render the application.
 */
void render_app(App* app);

/**
 * Destroy the application.
 */
void destroy_app(App* app);

/**
 * Updated the manual's parameter based on display's aspect ratio.
 */
void update_manual_display_params(App* app);

/**
 * Toggle between scene and manual view.
 */
void handle_manual(App* app);

/**
 * Apply left / up / right / down motion to an object based on camera basis.
 */
void handle_move_selected_object(App* app, int dx, int dy);

/**
 * Apply forward / backward motion to an object.
 */
void handle_mouse_wheel(App* app, int y_wheel);

/**
 * Draw a crosshair in the center of the screen.
 */
void draw_crosshair(const App* app);

/**
 * Render the manual.
 */
void draw_manual(const App* app);

#endif /* APP_H */
