#include <GL/gl.h>
#include <stdio.h>
#include <string.h>
#include "pong.h"
#include "text.h"

const float BALL_ROTATION_SPEED_CHANGE = 50.0f;
const float BALL_SPEED_Y_FACTOR = 2.0f;
const float BALL_SPEED_Y_OFFSET = 0.5f;

void init_pong(Pong* pong, int width, int height)
{
    pong->width = width;
    pong->height = height;
    init_pad(&(pong->left_pad), 0, height, RED_THEME);
    init_pad(&(pong->right_pad), width - 50, height, GREEN_THEME);
    init_ball(&(pong->ball), width / 2, height / 2);
    pong->left_score = 0;
    pong->right_score = 0;
}

void update_pong(Pong* pong, double time)
{
    update_pad(&(pong->left_pad), time);
    update_pad(&(pong->right_pad), time);
    update_ball(&(pong->ball), time);
    bounce_ball(pong);
}

void render_pong(Pong* pong, Texture* texture)
{
    render_pad(&(pong->left_pad));
    render_pad(&(pong->right_pad));
    render_ball(&(pong->ball));

    char score_text[50];
    sprintf(score_text, "%d - %d", pong->left_score, pong->right_score);

    int text_length = strlen(score_text);
    float text_width = text_length * 32;

    float start_x = (pong->width - text_width) / 2;

    render_text(texture, start_x, 20, score_text);
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
        reset_ball(ball, true);
        return;
    }
    if (ball_misses_right) {
        pong->left_score++;
        reset_ball(ball, false);
        return;
    }

    if (ball_hits_left_pad) {
        ball->x = left_pad->x + left_pad->width + ball->radius;
        ball->speed_x *= -1;
        ball->rotation_speed += BALL_ROTATION_SPEED_CHANGE;

        hit_position = (ball->y - left_pad->y) / left_pad->height;
        ball->speed_y += (hit_position - BALL_SPEED_Y_OFFSET) * BALL_SPEED_Y_FACTOR;
    }
    if (ball_hits_right_pad) {
        ball->x = right_pad->x - ball->radius;
        ball->speed_x *= -1;
        ball->rotation_speed -= BALL_ROTATION_SPEED_CHANGE;

        hit_position = (ball->y - right_pad->y) / right_pad->height;
        ball->speed_y += (hit_position - BALL_SPEED_Y_OFFSET) * BALL_SPEED_Y_FACTOR;
    }

    if (ball_hits_top || ball_hits_bottom) {
        ball->speed_y *= -1;
        ball->y = ball_hits_top ? ball->radius : pong->height - ball->radius;
        ball->rotation_speed *= -1;
    }
}