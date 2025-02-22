## 2. gyakorlati feladat: Mátrixok, transzformációk

A matrix.h-ban definiált **N** konstanssal állítható a négyzetes mátrixok mérete, **N = 3** esetre íródott a program.
```c
#define N 3
```
A matrix.h-ban definiált **MAX_STACK** konstanssal állítható a verem mérete.
```c
#define MAX_STACK 10
```
Homogén koordinátás pont struktúra
```c
typedef struct HomPoint {
    float x;
    float y;
    float weight;
} HomPoint;
```
Pozitív egész szám bekérés
```c
void get_positive_int(const char *prompt, int *value);
```
Input buffer törlése
```c
void clear_input_buffer();
```
Felhasználótól mátrix bekérés
```c
void get_matrix(const char *prompt, int n, int matrix[n][n]);
```
Mátrix kiírása
```c
void print_matrix(int n, int matrix[n][n]);
```
Nulla mátrix inicializálása
```c
void init_zero_matrix(int n, int matrix[n][n]);
```
Egység mátrix inicializálása
```c
void init_identity_matrix(int n, int matrix[n][n]);
```
Mátrix skaláris szorzása
```c
void scalar_multiply(int n, int matrix[n][n], int scalar);
```
Két mátrix szorzása
```c
void multiply_matrices(int n, int res[n][n], int m1[n][n], int m2[n][n]);
```
Homogén koordinátás pont transzformációja, transzformációs mátrix-al
```c
void transform_point(HomPoint *point, int n, int t_matrix[n][n]);
```
Transzformációs mátrix skálázása, a multip paraméterrel
```c
void scale(int n, int matrix[n][n], int multip);
```
Transzformációs mátrix eltolása, az x, y paraméterekkel
```c
void shift(int n, int matrix[n][n], int x, int y);
```
Transzformációs mátrix forgatása, a rotate_deg paraméterrel
```c
void rotate(int n, int matrix[n][n], int rotate_deg);
```
Mátrix berakása a verembe
```c
void push_matrix(int n, int matrix[n][n]);
```
Mátrix kivétele a veremből
```c
void pop_matrix(int n, int matrix[n][n]);
```