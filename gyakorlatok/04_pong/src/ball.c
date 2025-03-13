#include "ball.h"
#include <stdio.h>
#include <GL/gl.h>

#include <math.h>

const float BALL_INIT_RADIUS = 50;
const float BALL_MIN_RADIUS = 10;
const float BALL_MAX_RADIUS = 100;
const float BALL_INIT_SPEED = 200;
const float BALL_RESIZE_STEP = 10;

float BALL_INIT_X = 0.0;
float BALL_INIT_Y = 0.0;

void init_ball(Ball *ball, float x, float y)
{
    BALL_INIT_X = x;
    BALL_INIT_Y = y;

    ball->x = BALL_INIT_X;
    ball->y = BALL_INIT_Y;
    ball->radius = BALL_INIT_RADIUS;
    ball->speed_x = BALL_INIT_SPEED;
    ball->speed_y = BALL_INIT_SPEED;
}

void reset_ball(Ball *ball, bool rev_dir)
{
    float dir_speed = rev_dir ? BALL_INIT_SPEED : -BALL_INIT_SPEED;
    ball->x = BALL_INIT_X;
    ball->y = BALL_INIT_Y;
    ball->speed_x = dir_speed;
    ball->speed_y = dir_speed;
}

void move_ball(Ball *ball, float x, float y)
{
    ball->x = x;
    ball->y = y;
}

bool is_in_range(float radi)
{
    return BALL_MIN_RADIUS <= radi && radi <= BALL_MAX_RADIUS;
}

void increase_ball_radi(Ball *ball)
{
    float radi = ball->radius + BALL_RESIZE_STEP;
    if (is_in_range(radi))
    {
        ball->radius = radi;
    }
}

void decrease_ball_radi(Ball *ball)
{
    float radi = ball->radius - BALL_RESIZE_STEP;
    if (is_in_range(radi))
    {
        ball->radius = radi;
    }
}

void update_ball(Ball *ball, double time)
{
    ball->x += ball->speed_x * time;
    ball->y += ball->speed_y * time;
}

void render_ball(Ball *ball)
{
    double angle;
    double x;
    double y;

    glPushMatrix();
    glTranslatef(ball->x, ball->y, 0.0);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0, 0.9, 0.8);
    glVertex2f(0, 0);
    angle = 0;
    while (angle < 2.0 * M_PI + 1)
    {
        x = cos(angle) * ball->radius;
        y = sin(angle) * ball->radius;
        glVertex2f(x, y);
        angle += 0.8;
    }
    glEnd();
    glPopMatrix();
}