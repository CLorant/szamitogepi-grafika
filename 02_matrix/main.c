#include "matrix.h"

#include <stdio.h>

#define PROMPT_MATRIX "\nAdja meg a mátrixot! (formátum: 1 2 3)\n"
#define PROMPT_SCALAR "Adja meg a skaláris szorzót: "
#define PROMPT_SCALE "Adja meg a nagyítási arányt: "
#define PROMPT_SHIFT_X "Adja meg az eltolási x koordinátát: "
#define PROMPT_SHIFT_Y "Adja meg az eltolási y koordinátát: "
#define PROMPT_ROTATE "Adja meg az elforgatási fokot: "

int main(void) {
    printf("\n2. gyakorlat - Mátrixok, transzformációk Program\n\n");
    int matrix[N][N]; // Ennek a mátrixnak az állapotai kerülnek a verembe

    printf("Egységmátrix:\n");
    init_identity_matrix(N, matrix);
    print_matrix(N, matrix);

    int scalar_multi;
    get_positive_int(PROMPT_SCALAR, &scalar_multi);
    printf("Skalárisan szorzott (x%d):\n", scalar_multi);
    scalar_multiply(N, matrix, scalar_multi);
    print_matrix(N, matrix);

    // Csak a mátrix szorzásnál alkalmaztam a felhasználó általi mátrix megadást
    clear_input_buffer(); // Az előző input miatt szükséges
    printf("Mátrix szorzás\n");
    int result[N][N];
    init_zero_matrix(N, result);
    int m1[N][N];
    get_matrix(PROMPT_MATRIX, N, m1);
    int m2[N][N];
    get_matrix(PROMPT_MATRIX, N, m2);

    multiply_matrices(N, result, m1, m2);
    printf("Eredmény mátrix:\n");
    print_matrix(N, result);

    printf("Pont transzformáció\n");

    HomPoint point = {3.0, 1.0, 2.5};
    printf("Eredeti pont: (%f, %f, %f)\n", point.x, point.y, point.weight);
    int t_matrix[3][3] = {
        {1, 0, 2},
        {0, 1, 3},
        {0, 0, 1}
    };
    push_matrix(3, t_matrix);
    printf("\nTranszformációs mátrix:\n");
    print_matrix(3, t_matrix);
    transform_point(&point, N, t_matrix);
    printf("Transzformált pont: (%f, %f, %f)\n", point.x, point.y, point.weight);

    printf("\nFolytatás a transzformációs mátrixal:\n");
    print_matrix(3, t_matrix);

    printf("\nMátrix forgatás (origó központú)\n");
    int rotate_deg;
    get_positive_int(PROMPT_ROTATE, &rotate_deg);
    rotate(3, t_matrix, rotate_deg);
    printf("Forgatott mátrix (%d fok):\n", rotate_deg);
    print_matrix(3, t_matrix);
    push_matrix(3, t_matrix);

    printf("Mátrix skálázás (origó központú)\n");
    int scale_multi;
    get_positive_int(PROMPT_SCALE, &scale_multi);
    scale(3, t_matrix, scale_multi);
    printf("Skalázott mátrix (%dx):\n", scale_multi);
    print_matrix(3, t_matrix);
    push_matrix(3, t_matrix);

    printf("\nMátrix eltolás\n");
    int shift_x, shift_y;
    get_positive_int(PROMPT_SHIFT_X, &shift_x);
    get_positive_int(PROMPT_SHIFT_Y, &shift_y);
    shift(3, t_matrix, shift_x, shift_y);
    printf("Eltolt mátrix (x=%d, y=%d):\n", shift_x, shift_y);
    print_matrix(3, t_matrix);
    push_matrix(3, t_matrix);

    pop_matrix(3, t_matrix);
    printf("Visszaállítva az eltolás előtti állapot:\n");
    print_matrix(3, t_matrix);

    pop_matrix(3, t_matrix);
    printf("Visszaállítva a skalázás előtti állapot:\n");
    print_matrix(3, t_matrix);

    pop_matrix(3, t_matrix);
    printf("Visszaállítva a forgatás előtti állapot:\n");
    print_matrix(3, t_matrix);

    return 0;
}