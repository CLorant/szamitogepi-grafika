#ifndef DRAW_STATE_H
#define DRAW_STATE_H

typedef enum DrawState {
    STATE_IDLE,
    STATE_FIRST_CLICK,
    STATE_SECOND_CLICK,
    STATE_DRAGGING
} DrawState;

#endif