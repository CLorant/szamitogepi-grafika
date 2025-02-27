#ifndef LINE_H
#define LINE_H

#define MAX_LINE_COUNT 100

typedef struct Point {
    double x;
    double y;
} Point;

typedef struct Color {
    int red;
    int green;
    int blue;
} Color;

typedef struct Line {
    Point point1;
    Point point2;
    Color color;
} Line;

void get_click_coord(Point* point);

void set_color(Color* color, int red, int green, int blue);

void set_line(Line* line, Point p1, Point p2, Color color);

#endif