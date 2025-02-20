#include "matrix.h"

#include <stdio.h>

// Helper
void print_matrix(int n, int matrix[n][n]) {
    printf("\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Helper
void init_zero_matrix(int n, int matrix[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = 0;
        }
    }
}

void init_identity_matrix(int n, int matrix[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (i == j) ? 1 : 0;
        }
    }
}

void scalar_multiply(int n, int matrix[n][n], int scalar) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] *= scalar;
        }
    }
}

void multiply_matrices(int n, int res[n][n], int m1[n][n], int m2[n][n]) {
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                res[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    print_matrix(n, m1);
    print_matrix(n, m2);
}

void transform_point(float *point, int n, float trans[n][n]) {
    float x = point[0];
    float y = point[1];
    float w = point[2];

    point[0] = trans[0][0] * x + trans[0][1] * y + trans[0][2] * w;
    point[1] = trans[1][0] * x + trans[1][1] * y + trans[1][2] * w;
}

void scale(int n, int matrix[n][n], int s) {
    
}

void shift(int n, int matrix[n][n], int x, int y) {
    
}

void rotate(int n, int matrix[n][n]) {
    
}

void push_matrix(int n, int matrix[n][n]) {
    
}

void pop_matrix(int n, int matrix[n][n]) {
    
}