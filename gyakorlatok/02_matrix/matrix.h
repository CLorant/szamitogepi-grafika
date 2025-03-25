#ifndef MATRIX_H
#define MATRIX_H

/**
 * The size for uniform matrices in the program
 * Usually best kept as 3
 */
#define N 3

/**
 * The limit for how many operations can be saved in the stack
 */
#define MAX_STACK 10

/**
 * Homogen coordinate point struct
 */
typedef struct HomPoint {
    float x;
    float y;
    float weight;
} HomPoint;

/**
 * Gets a positive int value from user input
 */
void get_positive_int(const char *prompt, int *value);

/**
 * Clears the input buffer, which is important for
 * the line by line matrix console input
 */
void clear_input_buffer();

/**
 * Get a matrix line by line from user input
 */
void get_matrix(const char *prompt, int n, int matrix[n][n]);

/**
 * Print matrix
 */
void print_matrix(int n, int matrix[n][n]);

/**
 * Initalizes a matrix containing zeroes
 */
void init_zero_matrix(int n, int matrix[n][n]);

/**
 * Initalizes an identity matrix
 */
void init_identity_matrix(int n, int matrix[n][n]);

/**
 * Scalar multiplies a matrix
 */
void scalar_multiply(int n, int matrix[n][n], int scalar);

/**
 * Multiplies two matrices
 */
void multiply_matrices(int n, int res[n][n], int m1[n][n], int m2[n][n]);

/**
 * Transformations a homogen coordinate point, using a transformation matrix
 */
void transform_point(HomPoint *point, int n, int t_matrix[n][n]);

/**
 * Scales a transformation matrix by multip
 */
void scale(int n, int matrix[n][n], int multip);

/**
 * Shifts a transformation matrix by x, y
 */
void shift(int n, int matrix[n][n], int x, int y);

/**
 * Rotates a transformation matrix by rotate_deg
 */
void rotate(int n, int matrix[n][n], int rotate_deg);

/**
 * Pushes a matrix into the stack
 */
void push_matrix(int n, int matrix[n][n]);

/**
 * Removes a matrix from the stack
 */
void pop_matrix(int n, int matrix[n][n]);

#endif