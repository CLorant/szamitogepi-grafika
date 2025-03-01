#ifndef DRAW_STATE_H
#define DRAW_STATE_H

// Rajzolási állapotok
typedef enum DrawState {
    STATE_IDLE,        // Semmi nem történik
    STATE_FIRST_CLICK, // Az első kattintás megtörtént
    STATE_SECOND_CLICK,// A második kattintás megtörtént
    STATE_DRAGGING     // Kör húzása folyamatban
} DrawState;

#endif