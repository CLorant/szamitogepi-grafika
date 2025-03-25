#ifndef CONSTS_H
#define CONSTS_H

#include <stdbool.h>
#include "structs/line_struct.h"
#include "structs/rect_struct.h"
#include "structs/circle_struct.h"
#include "structs/draw_state.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

/**
 * Maximum number of shapes in the stack
 */
#define MAX_LINE_COUNT 20
#define MAX_RECT_COUNT 20
#define MAX_CIRCLE_COUNT 20

/**
 * The size of the crosshair when hovering over a circle
 */
#define CROSSHAIR_SIZE 5

/**
 * The number of lines used to approximate a circle
 */
#define CIRCLE_SEGMENTS 20

#define WINDOW_TITLE                "Line, rectangle and circle drawing"

#define START_TITLE                 "Instructions"
#define START_DESC                  "1) Choose between (1) line, (2) rectangle, (3) circle drawing modes.\n2) Click on the colour palette to choose a colour.\n3) Click to start the shape.\n4) Click again to finish the shape.\n5) You can move the circles by holding down the left mouse button."

#define ERR_CRITICAL_TITLE          "Critical error"
#define ERR_RUN_TITLE               "Runtime error"
#define ERR_INIT_SDL_DESC           "SDL initialization error: %s"
#define ERR_CREATE_WINDOW_DESC      "Window creation error: %s"
#define ERR_CREATE_RENDERER_DESC    "Renderer creation error: %s"
#define ERR_STACK_OVERFLOW_DESC     "Stack overflow: No more shapes can be saved!"
#define ERR_STACK_UNDERFLOW_DESC    "Stack underflow: No more saved shapes!"

#define MSG_START_SET               "\nStarting point: (%3d, %3d)\nClick to finish the %s.\n"
#define MSG_END_SET                 "\n%s set!\n(%3d, %3d) -> (%3d, %3d)\n" 
#define MSG_UNDO                    "Shape undo\n"
#define MSG_REDO                    "Shape redo\n"    
#define MSG_SWITCH_SHAPE            "Switching to %s drawing mode\n"
#define MSG_PICKED_COLOR            "Chosen color - R: %d, G: %d, B: %d\n"

extern Line line_stack[MAX_LINE_COUNT];
extern int line_stack_pointer;
extern Rectangle rect_stack[MAX_RECT_COUNT];
extern int rect_stack_pointer;
extern Circle circle_stack[MAX_CIRCLE_COUNT];
extern int circle_stack_pointer;
extern bool circle_selected[MAX_CIRCLE_COUNT];
extern bool dragging;
extern int dragged_circle_index;
extern DrawState draw_state;
extern Point p1, p2;

#endif
