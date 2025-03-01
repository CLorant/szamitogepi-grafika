#ifndef CONSTS_H
#define CONSTS_H

#include <stdbool.h>
#include "structs/line_struct.h"
#include "structs/rect_struct.h"
#include "structs/circle_struct.h"
#include "structs/draw_state.h"

// Képernyő szélessége és magassága
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

// Maximális alakzatok száma a veremben
#define MAX_LINE_COUNT 20
#define MAX_RECT_COUNT 20
#define MAX_CIRCLE_COUNT 20

// Kijelölés jelző mérete a köröknél
#define CROSSHAIR_SIZE 5

// Körök közelítéséhez használt szakaszok száma
#define CIRCLE_SEGMENTS 20

#define WINDOW_TITLE                "3. gyakorlati feladat: Szakaszok rajzolása, Kör közelítése"

#define START_TITLE                 "Instrukciók"
#define START_DESC                  "1) Válasszon az (1) vonal, (2) téglalap, (3) kör rajzolási módok közül billenyűkkel.\n2) Kattintson a színpalettára színválasztáshoz.\n3) Kattintson az alakzat elkezdéséhez.\n4) Kattintson újra az alakzat befejezéséhez.\n5) A körök bal egér gomb lenyomva tartásával mozgathatóak."

#define ERR_CRITICAL_TITLE          "Kritikus hiba"
#define ERR_RUN_TITLE               "Futási hiba"
#define ERR_INIT_SDL_DESC           "SDL indítási hiba: %s" // Param: Hiba üzenet
#define ERR_CREATE_WINDOW_DESC      "Ablak létrehozási hiba: %s" // Param: Hiba üzenet
#define ERR_CREATE_RENDERER_DESC    "Renderer létrehozási hiba: %s" // Param: Hiba üzenet
#define ERR_STACK_OVERFLOW_DESC     "Stack overflow: Nem lehet több alakzatot menteni!"
#define ERR_STACK_UNDERFLOW_DESC    "Stack underflow: Nincs több mentett alakzat!"

#define MSG_START_SET               "\nKiinduló pont: (%3d, %3d)\nKattintson a(z) %s befejezéséhez.\n" // Params: x , y, alakzat neve
#define MSG_END_SET                 "\n%s beállítva!\n(%3d, %3d) -> (%3d, %3d)\n" // Params: Alakzat neve, x1, y1, x2, y2, 
#define MSG_UNDO                    "Alakzat visszatörölve\n"
#define MSG_REDO                    "Alakzat visszaállítva\n"    
#define MSG_SWITCH_SHAPE            "Váltás %s alakzatra\n" // Param: Alakzat neve
#define MSG_PICKED_COLOR            "Választott szín - R: %d, G: %d, B: %d\n" // Params: r, g, b

// Globális változók deklarációja
extern Line line_stack[MAX_LINE_COUNT];       // Vonalak vereme
extern int line_stack_pointer;                // Vonal verem mutató
extern Rectangle rect_stack[MAX_RECT_COUNT];  // Téglalapok vereme
extern int rect_stack_pointer;                // Téglalap verem mutató
extern Circle circle_stack[MAX_CIRCLE_COUNT]; // Körök vereme
extern int circle_stack_pointer;              // Kör verem mutató
extern bool circle_selected[MAX_CIRCLE_COUNT];// Kijelölt körök tömbje
extern bool dragging;                         // Éppen húzás történik-e
extern int dragged_circle_index;              // Éppen húzott kör indexe
extern DrawState draw_state;                  // Rajzolási állapot
extern Point p1, p2;                          // Két pont a rajzoláshoz

#endif
