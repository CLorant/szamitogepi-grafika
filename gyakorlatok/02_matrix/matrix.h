#ifndef MATRIX_H
#define MATRIX_H

/**
 * @brief Négyzetes mátrix mérete, N = 3 eseten optimáls
 */
#define N 3

/**
 * @brief A verem mérete, optimálisan egy kellően nagy szám
 */
#define MAX_STACK 10

/**
 * @brief Homogén koordinátás pont struktúra
 */
typedef struct HomPoint {
    float x;
    float y;
    float weight;
} HomPoint;

/**
 * @brief Pozitív egész szám bekérés
 */
void get_positive_int(const char *prompt, int *value);

/**
 * @brief Input buffer törlése
 */
void clear_input_buffer();

/**
 * @brief Felhasználótól mátrix bekérés
 */
void get_matrix(const char *prompt, int n, int matrix[n][n]);

/**
 * @brief Mátrix kiírása
 */
void print_matrix(int n, int matrix[n][n]);

/**
 * @brief Nulla mátrix inicializálása
 */
void init_zero_matrix(int n, int matrix[n][n]);

/**
 * @brief Egység mátrix inicializálása
 */
void init_identity_matrix(int n, int matrix[n][n]);

/**
 * @brief Mátrix skaláris szorzása
 */
void scalar_multiply(int n, int matrix[n][n], int scalar);

/**
 * @brief Két mátrix szorzása
 */
void multiply_matrices(int n, int res[n][n], int m1[n][n], int m2[n][n]);

/**
 * @brief Homogén koordinátás pont transzformációja, transzformációs mátrix-al
 */
void transform_point(HomPoint *point, int n, int t_matrix[n][n]);

/**
 * @brief Transzformációs mátrix skálázása, a multip paraméterrel
 */
void scale(int n, int matrix[n][n], int multip);

/**
 * @brief Transzformációs mátrix eltolása, az x, y paraméterekkel
 */
void shift(int n, int matrix[n][n], int x, int y);

/**
 * @brief Transzformációs mátrix forgatása, a rotate_deg paraméterrel
 */
void rotate(int n, int matrix[n][n], int rotate_deg);

/**
 * @brief Mátrix berakása a verembe
 */
void push_matrix(int n, int matrix[n][n]);

/**
 * @brief Mátrix kivétele a veremből
 */
void pop_matrix(int n, int matrix[n][n]);

#endif