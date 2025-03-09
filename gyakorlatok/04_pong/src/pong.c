#include "pong.h"

void init_pong(Pong* pong, int width, int height)
{
    pong->width = width;
    pong->height = height;
    init_pad(&(pong->left_pad), 0, height, RED_THEME);
    init_pad(&(pong->right_pad), width - 50, height, GREEN_THEME);
    init_ball(&(pong->ball), width / 2, height / 2, false);
}

void update_pong(Pong* pong, double time)
{
    update_pad(&(pong->left_pad), time);
    update_pad(&(pong->right_pad), time);
    update_ball(&(pong->ball), time);
    bounce_ball(pong);
}

void render_pong(Pong* pong)
{
    render_pad(&(pong->left_pad));
    render_pad(&(pong->right_pad));
    render_ball(&(pong->ball));
}

void set_left_pad_position(Pong* pong, float position)
{
    set_pad_position(&(pong->left_pad), position);
}

void set_left_pad_speed(Pong* pong, float speed)
{
    set_pad_speed(&(pong->left_pad), speed);
}

void set_right_pad_position(Pong* pong, float position)
{
    set_pad_position(&(pong->right_pad), position);
}

void set_right_pad_speed(Pong* pong, float speed)
{
    set_pad_speed(&(pong->right_pad), speed);
}

void bounce_ball(Pong* pong) {
    Ball* ball = &pong->ball;
    Pad* left_pad = &pong->left_pad;
    Pad* right_pad = &pong->right_pad;
    float hit_position;

    bool ball_misses_left = ball->x - ball->radius < 0;
    bool ball_misses_right = ball->x + ball->radius > pong->width;

    bool ball_hits_top = ball->y - ball->radius < 0;
    bool ball_hits_bottom = ball->y + ball->radius > pong->height;

    bool ball_hits_left_pad = ball->x - ball->radius < left_pad->x + left_pad->width &&
                              ball->y > left_pad->y && ball->y < left_pad->y + left_pad->height;

    bool ball_hits_right_pad = ball->x + ball->radius > right_pad->x &&
                               ball->y > right_pad->y && ball->y < right_pad->y + right_pad->height;

    if (ball_misses_left) {
        pong->right_score++;
        init_ball(ball, pong->width / 2, pong->height / 2, true);
        return;
    }
    if (ball_misses_right) {
        pong->left_score++;
        init_ball(ball, pong->width / 2, pong->height / 2, false);
        return;
    }

    if (ball_hits_left_pad) {
        ball->x = left_pad->x + left_pad->width + ball->radius;
        ball->speed_x *= -1;

        hit_position = (ball->y - left_pad->y) / left_pad->height;
        ball->speed_y += (hit_position - 0.5f) * 2.0f;
    }
    if (ball_hits_right_pad) {
        ball->x = right_pad->x - ball->radius;
        ball->speed_x *= -1;

        hit_position = (ball->y - right_pad->y) / right_pad->height;
        ball->speed_y += (hit_position - 0.5f) * 2.0f;
    }

    if (ball_hits_top || ball_hits_bottom) {
        ball->speed_y *= -1;
        ball->y = ball_hits_top ? ball->radius : pong->height - ball->radius;
    }
}