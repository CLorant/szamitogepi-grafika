#include "matrix.h"

#include <stdio.h>

int main(void) {
    printf("\nMatrices, matrix transformations\n\n");
    int matrix[N][N];

    printf("Identity matrix:\n");
    init_identity_matrix(N, matrix);
    print_matrix(N, matrix);

    int scalar_multi;
    get_positive_int("Scalar multiplier: ", &scalar_multi);
    printf("Scalar multiplied (x%d):\n", scalar_multi);
    scalar_multiply(N, matrix, scalar_multi);
    print_matrix(N, matrix);

    clear_input_buffer();
    printf("Matrix multiplication\n");
    int result[N][N];
    init_zero_matrix(N, result);

    const char* PROMPT_MATRIX = "\nEnter a matrix per row (format: 1 2 3)\n";

    int m1[N][N];
    get_matrix(PROMPT_MATRIX, N, m1);
    int m2[N][N];
    get_matrix(PROMPT_MATRIX, N, m2);

    multiply_matrices(N, result, m1, m2);
    printf("Result matrix:\n");
    print_matrix(N, result);

    printf("Point transformation\n");

    HomPoint point = {3.0, 1.0, 2.5};
    printf("Original point: (%f, %f, %f)\n", point.x, point.y, point.weight);
    int t_matrix[3][3] = {
        {1, 0, 2},
        {0, 1, 3},
        {0, 0, 1}
    };
    push_matrix(3, t_matrix);
    printf("\nTransformation matrix:\n");
    print_matrix(3, t_matrix);
    transform_point(&point, N, t_matrix);
    printf("Transformed point: (%f, %f, %f)\n", point.x, point.y, point.weight);

    printf("\nContinuing with the transformation matrix:\n");
    print_matrix(3, t_matrix);

    printf("\nMatrix rotation (origo centered)\n");
    int rotate_deg;
    get_positive_int("Degree: ", &rotate_deg);
    rotate(3, t_matrix, rotate_deg);
    printf("Rotated matrix (%d°):\n", rotate_deg);
    print_matrix(3, t_matrix);
    push_matrix(3, t_matrix);

    printf("Matrix scaling (origo centered)\n");
    int scale_multi;
    get_positive_int("Scale ratio: ", &scale_multi);
    scale(3, t_matrix, scale_multi);
    printf("Scaled matrix (%dx):\n", scale_multi);
    print_matrix(3, t_matrix);
    push_matrix(3, t_matrix);

    printf("\nMatrix shifting\n");
    int shift_x, shift_y;
    get_positive_int("Shift x: ", &shift_x);
    get_positive_int("Shift y: ", &shift_y);
    shift(3, t_matrix, shift_x, shift_y);
    printf("Shifted matrix (x=%d, y=%d):\n", shift_x, shift_y);
    print_matrix(3, t_matrix);
    push_matrix(3, t_matrix);

    pop_matrix(3, t_matrix);
    printf("Restored before shifting:\n");
    print_matrix(3, t_matrix);

    pop_matrix(3, t_matrix);
    printf("Restored before scaling:\n");
    print_matrix(3, t_matrix);

    pop_matrix(3, t_matrix);
    printf("Restored before rotating:\n");
    print_matrix(3, t_matrix);

    return 0;
}