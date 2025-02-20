#include "matrix.h"

#include <stdio.h>

#define N 3

int main(void) {
    int matrix[N][N];

    init_identity_matrix(N, matrix);
    print_matrix(N, matrix);

    scalar_multiply(N, matrix, 2);
    print_matrix(N, matrix);

    int result[N][N];
    init_zero_matrix(N, result);
    int example[3][3] = {
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 }
    };
    multiply_matrices(N, result, matrix, example);
    print_matrix(N, result);

    float point[3] = {3.0, 1.0, 2.5};
    printf("Eredeti pont: (%f, %f, %f)\n", point[0], point[1], point[2]);
    float trans[3][3] = {
        {1, 0, 2},
        {0, 1, 3},
        {0, 0, 1}
    };
    printf("Transzformacios matrix:\n");

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%f ", trans[i][j]);
        }
        printf("\n");
    }

    transform_point(point, N, trans);
    printf("Transzformalt pont: (%f, %f, %f)\n", point[0], point[1], point[2]);


    return 0;
}