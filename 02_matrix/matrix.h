#ifndef MATRIX_H
#define MATRIX_H

void print_matrix(int n, int matrix[n][n]);

void init_zero_matrix(int n, int matrix[n][n]);

void init_identity_matrix(int n, int matrix[n][n]);

void scalar_multiply(int n, int matrix[n][n], int scalar);

void multiply_matrices(int n, int res[n][n], int m1[n][n], int m2[n][n]);

void transform_point(float *point, int n, float matrix[n][n]);

void scale(int n, int matrix[n][n], int s);

void shift(int n, int matrix[n][n], int x, int y);

void rotate(int n, int matrix[n][n]);

void push_matrix(int n, int matrix[n][n]);

void pop_matrix(int n, int matrix[n][n]);

#endif