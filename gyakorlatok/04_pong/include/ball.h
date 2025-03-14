#ifndef BALL_H
#define BALL_H

#include <stdbool.h>

/**
 * Ball position and speed
 */
typedef struct Ball
{
    float x;
    float y;
    float radius;
    float speed_x;
    float speed_y;
    float rotation_speed;
} Ball;

/**
 * Move the ball to the given position.
 */
void init_ball(Ball* ball, float x, float y);

/**
 * Reset the ball's position
 * @param rev_dir for reversing the ball's direction.
 */
void reset_ball(Ball* ball, bool rev_dir);

/**
 * Move the ball to the given x, y coordinates
 */
void move_ball(Ball *ball, float x, float y);

/**
 * Check if the updated ball radius is valid
 */
bool is_in_range(float radi);

/**
 * Increase the ball radius by BALL_RADIUS_STEP
 */
void increase_ball_radi(Ball *ball);

/**
 * Decrease the ball radius by BALL_RADIUS_STEP
 */
void decrease_ball_radi(Ball *ball);

/**
 * Update the ball.
 * @param time elapsed time in seconds.
 */
void update_ball(Ball* ball, double time);

/**
 * Render the ball.
 */
void render_ball(Ball* ball);

#endif /* BALL_H */
