## 3. gyakorlati feladat: Szakaszok rajzolása, Kör közelítése

A /src mappán belül:

### /structs:
```c
typedef struct Color {
    int r, g, b;
} Color;
```
/structs/color: RGB szín struktúra
```c
typedef struct Point {
    int x, y;
} Point;
```
/structs/point: 2D-s pont struktúra
```c
typedef struct Line {
    Point p1, p2;
    Color color;
} Line;
```
/structs/line_struct: 2D-s vonal struktúra
```c
typedef struct Rectangle {
    Point p1, p2;
    Color color;
} Rectangle;
```
/structs/rect_struct: 2D-s téglalap struktúra

### /shapes
```c
#define MAX_LINE_COUNT 10

void set_line(Line* line, Point p1, Point p2, Color color);

bool push_line(Line* stack, int* stack_pointer, Line line);

bool pop_line(Line* stack, int* stack_pointer, Line* line);
```
/shapes/line: Vonal beállítása és veremben való kezelése
```c
#define MAX_RECT_COUNT 10

void set_rect(Rectangle* rect, Point p1, Point p2, Color color);

bool push_rect(Rectangle* stack, int* stack_pointer, Rectangle rect);

bool pop_rect(Rectangle* stack, int* stack_pointer, Rectangle* rect);
```
/shapes/rectangle: Téglalap beállítása és veremben való kezelése

### /palette
```c
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

void draw_palette(SDL_Renderer* renderer);

bool is_cursor_over_palette(int mx, int my);

void get_color_from_palette(Color* color, int mx, int my);
```
/palette: A képernyőméret beállítása és a színválasztó funkcionalitását tartalmazza.
